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
#include <Functions/String/FdoFunctionInstr.h>


// ----------------------------------------------------------------------------
// --                         Constructors/Destructors                       --
// ----------------------------------------------------------------------------

FdoFunctionInstr::FdoFunctionInstr ()

// +---------------------------------------------------------------------------
// | The class constructor.
// +---------------------------------------------------------------------------

{

    // Initialize all class variables.

    function_definition = NULL;

}  //  FdoFunctionInstr ()


FdoFunctionInstr::~FdoFunctionInstr ()

// +---------------------------------------------------------------------------
// | The class destructor.
// +---------------------------------------------------------------------------

{

    // Delete the function definition.

    FDO_SAFE_RELEASE(function_definition);

}  //  ~FdoFunctionInstr ()


// ----------------------------------------------------------------------------
// --                            Public Class APIs                           --
// ----------------------------------------------------------------------------

FdoFunctionInstr *FdoFunctionInstr::Create ()

// +---------------------------------------------------------------------------
// | The function creates a new instance of the class.
// +---------------------------------------------------------------------------

{

    return new FdoFunctionInstr();

}  //  Create ()

FdoFunctionInstr *FdoFunctionInstr::CreateObject ()

// +---------------------------------------------------------------------------
// | The function creates a new instance of the class.
// +---------------------------------------------------------------------------

{

    return new FdoFunctionInstr();

}  //  CreateObject ()

FdoFunctionDefinition *FdoFunctionInstr::GetFunctionDefinition ()

// +---------------------------------------------------------------------------
// | The function creates the supported signature list for the function INSTR.
// +---------------------------------------------------------------------------

{

    if (function_definition == NULL)
        CreateFunctionDefinition();

    return FDO_SAFE_ADDREF(function_definition);

}  //  GetFunctionDefinition ()

FdoLiteralValue *FdoFunctionInstr::Evaluate (
                                    FdoLiteralValueCollection *literal_values)

// +---------------------------------------------------------------------------
// | The function processes a call to the function INSTR.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoInt32               i;

    FdoStringP             result,
                           base_string,
                           search_string;

    FdoPtr<FdoStringValue> string_value;

    // Validate the function call.

    Validate(literal_values);

    // Get the strings provided as arguments. If any of them represents an
    // empty value terminate the function by returning 0.

    for (i = 0; i < 2; i++) {

      string_value = (FdoStringValue *) literal_values->GetItem(i);
      if (string_value->IsNull())
          return FdoInt64Value::Create((FdoInt64)0);
      else {

        if (i == 0)
            base_string = base_string + string_value->GetString();
        else
          search_string = search_string + string_value->GetString();

      }  //  else ...

    }  //  for (i = 0; i < 2; i++) ...

    // Process the request and return the position of the search string within
    // the given string.

    if (!base_string.Contains(search_string))
        return FdoInt64Value::Create((FdoInt64)0);

    result = base_string.Left(search_string);
    return FdoInt64Value::Create(result.GetLength()+1);

}  //  Evaluate ()


// ----------------------------------------------------------------------------
// --                          Supporting functions                          --
// ----------------------------------------------------------------------------

void FdoFunctionInstr::CreateFunctionDefinition ()

// +---------------------------------------------------------------------------
// | The procedure creates the function definition for the function INSTR. The
// | function definition includes the list of supported signatures. The follow-
// | ing signatures are supported:
// |
// |    INSTR (string, string)
// |
// | The function always returns an INT64.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoString *desc = NULL;

    FdoStringP arg1_description;
    FdoStringP arg2_description;
    FdoStringP str_arg_literal;
    FdoStringP str2_arg_literal;

    FdoPtr<FdoArgumentDefinition> str1_arg;
    FdoPtr<FdoArgumentDefinition> str2_arg;

    FdoPtr<FdoArgumentDefinitionCollection> str_args;

    FdoPtr<FdoSignatureDefinition>          signature;
    FdoSignatureDefinitionCollection        *signatures;

    // Get the general descriptions for the arguments.

    arg1_description = FdoException::NLSGetMessage(
                                                FUNCTION_INSTR_STRING1_ARG,
                                                "String to search in");

    arg2_description = FdoException::NLSGetMessage(
                                                FUNCTION_INSTR_STRING2_ARG,
                                                "String to look for");

    // The following defines the different argument definition collections.

    str_arg_literal = FdoException::NLSGetMessage(FUNCTION_STRING_ARG_LIT,
                                                  "text property");

    str2_arg_literal =
                FdoException::NLSGetMessage(FUNCTION_STRING_SEARCH_ARG_LIT,
                                            "search text");

    str1_arg = FdoArgumentDefinition::Create(
                    str_arg_literal, arg1_description, FdoDataType_String);
    str2_arg = FdoArgumentDefinition::Create(
                    str2_arg_literal, arg2_description, FdoDataType_String);

    str_args = FdoArgumentDefinitionCollection::Create();
    str_args->Add(str1_arg);
    str_args->Add(str2_arg);

    // Create the signature collection.

    signatures = FdoSignatureDefinitionCollection::Create();

    signature = FdoSignatureDefinition::Create(FdoDataType_Int64, str_args);
    signatures->Add(signature);

    // Create the function definition.

    desc = 
        FdoException::NLSGetMessage(
                    FUNCTION_INSTR,
                    "Returns the position of a string within a base string");
    function_definition =
                FdoFunctionDefinition::Create(
                                        FDO_FUNCTION_INSTR,
                                        desc,
                                        false,
                                        signatures,
                                        FdoFunctionCategoryType_String);

}  //  CreateFunctionDefinition ()

void FdoFunctionInstr::Validate (FdoLiteralValueCollection *literal_values)

// +---------------------------------------------------------------------------
// | The function validates the argument list that was passed in.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoInt32                i,
                            count           = literal_values->GetCount();

    FdoDataType             data_type;

    FdoDataValue            *data_value     = NULL;

    FdoPtr<FdoLiteralValue> literal_value;

    // Check the number of arguments. INSTR accepts two parameters. If the
    // number of parameters is not correct issue an exception.

    if (count != 2) 
        throw FdoException::Create(
                FdoException::NLSGetMessage(
                  FUNCTION_PARAMETER_NUMBER_ERROR, 
                  "Expression Engine: Invalid number of parameters for function '%1$ls'",
                  FDO_FUNCTION_INSTR));

    // Next, identify the data type associated with the value to be processed.
    // An exception is issued if the data type does not match any of the ones
    // the function supports

    for (i = 0; i < count; i++) {

      literal_value = literal_values->GetItem(i);
      if (literal_value->GetLiteralValueType() != FdoLiteralValueType_Data)
          throw FdoException::Create(
                 FdoException::NLSGetMessage(
                    FUNCTION_PARAMETER_ERROR, 
                    "Expression Engine: Invalid parameters for function '%1$ls'",
                    FDO_FUNCTION_INSTR));

      data_value = static_cast<FdoDataValue *>(literal_value.p);
      data_type = data_value->GetDataType();
      if (data_type != FdoDataType_String)
          throw FdoException::Create(
                 FdoException::NLSGetMessage(
                   FUNCTION_PARAMETER_DATA_TYPE_ERROR, 
                   "Expression Engine: Invalid parameter data type for function '%1$ls'",
                   FDO_FUNCTION_INSTR));

    }  //  for (i = 0; i < count; i++) ...

}  //  Validate ()

