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
#include <Functions/Aggregate/FdoFunctionStddev.h>
#include <FdoCommonStringUtil.h>


// ----------------------------------------------------------------------------
// --                         Constructors/Destructors                       --
// ----------------------------------------------------------------------------

FdoFunctionStddev::FdoFunctionStddev ()

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
    is_distinct_request = false;

    function_definition = NULL;

    value_sum           = 0;
    value_count         = 0;
    process_value       = 0;

    incoming_data_type  = FdoDataType_CLOB;

    // Setup the result cache.

    value_cache = CacheValueCollection::Create();

}  //  FdoFunctionStddev ()


FdoFunctionStddev::~FdoFunctionStddev ()

// +---------------------------------------------------------------------------
// | The class destructor.
// +---------------------------------------------------------------------------

{

    // Delete the result cache and function definition.

    FDO_SAFE_RELEASE(value_cache);
    FDO_SAFE_RELEASE(function_definition);

}  //  ~FdoFunctionStddev ()


// ----------------------------------------------------------------------------
// --                            Public Class APIs                           --
// ----------------------------------------------------------------------------

FdoFunctionStddev *FdoFunctionStddev::Create ()

// +---------------------------------------------------------------------------
// | The function creates a new instance of the class.
// +---------------------------------------------------------------------------

{

    return new FdoFunctionStddev();

}  //  Create ()

FdoFunctionStddev *FdoFunctionStddev::CreateObject ()

// +---------------------------------------------------------------------------
// | The function creates a new instance of the class.
// +---------------------------------------------------------------------------

{

    return new FdoFunctionStddev();

}  //  CreateObject ()

FdoFunctionDefinition *FdoFunctionStddev::GetFunctionDefinition ()

// +---------------------------------------------------------------------------
// | The function creates the supported signature list for the function STDDEV.
// +---------------------------------------------------------------------------

{

    if (function_definition == NULL)
        CreateFunctionDefinition();

    return FDO_SAFE_ADDREF(function_definition);

}  //  GetFunctionDefinition ()

void FdoFunctionStddev::Process (FdoLiteralValueCollection *literal_values)

// +---------------------------------------------------------------------------
// | The function processes a call to the function STDDEV.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    bool                    add_to_cache      = true,
                            is_NULL_value     = false;

    FdoDouble               curr_value        = 0;

    FdoPtr<FdoDecimalValue> decimal_value;
    FdoPtr<FdoDoubleValue>  double_value;
    FdoPtr<FdoInt16Value>   int16_value;
    FdoPtr<FdoInt32Value>   int32_value;
    FdoPtr<FdoInt64Value>   int64_value;
    FdoPtr<FdoSingleValue>  single_value;
    FdoPtr<CacheValue>      new_cache_value;

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
        decimal_value = (FdoDecimalValue *) literal_values->GetItem(process_value);
        is_NULL_value = decimal_value->IsNull();
        if (!is_NULL_value)
            curr_value = decimal_value->GetDecimal();
        break;

      case FdoDataType_Double:
        double_value  = (FdoDoubleValue *) literal_values->GetItem(process_value);
        is_NULL_value = double_value->IsNull();
        if (!is_NULL_value)
            curr_value = double_value->GetDouble();
        break;

      case FdoDataType_Int16:
        int16_value   = (FdoInt16Value *) literal_values->GetItem(process_value);
        is_NULL_value = int16_value->IsNull();
        if (!is_NULL_value)
            curr_value = (FdoDouble) int16_value->GetInt16();
        break;

      case FdoDataType_Int32:
        int32_value = (FdoInt32Value *) literal_values->GetItem(process_value);
        is_NULL_value = int32_value->IsNull();
        if (!is_NULL_value)
            curr_value = (FdoDouble) int32_value->GetInt32();
        break;

      case FdoDataType_Int64:
        int64_value   = (FdoInt64Value *) literal_values->GetItem(process_value);
        is_NULL_value = int64_value->IsNull();
        if (!is_NULL_value)
            curr_value = (FdoDouble) int64_value->GetInt64();
        break;

      case FdoDataType_Single:
        single_value = (FdoSingleValue *) literal_values->GetItem(process_value);
        is_NULL_value = single_value->IsNull();
        if (!is_NULL_value)
            curr_value = (FdoDouble) single_value->GetSingle();
        break;

      default:
        throw FdoException::Create(
               FdoException::NLSGetMessage(
                  FUNCTION_PARAMETER_DATA_TYPE_ERROR, 
                  "Expression Engine: Invalid parameter data type for function '%1$ls'",
                  FDO_FUNCTION_STDDEV));
        break;

    }  //  switch ...

    // If a valid value is provided, add it to the cache unless this is a re-
    // quest on distinct values and the value is already stored in the cache.
    // If the value is added to the cache, then also add it to the current sum
    // and increase the value count.

    if (!is_NULL_value) {

        if (is_distinct_request)
            add_to_cache = !ValueIsInCache(curr_value);

        if (add_to_cache) {

            new_cache_value = CacheValue::Create(curr_value);
            value_cache->Add(new_cache_value);
            value_sum = value_sum + curr_value;
            value_count++;

        }  //  if (add_to_cache) ...

    }  //  if (!is_NULL_value) ...

}  //  Process ()

