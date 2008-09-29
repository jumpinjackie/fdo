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
#ifndef FDOFUNCTIONSTDDEV_H
#define FDOFUNCTIONSTDDEV_H

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
// The class FdoFunctionStddev implements the Expression Engine function STDDEV
// ============================================================================

class FdoFunctionStddev : public FdoExpressionEngineIAggregateFunction
{

    public:

        // ********************************************************************
        // *                         Public Interfaces                        *
        // ********************************************************************

        // Create:
        //  Function to create an instance of this class.

        static FdoFunctionStddev *Create ();

        // CreateObject:
        //  Function to create an instance of this class.

        virtual FdoFunctionStddev *CreateObject ();

        // GetFunctionDefinition:
        //  The function returns the function definition for the function
        //  STDDEV. The definition includes the list of supported signatures
        //  for the function.

        virtual FdoFunctionDefinition *GetFunctionDefinition ();

        // Process:
        //  The function executes the call of the function STDDEV on an
        //  expression value.

        virtual void Process (FdoLiteralValueCollection *literal_values);

        // GetResult:
        //  The function returns the result of the function STDDEV.

        virtual FdoLiteralValue *GetResult ();


    private:

        // ********************************************************************
        // *                        Private Interfaces                        *
        // ********************************************************************

        // FdoFunctionStddev:
        //  The function represents the class constructor.

        FdoFunctionStddev ();

        // ~FdoFunctionStddev:
        //  The function represents the class destructor.

        ~FdoFunctionStddev ();

        // CalculateVariance:
        //  The function calculates the required variance to determine the
        //  standard deviation.

        FdoDouble CalculateVariance (FdoDouble average);

        // CreateFunctionDefinition:
        //  The routine creates the function definition for the function
        //  STDDEV.

        void CreateFunctionDefinition ();

        // Dispose:
        //  The function disposes the current object.

        virtual void Dispose () { delete this; };

        // Validate:
        //  The function validates the provided parameters for the function
        //  STDDEV.

        void Validate (FdoLiteralValueCollection *literal_values);

        // ValueIsInCache:
        //  The function checks if the provided value is already stored in the
        //  cache and returns TRUE if this is the case, FALSE otherwise.

        bool ValueIsInCache (FdoDouble value);


        // ********************************************************************
        // *                      Private Member Variables                    *
        // ********************************************************************

        // function_definition:
        //  The variable references the function definition for the function
        //  STDDEV.

        FdoFunctionDefinition *function_definition;

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
        //  The variable serves as the cache for the average value to be re-
        //  turned.

        CacheValueCollection *value_cache;

        // value_count:
        //  The variable is used to count the number of processed values.

        FdoDouble value_count;

        // value_sum:
        //  The variable is used to sum-up the processed values.

        FdoDouble value_sum;

};  //  class FdoFunctionStddev

#endif
