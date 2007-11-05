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
#include <Functions/String/FdoFunctionLpad.h>


// ----------------------------------------------------------------------------
// --                         Constructors/Destructors                       --
// ----------------------------------------------------------------------------

FdoFunctionLpad::FdoFunctionLpad ()

// +---------------------------------------------------------------------------
// | The class constructor.
// +---------------------------------------------------------------------------

{

    // Initialize all class variables.
    // NOTE: Due to the fact that data type enumeration misses an entry to
    //       indicate a not-set value, the variables "para1_data_type", "para-
    //       "_data_type" and "para3_data_type" are set to "FdoDataType_CLOB"
    //       to indicate an invalid data type because this function does not
    //       support this type. 

    function_definition  = NULL;

    number_of_parameters = 0;

    para1_data_type      = FdoDataType_CLOB;
    para2_data_type      = FdoDataType_CLOB;
    para3_data_type      = FdoDataType_CLOB;

}  //  FdoFunctionLpad ()


FdoFunctionLpad::~FdoFunctionLpad ()

// +---------------------------------------------------------------------------
// | The class destructor.
// +---------------------------------------------------------------------------

{

    // Delete the function definition.

    FDO_SAFE_RELEASE(function_definition);

}  //  ~FdoFunctionLpad ()


// ----------------------------------------------------------------------------
// --                            Public Class APIs                           --
// ----------------------------------------------------------------------------

FdoFunctionLpad *FdoFunctionLpad::Create ()

// +---------------------------------------------------------------------------
// | The function creates a new instance of the class.
// +---------------------------------------------------------------------------

{

    return new FdoFunctionLpad();

}  //  Create ()

FdoFunctionLpad *FdoFunctionLpad::CreateObject ()

// +---------------------------------------------------------------------------
// | The function creates a new instance of the class.
// +---------------------------------------------------------------------------

{

    return new FdoFunctionLpad();

}  //  CreateObject ()

FdoFunctionDefinition *FdoFunctionLpad::GetFunctionDefinition ()

// +---------------------------------------------------------------------------
// | The function creates the supported signature list for the function LPAD.
// +---------------------------------------------------------------------------

{

    if (function_definition == NULL)
        CreateFunctionDefinition();

    return FDO_SAFE_ADDREF(function_definition);

}  //  GetFunctionDefinition ()

FdoLiteralValue *FdoFunctionLpad::Evaluate (
                                    FdoLiteralValueCollection *literal_values)

// +---------------------------------------------------------------------------
// | The function processes a call to the function LPAD.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    bool                   is_NULL_value                = false;

    FdoInt64               string_length,
                           pad_string_length            = 0,
                           desired_string_length,
                           current_pad_string_length    = 0,
                           necessary_pad_string_length;

    FdoStringP             base_string,
                           pad_string,
                           current_pad_string;

    FdoPtr<FdoStringValue> string_value;

    // Validate the function call.

    Validate(literal_values);

    // Get the string that needs to be padded. If no value is provided, termi-
    // nate the function.

    string_value = (FdoStringValue *) literal_values->GetItem(0);
    if (string_value->IsNull())
        return FdoStringValue::Create();
    else
      base_string = base_string + string_value->GetString();
    string_length = base_string.GetLength();

    // Next, get the desired string length. If no length is provided, it is
    // assumed that no padding needs to be done and the current string is re-
    // turned as a result.

    desired_string_length =
            GetPaddingLength(literal_values, para2_data_type, &is_NULL_value);
    if (is_NULL_value)
        return FdoStringValue::Create(base_string);

    // If the desired string length corresonds to the actual string length,
    // return the current string back to the calling routine.

    if (desired_string_length == string_length)
        return FdoStringValue::Create(base_string);

    // If the desired string length is 0, return a NULL string back to the
    // calling routine.

    if (desired_string_length <= 0)
        return FdoStringValue::Create();

    // If the desired length is less than the current string length, the pro-
    // vided string needs to be cut in size and the resulting string returned
    // to the calling routine.

    if (desired_string_length < string_length)
        return FdoStringValue::Create(
                            base_string.Mid(0,
                                            (size_t)desired_string_length));

    // The provided string needs to be padded. If three arguments were provided
    // the last argument represent the pad-string to be used. If the string is
    // a NULL value, use the standard padding string (a blank) instead.

    if (number_of_parameters == 3) {

        string_value = (FdoStringValue *) literal_values->GetItem(2);
        if (string_value->IsNull())
            pad_string = pad_string + L" ";
        else
          pad_string = pad_string + string_value->GetString();

    }  //  if (number_of_parameters == 3) ...
    else
      pad_string = pad_string + L" ";

    // Get the length of the pad-string being used in the operation.

    pad_string_length = pad_string.GetLength();

    // Extend the pad-string to the necessary length.

    necessary_pad_string_length = (desired_string_length - string_length);
    while (current_pad_string.GetLength() < necessary_pad_string_length)
      current_pad_string = current_pad_string + pad_string;

    // The pad-string is created. Note that if the provided pad-string consists
    // of multiple characters, it is possible that the generated pad-string is
    // too long and hence needs to be cut to the necessary size.

    if (current_pad_string.GetLength() > necessary_pad_string_length)
        current_pad_string =
            current_pad_string.Mid(0, (size_t) necessary_pad_string_length);

    // Create the resulting string and return it back to the calling routine.

    return FdoStringValue::Create(current_pad_string + base_string);

}  //  Evaluate ()


