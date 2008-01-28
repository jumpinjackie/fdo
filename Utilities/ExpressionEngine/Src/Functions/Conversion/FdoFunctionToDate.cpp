//
// 
// Copyright (C) 2004-2007  Autodesk, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
// 
//

#include <stdafx.h>
#include <Functions/Conversion/FdoFunctionToDate.h>
#include <FdoCommonStringUtil.h>


// ----------------------------------------------------------------------------
// --                         Constructors/Destructors                       --
// ----------------------------------------------------------------------------

FdoFunctionToDate::FdoFunctionToDate ()

// +---------------------------------------------------------------------------
// | The class constructor.
// +---------------------------------------------------------------------------

{

    // Initialize all class variables.

    is_pm_hour          = false;
    is_hour_set         = false;
    is_hh12_format      = false;

    function_definition = NULL;

    format_token_count  = 0;

    dt_object.year      = -1;
    dt_object.month     = -1;
    dt_object.day       = -1;
    dt_object.hour      = -1;
    dt_object.minute    = -1;
    dt_object.seconds   = 0;

    first = true;
    tmp_buffer = NULL;


}  //  FdoFunctionToDate ()

FdoFunctionToDate::~FdoFunctionToDate ()

// +---------------------------------------------------------------------------
// | The class destructor.
// +---------------------------------------------------------------------------

{

    // Delete the function definition.

    FDO_SAFE_RELEASE(function_definition);

    delete [] tmp_buffer;

}  //  ~FdoFunctionToDate ()


// ----------------------------------------------------------------------------
// --                            Public Class APIs                           --
// ----------------------------------------------------------------------------

FdoFunctionToDate *FdoFunctionToDate::Create ()

// +---------------------------------------------------------------------------
// | The function creates a new instance of the class.
// +---------------------------------------------------------------------------

{

    return new FdoFunctionToDate();

}  //  Create ()

FdoFunctionToDate *FdoFunctionToDate::CreateObject ()

// +---------------------------------------------------------------------------
// | The function creates a new instance of the class.
// +---------------------------------------------------------------------------

{

    return new FdoFunctionToDate();

}  //  CreateObject ()

FdoFunctionDefinition *FdoFunctionToDate::GetFunctionDefinition ()

// +---------------------------------------------------------------------------
// | The function creates the supported signature list for the function TODATE.
// +---------------------------------------------------------------------------

{

    if (function_definition == NULL)
        CreateFunctionDefinition();

    return FDO_SAFE_ADDREF(function_definition);

}  //  GetFunctionDefinition ()

FdoLiteralValue *FdoFunctionToDate::Evaluate (
                                    FdoLiteralValueCollection *literal_values)

// +---------------------------------------------------------------------------
// | The function processes a call to the function TODATE.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoDateTime            dt;

    FdoString              *dt_string,
                           *format_string;

    FdoPtr<FdoStringValue> string_value;


    if (first)
    {
        Validate(literal_values);
        return_datetime_value = FdoDateTimeValue::Create();
        tmp_buffer      = new wchar_t[INIT_ALLOCATE_SIZE+1];
        tmp_buffer_size = INIT_ALLOCATE_SIZE;
        first = false;
    }

    // Get the first parameter representing a string with some sort of date/
    // time information. If the value is NULL, terminate the routine.

    string_value = (FdoStringValue *) literal_values->GetItem(0);
    if (string_value->IsNull())
    {
        return_datetime_value->SetNull();
        return FDO_SAFE_ADDREF(return_datetime_value.p);
    }

    dt_string = string_value->GetString();

    // Check whether or not there is some format information passed along. If
    // this is the case, get the information. If the information is NULL, it
    // is considered invalid input data and an exception is issued. If no
    // format instruction is passed along, use the default one.

    if (literal_values->GetCount() == 2) {

        string_value = (FdoStringValue *) literal_values->GetItem(1);
        if (string_value->IsNull())
            throw FdoException::Create(
                    FdoException::NLSGetMessage(
                    FUNCTION_DATA_VALUE_ERROR, 
                    "Expression Engine: Invalid value for execution of function '%1$ls'",
                    FDO_FUNCTION_TODATE));
        else {

          format_string = string_value->GetString();
          ValidateFormatSpecification(format_string);

        }  //  else ...

    }  //  if (literal_values->GetCount() == 2) ...
    else {

      format_tokens[0] = ToDateFormatSpecifierTokens_Day_Number;
      format_tokens[1] = ToDateFormatSpecifierTokens_Month_AbbName_All_Upper;
      format_tokens[2] = ToDateFormatSpecifierTokens_Year4;
      format_tokens[3] = ToDateFormatSpecifierTokens_Hour24;
      format_tokens[4] = ToDateFormatSpecifierTokens_Minute;
      format_tokens[5] = ToDateFormatSpecifierTokens_Second;

      format_token_count = 6;

    }  //  else ...

    // Process the request. If the structure of the string holding the date/
    // time information does not match the expected format, issue an excep-
    //tion.
    // NOTE: If the format string differs in the separators from the date/time
    //       string, it will not be considered an error. Such differences will
    //       be handled.

    return Process(dt_string);

}  //  Evaluate ()


