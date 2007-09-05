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
#include <Functions/Aggregate/FdoFunctionMax.h>
#include "StringUtility.h"


// ----------------------------------------------------------------------------
// --                         Constructors/Destructors                       --
// ----------------------------------------------------------------------------

FdoFunctionMax::FdoFunctionMax ()

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

    process_value       = 0;

    function_definition = NULL;

    incoming_data_type  = FdoDataType_CLOB;

    // Setup the result cache.

    value_cache               = new ResultCacheClass();
    value_cache->string_value = NULL;
    value_cache->value_set    = false;

}  //  FdoFunctionMax ()


FdoFunctionMax::~FdoFunctionMax ()

// +---------------------------------------------------------------------------
// | The class destructor.
// +---------------------------------------------------------------------------

{

    // Delete the result cache and function definition.

    if (value_cache->string_value != NULL)
        delete[] value_cache->string_value;
    delete value_cache;
    FDO_SAFE_RELEASE(function_definition);

}  //  ~FdoFunctionMax ()


// ----------------------------------------------------------------------------
// --                            Public Class APIs                           --
// ----------------------------------------------------------------------------

FdoFunctionMax *FdoFunctionMax::Create ()

// +---------------------------------------------------------------------------
// | The function creates a new instance of the class.
// +---------------------------------------------------------------------------

{

    return new FdoFunctionMax();

}  //  Create ()

FdoFunctionMax *FdoFunctionMax::CreateObject ()

// +---------------------------------------------------------------------------
// | The function creates a new instance of the class.
// +---------------------------------------------------------------------------

{

    return new FdoFunctionMax();

}  //  CreateObject ()

FdoFunctionDefinition *FdoFunctionMax::GetFunctionDefinition ()

// +---------------------------------------------------------------------------
// | The function creates the supported signature list for the function MAX.
// +---------------------------------------------------------------------------

{

    if (function_definition == NULL)
        CreateFunctionDefinition();

    return FDO_SAFE_ADDREF(function_definition);

}  //  GetFunctionDefinition ()

void FdoFunctionMax::Process (FdoLiteralValueCollection *literal_values)

// +---------------------------------------------------------------------------
// | The function processes a call to the function MAX.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoPtr<FdoByteValue>     byte_value;
    FdoPtr<FdoDateTimeValue> date_time_value;
    FdoPtr<FdoDecimalValue>  decimal_value;
    FdoPtr<FdoDoubleValue>   double_value;
    FdoPtr<FdoInt16Value>    int16_value;
    FdoPtr<FdoInt32Value>    int32_value;
    FdoPtr<FdoInt64Value>    int64_value;
    FdoPtr<FdoSingleValue>   single_value;
    FdoPtr<FdoStringValue>   string_value;

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
        byte_value = (FdoByteValue *) literal_values->GetItem(process_value);
        if (!byte_value->IsNull())
            ProcessRequest(byte_value->GetByte());
        break;

      case FdoDataType_DateTime:
        date_time_value =
                (FdoDateTimeValue *) literal_values->GetItem(process_value);
        if (!date_time_value->IsNull())
            ProcessRequest(date_time_value->GetDateTime());
        break;

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

      case FdoDataType_String:
        string_value =
                    (FdoStringValue *) literal_values->GetItem(process_value);
        if (!string_value->IsNull())
            ProcessRequest(string_value->GetString());
        break;

    }  //  switch ...

    value_cache->value_set = true;

}  //  Process ()

FdoLiteralValue *FdoFunctionMax::GetResult ()

// +---------------------------------------------------------------------------
// | The function returns the result of a call to the function MAX.
// +---------------------------------------------------------------------------