// ----------------------------------------------------------------------------
// --                          Supporting functions                          --
// ----------------------------------------------------------------------------

void FdoFunctionLpad::CreateFunctionDefinition ()

// +---------------------------------------------------------------------------
// | The procedure creates the function definition for the function LPAD. The
// | function definition includes the list of supported signatures. The follow-
// | ing signatures are supported:
// |
// |    LPAD (string, 
// |          {byte, decimal, double, int16, int32, int64, single}
// |          [, string] )
// |
// | The function always returns a STRING.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoString                               *desc               = NULL;

    FdoStringP                              arg1_description;
    FdoStringP                              arg2_description;
    FdoStringP                              arg3_description;
    FdoStringP                              str_arg_literal;
    FdoStringP                              length_arg_literal;
    FdoStringP                              pad_str_arg_literal;

    FdoPtr<FdoArgumentDefinition>           byte_arg;
    FdoPtr<FdoArgumentDefinition>           dcl_arg;
    FdoPtr<FdoArgumentDefinition>           dbl_arg;
    FdoPtr<FdoArgumentDefinition>           int16_arg;
    FdoPtr<FdoArgumentDefinition>           int32_arg;
    FdoPtr<FdoArgumentDefinition>           int64_arg;
    FdoPtr<FdoArgumentDefinition>           sgl_arg;
    FdoPtr<FdoArgumentDefinition>           str_arg;
    FdoPtr<FdoArgumentDefinition>           str_opt_arg;

    FdoPtr<FdoArgumentDefinitionCollection> str_byte_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_dcl_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_dbl_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_int16_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_int32_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_int64_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_sgl_args;

    FdoPtr<FdoArgumentDefinitionCollection> str_byte_str_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_dcl_str_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_dbl_str_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_int16_str_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_int32_str_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_int64_str_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_sgl_str_args;

    FdoPtr<FdoSignatureDefinition>          signature;
    FdoSignatureDefinitionCollection        *signatures;

    // Get the general descriptions for the arguments.

    arg1_description =
                   FdoException::NLSGetMessage(FUNCTION_LPAD_STRING_ARG,
                                               "String to pad to the left");

    arg2_description =
            FdoException::NLSGetMessage(
                            FUNCTION_PAD_LENGTH_ARG,
                            "The length the string should be padded to");

    arg3_description =
                   FdoException::NLSGetMessage(FUNCTION_PAD_STRING_ARG,
                                               "The string to be used to pad");

    // The following defines the different argument definition collections.

    str_arg_literal = FdoException::NLSGetMessage(FUNCTION_STRING_ARG_LIT,
                                                  "text property");

    length_arg_literal =
                    FdoException::NLSGetMessage(FUNCTION_PAD_LENGTH_ARG_LIT,
                                                "number of characters");

    pad_str_arg_literal =
                    FdoException::NLSGetMessage(FUNCTION_PAD_STRING_ARG_LIT,
                                                "pad string");

    str_arg   = FdoArgumentDefinition::Create(
                    str_arg_literal, arg1_description, FdoDataType_String);

    byte_arg  = FdoArgumentDefinition::Create(
                    length_arg_literal, arg2_description, FdoDataType_Byte);
    dcl_arg   = FdoArgumentDefinition::Create(
                    length_arg_literal, arg2_description, FdoDataType_Decimal);
    dbl_arg   = FdoArgumentDefinition::Create(
                    length_arg_literal, arg2_description, FdoDataType_Double);
    int16_arg = FdoArgumentDefinition::Create(
                    length_arg_literal, arg2_description, FdoDataType_Int16);
    int32_arg = FdoArgumentDefinition::Create(
                    length_arg_literal, arg2_description, FdoDataType_Int32);
    int64_arg = FdoArgumentDefinition::Create(
                    length_arg_literal, arg2_description, FdoDataType_Int64);
    sgl_arg   = FdoArgumentDefinition::Create(
                    length_arg_literal, arg2_description, FdoDataType_Single);

    str_opt_arg =
            FdoArgumentDefinition::Create(
                pad_str_arg_literal, arg3_description, FdoDataType_String);

    str_byte_args = FdoArgumentDefinitionCollection::Create();
    str_byte_args->Add(str_arg);
    str_byte_args->Add(byte_arg);

    str_dcl_args = FdoArgumentDefinitionCollection::Create();
    str_dcl_args->Add(str_arg);
    str_dcl_args->Add(dcl_arg);

    str_dbl_args = FdoArgumentDefinitionCollection::Create();
    str_dbl_args->Add(str_arg);
    str_dbl_args->Add(dbl_arg);

    str_int16_args = FdoArgumentDefinitionCollection::Create();
    str_int16_args->Add(str_arg);
    str_int16_args->Add(int16_arg);

    str_int32_args = FdoArgumentDefinitionCollection::Create();
    str_int32_args->Add(str_arg);
    str_int32_args->Add(int32_arg);

    str_int64_args = FdoArgumentDefinitionCollection::Create();
    str_int64_args->Add(str_arg);
    str_int64_args->Add(int64_arg);

    str_sgl_args = FdoArgumentDefinitionCollection::Create();
    str_sgl_args->Add(str_arg);
    str_sgl_args->Add(sgl_arg);

    str_byte_str_args = FdoArgumentDefinitionCollection::Create();
    str_byte_str_args->Add(str_arg);
    str_byte_str_args->Add(byte_arg);
    str_byte_str_args->Add(str_opt_arg);

    str_dcl_str_args = FdoArgumentDefinitionCollection::Create();
    str_dcl_str_args->Add(str_arg);
    str_dcl_str_args->Add(dcl_arg);
    str_dcl_str_args->Add(str_opt_arg);

    str_dbl_str_args = FdoArgumentDefinitionCollection::Create();
    str_dbl_str_args->Add(str_arg);
    str_dbl_str_args->Add(dbl_arg);
    str_dbl_str_args->Add(str_opt_arg);

    str_int16_str_args = FdoArgumentDefinitionCollection::Create();
    str_int16_str_args->Add(str_arg);
    str_int16_str_args->Add(int16_arg);
    str_int16_str_args->Add(str_opt_arg);

    str_int32_str_args = FdoArgumentDefinitionCollection::Create();
    str_int32_str_args->Add(str_arg);
    str_int32_str_args->Add(int32_arg);
    str_int32_str_args->Add(str_opt_arg);

    str_int64_str_args = FdoArgumentDefinitionCollection::Create();
    str_int64_str_args->Add(str_arg);
    str_int64_str_args->Add(int64_arg);
    str_int64_str_args->Add(str_opt_arg);

    str_sgl_str_args = FdoArgumentDefinitionCollection::Create();
    str_sgl_str_args->Add(str_arg);
    str_sgl_str_args->Add(sgl_arg);
    str_sgl_str_args->Add(str_opt_arg);

    // Create the signature collection.

    signatures = FdoSignatureDefinitionCollection::Create();

    signature = FdoSignatureDefinition::Create(
                                    FdoDataType_String, str_byte_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                    FdoDataType_String, str_dcl_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                    FdoDataType_String, str_dbl_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                    FdoDataType_String, str_int16_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                    FdoDataType_String, str_int32_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                    FdoDataType_String, str_int64_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                    FdoDataType_String, str_sgl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                    FdoDataType_String, str_byte_str_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                    FdoDataType_String, str_dcl_str_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                    FdoDataType_String, str_dbl_str_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                    FdoDataType_String, str_int16_str_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                    FdoDataType_String, str_int32_str_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                    FdoDataType_String, str_int64_str_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                    FdoDataType_String, str_sgl_str_args);
    signatures->Add(signature);

    // Create the function definition.

    desc =
         FdoException::NLSGetMessage(
                FUNCTION_LPAD,
                "Pads a string expression as directed to the left");
    function_definition =
                FdoFunctionDefinition::Create(
                                        FDO_FUNCTION_LPAD,
                                        desc,
                                        false,
                                        signatures,
                                        FdoFunctionCategoryType_String);

}  //  CreateFunctionDefinition ()