// ----------------------------------------------------------------------------
// --                          Supporting functions                          --
// ----------------------------------------------------------------------------

void FdoFunctionToDate::CreateFunctionDefinition ()

// +---------------------------------------------------------------------------
// | The procedure creates the function definition for the function TODATE. The
// | function definition includes the list of supported signatures. The follow-
// | ing signatures are supported:
// |
// |    TODATE (string [, string])
// |
// | The function always returns a DATETIME.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoString                               *desc               = NULL;

    FdoStringP                              arg1_description;
    FdoStringP                              arg2_description;
    FdoStringP                              str_arg_literal;
    FdoStringP                              form_arg_literal;

    FdoPtr<FdoArgumentDefinition>           form_arg;
    FdoPtr<FdoArgumentDefinition>           str_arg;

    FdoPtr<FdoArgumentDefinitionCollection> str_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_form_args;

    FdoPtr<FdoSignatureDefinition>          signature;
    FdoPtr<FdoSignatureDefinitionCollection> signatures;

    // Get the general descriptions for the arguments.

    arg1_description = FdoException::NLSGetMessage(
                                                FUNCTION_GENERAL_ARG,
                                                "Argument to be processed");

    arg2_description = FdoException::NLSGetMessage(
                                                FUNCTION_GENERAL_FORMAT_ARG,
                                                "Format specification");

    form_arg_literal = FdoException::NLSGetMessage(FUNCTION_FORMAT_ARG_LIT,
                                                   "optional format");

    str_arg_literal  = FdoException::NLSGetMessage(
                                    FUNCTION_STRING_ARG_LIT, "text property");

    // The following defines the different argument definition collections.

    str_arg  = FdoArgumentDefinition::Create(
                    str_arg_literal, arg1_description, FdoDataType_String);

    form_arg = FdoArgumentDefinition::Create(
                    form_arg_literal, arg2_description, FdoDataType_String);

    str_args = FdoArgumentDefinitionCollection::Create();
    str_args->Add(str_arg);

    str_form_args = FdoArgumentDefinitionCollection::Create();
    str_form_args->Add(str_arg);
    str_form_args->Add(form_arg);

    // Create the signature collection.

    signatures = FdoSignatureDefinitionCollection::Create();

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_DateTime, str_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_DateTime, str_form_args);
    signatures->Add(signature);

    // Create the function definition.

    desc = FdoException::NLSGetMessage(FUNCTION_TODATE,
                                       "Converts a string to a date");
    function_definition =
                FdoFunctionDefinition::Create(
                                        FDO_FUNCTION_TODATE,
                                        desc,
                                        false,
                                        signatures,
                                        FdoFunctionCategoryType_Conversion);

}  //  CreateFunctionDefinition ()

FdoFunctionToDate::ToDateFormatSpecifierTokens
                            FdoFunctionToDate::GetToken (
                                                        FdoStringP specifier)

// +---------------------------------------------------------------------------
// | The function returns the token for the provided date format specifier.
// +---------------------------------------------------------------------------

{

    // Compare the string against expected and valid format specifiers. If the
    // provided string represents one of them return the corresponding token.

    if (specifier == L"YY")
        return ToDateFormatSpecifierTokens_Year2;

    if (specifier == L"YYYY")
        return ToDateFormatSpecifierTokens_Year4;

    if (specifier == L"MONTH")
        return ToDateFormatSpecifierTokens_Month_FullName_All_Upper;

    if (specifier == L"Month")
        return ToDateFormatSpecifierTokens_Month_FullName_First_Upper;

    if (specifier == L"month")
        return ToDateFormatSpecifierTokens_Month_FullName_All_Lower;

    if (specifier == L"MON")
        return ToDateFormatSpecifierTokens_Month_AbbName_All_Upper;

    if (specifier == L"mon")
        return ToDateFormatSpecifierTokens_Month_AbbName_All_Lower;

    if (specifier == L"MM")
        return ToDateFormatSpecifierTokens_Month_Number;

    if (specifier == L"DAY")
        return ToDateFormatSpecifierTokens_Day_FullName_All_Upper;

    if (specifier == L"Day")
        return ToDateFormatSpecifierTokens_Day_FullName_First_Upper;

    if (specifier == L"day")
        return ToDateFormatSpecifierTokens_Day_FullName_All_Lower;

    if (specifier == L"DY")
        return ToDateFormatSpecifierTokens_Day_AbbName_All_Upper;

    if (specifier == L"dy")
        return ToDateFormatSpecifierTokens_Day_AbbName_All_Lower;

    if (specifier == L"DD")
        return ToDateFormatSpecifierTokens_Day_Number;

    if ((specifier == L"hh24") ||
        (specifier == L"hh"  )    )
        return ToDateFormatSpecifierTokens_Hour24;

    if (specifier == L"hh12")
        return ToDateFormatSpecifierTokens_Hour12;

    if (specifier == L"mm")
        return ToDateFormatSpecifierTokens_Minute;

    if (specifier == L"ss")
        return ToDateFormatSpecifierTokens_Second;

    if ((specifier == L"am") || 
        (specifier == L"AM")    )
        return ToDateFormatSpecifierTokens_am;

    if ((specifier == L"pm") || 
        (specifier == L"PM")    )
        return ToDateFormatSpecifierTokens_pm;

    // The given format specifier is unknown. Issue an exception.

    throw FdoException::Create(
            FdoException::NLSGetMessage(
            FUNCTION_DATA_VALUE_ERROR, 
            "Expression Engine: Invalid value for execution of function '%1$ls'",
            FDO_FUNCTION_TODATE));

}  //  GetToken ()

