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
#include <Functions/String/FdoFunctionSubstr.h>


// ----------------------------------------------------------------------------
// --                         Constructors/Destructors                       --
// ----------------------------------------------------------------------------

FdoFunctionSubstr::FdoFunctionSubstr ()

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

}  //  FdoFunctionSubstr ()


FdoFunctionSubstr::~FdoFunctionSubstr ()

// +---------------------------------------------------------------------------
// | The class destructor.
// +---------------------------------------------------------------------------

{

    // Delete the function definition.

    FDO_SAFE_RELEASE(function_definition);

}  //  ~FdoFunctionSubstr ()


// ----------------------------------------------------------------------------
// --                            Public Class APIs                           --
// ----------------------------------------------------------------------------

FdoFunctionSubstr *FdoFunctionSubstr::Create ()

// +---------------------------------------------------------------------------
// | The function creates a new instance of the class.
// +---------------------------------------------------------------------------

{

    return new FdoFunctionSubstr();

}  //  Create ()

FdoFunctionSubstr *FdoFunctionSubstr::CreateObject ()

// +---------------------------------------------------------------------------
// | The function creates a new instance of the class.
// +---------------------------------------------------------------------------

{

    return new FdoFunctionSubstr();

}  //  CreateObject ()

FdoFunctionDefinition *FdoFunctionSubstr::GetFunctionDefinition ()

// +---------------------------------------------------------------------------
// | The function creates the supported signature list for the function SUBSTR.
// +---------------------------------------------------------------------------

{

    if (function_definition == NULL)
        CreateFunctionDefinition();

    return FDO_SAFE_ADDREF(function_definition);

}  //  GetFunctionDefinition ()

FdoLiteralValue *FdoFunctionSubstr::Evaluate (
                                    FdoLiteralValueCollection *literal_values)

// +---------------------------------------------------------------------------
// | The function processes a call to the function SUBSTR.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    bool                   is_NULL_value                = false;

    FdoInt64               start_pos                    = 0,
                           string_length                = -1,
                           substring_length             = 0;

    FdoStringP             base_string;

    FdoPtr<FdoStringValue> string_value;

    // Validate the function call.

    Validate(literal_values);

    // Get the string from which to extract a substring. If no value is pro-
    // vided, terminate the function.

    string_value = (FdoStringValue *) literal_values->GetItem(0);
    if (string_value->IsNull())
        return FdoStringValue::Create();
    else
      base_string = base_string + string_value->GetString();
    string_length = base_string.GetLength();

    // Next, get the start position for the substring. If no value is pro-
    // vided, an empty string is returned back to the calling routine. If
    // the value is negativ, the position is corrected to 0.
    // NOTE: The user supplied start position is 1-based whereas strings are
    //       0-based. Therefore, the retrieved start position needs to be
    //       adjusted.

    start_pos =
        GetNumericValue(literal_values, 1, para2_data_type, &is_NULL_value);
    if (is_NULL_value)
        return FdoStringValue::Create();
    start_pos = start_pos - 1;
    if (start_pos < 0)
        start_pos = 0;

    // If a third argument was supplied, it represents the length of the sub-
    // string to extract. Get the argument value. If no value is supplied, an
    // empty string is returned back to the calling routine. A negativ number
    // is valid as it indicates to extract to the end of the original string.

    if (literal_values->GetCount() == 3) {

        substring_length = 
            GetNumericValue(
                        literal_values, 2, para3_data_type, &is_NULL_value);
        if (is_NULL_value)
            return FdoStringValue::Create();
        if (substring_length < 0)
            return FdoStringValue::Create();

    } //  if (literal_values->GetCount() == 3) ...
    else
      substring_length = string_length;

    // Create the resulting string and return it back to the calling routine.

    return FdoStringValue::Create(
                    base_string.Mid(
                            (size_t) start_pos, (size_t) substring_length));

}  //  Evaluate ()


// ----------------------------------------------------------------------------
// --                          Supporting functions                          --
// ----------------------------------------------------------------------------

void FdoFunctionSubstr::CreateFunctionDefinition ()

