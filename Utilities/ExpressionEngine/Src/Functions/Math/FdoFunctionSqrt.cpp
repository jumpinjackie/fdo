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
#include <Functions/Math/FdoFunctionSqrt.h>


// ----------------------------------------------------------------------------
// --                         Constructors/Destructors                       --
// ----------------------------------------------------------------------------

FdoFunctionSqrt::FdoFunctionSqrt ()

// +---------------------------------------------------------------------------
// | The class constructor.
// +---------------------------------------------------------------------------

{

    // Initialize all class variables.
    // NOTE: Due to the fact that data type enumeration misses an entry to
    //       indicate a not-set value, the variable "incoming_data_type" is
    //       set to "FdoDataType_CLOB" to indicate an invalid data type be-
    //       cause this function does not support this type. 

    function_definition = NULL;

    incoming_data_type  = FdoDataType_CLOB;

}  //  FdoFunctionSqrt ()


FdoFunctionSqrt::~FdoFunctionSqrt ()

// +---------------------------------------------------------------------------
// | The class destructor.
// +---------------------------------------------------------------------------

{

    // Delete the function definition.

    FDO_SAFE_RELEASE(function_definition);

}  //  ~FdoFunctionSqrt ()


// ----------------------------------------------------------------------------
// --                            Public Class APIs                           --
// ----------------------------------------------------------------------------

FdoFunctionSqrt *FdoFunctionSqrt::Create ()

// +---------------------------------------------------------------------------
// | The function creates a new instance of the class.
// +---------------------------------------------------------------------------

{

    return new FdoFunctionSqrt();

}  //  Create ()

FdoFunctionSqrt *FdoFunctionSqrt::CreateObject ()

// +---------------------------------------------------------------------------
// | The function creates a new instance of the class.
// +---------------------------------------------------------------------------

{

    return new FdoFunctionSqrt();

}  //  CreateObject ()

FdoFunctionDefinition *FdoFunctionSqrt::GetFunctionDefinition ()

// +---------------------------------------------------------------------------
// | The function creates the supported signature list for the function SQRT.
// +---------------------------------------------------------------------------

{

    if (function_definition == NULL)
        CreateFunctionDefinition();

    return FDO_SAFE_ADDREF(function_definition);

}  //  GetFunctionDefinition ()

FdoLiteralValue *FdoFunctionSqrt::Evaluate (
                                    FdoLiteralValueCollection *literal_values)

// +---------------------------------------------------------------------------
// | The function processes a call to the function SQRT.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoPtr<FdoDecimalValue> decimal_value;
    FdoPtr<FdoDoubleValue>  double_value;
    FdoPtr<FdoInt16Value>   int16_value;
    FdoPtr<FdoInt32Value>   int32_value;
    FdoPtr<FdoInt64Value>   int64_value;
    FdoPtr<FdoSingleValue>  single_value;

    // Validate the function call.

    Validate(literal_values);

    // Process the request and return the result back to the calling routine.

    switch (incoming_data_type) {

      case FdoDataType_Decimal:
        decimal_value =(FdoDecimalValue *) literal_values->GetItem(0);
        if (!decimal_value->IsNull()) {

            if (decimal_value->GetDecimal() >= 0)
                return FdoDoubleValue::Create(
                                        sqrt(decimal_value->GetDecimal()));

        }  //  if (!decimal_value->IsNull()) ...
        else
          return FdoDoubleValue::Create();
        break;

      case FdoDataType_Double:
        double_value = (FdoDoubleValue *) literal_values->GetItem(0);
        if (!double_value->IsNull()) {

            if (double_value->GetDouble() >= 0)
                return FdoDoubleValue::Create(
                                        sqrt(double_value->GetDouble()));

        }  //  if (!double_value->IsNull()) ...
        else
          return FdoDoubleValue::Create();
        break;

      case FdoDataType_Int16:
        int16_value = (FdoInt16Value *) literal_values->GetItem(0);
        if (!int16_value->IsNull()) {

            if (int16_value->GetInt16() >= 0)
                return FdoDoubleValue::Create(
                                        sqrt((double)int16_value->GetInt16()));

        }  //  if (!int16_value->IsNull()) ...
        else
          return FdoDoubleValue::Create();
        break;

      case FdoDataType_Int32:
        int32_value = (FdoInt32Value *) literal_values->GetItem(0);
        if (!int32_value->IsNull()) {

            if (int32_value->GetInt32() >= 0)
                return FdoDoubleValue::Create(
                                        sqrt((double)int32_value->GetInt32()));

        }  //  if (!int32_value->IsNull()) ...
        else
          return FdoDoubleValue::Create();
        break;

      case FdoDataType_Int64:
        int64_value = (FdoInt64Value *) literal_values->GetItem(0);
        if (!int64_value->IsNull()) {

            if (int64_value->GetInt64() >= 0)
                return FdoDoubleValue::Create(
                                        sqrt((double)int64_value->GetInt64()));

        }  //  if (!int64_value->IsNull()) ...
        else
          return FdoDoubleValue::Create();
        break;

      case FdoDataType_Single:
        single_value = (FdoSingleValue *) literal_values->GetItem(0);
        if (!single_value->IsNull()) {

            if (single_value->GetSingle() >= 0)
                return FdoDoubleValue::Create(
                                        sqrt(single_value->GetSingle()));

        }  //  if (!single_value->IsNull()) ...
        else
          return FdoDoubleValue::Create();
        break;

    }  //  switch ...

    // Because of the validation of the request at the beginning of the
    // function, any use of an invalid data type as a parameter has already
    // been caught. Therefore, the only reason why the switch statement is
    // exited without returning a result is if there was an invalid value
    // provided. In this case issue an exception. 

    throw FdoException::Create(
            FdoException::NLSGetMessage(
                FUNCTION_DATA_VALUE_ERROR, 
                "Expression Engine: Invalid value for execution of function '%1$ls'",
                FDO_FUNCTION_SQRT));

}  //  Evaluate ()


