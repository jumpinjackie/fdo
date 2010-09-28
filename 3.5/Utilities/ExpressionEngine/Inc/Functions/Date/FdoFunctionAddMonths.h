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
#ifndef FDOFUNCTIONADDMONTHS_H
#define FDOFUNCTIONADDMONTHS_H

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
// The class FdoFunctionAddMonths implements the Expression Engine function
// ADDMONTHS.
// ============================================================================

class FdoFunctionAddMonths : public FdoExpressionEngineINonAggregateFunction
{

    public:

        // ********************************************************************
        // *                         Public Interfaces                        *
        // ********************************************************************

        // Create:
        //  Function to create an instance of this class.

        static FdoFunctionAddMonths *Create ();

        // CreateObject:
        //  Function to create an instance of this class.

        virtual FdoFunctionAddMonths *CreateObject ();

        // GetFunctionDefinition:
        //  The function returns the function definition for the function
        //  ADDMONTHS. The definition includes the list of supported signatures
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

        // FdoFunctionAddMonths:
        //  The function represents the class constructor.

        FdoFunctionAddMonths ();

        // ~FdoFunctionAddMonths:
        //  The function represents the class destructor.

        ~FdoFunctionAddMonths ();

        // CreateFunctionDefinition:
        //  The routine creates the function definition for the function
        //  ADDMONTHS.

        void CreateFunctionDefinition ();

        // Dispose:
        //  The function disposes the current object.

        virtual void Dispose () { delete this; };

        // GetNumberOfMonthsToAdd:
        //  The function returns the number of months to be added to the pro-
        //  vided date/time object.

        FdoInt64 GetNumberOfMonthsToAdd (
                                FdoLiteralValueCollection *literal_values,
                                FdoDataType               data_type);

        // Validate:
        //  The function validates the provided parameters for the function
        //  ADDMONTHS.

        void Validate (FdoLiteralValueCollection *literal_values);


        // ********************************************************************
        // *                      Private Member Variables                    *
        // ********************************************************************

        // function_definition:
        //  The variable references the function definition for the function
        //  ADDMONTHS.

        FdoFunctionDefinition *function_definition;

        // is_validated:
        //  For performance reasons the arguments passed to the procedure
        //  processing the request is done once only for the time of its
        //  execution. This variable stores whether or not the validation
        //  has been performed.

        bool is_validated;

        // numeric_data_type:
        //  Stores the kind of data type associated with the numeric argument.

        FdoDataType numeric_data_type;

        // result:
        //  The variable represents the object to be returned by the function
        //  "Evaluate". For performance reasons, this object is allocated only
        //  once.

        FdoPtr<FdoDateTimeValue> result;

};  //  class FdoFunctionAddMonths

#endif