// +---------------------------------------------------------------------------
// | The procedure creates the function definition for the function SUBSTR. The
// | function definition includes the list of supported signatures. The follow-
// | ing signatures are supported:
// |
// |    SUBSTR (string,
// |            {byte, decimal, double, int16, int32, int64, single},
// |            {byte, decimal, double, int16, int32, int64, single} )
// |
// | The function always returns a STRING.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoString                               *desc                   = NULL;

    FdoStringP                              arg1_description;
    FdoStringP                              arg2_description;
    FdoStringP                              arg3_description;
    FdoStringP                              str_arg_literal;
    FdoStringP                              start_pos_arg_literal;
    FdoStringP                              substr_length_arg_literal;

    FdoPtr<FdoArgumentDefinition>           str_arg;

    FdoPtr<FdoArgumentDefinition>           byte_pos_arg;
    FdoPtr<FdoArgumentDefinition>           dcl_pos_arg;
    FdoPtr<FdoArgumentDefinition>           dbl_pos_arg;
    FdoPtr<FdoArgumentDefinition>           int16_pos_arg;
    FdoPtr<FdoArgumentDefinition>           int32_pos_arg;
    FdoPtr<FdoArgumentDefinition>           int64_pos_arg;
    FdoPtr<FdoArgumentDefinition>           sgl_pos_arg;

    FdoPtr<FdoArgumentDefinition>           byte_lng_arg;
    FdoPtr<FdoArgumentDefinition>           dcl_lng_arg;
    FdoPtr<FdoArgumentDefinition>           dbl_lng_arg;
    FdoPtr<FdoArgumentDefinition>           int16_lng_arg;
    FdoPtr<FdoArgumentDefinition>           int32_lng_arg;
    FdoPtr<FdoArgumentDefinition>           int64_lng_arg;
    FdoPtr<FdoArgumentDefinition>           sgl_lng_arg;

    FdoPtr<FdoArgumentDefinitionCollection> str_byte_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_dcl_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_dbl_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_int16_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_int32_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_int64_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_sgl_args;

    FdoPtr<FdoArgumentDefinitionCollection> str_byte_byte_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_byte_dcl_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_byte_dbl_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_byte_int16_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_byte_int32_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_byte_int64_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_byte_sgl_args;

    FdoPtr<FdoArgumentDefinitionCollection> str_dcl_byte_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_dcl_dcl_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_dcl_dbl_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_dcl_int16_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_dcl_int32_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_dcl_int64_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_dcl_sgl_args;

    FdoPtr<FdoArgumentDefinitionCollection> str_dbl_byte_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_dbl_dcl_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_dbl_dbl_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_dbl_int16_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_dbl_int32_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_dbl_int64_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_dbl_sgl_args;

    FdoPtr<FdoArgumentDefinitionCollection> str_int16_byte_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_int16_dcl_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_int16_dbl_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_int16_int16_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_int16_int32_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_int16_int64_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_int16_sgl_args;

    FdoPtr<FdoArgumentDefinitionCollection> str_int32_byte_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_int32_dcl_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_int32_dbl_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_int32_int16_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_int32_int32_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_int32_int64_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_int32_sgl_args;

    FdoPtr<FdoArgumentDefinitionCollection> str_int64_byte_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_int64_dcl_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_int64_dbl_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_int64_int16_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_int64_int32_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_int64_int64_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_int64_sgl_args;

    FdoPtr<FdoArgumentDefinitionCollection> str_sgl_byte_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_sgl_dcl_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_sgl_dbl_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_sgl_int16_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_sgl_int32_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_sgl_int64_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_sgl_sgl_args;

    FdoPtr<FdoSignatureDefinition>          signature;
    FdoSignatureDefinitionCollection        *signatures;

    // Get the general descriptions for the arguments.

    arg1_description =
                   FdoException::NLSGetMessage(FUNCTION_SUBSTR_STRING_ARG,
                                               "String from which to extract");
    arg2_description =
            FdoException::NLSGetMessage(
                            FUNCTION_SUBSTR_START_POS_ARG,
                            "Position from which to extract a substring");
    arg3_description =
        FdoException::NLSGetMessage(FUNCTION_SUBSTR_LENGTH_ARG,
                                    "The length of the substring to extract");

    // The following defines the different argument definition collections.

    str_arg_literal = FdoException::NLSGetMessage(FUNCTION_STRING_ARG_LIT,
                                                  "text property");
    start_pos_arg_literal =
                FdoException::NLSGetMessage(FUNCTION_SUBSTR_START_POS_ARG_LIT,
                                            "position to start");
    substr_length_arg_literal =
                FdoException::NLSGetMessage(FUNCTION_SUBSTR_LENGTH_ARG_LIT,
                                            "optional length of substring");

    str_arg   = FdoArgumentDefinition::Create(
                    str_arg_literal, arg1_description, FdoDataType_String);

    byte_pos_arg  =
        FdoArgumentDefinition::Create(
                start_pos_arg_literal, arg2_description, FdoDataType_Byte);
    dcl_pos_arg   =
        FdoArgumentDefinition::Create(
                start_pos_arg_literal, arg2_description, FdoDataType_Decimal);
    dbl_pos_arg   =
        FdoArgumentDefinition::Create(
                start_pos_arg_literal, arg2_description, FdoDataType_Double);
    int16_pos_arg =
        FdoArgumentDefinition::Create(
                start_pos_arg_literal, arg2_description, FdoDataType_Int16);
    int32_pos_arg =
        FdoArgumentDefinition::Create(
                start_pos_arg_literal, arg2_description, FdoDataType_Int32);
    int64_pos_arg =
        FdoArgumentDefinition::Create(
                start_pos_arg_literal, arg2_description, FdoDataType_Int64);
    sgl_pos_arg   =
        FdoArgumentDefinition::Create(
                start_pos_arg_literal, arg2_description, FdoDataType_Single);

    byte_lng_arg  =
        FdoArgumentDefinition::Create(
            substr_length_arg_literal, arg3_description, FdoDataType_Byte);
    dcl_lng_arg   =
        FdoArgumentDefinition::Create(
            substr_length_arg_literal, arg3_description, FdoDataType_Decimal);
    dbl_lng_arg   =
        FdoArgumentDefinition::Create(
            substr_length_arg_literal, arg3_description, FdoDataType_Double);
    int16_lng_arg =
        FdoArgumentDefinition::Create(
            substr_length_arg_literal, arg3_description, FdoDataType_Int16);
    int32_lng_arg =
        FdoArgumentDefinition::Create(
            substr_length_arg_literal, arg3_description, FdoDataType_Int32);
    int64_lng_arg =
        FdoArgumentDefinition::Create(
            substr_length_arg_literal, arg3_description, FdoDataType_Int64);
    sgl_lng_arg   =
        FdoArgumentDefinition::Create(
            substr_length_arg_literal, arg3_description, FdoDataType_Single);

    str_byte_args = FdoArgumentDefinitionCollection::Create();
    str_byte_args->Add(str_arg);
    str_byte_args->Add(byte_pos_arg);

    str_dcl_args = FdoArgumentDefinitionCollection::Create();
    str_dcl_args->Add(str_arg);
    str_dcl_args->Add(dcl_pos_arg);

    str_dbl_args = FdoArgumentDefinitionCollection::Create();
    str_dbl_args->Add(str_arg);
    str_dbl_args->Add(dbl_pos_arg);

    str_int16_args = FdoArgumentDefinitionCollection::Create();
    str_int16_args->Add(str_arg);
    str_int16_args->Add(int16_pos_arg);

    str_int32_args = FdoArgumentDefinitionCollection::Create();
    str_int32_args->Add(str_arg);
    str_int32_args->Add(int32_pos_arg);

    str_int64_args = FdoArgumentDefinitionCollection::Create();
    str_int64_args->Add(str_arg);
    str_int64_args->Add(int64_pos_arg);

    str_sgl_args = FdoArgumentDefinitionCollection::Create();
    str_sgl_args->Add(str_arg);
    str_sgl_args->Add(sgl_pos_arg);

    str_byte_byte_args = FdoArgumentDefinitionCollection::Create();
    str_byte_byte_args->Add(str_arg);
    str_byte_byte_args->Add(byte_pos_arg);
    str_byte_byte_args->Add(byte_lng_arg);

    str_byte_dcl_args = FdoArgumentDefinitionCollection::Create();
    str_byte_dcl_args->Add(str_arg);
    str_byte_dcl_args->Add(byte_pos_arg);
    str_byte_dcl_args->Add(dcl_lng_arg);

    str_byte_dbl_args = FdoArgumentDefinitionCollection::Create();
    str_byte_dbl_args->Add(str_arg);
    str_byte_dbl_args->Add(byte_pos_arg);
    str_byte_dbl_args->Add(dbl_lng_arg);

    str_byte_int16_args = FdoArgumentDefinitionCollection::Create();
    str_byte_int16_args->Add(str_arg);
    str_byte_int16_args->Add(byte_pos_arg);
    str_byte_int16_args->Add(int16_lng_arg);

    str_byte_int32_args = FdoArgumentDefinitionCollection::Create();
    str_byte_int32_args->Add(str_arg);
    str_byte_int32_args->Add(byte_pos_arg);
    str_byte_int32_args->Add(int32_lng_arg);

    str_byte_int64_args = FdoArgumentDefinitionCollection::Create();
    str_byte_int64_args->Add(str_arg);
    str_byte_int64_args->Add(byte_pos_arg);
    str_byte_int64_args->Add(int64_lng_arg);

    str_byte_sgl_args = FdoArgumentDefinitionCollection::Create();
    str_byte_sgl_args->Add(str_arg);
    str_byte_sgl_args->Add(byte_pos_arg);
    str_byte_sgl_args->Add(sgl_lng_arg);

    str_dcl_byte_args = FdoArgumentDefinitionCollection::Create();
    str_dcl_byte_args->Add(str_arg);
    str_dcl_byte_args->Add(dcl_pos_arg);
    str_dcl_byte_args->Add(byte_lng_arg);

    str_dcl_dcl_args = FdoArgumentDefinitionCollection::Create();
    str_dcl_dcl_args->Add(str_arg);
    str_dcl_dcl_args->Add(dcl_pos_arg);
    str_dcl_dcl_args->Add(dcl_lng_arg);

    str_dcl_dbl_args = FdoArgumentDefinitionCollection::Create();
    str_dcl_dbl_args->Add(str_arg);
    str_dcl_dbl_args->Add(dcl_pos_arg);
    str_dcl_dbl_args->Add(dbl_lng_arg);

    str_dcl_int16_args = FdoArgumentDefinitionCollection::Create();
    str_dcl_int16_args->Add(str_arg);
    str_dcl_int16_args->Add(dcl_pos_arg);
    str_dcl_int16_args->Add(int16_lng_arg);

    str_dcl_int32_args = FdoArgumentDefinitionCollection::Create();
    str_dcl_int32_args->Add(str_arg);
    str_dcl_int32_args->Add(dcl_pos_arg);
    str_dcl_int32_args->Add(int32_lng_arg);

    str_dcl_int64_args = FdoArgumentDefinitionCollection::Create();
    str_dcl_int64_args->Add(str_arg);
    str_dcl_int64_args->Add(dcl_pos_arg);
    str_dcl_int64_args->Add(int64_lng_arg);

    str_dcl_sgl_args = FdoArgumentDefinitionCollection::Create();
    str_dcl_sgl_args->Add(str_arg);
    str_dcl_sgl_args->Add(dcl_pos_arg);
    str_dcl_sgl_args->Add(sgl_lng_arg);

    str_dbl_byte_args = FdoArgumentDefinitionCollection::Create();
    str_dbl_byte_args->Add(str_arg);
    str_dbl_byte_args->Add(dbl_pos_arg);
    str_dbl_byte_args->Add(byte_lng_arg);

    str_dbl_dcl_args = FdoArgumentDefinitionCollection::Create();
    str_dbl_dcl_args->Add(str_arg);
    str_dbl_dcl_args->Add(dbl_pos_arg);
    str_dbl_dcl_args->Add(dcl_lng_arg);

    str_dbl_dbl_args = FdoArgumentDefinitionCollection::Create();
    str_dbl_dbl_args->Add(str_arg);
    str_dbl_dbl_args->Add(dbl_pos_arg);
    str_dbl_dbl_args->Add(dbl_lng_arg);

    str_dbl_int16_args = FdoArgumentDefinitionCollection::Create();
    str_dbl_int16_args->Add(str_arg);
    str_dbl_int16_args->Add(dbl_pos_arg);
    str_dbl_int16_args->Add(int16_lng_arg);

    str_dbl_int32_args = FdoArgumentDefinitionCollection::Create();
    str_dbl_int32_args->Add(str_arg);
    str_dbl_int32_args->Add(dbl_pos_arg);
    str_dbl_int32_args->Add(int32_lng_arg);

    str_dbl_int64_args = FdoArgumentDefinitionCollection::Create();
    str_dbl_int64_args->Add(str_arg);
    str_dbl_int64_args->Add(dbl_pos_arg);
    str_dbl_int64_args->Add(int64_lng_arg);

    str_dbl_sgl_args = FdoArgumentDefinitionCollection::Create();
    str_dbl_sgl_args->Add(str_arg);
    str_dbl_sgl_args->Add(dbl_pos_arg);
    str_dbl_sgl_args->Add(sgl_lng_arg);

    str_int16_byte_args = FdoArgumentDefinitionCollection::Create();
    str_int16_byte_args->Add(str_arg);
    str_int16_byte_args->Add(int16_pos_arg);
    str_int16_byte_args->Add(byte_lng_arg);

    str_int16_dcl_args = FdoArgumentDefinitionCollection::Create();
    str_int16_dcl_args->Add(str_arg);
    str_int16_dcl_args->Add(int16_pos_arg);
    str_int16_dcl_args->Add(dcl_lng_arg);

    str_int16_dbl_args = FdoArgumentDefinitionCollection::Create();
    str_int16_dbl_args->Add(str_arg);
    str_int16_dbl_args->Add(int16_pos_arg);
    str_int16_dbl_args->Add(dbl_lng_arg);

    str_int16_int16_args = FdoArgumentDefinitionCollection::Create();
    str_int16_int16_args->Add(str_arg);
    str_int16_int16_args->Add(int16_pos_arg);
    str_int16_int16_args->Add(int16_lng_arg);

    str_int16_int32_args = FdoArgumentDefinitionCollection::Create();
    str_int16_int32_args->Add(str_arg);
    str_int16_int32_args->Add(int16_pos_arg);
    str_int16_int32_args->Add(int32_lng_arg);

    str_int16_int64_args = FdoArgumentDefinitionCollection::Create();
    str_int16_int64_args->Add(str_arg);
    str_int16_int64_args->Add(int16_pos_arg);
    str_int16_int64_args->Add(int64_lng_arg);

    str_int16_sgl_args = FdoArgumentDefinitionCollection::Create();
    str_int16_sgl_args->Add(str_arg);
    str_int16_sgl_args->Add(int16_pos_arg);
    str_int16_sgl_args->Add(sgl_lng_arg);

    str_int32_byte_args = FdoArgumentDefinitionCollection::Create();
    str_int32_byte_args->Add(str_arg);
    str_int32_byte_args->Add(int32_pos_arg);
    str_int32_byte_args->Add(byte_lng_arg);

    str_int32_dcl_args = FdoArgumentDefinitionCollection::Create();
    str_int32_dcl_args->Add(str_arg);
    str_int32_dcl_args->Add(int32_pos_arg);
    str_int32_dcl_args->Add(dcl_lng_arg);

    str_int32_dbl_args = FdoArgumentDefinitionCollection::Create();
    str_int32_dbl_args->Add(str_arg);
    str_int32_dbl_args->Add(int32_pos_arg);
    str_int32_dbl_args->Add(dbl_lng_arg);

    str_int32_int16_args = FdoArgumentDefinitionCollection::Create();
    str_int32_int16_args->Add(str_arg);
    str_int32_int16_args->Add(int32_pos_arg);
    str_int32_int16_args->Add(int16_lng_arg);

    str_int32_int32_args = FdoArgumentDefinitionCollection::Create();
    str_int32_int32_args->Add(str_arg);
    str_int32_int32_args->Add(int32_pos_arg);
    str_int32_int32_args->Add(int32_lng_arg);

    str_int32_int64_args = FdoArgumentDefinitionCollection::Create();
    str_int32_int64_args->Add(str_arg);
    str_int32_int64_args->Add(int32_pos_arg);
    str_int32_int64_args->Add(int64_lng_arg);

    str_int32_sgl_args = FdoArgumentDefinitionCollection::Create();
    str_int32_sgl_args->Add(str_arg);
    str_int32_sgl_args->Add(int32_pos_arg);
    str_int32_sgl_args->Add(sgl_lng_arg);

    str_int64_byte_args = FdoArgumentDefinitionCollection::Create();
    str_int64_byte_args->Add(str_arg);
    str_int64_byte_args->Add(int64_pos_arg);
    str_int64_byte_args->Add(byte_lng_arg);

    str_int64_dcl_args = FdoArgumentDefinitionCollection::Create();
    str_int64_dcl_args->Add(str_arg);
    str_int64_dcl_args->Add(int64_pos_arg);
    str_int64_dcl_args->Add(dcl_lng_arg);

    str_int64_dbl_args = FdoArgumentDefinitionCollection::Create();
    str_int64_dbl_args->Add(str_arg);
    str_int64_dbl_args->Add(int64_pos_arg);
    str_int64_dbl_args->Add(dbl_lng_arg);

    str_int64_int16_args = FdoArgumentDefinitionCollection::Create();
    str_int64_int16_args->Add(str_arg);
    str_int64_int16_args->Add(int64_pos_arg);
    str_int64_int16_args->Add(int16_lng_arg);

    str_int64_int32_args = FdoArgumentDefinitionCollection::Create();
    str_int64_int32_args->Add(str_arg);
    str_int64_int32_args->Add(int64_pos_arg);
    str_int64_int32_args->Add(int32_lng_arg);

    str_int64_int64_args = FdoArgumentDefinitionCollection::Create();
    str_int64_int64_args->Add(str_arg);
    str_int64_int64_args->Add(int64_pos_arg);
    str_int64_int64_args->Add(int64_lng_arg);

    str_int64_sgl_args = FdoArgumentDefinitionCollection::Create();
    str_int64_sgl_args->Add(str_arg);
    str_int64_sgl_args->Add(int64_pos_arg);
    str_int64_sgl_args->Add(sgl_lng_arg);

    str_sgl_byte_args = FdoArgumentDefinitionCollection::Create();
    str_sgl_byte_args->Add(str_arg);
    str_sgl_byte_args->Add(sgl_pos_arg);
    str_sgl_byte_args->Add(byte_lng_arg);

    str_sgl_dcl_args = FdoArgumentDefinitionCollection::Create();
    str_sgl_dcl_args->Add(str_arg);
    str_sgl_dcl_args->Add(sgl_pos_arg);
    str_sgl_dcl_args->Add(dcl_lng_arg);

    str_sgl_dbl_args = FdoArgumentDefinitionCollection::Create();
    str_sgl_dbl_args->Add(str_arg);
    str_sgl_dbl_args->Add(sgl_pos_arg);
    str_sgl_dbl_args->Add(dbl_lng_arg);

    str_sgl_int16_args = FdoArgumentDefinitionCollection::Create();
    str_sgl_int16_args->Add(str_arg);
    str_sgl_int16_args->Add(sgl_pos_arg);
    str_sgl_int16_args->Add(int16_lng_arg);

    str_sgl_int32_args = FdoArgumentDefinitionCollection::Create();
    str_sgl_int32_args->Add(str_arg);
    str_sgl_int32_args->Add(sgl_pos_arg);
    str_sgl_int32_args->Add(int32_lng_arg);

    str_sgl_int64_args = FdoArgumentDefinitionCollection::Create();
    str_sgl_int64_args->Add(str_arg);
    str_sgl_int64_args->Add(sgl_pos_arg);
    str_sgl_int64_args->Add(int64_lng_arg);

    str_sgl_sgl_args = FdoArgumentDefinitionCollection::Create();
    str_sgl_sgl_args->Add(str_arg);
    str_sgl_sgl_args->Add(sgl_pos_arg);
    str_sgl_sgl_args->Add(sgl_lng_arg);

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
                                    FdoDataType_String, str_byte_byte_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                    FdoDataType_String, str_byte_dcl_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                    FdoDataType_String, str_byte_dbl_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                    FdoDataType_String, str_byte_int16_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                    FdoDataType_String, str_byte_int32_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                    FdoDataType_String, str_byte_int64_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                    FdoDataType_String, str_byte_sgl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                    FdoDataType_String, str_dcl_byte_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                    FdoDataType_String, str_dcl_dcl_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                    FdoDataType_String, str_dcl_dbl_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                    FdoDataType_String, str_dcl_int16_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                    FdoDataType_String, str_dcl_int32_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                    FdoDataType_String, str_dcl_int64_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                    FdoDataType_String, str_dcl_sgl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                    FdoDataType_String, str_dbl_byte_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                    FdoDataType_String, str_dbl_dcl_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                    FdoDataType_String, str_dbl_dbl_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                    FdoDataType_String, str_dbl_int16_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                    FdoDataType_String, str_dbl_int32_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                    FdoDataType_String, str_dbl_int64_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                    FdoDataType_String, str_dbl_sgl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                    FdoDataType_String, str_int16_byte_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                    FdoDataType_String, str_int16_dcl_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                    FdoDataType_String, str_int16_dbl_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                    FdoDataType_String, str_int16_int16_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                    FdoDataType_String, str_int16_int32_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                    FdoDataType_String, str_int16_int64_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                    FdoDataType_String, str_int16_sgl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                    FdoDataType_String, str_int32_byte_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                    FdoDataType_String, str_int32_dcl_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                    FdoDataType_String, str_int32_dbl_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                    FdoDataType_String, str_int32_int16_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                    FdoDataType_String, str_int32_int32_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                    FdoDataType_String, str_int32_int64_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                    FdoDataType_String, str_int32_sgl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                    FdoDataType_String, str_int64_byte_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                    FdoDataType_String, str_int64_dcl_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                    FdoDataType_String, str_int64_dbl_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                    FdoDataType_String, str_int64_int16_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                    FdoDataType_String, str_int64_int32_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                    FdoDataType_String, str_int64_int64_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                    FdoDataType_String, str_int64_sgl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                    FdoDataType_String, str_sgl_byte_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                    FdoDataType_String, str_sgl_dcl_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                    FdoDataType_String, str_sgl_dbl_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                    FdoDataType_String, str_sgl_int16_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                    FdoDataType_String, str_sgl_int32_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                    FdoDataType_String, str_sgl_int64_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                    FdoDataType_String, str_sgl_sgl_args);
    signatures->Add(signature);

    // Create the function definition.

    desc = FdoException::NLSGetMessage(
                    FUNCTION_SUBSTR,
                    "Returns a substring from a provided string as defined");
    function_definition =
                FdoFunctionDefinition::Create(
                                        FDO_FUNCTION_SUBSTR,
                                        desc,
                                        false,
                                        signatures,
                                        FdoFunctionCategoryType_String);

}  //  CreateFunctionDefinition ()

