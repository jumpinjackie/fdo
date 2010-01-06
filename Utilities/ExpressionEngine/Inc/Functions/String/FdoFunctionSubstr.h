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
#ifndef FDOFUNCTIONSUBSTR_H
#define FDOFUNCTIONSUBSTR_H

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
// The class FdoFunctionSubstr implements the Expression Engine function
// SUBSTR.
// ============================================================================

class FdoFunctionSubstr : public FdoExpressionEngineINonAggregateFunction
{

    // INIT_ALLOCATE_SIZE:
    //  The constant defines the initial size for the internal buffer holding
    //  the function result.

    static const FdoInt32 INIT_ALLOCATE_SIZE = 100;

    public:

        // ********************************************************************
        // *                         Public Interfaces                        *
        // ********************************************************************

        // Create:
        //  Function to create an instance of this class.

        static FdoFunctionSubstr *Create ();

        // CreateObject:
        //  Function to create an instance of this class.

        virtual FdoFunctionSubstr *CreateObject ();

        // GetFunctionDefinition:
        //  The function returns the function definition for the function
        //  SUBSTR. The definition includes the list of supported signatures
        //  for the function.

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

        // FdoFunctionSubstr:
        //  The function represents the class constructor.

        FdoFunctionSubstr ();

        // ~FdoFunctionSubstr:
        //  The function represents the class destructor.

        ~FdoFunctionSubstr ();

        // CreateFunctionDefinition:
        //  The routine creates the function definition for the function
        //  SUBSTR.

        void CreateFunctionDefinition ();

        // Dispose:
        //  The function disposes the current object.

        virtual void Dispose () { delete this; };

        // GetNumericValue:
        //  The function retrieves the value of a numeric argument.

        FdoInt64 GetNumericValue (FdoLiteralValueCollection *literal_values,
                                  FdoInt32                  pos,
                                  FdoDataType               data_type,
                                  bool                      *is_NULL_value);

        // Validate:
        //  The function validates the provided parameters for the function
        //  SUBSTR.

        void Validate (FdoLiteralValueCollection *literal_values);


        // ********************************************************************
        // *                      Private Member Variables                    *
        // ********************************************************************

        // function_definition:
        //  The variable references the function definition for the function
        //  SUBSTR.

        FdoFunctionDefinition *function_definition;

        // is_validated:
        //  For performance reasons the arguments passed to the procedure
        //  processing the request is done once only for the time of its
        //  execution. This variable stores whether or not the validation
        //  has been performed.

        bool is_validated;

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

        // para3_data_type:
        //  References the data type associated with the third function para-
        //  meter.

        FdoDataType para3_data_type;

        // result:
        //  The variable represents the object to be returned by the function
        //  "Evaluate". For performance reasons, this object is allocated only
        //  once.

        FdoPtr<FdoStringValue> result;

        // tmp_buffer:
        //  The variable represents the internal buffer used to hold the func-
        //  tion result.

        wchar_t *tmp_buffer;

        // tmp_buffer_size:
        //  The variable holds the current size of the internal buffer.

        size_t tmp_buffer_size;

};  //  class FdoFunctionSubstr

#endif