bool FdoFunctionToDate::IsValidDayAbb (FdoInt8    index,
                                       FdoStringP day_abb)

// +---------------------------------------------------------------------------
// | The function checks whether or not the given day identifies a known day
// | abbreviation and returns TRUE if this is the case, FALSE otherwise.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoStringP d_abb;

    // Get the name that matches the provided index and compare it to the 
    // given day abbreviation. If they match, return TRUE, FALSE otherwise.

    switch (index) {

      case 1:
        d_abb = FdoException::NLSGetMessage(DAY_ABB_SUNDAY, "Sun");
        break;

      case 2:
        d_abb = FdoException::NLSGetMessage(DAY_ABB_MONDAY, "Mon");
        break;

      case 3:
        d_abb = FdoException::NLSGetMessage(DAY_ABB_TUESDAY, "Tue");
        break;

      case 4:
        d_abb = FdoException::NLSGetMessage(DAY_ABB_WEDNESDAY, "Wed");
        break;

      case 5:
        d_abb = FdoException::NLSGetMessage(DAY_ABB_THURSDAY, "Thu");
        break;

      case 6:
        d_abb = FdoException::NLSGetMessage(DAY_ABB_FRIDAY, "Fri");
        break;

      case 7:
        d_abb = FdoException::NLSGetMessage(DAY_ABB_SATURDAY, "Sat");
        break;

      default:
        return false;
        break;

    }  //  switch ...

    // On Windows, the name received from the NLS catalog has a trailing blank.
    // For the comparison, this needs to be removed.

#ifdef _WIN32
    d_abb = d_abb.Mid(0, (d_abb.GetLength() - 1));
#endif

    return (FdoCommonStringUtil::StringCompareNoCase(day_abb, d_abb) == 0);

}  //  IsValidDayAbb ()

bool FdoFunctionToDate::IsValidDayName (FdoInt8    index,
                                        FdoStringP day_name)

// +---------------------------------------------------------------------------
// | The function checks whether or not the given day identifies a known day
// | name and returns TRUE if this is the case, FALSE otherwise.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoStringP d_name;

    // Get the name that matches the provided index and compare it to the 
    // given day name. If there is a match, return TRUE, FALSE otherwise.

    switch (index) {

      case 1:
        d_name = FdoException::NLSGetMessage(DAY_NAME_SUNDAY, "Sunday");
        break;

      case 2:
        d_name = FdoException::NLSGetMessage(DAY_NAME_MONDAY, "Monday");
        break;

      case 3:
        d_name = FdoException::NLSGetMessage(DAY_NAME_TUESDAY, "Tuesday");
        break;

      case 4:
        d_name = FdoException::NLSGetMessage(DAY_NAME_WEDNESDAY, "Wednesday");
        break;

      case 5:
        d_name = FdoException::NLSGetMessage(DAY_NAME_THURSDAY, "Thursday");
        break;

      case 6:
        d_name = FdoException::NLSGetMessage(DAY_NAME_FRIDAY, "Friday");
        break;

      case 7:
        d_name = FdoException::NLSGetMessage(DAY_NAME_SATURDAY, "Saturday");
        break;

      default:
        return false;
        break;

    }  //  switch ...

    // On Windows, the name received from the NLS catalog has a trailing blank.
    // For the comparison, this needs to be removed.

#ifdef _WIN32
    d_name = d_name.Mid(0, (d_name.GetLength() - 1));
#endif

    return (FdoCommonStringUtil::StringCompareNoCase(day_name, d_name) == 0);

}  //  IsValidDayName ()

bool FdoFunctionToDate::IsValidMonthAbb (FdoInt8    index,
                                         FdoStringP month_abb)

