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
#ifndef FDOFUNCTIONSUM_H
#define FDOFUNCTIONSUM_H

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
// The class FdoFunctionSum implements the Expression Engine function SUM.
// ============================================================================

class FdoFunctionSum : public FdoExpressionEngineIAggregateFunction
{

    public:

        // ********************************************************************
        // *                         Public Interfaces                        *
        // ********************************************************************

        // Create:
        //  Function to create an instance of this class.

        static FdoFunctionSum *Create ();

        // CreateObject:
        //  Function to create an instance of this class.

        virtual FdoFunctionSum *CreateObject ();

        // GetFunctionDefinition:
        //  The function returns the function definition for the function SUM.
        //  The definition includes the list of supported signatures for the
        //  function.

        virtual FdoFunctionDefinition *GetFunctionDefinition ();

        // Process:
        //  The function executes the call of the function SUM on an expression
        //  value.

        virtual void Process (FdoLiteralValueCollection *literal_values);

        // GetResult:
        //  The function returns the result of the function SUM.

        virtual FdoLiteralValue *GetResult ();


    private:

        // ********************************************************************
        // *                        Private Interfaces                        *
        // ********************************************************************

        // FdoFunctionSum:
        //  The function represents the class constructor.

        FdoFunctionSum ();

        // ~FdoFunctionSum:
        //  The function represents the class destructor.

        ~FdoFunctionSum ();

        // CreateFunctionDefinition:
        //  The routine creates the function definition for the function SUM.

        void CreateFunctionDefinition ();

        // Dispose:
        //  The function disposes the current object.

        virtual void Dispose () { delete this; };

        // ProcessRequest:
        //  The function processes a call to the Expression Engine function SUM.

        void ProcessRequest (FdoByte     value);
        void ProcessRequest (FdoDouble   value);
        void ProcessRequest (FdoFloat    value);
        void ProcessRequest (FdoInt16    value);
        void ProcessRequest (FdoInt32    value);
        void ProcessRequest (FdoInt64    value);

        // Validate:
        //  The function validates the provided parameters for the function
        //  SUM.

        void Validate (FdoLiteralValueCollection *literal_values);


        // ********************************************************************
        // *                      Private Member Variables                    *
        // ********************************************************************

        // function_definition:
        //  The variable references the function definition for the function
        //  SUM.

        FdoFunctionDefinition *function_definition;

        // function_result:
        //  The variable stores the function result.

        FdoDouble function_result;

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

        // process_value:
        //  The variable indicates which of the provided parameter values
        //  needs to be processed.

        FdoInt32 process_value;

        // value_cache:
        //  The variable serves as the cache for the sum of all values to be
        //  returned.

        CacheValueCollection *value_cache;

};  //  class FdoFunctionSum

#endif
