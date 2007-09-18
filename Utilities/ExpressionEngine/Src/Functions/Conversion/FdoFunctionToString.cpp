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
#include <Functions/Conversion/FdoFunctionToString.h>


// ----------------------------------------------------------------------------
// --                         Constructors/Destructors                       --
// ----------------------------------------------------------------------------

FdoFunctionToString::FdoFunctionToString ()

// +---------------------------------------------------------------------------
// | The class constructor.
// +---------------------------------------------------------------------------

{

    // Initialize all class variables.
    // NOTE: Due to the fact that data type enumeration misses an entry to
    //       indicate a not-set value, the variable "para1_data_type" is set
    //       to "FdoDataType_CLOB" to indicate an invalid data type because
    //       this function does not support this type. 

    function_definition   = NULL;

    para1_data_type       = FdoDataType_CLOB;

    format_token_count    = 0;
    separators_list_count = 0;

    is_pm_time            = false;
    is_h12_representation = false;

}  //  FdoFunctionToString ()


FdoFunctionToString::~FdoFunctionToString ()

// +---------------------------------------------------------------------------
// | The class destructor.
// +---------------------------------------------------------------------------

{

    // Delete the function definition.

    FDO_SAFE_RELEASE(function_definition);

}  //  ~FdoFunctionToString ()


// ----------------------------------------------------------------------------
// --                            Public Class APIs                           --
// ----------------------------------------------------------------------------

FdoFunctionToString *FdoFunctionToString::Create ()

// +---------------------------------------------------------------------------
// | The function creates a new instance of the class.
// +---------------------------------------------------------------------------

{

    return new FdoFunctionToString();

}  //  Create ()

FdoFunctionToString *FdoFunctionToString::CreateObject ()

// +---------------------------------------------------------------------------
// | The function creates a new instance of the class.
// +---------------------------------------------------------------------------

{

    return new FdoFunctionToString();

}  //  CreateObject ()

FdoFunctionDefinition *FdoFunctionToString::GetFunctionDefinition ()

// +---------------------------------------------------------------------------
// | The function creates the supported signature list for the function
// | TOSTRING.
// +---------------------------------------------------------------------------

{

    if (function_definition == NULL)
        CreateFunctionDefinition();

    return FDO_SAFE_ADDREF(function_definition);

}  //  GetFunctionDefinition ()

FdoLiteralValue *FdoFunctionToString::Evaluate (
                                    FdoLiteralValueCollection *literal_values)

// +---------------------------------------------------------------------------
// | The function processes a call to the function TOSTRING.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoPtr<FdoDecimalValue>  decimal_value;
    FdoPtr<FdoDoubleValue>   double_value;
    FdoPtr<FdoInt16Value>    int16_value;
    FdoPtr<FdoInt32Value>    int32_value;
    FdoPtr<FdoInt64Value>    int64_value;
    FdoPtr<FdoSingleValue>   single_value;

    // Validate the function call.

    Validate(literal_values);

    // The processing depends on the data type of the first argument.

    switch (para1_data_type) {

      case FdoDataType_DateTime:
        return ProcessDateTime(literal_values);
        break;

      case FdoDataType_Decimal:
        decimal_value = (FdoDecimalValue *) literal_values->GetItem(0);
        if (decimal_value->IsNull())
            return FdoStringValue::Create();
        else
          return FdoStringValue::Create(
                    FdoStringP::Format(L"%f", decimal_value->GetDecimal()));
        break;

      case FdoDataType_Double:
        double_value = (FdoDoubleValue *) literal_values->GetItem(0);
        if (double_value->IsNull())
            return FdoStringValue::Create();
        else
          return FdoStringValue::Create(
                    FdoStringP::Format(L"%f", double_value->GetDouble()));
        break;

      case FdoDataType_Int16:
        int16_value = (FdoInt16Value *) literal_values->GetItem(0);
        if (int16_value->IsNull())
            return FdoStringValue::Create();
        else
          return FdoStringValue::Create(
                    FdoStringP::Format(L"%d", int16_value->GetInt16()));
        break;

      case FdoDataType_Int32:
        int32_value = (FdoInt32Value *) literal_values->GetItem(0);
        if (int32_value->IsNull())
            return FdoStringValue::Create();
        else
          return FdoStringValue::Create(
                    FdoStringP::Format(L"%d", int32_value->GetInt32()));
        break;

      case FdoDataType_Int64:
        int64_value = (FdoInt64Value *) literal_values->GetItem(0);
        if (int64_value->IsNull())
            return FdoStringValue::Create();
        else
#ifdef _WIN32
          return FdoStringValue::Create(
                    FdoStringP::Format(L"%I64d", int64_value->GetInt64()));
#else
          return FdoStringValue::Create(
                    FdoStringP::Format(L"%lld", int64_value->GetInt64());
#endif
        break;

      case FdoDataType_Single:
        single_value = (FdoSingleValue *) literal_values->GetItem(0);
        if (single_value->IsNull())
            return FdoStringValue::Create();
        else
          return FdoStringValue::Create(
                    FdoStringP::Format(L"%f", single_value->GetSingle()));
        break;

    }  //  switch ...

    // The previous switch statement should have handled all valid cases. As a
    // result the following code should not be reached, but just in case it is
    // reached, issue an exception.

    throw FdoException::Create(
            FdoException::NLSGetMessage(
                FUNCTION_UNEXPECTED_RESULT_ERROR, 
                "Expression Engine: Unexpected result for function '%1$ls'",
                FDO_FUNCTION_TOSTRING));

}  //  Evaluate ()