{

    // Invalidate the flag indicating that the validation has been done.

    is_validated = false;


    // Return the result.

    switch (incoming_data_type) {

      case FdoDataType_Byte:
        return (value_cache->value_set)
                ? FdoByteValue::Create(value_cache->byte_value)
                : FdoByteValue::Create();
        break;

      case FdoDataType_DateTime:
        return (value_cache->value_set)
                ? FdoDateTimeValue::Create(value_cache->date_time_value)
                : FdoDateTimeValue::Create();
        break;

      case FdoDataType_Decimal:
        return (value_cache->value_set)
                ? FdoDecimalValue::Create(value_cache->double_value)
                : FdoDecimalValue::Create();
        break;

      case FdoDataType_Double:
        return (value_cache->value_set)
                ? FdoDoubleValue::Create(value_cache->double_value)
                : FdoDoubleValue::Create();
        break;

      case FdoDataType_Int16:
        return (value_cache->value_set)
                ? FdoInt16Value::Create(value_cache->int16_value)
                : FdoInt16Value::Create();
        break;

      case FdoDataType_Int32:
        return (value_cache->value_set)
                ? FdoInt32Value::Create(value_cache->int32_value)
                : FdoInt32Value::Create();
        break;

      case FdoDataType_Int64:
        return (value_cache->value_set)
                ? FdoInt64Value::Create(value_cache->int64_value)
                : FdoInt64Value::Create();
        break;

      case FdoDataType_Single:
        return (value_cache->value_set)
                ? FdoSingleValue::Create(value_cache->float_value)
                : FdoSingleValue::Create();
        break;

      case FdoDataType_String:
        return (value_cache->value_set)
                ? FdoStringValue::Create(value_cache->string_value)
                : FdoStringValue::Create();
        break;

    }  //  switch ...

    throw FdoCommandException::Create(
            FdoException::NLSGetMessage(
                FUNCTION_RESULT_DATA_TYPE_ERROR, 
                "Expression Engine: Invalid data type for return value for function '%1$ls'",
                FDO_FUNCTION_MAX));

}  //  GetResult ()


// ----------------------------------------------------------------------------
// --                          Supporting functions                          --
// ----------------------------------------------------------------------------

void FdoFunctionMax::CreateFunctionDefinition ()

// +---------------------------------------------------------------------------
// | The procedure creates the function definition for the function MAX. The
// | function definition includes the list of supported signatures. The follow-
// | ing signatures are supported:
// |
// |    MAX ([string, ]
// |         {byte, date/time, decimal, double, int16, int32, int64, single,
// |          string})
// |
// | If the optional first parameter is used, then the parameter value must be
// | ALL or DISTINCT. The function returns the same data type as the input pa-
// | rameter.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoString *desc = NULL;

    FdoStringP arg1_description;
    FdoStringP arg2_description;
    FdoStringP byte_arg_literal;
    FdoStringP date_arg_literal;
    FdoStringP num_arg_literal;
    FdoStringP str_arg_literal;
    FdoStringP opt_arg_literal;

    FdoPtr<FdoArgumentDefinition> byte_arg;
    FdoPtr<FdoArgumentDefinition> date_arg;
    FdoPtr<FdoArgumentDefinition> dcl_arg;
    FdoPtr<FdoArgumentDefinition> dbl_arg;
    FdoPtr<FdoArgumentDefinition> int16_arg;
    FdoPtr<FdoArgumentDefinition> int32_arg;
    FdoPtr<FdoArgumentDefinition> int64_arg;
    FdoPtr<FdoArgumentDefinition> opt_arg;
    FdoPtr<FdoArgumentDefinition> sgl_arg;
    FdoPtr<FdoArgumentDefinition> str_arg;

    FdoPtr<FdoArgumentDefinitionCollection> byte_args;
    FdoPtr<FdoArgumentDefinitionCollection> date_args;
    FdoPtr<FdoArgumentDefinitionCollection> dcl_args;
    FdoPtr<FdoArgumentDefinitionCollection> dbl_args;
    FdoPtr<FdoArgumentDefinitionCollection> int16_args;
    FdoPtr<FdoArgumentDefinitionCollection> int32_args;
    FdoPtr<FdoArgumentDefinitionCollection> int64_args;
    FdoPtr<FdoArgumentDefinitionCollection> sgl_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_args;

    FdoPtr<FdoArgumentDefinitionCollection> byte_opt_args;
    FdoPtr<FdoArgumentDefinitionCollection> date_opt_args;
    FdoPtr<FdoArgumentDefinitionCollection> dcl_opt_args;
    FdoPtr<FdoArgumentDefinitionCollection> dbl_opt_args;
    FdoPtr<FdoArgumentDefinitionCollection> int16_opt_args;
    FdoPtr<FdoArgumentDefinitionCollection> int32_opt_args;
    FdoPtr<FdoArgumentDefinitionCollection> int64_opt_args;
    FdoPtr<FdoArgumentDefinitionCollection> sgl_opt_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_opt_args;

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

    byte_arg_literal =
                FdoException::NLSGetMessage(FUNCTION_BYTE_ARG_LIT, "byte");
    byte_arg = FdoArgumentDefinition::Create(
                        byte_arg_literal, arg1_description, FdoDataType_Byte);

    byte_args = FdoArgumentDefinitionCollection::Create();
    byte_args->Add(byte_arg);

    byte_opt_args = FdoArgumentDefinitionCollection::Create();
    byte_opt_args->Add(opt_arg);
    byte_opt_args->Add(byte_arg);

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
            FdoException::NLSGetMessage(FUNCTION_STRING_ARG_LIT, "string");
    str_arg =
        FdoArgumentDefinition::Create(
                        str_arg_literal, arg1_description, FdoDataType_String);

    str_args = FdoArgumentDefinitionCollection::Create();
    str_args->Add(str_arg);

    str_opt_args = FdoArgumentDefinitionCollection::Create();
    str_opt_args->Add(opt_arg);
    str_opt_args->Add(str_arg);

    // Create the signature collection.

    signatures = FdoSignatureDefinitionCollection::Create();

    signature = FdoSignatureDefinition::Create(FdoDataType_Byte, byte_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                            FdoDataType_Byte, byte_opt_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                            FdoDataType_DateTime, date_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_DateTime, date_opt_args);
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
                                            FdoDataType_Decimal, dcl_opt_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                            FdoDataType_Double, dbl_opt_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                            FdoDataType_Int16, int16_opt_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                            FdoDataType_Int32, int32_opt_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                            FdoDataType_Int64, int64_opt_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                            FdoDataType_Single, sgl_opt_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(FdoDataType_String, str_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                            FdoDataType_String, str_opt_args);
    signatures->Add(signature);

    // Create the function definition.

    desc = FdoException::NLSGetMessage(
                            FUNCTION_MAX,
                            "Determines the maximum value of an expression");
    function_definition =
                FdoFunctionDefinition::Create(
                                        FDO_FUNCTION_MAX,
                                        desc,
                                        true,
                                        signatures,
                                        FdoFunctionCategoryType_Aggregate);

}  //  CreateFunctionDefinition ()