FdoInt64 FdoFunctionLpad::GetPaddingLength (
                                FdoLiteralValueCollection *literal_values,
                                FdoDataType               data_type,
                                bool                      *is_NULL_value)

// +---------------------------------------------------------------------------
// | The function returns the value of the second function parameter.
// +---------------------------------------------------------------------------

{

    // Declare all necessary local variables and initialize them.

    FdoPtr<FdoByteValue>    byte_value;
    FdoPtr<FdoDecimalValue> decimal_value;
    FdoPtr<FdoDoubleValue>  double_value;
    FdoPtr<FdoInt16Value>   int16_value;
    FdoPtr<FdoInt32Value>   int32_value;
    FdoPtr<FdoInt64Value>   int64_value;
    FdoPtr<FdoSingleValue>  single_value;

    // Process the identified parameter and return its value back to the
    // calling routine.

    switch (data_type) {

      case FdoDataType_Byte:
        byte_value =(FdoByteValue *) literal_values->GetItem(1);
        if (!byte_value->IsNull())
            return (FdoInt64) (byte_value->GetByte());
        else {

          *is_NULL_value = true;
          return 0;

        }  //  else ...
        break;

      case FdoDataType_Decimal:
        decimal_value =(FdoDecimalValue *) literal_values->GetItem(1);
        if (!decimal_value->IsNull())
            return (FdoInt64) (floor(decimal_value->GetDecimal()));
        else {

          *is_NULL_value = true;
          return 0;

        }  //  else ...
        break;

      case FdoDataType_Double:
        double_value = (FdoDoubleValue *) literal_values->GetItem(1);
        if (!double_value->IsNull())
            return (FdoInt64) (floor(double_value->GetDouble()));
        else {

          *is_NULL_value = true;
          return 0;

        }  //  else ...
        break;

      case FdoDataType_Int16:
        int16_value = (FdoInt16Value *) literal_values->GetItem(1);
        if (!int16_value->IsNull())
            return (FdoInt64) (int16_value->GetInt16());
        else {

          *is_NULL_value = true;
          return 0;

        }  //  else ...
        break;

      case FdoDataType_Int32:
        int32_value = (FdoInt32Value *) literal_values->GetItem(1);
        if (!int32_value->IsNull())
            return (FdoInt64) (int32_value->GetInt32());
        else {

          *is_NULL_value = true;
          return 0;

        }  //  else ...
        break;

      case FdoDataType_Int64:
        int64_value = (FdoInt64Value *) literal_values->GetItem(1);
        if (!int64_value->IsNull())
            return int64_value->GetInt64();
        else {

          *is_NULL_value = true;
          return 0;

        }  //  else ...
        break;

      case FdoDataType_Single:
        single_value = (FdoSingleValue *) literal_values->GetItem(1);
        if (!single_value->IsNull())
            return (FdoInt64) (floor(single_value->GetSingle()));
        else {

          *is_NULL_value = true;
          return 0;

        }  //  else ...
        break;

    }  //  switch ...

    // The switch statement should handle all valid scenarios. Therefore, the
    // above statements should have returned valid information to the calling
    // routine. Just in case there is an issue, the following exception is
    // thrown if this part of the function implementation is reached.

    throw FdoException::Create(
            FdoException::NLSGetMessage(
              FUNCTION_UNEXPECTED_RESULT_ERROR, 
              "Expression Engine: Unexpected result for function '%1$ls'",
              FDO_FUNCTION_LPAD));

}  //  GetPaddingLength ()

