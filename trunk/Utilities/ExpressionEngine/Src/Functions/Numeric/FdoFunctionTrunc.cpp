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
#include <Functions/Numeric/FdoFunctionTrunc.h>
#include <FdoCommonStringUtil.h>


// ----------------------------------------------------------------------------
// --                         Constructors/Destructors                       --
// ----------------------------------------------------------------------------

FdoFunctionTrunc::FdoFunctionTrunc ()

// +---------------------------------------------------------------------------
// | The class constructor.
// +---------------------------------------------------------------------------

{

    // Initialize all class variables.
    // NOTE: Due to the fact that data type enumeration misses an entry to
    //       indicate a not-set value, the variables "para1_data_type" and
    //       "para2_data_type" are set to "FdoDataType_CLOB" to indicate an
    //       invalid data type because this function does not support this
    //       type. 

    function_definition  = NULL;

    number_of_parameters = 0;

    is_date_truncation   = false;

    para1_data_type      = FdoDataType_CLOB;
    para2_data_type      = FdoDataType_CLOB;

    first = true;

}  //  FdoFunctionTrunc ()


FdoFunctionTrunc::~FdoFunctionTrunc ()

// +---------------------------------------------------------------------------
// | The class destructor.
// +---------------------------------------------------------------------------

{

    // Delete the function definition.

    FDO_SAFE_RELEASE(function_definition);

}  //  ~FdoFunctionTrunc ()


// ----------------------------------------------------------------------------
// --                            Public Class APIs                           --
// ----------------------------------------------------------------------------

FdoFunctionTrunc *FdoFunctionTrunc::Create ()

// +---------------------------------------------------------------------------
// | The function creates a new instance of the class.
// +---------------------------------------------------------------------------

{

    return new FdoFunctionTrunc();

}  //  Create ()

FdoFunctionTrunc *FdoFunctionTrunc::CreateObject ()

// +---------------------------------------------------------------------------
// | The function creates a new instance of the class.
// +---------------------------------------------------------------------------

{

    return new FdoFunctionTrunc();

}  //  CreateObject ()

FdoFunctionDefinition *FdoFunctionTrunc::GetFunctionDefinition ()

// +---------------------------------------------------------------------------
// | The function creates the supported signature list for the function TRUNC.
// +---------------------------------------------------------------------------

{

    if (function_definition == NULL)
        CreateFunctionDefinition();

    return FDO_SAFE_ADDREF(function_definition);

}  //  GetFunctionDefinition ()

FdoLiteralValue *FdoFunctionTrunc::Evaluate (
                                    FdoLiteralValueCollection *literal_values)

// +---------------------------------------------------------------------------
// | The function processes a call to the function TRUNC.
// +---------------------------------------------------------------------------

{

    if (first)
    {
        Validate(literal_values);
    }

    // Processing differs depending on the data type of the first parameter.

    if (is_date_truncation)
        return ProcessDateTruncationRequest(literal_values);
    else
      return ProcessNumericTruncationRequest(literal_values);

}  //  Evaluate ()


// ----------------------------------------------------------------------------
// --                          Supporting functions                          --
// ----------------------------------------------------------------------------

void FdoFunctionTrunc::CreateFunctionDefinition ()

