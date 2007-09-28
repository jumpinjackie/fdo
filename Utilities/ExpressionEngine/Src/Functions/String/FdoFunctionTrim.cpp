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
#include <Functions/String/FdoFunctionTrim.h>
#include <FdoCommonStringUtil.h>


// ----------------------------------------------------------------------------
// --                         Constructors/Destructors                       --
// ----------------------------------------------------------------------------

FdoFunctionTrim::FdoFunctionTrim ()

// +---------------------------------------------------------------------------
// | The class constructor.
// +---------------------------------------------------------------------------

{

    // Initialize all class variables.

    function_definition = NULL;

}  //  FdoFunctionTrim ()


FdoFunctionTrim::~FdoFunctionTrim ()

// +---------------------------------------------------------------------------
// | The class destructor.
// +---------------------------------------------------------------------------

{

    // Delete the function definition.

    FDO_SAFE_RELEASE(function_definition);

}  //  ~FdoFunctionTrim ()


// ----------------------------------------------------------------------------
// --                            Public Class APIs                           --
// ----------------------------------------------------------------------------

FdoFunctionTrim *FdoFunctionTrim::Create ()

// +---------------------------------------------------------------------------
// | The function creates a new instance of the class.
// +---------------------------------------------------------------------------

{

    return new FdoFunctionTrim();

}  //  Create ()

FdoFunctionTrim *FdoFunctionTrim::CreateObject ()

// +---------------------------------------------------------------------------
// | The function creates a new instance of the class.
// +---------------------------------------------------------------------------

{

    return new FdoFunctionTrim();

}  //  CreateObject ()

FdoFunctionDefinition *FdoFunctionTrim::GetFunctionDefinition ()

// +---------------------------------------------------------------------------
// | The function creates the supported signature list for the function TRIM.
// +---------------------------------------------------------------------------

{

    if (function_definition == NULL)
        CreateFunctionDefinition();

    return FDO_SAFE_ADDREF(function_definition);

}  //  GetFunctionDefinition ()

FdoLiteralValue *FdoFunctionTrim::Evaluate (
                                    FdoLiteralValueCollection *literal_values)

// +---------------------------------------------------------------------------
// | The function processes a call to the function TRIM.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    bool                   endloop          = false;

    FdoInt64               pos              = 0,
                           string_length;

    FdoString              *curr_char       = NULL;

    FdoStringP             tmp_str,
                           base_string;

    FdoPtr<FdoStringValue> string_value;

    // Validate the function call.

    Validate(literal_values);

    // Get the string that needs to be trimmed. If no value is provided, termi-
    // nate the function.

    string_value = (literal_values->GetCount() == 2)
                 ? (FdoStringValue *) literal_values->GetItem(1)
                 : (FdoStringValue *) literal_values->GetItem(0);
    if (string_value->IsNull())
        return FdoStringValue::Create();
    else
      base_string = base_string + string_value->GetString();
    string_length = base_string.GetLength();
    if (string_length == 0)
        return FdoStringValue::Create();

    // Process the string. If requested, process leading blanks first.

    if ((FdoCommonStringUtil::StringCompareNoCase(
                        function_operation_request, L"BOTH"   ) == 0) ||
        (FdoCommonStringUtil::StringCompareNoCase(
                        function_operation_request, L"LEADING") == 0)    ) {

        // Navigate the given string from the left and find the first character
        // different from a blank. 

        curr_char = (FdoString *)base_string;

        while (pos < string_length) {

          if (curr_char[pos] != ' ')
              break;
          pos++;

        }  //  while (pos < string_length) ...

        // If no character other than blanks were found return an empty string.
        // Otherwise get the part of the original string without the leading
        // blanks.

        if (pos == string_length)
            return FdoStringValue::Create();
        else
          base_string = base_string.Mid((size_t) pos, (size_t) string_length);

    }  //  if ((FdoCommonStringUtil::StringCompareNoCase( ...

    // If requested, process trailing blanks next.

    if ((FdoCommonStringUtil::StringCompareNoCase(
                        function_operation_request, L"BOTH"    ) == 0) ||
        (FdoCommonStringUtil::StringCompareNoCase(
                        function_operation_request, L"TRAILING") == 0)    ) {

        // Navigate the given string from the right and find the first
        // character different from a blank. 

        string_length = base_string.GetLength();
        pos       = string_length - 1;

        while (!endloop) {

          tmp_str = base_string.Mid((size_t) pos, 1);
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
            return FdoStringValue::Create();
        else
          return FdoStringValue::Create(base_string.Mid(0, (size_t) (pos+1)));

    }  //  if ((FdoCommonStringUtil::StringCompareNoCase( ...

    // Return the function result.

    return FdoStringValue::Create(base_string);

}  //  Evaluate ()


