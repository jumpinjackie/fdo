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
#ifndef FDOFUNCTIONCOUNT_H
#define FDOFUNCTIONCOUNT_H

#include <FdoExpressionEngine.h>
#include <FdoCommonOSUtil.h>
#include <FdoCommonMiscUtil.h>

#include <Spatial/SpatialStd.h>
#include <Spatial/SpatialUtility.h>

#include <wctype.h>
#include <malloc.h>
#include <math.h>
#include <limits.h>
#include <vector>

#include <FdoExpressionEngineIAggregateFunction.h>
#include <Functions/Aggregate/CacheValueCollection.h>


// ============================================================================
// The class FdoFunctionCount implements the Expression Engine function COUNT.
// ============================================================================

class FdoFunctionCount : public FdoExpressionEngineIAggregateFunction
{

    public:

        // ********************************************************************
        // *                         Public Interfaces                        *
        // ********************************************************************

        // Create:
        //  Function to create an instance of this class.

        static FdoFunctionCount *Create ();

        // CreateObject:
        //  Function to create an instance of this class.

        virtual FdoFunctionCount *CreateObject ();

        // GetFunctionDefinition:
        //  The function returns the function definition for the function COUNT.
        //  The definition includes the list of supported signatures for the
        //  function.

        virtual FdoFunctionDefinition *GetFunctionDefinition ();

        // Process:
        //  The function executes the call of the function COUNT on an expres-
        //  sion value.

        virtual void Process (FdoLiteralValueCollection *literal_values);

        // GetResult:
        //  The function returns the result of the function COUNT.

        virtual FdoLiteralValue *GetResult ();


    private:

        // ********************************************************************
        // *                        Private Interfaces                        *
        // ********************************************************************

        // FdoFunctionCount:
        //  The function represents the class constructor.

        FdoFunctionCount ();

        // ~FdoFunctionCount:
        //  The function represents the class destructor.

        ~FdoFunctionCount ();

        // CreateFunctionDefinition:
        //  The routine creates the function definition for the function COUNT.

        void CreateFunctionDefinition ();

        // Dispose:
        //  The function disposes the current object.

        virtual void Dispose () { delete this; };

        // ProcessRequest:
        //  The function processes a call to the Expression Engine function
        //  COUNT.

        void ProcessRequest (bool        value);
        void ProcessRequest (FdoByte     value);
        void ProcessRequest (FdoDateTime value);
        void ProcessRequest (FdoDouble   value);
        void ProcessRequest (FdoFloat    value);
        void ProcessRequest (FdoInt16    value);
        void ProcessRequest (FdoInt32    value);
        void ProcessRequest (FdoInt64    value);
        void ProcessRequest (FdoString   *value);

        // Validate:
        //  The function validates the provided parameters for the function
        //  COUNT.

        void Validate (FdoLiteralValueCollection *literal_values);


        // ********************************************************************
        // *                      Private Member Variables                    *
        // ********************************************************************

        // bool_false_found:
        //  A flag indicating whether or not the boolean value for FALSE has
        //  been found.

        bool bool_false_found;

        // bool_true_found:
        //  A flag indicating whether or not the boolean value for TRUE has
        //  been found.

        bool bool_true_found;

        // function_definition:
        //  The variable references the function definition for the function
        //  COUNT.

        FdoFunctionDefinition *function_definition;

        // function_result:
        //  The variable holds the function result.

        FdoInt64 function_result;

        // incoming_data_type:
        //  References the data type associated with the provided parameter.

        FdoDataType incoming_data_type;

        // is_distinct_request:
        //  The flag indicates whether or not this is a request to determine
        //  the average expression value on a distinct set of data.

        bool is_distinct_request;

        // is_validated:
        //  For performance reasons the arguments passed to the procedure
        //  processing the request is done once only for the time of its
        //  execution. This variable stores whether or not the validation
        //  has been performed.

        bool is_validated;

        // NULL_value_count:
        //  The variable counts the number of NULL values identified in the
        //  function "Process".

        FdoInt64 NULL_value_count;

        // process_call_count:
        //  The variable counts the number of calls to the function "Process".

        FdoInt64 process_call_count;

        // process_value:
        //  The variable indicates which of the provided parameter values
        //  needs to be processed.

        FdoInt32 process_value;

        // value_cache:
        //  The variable serves as the cache for the average value to be re-
        //  turned.

        CacheValueCollection *value_cache;

};  //  class FdoFunctionCount

#endif