// ----------------------------------------------------------------------------
// --                          Supporting functions                          --
// ----------------------------------------------------------------------------

void FdoFunctionToString::CreateFunctionDefinition ()

// +---------------------------------------------------------------------------
// | The procedure creates the function definition for the function TOSTRING.
// | The function definition includes the list of supported signatures. The
// | following signatures are supported:
// |
// |    TOSTRING (date [, string])
// |    TOSTRING ({decimal, double, int16, int32, int64, single})
// |
// | The function always returns a STRING.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoString                               *desc               = NULL;

    FdoStringP                              arg1_description;
    FdoStringP                              arg2_description;
    FdoStringP                              dt_arg_literal;
    FdoStringP                              form_arg_literal;
    FdoStringP                              num_arg_literal;

    FdoPtr<FdoArgumentDefinition>           dt_arg;
    FdoPtr<FdoArgumentDefinition>           dcl_arg;
    FdoPtr<FdoArgumentDefinition>           dbl_arg;
    FdoPtr<FdoArgumentDefinition>           int16_arg;
    FdoPtr<FdoArgumentDefinition>           int32_arg;
    FdoPtr<FdoArgumentDefinition>           int64_arg;
    FdoPtr<FdoArgumentDefinition>           sgl_arg;
    FdoPtr<FdoArgumentDefinition>           form_arg;

    FdoPtr<FdoArgumentDefinitionCollection> dt_args;
    FdoPtr<FdoArgumentDefinitionCollection> dt_form_args;

    FdoPtr<FdoArgumentDefinitionCollection> dcl_args;
    FdoPtr<FdoArgumentDefinitionCollection> dbl_args;
    FdoPtr<FdoArgumentDefinitionCollection> int16_args;
    FdoPtr<FdoArgumentDefinitionCollection> int32_args;
    FdoPtr<FdoArgumentDefinitionCollection> int64_args;
    FdoPtr<FdoArgumentDefinitionCollection> sgl_args;

    FdoPtr<FdoSignatureDefinition>          signature;
    FdoSignatureDefinitionCollection        *signatures;

    // Get the general descriptions for the arguments.

    arg1_description = FdoException::NLSGetMessage(
                                                FUNCTION_GENERAL_ARG,
                                                "Argument to be processed");

    arg2_description = FdoException::NLSGetMessage(
                                                FUNCTION_GENERAL_FORMAT_ARG,
                                                "Format specification");

    dt_arg_literal = FdoException::NLSGetMessage(FUNCTION_DATE_ARG_LIT,
                                                 "date/time");

    form_arg_literal = FdoException::NLSGetMessage(FUNCTION_FORMAT_ARG_LIT,
                                                   "optional format");

    num_arg_literal  = FdoException::NLSGetMessage(FUNCTION_NUMBER_ARG_LIT,
                                                   "number");

    // The following defines the different argument definition collections.

    dt_arg    = FdoArgumentDefinition::Create(
                    dt_arg_literal, arg1_description, FdoDataType_DateTime);

    form_arg  = FdoArgumentDefinition::Create(
                    form_arg_literal, arg2_description, FdoDataType_String);

    dcl_arg   = FdoArgumentDefinition::Create(
                    num_arg_literal, arg1_description, FdoDataType_Decimal);

    dbl_arg   = FdoArgumentDefinition::Create(
                    num_arg_literal, arg1_description, FdoDataType_Double);

    int16_arg = FdoArgumentDefinition::Create(
                    num_arg_literal, arg1_description, FdoDataType_Int16);

    int32_arg = FdoArgumentDefinition::Create(
                    num_arg_literal, arg1_description, FdoDataType_Int32);

    int64_arg = FdoArgumentDefinition::Create(
                    num_arg_literal, arg1_description, FdoDataType_Int64);

    sgl_arg   = FdoArgumentDefinition::Create(
                    num_arg_literal, arg1_description, FdoDataType_Single);

    dt_args = FdoArgumentDefinitionCollection::Create();
    dt_args->Add(dt_arg);

    dt_form_args = FdoArgumentDefinitionCollection::Create();
    dt_form_args->Add(dt_arg);
    dt_form_args->Add(form_arg);

    dcl_args = FdoArgumentDefinitionCollection::Create();
    dcl_args->Add(dcl_arg);

    dbl_args = FdoArgumentDefinitionCollection::Create();
    dbl_args->Add(dbl_arg);

    int16_args = FdoArgumentDefinitionCollection::Create();
    int16_args->Add(int16_arg);

    int32_args = FdoArgumentDefinitionCollection::Create();
    int32_args->Add(int32_arg);

    int64_args = FdoArgumentDefinitionCollection::Create();
    int64_args->Add(int64_arg);

    sgl_args = FdoArgumentDefinitionCollection::Create();
    sgl_args->Add(sgl_arg);

    // Create the signature collection.

    signatures = FdoSignatureDefinitionCollection::Create();

    signature = FdoSignatureDefinition::Create(FdoDataType_String, dt_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                            FdoDataType_String, dt_form_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(FdoDataType_String, dcl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(FdoDataType_String, dbl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(FdoDataType_String, int16_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(FdoDataType_String, int32_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(FdoDataType_String, int64_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(FdoDataType_String, sgl_args);
    signatures->Add(signature);

    // Create the function definition.

    desc =
        FdoException::NLSGetMessage(FUNCTION_TOSTRING,
                                    "Converts a number or date to a string");
    function_definition =
                FdoFunctionDefinition::Create(
                                        FDO_FUNCTION_TOSTRING,
                                        desc,
                                        false,
                                        signatures,
                                        FdoFunctionCategoryType_Conversion);

}  //  CreateFunctionDefinition ()

FdoStringP FdoFunctionToString::GetDay (FdoInt8 day,
                                        bool    ret_abbreviation)

// +---------------------------------------------------------------------------
// | The function returns either the day name or abbreviation for the pro-
// | vided numeric day identifier based on the requested data.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoStringP d_name;

    // Get the requested value.

    switch (day) {

      case 1:
        d_name = (ret_abbreviation)
            ? FdoException::NLSGetMessage(DAY_ABB_SUNDAY, "Sun")
            : FdoException::NLSGetMessage(DAY_NAME_SUNDAY, "Sunday");
        break;

      case 2:
        d_name = (ret_abbreviation)
            ? FdoException::NLSGetMessage(DAY_ABB_MONDAY, "Mon")
            : FdoException::NLSGetMessage(DAY_NAME_MONDAY, "Monday");
        break;

      case 3:
        d_name = (ret_abbreviation)
            ? FdoException::NLSGetMessage(DAY_ABB_TUESDAY, "Tue")
            : FdoException::NLSGetMessage(DAY_NAME_TUESDAY, "Tuesday");
        break;

      case 4:
        d_name = (ret_abbreviation)
            ? FdoException::NLSGetMessage(DAY_ABB_WEDNESDAY, "Wed")
            : FdoException::NLSGetMessage(DAY_NAME_WEDNESDAY, "Wednesday");
        break;

      case 5:
        d_name = (ret_abbreviation)
            ? FdoException::NLSGetMessage(DAY_ABB_THURSDAY, "Thu")
            : FdoException::NLSGetMessage(DAY_NAME_THURSDAY, "Thursday");
        break;

      case 6:
        d_name = (ret_abbreviation)
            ? FdoException::NLSGetMessage(DAY_ABB_FRIDAY, "Fri")
            : FdoException::NLSGetMessage(DAY_NAME_FRIDAY, "Friday");
        break;

      case 7:
        d_name = (ret_abbreviation)
            ? FdoException::NLSGetMessage(DAY_ABB_SATURDAY, "Sat")
            : FdoException::NLSGetMessage(DAY_NAME_SATURDAY, "Saturday");
        break;

      default:
        throw FdoException::Create(
                FdoException::NLSGetMessage(
                  FUNCTION_UNEXPECTED_RESULT_ERROR, 
                  "Expression Engine: Unexpected result for function '%1$ls'",
                  FDO_FUNCTION_TOSTRING));
        break;

    }  //  switch ...

    // On Windows, the name received from the NLS catalog has a trailing blank.
    // For the comparison, this needs to be removed.

#ifdef _WIN32
    d_name = d_name.Mid(0, (d_name.GetLength() - 1));
#endif

    return (d_name);

}  //  GetDay ()

FdoStringP FdoFunctionToString::GetMonth (FdoInt8 month,
                                          bool    ret_abbreviation)

// +---------------------------------------------------------------------------
// | The function returns either the month name or abbreviation for the pro-
// | vided numeric month identifier based on the requested data.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoStringP m_name;

    // Get the requested value.

    switch (month) {

      case 1:
        m_name = (ret_abbreviation)
            ? FdoException::NLSGetMessage(MONTH_ABB_JANUARY, "Jan")
            : FdoException::NLSGetMessage(MONTH_NAME_JANUARY, "January");
        break;

      case 2:
        m_name = (ret_abbreviation)
            ? FdoException::NLSGetMessage(MONTH_ABB_FEBRUARY, "Feb")
            : FdoException::NLSGetMessage(MONTH_NAME_FEBRUARY, "February");
        break;

      case 3:
        m_name = (ret_abbreviation)
            ? FdoException::NLSGetMessage(MONTH_ABB_MARCH, "Mar")
            : FdoException::NLSGetMessage(MONTH_NAME_MARCH, "March");
        break;

      case 4:
        m_name = (ret_abbreviation)
            ? FdoException::NLSGetMessage(MONTH_ABB_APRIL, "Apr")
            : FdoException::NLSGetMessage(MONTH_NAME_APRIL, "April");
        break;

      case 5:
        m_name = (ret_abbreviation)
            ? FdoException::NLSGetMessage(MONTH_ABB_MAY, "May")
            : FdoException::NLSGetMessage(MONTH_NAME_MAY, "May");
        break;

      case 6:
        m_name = (ret_abbreviation)
            ? FdoException::NLSGetMessage(MONTH_ABB_JUNE, "Jun")
            : FdoException::NLSGetMessage(MONTH_NAME_JUNE, "June");
        break;

      case 7:
        m_name = (ret_abbreviation)
            ? FdoException::NLSGetMessage(MONTH_ABB_JULY, "Jul")
            : FdoException::NLSGetMessage(MONTH_NAME_JULY, "July");
        break;

      case 8:
        m_name = (ret_abbreviation)
            ? FdoException::NLSGetMessage(MONTH_ABB_AUGUST, "Aug")
            : FdoException::NLSGetMessage(MONTH_NAME_AUGUST, "August");
        break;

      case 9:
        m_name = (ret_abbreviation)
            ? FdoException::NLSGetMessage(MONTH_ABB_SEPTEMBER, "Sep")
            : FdoException::NLSGetMessage(MONTH_NAME_SEPTEMBER, "September");
        break;

      case 10:
        m_name = (ret_abbreviation)
            ? FdoException::NLSGetMessage(MONTH_ABB_OCTOBER, "Oct")
            : FdoException::NLSGetMessage(MONTH_NAME_OCTOBER, "October");
        break;

      case 11:
        m_name = (ret_abbreviation)
            ? FdoException::NLSGetMessage(MONTH_ABB_NOVEMBER, "Nov")
            : FdoException::NLSGetMessage(MONTH_NAME_NOVEMBER, "November");
        break;

      case 12:
        m_name = (ret_abbreviation)
            ? FdoException::NLSGetMessage(MONTH_ABB_DECEMBER, "Dec")
            : FdoException::NLSGetMessage(MONTH_NAME_DECEMBER, "December");
        break;

      default:
        throw FdoException::Create(
                FdoException::NLSGetMessage(
                  FUNCTION_UNEXPECTED_RESULT_ERROR, 
                  "Expression Engine: Unexpected result for function '%1$ls'",
                  FDO_FUNCTION_TOSTRING));
        break;

    }  //  switch ...

    // On Windows, the name received from the NLS catalog has a trailing blank.
    // For the comparison, this needs to be removed.

#ifdef _WIN32
    m_name = m_name.Mid(0, (m_name.GetLength() - 1));
#endif

    return (m_name);

}  //  GetMonth ()

FdoFunctionToString::ToStringFormatSpecifierTokens
                            FdoFunctionToString::GetToken (
                                                        FdoStringP specifier)

// +---------------------------------------------------------------------------
// | The function returns the token for the provided date format specifier.
// +---------------------------------------------------------------------------

{

    // Compare the string against expected and valid format specifiers. If the
    // provided string represents one of them return the corresponding token.

    if (specifier == L"YY")
        return ToStringFormatSpecifierTokens_Year2;

    if (specifier == L"YYYY")
        return ToStringFormatSpecifierTokens_Year4;

    if (specifier == L"MONTH")
        return ToStringFormatSpecifierTokens_Month_FullName_All_Upper;

    if (specifier == L"Month")
        return ToStringFormatSpecifierTokens_Month_FullName_First_Upper;

    if (specifier == L"month")
        return ToStringFormatSpecifierTokens_Month_FullName_All_Lower;

    if (specifier == L"MON")
        return ToStringFormatSpecifierTokens_Month_AbbName_All_Upper;

    if (specifier == L"mon")
        return ToStringFormatSpecifierTokens_Month_AbbName_All_Lower;

    if (specifier == L"MM")
        return ToStringFormatSpecifierTokens_Month_Number;

    if (specifier == L"DAY")
        return ToStringFormatSpecifierTokens_Day_FullName_All_Upper;

    if (specifier == L"Day")
        return ToStringFormatSpecifierTokens_Day_FullName_First_Upper;

    if (specifier == L"day")
        return ToStringFormatSpecifierTokens_Day_FullName_All_Lower;

    if (specifier == L"DY")
        return ToStringFormatSpecifierTokens_Day_AbbName_All_Upper;

    if (specifier == L"dy")
        return ToStringFormatSpecifierTokens_Day_AbbName_All_Lower;

    if (specifier == L"DD")
        return ToStringFormatSpecifierTokens_Day_Number;

    if ((specifier == L"hh24") ||
        (specifier == L"hh"  )    )
        return ToStringFormatSpecifierTokens_Hour24;

    if (specifier == L"hh12")
        return ToStringFormatSpecifierTokens_Hour12;

    if (specifier == L"mm")
        return ToStringFormatSpecifierTokens_Minute;

    if (specifier == L"ss")
        return ToStringFormatSpecifierTokens_Second;

    if ((specifier == L"am") || 
        (specifier == L"AM")    )
        return ToStringFormatSpecifierTokens_am;

    if ((specifier == L"pm") || 
        (specifier == L"PM")    )
        return ToStringFormatSpecifierTokens_pm;

    // The given format specifier is unknown. Issue an exception.

    throw FdoException::Create(
            FdoException::NLSGetMessage(
            FUNCTION_DATA_VALUE_ERROR, 
            "Expression Engine: Invalid value for execution of function '%1$ls'",
            FDO_FUNCTION_TOSTRING));

}  //  GetToken ()

FdoLiteralValue *FdoFunctionToString::ProcessDateTime (
                                    FdoLiteralValueCollection *literal_values)

// +---------------------------------------------------------------------------
// | The function processes the request on a date/time value and returns the
// | result back to the calling routine.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    bool                     ignore_format  = false;

    FdoInt16                 i;

    FdoStringP               dt_string,
                             format_string;

    FdoDateTime              dt_object;

    FdoPtr<FdoDateTimeValue> dt_value;
    FdoPtr<FdoStringValue>   format_specs;

    // Get the date/time information. If the provided value is NULL, return a
    // NULL string value back to the calling routine.

    dt_value = (FdoDateTimeValue *) literal_values->GetItem(0);
    if (dt_value->IsNull())
        return FdoStringValue::Create();

    // Get the date/time information. If the provided information does not
    // representa a valid date or time issue an exception.

    dt_object = dt_value->GetDateTime();
    //if ((!dt_object.IsDate()) && (!dt_object.IsTime()))
    //    throw FdoException::Create(
    //             FdoException::NLSGetMessage(
    //                 FUNCTION_DATA_VALUE_ERROR, 
    //                 "Expression Engine: Invalid value for execution of function '%1$ls'",
    //                 FDO_FUNCTION_TOSTRING));

    // Next check whether or not format instructions were provided. If this is
    // the case get the format instructions. If it is a NULL pointer or if no
    // format instructions were provided, use the default settings.

    if (literal_values->GetCount() == 2) {

        format_specs = (FdoStringValue *) literal_values->GetItem(1);
        if (!format_specs->IsNull()) {

            format_string = format_specs->GetString();
            ValidateFormatSpecification(format_string);

        }  //  if (!format_specs->IsNull()) ...

    }  //  if (literal_values->GetCount() == 2) ...

    if (format_token_count == 0) {

        format_tokens[0] = ToStringFormatSpecifierTokens_Day_Number;
        format_tokens[1] = ToStringFormatSpecifierTokens_Month_AbbName_All_Upper;
        format_tokens[2] = ToStringFormatSpecifierTokens_Year4;
        format_tokens[3] = ToStringFormatSpecifierTokens_Hour24;
        format_tokens[4] = ToStringFormatSpecifierTokens_Minute;
        format_tokens[5] = ToStringFormatSpecifierTokens_Second;

        format_token_count = 6;

        separators_list[0] = L"-";
        separators_list[1] = L"-";
        separators_list[2] = L" ";
        separators_list[3] = L":";
        separators_list[4] = L":";

        separators_list_count = 5;

    }  //  if (format_token_count == 0) ...    

    // If time should be presented using the 12-hour model, update the value
    // in the date/time object to reflect this.

    if (is_h12_representation)
        dt_object.hour = UpdateHour(dt_object.hour);

    // Construct the string based on the current format specification.

    for (i = 0; i < format_token_count; i++) {

      ignore_format = false;

      switch (format_tokens[i]) {

        case ToStringFormatSpecifierTokens_Year2:
          dt_string = dt_string + ProcessYear(dt_object.year, true);
          break;

        case ToStringFormatSpecifierTokens_Year4:
          dt_string = dt_string + ProcessYear(dt_object.year, false);
          break;

        case ToStringFormatSpecifierTokens_Month_AbbName_All_Lower:
          dt_string =
                dt_string + ProcessMonthAbb(dt_object.month, false, false);
          break;

        case ToStringFormatSpecifierTokens_Month_AbbName_All_Upper:
          dt_string =
                dt_string + ProcessMonthAbb(dt_object.month, false, true);
          break;

        case ToStringFormatSpecifierTokens_Month_FullName_All_Lower:
          dt_string =
                dt_string + ProcessMonthName(dt_object.month, false, false);
          break;

        case ToStringFormatSpecifierTokens_Month_FullName_All_Upper:
          dt_string =
                dt_string + ProcessMonthName(dt_object.month, false, true);
          break;

        case ToStringFormatSpecifierTokens_Month_FullName_First_Upper:
          dt_string =
                dt_string + ProcessMonthName(dt_object.month, true, false);
          break;

        case ToStringFormatSpecifierTokens_Month_Number:
          dt_string = dt_string + ProcessMonth(dt_object.month);
          break;

        case ToStringFormatSpecifierTokens_Day_AbbName_All_Lower:
          // No handling of day abbreviation name yet supported.
          ignore_format = true;
          break;

        case ToStringFormatSpecifierTokens_Day_AbbName_All_Upper:
          // No handling of day abbreviation name yet supported.
          ignore_format = true;
          break;

        case ToStringFormatSpecifierTokens_Day_FullName_All_Lower:
          // No handling of day name yet supported.
          ignore_format = true;
          break;

        case ToStringFormatSpecifierTokens_Day_FullName_All_Upper:
          // No handling of day name yet supported.
          ignore_format = true;
          break;

        case ToStringFormatSpecifierTokens_Day_FullName_First_Upper:
          // No handling of day name yet supported.
          ignore_format = true;
          break;

        case ToStringFormatSpecifierTokens_Day_Number:
          dt_string = dt_string + ProcessDay(dt_object.day);
          break;

        case ToStringFormatSpecifierTokens_Hour24:
        case ToStringFormatSpecifierTokens_Hour12:
          dt_string = dt_string + ProcessHour(dt_object.hour);
          break;

        case ToStringFormatSpecifierTokens_Minute:
          dt_string = dt_string + ProcessMinute(dt_object.minute);
          break;

        case ToStringFormatSpecifierTokens_Second:
          dt_string = dt_string + ProcessSeconds(dt_object.seconds);
          break;

        case ToStringFormatSpecifierTokens_am:
          if (is_pm_time)
              dt_string = dt_string + L"PM";
          else
            dt_string = dt_string + L"AM";
          break;

        case ToStringFormatSpecifierTokens_pm:
          if (is_pm_time)
              dt_string = dt_string + L"PM";
          else
            dt_string = dt_string + L"AM";
          break;

      }  //  switch (format_tokens[i]) ...

      if ((i < separators_list_count) && (!ignore_format))
          dt_string = dt_string + separators_list[i];

    }  //  for (i = 0; i < format_token_count; i++) ...

    // Return the generated string.

    return FdoStringValue::Create(dt_string);

}  //  ProcessDateTime ()

FdoStringP FdoFunctionToString::ProcessDay (FdoInt8 day)

// +---------------------------------------------------------------------------
// | Returns a string containing the numeric representation of the provided
// | day information.
// +---------------------------------------------------------------------------

{

    // Check whether or not the value is valid. If it is not valid issue an
    // exception. Otherwise, convert the day to a string and return it back
    // to the calling routine.

    ValidateDay(day);
    return (FdoStringP::Format(L"%d", day));

}  //  ProcessDay ()

FdoStringP FdoFunctionToString::ProcessDayAbb (
                                            FdoInt8 day,
                                            bool    mixedcase_representation,
                                            bool    uppercase_representation)

// +---------------------------------------------------------------------------
// | Returns a string containing the corresponding day abbreviation for the
// | provided day information.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoStringP ret_value;

    // Validate the day information. If the day information is valid get the
    // corresponding abbreviation. If the abbreviation has to be provided in
    // mixed-case letters, nothing else needs to be done as this is the case
    // the abbreviations are defined. In case the abbreviation has not to be
    // provided in mixed case, either convert it to uppercase or lowercase
    // letters depending on the request.

    ValidateDay(day);
    ret_value = GetDay(day, true);
    if (!mixedcase_representation) {

        if (uppercase_representation)
            ret_value = ret_value.Upper();
        else
          ret_value = ret_value.Lower();

    }  //  if (!mixedcase_representation) ...

    // Return the resulting string back to the calling routine.

    return ret_value;

}  //  ProcessDayAbb ()

FdoStringP FdoFunctionToString::ProcessDayName (
                                            FdoInt8 day,
                                            bool    mixedcase_representation,
                                            bool    uppercase_representation)

// +---------------------------------------------------------------------------
// | Returns a string containing the corresponding day name for the provided
// | day information.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoStringP ret_value;

    // Validate the day information. If the day information is valid get the
    // corresponding day name. If the name has to be provided in mixed case
    // letters, nothing else needs to be done as this is the way the names are
    // defined. In case the name has not to be provided in mixed case, either
    // convert it to uppercase or lowercase letters depending on the request.

    ValidateDay(day);
    ret_value = GetDay(day, false);
    if (!mixedcase_representation) {

        if (uppercase_representation)
            ret_value = ret_value.Upper();
        else
          ret_value = ret_value.Lower();

    }  //  if (!mixedcase_representation) ...

    // Return the resulting string back to the calling routine.

    return ret_value;

}  //  ProcessDayName ()

