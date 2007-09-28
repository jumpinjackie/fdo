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
#include <Functions/Conversion/FdoFunctionToInt32.h>


// ----------------------------------------------------------------------------
// --                         Constructors/Destructors                       --
// ----------------------------------------------------------------------------

FdoFunctionToInt32::FdoFunctionToInt32 ()

// +---------------------------------------------------------------------------
// | The class constructor.
// +---------------------------------------------------------------------------

{

    // Initialize all class variables.

    function_definition = NULL;

}  //  FdoFunctionToInt32 ()

FdoFunctionToInt32::~FdoFunctionToInt32 ()

// +---------------------------------------------------------------------------
// | The class destructor.
// +---------------------------------------------------------------------------

{

    // Delete the function definition.

    FDO_SAFE_RELEASE(function_definition);

}  //  ~FdoFunctionToInt32 ()


// ----------------------------------------------------------------------------
// --                            Public Class APIs                           --
// ----------------------------------------------------------------------------

FdoFunctionToInt32 *FdoFunctionToInt32::Create ()

// +---------------------------------------------------------------------------
// | The function creates a new instance of the class.
// +---------------------------------------------------------------------------

{

    return new FdoFunctionToInt32();

}  //  Create ()

FdoFunctionToInt32 *FdoFunctionToInt32::CreateObject ()

// +---------------------------------------------------------------------------
// | The function creates a new instance of the class.
// +---------------------------------------------------------------------------

{

    return new FdoFunctionToInt32();

}  //  CreateObject ()

FdoFunctionDefinition *FdoFunctionToInt32::GetFunctionDefinition ()

// +---------------------------------------------------------------------------
// | The function creates the supported signature list for the function TOINT32
// +---------------------------------------------------------------------------

{

    if (function_definition == NULL)
        CreateFunctionDefinition();

    return FDO_SAFE_ADDREF(function_definition);

}  //  GetFunctionDefinition ()

FdoLiteralValue *FdoFunctionToInt32::Evaluate (
                                    FdoLiteralValueCollection *literal_values)

