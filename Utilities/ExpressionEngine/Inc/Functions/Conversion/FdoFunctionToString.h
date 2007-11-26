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
#ifndef FDOFUNCTIONTOSTRING_H
#define FDOFUNCTIONTOSTRING_H

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
// The class FdoFunctionToString implements the Expression Engine function
// TOSTRING
// ============================================================================

class FdoFunctionToString : public FdoExpressionEngineINonAggregateFunction
{

    public:

        // ********************************************************************
        // *                         Public Interfaces                        *
        // ********************************************************************

        // Create:
        //  Function to create an instance of this class.

        static FdoFunctionToString *Create ();

        // CreateObject:
        //  Function to create an instance of this class.

        virtual FdoFunctionToString *CreateObject ();

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

        enum ToStringFormatSpecifierTokens
        {

            ToStringFormatSpecifierTokens_Year2,
            ToStringFormatSpecifierTokens_Year4,
            ToStringFormatSpecifierTokens_Month_AbbName_All_Lower,
            ToStringFormatSpecifierTokens_Month_AbbName_All_Upper,
            ToStringFormatSpecifierTokens_Month_FullName_All_Lower,
            ToStringFormatSpecifierTokens_Month_FullName_All_Upper,
            ToStringFormatSpecifierTokens_Month_FullName_First_Upper,
            ToStringFormatSpecifierTokens_Month_Number,
            ToStringFormatSpecifierTokens_Day_AbbName_All_Lower,
            ToStringFormatSpecifierTokens_Day_AbbName_All_Upper,
            ToStringFormatSpecifierTokens_Day_FullName_All_Lower,
            ToStringFormatSpecifierTokens_Day_FullName_All_Upper,
            ToStringFormatSpecifierTokens_Day_FullName_First_Upper,
            ToStringFormatSpecifierTokens_Day_Number,
            ToStringFormatSpecifierTokens_Hour24,
            ToStringFormatSpecifierTokens_Hour12,
            ToStringFormatSpecifierTokens_Minute,
            ToStringFormatSpecifierTokens_Second,
            ToStringFormatSpecifierTokens_am,
            ToStringFormatSpecifierTokens_pm,

        };  //  ToStringFormatSpecifierTokens

        // ********************************************************************
        // *                        Private Interfaces                        *
        // ********************************************************************

        // FdoFunctionToString:
        //  The function represents the class constructor.

        FdoFunctionToString ();

        // ~FdoFunctionToString:
        //  The function represents the class destructor.

        ~FdoFunctionToString ();

        // CreateFunctionDefinition:
        //  The routine creates the function definition for the function NULL-
        //  VALUE.

        void CreateFunctionDefinition ();

        // Dispose:
        //  The function disposes the current object.

        virtual void Dispose () { delete this; };

        // GetDay:
        //  The function returns either the day name or abbreviation for the
        //  provided numeric day identifier based on the requested data.

        FdoStringP GetDay (FdoInt16 year,
                           FdoInt8  month,
                           FdoInt8  day,
                           bool     ret_abbreviation);

        // GetMonth:
        //  The function returns either the month name or abbreviation for the
        //  provided numeric month identifier based on the requested data.

        FdoStringP GetMonth (FdoInt8 month,
                             bool    ret_abbreviation);

        // GetToken:
        //  The function returns the token for the provided format specifier.

        ToStringFormatSpecifierTokens GetToken (FdoStringP specifier);

        // ProcessDateTime:
        //  The function processes the request on a date/time value and returns
        //  the result back to the calling routine.

        FdoLiteralValue *ProcessDateTime (
                                    FdoLiteralValueCollection *literal_values);

        // ProcessDay:
        //  Returns a string containing the numeric representation of the
        //  provided day information.

        FdoStringP ProcessDay (FdoInt8 day);

        // ProcessDayAbb:
        //  Returns a string containing the corresponding day abbreviation
        //  for the provided day information.

        FdoStringP ProcessDayAbb (FdoInt16 year,
                                  FdoInt8  month,
                                  FdoInt8  day,
                                  bool     mixedcase_representation,
                                  bool     uppercase_representation);

        // ProcessDayName:
        //  Returns a string containing the corresponding day name for the
        //  provided day information.

        FdoStringP ProcessDayName (FdoInt16 year,
                                   FdoInt8  month,
                                   FdoInt8  day,
                                   bool     mixedcase_representation,
                                   bool     uppercase_representation);

