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

    first_value         = 0;
    value_count         = 0;

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

    bool                    is_NULL_value     = false;

    FdoInt32                insert_index      = 0;

    FdoDouble               curr_value        = 0;

    FdoPtr<FdoByteValue>    byte_value;
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

      case FdoDataType_Byte:
        byte_value = (FdoByteValue *) literal_values->GetItem(0);
        is_NULL_value = byte_value->IsNull();
        if (!is_NULL_value)
            curr_value = (FdoDouble) byte_value->GetByte();
        break;

      case FdoDataType_Decimal:
        decimal_value = (FdoDecimalValue *) literal_values->GetItem(0);
        is_NULL_value = decimal_value->IsNull();
        if (!is_NULL_value)
            curr_value = decimal_value->GetDecimal();
        break;

      case FdoDataType_Double:
        double_value = (FdoDoubleValue *) literal_values->GetItem(0);
        is_NULL_value = double_value->IsNull();
        if (!is_NULL_value)
            curr_value = double_value->GetDouble();
        break;

      case FdoDataType_Int16:
        int16_value = (FdoInt16Value *) literal_values->GetItem(0);
        is_NULL_value = int16_value->IsNull();
        if (!is_NULL_value)
            curr_value = (FdoDouble) int16_value->GetInt16();
        break;

      case FdoDataType_Int32:
        int32_value = (FdoInt32Value *) literal_values->GetItem(0);
        is_NULL_value = int32_value->IsNull();
        if (!is_NULL_value)
            curr_value = (FdoDouble) int32_value->GetInt32();
        break;

      case FdoDataType_Int64:
        int64_value = (FdoInt64Value *) literal_values->GetItem(0);
        is_NULL_value = int64_value->IsNull();
        if (!is_NULL_value)
            curr_value = (FdoDouble) int64_value->GetInt64();
        break;

      case FdoDataType_Single:
        single_value = (FdoSingleValue *) literal_values->GetItem(0);
        is_NULL_value = single_value->IsNull();
        if (!is_NULL_value)
            curr_value = (FdoDouble) single_value->GetSingle();
        break;

      default:
        throw FdoException::Create(
               FdoException::NLSGetMessage(
                  FUNCTION_PARAMETER_DATA_TYPE_ERROR, 
                  "Expression Engine: Invalid parameter data type for function '%1$ls'",
                  FDO_FUNCTION_MEDIAN));
        break;

    }  //  switch ...

    // If a valid value is provided, add it to the cache and increase the value
    // count. Note that the values in the cache must be sorted ascending.

    if (!is_NULL_value) {

        insert_index = GetInsertIndex(curr_value);
        new_cache_value = CacheValue::Create(curr_value);
        value_cache->Insert(insert_index, new_cache_value);
        value_count++;
        first_value = curr_value;

    }  //  if (!is_NULL_value) ...

}  //  Process ()

FdoLiteralValue *FdoFunctionMedian::GetResult ()

