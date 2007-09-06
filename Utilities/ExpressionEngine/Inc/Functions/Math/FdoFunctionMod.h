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
#ifndef FDOFUNCTIONMOD_H
#define FDOFUNCTIONMOD_H

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
// The class FdoFunctionMod implements the Expression Engine function MOD.
// ============================================================================

class FdoFunctionMod : public FdoExpressionEngineINonAggregateFunction
{

    public:

        // ********************************************************************
        // *                         Public Interfaces                        *
        // ********************************************************************

        // Create:
        //  Function to create an instance of this class.

        static FdoFunctionMod *Create ();

        // CreateObject:
        //  Function to create an instance of this class.

        virtual FdoFunctionMod *CreateObject ();

        // GetFunctionDefinition:
        //  The function returns the function definition for the function MOD.
        //  The definition includes the list of supported signatures for the
        //  function.

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

        // FdoFunctionMod:
        //  The function represents the class constructor.

        FdoFunctionMod ();

        // ~FdoFunctionMod:
        //  The function represents the class destructor.

        ~FdoFunctionMod ();

        // CreateFunctionDefinition:
        //  The routine creates the function definition for the function MOD.

        void CreateFunctionDefinition ();

        // Dispose:
        //  The function disposes the current object.

        virtual void Dispose () { delete this; };

        // GetReturnDataType:
        //  Based on the provided data types of the input parameters, the
        //  function determines the data type to be used to communicate
        //  the function result back to the caller.

        FdoDataType GetReturnDataType (FdoDataType inp1_data_type,
                                       FdoDataType inp2_data_type);

        // Validate:
        //  The function validates the provided parameters for the function
        //  MOD.

        void Validate (FdoLiteralValueCollection *literal_values);


        // ********************************************************************
        // *                      Private Member Variables                    *
        // ********************************************************************

        // function_definition:
        //  The variable references the function definition for the function
        //  MOD.

        FdoFunctionDefinition *function_definition;

        // para1_data_type:
        //  References the data type associated with the first function para-
        //  meter.

        FdoDataType para1_data_type;

        // para2_data_type:
        //  References the data type associated with the second function para-
        //  meter.

        FdoDataType para2_data_type;

};  //  class FdoFunctionMod

#endif