// +---------------------------------------------------------------------------
// | The procedure creates the function definition for the function TRUNC. The
// | function definition includes the list of supported signatures. The follow-
// | ing signatures are supported:
// |
// |    TRUNC (date, {YEAR, MONTH, DAY, HOUR, MINUTE} )
// |
// |    TRUNC ({byte, decimal, double, int16, int32, int64, single}
// |           [, {byte, decimal, double, int16, int32, int64, single}] )
// |
// | The function always returns a DOUBLE.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoString                               *desc               = NULL;

    FdoStringP                              arg1_description;
    FdoStringP                              date_op_arg_description;
    FdoStringP                              num_opt_dec_arg_description;

    FdoStringP                              date_arg_literal;
    FdoStringP                              date_op_arg_literal;
    FdoStringP                              num_arg_literal;
    FdoStringP                              num_opt_dec_arg_literal;

    FdoPtr<FdoArgumentDefinition>           date_arg;
    FdoPtr<FdoArgumentDefinition>           date_op_arg;

    FdoPtr<FdoArgumentDefinition>           byte_arg;
    FdoPtr<FdoArgumentDefinition>           dcl_arg;
    FdoPtr<FdoArgumentDefinition>           dbl_arg;
    FdoPtr<FdoArgumentDefinition>           int16_arg;
    FdoPtr<FdoArgumentDefinition>           int32_arg;
    FdoPtr<FdoArgumentDefinition>           int64_arg;
    FdoPtr<FdoArgumentDefinition>           sgl_arg;

    FdoPtr<FdoArgumentDefinition>           opt_byte_arg;
    FdoPtr<FdoArgumentDefinition>           opt_dcl_arg;
    FdoPtr<FdoArgumentDefinition>           opt_dbl_arg;
    FdoPtr<FdoArgumentDefinition>           opt_int16_arg;
    FdoPtr<FdoArgumentDefinition>           opt_int32_arg;
    FdoPtr<FdoArgumentDefinition>           opt_int64_arg;
    FdoPtr<FdoArgumentDefinition>           opt_sgl_arg;

    FdoPtr<FdoArgumentDefinitionCollection> date_args;

    FdoPtr<FdoArgumentDefinitionCollection> byte_args;
    FdoPtr<FdoArgumentDefinitionCollection> dcl_args;
    FdoPtr<FdoArgumentDefinitionCollection> dbl_args;
    FdoPtr<FdoArgumentDefinitionCollection> int16_args;
    FdoPtr<FdoArgumentDefinitionCollection> int32_args;
    FdoPtr<FdoArgumentDefinitionCollection> int64_args;
    FdoPtr<FdoArgumentDefinitionCollection> sgl_args;

    FdoPtr<FdoArgumentDefinitionCollection> byte_byte_args;
    FdoPtr<FdoArgumentDefinitionCollection> byte_dcl_args;
    FdoPtr<FdoArgumentDefinitionCollection> byte_dbl_args;
    FdoPtr<FdoArgumentDefinitionCollection> byte_int16_args;
    FdoPtr<FdoArgumentDefinitionCollection> byte_int32_args;
    FdoPtr<FdoArgumentDefinitionCollection> byte_int64_args;
    FdoPtr<FdoArgumentDefinitionCollection> byte_sgl_args;

    FdoPtr<FdoArgumentDefinitionCollection> dcl_byte_args;
    FdoPtr<FdoArgumentDefinitionCollection> dcl_dcl_args;
    FdoPtr<FdoArgumentDefinitionCollection> dcl_dbl_args;
    FdoPtr<FdoArgumentDefinitionCollection> dcl_int16_args;
    FdoPtr<FdoArgumentDefinitionCollection> dcl_int32_args;
    FdoPtr<FdoArgumentDefinitionCollection> dcl_int64_args;
    FdoPtr<FdoArgumentDefinitionCollection> dcl_sgl_args;

    FdoPtr<FdoArgumentDefinitionCollection> dbl_byte_args;
    FdoPtr<FdoArgumentDefinitionCollection> dbl_dcl_args;
    FdoPtr<FdoArgumentDefinitionCollection> dbl_dbl_args;
    FdoPtr<FdoArgumentDefinitionCollection> dbl_int16_args;
    FdoPtr<FdoArgumentDefinitionCollection> dbl_int32_args;
    FdoPtr<FdoArgumentDefinitionCollection> dbl_int64_args;
    FdoPtr<FdoArgumentDefinitionCollection> dbl_sgl_args;

    FdoPtr<FdoArgumentDefinitionCollection> int16_byte_args;
    FdoPtr<FdoArgumentDefinitionCollection> int16_dcl_args;
    FdoPtr<FdoArgumentDefinitionCollection> int16_dbl_args;
    FdoPtr<FdoArgumentDefinitionCollection> int16_int16_args;
    FdoPtr<FdoArgumentDefinitionCollection> int16_int32_args;
    FdoPtr<FdoArgumentDefinitionCollection> int16_int64_args;
    FdoPtr<FdoArgumentDefinitionCollection> int16_sgl_args;

    FdoPtr<FdoArgumentDefinitionCollection> int32_byte_args;
    FdoPtr<FdoArgumentDefinitionCollection> int32_dcl_args;
    FdoPtr<FdoArgumentDefinitionCollection> int32_dbl_args;
    FdoPtr<FdoArgumentDefinitionCollection> int32_int16_args;
    FdoPtr<FdoArgumentDefinitionCollection> int32_int32_args;
    FdoPtr<FdoArgumentDefinitionCollection> int32_int64_args;
    FdoPtr<FdoArgumentDefinitionCollection> int32_sgl_args;

    FdoPtr<FdoArgumentDefinitionCollection> int64_byte_args;
    FdoPtr<FdoArgumentDefinitionCollection> int64_dcl_args;
    FdoPtr<FdoArgumentDefinitionCollection> int64_dbl_args;
    FdoPtr<FdoArgumentDefinitionCollection> int64_int16_args;
    FdoPtr<FdoArgumentDefinitionCollection> int64_int32_args;
    FdoPtr<FdoArgumentDefinitionCollection> int64_int64_args;
    FdoPtr<FdoArgumentDefinitionCollection> int64_sgl_args;

    FdoPtr<FdoArgumentDefinitionCollection> sgl_byte_args;
    FdoPtr<FdoArgumentDefinitionCollection> sgl_dcl_args;
    FdoPtr<FdoArgumentDefinitionCollection> sgl_dbl_args;
    FdoPtr<FdoArgumentDefinitionCollection> sgl_int16_args;
    FdoPtr<FdoArgumentDefinitionCollection> sgl_int32_args;
    FdoPtr<FdoArgumentDefinitionCollection> sgl_int64_args;
    FdoPtr<FdoArgumentDefinitionCollection> sgl_sgl_args;

    FdoPtr<FdoDataValueCollection>          values;
    FdoPtr<FdoPropertyValueConstraintList>  argument_value_list;

    FdoPtr<FdoSignatureDefinition>          signature;
    FdoPtr<FdoSignatureDefinitionCollection> signatures;

    // The function contains signatures that requires a second parameter for
    // which the value has to be one of YEAR, MONTH, DAY, HOUR or MINUTE. The
    // following sets up an argument value list to be used for the argument
    // describing this parameter.

    argument_value_list = FdoPropertyValueConstraintList::Create();
    values              = argument_value_list->GetConstraintList();
    values->Add(FdoStringValue::Create(L"YEAR"));
    values->Add(FdoStringValue::Create(L"MONTH"));
    values->Add(FdoStringValue::Create(L"DAY"));
    values->Add(FdoStringValue::Create(L"HOUR"));
    values->Add(FdoStringValue::Create(L"MINUTE"));

    // Get the descriptions for the arguments.

    arg1_description = FdoException::NLSGetMessage(
                                        FUNCTION_GENERAL_ARG,
                                        "Argument to be processed");

    date_op_arg_description =
            FdoException::NLSGetMessage(
                    FUNCTION_TRUNC_DATE_OP_ARG,
                    "Truncation indicator (YEAR, MONTH, DAY, HOUR, MINUTE)");

    num_opt_dec_arg_description =
        FdoException::NLSGetMessage(
                FUNCTION_TRUNC_OPT_DECIMAL_ARG,
                "Represents an optional number of decimals to truncate to");

    // The following defines the different argument definition collections.

    num_arg_literal =
            FdoException::NLSGetMessage(FUNCTION_NUMBER_ARG_LIT, "number");

    num_opt_dec_arg_literal =
                FdoException::NLSGetMessage(FUNCTION_OPT_DECIMAL_ARG_LIT,
                                            "optional number of decimals");

    date_arg_literal =
            FdoException::NLSGetMessage(FUNCTION_DATE_ARG_LIT, "date");

    date_op_arg_literal =
                    FdoException::NLSGetMessage(FUNCTION_OPERATOR_ARG_LIT,
                                                "Operation Indicator");

    date_arg    =
        FdoArgumentDefinition::Create(
            date_arg_literal, arg1_description, FdoDataType_DateTime);
    date_op_arg =
        FdoArgumentDefinition::Create(
            date_op_arg_literal, date_op_arg_description, FdoDataType_String);
    date_op_arg->SetArgumentValueList(argument_value_list);

    byte_arg  = FdoArgumentDefinition::Create(
                    num_arg_literal, arg1_description, FdoDataType_Byte);
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

    opt_byte_arg  = FdoArgumentDefinition::Create(num_opt_dec_arg_literal,
                                                  num_opt_dec_arg_description,
                                                  FdoDataType_Byte);
    opt_dcl_arg   = FdoArgumentDefinition::Create(num_opt_dec_arg_literal,
                                                  num_opt_dec_arg_description,
                                                  FdoDataType_Decimal);
    opt_dbl_arg   = FdoArgumentDefinition::Create(num_opt_dec_arg_literal,
                                                  num_opt_dec_arg_description,
                                                  FdoDataType_Double);
    opt_int16_arg = FdoArgumentDefinition::Create(num_opt_dec_arg_literal,
                                                  num_opt_dec_arg_description,
                                                  FdoDataType_Int16);
    opt_int32_arg = FdoArgumentDefinition::Create(num_opt_dec_arg_literal,
                                                  num_opt_dec_arg_description,
                                                  FdoDataType_Int32);
    opt_int64_arg = FdoArgumentDefinition::Create(num_opt_dec_arg_literal,
                                                  num_opt_dec_arg_description,
                                                  FdoDataType_Int64);
    opt_sgl_arg   = FdoArgumentDefinition::Create(num_opt_dec_arg_literal,
                                                  num_opt_dec_arg_description,
                                                  FdoDataType_Single);

    date_args = FdoArgumentDefinitionCollection::Create();
    date_args->Add(date_arg);
    date_args->Add(date_op_arg);

    byte_args = FdoArgumentDefinitionCollection::Create();
    byte_args->Add(byte_arg);

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

    byte_byte_args = FdoArgumentDefinitionCollection::Create();
    byte_byte_args->Add(byte_arg);
    byte_byte_args->Add(opt_byte_arg);

    byte_dcl_args = FdoArgumentDefinitionCollection::Create();
    byte_dcl_args->Add(byte_arg);
    byte_dcl_args->Add(opt_dcl_arg);

    byte_dbl_args = FdoArgumentDefinitionCollection::Create();
    byte_dbl_args->Add(byte_arg);
    byte_dbl_args->Add(opt_dbl_arg);

    byte_int16_args = FdoArgumentDefinitionCollection::Create();
    byte_int16_args->Add(byte_arg);
    byte_int16_args->Add(opt_int16_arg);

    byte_int32_args = FdoArgumentDefinitionCollection::Create();
    byte_int32_args->Add(byte_arg);
    byte_int32_args->Add(opt_int32_arg);

    byte_int64_args = FdoArgumentDefinitionCollection::Create();
    byte_int64_args->Add(byte_arg);
    byte_int64_args->Add(opt_int64_arg);

    byte_sgl_args = FdoArgumentDefinitionCollection::Create();
    byte_sgl_args->Add(byte_arg);
    byte_sgl_args->Add(opt_sgl_arg);

    dcl_byte_args = FdoArgumentDefinitionCollection::Create();
    dcl_byte_args->Add(dcl_arg);
    dcl_byte_args->Add(opt_byte_arg);

    dcl_dcl_args = FdoArgumentDefinitionCollection::Create();
    dcl_dcl_args->Add(dcl_arg);
    dcl_dcl_args->Add(opt_dcl_arg);

    dcl_dbl_args = FdoArgumentDefinitionCollection::Create();
    dcl_dbl_args->Add(dcl_arg);
    dcl_dbl_args->Add(opt_dbl_arg);

    dcl_int16_args = FdoArgumentDefinitionCollection::Create();
    dcl_int16_args->Add(dcl_arg);
    dcl_int16_args->Add(opt_int16_arg);

    dcl_int32_args = FdoArgumentDefinitionCollection::Create();
    dcl_int32_args->Add(dcl_arg);
    dcl_int32_args->Add(opt_int32_arg);

    dcl_int64_args = FdoArgumentDefinitionCollection::Create();
    dcl_int64_args->Add(dcl_arg);
    dcl_int64_args->Add(opt_int64_arg);

    dcl_sgl_args = FdoArgumentDefinitionCollection::Create();
    dcl_sgl_args->Add(dcl_arg);
    dcl_sgl_args->Add(opt_sgl_arg);

    dbl_byte_args = FdoArgumentDefinitionCollection::Create();
    dbl_byte_args->Add(dbl_arg);
    dbl_byte_args->Add(opt_byte_arg);

    dbl_dcl_args = FdoArgumentDefinitionCollection::Create();
    dbl_dcl_args->Add(dbl_arg);
    dbl_dcl_args->Add(opt_dcl_arg);

    dbl_dbl_args = FdoArgumentDefinitionCollection::Create();
    dbl_dbl_args->Add(dbl_arg);
    dbl_dbl_args->Add(opt_dbl_arg);

    dbl_int16_args = FdoArgumentDefinitionCollection::Create();
    dbl_int16_args->Add(dbl_arg);
    dbl_int16_args->Add(opt_int16_arg);

    dbl_int32_args = FdoArgumentDefinitionCollection::Create();
    dbl_int32_args->Add(dbl_arg);
    dbl_int32_args->Add(opt_int32_arg);

    dbl_int64_args = FdoArgumentDefinitionCollection::Create();
    dbl_int64_args->Add(dbl_arg);
    dbl_int64_args->Add(opt_int64_arg);

    dbl_sgl_args = FdoArgumentDefinitionCollection::Create();
    dbl_sgl_args->Add(dbl_arg);
    dbl_sgl_args->Add(opt_sgl_arg);

    int16_byte_args = FdoArgumentDefinitionCollection::Create();
    int16_byte_args->Add(int16_arg);
    int16_byte_args->Add(opt_byte_arg);

    int16_dcl_args = FdoArgumentDefinitionCollection::Create();
    int16_dcl_args->Add(int16_arg);
    int16_dcl_args->Add(opt_dcl_arg);

    int16_dbl_args = FdoArgumentDefinitionCollection::Create();
    int16_dbl_args->Add(int16_arg);
    int16_dbl_args->Add(opt_dbl_arg);

    int16_int16_args = FdoArgumentDefinitionCollection::Create();
    int16_int16_args->Add(int16_arg);
    int16_int16_args->Add(opt_int16_arg);

    int16_int32_args = FdoArgumentDefinitionCollection::Create();
    int16_int32_args->Add(int16_arg);
    int16_int32_args->Add(opt_int32_arg);

    int16_int64_args = FdoArgumentDefinitionCollection::Create();
    int16_int64_args->Add(int16_arg);
    int16_int64_args->Add(opt_int64_arg);

    int16_sgl_args = FdoArgumentDefinitionCollection::Create();
    int16_sgl_args->Add(int16_arg);
    int16_sgl_args->Add(opt_sgl_arg);

    int32_byte_args = FdoArgumentDefinitionCollection::Create();
    int32_byte_args->Add(int32_arg);
    int32_byte_args->Add(opt_byte_arg);

    int32_dcl_args = FdoArgumentDefinitionCollection::Create();
    int32_dcl_args->Add(int32_arg);
    int32_dcl_args->Add(opt_dcl_arg);

    int32_dbl_args = FdoArgumentDefinitionCollection::Create();
    int32_dbl_args->Add(int32_arg);
    int32_dbl_args->Add(opt_dbl_arg);

    int32_int16_args = FdoArgumentDefinitionCollection::Create();
    int32_int16_args->Add(int32_arg);
    int32_int16_args->Add(opt_int16_arg);

    int32_int32_args = FdoArgumentDefinitionCollection::Create();
    int32_int32_args->Add(int32_arg);
    int32_int32_args->Add(opt_int32_arg);

    int32_int64_args = FdoArgumentDefinitionCollection::Create();
    int32_int64_args->Add(int32_arg);
    int32_int64_args->Add(opt_int64_arg);

    int32_sgl_args = FdoArgumentDefinitionCollection::Create();
    int32_sgl_args->Add(int32_arg);
    int32_sgl_args->Add(opt_sgl_arg);

    int64_byte_args = FdoArgumentDefinitionCollection::Create();
    int64_byte_args->Add(int64_arg);
    int64_byte_args->Add(opt_byte_arg);

    int64_dcl_args = FdoArgumentDefinitionCollection::Create();
    int64_dcl_args->Add(int64_arg);
    int64_dcl_args->Add(opt_dcl_arg);

    int64_dbl_args = FdoArgumentDefinitionCollection::Create();
    int64_dbl_args->Add(int64_arg);
    int64_dbl_args->Add(opt_dbl_arg);

    int64_int16_args = FdoArgumentDefinitionCollection::Create();
    int64_int16_args->Add(int64_arg);
    int64_int16_args->Add(opt_int16_arg);

    int64_int32_args = FdoArgumentDefinitionCollection::Create();
    int64_int32_args->Add(int64_arg);
    int64_int32_args->Add(opt_int32_arg);

    int64_int64_args = FdoArgumentDefinitionCollection::Create();
    int64_int64_args->Add(int64_arg);
    int64_int64_args->Add(opt_int64_arg);

    int64_sgl_args = FdoArgumentDefinitionCollection::Create();
    int64_sgl_args->Add(int64_arg);
    int64_sgl_args->Add(opt_sgl_arg);

    sgl_byte_args = FdoArgumentDefinitionCollection::Create();
    sgl_byte_args->Add(sgl_arg);
    sgl_byte_args->Add(opt_byte_arg);

    sgl_dcl_args = FdoArgumentDefinitionCollection::Create();
    sgl_dcl_args->Add(sgl_arg);
    sgl_dcl_args->Add(opt_dcl_arg);

    sgl_dbl_args = FdoArgumentDefinitionCollection::Create();
    sgl_dbl_args->Add(sgl_arg);
    sgl_dbl_args->Add(opt_dbl_arg);

    sgl_int16_args = FdoArgumentDefinitionCollection::Create();
    sgl_int16_args->Add(sgl_arg);
    sgl_int16_args->Add(opt_int16_arg);

    sgl_int32_args = FdoArgumentDefinitionCollection::Create();
    sgl_int32_args->Add(sgl_arg);
    sgl_int32_args->Add(opt_int32_arg);

    sgl_int64_args = FdoArgumentDefinitionCollection::Create();
    sgl_int64_args->Add(sgl_arg);
    sgl_int64_args->Add(opt_int64_arg);

    sgl_sgl_args = FdoArgumentDefinitionCollection::Create();
    sgl_sgl_args->Add(sgl_arg);
    sgl_sgl_args->Add(opt_sgl_arg);

    // Create the signature collection.

    signatures = FdoSignatureDefinitionCollection::Create();

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_DateTime, date_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Byte, byte_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Decimal, dcl_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Double, dbl_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Int16, int16_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Int32, int32_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Int64, int64_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Single, sgl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Byte, byte_byte_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Byte, byte_dcl_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Byte, byte_dbl_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Byte, byte_int16_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Byte, byte_int32_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Byte, byte_int64_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Byte, byte_sgl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Decimal, dcl_byte_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Decimal, dcl_dcl_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Decimal, dcl_dbl_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Decimal, dcl_int16_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Decimal, dcl_int32_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Decimal, dcl_int64_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Decimal, dcl_sgl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Double, dbl_byte_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Double, dbl_dcl_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Double, dbl_dbl_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Double, dbl_int16_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Double, dbl_int32_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Double, dbl_int64_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Double, dbl_sgl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Int16, int16_byte_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Int16, int16_dcl_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Int16, int16_dbl_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Int16, int16_int16_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Int16, int16_int32_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Int16, int16_int64_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Int16, int16_sgl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Int32, int32_byte_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Int32, int32_dcl_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Int32, int32_dbl_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Int32, int32_int16_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Int32, int32_int32_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Int32, int32_int64_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Int32, int32_sgl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Int64, int64_byte_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Int64, int64_dcl_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Int64, int64_dbl_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Int64, int64_int16_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Int64, int64_int32_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Int64, int64_int64_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Int64, int64_sgl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Single, sgl_byte_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Single, sgl_dcl_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Single, sgl_dbl_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Single, sgl_int16_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Single, sgl_int32_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Single, sgl_int64_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Single, sgl_sgl_args);
    signatures->Add(signature);

    // Create the function definition.

    desc =
        FdoException::NLSGetMessage(
                    FUNCTION_TRUNC,
                    "Truncates a numeric or date expression as specified");
    function_definition =
                FdoFunctionDefinition::Create(
                                        FDO_FUNCTION_TRUNC,
                                        desc,
                                        false,
                                        signatures,
                                        FdoFunctionCategoryType_Numeric);

}  //  CreateFunctionDefinition ()

