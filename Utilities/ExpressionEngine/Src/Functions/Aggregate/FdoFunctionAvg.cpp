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
#include <Functions/Aggregate/FdoFunctionAvg.h>


// ----------------------------------------------------------------------------
// --                         Constructors/Destructors                       --
// ----------------------------------------------------------------------------

FdoFunctionAvg::FdoFunctionAvg ()

// +---------------------------------------------------------------------------
// | The class constructor.
// +---------------------------------------------------------------------------

{

    // Initialize all class variables.
    // NOTE: Due to the fact that data type enumeration misses an entry to
    //       indicate a not-set value, the variable "incoming_data_type" is
    //       set to "FdoDataType_CLOB" to indicate an invalid data type be-
    //       cause this function does not support this type. 

    is_distinct_request = false;
    is_validated        = false;

    function_sum        = 0;
    process_value       = 0;
    function_count      = 0;

    function_definition = NULL;

    incoming_data_type  = FdoDataType_CLOB;

    // Setup the result cache.

    value_cache = CacheValueCollection::Create();

}  //  FdoFunctionAvg ()


FdoFunctionAvg::~FdoFunctionAvg ()

// +---------------------------------------------------------------------------
// | The class destructor.
// +---------------------------------------------------------------------------

{

    // Delete the result cache and function definition.

    FDO_SAFE_RELEASE(value_cache);
    FDO_SAFE_RELEASE(function_definition);

}  //  ~FdoFunctionAvg ()


// ----------------------------------------------------------------------------
// --                            Public Class APIs                           --
// ----------------------------------------------------------------------------

FdoFunctionAvg *FdoFunctionAvg::Create ()

// +---------------------------------------------------------------------------
// | The function creates a new instance of the class.
// +---------------------------------------------------------------------------

{

    return new FdoFunctionAvg();

}  //  Create ()

FdoFunctionAvg *FdoFunctionAvg::CreateObject ()

// +---------------------------------------------------------------------------
// | The function creates a new instance of the class.
// +---------------------------------------------------------------------------

{

    return new FdoFunctionAvg();

}  //  CreateObject ()

FdoFunctionDefinition *FdoFunctionAvg::GetFunctionDefinition ()

// +---------------------------------------------------------------------------
// | The function creates the supported signature list for the function AVG.
// +---------------------------------------------------------------------------

{

    if (function_definition == NULL)
        CreateFunctionDefinition();

    return FDO_SAFE_ADDREF(function_definition);

}  //  GetFunctionDefinition ()

void FdoFunctionAvg::Process (FdoLiteralValueCollection *literal_values)

// +---------------------------------------------------------------------------
// | The function processes a call to the function AVG.
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
        decimal_value =
                    (FdoDecimalValue *) literal_values->GetItem(process_value);
        if (!decimal_value->IsNull())
            ProcessRequest(decimal_value->GetDecimal());
        break;

      case FdoDataType_Double:
        double_value =
                    (FdoDoubleValue *) literal_values->GetItem(process_value);
        if (!double_value->IsNull())
            ProcessRequest(double_value->GetDouble());
        break;

      case FdoDataType_Int16:
        int16_value = (FdoInt16Value *) literal_values->GetItem(process_value);
        if (!int16_value->IsNull())
            ProcessRequest(int16_value->GetInt16());
        break;

      case FdoDataType_Int32:
        int32_value = (FdoInt32Value *) literal_values->GetItem(process_value);
        if (!int32_value->IsNull())
            ProcessRequest(int32_value->GetInt32());
        break;

      case FdoDataType_Int64:
        int64_value = (FdoInt64Value *) literal_values->GetItem(process_value);
        if (!int64_value->IsNull())
            ProcessRequest(int64_value->GetInt64());
        break;

      case FdoDataType_Single:
        single_value =
                    (FdoSingleValue *) literal_values->GetItem(process_value);
        if (!single_value->IsNull())
            ProcessRequest(single_value->GetSingle());
        break;

    }  //  switch ...

}  //  Process ()

FdoLiteralValue *FdoFunctionAvg::GetResult ()

