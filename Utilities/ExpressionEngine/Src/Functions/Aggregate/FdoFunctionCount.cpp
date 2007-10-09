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
#include <Functions/Aggregate/FdoFunctionCount.h>
#include <FdoCommonStringUtil.h>


// ----------------------------------------------------------------------------
// --                         Constructors/Destructors                       --
// ----------------------------------------------------------------------------

FdoFunctionCount::FdoFunctionCount ()

// +---------------------------------------------------------------------------
// | The class constructor.
// +---------------------------------------------------------------------------

{

    // Initialize all class variables.

    is_distinct_request = false;
    is_validated        = false;

    process_value       = 0;
    function_result     = 0;
    NULL_value_count    = 0;
    process_call_count  = 0;

    bool_true_found     = false;
    bool_false_found    = false;

    function_definition = NULL;

    // Setup the result cache.

    value_cache = CacheValueCollection::Create();

}  //  FdoFunctionCount ()


FdoFunctionCount::~FdoFunctionCount ()

// +---------------------------------------------------------------------------
// | The class destructor.
// +---------------------------------------------------------------------------

{

    // Delete the result cache and function definition.

    FDO_SAFE_RELEASE(value_cache);
    FDO_SAFE_RELEASE(function_definition);

}  //  ~FdoFunctionCount ()


// ----------------------------------------------------------------------------
// --                            Public Class APIs                           --
// ----------------------------------------------------------------------------

FdoFunctionCount *FdoFunctionCount::Create ()

// +---------------------------------------------------------------------------
// | The function creates a new instance of the class.
// +---------------------------------------------------------------------------

{

    return new FdoFunctionCount();

}  //  Create ()

FdoFunctionCount *FdoFunctionCount::CreateObject ()

// +---------------------------------------------------------------------------
// | The function creates a new instance of the class.
// +---------------------------------------------------------------------------

{

    return new FdoFunctionCount();

}  //  CreateObject ()

FdoFunctionDefinition *FdoFunctionCount::GetFunctionDefinition ()

// +---------------------------------------------------------------------------
// | The function creates the supported signature list for the function COUNT.
// +---------------------------------------------------------------------------

{

    if (function_definition == NULL)
        CreateFunctionDefinition();

    return FDO_SAFE_ADDREF(function_definition);

}  //  GetFunctionDefinition ()

void FdoFunctionCount::Process (FdoLiteralValueCollection *literal_values)