FdoStringP FdoFunctionToString::ProcessHour (FdoInt8 hour)

// +---------------------------------------------------------------------------
// | Returns a string containing the numeric representation of the provided
// | hour information.
// +---------------------------------------------------------------------------

{

    // The representation in the date/time object is in a 24-hour model.
    // Validate that the value is in the valid range. If it is not issue an
    // exception. Otherwise convert it to a string.

    if ((hour < 0) || (hour > 24))
        throw FdoException::Create(
                 FdoException::NLSGetMessage(
                     FUNCTION_DATA_VALUE_ERROR, 
                     "Expression Engine: Invalid value for execution of function '%1$ls'",
                     FDO_FUNCTION_TOSTRING));

    return(FdoStringP::Format(L"%d", hour));

}  //  ProcessHour ()

FdoStringP FdoFunctionToString::ProcessMinute (FdoInt8 minute)

// +---------------------------------------------------------------------------
// | Returns a string containing the numeric representation of the provided
// | minute information.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoStringP ret_value;

    // Validate that the value is in the valid range. If it is not issue an
    // exception. Otherwise convert it to a string.

    if ((minute < 0) || (minute > 59))
        throw FdoException::Create(
                 FdoException::NLSGetMessage(
                     FUNCTION_DATA_VALUE_ERROR, 
                     "Expression Engine: Invalid value for execution of function '%1$ls'",
                     FDO_FUNCTION_TOSTRING));

    ret_value = FdoStringP::Format(L"%d", minute);
    if (ret_value.GetLength() == 1)
        ret_value = FdoStringP::Format(L"%s", L"0") + ret_value;

    return ret_value;

}  //  ProcessMinute ()