FdoLiteralValue *FdoFunctionTrunc::CreateReturnValue (FdoDataType data_type,
                                                      FdoDouble   d_value,
                                                      FdoFloat    f_value,
                                                      bool        no_value)

// +---------------------------------------------------------------------------
// | The function creates the function result object.
// +---------------------------------------------------------------------------

{

    switch (data_type) {

      case FdoDataType_Decimal:
        if (first)
        {
            return_data_value = FdoDecimalValue::Create();
            first = false;
        }
        if (no_value)
            return_data_value->SetNull();
        else
            (static_cast<FdoDecimalValue *> (return_data_value.p))->SetDecimal(d_value);
        return FDO_SAFE_ADDREF(return_data_value.p);
        break;

      case FdoDataType_Double:
        if (first)
        {
            return_data_value = FdoDoubleValue::Create();
            first = false;
        }
        if (no_value)
            return_data_value->SetNull();
        else
            (static_cast<FdoDoubleValue *> (return_data_value.p))->SetDouble(d_value);
        return FDO_SAFE_ADDREF(return_data_value.p);
        break;

      case FdoDataType_Single:
        if (first)
        {
            return_data_value = FdoSingleValue::Create();
            first = false;
        }
        if (no_value)
            return_data_value->SetNull();
        else
            (static_cast<FdoSingleValue *> (return_data_value.p))->SetSingle(f_value);
        return FDO_SAFE_ADDREF(return_data_value.p);
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
              FDO_FUNCTION_TRUNC));

}  //  CreateReturnValue ()

