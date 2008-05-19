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
#ifndef FDOFUNCTIONTRIM_H
#define FDOFUNCTIONTRIM_H

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
// The class FdoFunctionTrim implements the Expression Engine function TRIM.
// ============================================================================

class FdoFunctionTrim : public FdoExpressionEngineINonAggregateFunction
{

    static const FdoInt32 INIT_ALLOCATE_SIZE = 100;

    public:

        // ********************************************************************
        // *                         Public Interfaces                        *
        // ********************************************************************

        // Create:
        //  Function to create an instance of this class.

        static FdoFunctionTrim *Create ();

        // CreateObject:
        //  Function to create an instance of this class.

        virtual FdoFunctionTrim *CreateObject ();

        // GetFunctionDefinition:
        //  The function returns the function definition for the function
        //  TRIM. The definition includes the list of supported signatures
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

        // FdoFunctionTrim:
        //  The function represents the class constructor.

        FdoFunctionTrim ();

        // ~FdoFunctionTrim:
        //  The function represents the class destructor.

        ~FdoFunctionTrim ();

        // CreateFunctionDefinition:
        //  The routine creates the function definition for the function TRIM.

        void CreateFunctionDefinition ();

        // Dispose:
        //  The function disposes the current object.

        virtual void Dispose () { delete this; };

        // Validate:
        //  The function validates the provided parameters for the function
        //  TRIM.

        void Validate (FdoLiteralValueCollection *literal_values);


        // ********************************************************************
        // *                      Private Member Variables                    *
        // ********************************************************************

        // function_definition:
        //  The variable references the function definition for the function
        //  TRIM.

        FdoFunctionDefinition *function_definition;

        // function_operation_request:
        //  The caller of the function can indicate what kind of trim opera-
        //  tion to execute. Possible indicators are BOTH (remove leading and
        //  trailing blanks), LEADING (remove leading blanks only) and TRAIL-
        //  ING (remove trailing blanks only). This variable stores the in-
        //  formation that is provided with the call.

        FdoStringP function_operation_request;

        FdoPtr<FdoStringValue> return_string_value;

        bool first;

        wchar_t *tmp_buffer;

        size_t tmp_buffer_size;

};  //  class FdoFunctionTrim

#endif