FdoStringP FdoFunctionToString::ProcessMonth (FdoInt8 month)

// +---------------------------------------------------------------------------
// | Returns a string containing the numeric representation of the provided
// | month information.
// +---------------------------------------------------------------------------

{

    // Check whether or not the value is valid. If it is not valid issue an
    // exception. Otherwise, convert the month to a string and return it back
    // to the calling routine.

    ValidateMonth(month);
    return (FdoStringP::Format(L"%d", month));

}  //  ProcessMonth ()

FdoStringP FdoFunctionToString::ProcessMonthAbb (
                                            FdoInt8 month,
                                            bool    mixedcase_representation,
                                            bool    uppercase_representation)

// +---------------------------------------------------------------------------
// | Returns a string containing the corresponding month abbreviation for the
// | provided month information.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoStringP ret_value;

    // Validate the month information. If the month information is valid get
    // the corresponding abbreviation. If the abbreviation has to be provided
    // in mixed-case letters, nothing else needs to be done as this is the
    // case the abbreviations are defined. In case the abbreviation has not to
    // be provided in mixed case, either convert it to uppercase or lowercase
    // letters depending on the request.

    ValidateMonth(month);
    ret_value = GetMonth(month, true);
    if (!mixedcase_representation) {

        if (uppercase_representation)
            ret_value = ret_value.Upper();
        else
          ret_value = ret_value.Lower();

    }  //  if (!mixedcase_representation) ...

    // Return the resulting string back to the calling routine.

    return ret_value;

}  //  ProcessMonthAbb ()