// +---------------------------------------------------------------------------
// | The function returns the result of a call to the function MEDIAN.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoDouble roi      = 0,
              c_roi    = 0,
              f_roi    = 0,
              result   = 0,
              tmp_val1 = 0,
              tmp_val2 = 0;
    
    // Invalidate the flag indicating that the validation has been done.

    is_validated = false;

    // If no values have been processed terminate the function.

    if (value_count == 0)
        return FdoDoubleValue::Create();

    // If there was just one value processed return that value as the function
    // result.

    if (value_count == 1)
        return FdoDoubleValue::Create(first_value);

    // Multiple rows have been processed. Next, determine the rows of interest.
    // The main Row Of Interest is determined with the following formula:
    //
    //      roi = (1 + ((<number_of_processed_values> - 1)/2)
    //
    // If this is not an even number, the rows of interest are the ones that
    // represents c_roi = Ceil(roi) and f_roi = Floor(roi). In the first case
    // (the row of interest is even) return the value stored at that position
    // in the cache. Otherwise add up the values stored at the positions iden-
    // tified by the c_roi and f_roi, divide it by 2 and return that result
    // back to the calling routine.

    roi = (1 + ((value_count - 1)/2));
    c_roi = ceil(roi);
    f_roi = floor(roi);

    if ((roi == c_roi) && (c_roi == f_roi))
        result = GetValueAtIndex((FdoInt32) (roi - 1));
    else {

      tmp_val1 = GetValueAtIndex((FdoInt32) (c_roi - 1));
      tmp_val2 = GetValueAtIndex((FdoInt32) (f_roi - 1));
      result   = (tmp_val1 + tmp_val2) / 2;

    }  //  else ...

    return FdoDoubleValue::Create(result);

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
// |    MEDIAN ({byte decimal, double, int16, int32, int64, single})
// |
// | The function always returns a DOUBLE.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoString                               *desc               = NULL;

    FdoStringP                              arg1_description;
    FdoStringP                              num_arg_literal;

    FdoPtr<FdoArgumentDefinition>           byte_arg;
    FdoPtr<FdoArgumentDefinition>           dcl_arg;
    FdoPtr<FdoArgumentDefinition>           dbl_arg;
    FdoPtr<FdoArgumentDefinition>           int16_arg;
    FdoPtr<FdoArgumentDefinition>           int32_arg;
    FdoPtr<FdoArgumentDefinition>           int64_arg;
    FdoPtr<FdoArgumentDefinition>           opt_arg;
    FdoPtr<FdoArgumentDefinition>           sgl_arg;

    FdoPtr<FdoArgumentDefinitionCollection> byte_args;
    FdoPtr<FdoArgumentDefinitionCollection> dcl_args;
    FdoPtr<FdoArgumentDefinitionCollection> dbl_args;
    FdoPtr<FdoArgumentDefinitionCollection> int16_args;
    FdoPtr<FdoArgumentDefinitionCollection> int32_args;
    FdoPtr<FdoArgumentDefinitionCollection> int64_args;
    FdoPtr<FdoArgumentDefinitionCollection> sgl_args;

    FdoPtr<FdoSignatureDefinition>          signature;
    FdoPtr<FdoSignatureDefinitionCollection> signatures;

    // Get the general descriptions for the arguments.

    arg1_description = FdoException::NLSGetMessage(
                                        FUNCTION_GENERAL_ARG,
                                        "Argument to be processed");

    // The following defines the different argument definition collections.

    num_arg_literal =
            FdoException::NLSGetMessage(FUNCTION_NUMBER_ARG_LIT, "number");

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

    // Create the signature collection.

    signatures = FdoSignatureDefinitionCollection::Create();

    signature = FdoSignatureDefinition::Create(FdoDataType_Double, byte_args);
    signatures->Add(signature);

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

FdoInt32 FdoFunctionMedian::GetInsertIndex (FdoDouble value)

// +---------------------------------------------------------------------------
// | The function determines the position in the cache where a new entry must
// | be entered. This is to ensure that the values in the cache remain sorted
// | in an ascending order.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoInt32 loop_count,
             cache_count                 = 0;

    FdoPtr<CacheValue> curr_cache_value;

    // Loop through the cache and check if the given value is greater than
    // the one at the current position in the cache. If this is the case then
    // the new value has to be inserted at this position. If the provided
    // value is bigger than all of the values currently stored, the value
    // must be added at the end.

    cache_count = value_cache->GetCount();
    if (cache_count == 0)
        return 0;

    for (loop_count = 0; loop_count < cache_count; loop_count++) {

        curr_cache_value = value_cache->GetItem(loop_count);
        if (curr_cache_value->GetDoubleValue() > value)
            return loop_count;

    }  //  for ...

    return loop_count;

}  //  GetInsertIndex ()

FdoDouble FdoFunctionMedian::GetValueAtIndex (FdoInt32 index)

// +---------------------------------------------------------------------------
// | The function returns the value stored in the cache at the specified
// | location
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoInt32 cache_count                 = 0;

    FdoPtr<CacheValue> curr_cache_value;

    // Ensure the indexes are within the range for the cache. If this is not
    // the case issue an exception.

    cache_count = value_cache->GetCount();
    if ((index < 0) || (index > cache_count))
        throw FdoException::Create(
                FdoException::NLSGetMessage(
                    FUNCTION_UNEXPECTED_RESULT_ERROR, 
                    "Expression Engine: Unexpected result for function '%1$ls'",
                    FDO_FUNCTION_MEDIAN));

    // Get the value at the specified location and return it back to the call-
    // ing routine.

    curr_cache_value = value_cache->GetItem(index);
    return (curr_cache_value->GetDoubleValue());

}  //  GetValueAtIndex ()

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
    if ((incoming_data_type != FdoDataType_Byte   ) &&
        (incoming_data_type != FdoDataType_Decimal) &&
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