FdoLiteralValue *FdoFunctionStddev::GetResult ()

// +---------------------------------------------------------------------------
// | The function returns the result of a call to the function STDDEV.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoDouble dist_sum      = 0,
              stddev_value  = 0,
              average_value = 0;

    // Invalidate the flag indicating that the validation has been done.

    is_validated = false;

    // If no value was processed terminate the function.

    if (value_count == 0)
        return FdoDoubleValue::Create();

    // If only one value was processed the standard deviation is 0.

    if (value_count == 1)
        return FdoDoubleValue::Create((FdoDouble) 0);

    // Calculate the average value for the processed values.

    average_value = (value_sum/value_count);

    // The following call calculates variance required to determine the
    // standard deviation.

    dist_sum = CalculateVariance(average_value);

    // Calculate the standard deviation and return the result back to the
    // calling routine.

    stddev_value = sqrt((dist_sum/(value_count-1)));
    return FdoDoubleValue::Create(stddev_value);

}  //  GetResult ()


// ----------------------------------------------------------------------------
// --                          Supporting functions                          --
// ----------------------------------------------------------------------------

FdoDouble FdoFunctionStddev::CalculateVariance (FdoDouble average)

// +---------------------------------------------------------------------------
// | The function calculates the required variance to determine the standard
// | deviation.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoInt32           loop_count,
                       cache_count       = 0;

    FdoDouble          curr_sum          = 0,
                       curr_value        = 0;

    FdoPtr<CacheValue> curr_cache_value;

    // Navigate through the cache. For each entry in the cache, calculate the
    // distance off the average and raise the result to the power of 2. All
    // results are added up.

    cache_count = value_cache->GetCount();
    for (loop_count = 0; loop_count < cache_count; loop_count++) {

      curr_cache_value = value_cache->GetItem(loop_count);
      curr_value       = curr_cache_value->GetDoubleValue();
      curr_sum         = curr_sum +
                         ((curr_value - average) * (curr_value - average));

    }  //  for ...

    return curr_sum;

}  //  CalculateVariance ()

void FdoFunctionStddev::CreateFunctionDefinition ()

