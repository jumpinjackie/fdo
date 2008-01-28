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
#include <Functions/Numeric/FdoFunctionRound.h>


// ----------------------------------------------------------------------------
// --                         Constructors/Destructors                       --
// ----------------------------------------------------------------------------

FdoFunctionRound::FdoFunctionRound ()

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

    para1_data_type      = FdoDataType_CLOB;
    para2_data_type      = FdoDataType_CLOB;

    first = true;

}  //  FdoFunctionRound ()


FdoFunctionRound::~FdoFunctionRound ()

// +---------------------------------------------------------------------------
// | The class destructor.
// +---------------------------------------------------------------------------

{

    // Delete the function definition.

    FDO_SAFE_RELEASE(function_definition);

}  //  ~FdoFunctionRound ()


// ----------------------------------------------------------------------------
// --                            Public Class APIs                           --
// ----------------------------------------------------------------------------

FdoFunctionRound *FdoFunctionRound::Create ()

// +---------------------------------------------------------------------------
// | The function creates a new instance of the class.
// +---------------------------------------------------------------------------

{

    return new FdoFunctionRound();

}  //  Create ()

FdoFunctionRound *FdoFunctionRound::CreateObject ()

// +---------------------------------------------------------------------------
// | The function creates a new instance of the class.
// +---------------------------------------------------------------------------

{

    return new FdoFunctionRound();

}  //  CreateObject ()

FdoFunctionDefinition *FdoFunctionRound::GetFunctionDefinition ()

// +---------------------------------------------------------------------------
// | The function creates the supported signature list for the function ROUND.
// +---------------------------------------------------------------------------

{

    if (function_definition == NULL)
        CreateFunctionDefinition();

    return FDO_SAFE_ADDREF(function_definition);

}  //  GetFunctionDefinition ()

FdoLiteralValue *FdoFunctionRound::Evaluate (
                                    FdoLiteralValueCollection *literal_values)

// +---------------------------------------------------------------------------
// | The function processes a call to the function ROUND.
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

    if (first)
    {
        Validate(literal_values);
        // first will be clear after the return object is created
    }

    // The value to be processed is the first function parameter. If this para-
    // meter is of type BYTE, INT16, INT32 or INT64 nothing further needs to be
    // done and the function result can be returned to the calling routine.

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
    // digits after the decimal point to which to round to. If this is not
    // specified, it is assumed that all digits after the decimal point should
    // be removed.

    if (number_of_parameters == 2) 
        p2 = GetParameterValue(literal_values, para2_data_type);

    // Calculate the result. If the number of digits after the decimal point
    // left after the operation is 0, then round the value based on the part
    // after the decimal point and return the result back to the calling pro-
    // cedure.

    if (p2 == 0) {

        if ((para1_data_type == FdoDataType_Decimal) ||
            (para1_data_type == FdoDataType_Double )    ) {

            d_result = ((p1 - floor(p1)) < 0.5)
                    ? floor(p1)
                    : ceil(p1); 

            return CreateReturnValue(para1_data_type, d_result, 0, false);

        }  //  if ((para1_data_type == FdoDataType_Decimal) ...
        else {

          f_result = (FdoFloat) (((p1 - floor(p1)) < 0.5)
                                    ? floor(p1)
                                    : ceil(p1)); 

          return CreateReturnValue(para1_data_type, 0, f_result, false);

        }  //  else ...

    }  //  if (p2 == 0) ...

    // The rounding should be done to the precision of some digits after the
    // decimal points. To calculate the result, multiply the given value by
    // a multiple of 10 as indicated by the number of digits after the decimal
    // point to be left, do the rounding on the temporary value and then undo
    // the temporary multiplication.

    multiplier = (FdoInt64) (pow(10, p2));
    p_tmp      = (p1 * multiplier);
    tmp_result = ((p_tmp - floor(p_tmp)) < 0.5)
                    ? floor(p_tmp)
                    : ceil(p_tmp); 

    if ((para1_data_type == FdoDataType_Decimal) ||
        (para1_data_type == FdoDataType_Double )    ) {

        d_result = tmp_result / multiplier; 
        return CreateReturnValue(para1_data_type, d_result, 0, false);

    }  //  if ((para1_data_type == FdoDataType_Decimal) ...
    else {

      f_result = (FdoFloat) (tmp_result / multiplier); 
      return CreateReturnValue(para1_data_type, 0, f_result, false);

    }  //  else ...

    // At this point all valid scenarios should have been procesed and a re-
    // sult returned to the calling routine. Just in case there is an issue,
    // the following exception is thrown if this part of the function imple-
    // mentation is reached.

    throw FdoException::Create(
            FdoException::NLSGetMessage(
              FUNCTION_UNEXPECTED_RESULT_ERROR, 
              "Expression Engine: Unexpected result for function '%1$ls'",
              FDO_FUNCTION_ROUND));

}  //  Evaluate ()


// ----------------------------------------------------------------------------
// --                          Supporting functions                          --
// ----------------------------------------------------------------------------

void FdoFunctionRound::CreateFunctionDefinition ()