FdoDouble FdoFunctionTrunc::GetParameterValue (
                                FdoLiteralValueCollection *literal_values,
                                FdoDataType               data_type,
                                bool                      *is_NULL_value)

// +---------------------------------------------------------------------------
// | The function returns the value of the first function parameter.
// +---------------------------------------------------------------------------

{

    // Declare all necessary local variables and initialize them.

    FdoPtr<FdoDecimalValue> decimal_value;
    FdoPtr<FdoDoubleValue>  double_value;
    FdoPtr<FdoSingleValue>  single_value;

    // Process the identified parameter and return its value back to the
    // calling routine.

    *is_NULL_value = false;

    switch (data_type) {

      case FdoDataType_Decimal:
        decimal_value =(FdoDecimalValue *) literal_values->GetItem(0);
        if (!decimal_value->IsNull())
            return decimal_value->GetDecimal();
        else {

          *is_NULL_value = true;
          return 0;

        }  //  else ...
        break;

      case FdoDataType_Double:
        double_value = (FdoDoubleValue *) literal_values->GetItem(0);
        if (!double_value->IsNull())
            return double_value->GetDouble();
        else {

          *is_NULL_value = true;
          return 0;

        }  //  else ...
        break;

      case FdoDataType_Single:
        single_value = (FdoSingleValue *) literal_values->GetItem(0);
        if (!single_value->IsNull())
            return single_value->GetSingle();
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
              FDO_FUNCTION_TRUNC));

}  //  GetParameterValue ()

