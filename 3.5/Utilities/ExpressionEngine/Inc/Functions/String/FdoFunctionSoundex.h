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
#ifndef FDOFUNCTIONSOUNDEX_H
#define FDOFUNCTIONSOUNDEX_H

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
// The class FdoFunctionSoundex implements the Expression Engine function
// SOUNDEX.
// ============================================================================

class FdoFunctionSoundex : public FdoExpressionEngineINonAggregateFunction
{

    static const FdoInt32 INIT_ALLOCATE_SIZE = 100;

    public:

        // ********************************************************************
        // *                         Public Interfaces                        *
        // ********************************************************************

        // Create:
        //  Function to create an instance of this class.

        static FdoFunctionSoundex *Create ();

        // CreateObject:
        //  Function to create an instance of this class.

        virtual FdoFunctionSoundex *CreateObject ();

        // GetFunctionDefinition:
        //  The function returns the function definition for the function
        //  SOUNDEX. The definition includes the list of supported signatures
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

        // FdoFunctionSoundex:
        //  The function represents the class constructor.

        FdoFunctionSoundex ();

        // ~FdoFunctionSoundex:
        //  The function represents the class destructor.

        ~FdoFunctionSoundex ();

        // CreateFunctionDefinition:
        //  The routine creates the function definition for the function
        //  SOUNDEX.

        void CreateFunctionDefinition ();

        // Dispose:
        //  The function disposes the current object.

        virtual void Dispose () { delete this; };

        // EliminateDuplicateNumbers:
        //  The function executes the fifth step as outlined in the algorithm
        //  for the function SOUNDEX and eliminates all duplicate numbers.

        void EliminateDuplicateNumbers (wchar_t* value,
                                              FdoInt64   length);

        // EliminateNonAlphaChars:
        //  The function executes the first step as outlined in the algorithm
        //  for the function SOUNDEX and eliminates all characters that are
        //  not letters.

        void EliminateNonAlphaChars (wchar_t* value,
                                    FdoInt64   length);

        // EliminateZeros:
        //  The function executes the sixth step as outlined in the algorithm
        //  for the function SOUNDEX and eliminates all zeros.

        void EliminateZeros (wchar_t* value,
                                FdoInt64   length);

        // Validate:
        //  The function validates the provided parameters for the function
        //  SOUNDEX.

        void Validate (FdoLiteralValueCollection *literal_values);


        // ********************************************************************
        // *                      Private Member Variables                    *
        // ********************************************************************

        // function_definition:
        //  The variable references the function definition for the function
        //  SOUNDEX.

        FdoFunctionDefinition *function_definition;

        FdoPtr<FdoStringValue> return_string_value;

        wchar_t *tmp_buffer;

        size_t tmp_buffer_size;

        bool first;

};  //  class FdoFunctionSoundex

#endif