// +---------------------------------------------------------------------------
// | The procedure creates the function definition for the function ROUND. The
// | function definition includes the list of supported signatures. The follow-
// | ing signatures are supported:
// |
// |    ROUND ({byte, decimal, double, int16, int32, int64, single}
// |           [, {byte, decimal, double, int16, int32, int64, single}] )
// |
// | The function returns a data type depending on the provided data types.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoString                               *desc               = NULL;

    FdoStringP                              arg1_description;
    FdoStringP                              arg2_description;
    FdoStringP                              num_arg_literal;
    FdoStringP                              opt_dec_arg_literal;

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

    FdoPtr<FdoSignatureDefinition>          signature;
    FdoPtr<FdoSignatureDefinitionCollection> signatures;

    // Get the general descriptions for the arguments.

    arg1_description = FdoException::NLSGetMessage(
                                        FUNCTION_GENERAL_ARG,
                                        "Argument to be processed");

    arg2_description =
            FdoException::NLSGetMessage(
                    FUNCTION_TRUNC_OPT_DECIMAL_ARG,
                    "Represents an optional number of decimals to round to");

    // The following defines the different argument definition collections.

    num_arg_literal =
            FdoException::NLSGetMessage(FUNCTION_NUMBER_ARG_LIT, "number");

    opt_dec_arg_literal =
                FdoException::NLSGetMessage(FUNCTION_OPT_DECIMAL_ARG_LIT,
                                            "optional number of decimals");

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

    opt_byte_arg  =
            FdoArgumentDefinition::Create(
                opt_dec_arg_literal, arg2_description, FdoDataType_Byte);
    opt_dcl_arg   =
            FdoArgumentDefinition::Create(
                opt_dec_arg_literal, arg2_description, FdoDataType_Decimal);
    opt_dbl_arg   =
            FdoArgumentDefinition::Create(
                opt_dec_arg_literal, arg2_description, FdoDataType_Double);
    opt_int16_arg =
            FdoArgumentDefinition::Create(
                opt_dec_arg_literal, arg2_description, FdoDataType_Int16);
    opt_int32_arg =
            FdoArgumentDefinition::Create(
                opt_dec_arg_literal, arg2_description, FdoDataType_Int32);
    opt_int64_arg =
            FdoArgumentDefinition::Create(
                opt_dec_arg_literal, arg2_description, FdoDataType_Int64);
    opt_sgl_arg   =
            FdoArgumentDefinition::Create(
                opt_dec_arg_literal, arg2_description, FdoDataType_Single);

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

    signature = FdoSignatureDefinition::Create(FdoDataType_Byte, byte_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(FdoDataType_Decimal, dcl_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(FdoDataType_Double, dbl_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(FdoDataType_Int16, int16_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(FdoDataType_Int32, int32_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(FdoDataType_Int64, int64_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(FdoDataType_Single, sgl_args);
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
                    FUNCTION_ROUND,
                    "Rounds a numeric expressions to the specified degree");
    function_definition =
                FdoFunctionDefinition::Create(
                                        FDO_FUNCTION_ROUND,
                                        desc,
                                        false,
                                        signatures,
                                        FdoFunctionCategoryType_Numeric);

}  //  CreateFunctionDefinition ()

FdoLiteralValue *FdoFunctionRound::CreateReturnValue (FdoDataType data_type,
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
              FDO_FUNCTION_ROUND));

}  //  CreateReturnValue ()

FdoDouble FdoFunctionRound::GetParameterValue (
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
              FDO_FUNCTION_ROUND));

}  //  GetParameterValue ()

FdoDouble FdoFunctionRound::GetParameterValue (
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
              FDO_FUNCTION_ROUND));

}  //  GetParameterValue ()

void FdoFunctionRound::Validate (FdoLiteralValueCollection *literal_values)

// +---------------------------------------------------------------------------
// | The function validates the argument list that was passed in.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoInt32                i               = 0;

    FdoDataValue            *data_value     = NULL;

    FdoPtr<FdoLiteralValue> literal_value;

    // Check the number of arguments. ROUND accepts either one or two numeric
    // parameters. If the number of parameters is not correct issue an excep-
    // tion.

    number_of_parameters = literal_values->GetCount();

    if ((number_of_parameters < 1) && (number_of_parameters > 2)) 
        throw FdoException::Create(
                FdoException::NLSGetMessage(
                  FUNCTION_PARAMETER_NUMBER_ERROR, 
                  "Expression Engine: Invalid number of parameters for function '%1$ls'",
                  FDO_FUNCTION_ROUND));

    // Next, identify the data type associated with the value to be processed.
    // An exception is issued if the data type does not match any of the ones
    // the function supports

    for (i = 0; i < number_of_parameters; i++) {

      literal_value = literal_values->GetItem(i);
      if (literal_value->GetLiteralValueType() != FdoLiteralValueType_Data)
          throw FdoException::Create(
                  FdoException::NLSGetMessage(
                      FUNCTION_PARAMETER_ERROR, 
                      "Expression Engine: Invalid parameters for function '%1$ls'",
                      FDO_FUNCTION_ROUND));

      data_value = static_cast<FdoDataValue *>(literal_value.p);
      if (i == 0)
          para1_data_type = data_value->GetDataType();
      else
        para2_data_type = data_value->GetDataType();

    }  //  for (i = 0; i < number_of_parameters; i++) ...

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
                  FDO_FUNCTION_ROUND));

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
                  FDO_FUNCTION_ROUND));

    }  //  if (number_of_parameters == 2) ...

}  //  Validate ()

