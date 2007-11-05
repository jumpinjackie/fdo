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
#include <Functions/String/FdoFunctionRtrim.h>


// ----------------------------------------------------------------------------
// --                         Constructors/Destructors                       --
// ----------------------------------------------------------------------------

FdoFunctionRtrim::FdoFunctionRtrim ()

// +---------------------------------------------------------------------------
// | The class constructor.
// +---------------------------------------------------------------------------

{

    // Initialize all class variables.

    function_definition = NULL;

}  //  FdoFunctionRtrim ()


FdoFunctionRtrim::~FdoFunctionRtrim ()

// +---------------------------------------------------------------------------
// | The class destructor.
// +---------------------------------------------------------------------------

{

    // Delete the function definition.

    FDO_SAFE_RELEASE(function_definition);

}  //  ~FdoFunctionRtrim ()


// ----------------------------------------------------------------------------
// --                            Public Class APIs                           --
// ----------------------------------------------------------------------------

FdoFunctionRtrim *FdoFunctionRtrim::Create ()

// +---------------------------------------------------------------------------
// | The function creates a new instance of the class.
// +---------------------------------------------------------------------------

{

    return new FdoFunctionRtrim();

}  //  Create ()

FdoFunctionRtrim *FdoFunctionRtrim::CreateObject ()

// +---------------------------------------------------------------------------
// | The function creates a new instance of the class.
// +---------------------------------------------------------------------------

{

    return new FdoFunctionRtrim();

}  //  CreateObject ()

FdoFunctionDefinition *FdoFunctionRtrim::GetFunctionDefinition ()

// +---------------------------------------------------------------------------
// | The function creates the supported signature list for the function RTRIM.
// +---------------------------------------------------------------------------

{

    if (function_definition == NULL)
        CreateFunctionDefinition();

    return FDO_SAFE_ADDREF(function_definition);

}  //  GetFunctionDefinition ()

FdoLiteralValue *FdoFunctionRtrim::Evaluate (
                                    FdoLiteralValueCollection *literal_values)

// +---------------------------------------------------------------------------
// | The function processes a call to the function RTRIM.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    bool                   endloop          = false;

    FdoInt64               pos              = 0,
                           string_length;

    FdoStringP             tmp_str,
                           base_string;

    FdoPtr<FdoStringValue> string_value;

    // Validate the function call.

    Validate(literal_values);

    // Get the string that needs to be trimmed. If no value is provided, termi-
    // nate the function.

    string_value = (FdoStringValue *) literal_values->GetItem(0);
    if (string_value->IsNull())
        return FdoStringValue::Create();
    else
      base_string = base_string + string_value->GetString();
    string_length = base_string.GetLength();
    if (string_length == 0)
        return FdoStringValue::Create();

    // Navigate the given string from the right and find the first character
    // different from a blank. 

    pos = string_length - 1;

    while (!endloop) {

      tmp_str = base_string.Mid((size_t)pos, 1);
      if (tmp_str != L" ")
          break;
       pos--;
       endloop = (pos == -1);

    }  //  while ...

    // If no character other than blanks were found return an empty string.
    // Otherwise return the substring of the provided string starting from
    // the beginning until the first position that identifies a character
    // other than a blank from the right.

    if (pos == -1)
        return FdoStringValue::Create(base_string);
    else
      return FdoStringValue::Create(base_string.Mid(0, (size_t) (pos+1)));

}  //  Evaluate ()


// ----------------------------------------------------------------------------
// --                          Supporting functions                          --
// ----------------------------------------------------------------------------

void FdoFunctionRtrim::CreateFunctionDefinition ()

// +---------------------------------------------------------------------------
// | The procedure creates the function definition for the function RTRIM. The
// | function definition includes the list of supported signatures. The follow-
// | ing signatures are supported:
// |
// |    RTRIM (string)
// |
// | The function always returns a STRING.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoString                               *desc               = NULL;

    FdoStringP                              arg1_description;
    FdoStringP                              str_arg_literal;

    FdoPtr<FdoArgumentDefinition>           str_arg;

    FdoPtr<FdoArgumentDefinitionCollection> str_args;

    FdoPtr<FdoSignatureDefinition>          signature;
    FdoSignatureDefinitionCollection        *signatures;

    // Get the general descriptions for the arguments.

    arg1_description =
                   FdoException::NLSGetMessage(FUNCTION_RTRIM_STRING_ARG,
                                               "String to trim on the right");

    // The following defines the different argument definition collections.

    str_arg_literal = FdoException::NLSGetMessage(FUNCTION_STRING_ARG_LIT,
                                                  "text property");

    str_arg   = FdoArgumentDefinition::Create(
                    str_arg_literal, arg1_description, FdoDataType_String);

    str_args = FdoArgumentDefinitionCollection::Create();
    str_args->Add(str_arg);

    // Create the signature collection.

    signatures = FdoSignatureDefinitionCollection::Create();

    signature = FdoSignatureDefinition::Create(FdoDataType_String, str_args);
    signatures->Add(signature);

    // Create the function definition.

    desc = FdoException::NLSGetMessage(
                                    FUNCTION_RTRIM,
                                    "Trims a string expression on the right");
    function_definition =
                FdoFunctionDefinition::Create(
                                        FDO_FUNCTION_RTRIM,
                                        desc,
                                        false,
                                        signatures,
                                        FdoFunctionCategoryType_String);

}  //  CreateFunctionDefinition ()

void FdoFunctionRtrim::Validate (FdoLiteralValueCollection *literal_values)

// +---------------------------------------------------------------------------
// | The function validates the argument list that was passed in.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoInt32                count           = literal_values->GetCount();

    FdoDataType             data_type;

    FdoDataValue            *data_value     = NULL;

    FdoPtr<FdoLiteralValue> literal_value;

    // Check the number of arguments. RTRIM accepts one parameter only. If the
    // number of parameters is not correct issue an exception.

    count = literal_values->GetCount();

    if (count != 1) 
        throw FdoException::Create(
               FdoException::NLSGetMessage(
                  FUNCTION_PARAMETER_NUMBER_ERROR, 
                  "Expression Engine: Invalid number of parameters for function '%1$ls'",
                  FDO_FUNCTION_RTRIM));

    // Next, identify the data type associated with the value to be processed.
    // An exception is issued if the data type does not match any of the ones
    // the function supports

    literal_value = literal_values->GetItem(0);
    if (literal_value->GetLiteralValueType() != FdoLiteralValueType_Data)
        throw FdoException::Create(
                FdoException::NLSGetMessage(
                  FUNCTION_PARAMETER_ERROR, 
                  "Expression Engine: Invalid parameters for function '%1$ls'",
                  FDO_FUNCTION_RTRIM));

    data_value = static_cast<FdoDataValue *>(literal_value.p);
    data_type = data_value->GetDataType();
    if (data_type != FdoDataType_String)
        throw FdoException::Create(
                FdoException::NLSGetMessage(
                  FUNCTION_PARAMETER_DATA_TYPE_ERROR, 
                  "Expression Engine: Invalid parameter data type for function '%1$ls'",
                  FDO_FUNCTION_RTRIM));

}  //  Validate ()