FdoDouble FdoFunctionTrunc::GetParameterValue (
                                FdoLiteralValueCollection *literal_values,
                                FdoDataType               data_type)

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
        byte_value = (FdoByteValue *) literal_values->GetItem(1);
        if (!byte_value->IsNull())
            return (FdoDouble) (byte_value->GetByte());
        else
          return 0;
        break;

      case FdoDataType_Decimal:
        decimal_value =(FdoDecimalValue *) literal_values->GetItem(1);
        if (!decimal_value->IsNull())
            return floor(decimal_value->GetDecimal());
        else
          return 0;
        break;

      case FdoDataType_Double:
        double_value = (FdoDoubleValue *) literal_values->GetItem(1);
        if (!double_value->IsNull())
            return floor(double_value->GetDouble());
        else
          return 0;
        break;

      case FdoDataType_Int16:
        int16_value = (FdoInt16Value *) literal_values->GetItem(1);
        if (!int16_value->IsNull())
            return (FdoDouble) (int16_value->GetInt16());
        else
          return 0;
        break;

      case FdoDataType_Int32:
        int32_value = (FdoInt32Value *) literal_values->GetItem(1);
        if (!int32_value->IsNull())
            return (FdoDouble) (int32_value->GetInt32());
        else
          return 0;
        break;

      case FdoDataType_Int64:
        int64_value = (FdoInt64Value *) literal_values->GetItem(1);
        if (!int64_value->IsNull())
            return (FdoDouble) (int64_value->GetInt64());
        else
          return 0;
        break;

      case FdoDataType_Single:
        single_value = (FdoSingleValue *) literal_values->GetItem(1);
        if (!single_value->IsNull())
            return floor(single_value->GetSingle());
        else
          return 0;
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
              FDO_FUNCTION_TRUNC));

}  //  GetParameterValue ()