// +---------------------------------------------------------------------------
// | The function processes a call to the function TOINT32.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    bool                    is_NULL         = false;

    FdoInt32                i32_value       = 0;

    FdoInt64                i64_value       = 0;

    FdoDouble               dbl_value       = 0;

    FdoStringP              str_value;

    FdoPtr<FdoDecimalValue> decimal_value;
    FdoPtr<FdoDoubleValue>  double_value;
    FdoPtr<FdoInt16Value>   int16_value;
    FdoPtr<FdoInt32Value>   int32_value;
    FdoPtr<FdoInt64Value>   int64_value;
    FdoPtr<FdoSingleValue>  single_value;
    FdoPtr<FdoStringValue>  string_value;

    // Validate the function call.

    Validate(literal_values);

    // Get the parameter and process it.

    switch (incoming_data_type) {

      case FdoDataType_Decimal:
        decimal_value = (FdoDecimalValue *) literal_values->GetItem(0);
        is_NULL       = decimal_value->IsNull();
        if (!is_NULL) {

            dbl_value = decimal_value->GetDecimal();
            if ((dbl_value < INT_MIN) || (dbl_value > INT_MAX))
                throw FdoException::Create(
                        FdoException::NLSGetMessage(
                        FUNCTION_DATA_VALUE_ERROR, 
                        "Expression Engine: Invalid value for execution of function '%1$ls'",
                        FDO_FUNCTION_TOINT32));
            else
              i32_value = (FdoInt32) dbl_value;

        }  //  if (!is_NULL) ...
        break;

      case FdoDataType_Double:
        double_value = (FdoDoubleValue *) literal_values->GetItem(0);
        is_NULL      = double_value->IsNull();
        if (!is_NULL) {

            dbl_value = double_value->GetDouble();
            if ((dbl_value < INT_MIN) || (dbl_value > INT_MAX))
                throw FdoException::Create(
                        FdoException::NLSGetMessage(
                        FUNCTION_DATA_VALUE_ERROR, 
                        "Expression Engine: Invalid value for execution of function '%1$ls'",
                        FDO_FUNCTION_TOINT32));
            else
              i32_value = (FdoInt32) dbl_value;

        }  //  if (!is_NULL) ...
        break;

      case FdoDataType_Int16:
        int16_value = (FdoInt16Value *) literal_values->GetItem(0);
        is_NULL     = int16_value->IsNull();
        if (!is_NULL)
            i32_value = int16_value->GetInt16();
        break;

      case FdoDataType_Int32:
        int32_value = (FdoInt32Value *) literal_values->GetItem(0);
        is_NULL     = int32_value->IsNull();
        if (!is_NULL)
            i32_value = int32_value->GetInt32();
        break;

      case FdoDataType_Int64:
        int64_value = (FdoInt64Value *) literal_values->GetItem(0);
        is_NULL     = int64_value->IsNull();
        if (!is_NULL) {

            i64_value = int64_value->GetInt64();
            if ((i64_value < INT_MIN) || (i64_value > INT_MAX))
                throw FdoException::Create(
                        FdoException::NLSGetMessage(
                        FUNCTION_DATA_VALUE_ERROR, 
                        "Expression Engine: Invalid value for execution of function '%1$ls'",
                        FDO_FUNCTION_TOINT32));
            else
              i32_value = (FdoInt32) i64_value;

        }  //  if (!is_NULL) ...
        break;

      case FdoDataType_Single:
        single_value = (FdoSingleValue *) literal_values->GetItem(0);
        is_NULL      = single_value->IsNull();
        if (!is_NULL) {

            dbl_value = single_value->GetSingle();
            if ((dbl_value < INT_MIN) || (dbl_value > INT_MAX))
                throw FdoException::Create(
                        FdoException::NLSGetMessage(
                        FUNCTION_DATA_VALUE_ERROR, 
                        "Expression Engine: Invalid value for execution of function '%1$ls'",
                        FDO_FUNCTION_TOINT32));
            else
              i32_value = (FdoInt32) dbl_value;

        }  //  if (!is_NULL) {
        break;

      case FdoDataType_String:
        string_value = (FdoStringValue *) literal_values->GetItem(0);
        is_NULL      = string_value->IsNull();
        if (!is_NULL) {

            str_value = string_value->GetString();
            if (str_value.IsNumber())
                dbl_value = str_value.ToDouble();
            else {

              // The current string does not represent a number. This may be
              // the case because of leading and/or trailing blanks. If those
              // are present remove them and try again. If that string still
              // does not represent a number issue an exception.

              str_value = RemoveBlanks(str_value);
              if (str_value.IsNumber())
                  dbl_value = str_value.ToDouble();
              else
                throw FdoException::Create(
                        FdoException::NLSGetMessage(
                        FUNCTION_DATA_VALUE_ERROR, 
                        "Expression Engine: Invalid value for execution of function '%1$ls'",
                        FDO_FUNCTION_TODOUBLE));

            }  //  else ...

            if ((dbl_value < INT_MIN) || (dbl_value > INT_MAX))
                throw FdoException::Create(
                        FdoException::NLSGetMessage(
                        FUNCTION_DATA_VALUE_ERROR, 
                        "Expression Engine: Invalid value for execution of function '%1$ls'",
                        FDO_FUNCTION_TOINT32));
            else
              i32_value = (FdoInt32) dbl_value;

        }  //  if (!is_NULL) ...
        break;

      default:
        throw FdoException::Create(
                FdoException::NLSGetMessage(
                FUNCTION_PARAMETER_DATA_TYPE_ERROR, 
                "Expression Engine: Invalid parameter data type for function '%1$ls'",
                FDO_FUNCTION_TOINT32));
        break;

    }  //  switch ...

    if (is_NULL)
        return FdoInt32Value::Create();
    else
      return FdoInt32Value::Create(i32_value);

}  //  Evaluate ()


// ----------------------------------------------------------------------------
// --                          Supporting functions                          --
// ----------------------------------------------------------------------------

void FdoFunctionToInt32::CreateFunctionDefinition ()

