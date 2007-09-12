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
#include <Functions/Date/FdoFunctionAddMonths.h>


// ----------------------------------------------------------------------------
// --                         Constructors/Destructors                       --
// ----------------------------------------------------------------------------

FdoFunctionAddMonths::FdoFunctionAddMonths ()

// +---------------------------------------------------------------------------
// | The class constructor.
// +---------------------------------------------------------------------------

{

    // Initialize all class variables.
    // NOTE: Due to the fact that data type enumeration misses an entry to
    //       indicate a not-set value, the variable "numeric_data_type" is
    //       set to "FdoDataType_CLOB" to indicate an invalid data type be-
    //       cause this function does not support this type. 

    function_definition = NULL;

    numeric_data_type   = FdoDataType_CLOB;

}  //  FdoFunctionAddMonths ()


FdoFunctionAddMonths::~FdoFunctionAddMonths ()

// +---------------------------------------------------------------------------
// | The class destructor.
// +---------------------------------------------------------------------------

{

    // Delete the function definition.

    FDO_SAFE_RELEASE(function_definition);

}  //  ~FdoFunctionAddMonths ()


// ----------------------------------------------------------------------------
// --                            Public Class APIs                           --
// ----------------------------------------------------------------------------

FdoFunctionAddMonths *FdoFunctionAddMonths::Create ()

// +---------------------------------------------------------------------------
// | The function creates a new instance of the class.
// +---------------------------------------------------------------------------

{

    return new FdoFunctionAddMonths();

}  //  Create ()

FdoFunctionAddMonths *FdoFunctionAddMonths::CreateObject ()

// +---------------------------------------------------------------------------
// | The function creates a new instance of the class.
// +---------------------------------------------------------------------------

{

    return new FdoFunctionAddMonths();

}  //  CreateObject ()

FdoFunctionDefinition *FdoFunctionAddMonths::GetFunctionDefinition ()

// +---------------------------------------------------------------------------
// | The function creates the supported signature list for the function ADD-
// | MONTHS.
// +---------------------------------------------------------------------------

{

    if (function_definition == NULL)
        CreateFunctionDefinition();

    return FDO_SAFE_ADDREF(function_definition);

}  //  GetFunctionDefinition ()

FdoLiteralValue *FdoFunctionAddMonths::Evaluate (
                                    FdoLiteralValueCollection *literal_values)

// +---------------------------------------------------------------------------
// | The function processes a call to the function ADDMONTHS.
// +---------------------------------------------------------------------------

{

    // NOT YET IMPLEMENTED

    return FdoDateTimeValue::Create();

}  //  Evaluate ()


// ----------------------------------------------------------------------------
// --                          Supporting functions                          --
// ----------------------------------------------------------------------------

void FdoFunctionAddMonths::CreateFunctionDefinition ()