// ----------------------------------------------------------------------------
// --                          Supporting functions                          --
// ----------------------------------------------------------------------------

void FdoFunctionSqrt::CreateFunctionDefinition ()

// +---------------------------------------------------------------------------
// | The procedure creates the function definition for the function SQRT. The
// | function definition includes the list of supported signatures. The follow-
// | ing signatures are supported:
// |
// |    SQRT ({decimal, double, int16, int32, int64, single})
// |
// | The function always returns a DOUBLE.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoString *desc = NULL;

    FdoStringP arg1_description;
    FdoStringP num_arg_literal;

    FdoPtr<FdoArgumentDefinition> dcl_arg;
    FdoPtr<FdoArgumentDefinition> dbl_arg;
    FdoPtr<FdoArgumentDefinition> int16_arg;
    FdoPtr<FdoArgumentDefinition> int32_arg;
    FdoPtr<FdoArgumentDefinition> int64_arg;
    FdoPtr<FdoArgumentDefinition> sgl_arg;

    FdoPtr<FdoArgumentDefinitionCollection> dcl_args;
    FdoPtr<FdoArgumentDefinitionCollection> dbl_args;
    FdoPtr<FdoArgumentDefinitionCollection> int16_args;
    FdoPtr<FdoArgumentDefinitionCollection> int32_args;
    FdoPtr<FdoArgumentDefinitionCollection> int64_args;
    FdoPtr<FdoArgumentDefinitionCollection> sgl_args;

    FdoPtr<FdoSignatureDefinition>          signature;
    FdoSignatureDefinitionCollection        *signatures;

    // Get the general descriptions for the arguments.

    arg1_description = FdoException::NLSGetMessage(
                                        FUNCTION_GENERAL_ARG,
                                        "Argument to be processed");

    // The following defines the different argument definition collections.

    num_arg_literal =
            FdoException::NLSGetMessage(FUNCTION_NUMBER_ARG_LIT, "number");

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

    // Create the signature collection.

    signatures = FdoSignatureDefinitionCollection::Create();

    signature = FdoSignatureDefinition::Create(FdoDataType_Double, dcl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(FdoDataType_Double, dbl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(FdoDataType_Double, int16_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(FdoDataType_Double, int32_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(FdoDataType_Double, int64_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(FdoDataType_Double, sgl_args);
    signatures->Add(signature);

    // Create the function definition.

    desc =
        FdoException::NLSGetMessage(
                        FUNCTION_SQRT,
                        "Determines the square root of a numeric expression");
    function_definition =
                FdoFunctionDefinition::Create(
                                        FDO_FUNCTION_SQRT,
                                        desc,
                                        false,
                                        signatures,
                                        FdoFunctionCategoryType_Math);

}  //  CreateFunctionDefinition ()

void FdoFunctionSqrt::Validate (FdoLiteralValueCollection *literal_values)

// +---------------------------------------------------------------------------
// | The function validates the argument list that was passed in.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoInt32                count           = literal_values->GetCount();

    FdoDataValue            *data_value     = NULL;

    FdoPtr<FdoLiteralValue> literal_value;

    // Check the number of arguments. SQRT accepts one parameter only. If the
    // number of parameters is not correct issue an exception.

    if (count != 1) 
        throw FdoException::Create(
                FdoException::NLSGetMessage(
                  FUNCTION_PARAMETER_NUMBER_ERROR, 
                  "Expression Engine: Invalid number of parameters for function '%1$ls'",
                  FDO_FUNCTION_SQRT));

    // Next, identify the data type associated with the value to be processed.
    // An exception is issued if the data type does not match any of the ones
    // the function supports

    literal_value = literal_values->GetItem(0);
    if (literal_value->GetLiteralValueType() != FdoLiteralValueType_Data)
        throw FdoException::Create(
                FdoException::NLSGetMessage(
                  FUNCTION_PARAMETER_ERROR, 
                  "Expression Engine: Invalid parameters for function '%1$ls'",
                  FDO_FUNCTION_SQRT));

    data_value = static_cast<FdoDataValue *>(literal_value.p);
    incoming_data_type = data_value->GetDataType();
    if ((incoming_data_type != FdoDataType_Decimal) &&
        (incoming_data_type != FdoDataType_Double ) &&
        (incoming_data_type != FdoDataType_Int16  ) &&
        (incoming_data_type != FdoDataType_Int32  ) &&
        (incoming_data_type != FdoDataType_Int64  ) &&
        (incoming_data_type != FdoDataType_Single )    )
        throw FdoException::Create(
                FdoException::NLSGetMessage(
                  FUNCTION_PARAMETER_DATA_TYPE_ERROR, 
                  "Expression Engine: Invalid parameter data type for function '%1$ls'",
                  FDO_FUNCTION_SQRT));

}  //  Validate ()