// +---------------------------------------------------------------------------
// | The function processes a call to the function COUNT.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoPtr<FdoBooleanValue>  bool_value;
    FdoPtr<FdoByteValue>     byte_value;
    FdoPtr<FdoBLOBValue>     blob_value;
    FdoPtr<FdoCLOBValue>     clob_value;
    FdoPtr<FdoDateTimeValue> date_time_value;
    FdoPtr<FdoDecimalValue>  decimal_value;
    FdoPtr<FdoDoubleValue>   double_value;
    FdoPtr<FdoInt16Value>    int16_value;
    FdoPtr<FdoInt32Value>    int32_value;
    FdoPtr<FdoInt64Value>    int64_value;
    FdoPtr<FdoSingleValue>   single_value;
    FdoPtr<FdoStringValue>   string_value;
	FdoPtr<FdoGeometryValue> geom_value;

    // If the argument list has not been validated, execute the check next.
    // NOTE: the validation is executed only the first time the procedure is
    //       invoked and assumes that it remains the same until the function
    //       result is retrieved.

    if (!is_validated) {

        Validate(literal_values);
        is_validated = true;

    }  //  if (!is_validated) ...

    // Process the request.

    process_call_count++;

    switch (incoming_data_type) {

      case FdoDataType_Boolean:
        bool_value =
                    (FdoBooleanValue *) literal_values->GetItem(process_value);
        if (!bool_value->IsNull())
            ProcessRequest(bool_value->GetBoolean());
        else
          NULL_value_count++;
        break;

      case FdoDataType_Byte:
        byte_value = (FdoByteValue *) literal_values->GetItem(process_value);
        if (!byte_value->IsNull())
            ProcessRequest(byte_value->GetByte());
        else
          NULL_value_count++;
        break;

      case FdoDataType_BLOB:
        blob_value =
                (FdoBLOBValue *) literal_values->GetItem(process_value);
        if (!blob_value->IsNull())
            function_result++;
        else
          NULL_value_count++;
        break;

      case FdoDataType_CLOB:
        clob_value =
                (FdoCLOBValue *) literal_values->GetItem(process_value);
        if (!clob_value->IsNull())
            function_result++;
        else
          NULL_value_count++;
        break;

      case FdoDataType_DateTime:
        date_time_value =
                (FdoDateTimeValue *) literal_values->GetItem(process_value);
        if (!date_time_value->IsNull())
            ProcessRequest(date_time_value->GetDateTime());
        else
          NULL_value_count++;
        break;

      case FdoDataType_Decimal:
        decimal_value =
                    (FdoDecimalValue *) literal_values->GetItem(process_value);
        if (!decimal_value->IsNull())
            ProcessRequest(decimal_value->GetDecimal());
        else
          NULL_value_count++;
        break;

      case FdoDataType_Double:
        double_value =
                    (FdoDoubleValue *) literal_values->GetItem(process_value);
        if (!double_value->IsNull())
            ProcessRequest(double_value->GetDouble());
        else
          NULL_value_count++;
        break;

      case FdoDataType_Int16:
        int16_value = (FdoInt16Value *) literal_values->GetItem(process_value);
        if (!int16_value->IsNull())
            ProcessRequest(int16_value->GetInt16());
        else
          NULL_value_count++;
        break;

      case FdoDataType_Int32:
        int32_value = (FdoInt32Value *) literal_values->GetItem(process_value);
        if (!int32_value->IsNull())
            ProcessRequest(int32_value->GetInt32());
        else
          NULL_value_count++;
        break;

      case FdoDataType_Int64:
        int64_value = (FdoInt64Value *) literal_values->GetItem(process_value);
        if (!int64_value->IsNull())
            ProcessRequest(int64_value->GetInt64());
        else
          NULL_value_count++;
        break;

      case FdoDataType_Single:
        single_value =
                    (FdoSingleValue *) literal_values->GetItem(process_value);
        if (!single_value->IsNull())
            ProcessRequest(single_value->GetSingle());
        else
          NULL_value_count++;
        break;

      case FdoDataType_String:
        string_value =
                    (FdoStringValue *) literal_values->GetItem(process_value);
        if (!string_value->IsNull())
            ProcessRequest(string_value->GetString());
        else
          NULL_value_count++;
        break;

      case -1:  // Geometry
        geom_value = (FdoGeometryValue *) literal_values->GetItem(process_value);
        if (!geom_value->IsNull())
            function_result++;
        else
          NULL_value_count++;
        break;

    }  //  switch ...

}  //  Process ()

FdoLiteralValue *FdoFunctionCount::GetResult ()

// +---------------------------------------------------------------------------
// | The function returns the result of a call to the function COUNT.
// +---------------------------------------------------------------------------

{

    // Invalidate the flag indicating that the validation has been done.

    is_validated = false;

    // Return the result.

    return (NULL_value_count == process_call_count)
            ? FdoInt64Value::Create()
            : FdoInt64Value::Create(function_result);

}  //  GetResult ()


// ----------------------------------------------------------------------------
// --                          Supporting functions                          --
// ----------------------------------------------------------------------------

void FdoFunctionCount::CreateFunctionDefinition ()