// +---------------------------------------------------------------------------
// | The procedure creates the function definition for the function STDDEV. The
// | function definition includes the list of supported signatures. The follow-
// | ing signatures are supported:
// |
// |    STDDEV ([string, ] {decimal, double, int16, int32, int64, single})
// |
// | If the optional first parameter is used, then the parameter value must be
// | ALL or DISTINCT. The function always returns a DOUBLE.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoString *desc = NULL;

    FdoStringP arg1_description;
    FdoStringP arg2_description;
    FdoStringP num_arg_literal;
    FdoStringP opt_arg_literal;

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

    FdoPtr<FdoArgumentDefinitionCollection> dcl_opt_args;
    FdoPtr<FdoArgumentDefinitionCollection> dbl_opt_args;
    FdoPtr<FdoArgumentDefinitionCollection> int16_opt_args;
    FdoPtr<FdoArgumentDefinitionCollection> int32_opt_args;
    FdoPtr<FdoArgumentDefinitionCollection> int64_opt_args;
    FdoPtr<FdoArgumentDefinitionCollection> sgl_opt_args;

    FdoPtr<FdoDataValueCollection>           values;
    FdoPtr<FdoPropertyValueConstraintList>   argument_value_list;

    FdoPtr<FdoSignatureDefinition>          signature;
    FdoSignatureDefinitionCollection        *signatures;

    // The function contains signatures that allow an optional first parameter.
    // If used the value of this parameter must be either ALL or DISTINCT. The
    // following sets up an argument value list to be used for the argument
    // describing this parameter.

    argument_value_list = FdoPropertyValueConstraintList::Create();
    values              = argument_value_list->GetConstraintList();
    values->Add(FdoStringValue::Create(L"ALL"));
    values->Add(FdoStringValue::Create(L"DISTINCT"));

    // Get the general descriptions for the arguments.

    arg1_description = FdoException::NLSGetMessage(
                                        FUNCTION_GENERAL_ARG,
                                        "Argument to be processed");

    arg2_description = FdoException::NLSGetMessage(
                                    FUNCTION_OPERATOR_ARG,
                                    "Operation indicator (ALL or DISTINCT)");

    opt_arg_literal = FdoException::NLSGetMessage(FUNCTION_OPERATOR_ARG_LIT,
                                                  "Operation Indicator");

    // The supported signatures allow an optional first parameter. The follow-
    // ing defines the argument definition for this parameter.

    opt_arg =
           FdoArgumentDefinition::Create(
                        opt_arg_literal, arg2_description, FdoDataType_String);
    opt_arg->SetArgumentValueList(argument_value_list);

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

    dcl_opt_args = FdoArgumentDefinitionCollection::Create();
    dcl_opt_args->Add(opt_arg);
    dcl_opt_args->Add(dcl_arg);

    dbl_opt_args = FdoArgumentDefinitionCollection::Create();
    dbl_opt_args->Add(opt_arg);
    dbl_opt_args->Add(dbl_arg);

    int16_opt_args = FdoArgumentDefinitionCollection::Create();
    int16_opt_args->Add(opt_arg);
    int16_opt_args->Add(int16_arg);

    int32_opt_args = FdoArgumentDefinitionCollection::Create();
    int32_opt_args->Add(opt_arg);
    int32_opt_args->Add(int32_arg);

    int64_opt_args = FdoArgumentDefinitionCollection::Create();
    int64_opt_args->Add(opt_arg);
    int64_opt_args->Add(int64_arg);

    sgl_opt_args = FdoArgumentDefinitionCollection::Create();
    sgl_opt_args->Add(opt_arg);
    sgl_opt_args->Add(sgl_arg);

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

    signature = FdoSignatureDefinition::Create(
                                            FdoDataType_Double, dcl_opt_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                            FdoDataType_Double, dbl_opt_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                            FdoDataType_Double, int16_opt_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                            FdoDataType_Double, int32_opt_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                            FdoDataType_Double, int64_opt_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                            FdoDataType_Double, sgl_opt_args);
    signatures->Add(signature);

    // Create the function definition.

    desc =
      FdoException::NLSGetMessage(
        FUNCTION_STDDEV,
        "Represents an inverse distribution function with a continuous distribution model");
    function_definition =
                FdoFunctionDefinition::Create(
                                        FDO_FUNCTION_STDDEV,
                                        desc,
                                        true,
                                        signatures,
                                        FdoFunctionCategoryType_Aggregate);

}  //  CreateFunctionDefinition ()

void FdoFunctionStddev::Validate (FdoLiteralValueCollection *literal_values)