// +---------------------------------------------------------------------------
// | The function returns the result of a call to the function AVG.
// +---------------------------------------------------------------------------

{

    // Invalidate the flag indicating that the validation has been done.

    is_validated = false;

    // Return the result.

    return (function_count == 0)
            ? FdoDoubleValue::Create()
            : FdoDoubleValue::Create(function_sum/function_count);

}  //  GetResult ()


// ----------------------------------------------------------------------------
// --                          Supporting functions                          --
// ----------------------------------------------------------------------------

void FdoFunctionAvg::CreateFunctionDefinition ()

// +---------------------------------------------------------------------------
// | The procedure creates the function definition for the function AVG. The
// | function definition includes the list of supported signatures. The follow-
// | ing signatures are supported:
// |
// |    AVG ([string, ]
// |         {decimal, double, int16, int32, int64, single})
// |
// | If the optional first parameter is used, then the parameter value must be
// | ALL or DISTINCT. The function allways returns a DOUBLE.
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

    FdoPtr<FdoSignatureDefinition>           signature;
    FdoSignatureDefinitionCollection         *signatures;

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

    desc = FdoException::NLSGetMessage(
                            FUNCTION_AVG,
                            "Determines the average value of an expression");
    function_definition =
                FdoFunctionDefinition::Create(
                                        FDO_FUNCTION_AVG,
                                        desc,
                                        true,
                                        signatures,
                                        FdoFunctionCategoryType_Aggregate);

}  //  CreateFunctionDefinition ()

void FdoFunctionAvg::ProcessRequest (FdoDouble value)

// +---------------------------------------------------------------------------
// | The function processes a request to the Expression Engine function AVG
// | when applied to values of type DOUBLE.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    bool     data_processed              = false;

    FdoInt32 loop_count,
             cache_count                 = 0;

    FdoPtr<CacheValue> new_cache_value,
                       curr_cache_value;
    
    // If this is a request on a distinct set of data, only process the value
    // if it is not stored in the cache yet. 

    if (is_distinct_request) {

        cache_count = value_cache->GetCount();
        for (loop_count = 0; loop_count < cache_count; loop_count++) {

          curr_cache_value = value_cache->GetItem(loop_count);
          if (curr_cache_value->GetDoubleValue() == value) {

              data_processed = true;
              break;

          }  //  if (curr_cache_value->GetDoubleValue() == value) ...

        }  //  for ...

        // Data hasn't been processed yet. Add it to the cache.

        if (!data_processed) {

            new_cache_value = CacheValue::Create(value);
            value_cache->Add(new_cache_value);

        }  //  if (!data_processed) ...

    }  //  if (is_distinct_request) ...

    // If the data hasn't been processed yet, handle it.

    if (!data_processed) {

        function_sum = function_sum + value;
        function_count++;

    }  //  if (!data_processed) ...

}  //  ProcessRequest ()

void FdoFunctionAvg::ProcessRequest (FdoFloat value)

// +---------------------------------------------------------------------------
// | The function processes a request to the Expression Engine function AVG
// | when applied to values of type FLOAT.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    bool     data_processed              = false;

    FdoInt32 loop_count,
             cache_count                 = 0;

    FdoPtr<CacheValue> new_cache_value,
                       curr_cache_value;
    
    // If this is a request on a distinct set of data, only process the value
    // if it is not stored in the cache yet. 

    if (is_distinct_request) {

        cache_count = value_cache->GetCount();
        for (loop_count = 0; loop_count < cache_count; loop_count++) {

          curr_cache_value = value_cache->GetItem(loop_count);
          if (curr_cache_value->GetFloatValue() == value) {

              data_processed = true;
              break;

          }  //  if (curr_cache_value->GetFloatValue() == value) ...

        }  //  for ...

        // Data hasn't been processed yet. Add it to the cache.

        if (!data_processed) {

            new_cache_value = CacheValue::Create(value);
            value_cache->Add(new_cache_value);

        }  //  if (!data_processed) ...

    }  //  if (is_distinct_request) ...

    // If the data hasn't been processed yet, handle it.

    if (!data_processed) {

        function_sum = function_sum + value;
        function_count++;

    }  //  if (!data_processed) ...

}  //  ProcessRequest ()