FdoFunctionTrunc::DateTimeOperationTokens FdoFunctionTrunc::GetToken (
                                                        FdoStringP operation)

// +---------------------------------------------------------------------------
// | The function returns the token for the provided date truncate operator.
// +---------------------------------------------------------------------------

{

    if (FdoCommonStringUtil::StringCompareNoCase(operation, L"YEAR") == 0)
        return DateTimeOperationTokens_Year;

    if (FdoCommonStringUtil::StringCompareNoCase(operation, L"MONTH") == 0)
        return DateTimeOperationTokens_Month;

    if (FdoCommonStringUtil::StringCompareNoCase(operation, L"DAY") == 0)
        return DateTimeOperationTokens_Day;

    if (FdoCommonStringUtil::StringCompareNoCase(operation, L"HOUR") == 0)
        return DateTimeOperationTokens_Hour;

    if (FdoCommonStringUtil::StringCompareNoCase(operation, L"MINUTE") == 0)
        return DateTimeOperationTokens_Minute;

    // The function should never reach this part of the code, but just in case
    // an exception is issued.

    throw FdoException::Create(
            FdoException::NLSGetMessage(
              FUNCTION_OPERATOR_ERROR, 
              "Expression Engine: Invalid operator parameter value for function '%1$ls'",
              FDO_FUNCTION_TRUNC));

}  //  GetToken ()

FdoLiteralValue *FdoFunctionTrunc::ProcessDateTruncationRequest (
                                    FdoLiteralValueCollection *literal_values)

// +---------------------------------------------------------------------------
// | The function process a request where the truncate operation is executed
// | on a date/time argument. For truncating a date, the following rules apply:
// |
// |  TRUNCATE TO   RESULT
// | ---------------------------------------------------------------------
// |   YEAR         January 1st of the provided year. If time information
// |                is available, it defaults to 00:00:00.
// |   MONTH        The first of the month.  If time information is
// |                available, it defaults to 00:00:00.
// |   DAY          The Sunday of that week.
// |   HOUR         The current hour identified by the provided value
// |   MINUTE       The current minute identified by the provided value
// |
// | The following lists examples for the different options. The date/time
// | value used in this example is:07-SEP-2007 15:45:41                                                 
// |
// |  TRUNCATE TO   RESULT
// | ---------------------------------------------------------------------
// |   YEAR         01-JAN-2007 00:00:00
// |   MONTH        01-SEP-2007 00:00:00
// |   DAY          02-SEP-2007 00:00:00
// |   HOUR         07-SEP-2007 15:00:00
// |   MINUTE       07-SEP-2007 15:45:00
// |
// | A request will always work on the data presented with the provided value.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    bool                     truncation_done = false;

    FdoInt16                 year;

    FdoInt8                  month,
                             day,
                             hour            = 0,
                             minute          = 0;

    FdoFloat                 seconds         = 0;

    FdoDateTime              date_time;
 
    FdoPtr<FdoDateTimeValue> date_time_value; 


    if (first)
    {
        return_data_value = FdoDateTimeValue::Create();
        first = false;
    }
    // Get the value to be processed. If no data is provided, create an empty
    // object being returned and terminate the function.

    date_time_value = (FdoDateTimeValue *) literal_values->GetItem(0);
    if (date_time_value->IsNull())
    {
        return_data_value->SetNull();
        return FDO_SAFE_ADDREF(return_data_value.p);
    }

    date_time = date_time_value->GetDateTime();

    switch (GetToken(date_operator)) {

      case DateTimeOperationTokens_Year:

        if ((date_time.year  != -1) &&
            (date_time.month != -1) &&
            (date_time.day   != -1)    ) {

            year            = date_time.year;
            month           = 1;
            day             = 1;

            truncation_done = true;
            
        }  //  if ((date_time.year  != -1) ...

        break;

      case DateTimeOperationTokens_Month:

        if ((date_time.year  != -1) &&
            (date_time.month != -1) &&
            (date_time.day   != -1)    ) {

            year            = date_time.year;
            month           = date_time.month;
            day             = 1;

            truncation_done = true;
            
        }  //  if ((date_time.year  != -1) ...

        break;

      case DateTimeOperationTokens_Day:

        // DateTime dt;

        break;

      case DateTimeOperationTokens_Hour:

        if ((date_time.hour != -1  ) &&
            (date_time.minute != -1)    ) {

            hour            = date_time.hour;
            truncation_done = true;

            if ((date_time.year  != -1) &&
                (date_time.month != -1) &&
                (date_time.day   != -1)    ) {

                year  = date_time.year;
                month = date_time.month;
                day   = date_time.day;
            
            }  //  if ((date_time.year  != -1) ...
            
        }  //  if ((date_time.hour != -1  ) ...

        break;

      case DateTimeOperationTokens_Minute:

        if ((date_time.hour != -1  ) &&
            (date_time.minute != -1)    ) {

            hour            = date_time.hour;
            minute          = date_time.minute;
            truncation_done = true;

            if ((date_time.year  != -1) &&
                (date_time.month != -1) &&
                (date_time.day   != -1)    ) {

                year  = date_time.year;
                month = date_time.month;
                day   = date_time.day;
            
            }  //  if ((date_time.year  != -1) ...
            
        }  //  if if ((date_time.hour != -1  ) ...

        break;

    }  //  switch ...

    if (truncation_done)
    {
        FdoDateTimeValue *dateTimeValue = static_cast<FdoDateTimeValue *> (return_data_value.p);
        date_time.year = year;
        date_time.month = month;
        date_time.day = day;
        date_time.hour = hour;
        date_time.minute = minute;
        date_time.seconds = seconds;
        dateTimeValue->SetDateTime(date_time);
    
    }
    else
    {
        return_data_value->SetNull();
    }
    return FDO_SAFE_ADDREF(return_data_value.p);

}  //  ProcessDateTruncationRequest ()