// +---------------------------------------------------------------------------
// | The function validates the argument list that was passed in.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoInt32                count           = literal_values->GetCount();

    FdoStringP              param_value;

    FdoDataValue            *data_value     = NULL;

    FdoStringValue          *str_value      = NULL;

    FdoPtr<FdoLiteralValue> literal_value;

    // Check the number of arguments. AVG can accept either one or two para-
    // meters. If the number of parameters is not correct issue an exception.

    if ((count < 1) || (count > 2)) 
        throw FdoException::Create(
                FdoException::NLSGetMessage(
                  FUNCTION_PARAMETER_NUMBER_ERROR, 
                  "Expression Engine: Invalid number of parameters for function '%1$ls'",
                  FDO_FUNCTION_STDDEV));

    // If there are two function parameters the value of the first one must be
    // either "ALL" or "DISTINCT". If this is not the case issue an exception.

    if (count == 2) {
 
        literal_value = literal_values->GetItem(0);
        if (literal_value->GetLiteralValueType() != FdoLiteralValueType_Data)
            throw FdoException::Create(
                      FdoException::NLSGetMessage(
                          FUNCTION_PARAMETER_ERROR, 
                          "Expression Engine: Invalid parameters for function '%1$ls'",
                          FDO_FUNCTION_STDDEV));

        data_value = static_cast<FdoDataValue *>(literal_value.p);
        if (data_value->GetDataType() != FdoDataType_String)
            throw FdoException::Create(
                      FdoException::NLSGetMessage(
                          FUNCTION_PARAMETER_DATA_TYPE_ERROR, 
                          "Expression Engine: Invalid parameter data type for function '%1$ls'",
                          FDO_FUNCTION_STDDEV));

        str_value   = static_cast<FdoStringValue *>(literal_value.p);
        if (str_value->IsNull())
            throw FdoException::Create(
              FdoException::NLSGetMessage(
                  FUNCTION_OPERATOR_ERROR, 
                  "Expression Engine: Invalid operator parameter value for function '%1$ls'",
                  FDO_FUNCTION_STDDEV));

        param_value = str_value->GetString();
        if ((FdoCommonStringUtil::StringCompareNoCase(
                                        param_value, L"ALL"     ) != 0) &&
            (FdoCommonStringUtil::StringCompareNoCase(
                                        param_value, L"DISTINCT") != 0)    )
            throw FdoException::Create(
              FdoException::NLSGetMessage(
                  FUNCTION_OPERATOR_ERROR, 
                  "Expression Engine: Invalid operator parameter value for function '%1$ls'",
                  FDO_FUNCTION_STDDEV));

    }  //  if (count == 2) ...

    // If two arguments are provided, it is necessary to remember whether or
    // not the request is issued at a distinct value set as this affects the
    // result.

    if (count == 2)
        is_distinct_request =
                (FdoCommonStringUtil::StringCompareNoCase(
                                            param_value, L"DISTINCT") == 0);

    // If two parameters are provided, the value to be processed is provided
    // with the second parameter, otherwise (only one parameter is given) with
    // the first one.

    process_value = (count - 1);

    // Next, identify the data type associated with the value to be processed.
    // An exception is issued if the data type does not match any of the ones
    // the function supports

    literal_value = literal_values->GetItem(process_value);
    if (literal_value->GetLiteralValueType() != FdoLiteralValueType_Data)
        throw FdoException::Create(
                FdoException::NLSGetMessage(
                  FUNCTION_PARAMETER_ERROR, 
                  "Expression Engine: Invalid parameters for function '%1$ls'",
                  FDO_FUNCTION_STDDEV));

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
                  FDO_FUNCTION_STDDEV));

}  //  Validate ()

bool FdoFunctionStddev::ValueIsInCache (FdoDouble value)

// +---------------------------------------------------------------------------
// | The function checks if the provided value is already stored in the cache
// | and returns TRUE if this is the case, FALSE otherwise.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoInt32 loop_count,
             cache_count                 = 0;

    FdoPtr<CacheValue> curr_cache_value;

    // Loop through the cache and check if the given value is already stored
    // there. If this is the case return TRUE, FALSE otherwise.

    cache_count = value_cache->GetCount();
    for (loop_count = 0; loop_count < cache_count; loop_count++) {

        curr_cache_value = value_cache->GetItem(loop_count);
        if (curr_cache_value->GetDoubleValue() == value)
            return true;

    }  //  for ...

    return false;

}  //  ValueIsInCache ()