void FdoFunctionAvg::ProcessRequest (FdoInt16 value)

// +---------------------------------------------------------------------------
// | The function processes a request to the Expression Engine function AVG
// | when applied to values of type INT16.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    bool     data_processed              = false;

    FdoInt32 loop_count,
             cache_count                 = 0;

    FdoPtr<CacheValue> new_cache_value,
                       curr_cache_value;
    
    // If this is a request on a distinct set of data, only process the value
    // if it is not stored in the cache yet. 

    if (is_distinct_request) {

        cache_count = value_cache->GetCount();
        for (loop_count = 0; loop_count < cache_count; loop_count++) {

          curr_cache_value = value_cache->GetItem(loop_count);
          if (curr_cache_value->GetInt16Value() == value) {

              data_processed = true;
              break;

          }  //  if (curr_cache_value->GetInt16Value() == value) ...

        }  //  for ...

        // Data hasn't been processed yet. Add it to the cache.

        if (!data_processed) {

            new_cache_value = CacheValue::Create(value);
            value_cache->Add(new_cache_value);

        }  //  if (!data_processed) ...

    }  //  if (is_distinct_request) ...

    // If the data hasn't been processed yet, handle it.

    if (!data_processed) {

        function_sum = function_sum + value;
        function_count++;

    }  //  if (!data_processed) ...

}  //  ProcessRequest ()

void FdoFunctionAvg::ProcessRequest (FdoInt32 value)

// +---------------------------------------------------------------------------
// | The function processes a request to the Expression Engine function AVG
// | when applied to values of type INT32.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    bool     data_processed              = false;

    FdoInt32 loop_count,
             cache_count                 = 0;

    FdoPtr<CacheValue> new_cache_value,
                       curr_cache_value;
    
    // If this is a request on a distinct set of data, only process the value
    // if it is not stored in the cache yet. 

    if (is_distinct_request) {

        cache_count = value_cache->GetCount();
        for (loop_count = 0; loop_count < cache_count; loop_count++) {

          curr_cache_value = value_cache->GetItem(loop_count);
          if (curr_cache_value->GetInt32Value() == value) {

              data_processed = true;
              break;

          }  //  if (curr_cache_value->GetInt32Value() == value) ...

        }  //  for ...

        // Data hasn't been processed yet. Add it to the cache.

        if (!data_processed) {

            new_cache_value = CacheValue::Create(value);
            value_cache->Add(new_cache_value);

        }  //  if (!data_processed) ...

    }  //  if (is_distinct_request) ...

    // If the data hasn't been processed yet, handle it.

    if (!data_processed) {

        function_sum = function_sum + value;
        function_count++;

    }  //  if (!data_processed) ...

}  //  ProcessRequest ()

void FdoFunctionAvg::ProcessRequest (FdoInt64 value)

// +---------------------------------------------------------------------------
// | The function processes a request to the Expression Engine function AVG
// | when applied to values of type INT64.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    bool     data_processed              = false;

    FdoInt32 loop_count,
             cache_count                 = 0;

    FdoPtr<CacheValue> new_cache_value,
                       curr_cache_value;
    
    // If this is a request on a distinct set of data, only process the value
    // if it is not stored in the cache yet. 

    if (is_distinct_request) {

        cache_count = value_cache->GetCount();
        for (loop_count = 0; loop_count < cache_count; loop_count++) {

          curr_cache_value = value_cache->GetItem(loop_count);
          if (curr_cache_value->GetInt64Value() == value) {

              data_processed = true;
              break;

          }  //  if (curr_cache_value->GetInt64Value() == value) ...

        }  //  for ...

        // Data hasn't been processed yet. Add it to the cache.

        if (!data_processed) {

            new_cache_value = CacheValue::Create(value);
            value_cache->Add(new_cache_value);

        }  //  if (!data_processed) ...

    }  //  if (is_distinct_request) ...

    // If the data hasn't been processed yet, handle it.

    if (!data_processed) {

        function_sum = function_sum + value;
        function_count++;

    }  //  if (!data_processed) ...

}  //  ProcessRequest ()