void FdoFunctionMax::ProcessRequest (FdoByte value)

// +---------------------------------------------------------------------------
// | The function processes a request to the Expression Engine function MAX
// | when applied to values of type BYTE.
// +---------------------------------------------------------------------------

{

    if ((!value_cache->value_set) || (value > value_cache->byte_value))
        value_cache->byte_value = value;

}  //  ProcessRequest ()

void FdoFunctionMax::ProcessRequest (FdoDateTime value)

// +---------------------------------------------------------------------------
// | The function processes a request to the Expression Engine function MAX
// | when applied to values of type DATE/TIME.
// +---------------------------------------------------------------------------

{

    if ((!value_cache->value_set) ||
        (FdoCommonMiscUtil::CompareDateTimes(
                                    value, value_cache->date_time_value) > 0))
        value_cache->date_time_value = value;

}  //  ProcessRequest ()

void FdoFunctionMax::ProcessRequest (FdoDouble value)

// +---------------------------------------------------------------------------
// | The function processes a request to the Expression Engine function MAX
// | when applied to values of type DOUBLE.
// +---------------------------------------------------------------------------

{

    if ((!value_cache->value_set) || (value > value_cache->double_value))
        value_cache->double_value = value;

}  //  ProcessRequest ()

void FdoFunctionMax::ProcessRequest (FdoFloat value)

// +---------------------------------------------------------------------------
// | The function processes a request to the Expression Engine function MAX
// | when applied to values of type FLOAT.
// +---------------------------------------------------------------------------

{

    if ((!value_cache->value_set) || (value > value_cache->float_value))
        value_cache->float_value = value;

}  //  ProcessRequest ()

void FdoFunctionMax::ProcessRequest (FdoInt16 value)

// +---------------------------------------------------------------------------
// | The function processes a request to the Expression Engine function MAX
// | when applied to values of type INT16.
// +---------------------------------------------------------------------------

{

    if ((!value_cache->value_set) || (value > value_cache->int16_value))
        value_cache->int16_value = value;

}  //  ProcessRequest ()

void FdoFunctionMax::ProcessRequest (FdoInt32 value)

// +---------------------------------------------------------------------------
// | The function processes a request to the Expression Engine function MAX
// | when applied to values of type INT32.
// +---------------------------------------------------------------------------

{

    if ((!value_cache->value_set) || (value > value_cache->int32_value))
        value_cache->int32_value = value;

}  //  ProcessRequest ()