FdoLiteralValue *FdoFunctionTrunc::ProcessNumericTruncationRequest (
                                    FdoLiteralValueCollection *literal_values)

// +---------------------------------------------------------------------------
// | The function process a request where the truncate operation is executed
// | on a numeric argument.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    bool                  is_NULL_value = false;

    FdoFloat              f_result      = 0;

    FdoDouble             p1            = 0,
                          p2            = 0,
                          p_tmp         = 0,
                          d_result      = 0,
                          tmp_result    = 0;

    FdoInt64              multiplier    = 0;

    FdoPtr<FdoByteValue>  byte_value;
    FdoPtr<FdoInt16Value> int16_value;
    FdoPtr<FdoInt32Value> int32_value;
    FdoPtr<FdoInt64Value> int64_value;

    // If the data type for the first argument is an integer type, no further
    // processing is required and the function result can be returned back to
    // the calling routine.

    switch (para1_data_type) {

      case FdoDataType_Byte:
        if (first)
        {
            return_data_value = FdoByteValue::Create();
            first = false;
        }
        byte_value = (FdoByteValue *) literal_values->GetItem(0);
        if (!byte_value->IsNull())
            (static_cast<FdoByteValue *> (return_data_value.p))->SetByte(byte_value->GetByte());
        else
            return_data_value->SetNull();
        return FDO_SAFE_ADDREF(return_data_value.p);
        break;

      case FdoDataType_Int16:
        if (first)
        {
            return_data_value = FdoInt16Value::Create();
            first = false;
        }
        int16_value = (FdoInt16Value *) literal_values->GetItem(0);
        if (!int16_value->IsNull())
            (static_cast<FdoInt16Value *> (return_data_value.p))->SetInt16(int16_value->GetInt16());
        else
            return_data_value->SetNull();
        return FDO_SAFE_ADDREF(return_data_value.p);
        break;

      case FdoDataType_Int32:
        if (first)
        {
            return_data_value = FdoInt32Value::Create();
            first = false;
        }
        int32_value = (FdoInt32Value *) literal_values->GetItem(0);
        if (!int32_value->IsNull())
            (static_cast<FdoInt32Value *> (return_data_value.p))->SetInt32(int32_value->GetInt32());
        else
            return_data_value->SetNull();
        return FDO_SAFE_ADDREF(return_data_value.p);
        break;

      case FdoDataType_Int64:
        if (first)
        {
            return_data_value = FdoInt64Value::Create();
            first = false;
        }
        int64_value = (FdoInt64Value *) literal_values->GetItem(0);
        if (!int64_value->IsNull())
            (static_cast<FdoInt64Value *> (return_data_value.p))->SetInt64(int64_value->GetInt64());
        else
            return_data_value->SetNull();
        return FDO_SAFE_ADDREF(return_data_value.p);
        break;

    }  //  switch ...

    // The value to be processed is not of an INTEGER type and hence further
    // processing is required. Next, get the value to be handled. If the value
    // is NULL, no further steps need to be executed.

    p1 = GetParameterValue(literal_values, para1_data_type, &is_NULL_value);
    if (is_NULL_value)
        return CreateReturnValue(para1_data_type, 0, 0, true);

    // If the function was invoked with two parameters then get the number of
    // digits after the decimal point to which to truncate. If this is not
    // specified, it is assumed that all digits after the decimal point should
    // be removed.

    if (number_of_parameters == 2) 
        p2 = GetParameterValue(literal_values, para2_data_type);

    // Calculate the result. If the number of digits after the decimal point
    // left after the operation is 0, then truncate the fractial and return
    // the resulting value back to the calling routine.

    if (p2 == 0) {

        if ((para1_data_type == FdoDataType_Decimal) ||
            (para1_data_type == FdoDataType_Double )    ) {

            d_result = floor(p1); 
            return CreateReturnValue(para1_data_type, d_result, 0, false);

        }  //  if ((para1_data_type == FdoDataType_Decimal) ...
        else {

          f_result = (FdoFloat) (floor(p1)); 
          return CreateReturnValue(para1_data_type, 0, f_result, false);

        }  //  else ...

    }  //  if (p2 == 0) ...

    // The operation should be done to the precision of some digits after the
    // decimal points. To calculate the result, multiply the given value by
    // a multiple of 10 as indicated by the number of digits after the decimal
    // point to be left, do the truncation on the temporary value and then undo
    // the temporary multiplication.

    multiplier = (FdoInt64) (pow(10, p2));
    p_tmp      = (p1 * multiplier);
    tmp_result = floor(p_tmp); 

    if ((para1_data_type == FdoDataType_Decimal) ||
        (para1_data_type == FdoDataType_Double )    ) {

        d_result = tmp_result / multiplier; 
        return CreateReturnValue(para1_data_type, d_result, 0, false);

    }  //  if ((para1_data_type == FdoDataType_Decimal) ...
    else {

      f_result = (FdoFloat) (tmp_result / multiplier); 
      return CreateReturnValue(para1_data_type, 0, f_result, false);

    }  //  else ...

    // At this point all valid scenarios should have been processed and a re-
    // sult returned to the calling routine. Just in case there is an issue,
    // the following exception is thrown if this part of the function imple-
    // mentation is reached.

    throw FdoException::Create(
            FdoException::NLSGetMessage(
              FUNCTION_UNEXPECTED_RESULT_ERROR, 
              "Expression Engine: Unexpected result for function '%1$ls'",
              FDO_FUNCTION_TRUNC));

}  //  ProcessNumericTruncationRequest ()

void FdoFunctionTrunc::Validate (FdoLiteralValueCollection *literal_values)