// +---------------------------------------------------------------------------
// | The procedure creates the function definition for the function TOINT32.
// | The function definition includes the list of supported signatures. The
// | following signatures are supported:
// |
// |    TOINT32 ({decimal, double, int16, int32, int64, single, string})
// |
// | The function always returns an INT32.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoString                               *desc               = NULL;

    FdoStringP                              arg1_description;
    FdoStringP                              str_arg_literal;
    FdoStringP                              num_arg_literal;

    FdoPtr<FdoArgumentDefinition>           dcl_arg;
    FdoPtr<FdoArgumentDefinition>           dbl_arg;
    FdoPtr<FdoArgumentDefinition>           int16_arg;
    FdoPtr<FdoArgumentDefinition>           int32_arg;
    FdoPtr<FdoArgumentDefinition>           int64_arg;
    FdoPtr<FdoArgumentDefinition>           sgl_arg;
    FdoPtr<FdoArgumentDefinition>           str_arg;

    FdoPtr<FdoArgumentDefinitionCollection> dcl_args;
    FdoPtr<FdoArgumentDefinitionCollection> dbl_args;
    FdoPtr<FdoArgumentDefinitionCollection> int16_args;
    FdoPtr<FdoArgumentDefinitionCollection> int32_args;
    FdoPtr<FdoArgumentDefinitionCollection> int64_args;
    FdoPtr<FdoArgumentDefinitionCollection> sgl_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_args;

    FdoPtr<FdoSignatureDefinition>          signature;
    FdoSignatureDefinitionCollection        *signatures;

    // Get the general descriptions for the arguments.

    arg1_description = FdoException::NLSGetMessage(
                                                FUNCTION_GENERAL_ARG,
                                                "Argument to be processed");

    num_arg_literal  = FdoException::NLSGetMessage(FUNCTION_NUMBER_ARG_LIT,
                                                   "number");

    str_arg_literal  = FdoException::NLSGetMessage(
                                    FUNCTION_STRING_ARG_LIT, "text property");

    // The following defines the different argument definition collections.

    dcl_arg   = FdoArgumentDefinition::Create(
                    num_arg_literal, arg1_description, FdoDataType_Decimal);

    dbl_arg   = FdoArgumentDefinition::Create(
                    num_arg_literal, arg1_description, FdoDataType_Double);

    int16_arg = FdoArgumentDefinition::Create(
                    num_arg_literal, arg1_description, FdoDataType_Int16);

    int32_arg = FdoArgumentDefinition::Create(
                    num_arg_literal, arg1_description, FdoDataType_Int32);

    int64_arg = FdoArgumentDefinition::Create(
                    num_arg_literal, arg1_description, FdoDataType_Int64);

    sgl_arg   = FdoArgumentDefinition::Create(
                    num_arg_literal, arg1_description, FdoDataType_Single);

    str_arg  = FdoArgumentDefinition::Create(
                    str_arg_literal, arg1_description, FdoDataType_String);

    dcl_args = FdoArgumentDefinitionCollection::Create();
    dcl_args->Add(dcl_arg);

    dbl_args = FdoArgumentDefinitionCollection::Create();
    dbl_args->Add(dbl_arg);

    int16_args = FdoArgumentDefinitionCollection::Create();
    int16_args->Add(int16_arg);

    int32_args = FdoArgumentDefinitionCollection::Create();
    int32_args->Add(int32_arg);

    int64_args = FdoArgumentDefinitionCollection::Create();
    int64_args->Add(int64_arg);

    sgl_args = FdoArgumentDefinitionCollection::Create();
    sgl_args->Add(sgl_arg);

    str_args = FdoArgumentDefinitionCollection::Create();
    str_args->Add(str_arg);

    // Create the signature collection.

    signatures = FdoSignatureDefinitionCollection::Create();

    signature = FdoSignatureDefinition::Create(FdoDataType_Int32, dcl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(FdoDataType_Int32, dbl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(FdoDataType_Int32, int16_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(FdoDataType_Int32, int32_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(FdoDataType_Int32, int64_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(FdoDataType_Int32, sgl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(FdoDataType_Int32, str_args);
    signatures->Add(signature);

    // Create the function definition.

    desc =
        FdoException::NLSGetMessage(FUNCTION_TOINT32,
                                    "Converts a string or number to an int32");
    function_definition =
                FdoFunctionDefinition::Create(
                                        FDO_FUNCTION_TOINT32,
                                        desc,
                                        false,
                                        signatures,
                                        FdoFunctionCategoryType_Conversion);

}  //  CreateFunctionDefinition ()

FdoStringP FdoFunctionToInt32::RemoveBlanks (FdoStringP value)

// +---------------------------------------------------------------------------
// | The function removes leading and/or trailing blanks from the provided
// | string and returns the resulting string back to the calling routine.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    bool       endloop          = false;

    FdoString  *curr_char       = NULL;

    FdoInt64   pos              = 0,
               string_length;

    FdoStringP tmp_str,
               base_string;

    // Set the work-string.

    base_string   = base_string + value;
    string_length = base_string.GetLength();

    // Process the string and remove any leading blanks.

    curr_char = (FdoString *)base_string;

    while (pos < string_length) {

      if (curr_char[pos] != ' ')
          break;
      pos++;

    }  //  while (pos < string_length) ...

    // If no characters other than blanks were found, return the provided
    // string. Otherwise, get the part of the original string without the
    // leading blanks.

    if (pos == string_length)
        return value;
    else
      base_string = base_string.Mid((size_t) pos, (size_t) string_length);

    // Next, process any trailing blanks.

    string_length = base_string.GetLength();
    pos           = string_length - 1;

    while (!endloop) {

      tmp_str = base_string.Mid((size_t)pos, 1);
      if (tmp_str != L" ")
          break;
      pos--;
      endloop = (pos == -1);

    }  //  while ...

    base_string = base_string.Mid(0, (size_t) (pos+1));

    return base_string;

}  //  RemoveBlanks ()

void FdoFunctionToInt32::Validate (FdoLiteralValueCollection *literal_values)

// +---------------------------------------------------------------------------
// | The function validates the argument list that was passed in.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoInt32                count           = literal_values->GetCount();

    FdoDataValue            *data_value     = NULL;

    FdoPtr<FdoLiteralValue> literal_value;

    // Check the number of arguments. TOINT32 accepts one parameter only. If
    // the number of parameters is not correct issue an exception.

    if (count != 1) 
        throw FdoException::Create(
                FdoException::NLSGetMessage(
                  FUNCTION_PARAMETER_NUMBER_ERROR, 
                  "Expression Engine: Invalid number of parameters for function '%1$ls'",
                  FDO_FUNCTION_TOINT32));

    // Next, check whether or not the parameter has a valid data type. If this
    // is not the case issue an exception.

    literal_value = literal_values->GetItem(0);
    if (literal_value->GetLiteralValueType() != FdoLiteralValueType_Data)
        throw FdoException::Create(
                FdoException::NLSGetMessage(
                  FUNCTION_PARAMETER_ERROR, 
                  "Expression Engine: Invalid parameters for function '%1$ls'",
                  FDO_FUNCTION_TOINT32));

    data_value         = static_cast<FdoDataValue *>(literal_value.p);
    incoming_data_type = data_value->GetDataType();
    if ((incoming_data_type != FdoDataType_Decimal) &&
        (incoming_data_type != FdoDataType_Double ) &&
        (incoming_data_type != FdoDataType_Int16  ) &&
        (incoming_data_type != FdoDataType_Int32  ) &&
        (incoming_data_type != FdoDataType_Int64  ) &&
        (incoming_data_type != FdoDataType_Single ) &&
        (incoming_data_type != FdoDataType_String )    )
        throw FdoException::Create(
                FdoException::NLSGetMessage(
                  FUNCTION_PARAMETER_DATA_TYPE_ERROR, 
                  "Expression Engine: Invalid parameter data type for function '%1$ls'",
                  FDO_FUNCTION_TOINT32));

}  //  Validate ()