void FdoFunctionMax::ProcessRequest (FdoInt64 value)

// +---------------------------------------------------------------------------
// | The function processes a request to the Expression Engine function MAX
// | when applied to values of type INT64.
// +---------------------------------------------------------------------------

{

    if ((!value_cache->value_set) || (value > value_cache->int64_value))
        value_cache->int64_value = value;

}  //  ProcessRequest ()

void FdoFunctionMax::ProcessRequest (FdoString *value)

// +---------------------------------------------------------------------------
// | The function processes a request to the Expression Engine function MAX
// | when applied to values of type STRING.
// +---------------------------------------------------------------------------

{

    if ((!value_cache->value_set) ||
        (wcscmp(value, value_cache->string_value) > 0)) {

            if (value_cache->string_value != NULL) {

                delete[] value_cache->string_value;
                value_cache->string_value = NULL;

            }  //  if (value_cache->string_value != NULL) ...

            value_cache->string_value = FdoStringUtility::MakeString(value);

    } //  if ((!value_cache->value_set) || ...

}  //  ProcessRequest ()

void FdoFunctionMax::Validate (FdoLiteralValueCollection *literal_values)

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

    // Check the number of arguments. MAX can accept either one or two para-
    // meters. If the number of parameters is not correct issue an exception.

    if ((count < 1) || (count > 2)) 
        throw FdoCommandException::Create(
               FdoException::NLSGetMessage(
                  FUNCTION_PARAMETER_NUMBER_ERROR, 
                  "Expression Engine: Invalid number of parameters for function '%1$ls'",
                  FDO_FUNCTION_MAX));

    // If there are two function parameters the value of the first one must
    // be either "ALL" or "DISTINCT". If this is not the case issue an excep-
    // tion.

    if (count == 2) {
  
        literal_value = literal_values->GetItem(0);
        if (literal_value->GetLiteralValueType() != FdoLiteralValueType_Data)
            throw FdoCommandException::Create(
                    FdoException::NLSGetMessage(
                        FUNCTION_PARAMETER_ERROR, 
                        "Expression Engine: Invalid parameters for function '%1$ls'",
                        FDO_FUNCTION_MAX));

        data_value = static_cast<FdoDataValue *>(literal_value.p);
        if (data_value->GetDataType() != FdoDataType_String)
            throw FdoCommandException::Create(
                    FdoException::NLSGetMessage(
                        FUNCTION_PARAMETER_DATA_TYPE_ERROR, 
                        "Expression Engine: Invalid parameter data type for function '%1$ls'",
                        FDO_FUNCTION_MAX));

        str_value   = static_cast<FdoStringValue *>(literal_value.p);
        param_value = str_value->GetString();
        if ((_wcsicmp(param_value, L"ALL"     ) != 0) &&
            (_wcsicmp(param_value, L"DISTINCT") != 0)    )
            throw FdoCommandException::Create(
                   FdoException::NLSGetMessage(
                      FUNCTION_OPERATOR_ERROR, 
                      "Expression Engine: Invalid first parameter value for function '%1$ls'",
                      FDO_FUNCTION_MAX));

    }  //  if (count == 2) ...

    // If two arguments are provided the value of the first argument is either
    // "ALL" or "DISTINCT". For this function it does not really matter because
    // the result doesn't change. Therefore, remember to always get the second
    // value when processing the request. Otherwise, use the first parameter
    // value.

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
                  FDO_FUNCTION_MAX));

    data_value = static_cast<FdoDataValue *>(literal_value.p);
    incoming_data_type = data_value->GetDataType();
    if ((incoming_data_type == FdoDataType_Boolean) ||
        (incoming_data_type == FdoDataType_BLOB   ) ||
        (incoming_data_type == FdoDataType_CLOB   )    )
        throw FdoCommandException::Create(
                FdoException::NLSGetMessage(
                  FUNCTION_PARAMETER_DATA_TYPE_ERROR, 
                  "Expression Engine: Invalid parameter data type for function '%1$ls'",
                  FDO_FUNCTION_MAX));

}  //  Validate ()


// ----------------------------------------------------------------------------
// --                         Helper Class functions                         --
// ----------------------------------------------------------------------------

FdoFunctionMax::ResultCacheClass::ResultCacheClass ()
{
}

FdoFunctionMax::ResultCacheClass::~ResultCacheClass ()
{
}