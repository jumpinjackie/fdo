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
#include <Functions/Date/FdoFunctionExtractToInt.h>
#include <FdoCommonStringUtil.h>


// ----------------------------------------------------------------------------
// --                         Constructors/Destructors                       --
// ----------------------------------------------------------------------------

FdoFunctionExtractToInt::FdoFunctionExtractToInt ()

// +---------------------------------------------------------------------------
// | The class constructor.
// +---------------------------------------------------------------------------

{

    // Initialize all class variables.

    function_definition = NULL;

    validation_required = true;

}  //  FdoFunctionExtractToInt ()


FdoFunctionExtractToInt::~FdoFunctionExtractToInt ()

// +---------------------------------------------------------------------------
// | The class destructor.
// +---------------------------------------------------------------------------

{

    // Delete the function definition.

    FDO_SAFE_RELEASE(function_definition);

}  //  ~FdoFunctionExtractToInt ()


// ----------------------------------------------------------------------------
// --                            Public Class APIs                           --
// ----------------------------------------------------------------------------

FdoFunctionExtractToInt *FdoFunctionExtractToInt::Create ()

// +---------------------------------------------------------------------------
// | The function creates a new instance of the class.
// +---------------------------------------------------------------------------

{

    return new FdoFunctionExtractToInt();

}  //  Create ()

FdoFunctionExtractToInt *FdoFunctionExtractToInt::CreateObject ()

// +---------------------------------------------------------------------------
// | The function creates a new instance of the class.
// +---------------------------------------------------------------------------

{

    return new FdoFunctionExtractToInt();

}  //  CreateObject ()

FdoFunctionDefinition *FdoFunctionExtractToInt::GetFunctionDefinition ()

// +---------------------------------------------------------------------------
// | The function creates the supported signature list for the function EX-
// | TRACTTOINT.
// +---------------------------------------------------------------------------

{

    if (function_definition == NULL)
        CreateFunctionDefinition();

    return FDO_SAFE_ADDREF(function_definition);

}  //  GetFunctionDefinition ()

FdoLiteralValue *FdoFunctionExtractToInt::Evaluate (
                                    FdoLiteralValueCollection *literal_values)

// +---------------------------------------------------------------------------
// | The function processes a call to the function EXTRACTTOINT.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoInt16                 result         = -1;

    FdoDateTime              dt;

    FdoDateTimeValue         *dt_value      = NULL;

    FdoPtr<FdoLiteralValue>  literal_value;

    if (validation_required) {

        Validate(literal_values);
        return_int32_value  = FdoInt32Value::Create();
        validation_required = false;

    }  //  if (validation_required) ...

    // Get the date-time value to be processed. If the value is not provided
    // return an empty. Otherwise extract the requested data, create the
    // function result and return it back to the calling routine.

    literal_value = literal_values->GetItem(1);
    dt_value      = static_cast<FdoDateTimeValue *>(literal_value.p);
    if (dt_value->IsNull()) {

        return_int32_value->SetNull();
        return FDO_SAFE_ADDREF(return_int32_value.p);

    }  //  if (dt_value->IsNull()) ...

    dt = dt_value->GetDateTime();
    switch (GetToken(function_operation_request)) {

      case ExtractOperationTokens_Year  : result = dt.year;    break;
      case ExtractOperationTokens_Month : result = dt.month;   break;
      case ExtractOperationTokens_Day   : result = dt.day;     break;
      case ExtractOperationTokens_Hour  : result = dt.hour;    break;
      case ExtractOperationTokens_Minute: result = dt.minute;  break;
      case ExtractOperationTokens_Second: 

        result = (FdoInt16)((FdoInt32)((dt.seconds - floor(dt.seconds)) < 0.5)
                    ? floor(dt.seconds)
                    : ceil(dt.seconds)); 
        break;

    }  //  switch ...

    return_int32_value->SetInt32(result);
    return FDO_SAFE_ADDREF(return_int32_value.p);

}  //  Evaluate ()


// ----------------------------------------------------------------------------
// --                          Supporting functions                          --
// ----------------------------------------------------------------------------

void FdoFunctionExtractToInt::CreateFunctionDefinition ()