FdoStringP FdoFunctionToString::ProcessMonthName (
                                            FdoInt8 month,
                                            bool    mixedcase_representation,
                                            bool    uppercase_representation)

// +---------------------------------------------------------------------------
// | Returns a string containing the corresponding month name for the provided
// | month information.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoStringP ret_value;

    // Validate the month information. If the month information is valid get
    // the corresponding month name. If the name has to be provided in mixed
    // case letters, nothing else needs to be done as this is the way the names
    // are defined. In case the name has not to be provided in mixed case,
    // either convert it to uppercase or lowercase letters depending on the
    // request.

    ValidateMonth(month);
    ret_value = GetMonth(month, false);
    if (!mixedcase_representation) {

        if (uppercase_representation)
            ret_value = ret_value.Upper();
        else
          ret_value = ret_value.Lower();

    }  //  if (!mixedcase_representation) ...

    // Return the resulting string back to the calling routine.

    return ret_value;

}  //  ProcessMonthName ()

FdoStringP FdoFunctionToString::ProcessSeconds (FdoFloat seconds)

// +---------------------------------------------------------------------------
// | Returns a string containing the numeric representation of the provided
// | second information.
// +---------------------------------------------------------------------------

{

    // Convert the value to a string.

    if (seconds == 0)
        return(L"00");
    else
      return(FdoStringP::Format(L"%f", seconds));

}  //  ProcessSeconds ()