// ----------------------------------------------------------------------------
// --                          Supporting functions                          --
// ----------------------------------------------------------------------------

void FdoFunctionTrim::CreateFunctionDefinition ()

// +---------------------------------------------------------------------------
// | The procedure creates the function definition for the function TRIM. The
// | function definition includes the list of supported signatures. The follow-
// | ing signatures are supported:
// |
// |    TRIM ([{BOTH | LEADING | TRAILING}, ] string)
// |
// | The function always returns a STRING.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoString *desc = NULL;

    FdoStringP arg1_description;
    FdoStringP arg2_description;
    FdoStringP str_arg_literal;
    FdoStringP opt_arg_literal;

    FdoPtr<FdoArgumentDefinition> str_arg;
    FdoPtr<FdoArgumentDefinition> opt_arg;

    FdoPtr<FdoArgumentDefinitionCollection> str_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_opt_args;

    FdoPtr<FdoDataValueCollection>          values;
    FdoPtr<FdoPropertyValueConstraintList>  argument_value_list;

    FdoPtr<FdoSignatureDefinition>          signature;
    FdoSignatureDefinitionCollection        *signatures;

    // The function contains signatures that allow an optional parameter. If
    // used the value of this parameter must be either BOTH, LEADING or TRAIL-
    // ING. The following sets up an argument value list to be used for the
    // argument describing this parameter.

    argument_value_list = FdoPropertyValueConstraintList::Create();
    values              = argument_value_list->GetConstraintList();
    values->Add(FdoStringValue::Create(L"BOTH"));
    values->Add(FdoStringValue::Create(L"LEADING"));
    values->Add(FdoStringValue::Create(L"TRAILING"));

    // Get the general descriptions for the arguments.

    arg1_description = FdoException::NLSGetMessage(FUNCTION_TRIM_STRING_ARG,
                                                   "String to trim");

    arg2_description =
            FdoException::NLSGetMessage(
                                FUNCTION_TRIM_OP_ARG,
                                "Trim indicator (BOTH, LEADING, TRAILING)");

    // The following defines the different argument definition collections.

    str_arg_literal = FdoException::NLSGetMessage(FUNCTION_STRING_ARG_LIT,
                                                  "text property");

    opt_arg_literal = FdoException::NLSGetMessage(FUNCTION_OPERATOR_ARG_LIT,
                                                  "Operation Indicator");

    str_arg   = FdoArgumentDefinition::Create(
                    str_arg_literal, arg1_description, FdoDataType_String);

    opt_arg   = FdoArgumentDefinition::Create(
                    opt_arg_literal, arg2_description, FdoDataType_String);
    opt_arg->SetArgumentValueList(argument_value_list);

    str_args = FdoArgumentDefinitionCollection::Create();
    str_args->Add(str_arg);

    str_opt_args = FdoArgumentDefinitionCollection::Create();
    str_opt_args->Add(opt_arg);
    str_opt_args->Add(str_arg);

    // Create the signature collection.

    signatures = FdoSignatureDefinitionCollection::Create();

    signature = FdoSignatureDefinition::Create(FdoDataType_String, str_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(FdoDataType_String, str_opt_args);
    signatures->Add(signature);

    // Create the function definition.

    desc = FdoException::NLSGetMessage(FUNCTION_TRIM,
                                       "Trims a string expression");
    function_definition =
                FdoFunctionDefinition::Create(
                                        FDO_FUNCTION_TRIM,
                                        desc,
                                        false,
                                        signatures,
                                        FdoFunctionCategoryType_String);

}  //  CreateFunctionDefinition ()

