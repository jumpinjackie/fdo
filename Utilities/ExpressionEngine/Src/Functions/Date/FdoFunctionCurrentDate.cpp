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

#include <stdafx.h>
#include <Functions/Date/FdoFunctionCurrentDate.h>


// ----------------------------------------------------------------------------
// --                         Constructors/Destructors                       --
// ----------------------------------------------------------------------------

FdoFunctionCurrentDate::FdoFunctionCurrentDate ()

// +---------------------------------------------------------------------------
// | The class constructor.
// +---------------------------------------------------------------------------

{

    // Initialize all class variables.
    // NOTE: Due to the fact that data type enumeration misses an entry to
    //       indicate a not-set value, the variable "numeric_data_type" is
    //       set to "FdoDataType_CLOB" to indicate an invalid data type be-
    //       cause this function does not support this type. 

    function_definition = NULL;

    numeric_data_type   = FdoDataType_CLOB;

}  //  FdoFunctionCurrentDate ()


FdoFunctionCurrentDate::~FdoFunctionCurrentDate ()

// +---------------------------------------------------------------------------
// | The class destructor.
// +---------------------------------------------------------------------------

{

    // Delete the function definition.

    FDO_SAFE_RELEASE(function_definition);

}  //  ~FdoFunctionCurrentDate ()


// ----------------------------------------------------------------------------
// --                            Public Class APIs                           --
// ----------------------------------------------------------------------------

FdoFunctionCurrentDate *FdoFunctionCurrentDate::Create ()

// +---------------------------------------------------------------------------
// | The function creates a new instance of the class.
// +---------------------------------------------------------------------------

{

    return new FdoFunctionCurrentDate();

}  //  Create ()

FdoFunctionCurrentDate *FdoFunctionCurrentDate::CreateObject ()

// +---------------------------------------------------------------------------
// | The function creates a new instance of the class.
// +---------------------------------------------------------------------------

{

    return new FdoFunctionCurrentDate();

}  //  CreateObject ()

FdoFunctionDefinition *FdoFunctionCurrentDate::GetFunctionDefinition ()

// +---------------------------------------------------------------------------
// | The function creates the supported signature list for the function ADD-
// | MONTHS.
// +---------------------------------------------------------------------------

{

    if (function_definition == NULL)
        CreateFunctionDefinition();

    return FDO_SAFE_ADDREF(function_definition);

}  //  GetFunctionDefinition ()

FdoLiteralValue *FdoFunctionCurrentDate::Evaluate (
                                    FdoLiteralValueCollection *literal_values)

// +---------------------------------------------------------------------------
// | The function processes a call to the function CURRENTDATE.
// +---------------------------------------------------------------------------

{

    // NOT YET IMPLEMENTED

    throw FdoException::Create(
            FdoException::NLSGetMessage(
                FUNCTION_UNEXPECTED_RESULT_ERROR, 
                "Expression Engine: Unexpected result for function '%1$ls'",
                FDO_FUNCTION_CURRENTDATE));

    return FdoDateTimeValue::Create();

}  //  Evaluate ()


// ----------------------------------------------------------------------------
// --                          Supporting functions                          --
// ----------------------------------------------------------------------------

void FdoFunctionCurrentDate::CreateFunctionDefinition ()

// +---------------------------------------------------------------------------
// | The procedure creates the function definition for the function CURRENT-
// | DATE. The function definition includes the list of supported signatures.
// | The following signatures are supported:
// |
// |    CURRENTDATE ()
// |
// | The function always returns a DATETIME.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoString                        *desc          = NULL;

    FdoPtr<FdoSignatureDefinition>   signature;
    FdoSignatureDefinitionCollection *signatures;

    // Create the signature collection.

    signatures = FdoSignatureDefinitionCollection::Create();

    signature = FdoSignatureDefinition::Create(FdoDataType_DateTime, NULL);
    signatures->Add(signature);

    // Create the function definition.

    desc = FdoException::NLSGetMessage(FUNCTION_CURRENTDATE,
                                       "Returns the current date");
    function_definition =
                FdoFunctionDefinition::Create(
                                        FDO_FUNCTION_CURRENTDATE,
                                        desc,
                                        false,
                                        signatures,
                                        FdoFunctionCategoryType_Date);

}  //  CreateFunctionDefinition ()

void FdoFunctionCurrentDate::Validate (FdoLiteralValueCollection *literal_values)

// +---------------------------------------------------------------------------
// | The function validates the argument list that was passed in.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoInt32 count = literal_values->GetCount();

    // Check the number of arguments. CURRENTDATE does not accept any arguments
    // and hence an exception needs to be issued if the number of parameters is
    // incorrect.

    if (count > 0) 
        throw FdoException::Create(
                FdoException::NLSGetMessage(
                  FUNCTION_PARAMETER_NUMBER_ERROR, 
                  "Expression Engine: Invalid number of parameters for function '%1$ls'",
                  FDO_FUNCTION_CURRENTDATE));

}  //  Validate ()

