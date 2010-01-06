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
#ifndef FDOFUNCTIONTRUNC_H
#define FDOFUNCTIONTRUNC_H

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
// The class FdoFunctionTrunc implements the Expression Engine function TRUNC.
// ============================================================================

class FdoFunctionTrunc : public FdoExpressionEngineINonAggregateFunction
{

    public:

        // ********************************************************************
        // *                         Public Interfaces                        *
        // ********************************************************************

        // Create:
        //  Function to create an instance of this class.

        static FdoFunctionTrunc *Create ();

        // CreateObject:
        //  Function to create an instance of this class.

        virtual FdoFunctionTrunc *CreateObject ();

        // GetFunctionDefinition:
        //  The function returns the function definition for the function TRUNC.
        //  The definition includes the list of supported signatures for the
        //  function.

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

        // DateTimeOperationTokens:
        //  The enumeration contains tokens used when handling the truncation
        //  of a date to indicate the requested operation.

        enum DateTimeOperationTokens
        {

            DateTimeOperationTokens_Year,
            DateTimeOperationTokens_Month,
            DateTimeOperationTokens_Day,
            DateTimeOperationTokens_Hour,
            DateTimeOperationTokens_Minute,

        };  //  DateTimeOperationTokens

        // ********************************************************************
        // *                        Private Interfaces                        *
        // ********************************************************************

        // FdoFunctionTrunc:
        //  The function represents the class constructor.

        FdoFunctionTrunc ();

        // ~FdoFunctionTrunc:
        //  The function represents the class destructor.

        ~FdoFunctionTrunc ();

        // CreateFunctionDefinition:
        //  The routine creates the function definition for the function TRUNC.

        void CreateFunctionDefinition ();

        // CreateReturnValue:
        //  The function creates the function result object.

        FdoLiteralValue *CreateReturnValue (FdoDataType data_type,
                                            FdoDouble   d_value,
                                            FdoFloat    f_value,
                                            bool        no_value);

        // Dispose:
        //  The function disposes the current object.

        virtual void Dispose () { delete this; };

        // GetParameterValue:
        //  The function returns the value of the first function parameter.

        FdoDouble GetParameterValue (
                                FdoLiteralValueCollection *literal_values,
                                FdoDataType               data_type,
                                bool                      *is_NULL_value);

        // GetParameterValue:
        //  The function returns the value of the second function parameter.

        FdoDouble GetParameterValue (
                                FdoLiteralValueCollection *literal_values,
                                FdoDataType               data_type);

        // GetToken:
        //  The function returns the token for the provided date truncate
        //  operator.

        DateTimeOperationTokens GetToken (FdoStringP operation);

        // ProcessDateTruncationRequest:
        //  The function process a request where the truncate operation is
        //  executed on a date/time argument.

        FdoLiteralValue *ProcessDateTruncationRequest (
                                FdoLiteralValueCollection *literal_values);

        // ProcessNumericTruncationRequest:
        //  The function process a request where the truncate operation is
        //  executed on a numeric argument.

        FdoLiteralValue *ProcessNumericTruncationRequest (
                                FdoLiteralValueCollection *literal_values);

        // Validate:
        //  The function validates the provided parameters for the function
        //  TRUNC.

        void Validate (FdoLiteralValueCollection *literal_values);


        // ********************************************************************
        // *                      Private Member Variables                    *
        // ********************************************************************

        // date_operator:
        //  The variable stores the provided operator if the function is called
        //  to operate on a date argument.

        FdoStringP date_operator;

        // function_definition:
        //  The variable references the function definition for the function
        //  TRUNC.

        FdoFunctionDefinition *function_definition;

        // is_date_truncation:
        //  The flag is set to TRUE if the truncate operation is executed on
        //  date argument, FALSE otherwise.

        bool is_date_truncation;

        // number_of_parameters:
        //  The variable holds the number of parameters provided to the func-
        //  tion.

        FdoInt32 number_of_parameters;

        // para1_data_type:
        //  References the data type associated with the first function para-
        //  meter.

        FdoDataType para1_data_type;

        // para2_data_type:
        //  References the data type associated with the second function para-
        //  meter.

        FdoDataType para2_data_type;

        FdoPtr<FdoDataValue>    return_data_value;

        bool                    first;
};  //  class FdoFunctionTrunc

#endif