// +---------------------------------------------------------------------------
// | The function checks whether or not the given month identifies a known
// | month abbreviation and returns TRUE if this is the case, FALSE otherwise.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoStringP m_abb;

    // Get the name that matches the provided index and compare it to the 
    // given month abbreviation. If there is a match, return TRUE, FALSE
    // otherwise.

    switch (index) {

      case 1:
        m_abb = FdoException::NLSGetMessage(MONTH_ABB_JANUARY, "Jan");
        break;

      case 2:
        m_abb = FdoException::NLSGetMessage(MONTH_ABB_FEBRUARY, "Feb");
        break;

      case 3:
        m_abb = FdoException::NLSGetMessage(MONTH_ABB_MARCH, "Mar");
        break;

      case 4:
        m_abb = FdoException::NLSGetMessage(MONTH_ABB_APRIL, "Apr");
        break;

      case 5:
        m_abb = FdoException::NLSGetMessage(MONTH_ABB_MAY, "May");
        break;

      case 6:
        m_abb = FdoException::NLSGetMessage(MONTH_ABB_JUNE, "Jun");
        break;

      case 7:
        m_abb = FdoException::NLSGetMessage(MONTH_ABB_JULY, "Jul");
        break;

      case 8:
        m_abb = FdoException::NLSGetMessage(MONTH_ABB_AUGUST, "Aug");
        break;

      case 9:
        m_abb = FdoException::NLSGetMessage(MONTH_ABB_SEPTEMBER, "Sep");
        break;

      case 10:
        m_abb = FdoException::NLSGetMessage(MONTH_ABB_OCTOBER, "Oct");
        break;

      case 11:
        m_abb = FdoException::NLSGetMessage(MONTH_ABB_NOVEMBER, "Nov");
        break;

      case 12:
        m_abb = FdoException::NLSGetMessage(MONTH_ABB_DECEMBER, "Dec");
        break;

      default:
        return false;
        break;

    }  //  switch ...

    // On Windows, the name received from the NLS catalog has a trailing blank.
    // For the comparison, this needs to be removed.

#ifdef _WIN32
    m_abb = m_abb.Mid(0, (m_abb.GetLength() - 1));
#endif

    return (FdoCommonStringUtil::StringCompareNoCase(month_abb, m_abb) == 0);

}  //  IsValidMonthAbb ()

bool FdoFunctionToDate::IsValidMonthName (FdoInt8    index,
                                          FdoStringP month_name)

// +---------------------------------------------------------------------------
// | The function checks whether or not the given month identifies a known
// | month name and returns TRUE if this is the case, FALSE otherwise.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoStringP m_name;

    // Get the name that matches the provided index and compare it to the 
    // given month name. If there is a match, return TRUE, FALSE otherwise.

    switch (index) {

      case 1:
        m_name = FdoException::NLSGetMessage(MONTH_NAME_JANUARY, "January");
        break;

      case 2:
        m_name = FdoException::NLSGetMessage(MONTH_NAME_FEBRUARY, "February");
        break;

      case 3:
        m_name = FdoException::NLSGetMessage(MONTH_NAME_MARCH, "March");
        break;

      case 4:
        m_name = FdoException::NLSGetMessage(MONTH_NAME_APRIL, "April");
        break;

      case 5:
        m_name = FdoException::NLSGetMessage(MONTH_NAME_MAY, "May");
        break;

      case 6:
        m_name = FdoException::NLSGetMessage(MONTH_NAME_JUNE, "June");
        break;

      case 7:
        m_name = FdoException::NLSGetMessage(MONTH_NAME_JULY, "July");
        break;

      case 8:
        m_name = FdoException::NLSGetMessage(MONTH_NAME_AUGUST, "August");
        break;

      case 9:
        m_name = FdoException::NLSGetMessage(MONTH_NAME_SEPTEMBER, "September");
        break;

      case 10:
        m_name = FdoException::NLSGetMessage(MONTH_NAME_OCTOBER, "October");
        break;

      case 11:
        m_name = FdoException::NLSGetMessage(MONTH_NAME_NOVEMBER, "November");
        break;

      case 12:
        m_name = FdoException::NLSGetMessage(MONTH_NAME_DECEMBER, "December");
        break;

      default:
        return false;
        break;

    }  //  switch ...

    // On Windows, the name received from the NLS catalog has a trailing blank.
    // For the comparison, this needs to be removed.

#ifdef _WIN32
    m_name = m_name.Mid(0, (m_name.GetLength() - 1));
#endif

    return (FdoCommonStringUtil::StringCompareNoCase(month_name, m_name) == 0);

}  //  IsValidMonthName ()

FdoLiteralValue *FdoFunctionToDate::Process (FdoString *dt_string)

