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
#ifndef FDOFUNCTIONROUND_H
#define FDOFUNCTIONROUND_H

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
// The class FdoFunctionRound implements the Expression Engine function ROUND.
// ============================================================================

class FdoFunctionRound : public FdoExpressionEngineINonAggregateFunction
{

    public:

        // ********************************************************************
        // *                         Public Interfaces                        *
        // ********************************************************************

        // Create:
        //  Function to create an instance of this class.

        static FdoFunctionRound *Create ();

        // CreateObject:
        //  Function to create an instance of this class.

        virtual FdoFunctionRound *CreateObject ();

        // GetFunctionDefinition:
        //  The function returns the function definition for the function ROUND.
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

        // FdoFunctionRound:
        //  The function represents the class constructor.

        FdoFunctionRound ();

        // ~FdoFunctionRound:
        //  The function represents the class destructor.

        ~FdoFunctionRound ();

        // CreateFunctionDefinition:
        //  The routine creates the function definition for the function ROUND.

        void CreateFunctionDefinition ();

        // CreateReturnValue:
        //  The function creates the function result object.

        FdoLiteralValue *CreateReturnValue (FdoDataType data_type,
                                            FdoDouble   d_value,
                                            FdoFloat    f_value,
                                            bool        no_value);

        // Dispose:
        //  The function disposes the current object.

        virtual void Dispose () { delete this; };

        // GetParameterValue:
        //  The function returns the value of the first function parameter.

        FdoDouble GetParameterValue (
                                FdoLiteralValueCollection *literal_values,
                                FdoDataType               data_type,
                                bool                      *is_NULL_value);

        // GetParameterValue:
        //  The function returns the value of the second function parameter.

        FdoDouble GetParameterValue (
                                FdoLiteralValueCollection *literal_values,
                                FdoDataType               data_type);

        // Validate:
        //  The function validates the provided parameters for the function
        //  ROUND.

        void Validate (FdoLiteralValueCollection *literal_values);


        // ********************************************************************
        // *                      Private Member Variables                    *
        // ********************************************************************

        // function_definition:
        //  The variable references the function definition for the function
        //  ROUND.

        FdoFunctionDefinition *function_definition;

        // number_of_parameters:
        //  The variable holds the number of parameters provided to the func-
        //  tion.

        FdoInt32 number_of_parameters;

        // para1_data_type:
        //  References the data type associated with the first function para-
        //  meter.

        FdoDataType para1_data_type;

        // para2_data_type:
        //  References the data type associated with the second function para-
        //  meter.

        FdoDataType para2_data_type;

};  //  class FdoFunctionRound

#endif
