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
#ifndef FDOFUNCTIONTODATE_H
#define FDOFUNCTIONTODATE_H

#include <FdoExpressionEngine.h>
#include <FdoCommonOSUtil.h>
#include <FdoCommonMiscUtil.h>

#include <Spatial/SpatialStd.h>
#include <Spatial/SpatialUtility.h>

#include <wctype.h>
#include <malloc.h>
#include <math.h>
#include <limits.h>

#include <FdoExpressionEngineINonAggregateFunction.h>


// ============================================================================
// The class FdoFunctionToDate implements the Expression Engine function TODATE
// ============================================================================

class FdoFunctionToDate : public FdoExpressionEngineINonAggregateFunction
{

    static const FdoInt32 INIT_ALLOCATE_SIZE = 100;

    public:

        // ********************************************************************
        // *                         Public Interfaces                        *
        // ********************************************************************

        // Create:
        //  Function to create an instance of this class.

        static FdoFunctionToDate *Create ();

        // CreateObject:
        //  Function to create an instance of this class.

        virtual FdoFunctionToDate *CreateObject ();

        // GetFunctionDefinition:
        //  The function returns the function definition for the function NULL-
        //  VALUE. The definition includes the list of supported signatures for
        //  the function.

        virtual FdoFunctionDefinition *GetFunctionDefinition ();

        // Evaluate:
        //  The function determines the function result and returns it back to
        //  the calling routine.

        virtual FdoLiteralValue *Evaluate (
                                    FdoLiteralValueCollection *literal_values);


    private:

        // ********************************************************************
        // *                       Private Enumerations                       *
        // ********************************************************************

        // ToDateFormatSpecifierTokens:
        //  The enumeration contains tokens used identify format specifiers.

        enum ToDateFormatSpecifierTokens
        {

            ToDateFormatSpecifierTokens_Year2,
            ToDateFormatSpecifierTokens_Year4,
            ToDateFormatSpecifierTokens_Month_AbbName_All_Lower,
            ToDateFormatSpecifierTokens_Month_AbbName_All_Upper,
            ToDateFormatSpecifierTokens_Month_AbbName_First_Upper,
            ToDateFormatSpecifierTokens_Month_FullName_All_Lower,
            ToDateFormatSpecifierTokens_Month_FullName_All_Upper,
            ToDateFormatSpecifierTokens_Month_FullName_First_Upper,
            ToDateFormatSpecifierTokens_Month_Number,
            ToDateFormatSpecifierTokens_Day_AbbName_All_Lower,
            ToDateFormatSpecifierTokens_Day_AbbName_All_Upper,
            ToDateFormatSpecifierTokens_Day_AbbName_First_Upper,
            ToDateFormatSpecifierTokens_Day_FullName_All_Lower,
            ToDateFormatSpecifierTokens_Day_FullName_All_Upper,
            ToDateFormatSpecifierTokens_Day_FullName_First_Upper,
            ToDateFormatSpecifierTokens_Day_Number,
            ToDateFormatSpecifierTokens_Hour24,
            ToDateFormatSpecifierTokens_Hour12,
            ToDateFormatSpecifierTokens_Minute,
            ToDateFormatSpecifierTokens_Second,
            ToDateFormatSpecifierTokens_am,
            ToDateFormatSpecifierTokens_pm,

        };  //  ToDateFormatSpecifierTokens


        // ********************************************************************
        // *                        Private Interfaces                        *
        // ********************************************************************

        // FdoFunctionToDate:
        //  The function represents the class constructor.

        FdoFunctionToDate ();

        // ~FdoFunctionToDate:
        //  The function represents the class destructor.

        ~FdoFunctionToDate ();

        // CreateFunctionDefinition:
        //  The routine creates the function definition for the function NULL-
        //  VALUE.

        void CreateFunctionDefinition ();

        // Dispose:
        //  The function disposes the current object.

        virtual void Dispose () { delete this; };

        // GetToken:
        //  The function returns the token for the provided format specifier.

        ToDateFormatSpecifierTokens GetToken (FdoStringP specifier);

        // IsValidDayAbb:
        //  The function checks whether or not the given day identifies a known
        //  day abbreviation and returns TRUE if this is the case, FALSE other-
        //  wise.

        bool IsValidDayAbb (FdoInt8    index,
                            FdoStringP day_abb);

        // IsValidDayName:
        //  The function checks whether or not the given day identifies a known
        //  day name and returns TRUE if this is the case, FALSE otherwise.

        bool IsValidDayName (FdoInt8    index,
                             FdoStringP day_name);

        // IsValidMonthAbb:
        //  The function checks whether or not the given month identifies a
        //  known month abbreviation and returns TRUE if this is the case,
        //  FALSE otherwise.