FdoStringP FdoFunctionToString::ProcessYear (FdoInt16 year,
                                             bool     year2_representation)

// +---------------------------------------------------------------------------
// | Returns a string containing the numeric representation of the provided
// | year information.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoStringP ret_value;

    // Convert the year to a string and get the length of the resulting string.
    // If the format specification asks for two year columns only, cut off the
    // leading two characters.
    // NOTE: If the year-representation in the string is with 2 digits and the
    //       format instruction asks for 4 digits, nothing is done as it is not
    //       clear how to complete the year.

    ret_value = FdoStringP::Format(L"%d", year);
    if ((ret_value.GetLength() > 2) && (year2_representation))
        ret_value = ret_value.Mid((ret_value.GetLength() - 2), 2);

    return ret_value;

}  //  ProcessYear ()

FdoInt8 FdoFunctionToString::UpdateHour (FdoInt8 hour)

// +---------------------------------------------------------------------------
// | If the hour model to be used is HH12, this function adjusts the value
// | stored with the hour property to reflect the model. This helps setting
// | the AM/PM indicator accurately.
// +---------------------------------------------------------------------------

{

    // The representation of the time in the date/time object is using the
    // 24-hour model. Since the format instructions indicate to use the 12-
    // hour model, this function updates the value and sets a flag to indi-
    // cate whether AM or PM must be used to accurately represent the time.
    // In general, if the provided time information is not valid, issue an
    // exception.

    if ((hour < 0) || (hour > 23))
        throw FdoException::Create(
                 FdoException::NLSGetMessage(
                     FUNCTION_DATA_VALUE_ERROR, 
                     "Expression Engine: Invalid value for execution of function '%1$ls'",
                     FDO_FUNCTION_TOSTRING));

    is_pm_time = (hour > 12);

    if (hour > 12)
        return (hour - 12);
    else
      return hour;

}  //  UpdateHour ()

