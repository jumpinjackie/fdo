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
#ifndef FDOFUNCTIONTOFLOAT_H
#define FDOFUNCTIONTOFLOAT_H

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
// The class FdoFunctionToFloat implements the Expression Engine function
// TOFLOAT
// ============================================================================

class FdoFunctionToFloat : public FdoExpressionEngineINonAggregateFunction
{

    public:

        // ********************************************************************
        // *                         Public Interfaces                        *
        // ********************************************************************

        // Create:
        //  Function to create an instance of this class.

        static FdoFunctionToFloat *Create ();

        // CreateObject:
        //  Function to create an instance of this class.

        virtual FdoFunctionToFloat *CreateObject ();

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

        // FdoFunctionToFloat:
        //  The function represents the class constructor.

        FdoFunctionToFloat ();

        // ~FdoFunctionToFloat:
        //  The function represents the class destructor.

        ~FdoFunctionToFloat ();

        // CreateFunctionDefinition:
        //  The routine creates the function definition for the function NULL-
        //  VALUE.

        void CreateFunctionDefinition ();

        // Dispose:
        //  The function disposes the current object.

        virtual void Dispose () { delete this; };

        // RemoveBlanks:
        //  The function removes leading and/or trailing blanks from the pro-
        //  vided string and returns the resulting string back to the calling
        //  routine.

        FdoStringP RemoveBlanks (FdoStringP value);

        // Validate:
        //  The function validates the provided parameters for the function
        //  TOFLOAT.

        void Validate (FdoLiteralValueCollection *literal_values);


        // ********************************************************************
        // *                      Private Member Variables                    *
        // ********************************************************************

        // function_definition:
        //  The variable references the function definition for the function
        //  TOFLOAT.

        FdoFunctionDefinition *function_definition;

        // incoming_data_type:
        //  References the data type associated with the provided parameter.

        FdoDataType incoming_data_type;

        FdoPtr<FdoSingleValue>  return_single_value;

        bool                   first;

};  //  class FdoFunctionToFloat

#endif