FdoInt64 FdoFunctionSubstr::GetNumericValue (
                                    FdoLiteralValueCollection *literal_values,
                                    FdoInt32                  pos,
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
        byte_value =(FdoByteValue *) literal_values->GetItem(pos);
        if (!byte_value->IsNull())
            return (FdoInt64) (byte_value->GetByte());
        else {

          *is_NULL_value = true;
          return 0;

        }  //  else ...
        break;

      case FdoDataType_Decimal:
        decimal_value =(FdoDecimalValue *) literal_values->GetItem(pos);
        if (!decimal_value->IsNull())
            return (FdoInt64) (floor(decimal_value->GetDecimal()));
        else {

          *is_NULL_value = true;
          return 0;

        }  //  else ...
        break;

      case FdoDataType_Double:
        double_value = (FdoDoubleValue *) literal_values->GetItem(pos);
        if (!double_value->IsNull())
            return (FdoInt64) (floor(double_value->GetDouble()));
        else {

          *is_NULL_value = true;
          return 0;

        }  //  else ...
        break;

      case FdoDataType_Int16:
        int16_value = (FdoInt16Value *) literal_values->GetItem(pos);
        if (!int16_value->IsNull())
            return (FdoInt64) (int16_value->GetInt16());
        else {

          *is_NULL_value = true;
          return 0;

        }  //  else ...
        break;

      case FdoDataType_Int32:
        int32_value = (FdoInt32Value *) literal_values->GetItem(pos);
        if (!int32_value->IsNull())
            return (FdoInt64) (int32_value->GetInt32());
        else {

          *is_NULL_value = true;
          return 0;

        }  //  else ...
        break;

      case FdoDataType_Int64:
        int64_value = (FdoInt64Value *) literal_values->GetItem(pos);
        if (!int64_value->IsNull())
            return int64_value->GetInt64();
        else {

          *is_NULL_value = true;
          return 0;

        }  //  else ...
        break;

      case FdoDataType_Single:
        single_value = (FdoSingleValue *) literal_values->GetItem(pos);
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
              FDO_FUNCTION_SUBSTR));

}  //  GetPaddingLength ()