void FdoFunctionAvg::Validate (FdoLiteralValueCollection *literal_values)

// +---------------------------------------------------------------------------
// | The function validates the argument list that was passed in.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoInt32                count           = literal_values->GetCount();

    FdoStringP              param_value;

    FdoDataValue            *data_value     = NULL;

    FdoPtr<FdoStringValue>  str_value;

    FdoPtr<FdoLiteralValue> literal_value;

    // Check the number of arguments. AVG can accept either one or two para-
    // meters. If the number of parameters is not correct issue an exception.

    if ((count < 1) || (count > 2)) 
        throw FdoCommandException::Create(
                FdoException::NLSGetMessage(
                  FUNCTION_PARAMETER_NUMBER_ERROR, 
                  "Expression Engine: Invalid number of parameters for function '%1$ls'",
                  FDO_FUNCTION_AVG));

    // If there are two function parameters the value of the first one must be
    // either "ALL" or "DISTINCT". If this is not the case issue an exception.

    if (count == 2) {
 
        literal_value = literal_values->GetItem(0);
        if (literal_value->GetLiteralValueType() != FdoLiteralValueType_Data)
            throw FdoCommandException::Create(
                      FdoException::NLSGetMessage(
                          FUNCTION_PARAMETER_ERROR, 
                          "Expression Engine: Invalid parameters for function '%1$ls'",
                          FDO_FUNCTION_AVG));

        data_value = static_cast<FdoDataValue *>(literal_value.p);
        if (data_value->GetDataType() != FdoDataType_String)
            throw FdoCommandException::Create(
                      FdoException::NLSGetMessage(
                          FUNCTION_PARAMETER_DATA_TYPE_ERROR, 
                          "Expression Engine: Invalid parameter data type for function '%1$ls'",
                          FDO_FUNCTION_AVG));

        str_value   = static_cast<FdoStringValue *>(literal_value.p);
        param_value = str_value->GetString();
        if ((_wcsicmp(param_value, L"ALL"     ) != 0) &&
            (_wcsicmp(param_value, L"DISTINCT") != 0)    )
            throw FdoCommandException::Create(
              FdoException::NLSGetMessage(
                  FUNCTION_OPERATOR_ERROR, 
                  "Expression Engine: Invalid first parameter value for function '%1$ls'",
                  FDO_FUNCTION_AVG));

    }  //  if (count == 2) ...

    // If two arguments are provided, it is necessary to remember whether or
    // not the request is issued at a distinct value set as this affects the
    // result.

    if (count == 2)
        is_distinct_request = (_wcsicmp(param_value, L"DISTINCT") == 0);

    // If two parameters are provided, the value to be processed is provided
    // with the second parameter, otherwise (only one parameter is given) with
    // the first one.

    process_value = (count - 1);

    // Next, identify the data type associated with the value to be processed.
    // An exception is issued if the data type does not match any of the ones
    // the function supports

    literal_value = literal_values->GetItem(process_value);
    if (literal_value->GetLiteralValueType() != FdoLiteralValueType_Data)
        throw FdoCommandException::Create(
                FdoException::NLSGetMessage(
                  FUNCTION_PARAMETER_ERROR, 
                  "Expression Engine: Invalid parameters for function '%1$ls'",
                  FDO_FUNCTION_AVG));

    data_value = static_cast<FdoDataValue *>(literal_value.p);
    incoming_data_type = data_value->GetDataType();
    if ((incoming_data_type == FdoDataType_Boolean ) ||
        (incoming_data_type == FdoDataType_Byte    ) ||
        (incoming_data_type == FdoDataType_DateTime) ||
        (incoming_data_type == FdoDataType_String  ) ||
        (incoming_data_type == FdoDataType_BLOB    ) ||
        (incoming_data_type == FdoDataType_CLOB    )    )
       throw FdoCommandException::Create(
               FdoException::NLSGetMessage(
                  FUNCTION_PARAMETER_DATA_TYPE_ERROR, 
                  "Expression Engine: Invalid parameter data type for function '%1$ls'",
                  FDO_FUNCTION_AVG));

}  //  Validate ()