// +---------------------------------------------------------------------------
// | The function processes the request. It issues an exception if the struc-
// | ture of the date/time string does not match the structure defined in the
// | format specification.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    bool          start_pos_set          = false;

    size_t        pos                    = 0,
                  start_pos              = 0,
                  dt_string_length       = 0;

    FdoInt16      curr_token_pos         = 0;


    // The following moves through the date/time string and collects infor-
    // mation between the separators. Once the information is retrieved, the
    // corresponding format information is used to identify what the infor-
    // mation represents. If an error is detected during this mapping, an
    // exception is issued.

    dt_string_length = wcslen(dt_string);

    if (dt_string_length == 0)
        throw FdoException::Create(
                FdoException::NLSGetMessage(
                FUNCTION_DATA_VALUE_ERROR, 
                "Expression Engine: Invalid value for execution of function '%1$ls'",
                FDO_FUNCTION_TODATE));

    for (pos = 0; pos < dt_string_length; pos++) {

      if (!isalnum(dt_string[pos])) {

          if (start_pos_set) {
                
              if (pos - start_pos>tmp_buffer_size)
              {
                delete [] tmp_buffer;
                tmp_buffer_size = pos - start_pos;
                tmp_buffer      = new wchar_t[tmp_buffer_size + 1];  
              }
              wcsncpy(tmp_buffer, dt_string+start_pos, pos - start_pos);
              tmp_buffer[pos - start_pos] = '\0';

              if (curr_token_pos >= format_token_count)
                 throw FdoException::Create(
                         FdoException::NLSGetMessage(
                         FUNCTION_DATA_VALUE_ERROR, 
                         "Expression Engine: Invalid value for execution of function '%1$ls'",
                         FDO_FUNCTION_TODATE));

              ProcessDateTimeValue(tmp_buffer, curr_token_pos);

              curr_token_pos++;
              start_pos_set = false;

          }  //  if (start_pos_set) ...

      }  //  if (!isalnum( ...
      else {

        if (!start_pos_set) {

            start_pos     = pos;
            start_pos_set = true;

        }  //  if (!start_pos_set) ...

      }  //  else

    }  //  for (pos = 0; pos < format_length; pos++) ...

    // If the start position is set, there is still a format specifier to
    // analyze.

    if (start_pos_set) {

        if (pos - start_pos>tmp_buffer_size)
        {
          delete [] tmp_buffer;
          tmp_buffer_size = pos - start_pos;
          tmp_buffer      = new wchar_t[tmp_buffer_size + 1];  
        }
        wcsncpy(tmp_buffer, dt_string+start_pos, pos - start_pos);
        tmp_buffer[pos - start_pos] = '\0';

        if (curr_token_pos >= format_token_count)
            throw FdoException::Create(
                     FdoException::NLSGetMessage(
                     FUNCTION_DATA_VALUE_ERROR, 
                     "Expression Engine: Invalid value for execution of function '%1$ls'",
                     FDO_FUNCTION_TODATE));

        ProcessDateTimeValue(tmp_buffer, curr_token_pos);

    }  //  if (start_pos_set) ...

    return_datetime_value->SetDateTime(dt_object);
    return FDO_SAFE_ADDREF(return_datetime_value.p);

}  //  Process ()

void FdoFunctionToDate::ProcessDateTimeValue (FdoString  *dt_value,
                                              FdoInt16   curr_token)

// +---------------------------------------------------------------------------
// | The function processes the provided date/time information. If the value is
// | valid, it is stored in the corresponding property of the date/time object.
// | Otherwise, the function issues an exception.
// +---------------------------------------------------------------------------

{

    // The processing depends on the format token currently expected.

    switch (format_tokens[curr_token]) {

      case ToDateFormatSpecifierTokens_Year2:
      case ToDateFormatSpecifierTokens_Year4:
        dt_object.year = ProcessYear(dt_value);
        break;

      case ToDateFormatSpecifierTokens_Month_AbbName_All_Lower:
      case ToDateFormatSpecifierTokens_Month_AbbName_All_Upper:
        dt_object.month = ProcessMonthName(dt_value, true);
        break;

      case ToDateFormatSpecifierTokens_Month_FullName_All_Lower:
      case ToDateFormatSpecifierTokens_Month_FullName_All_Upper:
      case ToDateFormatSpecifierTokens_Month_FullName_First_Upper:
        dt_object.month = ProcessMonthName(dt_value, false);
        break;

      case ToDateFormatSpecifierTokens_Month_Number:
        dt_object.month = ProcessMonthNumber(dt_value);
        break;

      case ToDateFormatSpecifierTokens_Day_AbbName_All_Lower:
      case ToDateFormatSpecifierTokens_Day_AbbName_All_Upper:
        // This information is not stored in the date/time object and
        // hence not processed yet.
        break;

      case ToDateFormatSpecifierTokens_Day_FullName_All_Lower:
      case ToDateFormatSpecifierTokens_Day_FullName_All_Upper:
      case ToDateFormatSpecifierTokens_Day_FullName_First_Upper:
        // This information is not stored in the date/time object and
        // hence not processed yet.
        break;

      case ToDateFormatSpecifierTokens_Day_Number:
        dt_object.day = ProcessDayNumber(dt_value);
        break;

      case ToDateFormatSpecifierTokens_Hour24:
      case ToDateFormatSpecifierTokens_Hour12:
        dt_object.hour = ProcessHour(dt_value, format_tokens[curr_token]);
        is_hour_set    = true;
        is_hh12_format =
            (format_tokens[curr_token] == ToDateFormatSpecifierTokens_Hour12);
        if (is_pm_hour) {

            dt_object.hour = dt_object.hour + 12;
            if (dt_object.hour == 24)
                dt_object.hour = 0;

        }  //  if (is_pm_hour) ...
        break;

      case ToDateFormatSpecifierTokens_Minute:
        dt_object.minute = ProcessMinute(dt_value);
        break;

      case ToDateFormatSpecifierTokens_Second:
        dt_object.seconds = ProcessSecond(dt_value);
        break;

      case ToDateFormatSpecifierTokens_am:
        // Nothing to do.
        break;

      case ToDateFormatSpecifierTokens_pm:
        // If the hour was presented in a 12-hour format, adjust the current
        // hour if set. Otherwise remember this setting for when the hour is
        // processed.

        if (is_hour_set) {

            dt_object.hour = dt_object.hour + 12;
            if (dt_object.hour == 24)
                dt_object.hour = 0;

        }  //  if (is_hour_set) ...
        else
          is_pm_hour = true;
        break;

    }  //  switch ...

}  //  ProcessDateTimeValue ()