// +---------------------------------------------------------------------------
// | The function validates the argument list that was passed in.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoInt32                i               = 0;

    FdoDataValue            *data_value     = NULL;

    FdoStringValue          *str_value      = NULL;

    FdoPtr<FdoLiteralValue> literal_value;

    // Reset a flag.

    is_date_truncation = false;

    // The function works on different argument types. Depending on the type
    // of the first argument, the caller may or must provide a second argument
    // when invoking the function. The following checks whether or not the 
    // correct number of arguments that this function may accept are provided.
    // If this is not the case an exception is issued. Further, type dependent
    // checks on the number of arguments are done later.

    number_of_parameters = literal_values->GetCount();

    if ((number_of_parameters < 1) && (number_of_parameters > 2)) 
        throw FdoException::Create(
                FdoException::NLSGetMessage(
                  FUNCTION_PARAMETER_NUMBER_ERROR, 
                  "Expression Engine: Invalid number of parameters for function '%1$ls'",
                  FDO_FUNCTION_TRUNC));

    // Next, process the provided arguments and verify the data types for the
    // arguments match the possible function signatures. If this is not the
    // case issue an exception.

    for (i = 0; i < number_of_parameters; i++) {

      literal_value = literal_values->GetItem(i);
      if (literal_value->GetLiteralValueType() != FdoLiteralValueType_Data)
          throw FdoException::Create(
                  FdoException::NLSGetMessage(
                      FUNCTION_PARAMETER_ERROR, 
                      "Expression Engine: Invalid parameters for function '%1$ls'",
                      FDO_FUNCTION_TRUNC));

      data_value = static_cast<FdoDataValue *>(literal_value.p);
      if (i == 0)
          para1_data_type = data_value->GetDataType();
      else
        para2_data_type = data_value->GetDataType();

    }  //  for (i = 0; i < number_of_parameters; i++) ...

    // Execute the checks that depend on the data type of the first parameter.
    // The check handles the case where the first argument is of type DateTime.
    // In this case, there must be a second function argument of type String
    // and with a value of a predefined set of possible values. If there is
    // something not matching the expectation, issue an exception.

    if (para1_data_type == FdoDataType_DateTime) {

        if (number_of_parameters != 2)
            throw FdoException::Create(
                FdoException::NLSGetMessage(
                  FUNCTION_PARAMETER_NUMBER_ERROR, 
                  "Expression Engine: Invalid number of parameters for function '%1$ls'",
                  FDO_FUNCTION_TRUNC));

        if (para2_data_type != FdoDataType_String)
            throw FdoException::Create(
                FdoException::NLSGetMessage(
                  FUNCTION_PARAMETER_DATA_TYPE_ERROR, 
                  "Expression Engine: Invalid parameter data type for function '%1$ls'",
                  FDO_FUNCTION_TRUNC));

        literal_value = literal_values->GetItem(1);
        str_value     = static_cast<FdoStringValue *>(literal_value.p);
        if (str_value->IsNull())
            throw FdoException::Create(
              FdoException::NLSGetMessage(
                FUNCTION_OPERATOR_ERROR, 
                "Expression Engine: Invalid operator parameter value for function '%1$ls'",
                FDO_FUNCTION_TRUNC));

        date_operator = str_value->GetString();
        if ((FdoCommonStringUtil::StringCompareNoCase(
                                        date_operator, L"YEAR"  ) != 0) &&
            (FdoCommonStringUtil::StringCompareNoCase(
                                        date_operator, L"MONTH" ) != 0) &&
            (FdoCommonStringUtil::StringCompareNoCase(
                                        date_operator, L"DAY"   ) != 0) &&
            (FdoCommonStringUtil::StringCompareNoCase(
                                        date_operator, L"HOUR"  ) != 0) &&
            (FdoCommonStringUtil::StringCompareNoCase(
                                        date_operator, L"MINUTE") != 0)    )
            throw FdoException::Create(
              FdoException::NLSGetMessage(
                FUNCTION_OPERATOR_ERROR, 
                "Expression Engine: Invalid operator parameter value for function '%1$ls'",
                FDO_FUNCTION_TRUNC));

        // Remember that the operation is to be on a date argument.

        is_date_truncation = true;

        // All checks are complete for this case and the function can be
        // terminated.

        return;

    }  //  if (para1_data_type == FdoDataType_DateTime) ...

    // The next check handles the case where the first parameter is a numeric
    // data type. In this case, there may be a second parameter of a numeric
    // type. If there is something not matching the expectation, issue an
    // exception.

    if ((para1_data_type != FdoDataType_Byte   ) &&
        (para1_data_type != FdoDataType_Decimal) &&
        (para1_data_type != FdoDataType_Double ) &&
        (para1_data_type != FdoDataType_Int16  ) &&
        (para1_data_type != FdoDataType_Int32  ) &&
        (para1_data_type != FdoDataType_Int64  ) &&
        (para1_data_type != FdoDataType_Single )    )
        throw FdoException::Create(
                FdoException::NLSGetMessage(
                  FUNCTION_PARAMETER_DATA_TYPE_ERROR, 
                  "Expression Engine: Invalid parameter data type for function '%1$ls'",
                  FDO_FUNCTION_TRUNC));

    if (number_of_parameters == 2) {

        if ((para2_data_type != FdoDataType_Byte   ) &&
            (para2_data_type != FdoDataType_Decimal) &&
            (para2_data_type != FdoDataType_Double ) &&
            (para2_data_type != FdoDataType_Int16  ) &&
            (para2_data_type != FdoDataType_Int32  ) &&
            (para2_data_type != FdoDataType_Int64  ) &&
            (para2_data_type != FdoDataType_Single )    )
            throw FdoException::Create(
                FdoException::NLSGetMessage(
                  FUNCTION_PARAMETER_DATA_TYPE_ERROR, 
                  "Expression Engine: Invalid parameter data type for function '%1$ls'",
                  FDO_FUNCTION_TRUNC));

    }  //  if (number_of_parameters == 2) ...

}  //  Validate ()