// +---------------------------------------------------------------------------
// | The procedure creates the function definition for the function EXTRACTTO-
// | INT. The function definition includes the list of supported signatures.
// | The following signatures are supported:
// |
// |    EXTRACTTOINT ({YEAR | MONTH | DAY | HOUR | MINUTE | SECOND},
// |                  date_time)
// |
// | The function always returns a INT32.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoString                                *desc               = NULL;

    FdoStringP                               arg1_description;
    FdoStringP                               arg2_description;
    FdoStringP                               dt_arg_literal;
    FdoStringP                               op_arg_literal;

    FdoPtr<FdoArgumentDefinition>            dt_arg;
    FdoPtr<FdoArgumentDefinition>            op_arg;

    FdoPtr<FdoArgumentDefinitionCollection>  args;

    FdoPtr<FdoDataValueCollection>           values;
    FdoPtr<FdoPropertyValueConstraintList>   argument_value_list;

    FdoPtr<FdoSignatureDefinition>           signature;
    FdoPtr<FdoSignatureDefinitionCollection> signatures;

    // The value of the first argument of the supported signatures has to be
    // either YEAR, MONTH, DAY, HOUR, MINUTE or SECOND. The following sets up
    // an argument value list to be used for the argument describing this pa-
    // rameter.

    argument_value_list = FdoPropertyValueConstraintList::Create();
    values              = argument_value_list->GetConstraintList();
    values->Add(FdoPtr<FdoStringValue>(FdoStringValue::Create(L"YEAR")));
    values->Add(FdoPtr<FdoStringValue>(FdoStringValue::Create(L"MONTH")));
    values->Add(FdoPtr<FdoStringValue>(FdoStringValue::Create(L"DAY")));
    values->Add(FdoPtr<FdoStringValue>(FdoStringValue::Create(L"HOUR")));
    values->Add(FdoPtr<FdoStringValue>(FdoStringValue::Create(L"MINUTE")));
    values->Add(FdoPtr<FdoStringValue>(FdoStringValue::Create(L"SECOND")));

    // Get the general descriptions for the arguments.

    arg1_description =
        FdoException::NLSGetMessage(
                    FUNCTION_EXTRACT_OP_ARG,
                    "Indicates the portion of the date/time to be extracted");

    arg2_description =
            FdoException::NLSGetMessage(
                        FUNCTION_EXTRACT_DATE_ARG,
                        "The date/time from which to extract a portion from");

    // The following defines the different argument definition collections.

    op_arg_literal = FdoException::NLSGetMessage(FUNCTION_EXTRACT_OP_ARG_LIT,
                                                 "part of the date");

    dt_arg_literal = FdoException::NLSGetMessage(FUNCTION_EXTRACT_DT_ARG_LIT,
                                                 "from date");

    op_arg   = FdoArgumentDefinition::Create(
                    op_arg_literal, arg1_description, FdoDataType_String);
    op_arg->SetArgumentValueList(argument_value_list);

    dt_arg   = FdoArgumentDefinition::Create(
                    dt_arg_literal, arg2_description, FdoDataType_DateTime);

    args = FdoArgumentDefinitionCollection::Create();
    args->Add(op_arg);
    args->Add(dt_arg);

    // Create the signature collection.

    signatures = FdoSignatureDefinitionCollection::Create();

    signature = FdoSignatureDefinition::Create(FdoDataType_Int32, args);
    signatures->Add(signature);

    // Create the function definition.

    desc = FdoException::NLSGetMessage(FUNCTION_EXTRACT,
                                       "Extracts a portion of a date/time");
    function_definition =
                FdoFunctionDefinition::Create(
                                        FDO_FUNCTION_EXTRACTTOINT,
                                        desc,
                                        false,
                                        signatures,
                                        FdoFunctionCategoryType_Date);

}  //  CreateFunctionDefinition ()

FdoFunctionExtractToInt::ExtractOperationTokens
                                FdoFunctionExtractToInt::GetToken (
                                                        FdoStringP operation)

// +---------------------------------------------------------------------------
// | The function returns the token for the provided date truncate operator.
// +---------------------------------------------------------------------------