        bool IsValidMonthAbb (FdoInt8    index,
                              FdoStringP month_abb);

        // IsValidMonthName:
        //  The function checks whether or not the given month identifies a
        //  known month name and returns TRUE if this is the case, FALSE
        //  otherwise.

        bool IsValidMonthName (FdoInt8    index,
                               FdoStringP month_name);

        // Process:
        //  The function processes the request. It issues an exception if the
        //  structure of the date/time string does not match the structure de-
        //  fined in the format specification.

        FdoLiteralValue *Process (FdoString *dt_string);

        // ProcessDateTimeValue:
        //  The function processes the provided date/time information. If the
        //  value is valid, it is stored in the corresponding property of the
        //  date/time object. Otherwise, the function issues an exception.

        void ProcessDateTimeValue (FdoString  *dt_value,
                                   FdoInt16   curr_token);

        // ProcessDayName:
        //  The function assumes the given string contains the day informa-
        //  tion of a date/time string and returns the numeric value of the
        //  provided day. If an error is detected an exception is issued.

        FdoInt8 ProcessDayName (FdoStringP day,
                                bool       check_abbreviation);

        // ProcessDayNumber:
        //  The function assumes the given string contains the day informa-
        //  tion of a date/time string and returns the numeric value of the
        //  provided day. If an error is detected an exception is issued.

        FdoInt8 ProcessDayNumber (FdoStringP day);

        // ProcessHour:
        //  The function assumes the given string contains the hour informa-
        //  tion of a date/time string and returns the numeric value of the
        //  provided hour. If an error is detected an exception is issued.

        FdoInt8 ProcessHour (FdoStringP                  hour,
                             ToDateFormatSpecifierTokens curr_token);

        // ProcessMinute:
        //  The function assumes the given string contains the minute infor-
        //  mation of a date/time string and returns the numeric value of the
        //  provided minute. If an error is detected an exception is issued.

        FdoInt8 ProcessMinute (FdoStringP minute);

        // ProcessMonthName:
        //  The function assumes the given string contains the month informa-
        //  tion of a date/time string and returns the numeric value of the
        //  provided month. If an error is detected an exception is issued.

        FdoInt8 ProcessMonthName (FdoStringP month,
                                  bool       check_abbreviation);

        // ProcessMonthNumber:
        //  The function assumes the given string contains the month informa-
        //  tion of a date/time string and returns the numeric value of the
        //  provided month. If an error is detected an exception is issued.

        FdoInt8 ProcessMonthNumber (FdoStringP month);

        // ProcessSecond:
        //  The function assumes the given string contains the second informa-
        //  tion of a date/time string and returns the numeric value of the
        //  provided second. If an error is detected an exception is issued.

        FdoFloat ProcessSecond (FdoStringP second);

        // ProcessYear:
        //  The function assumes the given string contains the year information
        //  of a date/time string and returns the numeric value of the provided
        //  year. If an error is detected an exception is issued.

        FdoInt16 ProcessYear (FdoStringP year);

        // Validate:
        //  The function validates the provided parameters for the function
        //  TODATE.

        void Validate (FdoLiteralValueCollection *literal_values);

        // ValidateFormatSpecification:
        //  The function checks whether or not the string containing the 
        //  format specification is valid. If it is invalid, the function
        //  issues an exception.

        void ValidateFormatSpecification (FdoString *format_string);


        // ********************************************************************
        // *                      Private Member Variables                    *
        // ********************************************************************

        // dt_object:
        //  The variable references the date/time object used to keep date/time
        //  information extracted from the date/time string.

        FdoDateTime dt_object;

        // format_tokens:
        //  This array holds the components of the format specification.

        ToDateFormatSpecifierTokens format_tokens[500];

        // format_token_count:
        //  The variable holds the number of positions occupied in the format
        //  token array.

        FdoInt16 format_token_count;

        // function_definition:
        //  The variable references the function definition for the function
        //  TODATE.

        FdoFunctionDefinition *function_definition;

        // is_hh12_format:
        //  A flag indicating whether or not the hour information is provided
        //  in the 12-hour format.

        bool is_hh12_format;

        // is_hour_set:
        //  A flag indicating whether or not the hour information has already
        //  been set.

        bool is_hour_set;

        // is_pm_hour:
        //  A flag indicating whether or not the PM notation is used.

        bool is_pm_hour;


        FdoPtr<FdoDateTimeValue> return_datetime_value;

        bool first;

        wchar_t *tmp_buffer;

        size_t tmp_buffer_size;

};  //  class FdoFunctionToDate

#endif