void FdoFunctionLpad::Validate (FdoLiteralValueCollection *literal_values)

// +---------------------------------------------------------------------------
// | The function validates the argument list that was passed in.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoInt32                i               = 0;

    FdoDataValue            *data_value     = NULL;

    FdoPtr<FdoLiteralValue> literal_value;

    // Check the number of arguments. LPAD accepts either two or three para-
    // meters. If the number of parameters is not correct issue an exception.

    number_of_parameters = literal_values->GetCount();

    if ((number_of_parameters < 2) || (number_of_parameters > 3)) 
        throw FdoException::Create(
               FdoException::NLSGetMessage(
                  FUNCTION_PARAMETER_NUMBER_ERROR, 
                  "Expression Engine: Invalid number of parameters for function '%1$ls'",
                  FDO_FUNCTION_LPAD));

    // Next, navigate through the parameter list and get the data type for each
    // of the provided arguments.

    for (i = 0; i < number_of_parameters; i++) {

      literal_value = literal_values->GetItem(i);
      if (literal_value->GetLiteralValueType() != FdoLiteralValueType_Data)
          throw FdoException::Create(
                FdoException::NLSGetMessage(
                  FUNCTION_PARAMETER_ERROR, 
                  "Expression Engine: Invalid parameters for function '%1$ls'",
                  FDO_FUNCTION_LPAD));

      data_value = static_cast<FdoDataValue *>(literal_value.p);
      switch (i) {

        case 0: para1_data_type = data_value->GetDataType(); break;
        case 1: para2_data_type = data_value->GetDataType(); break;
        case 2: para3_data_type = data_value->GetDataType(); break;

      }  //  switch ...

    }  //  for (i = 0; ...

    // Verify that the provided data types match the ones defined in the func-
    // tion signatures. If this is not the case issue an exception.

    if ((para1_data_type  != FdoDataType_String      ) ||
        ((para2_data_type != FdoDataType_Byte   ) &&
         (para2_data_type != FdoDataType_Decimal) &&
         (para2_data_type != FdoDataType_Double ) &&
         (para2_data_type != FdoDataType_Int16  ) &&
         (para2_data_type != FdoDataType_Int32  ) &&
         (para2_data_type != FdoDataType_Int64  ) &&
         (para2_data_type != FdoDataType_Single )    )    )
        throw FdoException::Create(
                FdoException::NLSGetMessage(
                  FUNCTION_PARAMETER_DATA_TYPE_ERROR, 
                  "Expression Engine: Invalid parameter data type for function '%1$ls'",
                  FDO_FUNCTION_LPAD));

    if (number_of_parameters == 3)
        if (para3_data_type != FdoDataType_String)
            throw FdoException::Create(
                FdoException::NLSGetMessage(
                  FUNCTION_PARAMETER_DATA_TYPE_ERROR, 
                  "Expression Engine: Invalid parameter data type for function '%1$ls'",
                  FDO_FUNCTION_LPAD));

}  //  Validate ()