void FdoFunctionToString::Validate (FdoLiteralValueCollection *literal_values)

// +---------------------------------------------------------------------------
// | The function validates the argument list that was passed in.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoInt32                i,
                            count           = literal_values->GetCount();

    FdoDataType             p2_data_type    = FdoDataType_CLOB;

    FdoDataValue            *data_value     = NULL;

    FdoPtr<FdoLiteralValue> literal_value;

    // Check the number of arguments. TOSTRING accepts one or two parameters.
    // If the number of parameters is not correct issue an exception.

    if ((count < 1) || (count > 2)) 
        throw FdoException::Create(
                FdoException::NLSGetMessage(
                  FUNCTION_PARAMETER_NUMBER_ERROR, 
                  "Expression Engine: Invalid number of parameters for function '%1$ls'",
                  FDO_FUNCTION_TOSTRING));

    // Next, check whether or not the parameters have a valid data type. If
    // this is not the case issue an exception.

    for (i = 0; i < count; i++) {

      literal_value = literal_values->GetItem(i);
      if (literal_value->GetLiteralValueType() != FdoLiteralValueType_Data)
          throw FdoException::Create(
                  FdoException::NLSGetMessage(
                    FUNCTION_PARAMETER_ERROR, 
                    "Expression Engine: Invalid parameters for function '%1$ls'",
                    FDO_FUNCTION_TOSTRING));

      data_value = static_cast<FdoDataValue *>(literal_value.p);
      if (i == 0)
          para1_data_type  = data_value->GetDataType();
      else
        p2_data_type  = data_value->GetDataType();

    }  //  for (i = 0; i < count; i++) ...

    if ((para1_data_type != FdoDataType_Decimal ) &&
        (para1_data_type != FdoDataType_Double  ) &&
        (para1_data_type != FdoDataType_Int16   ) &&
        (para1_data_type != FdoDataType_Int32   ) &&
        (para1_data_type != FdoDataType_Int64   ) &&
        (para1_data_type != FdoDataType_Single  ) &&
        (para1_data_type != FdoDataType_DateTime)    )
        throw FdoException::Create(
                FdoException::NLSGetMessage(
                  FUNCTION_PARAMETER_DATA_TYPE_ERROR, 
                  "Expression Engine: Invalid parameter data type for function '%1$ls'",
                  FDO_FUNCTION_TOSTRING));

    if (count == 2)
        if (p2_data_type != FdoDataType_String)
            throw FdoException::Create(
                    FdoException::NLSGetMessage(
                    FUNCTION_PARAMETER_DATA_TYPE_ERROR, 
                    "Expression Engine: Invalid parameter data type for function '%1$ls'",
                    FDO_FUNCTION_TOSTRING));

}  //  Validate ()

