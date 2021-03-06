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
#ifndef FDOFUNCTIONCOS_H
#define FDOFUNCTIONCOS_H

#include <FdoExpressionEngineINonAggregateFunction.h>


// ============================================================================
// The class FdoFunctionCos implements the Expression Engine function COS.
// ============================================================================

class FdoFunctionCos : public FdoExpressionEngineINonAggregateFunction
{

    public:

        // ********************************************************************
        // *                         Public Interfaces                        *
        // ********************************************************************

        // Create:
        //  Function to create an instance of this class.

        static FdoFunctionCos *Create ();

        // CreateObject:
        //  Function to create an instance of this class.

        virtual FdoFunctionCos *CreateObject ();

        // GetFunctionDefinition:
        //  The function returns the function definition for the function COS.
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

        // FdoFunctionCos:
        //  The function represents the class constructor.

        FdoFunctionCos ();

        // ~FdoFunctionCos:
        //  The function represents the class destructor.

        ~FdoFunctionCos ();

        // CreateFunctionDefinition:
        //  The routine creates the function definition for the function COS.

        void CreateFunctionDefinition ();

        // Dispose:
        //  The function disposes the current object.

        virtual void Dispose () { delete this; };

        // Validate:
        //  The function validates the provided parameters for the function
        //  COS.

        void Validate (FdoLiteralValueCollection *literal_values);


        // ********************************************************************
        // *                      Private Member Variables                    *
        // ********************************************************************

        // function_definition:
        //  The variable references the function definition for the function
        //  COS.

        FdoFunctionDefinition *function_definition;

        // incoming_data_type:
        //  References the data type associated with the provided parameter.

        FdoDataType incoming_data_type;

        FdoPtr<FdoDoubleValue>  return_double_value;
        bool                    first;

};  //  class FdoFunctionCos

#endif
