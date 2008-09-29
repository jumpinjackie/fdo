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
#include "FdoRdbmsSqlServerFunctionIsValid.h"

FdoRdbmsSqlServerFunctionIsValid::FdoRdbmsSqlServerFunctionIsValid ()
{
    function_definition = NULL;
} 

FdoRdbmsSqlServerFunctionIsValid::~FdoRdbmsSqlServerFunctionIsValid ()
{
    FDO_SAFE_RELEASE(function_definition);
}

FdoRdbmsSqlServerFunctionIsValid *FdoRdbmsSqlServerFunctionIsValid::Create ()
{
    return new FdoRdbmsSqlServerFunctionIsValid();
} 

FdoRdbmsSqlServerFunctionIsValid *FdoRdbmsSqlServerFunctionIsValid::CreateObject ()
{
    return new FdoRdbmsSqlServerFunctionIsValid();
} 

FdoFunctionDefinition *FdoRdbmsSqlServerFunctionIsValid::GetFunctionDefinition ()
{
    if (function_definition == NULL)
        CreateFunctionDefinition();

    return FDO_SAFE_ADDREF(function_definition);
} 

FdoLiteralValue *FdoRdbmsSqlServerFunctionIsValid::Evaluate (
                                    FdoLiteralValueCollection *literal_values)
{
    throw FdoException::Create(NlsMsgGet(FDORDBMS_537, "Function IsValid() is executed only on the server side"));

    return NULL;
} 

void FdoRdbmsSqlServerFunctionIsValid::CreateFunctionDefinition ()

// +---------------------------------------------------------------------------
// | The procedure creates the function definition for the function IsValid. The
// | function definition includes the list of supported signatures. The follow-
// | ing signatures are supported:
// |
// |    IsValid (geometry)
// |
// | The function always returns a Int32 (NOTE: there are problems handling booleans).
// +---------------------------------------------------------------------------

{
    // Get the general descriptions for the arguments.
    FdoStringP  arg1_description = FdoException::NLSGetMessage(
                                        FUNCTION_GENERAL_ARG,
                                        "Argument to be processed");

    // The following defines the different argument definition collections.
    FdoStringP  geom_arg_literal = FdoException::NLSGetMessage(FUNCTION_GEOM_ARG_LIT,
                                        "geometry property");

    FdoPtr<FdoArgumentDefinition> geom_arg = FdoArgumentDefinition::Create(geom_arg_literal,
                                          arg1_description,
                                          FdoPropertyType_GeometricProperty,
                                          (FdoDataType)-1);

    FdoPtr<FdoArgumentDefinitionCollection> geom_args = FdoArgumentDefinitionCollection::Create();
    geom_args->Add(geom_arg);

    // Create the signature collection.
    FdoPtr<FdoSignatureDefinitionCollection> signatures = FdoSignatureDefinitionCollection::Create();

    FdoPtr<FdoSignatureDefinition> signature = FdoSignatureDefinition::Create(FdoDataType_Int32, geom_args);
    signatures->Add(signature);

    // Create the function definition.
    function_definition = FdoFunctionDefinition::Create(
                                        FDORDBMSSQLSERVER_FUNCTION_ISVALID,
                                        NlsMsgGet(FDORDBMS_536, "Determines the validity of a given geometry" ),
                                        false,
                                        signatures,
                                        FdoFunctionCategoryType_Geometry);
}

void FdoRdbmsSqlServerFunctionIsValid::Validate (FdoLiteralValueCollection *literal_values)
{
    // Check the number of arguments. IsValid() accepts one parameter only. If the
    // number of parameters is not correct issue an exception.
    FdoInt32                count = literal_values->GetCount();

    if (count != 1) 
        throw FdoException::Create(
                FdoException::NLSGetMessage(
                  FUNCTION_PARAMETER_NUMBER_ERROR, 
                  "Expression Engine: Invalid number of parameters for function '%1$ls'",
                  FDORDBMSSQLSERVER_FUNCTION_ISVALID));

    // Next, identify the data type associated with the value to be processed.
    // An exception is issued if the data type does not match any of the ones
    // the function supports

    FdoPtr<FdoLiteralValue> literal_value = literal_values->GetItem(0);
    if (literal_value->GetLiteralValueType() != FdoLiteralValueType_Geometry)
        throw FdoException::Create(
                FdoException::NLSGetMessage(
                  FUNCTION_PARAMETER_ERROR, 
                  "Expression Engine: Invalid parameters for function '%1$ls'",
                  FDORDBMSSQLSERVER_FUNCTION_ISVALID));
}