// +---------------------------------------------------------------------------
// | The procedure creates the function definition for the function ADDMONTHS.
// | The function definition includes the list of supported signatures. The
// | following signatures are supported:
// |
// |    ADDMONTHS (date_time, {decimal, double, int16, int32, int64, single})
// |
// | The function always returns a DATETIME.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoString *desc = NULL;

    FdoStringP arg1_description;
    FdoStringP arg2_description;
    FdoStringP dt_arg_literal;
    FdoStringP num_arg_literal;

    FdoPtr<FdoArgumentDefinition> dcl_arg;
    FdoPtr<FdoArgumentDefinition> dbl_arg;
    FdoPtr<FdoArgumentDefinition> int16_arg;
    FdoPtr<FdoArgumentDefinition> int32_arg;
    FdoPtr<FdoArgumentDefinition> int64_arg;
    FdoPtr<FdoArgumentDefinition> sgl_arg;
    FdoPtr<FdoArgumentDefinition> dt_arg;

    FdoPtr<FdoArgumentDefinitionCollection> dt_dcl_args;
    FdoPtr<FdoArgumentDefinitionCollection> dt_dbl_args;
    FdoPtr<FdoArgumentDefinitionCollection> dt_int16_args;
    FdoPtr<FdoArgumentDefinitionCollection> dt_int32_args;
    FdoPtr<FdoArgumentDefinitionCollection> dt_int64_args;
    FdoPtr<FdoArgumentDefinitionCollection> dt_sgl_args;

    FdoPtr<FdoSignatureDefinition>          signature;
    FdoSignatureDefinitionCollection        *signatures;

    // Get the general descriptions for the arguments.

    arg1_description = FdoException::NLSGetMessage(
                                                FUNCTION_GENERAL_ARG,
                                                "Argument to be processed");

    arg2_description = FdoException::NLSGetMessage(
                                            FUNCTION_ADDMONTHS_MONTH_ARG,
                                            "Number of months to be added");

    // The following defines the different argument definition collections.

    dt_arg_literal =
            FdoException::NLSGetMessage(FUNCTION_DATE_ARG_LIT, "date");

    num_arg_literal =
            FdoException::NLSGetMessage(FUNCTION_NUMBER_ARG_LIT, "number");

    dt_arg   = FdoArgumentDefinition::Create(
                    dt_arg_literal, arg1_description, FdoDataType_DateTime);

    dcl_arg   = FdoArgumentDefinition::Create(
                    num_arg_literal, arg2_description, FdoDataType_Decimal);
    dbl_arg   = FdoArgumentDefinition::Create(
                    num_arg_literal, arg2_description, FdoDataType_Double);
    int16_arg = FdoArgumentDefinition::Create(
                    num_arg_literal, arg2_description, FdoDataType_Int16);
    int32_arg = FdoArgumentDefinition::Create(
                    num_arg_literal, arg2_description, FdoDataType_Int32);
    int64_arg = FdoArgumentDefinition::Create(
                    num_arg_literal, arg2_description, FdoDataType_Int64);
    sgl_arg   = FdoArgumentDefinition::Create(
                    num_arg_literal, arg2_description, FdoDataType_Single);

    dt_dcl_args = FdoArgumentDefinitionCollection::Create();
    dt_dcl_args->Add(dt_arg);
    dt_dcl_args->Add(dcl_arg);

    dt_dbl_args = FdoArgumentDefinitionCollection::Create();
    dt_dbl_args->Add(dt_arg);
    dt_dbl_args->Add(dbl_arg);

    dt_int16_args = FdoArgumentDefinitionCollection::Create();
    dt_int16_args->Add(dt_arg);
    dt_int16_args->Add(int16_arg);

    dt_int32_args = FdoArgumentDefinitionCollection::Create();
    dt_int32_args->Add(dt_arg);
    dt_int32_args->Add(int32_arg);

    dt_int64_args = FdoArgumentDefinitionCollection::Create();
    dt_int64_args->Add(dt_arg);
    dt_int64_args->Add(int64_arg);

    dt_sgl_args = FdoArgumentDefinitionCollection::Create();
    dt_sgl_args->Add(dt_arg);
    dt_sgl_args->Add(sgl_arg);

    // Create the signature collection.

    signatures = FdoSignatureDefinitionCollection::Create();

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_DateTime, dt_dcl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_DateTime, dt_dbl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_DateTime, dt_int16_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_DateTime, dt_int32_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_DateTime, dt_int64_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_DateTime, dt_sgl_args);
    signatures->Add(signature);

    // Create the function definition.

    desc = FdoException::NLSGetMessage(
                            FUNCTION_ADDMONTHS,
                            "Adds a specified number of months to a date");
    function_definition =
                FdoFunctionDefinition::Create(
                                        FDO_FUNCTION_ADDMONTHS,
                                        desc,
                                        false,
                                        signatures,
                                        FdoFunctionCategoryType_Date);

}  //  CreateFunctionDefinition ()

void FdoFunctionAddMonths::Validate (FdoLiteralValueCollection *literal_values)

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

    FdoPtr<FdoLiteralValue> literal_value;

    // Check the number of arguments. ADDMONTHS accepts two parameters. If the
    // number of parameters is not correct issue an exception.

    if (count != 2) 
        throw FdoException::Create(
                FdoException::NLSGetMessage(
                  FUNCTION_PARAMETER_NUMBER_ERROR, 
                  "Expression Engine: Invalid number of parameters for function '%1$ls'",
                  FDO_FUNCTION_ADDMONTHS));

    // Navigate through the list of arguments and ensure that the data types
    // match the ones outlined in the list of supported signatures. If this
    // is not the case issue an exception.

    for (i = 0; i < count; i++) {

      literal_value = literal_values->GetItem(i);
      if (literal_value->GetLiteralValueType() != FdoLiteralValueType_Data)
          throw FdoException::Create(
                  FdoException::NLSGetMessage(
                    FUNCTION_PARAMETER_ERROR, 
                    "Expression Engine: Invalid parameters for function '%1$ls'",
                    FDO_FUNCTION_ADDMONTHS));

      data_value = static_cast<FdoDataValue *>(literal_value.p);
      data_type  = data_value->GetDataType();

      if (i == 0)
          invalid_data_type = (data_type != FdoDataType_DateTime);
      else {

        numeric_data_type = data_type;
        invalid_data_type = ((data_type != FdoDataType_Decimal) &&
                             (data_type != FdoDataType_Double ) &&
                             (data_type != FdoDataType_Int16  ) &&
                             (data_type != FdoDataType_Int32  ) &&
                             (data_type != FdoDataType_Int64  ) &&
                             (data_type != FdoDataType_Single )    );

      }  //  else ...

      if (invalid_data_type)
          throw FdoException::Create(
                  FdoException::NLSGetMessage(
                    FUNCTION_PARAMETER_DATA_TYPE_ERROR, 
                    "Expression Engine: Invalid parameter data type for function '%1$ls'",
                    FDO_FUNCTION_ADDMONTHS));

    }  //  for (i = 0; i < count; i++) ...

}  //  Validate ()