FdoInt8 FdoFunctionToDate::ProcessDayName (FdoStringP day,
                                           bool       check_abbreviation)

// +---------------------------------------------------------------------------
// | The function assumes the given string contains the day information of a
// | date/time string and returns the numeric value of the provided day. If
// | an error is detected an exception is issued.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    bool    is_valid_day = false,
            is_known_day = false;

    FdoInt8 i,
            day_value    = 0;

    // Check if the provided information to represent a day is valid. If it is
    // not valid, issue an exception. Otherwise return the numeric value back
    // to the calling routine.

    is_valid_day = ((!day.IsNumber()) && (day.GetLength() > 0));
    if (is_valid_day) {

        for (i = 1; i < 8; i++) {

          is_known_day = (check_abbreviation)
                       ? IsValidDayAbb(i, day)
                       : IsValidDayName(i, day);

          if (is_known_day) {

              day_value = i;
              break;

          }  //  if (is_known_month) ...

        }  //  for (i = 1; i < 8; i++) ...

        is_valid_day = (i < 8);

    }  //  if (is_valid_day) ...

    if (!is_valid_day)
        throw FdoException::Create(
                FdoException::NLSGetMessage(
                FUNCTION_DATA_VALUE_ERROR, 
                "Expression Engine: Invalid value for execution of function '%1$ls'",
                FDO_FUNCTION_TODATE));

    return day_value;

}  //  ProcessDayName ()

FdoInt8 FdoFunctionToDate::ProcessDayNumber (FdoStringP day)

// +---------------------------------------------------------------------------
// | The function assumes the given string contains the day information of a
// | date/time string and returns the numeric value of the provided day. If
// | an error is detected an exception is issued.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    bool    is_valid_day = false;

    FdoInt8 day_value    = 0;

    // Check if the provided information to represent a day is valid. If it is
    // no valid, issue an exception. Otherwise return the numeric value back to
    // the calling routine.

    is_valid_day = ((day.IsNumber()) && (day.GetLength() < 3));
    if (is_valid_day) {

        day_value    = (FdoInt8) day.ToDouble();
        is_valid_day = ((day_value >= 1) && (day_value <= 31));

    }  //  if (is_valid_day) ...

    if (!is_valid_day)
      throw FdoException::Create(
              FdoException::NLSGetMessage(
              FUNCTION_DATA_VALUE_ERROR, 
              "Expression Engine: Invalid value for execution of function '%1$ls'",
              FDO_FUNCTION_TODATE));

    return day_value;

}  //  ProcessDayNumber ()

FdoInt8 FdoFunctionToDate::ProcessHour (
                                    FdoStringP                  hour,
                                    ToDateFormatSpecifierTokens curr_token)

// +---------------------------------------------------------------------------
// | The function assumes the given string contains the hour information of a
// | date/time string and returns the numeric value of the provided hour. If
// | an error is detected an exception is issued.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    bool    is_valid_hour = false;

    FdoInt8 hour_value    = 0;

    // Check if the provided information to represent a hour is valid. If it is
    // no valid, issue an exception. Otherwise return the numeric value back to
    // the calling routine.

    is_valid_hour = ((hour.IsNumber()) && (hour.GetLength() < 3));

    if (is_valid_hour) {

        hour_value = (FdoInt8) hour.ToDouble();
        switch (curr_token) {

          case ToDateFormatSpecifierTokens_Hour12:
            is_valid_hour = ((hour_value > 0 ) && (hour_value <= 12));
            break;

          case ToDateFormatSpecifierTokens_Hour24:
            is_valid_hour = ((hour_value >= 0 ) && (hour_value < 23));
            break;

        }  //  switch ...

    }  //  if (is_valid_hour) ...

    if (!is_valid_hour)
      throw FdoException::Create(
              FdoException::NLSGetMessage(
              FUNCTION_DATA_VALUE_ERROR, 
              "Expression Engine: Invalid value for execution of function '%1$ls'",
              FDO_FUNCTION_TODATE));

    return hour_value;

}  //  ProcessHour ()

FdoInt8 FdoFunctionToDate::ProcessMinute (FdoStringP minute)

