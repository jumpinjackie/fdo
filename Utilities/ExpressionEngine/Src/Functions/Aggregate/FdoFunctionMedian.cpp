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
#include <Functions/Aggregate/FdoFunctionMedian.h>
#include <FdoCommonStringUtil.h>


// ----------------------------------------------------------------------------
// --                         Constructors/Destructors                       --
// ----------------------------------------------------------------------------

FdoFunctionMedian::FdoFunctionMedian ()

// +---------------------------------------------------------------------------
// | The class constructor.
// +---------------------------------------------------------------------------

{

    // Initialize all class variables.
    // NOTE: Due to the fact that data type enumeration misses an entry to
    //       indicate a not-set value, the variable "incoming_data_type" is
    //       set to "FdoDataType_CLOB" to indicate an invalid data type be-
    //       cause this function does not support this type. 

    is_validated        = false;

    function_definition = NULL;

    incoming_data_type  = FdoDataType_CLOB;

    // Setup the result cache.

    value_cache = CacheValueCollection::Create();

}  //  FdoFunctionMedian ()


FdoFunctionMedian::~FdoFunctionMedian ()

// +---------------------------------------------------------------------------
// | The class destructor.
// +---------------------------------------------------------------------------

{

    // Delete the result cache and function definition.

    FDO_SAFE_RELEASE(value_cache);
    FDO_SAFE_RELEASE(function_definition);

}  //  ~FdoFunctionMedian ()


// ----------------------------------------------------------------------------
// --                            Public Class APIs                           --
// ----------------------------------------------------------------------------

FdoFunctionMedian *FdoFunctionMedian::Create ()

// +---------------------------------------------------------------------------
// | The function creates a new instance of the class.
// +---------------------------------------------------------------------------

{

    return new FdoFunctionMedian();

}  //  Create ()

FdoFunctionMedian *FdoFunctionMedian::CreateObject ()

// +---------------------------------------------------------------------------
// | The function creates a new instance of the class.
// +---------------------------------------------------------------------------

{

    return new FdoFunctionMedian();

}  //  CreateObject ()

FdoFunctionDefinition *FdoFunctionMedian::GetFunctionDefinition ()

// +---------------------------------------------------------------------------
// | The function creates the supported signature list for the function MEDIAN.
// +---------------------------------------------------------------------------

{

    if (function_definition == NULL)
        CreateFunctionDefinition();

    return FDO_SAFE_ADDREF(function_definition);

}  //  GetFunctionDefinition ()

void FdoFunctionMedian::Process (FdoLiteralValueCollection *literal_values)

// +---------------------------------------------------------------------------
// | The function processes a call to the function MEDIAN.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoPtr<FdoDecimalValue> decimal_value;
    FdoPtr<FdoDoubleValue>  double_value;
    FdoPtr<FdoInt16Value>   int16_value;
    FdoPtr<FdoInt32Value>   int32_value;
    FdoPtr<FdoInt64Value>   int64_value;
    FdoPtr<FdoSingleValue>  single_value;

    // If the argument list has not been validated, execute the check next.
    // NOTE: the validation is executed only the first time the procedure is
    //       invoked and assumes that it remains the same until the function
    //       result is retrieved.

    if (!is_validated) {

        Validate(literal_values);
        is_validated = true;

    }  //  if (!is_validated) ...

    // Process the request.

    switch (incoming_data_type) {

      case FdoDataType_Decimal:
        decimal_value = (FdoDecimalValue *) literal_values->GetItem(0);
        if (!decimal_value->IsNull())
            ProcessRequest(decimal_value->GetDecimal());
        break;

      case FdoDataType_Double:
        double_value = (FdoDoubleValue *) literal_values->GetItem(0);
        if (!double_value->IsNull())
            ProcessRequest(double_value->GetDouble());
        break;

      case FdoDataType_Int16:
        int16_value = (FdoInt16Value *) literal_values->GetItem(0);
        if (!int16_value->IsNull())
            ProcessRequest(int16_value->GetInt16());
        break;

      case FdoDataType_Int32:
        int32_value = (FdoInt32Value *) literal_values->GetItem(0);
        if (!int32_value->IsNull())
            ProcessRequest(int32_value->GetInt32());
        break;

      case FdoDataType_Int64:
        int64_value = (FdoInt64Value *) literal_values->GetItem(0);
        if (!int64_value->IsNull())
            ProcessRequest(int64_value->GetInt64());
        break;

      case FdoDataType_Single:
        single_value = (FdoSingleValue *) literal_values->GetItem(0);
        if (!single_value->IsNull())
            ProcessRequest(single_value->GetSingle());
        break;

    }  //  switch ...

}  //  Process ()

