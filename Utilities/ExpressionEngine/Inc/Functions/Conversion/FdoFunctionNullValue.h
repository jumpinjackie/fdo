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
#ifndef FDOFUNCTIONNULLVALUE_H
#define FDOFUNCTIONNULLVALUE_H

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
// The class FdoFunctionNullValue implements the Expression Engine function
// NULLVALUE.
// ============================================================================

class FdoFunctionNullValue : public FdoExpressionEngineINonAggregateFunction
{

    public:

        // ********************************************************************
        // *                         Public Interfaces                        *
        // ********************************************************************

        // Create:
        //  Function to create an instance of this class.

        static FdoFunctionNullValue *Create ();

        // CreateObject:
        //  Function to create an instance of this class.

        virtual FdoFunctionNullValue *CreateObject ();

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
        // *                        Private Interfaces                        *
        // ********************************************************************

        // FdoFunctionNullValue:
        //  The function represents the class constructor.

        FdoFunctionNullValue ();

        // ~FdoFunctionNullValue:
        //  The function represents the class destructor.

        ~FdoFunctionNullValue ();

        // CreateFunctionDefinition:
        //  The routine creates the function definition for the function NULL-
        //  VALUE.

        void CreateFunctionDefinition ();

        // ProcessBooleanRequest:
        //  The function executes the request in case the provided parameters
        //  are of type BOOLEAN and returns the result back to the calling
        //  routine. 

        FdoLiteralValue *ProcessBooleanRequest (
                                FdoLiteralValueCollection *literal_values);

        // ProcessByteRequest:
        //  The function executes the request in case the provided parameters
        //  are of type BYTE and returns the result back to the calling pro-
        //  cedure. 

        FdoLiteralValue *ProcessByteRequest (
                                FdoLiteralValueCollection *literal_values);

        // ProcessDateTimeRequest:
        //  The function executes the request in case the provided parameters
        //  are of type DATETIME and returns the result back to the calling
        //  routine. 

        FdoLiteralValue *ProcessDateTimeRequest (
                                FdoLiteralValueCollection *literal_values);

        // ProcessDecimalRequest:
        //  The function executes the request in case the provided first para-
        //  meter is of type DECIMAL and returns the result back to the calling
        //  routine. 

        FdoLiteralValue *ProcessDecimalRequest (
                                FdoLiteralValueCollection *literal_values);

        // ProcessDoubleRequest:
        //  The function executes the request in case the provided first para-
        //  meter is of type DOUBLE and returns the result back to the calling
        //  routine. 

        FdoLiteralValue *ProcessDoubleRequest (
                                FdoLiteralValueCollection *literal_values);

        // ProcessInt16Request:
        //  The function executes the request in case the provided first para-
        //  meter is of type INT16 and returns the result back to the calling
        //  routine. 

        FdoLiteralValue *ProcessInt16Request (
                                FdoLiteralValueCollection *literal_values);

        // ProcessInt32Request:
        //  The function executes the request in case the provided first para-
        //  meter is of type INT32 and returns the result back to the calling
        //  routine. 

        FdoLiteralValue *ProcessInt32Request (
                                FdoLiteralValueCollection *literal_values);

        // ProcessInt64Request:
        //  The function executes the request in case the provided first para-
        //  meter is of type INT64 and returns the result back to the calling
        //  routine. 

        FdoLiteralValue *ProcessInt64Request (
                                FdoLiteralValueCollection *literal_values);

        // ProcessSingleRequest:
        //  The function executes the request in case the provided first para-
        //  meter is of type SINGLE and returns the result back to the calling
        //  routine. 

        FdoLiteralValue *ProcessSingleRequest (
                                FdoLiteralValueCollection *literal_values);

        // ProcessStringRequest:
        //  The function executes the request in case the provided first para-
        //  meter is of type STRING and returns the result back to the calling
        //  routine. 

        FdoLiteralValue *ProcessStringRequest (
                                FdoLiteralValueCollection *literal_values);

        // Dispose:
        //  The function disposes the current object.

        virtual void Dispose () { delete this; };

        // Validate:
        //  The function validates the provided parameters for the function
        //  NULLVALUE.

        void Validate (FdoLiteralValueCollection *literal_values);


        // ********************************************************************
        // *                      Private Member Variables                    *
        // ********************************************************************

        // function_definition:
        //  The variable references the function definition for the function
        //  NULLVALUE.

        FdoFunctionDefinition *function_definition;

        // para1_data_type:
        //  References the data type associated with the first parameter.

        FdoDataType para1_data_type;

        // para2_data_type:
        //  References the data type associated with the second parameter.

        FdoDataType para2_data_type;


        bool first;

        FdoPtr<FdoDataValue> return_data_value;

};  //  class FdoFunctionNullValue

#endif