// +---------------------------------------------------------------------------
// | The function assumes the given string contains the hour information of a
// | date/time string and returns the numeric value of the provided hour. If
// | an error is detected an exception is issued.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    bool    is_valid_minute = false;

    FdoInt8 minute_value    = 0;

    // Check if the provided information to represent a minute is valid. If it
    // is no valid, issue an exception. Otherwise return the numeric value back
    // to the calling routine.

    is_valid_minute = ((minute.IsNumber()) && (minute.GetLength() < 3));
    if (is_valid_minute) {

        minute_value    = (FdoInt8) minute.ToDouble();
        is_valid_minute = ((minute_value >= 0) && (minute_value <= 59));

    }  //  if (is_valid_minute) ...

    if (!is_valid_minute)
      throw FdoException::Create(
              FdoException::NLSGetMessage(
              FUNCTION_DATA_VALUE_ERROR, 
              "Expression Engine: Invalid value for execution of function '%1$ls'",
              FDO_FUNCTION_TODATE));

    return minute_value;

}  //  ProcessMinute ()

FdoInt8 FdoFunctionToDate::ProcessMonthName (FdoStringP month,
                                             bool       check_abbreviation)

// +---------------------------------------------------------------------------
// | The function assumes the given string contains the month information of a
// | date/time string and returns the numeric value of the provided month. If
// | an error is detected an exception is issued.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    bool    is_valid_month = false,
            is_known_month = false;

    FdoInt8 i,
            month_value    = 0;

    // Check if the provided information to represent a month is valid. If it
    // is not valid, issue an exception. Otherwise return the numeric value
    // back to the calling routine.

    is_valid_month = ((!month.IsNumber()) && (month.GetLength() > 0));
    if (is_valid_month) {

        for (i = 1; i < 13; i++) {

          is_known_month = (check_abbreviation)
                         ? IsValidMonthAbb(i, month)
                         : IsValidMonthName(i, month);

          if (is_known_month) {

              month_value = i;
              break;

          }  //  if (is_known_month) ...

        }  //  for (i = 1; i < 13; i++) ...

        is_valid_month = (i < 13);

    }  //  if (is_valid_month) ...

    if (!is_valid_month)
        throw FdoException::Create(
                FdoException::NLSGetMessage(
                FUNCTION_DATA_VALUE_ERROR, 
                "Expression Engine: Invalid value for execution of function '%1$ls'",
                FDO_FUNCTION_TODATE));

    return month_value;

}  //  ProcessMonthName ()

FdoInt8 FdoFunctionToDate::ProcessMonthNumber (FdoStringP month)

// +---------------------------------------------------------------------------
// | The function assumes the given string contains the month information of a
// | date/time string and returns the numeric value of the provided month. If
// | an error is detected an exception is issued.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    bool    is_valid_month = false;

    FdoInt8 month_value    = 0;

    // Check if the provided information to represent a month is valid. If it
    // is no valid, issue an exception. Otherwise return the numeric value back
    // to the calling routine.

    is_valid_month = ((month.IsNumber()) && (month.GetLength() < 3));
    if (is_valid_month) {

        month_value = (FdoInt8) month.ToDouble();
        is_valid_month = ((month_value >= 1) && (month_value <= 12));

    }  //  if (is_valid_month) ...


    if (!is_valid_month)
      throw FdoException::Create(
              FdoException::NLSGetMessage(
              FUNCTION_DATA_VALUE_ERROR, 
              "Expression Engine: Invalid value for execution of function '%1$ls'",
              FDO_FUNCTION_TODATE));

    return month_value;

}  //  ProcessMonthNumber ()

FdoFloat FdoFunctionToDate::ProcessSecond (FdoStringP second)

// +---------------------------------------------------------------------------
// | The function assumes the given string contains the second information of a
// | date/time string and returns the numeric value of the provided second. If
// | an error is detected an exception is issued.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    bool is_valid_second = false;

    // Check if the provided information to represent a second is valid. If it
    // is no valid, issue an exception. Otherwise return the numeric value back
    // to the calling routine.

    is_valid_second = ((second.IsNumber()) && (second.GetLength() < 3));
    if (!is_valid_second)
      throw FdoException::Create(
              FdoException::NLSGetMessage(
              FUNCTION_DATA_VALUE_ERROR, 
              "Expression Engine: Invalid value for execution of function '%1$ls'",
              FDO_FUNCTION_TODATE));

    return (FdoFloat) second.ToDouble();

}  //  ProcessSecond ()

FdoInt16 FdoFunctionToDate::ProcessYear (FdoStringP year)

// +---------------------------------------------------------------------------
// | The function assumes the given string contains the year information of a
// | date/time string and returns the numeric value of the provided year. If
// | an error is detected an exception is issued.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    bool is_valid_year = false;

    // Check if the provided information to represent a month is valid. If it
    // is no valid, issue an exception. Otherwise return the numeric value back
    // to the calling routine.

    is_valid_year = ((year.IsNumber()            ) &&
                     ((year.GetLength() == 1) || 
                      (year.GetLength() == 2) ||
                      (year.GetLength() == 4)    )    );
    if (!is_valid_year)
      throw FdoException::Create(
              FdoException::NLSGetMessage(
              FUNCTION_DATA_VALUE_ERROR, 
              "Expression Engine: Invalid value for execution of function '%1$ls'",
              FDO_FUNCTION_TODATE));

    return (FdoInt16) year.ToDouble();

}  //  ProcessYear ()