// +---------------------------------------------------------------------------
// | The procedure creates the function definition for the function COUNT. The
// | function definition includes the list of supported signatures. The follow-
// | ing signatures are supported:
// |
// |    COUNT ([string, ]
// |         {boolean, blob, byte, clob, date/time, decimal, double, int16,
// |          int32, int64, single, string})
// |    COUNT (geometry);
// |
// | If the optional first parameter is used, then the parameter value must be
// | ALL or DISTINCT. The paranmeter is invalid if the second parameter is of
// | type BLOB or CLOB. The function always returns a Int64.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoString *desc = NULL;

    FdoStringP arg1_description;
    FdoStringP arg2_description;
    FdoStringP bool_arg_literal;
    FdoStringP blob_arg_literal;
    FdoStringP byte_arg_literal;
    FdoStringP clob_arg_literal;
    FdoStringP date_arg_literal;
    FdoStringP num_arg_literal;
    FdoStringP opt_arg_literal;
    FdoStringP str_arg_literal;
    FdoStringP geom_arg_literal;

    FdoPtr<FdoArgumentDefinition> bool_arg;
    FdoPtr<FdoArgumentDefinition> blob_arg;
    FdoPtr<FdoArgumentDefinition> byte_arg;
    FdoPtr<FdoArgumentDefinition> clob_arg;
    FdoPtr<FdoArgumentDefinition> date_arg;
    FdoPtr<FdoArgumentDefinition> dcl_arg;
    FdoPtr<FdoArgumentDefinition> dbl_arg;
    FdoPtr<FdoArgumentDefinition> int16_arg;
    FdoPtr<FdoArgumentDefinition> int32_arg;
    FdoPtr<FdoArgumentDefinition> int64_arg;
    FdoPtr<FdoArgumentDefinition> opt_arg;
    FdoPtr<FdoArgumentDefinition> sgl_arg;
    FdoPtr<FdoArgumentDefinition> str_arg;
    FdoPtr<FdoArgumentDefinition> geom_arg;

    FdoPtr<FdoArgumentDefinitionCollection> bool_args;
    FdoPtr<FdoArgumentDefinitionCollection> blob_args;
    FdoPtr<FdoArgumentDefinitionCollection> byte_args;
    FdoPtr<FdoArgumentDefinitionCollection> clob_args;
    FdoPtr<FdoArgumentDefinitionCollection> date_args;
    FdoPtr<FdoArgumentDefinitionCollection> dcl_args;
    FdoPtr<FdoArgumentDefinitionCollection> dbl_args;
    FdoPtr<FdoArgumentDefinitionCollection> int16_args;
    FdoPtr<FdoArgumentDefinitionCollection> int32_args;
    FdoPtr<FdoArgumentDefinitionCollection> int64_args;
    FdoPtr<FdoArgumentDefinitionCollection> sgl_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_args;
    FdoPtr<FdoArgumentDefinitionCollection> geom_args;

    FdoPtr<FdoArgumentDefinitionCollection> bool_opt_args;
    FdoPtr<FdoArgumentDefinitionCollection> byte_opt_args;
    FdoPtr<FdoArgumentDefinitionCollection> date_opt_args;
    FdoPtr<FdoArgumentDefinitionCollection> dcl_opt_args;
    FdoPtr<FdoArgumentDefinitionCollection> dbl_opt_args;
    FdoPtr<FdoArgumentDefinitionCollection> int16_opt_args;
    FdoPtr<FdoArgumentDefinitionCollection> int32_opt_args;
    FdoPtr<FdoArgumentDefinitionCollection> int64_opt_args;
    FdoPtr<FdoArgumentDefinitionCollection> sgl_opt_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_opt_args;

    FdoPtr<FdoDataValueCollection>          values;
    FdoPtr<FdoPropertyValueConstraintList>  argument_value_list;

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

    geom_arg_literal = FdoException::NLSGetMessage(FUNCTION_GEOM_ARG_LIT,
                                                   "geometry property");

    // The supported signatures allow an optional first parameter. The follow-
    // ing defines the argument definition for this parameter.

    opt_arg =
           FdoArgumentDefinition::Create(
                        opt_arg_literal, arg2_description, FdoDataType_String);
    opt_arg->SetArgumentValueList(argument_value_list);

    // The following defines the different argument definition collections.

    bool_arg_literal =
                FdoException::NLSGetMessage(FUNCTION_BOOL_ARG_LIT, "boolean");
    bool_arg = FdoArgumentDefinition::Create(
                    bool_arg_literal, arg1_description, FdoDataType_Boolean);

    bool_args = FdoArgumentDefinitionCollection::Create();
    bool_args->Add(bool_arg);

    bool_opt_args = FdoArgumentDefinitionCollection::Create();
    bool_opt_args->Add(opt_arg);
    bool_opt_args->Add(bool_arg);

    blob_arg_literal =
                FdoException::NLSGetMessage(FUNCTION_BLOB_ARG_LIT, "blob");
    blob_arg = FdoArgumentDefinition::Create(
                    blob_arg_literal, arg1_description, FdoDataType_BLOB);

    blob_args = FdoArgumentDefinitionCollection::Create();
    blob_args->Add(blob_arg);

    byte_arg_literal =
                FdoException::NLSGetMessage(FUNCTION_BYTE_ARG_LIT, "byte");
    byte_arg = FdoArgumentDefinition::Create(
                        byte_arg_literal, arg1_description, FdoDataType_Byte);

    byte_args = FdoArgumentDefinitionCollection::Create();
    byte_args->Add(byte_arg);

    byte_opt_args = FdoArgumentDefinitionCollection::Create();
    byte_opt_args->Add(opt_arg);
    byte_opt_args->Add(byte_arg);

    clob_arg_literal =
                FdoException::NLSGetMessage(FUNCTION_CLOB_ARG_LIT, "clob");
    clob_arg = FdoArgumentDefinition::Create(
                    clob_arg_literal, arg1_description, FdoDataType_CLOB);

    clob_args = FdoArgumentDefinitionCollection::Create();
    clob_args->Add(clob_arg);

    date_arg_literal =
                FdoException::NLSGetMessage(FUNCTION_DATE_ARG_LIT, "date/time");
    date_arg = FdoArgumentDefinition::Create(
                    date_arg_literal, arg1_description, FdoDataType_DateTime);

    date_args = FdoArgumentDefinitionCollection::Create();
    date_args->Add(date_arg);

    date_opt_args = FdoArgumentDefinitionCollection::Create();
    date_opt_args->Add(opt_arg);
    date_opt_args->Add(date_arg);

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

    str_arg_literal =
            FdoException::NLSGetMessage(
                                    FUNCTION_STRING_ARG_LIT, "text property");
    str_arg =
        FdoArgumentDefinition::Create(
                        str_arg_literal, arg1_description, FdoDataType_String);

    str_args = FdoArgumentDefinitionCollection::Create();
    str_args->Add(str_arg);

    str_opt_args = FdoArgumentDefinitionCollection::Create();
    str_opt_args->Add(opt_arg);
    str_opt_args->Add(str_arg);

    geom_arg =
            FdoArgumentDefinition::Create(geom_arg_literal,
                                          arg1_description,
                                          FdoPropertyType_GeometricProperty,
                                          (FdoDataType)-1);

    geom_args = FdoArgumentDefinitionCollection::Create();
    geom_args->Add(geom_arg);

    // Create the signature collection.

    signatures = FdoSignatureDefinitionCollection::Create();

    signature = FdoSignatureDefinition::Create(FdoDataType_Int64, bool_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                            FdoDataType_Int64, bool_opt_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(FdoDataType_Int64, blob_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(FdoDataType_Int64, byte_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                            FdoDataType_Int64, byte_opt_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(FdoDataType_Int64, clob_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                            FdoDataType_Int64, date_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Int64, date_opt_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(FdoDataType_Int64, dcl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(FdoDataType_Int64, dbl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(FdoDataType_Int64, int16_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(FdoDataType_Int64, int32_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(FdoDataType_Int64, int64_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(FdoDataType_Int64, sgl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                            FdoDataType_Int64, dcl_opt_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                            FdoDataType_Int64, dbl_opt_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                            FdoDataType_Int64, int16_opt_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                            FdoDataType_Int64, int32_opt_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                            FdoDataType_Int64, int64_opt_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                            FdoDataType_Int64, sgl_opt_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(FdoDataType_String, str_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                            FdoDataType_String, str_opt_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(FdoDataType_Double, geom_args);
    signatures->Add(signature);

    // Create the function definition.

    desc = FdoException::NLSGetMessage(
                            FUNCTION_COUNT,
                            "Determines the number of objects in the query");
    function_definition =
                FdoFunctionDefinition::Create(
                                        FDO_FUNCTION_COUNT,
                                        desc,
                                        true,
                                        signatures,
                                        FdoFunctionCategoryType_Aggregate);

}  //  CreateFunctionDefinition ()

void FdoFunctionCount::ProcessRequest (bool value)

// +---------------------------------------------------------------------------
// | The function processes a request to the Expression Engine function COUNT
// | when applied to values of type BOOL.
// +---------------------------------------------------------------------------

{

    if (is_distinct_request) {

        if ((!value) && (!bool_false_found)) {

            bool_false_found = true;
            function_result++;

        }  //  if ((!value) && (!bool_false_found)) ...

        if ((value) && (!bool_true_found)) {

            bool_true_found = true;
            function_result++;

        }  //  if ((value) && (!bool_true_found)) ...

    }  //  if (is_distinct_request) ...
    else
      function_result++;

}  //  ProcessRequest ()

void FdoFunctionCount::ProcessRequest (FdoByte value)

// +---------------------------------------------------------------------------
// | The function processes a request to the Expression Engine function COUNT
// | when applied to values of type BYTE.
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
          if (curr_cache_value->GetByteValue() == value) {

              data_processed = true;
              break;

          }  //  if (curr_cache_value->GetByteValue() == value) ...

        }  //  for ...

        // Data hasn't been processed yet. Add it to the cache.

        if (!data_processed) {

            new_cache_value = CacheValue::Create(value);
            value_cache->Add(new_cache_value);

        }  //  if (!data_processed) ...

    }  //  if (is_distinct_request) ...

    // If the data hasn't been processed yet, count it.

    if (!data_processed)
        function_result++;

}  //  ProcessRequest ()

void FdoFunctionCount::ProcessRequest (FdoDateTime value)

// +---------------------------------------------------------------------------
// | The function processes a request to the Expression Engine function COUNT
// | when applied to values of type DATETIME.
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
          if ((FdoCommonMiscUtil::CompareDateTimes(
                                curr_cache_value->GetDateTimeValue(),
                                value                                ) == 0)) {

              data_processed = true;
              break;

          }  //  if (curr_cache_value->GetDateTimeValue() == value) ...

        }  //  for ...

        // Data hasn't been processed yet. Add it to the cache.

        if (!data_processed) {

            new_cache_value = CacheValue::Create(value);
            value_cache->Add(new_cache_value);

        }  //  if (!data_processed) ...

    }  //  if (is_distinct_request) ...

    // If the data hasn't been processed yet, count it.

    if (!data_processed)
        function_result++;

}  //  ProcessRequest ()

void FdoFunctionCount::ProcessRequest (FdoDouble value)

// +---------------------------------------------------------------------------
// | The function processes a request to the Expression Engine function COUNT
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

    // If the data hasn't been processed yet, count it.

    if (!data_processed)
        function_result++;

}  //  ProcessRequest ()

void FdoFunctionCount::ProcessRequest (FdoFloat value)

// +---------------------------------------------------------------------------
// | The function processes a request to the Expression Engine function COUNT
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

    // If the data hasn't been processed yet, count it.

    if (!data_processed)
        function_result++;

}  //  ProcessRequest ()

void FdoFunctionCount::ProcessRequest (FdoInt16 value)

// +---------------------------------------------------------------------------
// | The function processes a request to the Expression Engine function COUNT
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

    // If the data hasn't been processed yet, count it.

    if (!data_processed)
        function_result++;

}  //  ProcessRequest ()

void FdoFunctionCount::ProcessRequest (FdoInt32 value)

// +---------------------------------------------------------------------------
// | The function processes a request to the Expression Engine function COUNT
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

    // If the data hasn't been processed yet, count it.

    if (!data_processed)
        function_result++;

}  //  ProcessRequest ()

