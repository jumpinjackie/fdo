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
#ifndef FDOFUNCTIONMAX_H
#define FDOFUNCTIONMAX_H

#include <FdoExpressionEngine.h>
#include <FdoCommonOSUtil.h>
#include <FdoCommonMiscUtil.h>

#include <Spatial/SpatialStd.h>
#include <Spatial/SpatialUtility.h>

#include <wctype.h>
#include <malloc.h>
#include <math.h>
#include <limits.h>

#include <FdoExpressionEngineIAggregateFunction.h>


// ============================================================================
// The class FdoFunctionMax implements the Expression Engine function MAX.
// ============================================================================

class FdoFunctionMax : public FdoExpressionEngineIAggregateFunction
{

    private:

        // ********************************************************************
        // *                 Private Helper Class Definitions                 *
        // ********************************************************************

        // ResultCacheClass:
        //  This class represents the cache used to calculate the result of the
        //  Expression Engine function MAX.

        class ResultCacheClass
        {

            public:

                FdoByte     byte_value;
                FdoDateTime date_time_value;
                FdoDouble   double_value;
                FdoFloat    float_value;
                FdoInt16    int16_value;
                FdoInt32    int32_value;
                FdoInt64    int64_value;
                FdoString   *string_value;
                bool        value_set;

                ResultCacheClass ();
                virtual ~ResultCacheClass ();

        };  //  class ResultCacheClass


    public:

        // ********************************************************************
        // *                         Public Interfaces                        *
        // ********************************************************************

        // Create:
        //  Function to create an instance of this class.

        static FdoFunctionMax *Create ();

        // CreateObject:
        //  Function to create an instance of this class.

        virtual FdoFunctionMax *CreateObject ();

        // GetFunctionDefinition:
        //  The function returns the function definition for the function MAX.
        //  The definition includes the list of supported signatures for the
        //  function.

        virtual FdoFunctionDefinition *GetFunctionDefinition ();

        // Process:
        //  The function executes the call of the function MAX on an expression
        //  value.

        virtual void Process (FdoLiteralValueCollection *literal_values);

        // GetResult:
        //  The function returns the result of the function MAX.

        virtual FdoLiteralValue *GetResult ();


    private:

        // ********************************************************************
        // *                        Private Interfaces                        *
        // ********************************************************************

        // FdoFunctionMax:
        //  The function represents the class constructor.

        FdoFunctionMax ();

        // ~FdoFunctionMax:
        //  The function represents the class destructor.

        ~FdoFunctionMax ();

        // CreateFunctionDefinition:
        //  The routine creates the function definition for the function MAX.

        void CreateFunctionDefinition ();

        // Dispose:
        //  The function disposes the current object.

        virtual void Dispose () { delete this; };

        // ProcessRequest:
        //  The function processes a call to the Expression Engine function MAX.

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
        //  MAX.

        void Validate (FdoLiteralValueCollection *literal_values);


        // ********************************************************************
        // *                      Private Member Variables                    *
        // ********************************************************************

        // function_definition:
        //  The variable references the function definition for the function
        //  MAX.

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

        // process_value:
        //  The variable indicates which of the provided parameter values
        //  needs to be processed.

        FdoInt32 process_value;

        // value_cache:
        //  The variable serves as the cache for the maximum value to be re-
        //  turned.

        ResultCacheClass *value_cache;

};  //  class FdoFunctionMax

#endif