void FdoFunctionToString::ValidateFormatSpecification (
                                                    FdoStringP format_string)

// +---------------------------------------------------------------------------
// | The function checks whether or not the string containing the format spe-
// | cification is valid. If it is invalid, the function issues an exception.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    bool          start_pos_set          = false,
                  separator_added        = false,
                  format_specifier_found = false;

    size_t        pos                    = 0,
                  start_pos              = 0,
                  format_length          = 0;

    FdoStringP    mid_string,
                  format_spec;

    unsigned char *work_char             = NULL;

    // The following moves through the string and collects strings between
    // separators. Once a string is collected it is checked against the set
    // of valid format specifiers. If this test fails, the format is invalid.

    format_length = format_string.GetLength();

    if (format_length == 0)
        throw FdoException::Create(
                FdoException::NLSGetMessage(
                FUNCTION_DATA_VALUE_ERROR, 
                "Expression Engine: Invalid value for execution of function '%1$ls'",
                FDO_FUNCTION_TOSTRING));

    for (pos = 0; pos < format_length; pos++) {

      mid_string = format_string.Mid(pos, 1);
      work_char  = (unsigned char*)((const char *)mid_string);

      if (!isalnum(work_char[0])) {

          separators_list[separators_list_count] =
                        separators_list[separators_list_count] + mid_string;
          separator_added = true;

          if (start_pos_set) {

              format_specifier_found = true;
              format_spec = format_string.Mid(start_pos, (pos - start_pos));
              format_tokens[format_token_count] = GetToken(format_spec);
              if (format_tokens[format_token_count] ==
                                        ToStringFormatSpecifierTokens_Hour12)
                  is_h12_representation = true;
              format_token_count++;
              start_pos_set = false;

          }  //  if (start_pos_set) ...

      }  //  if (!isalnum( ...
      else {

        if (separator_added) {

            separators_list_count++;
            separator_added = false;

        }  //  if (separator_added) ...

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
        format_spec = format_string.Mid(start_pos, (pos - start_pos));
        format_tokens[format_token_count] = GetToken(format_spec);
        format_token_count++;

    }  //  if (start_pos_set) ...

    // If no format specifier was found issue an exception.

    if (!format_specifier_found)
        throw FdoException::Create(
                FdoException::NLSGetMessage(
                FUNCTION_DATA_VALUE_ERROR, 
                "Expression Engine: Invalid value for execution of function '%1$ls'",
                FDO_FUNCTION_TOSTRING));

}  //  ValidateFormatSpecification ()

void FdoFunctionToString::ValidateDay (FdoInt16 day)

// +---------------------------------------------------------------------------
// | The function checks whether or not the provided day value is valid. If
// | it is not valid the function issues an exception.
// +---------------------------------------------------------------------------

{

    if ((day < 1) || (day > 31))
        throw FdoException::Create(
                 FdoException::NLSGetMessage(
                     FUNCTION_DATA_VALUE_ERROR, 
                     "Expression Engine: Invalid value for execution of function '%1$ls'",
                     FDO_FUNCTION_TOSTRING));

}  //  ValidateDay ()

void FdoFunctionToString::ValidateMonth (FdoInt16 month)

// +---------------------------------------------------------------------------
// | The function checks whether or not the provided month value is valid. If
// | it is not valid the function issues an exception.
// +---------------------------------------------------------------------------

{

    if ((month < 1) || (month > 12))
        throw FdoException::Create(
                 FdoException::NLSGetMessage(
                     FUNCTION_DATA_VALUE_ERROR, 
                     "Expression Engine: Invalid value for execution of function '%1$ls'",
                     FDO_FUNCTION_TOSTRING));

}  //  ValidateMonth ()