{

    if (FdoCommonStringUtil::StringCompareNoCase(operation, L"YEAR") == 0)
        return ExtractOperationTokens_Year;

    if (FdoCommonStringUtil::StringCompareNoCase(operation, L"MONTH") == 0)
        return ExtractOperationTokens_Month;

    if (FdoCommonStringUtil::StringCompareNoCase(operation, L"DAY") == 0)
        return ExtractOperationTokens_Day;

    if (FdoCommonStringUtil::StringCompareNoCase(operation, L"HOUR") == 0)
        return ExtractOperationTokens_Hour;

    if (FdoCommonStringUtil::StringCompareNoCase(operation, L"MINUTE") == 0)
        return ExtractOperationTokens_Minute;

    if (FdoCommonStringUtil::StringCompareNoCase(operation, L"SECOND") == 0)
        return ExtractOperationTokens_Second;

    // The function should never reach this part of the code, but just in case
    // an exception is issued.

    throw FdoException::Create(
            FdoException::NLSGetMessage(
              FUNCTION_OPERATOR_ERROR, 
              "Expression Engine: Invalid operator parameter value for function '%1$ls'",
              FDO_FUNCTION_EXTRACTTOINT));

}  //  GetToken ()

void FdoFunctionExtractToInt::Validate (
                                    FdoLiteralValueCollection *literal_values)

// +---------------------------------------------------------------------------
// | The function validates the argument list that was passed in.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    bool                    invalid_data_type;

    FdoInt32                i,
                            count               = literal_values->GetCount();

    FdoDataType             data_type;

    FdoDataValue            *data_value         = NULL;

    FdoStringValue          *str_value          = NULL;

    ExtractOperationTokens  op_token;

    FdoPtr<FdoLiteralValue> literal_value;

    // Check the number of arguments. EXTRACTTOINT accepts two parameters. If
    // the number of parameters is not correct issue an exception.

    count = literal_values->GetCount();

    if (count != 2) 
        throw FdoException::Create(
               FdoException::NLSGetMessage(
                  FUNCTION_PARAMETER_NUMBER_ERROR, 
                  "Expression Engine: Invalid number of parameters for function '%1$ls'",
                  FDO_FUNCTION_EXTRACTTOINT));

    // Loop through the arguments and verify that all arguments are valid. If
    // this is not the case issue an exception.

    for (i = 0; i < count; i++) {

      literal_value = literal_values->GetItem(i);
      if (literal_value->GetLiteralValueType() != FdoLiteralValueType_Data)
          throw FdoException::Create(
                FdoException::NLSGetMessage(
                  FUNCTION_PARAMETER_ERROR, 
                  "Expression Engine: Invalid parameters for function '%1$ls'",
                  FDO_FUNCTION_EXTRACTTOINT));

      data_value = static_cast<FdoDataValue *>(literal_value.p);
      data_type = data_value->GetDataType();
      invalid_data_type = (i == 0)
                        ? (data_type != FdoDataType_String)
                        : (data_type != FdoDataType_DateTime);

      if (invalid_data_type)
          throw FdoException::Create(
                FdoException::NLSGetMessage(
                  FUNCTION_PARAMETER_DATA_TYPE_ERROR, 
                  "Expression Engine: Invalid parameter data type for function '%1$ls'",
                  FDO_FUNCTION_EXTRACTTOINT));

      // If this is the first argument, check whether or not the value is one
      // of TEAR, MONTH, DAY, HOUR, MINUTE or SECOND. If this is not the case
      // issue an exception. Otherwise remember it for the processing of the
      // request.

      if (i == 0) {

          str_value = static_cast<FdoStringValue *>(literal_value.p);
          if (str_value->IsNull())
              throw FdoException::Create(
                      FdoException::NLSGetMessage(
                      FUNCTION_OPERATOR_ERROR, 
                      "Expression Engine: Invalid operator parameter value for function '%1$ls'",
                      FDO_FUNCTION_EXTRACTTOINT));

          function_operation_request = str_value->GetString();
          op_token = GetToken(function_operation_request);

      }  //  if (i == 0) ...

    }  //  for (i = 0; ...

}  //  Validate ()