void FdoFunctionSubstr::Validate (FdoLiteralValueCollection *literal_values)

// +---------------------------------------------------------------------------
// | The function validates the argument list that was passed in.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoInt32                i               = 0;

    FdoDataValue            *data_value     = NULL;

    FdoPtr<FdoLiteralValue> literal_value;

    // Check the number of arguments. SUBSTR accepts either two or three para-
    // meters. If the number of parameters is not correct issue an exception.

    number_of_parameters = literal_values->GetCount();

    if ((number_of_parameters < 2) || (number_of_parameters > 3)) 
        throw FdoException::Create(
               FdoException::NLSGetMessage(
                  FUNCTION_PARAMETER_NUMBER_ERROR, 
                  "Expression Engine: Invalid number of parameters for function '%1$ls'",
                  FDO_FUNCTION_SUBSTR));

    // Next, navigate through the parameter list and get the data type for each
    // of the provided arguments.

    for (i = 0; i < number_of_parameters; i++) {

      literal_value = literal_values->GetItem(i);
      if (literal_value->GetLiteralValueType() != FdoLiteralValueType_Data)
          throw FdoException::Create(
                FdoException::NLSGetMessage(
                  FUNCTION_PARAMETER_ERROR, 
                  "Expression Engine: Invalid parameters for function '%1$ls'",
                  FDO_FUNCTION_SUBSTR));

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
                  FDO_FUNCTION_SUBSTR));

    if (number_of_parameters == 3)
        if ((para3_data_type != FdoDataType_Byte   ) &&
            (para3_data_type != FdoDataType_Decimal) &&
            (para3_data_type != FdoDataType_Double ) &&
            (para3_data_type != FdoDataType_Int16  ) &&
            (para3_data_type != FdoDataType_Int32  ) &&
            (para3_data_type != FdoDataType_Int64  ) &&
            (para3_data_type != FdoDataType_Single )    )
            throw FdoException::Create(
                FdoException::NLSGetMessage(
                  FUNCTION_PARAMETER_DATA_TYPE_ERROR, 
                  "Expression Engine: Invalid parameter data type for function '%1$ls'",
                  FDO_FUNCTION_SUBSTR));

}  //  Validate ()