void FdoFunctionToDate::Validate (FdoLiteralValueCollection *literal_values)

// +---------------------------------------------------------------------------
// | The function validates the argument list that was passed in.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoInt32                i,
                            count           = literal_values->GetCount();

    FdoDataValue            *data_value     = NULL;

    FdoPtr<FdoLiteralValue> literal_value;

    // Check the number of arguments. TODATE accepts either one or two para-
    // meters. If the number of parameters is not correct issue an exception.

    if ((count < 1) || (count > 2)) 
        throw FdoException::Create(
                FdoException::NLSGetMessage(
                  FUNCTION_PARAMETER_NUMBER_ERROR, 
                  "Expression Engine: Invalid number of parameters for function '%1$ls'",
                  FDO_FUNCTION_TODATE));

    // Next navigate through the argument list and ensure that the provided
    // data types match the ones defined in the list of supported signatures.
    // If this is not the case issue an exception.

    for (i = 0; i < count; i++) {

      literal_value = literal_values->GetItem(i);
      if (literal_value->GetLiteralValueType() != FdoLiteralValueType_Data)
          throw FdoException::Create(
                  FdoException::NLSGetMessage(
                    FUNCTION_PARAMETER_ERROR, 
                    "Expression Engine: Invalid parameters for function '%1$ls'",
                    FDO_FUNCTION_TODATE));

      data_value = static_cast<FdoDataValue *>(literal_value.p);
      if (data_value->GetDataType() != FdoDataType_String)
          throw FdoException::Create(
                  FdoException::NLSGetMessage(
                    FUNCTION_PARAMETER_DATA_TYPE_ERROR, 
                    "Expression Engine: Invalid parameter data type for function '%1$ls'",
                    FDO_FUNCTION_TODATE));

    }  //  for (i = 0; i < count; i++) ...

}  //  Validate ()

void FdoFunctionToDate::ValidateFormatSpecification (FdoString *format_string)

// +---------------------------------------------------------------------------
// | The function checks whether or not the string containing the format spe-
// | cification is valid. If it is invalid, the function issues an exception.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    bool          start_pos_set          = false,
                  format_specifier_found = false;

    size_t        pos                    = 0,
                  start_pos              = 0,
                  format_length          = 0;


    // The following moves through the string and collects strings between
    // separators. Once a string is collected it is checked against the set
    // of valid format specifiers. If this test fails, the format is invalid.

    format_length = wcslen(format_string);

    if (format_length == 0)
        throw FdoException::Create(
                FdoException::NLSGetMessage(
                FUNCTION_DATA_VALUE_ERROR, 
                "Expression Engine: Invalid value for execution of function '%1$ls'",
                FDO_FUNCTION_TODATE));

    for (pos = 0; pos < format_length; pos++) {

      if (!isalnum(format_string[pos])) {

          if (start_pos_set) {

              format_specifier_found = true;
              if (pos - start_pos>tmp_buffer_size)
              {
                delete [] tmp_buffer;
                tmp_buffer_size = pos - start_pos;
                tmp_buffer      = new wchar_t[tmp_buffer_size + 1];  
              }
              wcsncpy(tmp_buffer, format_string+start_pos, pos - start_pos);
              tmp_buffer[pos - start_pos] = '\0';
              format_tokens[format_token_count] = GetToken(tmp_buffer);
              format_token_count++;
              start_pos_set = false;

          }  //  if (start_pos_set) ...

      }  //  if (!isalnum( ...
      else {

        if (!start_pos_set) {

            start_pos     = pos;
            start_pos_set = true;

        }  //  if (!start_pos_set) ...

      }  //  else

    }  //  for (pos = 0; pos < format_length; pos++) ...

    // If the start position is set, there is still a format specifier to
    // analyze.

    if (start_pos_set) {

        format_specifier_found = true;
        if (pos - start_pos>tmp_buffer_size)
        {
          delete [] tmp_buffer;
          tmp_buffer_size = pos - start_pos;
          tmp_buffer      = new wchar_t[tmp_buffer_size + 1];  
        }
        wcsncpy(tmp_buffer, format_string+start_pos, pos - start_pos);
        tmp_buffer[pos - start_pos] = '\0';
        format_tokens[format_token_count] = GetToken(tmp_buffer);
        format_token_count++;

    }  //  if (start_pos_set) ...

    // If no format specifier was found issue an exception.

    if (!format_specifier_found)
        throw FdoException::Create(
                FdoException::NLSGetMessage(
                FUNCTION_DATA_VALUE_ERROR, 
                "Expression Engine: Invalid value for execution of function '%1$ls'",
                FDO_FUNCTION_TODATE));

}  //  ValidateFormatSpecification ()