        // ProcessHour:
        //  Returns a string containing the numeric representation of the
        //  provided hour information.

        FdoStringP ProcessHour (FdoInt8 hour);

        // ProcessMinute:
        //  Returns a string containing the numeric representation of the
        //  provided minute information.

        FdoStringP ProcessMinute (FdoInt8 minute);

        // ProcessMonth:
        //  Returns a string containing the numeric representation of the
        //  provided month information.

        FdoStringP ProcessMonth (FdoInt8 month);

        // ProcessMonthAbb:
        //  Returns a string containing the corresponding month abbreviation
        //  for the provided month information.

        FdoStringP ProcessMonthAbb (FdoInt8 month,
                                    bool    mixedcase_representation,
                                    bool    uppercase_representation);

        // ProcessMonthName:
        //  Returns a string containing the corresponding month name for the
        //  provided month information.

        FdoStringP ProcessMonthName (FdoInt8 month,
                                     bool    mixedcase_representation,
                                     bool    uppercase_representation);

        // ProcessSeconds:
        //  Returns a string containing the numeric representation of the
        //  provided second information.

        FdoStringP ProcessSeconds (FdoFloat seconds);

        // ProcessYear:
        //  Returns a string containing the numeric representation of the
        //  provided year information.

        FdoStringP ProcessYear (FdoInt16 year,
                                bool     year2_representation);

        // ResetFormatStructures:
        //  The function resets the format structures (format token and sepa-
        //  rator list).

        void ResetFormatStructures ();

        // UpdateHour:
        //  If the hour model to be used is HH12, this function adjusts the
        //  value stored with the hour property to reflect the model. This
        //  helps setting the AM/PM indicator accurately.

        FdoInt8 UpdateHour (FdoInt8 hour);

        // Validate:
        //  The function validates the provided parameters for the function
        //  TOSTRING.

        void Validate (FdoLiteralValueCollection *literal_values);

        // ValidateDay
        //  The function checks whether or not the provided day value is
        //  valid. If it is not valid the function issues an exception.

        void ValidateDay (FdoInt16 day);

        // ValidateFormatSpecification:
        //  The function checks whether or not the string containing the 
        //  format specification is valid. If it is invalid, the function
        //  issues an exception.

        void ValidateFormatSpecification (FdoStringP format_string);

        // ValidateHour
        //  The function checks whether or not the provided hour value is
        //  valid. If it is not valid the function issues an exception.

        void ValidateHour (FdoInt8 hour);

        // ValidateMinute
        //  The function checks whether or not the provided minute value is
        //  valid. If it is not valid the function issues an exception.

        void ValidateMinute (FdoInt8 minute);

        // ValidateMonth
        //  The function checks whether or not the provided month value is
        //  valid. If it is not valid the function issues an exception.

        void ValidateMonth (FdoInt8 month);

        // ValidateSecond
        //  The function checks whether or not the provided second value is
        //  valid. If it is not valid the function issues an exception.

        void ValidateSecond (FdoFloat second);


        // ********************************************************************
        // *                      Private Member Variables                    *
        // ********************************************************************

        // format_tokens:
        //  This array holds the components of the format specification.

        ToStringFormatSpecifierTokens format_tokens[500];

        // format_token_count:
        //  The variable holds the number of positions occupied in the format
        //  token array.

        FdoInt16 format_token_count;

        // function_definition:
        //  The variable references the function definition for the function
        //  TOSTRING.

        FdoFunctionDefinition *function_definition;

        // is_h12_representation:
        //  The flag is set to TRUE if the representation of hour information
        //  is to follow the 12-hour model, FALSE otherwise.

        bool is_h12_representation;

        // is_pm_time:
        //  A flag set when converting a time from the 24-hour to the 12-hour
        //  model and indicating whethe or not AM or PM needs to be set.

        bool is_pm_time;

        // para1_data_type:
        //  References the data type associated with the first parameter.

        FdoDataType para1_data_type;

        // separators_list
        //  This array holds the specified list of separators to be used.

        FdoStringP separators_list[500];

        // separators_list_count:
        //  The variable holds the number of positions occupied in the
        //  separator list array.

        FdoInt16 separators_list_count;

};  //  class FdoFunctionToString

#endif