void FdoFunctionCount::ProcessRequest (FdoInt64 value)

// +---------------------------------------------------------------------------
// | The function processes a request to the Expression Engine function COUNT
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

    // If the data hasn't been processed yet, count it.

    if (!data_processed)
        function_result++;

}  //  ProcessRequest ()

void FdoFunctionCount::ProcessRequest (FdoString *value)

// +---------------------------------------------------------------------------
// | The function processes a request to the Expression Engine function COUNT
// | when applied to values of type STRING.
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
          if (wcscmp(curr_cache_value->GetStringValue(), value) == 0) {

              data_processed = true;
              break;

          }  //  if (wcscmp(curr_cache_value->GetStringValue(), value) == 0) ...

        }  //  for ...

        // Data hasn't been processed yet. Add it to the cache.

        if (!data_processed) {

            new_cache_value = CacheValue::Create(value);
            value_cache->Add(new_cache_value);

        }  //  if (!data_processed) ...

    }  //  if (is_distinct_request) ...

    // If the data hasn't been processed yet, count it.

    if (!data_processed)
        function_result++;

}  //  ProcessRequest ()

void FdoFunctionCount::Validate (FdoLiteralValueCollection *literal_values)

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

    // Check the number of arguments. COUNT can accept either one or two para-
    // meters. If the number of parameters is not correct issue an exception.

    if ((count < 1) || (count > 2)) 
        throw FdoException::Create(
                FdoException::NLSGetMessage(
                  FUNCTION_PARAMETER_NUMBER_ERROR, 
                  "Expression Engine: Invalid number of parameters for function '%1$ls'",
                  FDO_FUNCTION_COUNT));

    // If there are two function parameters the value of the first one must be
    // either "ALL" or "DISTINCT". If this is not the case issue an exception.

    if (count == 2) {
  
        literal_value = literal_values->GetItem(0);
        if (literal_value->GetLiteralValueType() != FdoLiteralValueType_Data)
            throw FdoException::Create(
                      FdoException::NLSGetMessage(
                          FUNCTION_PARAMETER_ERROR, 
                          "Expression Engine: Invalid parameters for function '%1$ls'",
                          FDO_FUNCTION_COUNT));

        data_value = static_cast<FdoDataValue *>(literal_value.p);
        if (data_value->GetDataType() != FdoDataType_String)
            throw FdoException::Create(
                      FdoException::NLSGetMessage(
                          FUNCTION_PARAMETER_DATA_TYPE_ERROR, 
                          "Expression Engine: Invalid parameter data type for function '%1$ls'",
                          FDO_FUNCTION_COUNT));

        str_value   = static_cast<FdoStringValue *>(literal_value.p);
        if (str_value->IsNull())
            throw FdoException::Create(
                FdoException::NLSGetMessage(
                  FUNCTION_OPERATOR_ERROR, 
                  "Expression Engine: Invalid operator parameter value for function '%1$ls'",
                  FDO_FUNCTION_COUNT));

        param_value = str_value->GetString();
        if ((FdoCommonStringUtil::StringCompareNoCase(
                                        param_value, L"ALL"     ) != 0) &&
            (FdoCommonStringUtil::StringCompareNoCase(
                                        param_value, L"DISTINCT") != 0)    )
            throw FdoException::Create(
                FdoException::NLSGetMessage(
                  FUNCTION_OPERATOR_ERROR, 
                  "Expression Engine: Invalid operator parameter value for function '%1$ls'",
                  FDO_FUNCTION_COUNT));

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

    // The function works on all supported data types. Ensure this is the case.
    // If the data type is BLOB or CLOB, the caller cannot use DISTINCT as the
    // first parameter value if there are two parameters provided.

    literal_value = literal_values->GetItem(process_value);
    if (literal_value->GetLiteralValueType() == FdoLiteralValueType_Geometry)
        incoming_data_type = (FdoDataType) -1;
	else {

      data_value = static_cast<FdoDataValue *>(literal_value.p);
      incoming_data_type = data_value->GetDataType();
      if (((incoming_data_type == FdoDataType_BLOB ) ||
           (incoming_data_type == FdoDataType_CLOB )    ) &&
          (is_distinct_request)                               )
        throw FdoException::Create(
                FdoException::NLSGetMessage(
                    FUNCTION_PARAMETER_DISTINCT_ERROR, 
                    "Expression Engine: DISTINCT not allowed for BLOB/CLOB for function '%1$ls'",
                    FDO_FUNCTION_COUNT));

    }  //  else ...

}  //  Validate ()

