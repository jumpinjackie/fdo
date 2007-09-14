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
#ifndef FDOFUNCTIONEXTRACT_H
#define FDOFUNCTIONEXTRACT_H

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
// The class FdoFunctionExtract implements the Expression Engine function EX-
// TRACT.
// ============================================================================

class FdoFunctionExtract : public FdoExpressionEngineINonAggregateFunction
{

    public:

        // ********************************************************************
        // *                         Public Interfaces                        *
        // ********************************************************************

        // Create:
        //  Function to create an instance of this class.

        static FdoFunctionExtract *Create ();

        // CreateObject:
        //  Function to create an instance of this class.

        virtual FdoFunctionExtract *CreateObject ();

        // GetFunctionDefinition:
        //  The function returns the function definition for the function EX-
        //  TRACT. The definition includes the list of supported signatures
        //  for the function.

        virtual FdoFunctionDefinition *GetFunctionDefinition ();

        // Evaluate:
        //  The function determines the function result and returns it back to
        //  the calling routine.

        virtual FdoLiteralValue *Evaluate (
                                    FdoLiteralValueCollection *literal_values);


    private:

        // ********************************************************************
        // *                       Private Enumerations                       *
        // ********************************************************************

        // ExtractOperationTokens:
        //  The enumeration contains tokens used when handling the request.

        enum ExtractOperationTokens
        {

            ExtractOperationTokens_Year,
            ExtractOperationTokens_Month,
            ExtractOperationTokens_Day,
            ExtractOperationTokens_Hour,
            ExtractOperationTokens_Minute,
            ExtractOperationTokens_Second,

        };  //  ExtractOperationTokens

        // ********************************************************************
        // *                        Private Interfaces                        *
        // ********************************************************************

        // FdoFunctionExtract:
        //  The function represents the class constructor.

        FdoFunctionExtract ();

        // ~FdoFunctionv:
        //  The function represents the class destructor.

        ~FdoFunctionExtract ();

        // CreateFunctionDefinition:
        //  The routine creates the function definition for the function EX-
        //  TRACT.

        void CreateFunctionDefinition ();

        // Dispose:
        //  The function disposes the current object.

        virtual void Dispose () { delete this; };

        // GetToken:
        //  The function returns the token for the provided extract operator.

        ExtractOperationTokens GetToken (FdoStringP operation);

        // Validate:
        //  The function validates the provided parameters for the function
        //  Extract.

        void Validate (FdoLiteralValueCollection *literal_values);


        // ********************************************************************
        // *                      Private Member Variables                    *
        // ********************************************************************

        // function_definition:
        //  The variable references the function definition for the function
        //  Extract.

        FdoFunctionDefinition *function_definition;

        // function_operation_request:
        //  The caller of the function has to indicate what portion of the
        //  date/time data should be extracted. Possible values are YEAR,
        //  MONTH, DAY, HOUR, MINUTE and SECOND. This variable stores the in-
        //  formation that is provided with the call.

        FdoStringP function_operation_request;

};  //  class FdoFunctionExtract

#endif