void FdoFunctionTrim::Validate (FdoLiteralValueCollection *literal_values)

// +---------------------------------------------------------------------------
// | The function validates the argument list that was passed in.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoInt32                i,
                            count           = literal_values->GetCount();

    FdoDataType             data_type;

    FdoDataValue            *data_value     = NULL;

    FdoStringValue          *str_value      = NULL;

    FdoPtr<FdoLiteralValue> literal_value;

    // Check the number of arguments. TRIM accepts either one or two para-
    // meters. If the number of parameters is not correct issue an exception.

    count = literal_values->GetCount();

    if ((count < 1) || (count > 2)) 
        throw FdoException::Create(
               FdoException::NLSGetMessage(
                  FUNCTION_PARAMETER_NUMBER_ERROR, 
                  "Expression Engine: Invalid number of parameters for function '%1$ls'",
                  FDO_FUNCTION_TRIM));

    // Loop through the arguments and verify that all arguments are valid.

    for (i = 0; i < count; i++) {

      literal_value = literal_values->GetItem(i);
      if (literal_value->GetLiteralValueType() != FdoLiteralValueType_Data)
          throw FdoException::Create(
                FdoException::NLSGetMessage(
                  FUNCTION_PARAMETER_ERROR, 
                  "Expression Engine: Invalid parameters for function '%1$ls'",
                  FDO_FUNCTION_TRIM));

      data_value = static_cast<FdoDataValue *>(literal_value.p);
      data_type = data_value->GetDataType();
      if (data_type != FdoDataType_String)
          throw FdoException::Create(
                FdoException::NLSGetMessage(
                  FUNCTION_PARAMETER_DATA_TYPE_ERROR, 
                  "Expression Engine: Invalid parameter data type for function '%1$ls'",
                  FDO_FUNCTION_TRIM));

      // If this is the first argument and the number of arguments is 2, check
      // whether or not the value is one of BOTH, LEADING or TRAILING. If this
      // is not the case issue an exception. Otherwise remember it for the
      // processing of the request.

      if ((i == 0) && (count == 2)) {

          str_value = static_cast<FdoStringValue *>(literal_value.p);
          if (str_value->IsNull())
              throw FdoException::Create(
                      FdoException::NLSGetMessage(
                      FUNCTION_OPERATOR_ERROR, 
                      "Expression Engine: Invalid operator parameter value for function '%1$ls'",
                      FDO_FUNCTION_TRIM));

          function_operation_request = str_value->GetString();
          if ((FdoCommonStringUtil::StringCompareNoCase(
                            function_operation_request, L"BOTH"    ) != 0) &&
              (FdoCommonStringUtil::StringCompareNoCase(
                            function_operation_request, L"LEADING" ) != 0) &&
              (FdoCommonStringUtil::StringCompareNoCase(
                            function_operation_request, L"TRAILING") != 0)    )
              throw FdoException::Create(
                      FdoException::NLSGetMessage(
                      FUNCTION_OPERATOR_ERROR, 
                      "Expression Engine: Invalid operator parameter value for function '%1$ls'",
                      FDO_FUNCTION_TRIM));

      }  //  if ((i == 0) && ...

    }  //  for (i = 0; ...

    // If only one argument was provided, initialize the operator indicator.

    if (count == 1)
        function_operation_request = L"BOTH";

}  //  Validate ()