FdoLiteralValue *FdoFunctionMedian::GetResult ()

// +---------------------------------------------------------------------------
// | The function returns the result of a call to the function MEDIAN.
// +---------------------------------------------------------------------------

{

    // Invalidate the flag indicating that the validation has been done.

    is_validated = false;

    // Return the result.

    return FdoDoubleValue::Create();

}  //  GetResult ()


// ----------------------------------------------------------------------------
// --                          Supporting functions                          --
// ----------------------------------------------------------------------------

void FdoFunctionMedian::CreateFunctionDefinition ()

// +---------------------------------------------------------------------------
// | The procedure creates the function definition for the function MEDIAN. The
// | function definition includes the list of supported signatures. The follow-
// | ing signatures are supported:
// |
// |    MEDIAN ({decimal, double, int16, int32, int64, single})
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
    FdoPtr<FdoArgumentDefinition> opt_arg;
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
        FUNCTION_MEDIAN,
        "Represents an inverse distribution function with a continuous distribution model");
    function_definition =
                FdoFunctionDefinition::Create(
                                        FDO_FUNCTION_MEDIAN,
                                        desc,
                                        true,
                                        signatures,
                                        FdoFunctionCategoryType_Aggregate);

}  //  CreateFunctionDefinition ()

void FdoFunctionMedian::ProcessRequest (FdoDouble value)

// +---------------------------------------------------------------------------
// | The function processes a request to the Expression Engine function MEDIAN
// | when applied to values of type DOUBLE.
// +---------------------------------------------------------------------------

{

    // NOT YET IMPLEMENTED

}  //  ProcessRequest ()

void FdoFunctionMedian::ProcessRequest (FdoFloat value)

// +---------------------------------------------------------------------------
// | The function processes a request to the Expression Engine function MEDIAN
// | when applied to values of type FLOAT.
// +---------------------------------------------------------------------------

{

    // NOT YET IMPLEMENTED

}  //  ProcessRequest ()

void FdoFunctionMedian::ProcessRequest (FdoInt16 value)

// +---------------------------------------------------------------------------
// | The function processes a request to the Expression Engine function MEDIAN
// | when applied to values of type INT16.
// +---------------------------------------------------------------------------

{

    // NOT YET IMPLEMENTED

}  //  ProcessRequest ()

void FdoFunctionMedian::ProcessRequest (FdoInt32 value)

// +---------------------------------------------------------------------------
// | The function processes a request to the Expression Engine function MEDIAN
// | when applied to values of type INT32.
// +---------------------------------------------------------------------------

{

    // NOT YET IMPLEMENTED

}  //  ProcessRequest ()

void FdoFunctionMedian::ProcessRequest (FdoInt64 value)

// +---------------------------------------------------------------------------
// | The function processes a request to the Expression Engine function MEDIAN
// | when applied to values of type INT64.
// +---------------------------------------------------------------------------

{

    // NOT YET IMPLEMENTED

}  //  ProcessRequest ()

void FdoFunctionMedian::Validate (FdoLiteralValueCollection *literal_values)

// +---------------------------------------------------------------------------
// | The function validates the argument list that was passed in.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoInt32                count           = literal_values->GetCount();

    FdoDataValue            *data_value     = NULL;

    FdoPtr<FdoLiteralValue> literal_value;

    // Check the number of arguments. MEDIAN accepts a single numeric parameter.
    // If the number of parameters is not correct issue an exception.

    if (count != 1) 
        throw FdoException::Create(
                FdoException::NLSGetMessage(
                  FUNCTION_PARAMETER_NUMBER_ERROR, 
                  "Expression Engine: Invalid number of parameters for function '%1$ls'",
                  FDO_FUNCTION_MEDIAN));

    // Ensure the argument data types match the ones defined in the signatures.
    // If this is not the case issue an exception.

    literal_value = literal_values->GetItem(0);
    if (literal_value->GetLiteralValueType() != FdoLiteralValueType_Data)
        throw FdoException::Create(
                FdoException::NLSGetMessage(
                  FUNCTION_PARAMETER_ERROR, 
                  "Expression Engine: Invalid parameters for function '%1$ls'",
                  FDO_FUNCTION_MEDIAN));

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
                  FDO_FUNCTION_MEDIAN));

}  //  Validate ()

