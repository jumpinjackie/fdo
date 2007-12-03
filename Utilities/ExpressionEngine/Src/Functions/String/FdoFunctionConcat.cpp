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
#include <Functions/String/FdoFunctionConcat.h>


// ----------------------------------------------------------------------------
// --                         Constructors/Destructors                       --
// ----------------------------------------------------------------------------

FdoFunctionConcat::FdoFunctionConcat ()

// +---------------------------------------------------------------------------
// | The class constructor.
// +---------------------------------------------------------------------------

{

    // Initialize all class variables.

    function_definition = NULL;
    m_first = true;
    m_temp_buffer = NULL;

}  //  FdoFunctionConcat ()


FdoFunctionConcat::~FdoFunctionConcat ()

// +---------------------------------------------------------------------------
// | The class destructor.
// +---------------------------------------------------------------------------

{

    // Delete the function definition.

    FDO_SAFE_RELEASE(function_definition);

    delete [] m_temp_buffer;

}  //  ~FdoFunctionConcat ()


// ----------------------------------------------------------------------------
// --                            Public Class APIs                           --
// ----------------------------------------------------------------------------

FdoFunctionConcat *FdoFunctionConcat::Create ()

// +---------------------------------------------------------------------------
// | The function creates a new instance of the class.
// +---------------------------------------------------------------------------

{

    return new FdoFunctionConcat();

}  //  Create ()

FdoFunctionConcat *FdoFunctionConcat::CreateObject ()

// +---------------------------------------------------------------------------
// | The function creates a new instance of the class.
// +---------------------------------------------------------------------------

{

    return new FdoFunctionConcat();

}  //  CreateObject ()

FdoFunctionDefinition *FdoFunctionConcat::GetFunctionDefinition ()

// +---------------------------------------------------------------------------
// | The function creates the supported signature list for the function CONCAT.
// +---------------------------------------------------------------------------

{

    if (function_definition == NULL)
        CreateFunctionDefinition();

    return FDO_SAFE_ADDREF(function_definition);

}  //  GetFunctionDefinition ()

FdoLiteralValue *FdoFunctionConcat::Evaluate (
                                    FdoLiteralValueCollection *literal_values)

// +---------------------------------------------------------------------------
// | The function processes a call to the function CONCAT.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoPtr<FdoStringValue> string_value1;
    FdoPtr<FdoStringValue> string_value2;

    // Validate the function call.
    if (m_first == true)
    {
        Validate(literal_values);
        m_string_value = FdoStringValue::Create();
        m_temp_buffer = new wchar_t[INIT_ALLOCATE_SIZE+1];
        m_size = INIT_ALLOCATE_SIZE;
        m_first = false;
    }

    // Process the request and return the result back to the calling routine.

    string_value1 = (FdoStringValue *) literal_values->GetItem(0);
    string_value2 = (FdoStringValue *) literal_values->GetItem(1);

    size_t size = 0;
    FdoString *string1 = NULL;
    FdoString *string2 = NULL;
    if (!string_value1->IsNull())
    {
        string1 = string_value1->GetString();
        size = wcslen(string1);
    }

    if (!string_value2->IsNull())
    {
        string2 = string_value2->GetString();
        size += wcslen(string2);
    }

    if (size > m_size)
    {
        delete [] m_temp_buffer;
        m_size = size;
        m_temp_buffer = new wchar_t[m_size + 1];
    }

    m_temp_buffer[0] = '\0';

    if (string1)
        wcscpy(m_temp_buffer, string1);

    if (string2)
        wcscat(m_temp_buffer, string2);

    m_string_value->SetString(m_temp_buffer);
    return FDO_SAFE_ADDREF(m_string_value.p);

}  //  Evaluate ()


// ----------------------------------------------------------------------------
// --                          Supporting functions                          --
// ----------------------------------------------------------------------------

void FdoFunctionConcat::CreateFunctionDefinition ()

// +---------------------------------------------------------------------------
// | The procedure creates the function definition for the function CONCAT. The
// | function definition includes the list of supported signatures. The follow-
// | ing signatures are supported:
// |
// |    CONCAT (string, string)
// |
// | The function always returns a STRING.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoString                               *desc               = NULL;

    FdoStringP                              arg1_description;
    FdoStringP                              arg2_description;
    FdoStringP                              str_arg_literal;

    FdoPtr<FdoArgumentDefinition>           str1_arg;
    FdoPtr<FdoArgumentDefinition>           str2_arg;

    FdoPtr<FdoArgumentDefinitionCollection> str_args;

    FdoPtr<FdoSignatureDefinition>          signature;
    FdoSignatureDefinitionCollection        *signatures;

    // Get the general descriptions for the arguments.

    arg1_description = FdoException::NLSGetMessage(
                                        FUNCTION_CONCAT_STRING1_ARG,
                                        "First string to concatenate");

    arg2_description = FdoException::NLSGetMessage(
                                        FUNCTION_CONCAT_STRING2_ARG,
                                        "Second string to concatenate");

    // The following defines the different argument definition collections.

    str_arg_literal = FdoException::NLSGetMessage(FUNCTION_STRING_ARG_LIT,
                                                  "text property");

    str1_arg = FdoArgumentDefinition::Create(
                    str_arg_literal, arg1_description, FdoDataType_String);
    str2_arg = FdoArgumentDefinition::Create(
                    str_arg_literal, arg2_description, FdoDataType_String);

    str_args = FdoArgumentDefinitionCollection::Create();
    str_args->Add(str1_arg);
    str_args->Add(str2_arg);

    // Create the signature collection.

    signatures = FdoSignatureDefinitionCollection::Create();

    signature = FdoSignatureDefinition::Create(FdoDataType_String, str_args);
    signatures->Add(signature);

    // Create the function definition.

    desc = FdoException::NLSGetMessage(
                        FUNCTION_CONCAT,
                        "Returns the string concatenation of 2 expressions");
    function_definition =
                FdoFunctionDefinition::Create(
                                        FDO_FUNCTION_CONCAT,
                                        desc,
                                        false,
                                        signatures,
                                        FdoFunctionCategoryType_String);

}  //  CreateFunctionDefinition ()

void FdoFunctionConcat::Validate (FdoLiteralValueCollection *literal_values)

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

    // Check the number of arguments. CONCAT accepts two parameters. If the
    // number of parameters is not correct issue an exception.

    if (count != 2) 
        throw FdoException::Create(
                FdoException::NLSGetMessage(
                  FUNCTION_PARAMETER_NUMBER_ERROR, 
                  "Expression Engine: Invalid number of parameters for function '%1$ls'",
                  FDO_FUNCTION_CONCAT));

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
                    FDO_FUNCTION_CONCAT));

      data_value = static_cast<FdoDataValue *>(literal_value.p);
      data_type = data_value->GetDataType();
      if (data_type != FdoDataType_String)
          throw FdoException::Create(
                 FdoException::NLSGetMessage(
                   FUNCTION_PARAMETER_DATA_TYPE_ERROR, 
                   "Expression Engine: Invalid parameter data type for function '%1$ls'",
                   FDO_FUNCTION_CONCAT));

    }  //  for (i = 0; i < count; i++) ...

}  //  Validate ()

