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
#ifndef FDOFUNCTIONMEDIAN_H
#define FDOFUNCTIONMEDIAN_H

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
// The class FdoFunctionMedian implements the Expression Engine function MEDIAN
// ============================================================================

class FdoFunctionMedian : public FdoExpressionEngineIAggregateFunction
{

    public:

        // ********************************************************************
        // *                         Public Interfaces                        *
        // ********************************************************************

        // Create:
        //  Function to create an instance of this class.

        static FdoFunctionMedian *Create ();

        // CreateObject:
        //  Function to create an instance of this class.

        virtual FdoFunctionMedian *CreateObject ();

        // GetFunctionDefinition:
        //  The function returns the function definition for the function
        //  MEDIAN. The definition includes the list of supported signatures
        //  for the function.

        virtual FdoFunctionDefinition *GetFunctionDefinition ();

        // Process:
        //  The function executes the call of the function MEDIAN on an
        //  expression value.

        virtual void Process (FdoLiteralValueCollection *literal_values);

        // GetResult:
        //  The function returns the result of the function MEDIAN.

        virtual FdoLiteralValue *GetResult ();


    private:

        // ********************************************************************
        // *                        Private Interfaces                        *
        // ********************************************************************

        // FdoFunctionMedian:
        //  The function represents the class constructor.

        FdoFunctionMedian ();

        // ~FdoFunctionMedian:
        //  The function represents the class destructor.

        ~FdoFunctionMedian ();

        // CreateFunctionDefinition:
        //  The routine creates the function definition for the function
        //  MEDIAN.

        void CreateFunctionDefinition ();

        // Dispose:
        //  The function disposes the current object.

        virtual void Dispose () { delete this; };

        // GetInsertIndex:
        //  The function determines the position in the cache where a new
        //  entry must be entered. This is to ensure that the values in the
        //  cache remain sorted in an ascending order.

        FdoInt32 GetInsertIndex (FdoDouble value);

        // GetValueAtIndex:
        //  The function returns the value stored in the cache at the speci-
        //  fied location.

        FdoDouble GetValueAtIndex (FdoInt32 index);

        // Validate:
        //  The function validates the provided parameters for the function
        //  MEDIAN.

        void Validate (FdoLiteralValueCollection *literal_values);


        // ********************************************************************
        // *                      Private Member Variables                    *
        // ********************************************************************

        // first_value:
        //  The variable is used to remember the first processed value.

        FdoDouble first_value;

        // function_definition:
        //  The variable references the function definition for the function
        //  MEDIAN.

        FdoFunctionDefinition *function_definition;

        // incoming_data_type:
        //  References the data type associated with the provided parameter.

        FdoDataType incoming_data_type;

        // is_validated:
        //  For performance reasons the arguments passed to the procedure
        //  processing the request is done once only for the time of its
        //  execution. This variable stores whether or not the validation
        //  has been performed.

        bool is_validated;

        // value_cache:
        //  The variable serves as the cache for the average value to be re-
        //  turned.

        CacheValueCollection *value_cache;

        // value_count:
        //  The variable is used to count the number of processed values.

        FdoDouble value_count;

};  //  class FdoFunctionMedian

#endif
