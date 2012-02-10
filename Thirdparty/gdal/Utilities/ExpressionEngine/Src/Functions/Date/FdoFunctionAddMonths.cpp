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

    is_validated        = false;

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

    // Declare and initialize all necessary local variables.

    bool                     is_addition             = true;

    FdoInt8                  curr_month,
                             num_of_months;

    FdoInt16                 curr_year,
                             num_of_years;

    FdoInt64                 number_of_months_to_add = 0;

    FdoDateTime              dt;

    FdoPtr<FdoDateTimeValue> dt_value;

    // If this is the first call to this function validate the arguments and
    // initialize some member variables used to process the request.

    if (!is_validated) {

        Validate(literal_values);
        result       = FdoDateTimeValue::Create();
        is_validated = true;

    }  //  if (!is_validated) ...

    // Get the date/time parameter to which the months are added. If the value
    // is not set nothing else needs to be done. Otherwise get the value.

    dt_value = (FdoDateTimeValue *) literal_values->GetItem(0);
    if (dt_value->IsNull()) {

        result->SetNull();
        return FDO_SAFE_ADDREF(result.p);

    }  //  if (!dt_value->IsNull()) ...

    dt = dt_value->GetDateTime();

    // Get the number of months that need to be added. If the request is to
    // add 0 months return the original date/time object.

    number_of_months_to_add = GetNumberOfMonthsToAdd(
                                        literal_values, numeric_data_type);

    if (number_of_months_to_add == 0) {

        result->SetDateTime(dt);
        return FDO_SAFE_ADDREF(result.p);
        
    }  //  if (number_of_months_to_add) ...

    // Determine the sign of the months to be added as this affects further
    // processing.

    is_addition = (number_of_months_to_add > 0);

    // Determine the number of years defined by the number of months to be
    // added and the remaining months.

    num_of_years  = (FdoInt16) (number_of_months_to_add / 12);
    num_of_months = (FdoInt8)  (number_of_months_to_add % 12);

    // Get the current year and month data. The values need to be adjusted if
    // the values are the initial settings.

    curr_year  = (dt.year  == -1) ? 0 : dt.year;
    curr_month = (dt.month == -1) ? 1 : dt.month;

    // The number of years must be adjusted if the addition of the remaining
    // number of months crosses the year. If this is the case then the re-
    // sulting month can be set outright.

    if (is_addition) {

        if ((curr_month + num_of_months) > 12) {

            num_of_years++;
            dt.month = (curr_month + num_of_months) - 12;

        }  //  if ((curr_month + num_of_months) > 12) ...
        else
          dt.month = (curr_month + num_of_months);

        dt.year = curr_year + num_of_years;

    }  //  if (is_addition) ...
    else {

      // NOTE: If this is to subtract months, then the values of the variables
      //       num_of_months and num_of_years are negative.

      if ((curr_month + num_of_months) < 0) {

          num_of_years--;
          dt.month = 12 + (curr_month + num_of_months);

      }  //  if ((curr_month + num_of_months) > 12) ...
      else
        dt.month = (curr_month + num_of_months);

        dt.year = curr_year + num_of_years;

    }  //  else ...

    // Set the value and return a reference to it.

    result->SetDateTime(dt);
    return FDO_SAFE_ADDREF(result.p);

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
// |    ADDMONTHS (date_time,
// |               {byte, decimal, double, int16, int32, int64, single})
// |
// | The function always returns a DATETIME.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoString                               *desc               = NULL;

    FdoStringP                              arg1_description;
    FdoStringP                              arg2_description;
    FdoStringP                              dt_arg_literal;
    FdoStringP                              num_arg_literal;

    FdoPtr<FdoArgumentDefinition>           byte_arg;
    FdoPtr<FdoArgumentDefinition>           dcl_arg;
    FdoPtr<FdoArgumentDefinition>           dbl_arg;
    FdoPtr<FdoArgumentDefinition>           int16_arg;
    FdoPtr<FdoArgumentDefinition>           int32_arg;
    FdoPtr<FdoArgumentDefinition>           int64_arg;
    FdoPtr<FdoArgumentDefinition>           sgl_arg;
    FdoPtr<FdoArgumentDefinition>           dt_arg;

    FdoPtr<FdoArgumentDefinitionCollection> dt_byte_args;
    FdoPtr<FdoArgumentDefinitionCollection> dt_dcl_args;
    FdoPtr<FdoArgumentDefinitionCollection> dt_dbl_args;
    FdoPtr<FdoArgumentDefinitionCollection> dt_int16_args;
    FdoPtr<FdoArgumentDefinitionCollection> dt_int32_args;
    FdoPtr<FdoArgumentDefinitionCollection> dt_int64_args;
    FdoPtr<FdoArgumentDefinitionCollection> dt_sgl_args;

    FdoPtr<FdoSignatureDefinition>          signature;
    FdoPtr<FdoSignatureDefinitionCollection> signatures;

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

    dt_arg    = FdoArgumentDefinition::Create(
                    dt_arg_literal, arg1_description, FdoDataType_DateTime);
    byte_arg  = FdoArgumentDefinition::Create(
                    num_arg_literal, arg2_description, FdoDataType_Byte);
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

    dt_byte_args = FdoArgumentDefinitionCollection::Create();
    dt_byte_args->Add(dt_arg);
    dt_byte_args->Add(byte_arg);

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
                                        FdoDataType_DateTime, dt_byte_args);
    signatures->Add(signature);

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

FdoInt64 FdoFunctionAddMonths::GetNumberOfMonthsToAdd (
                                FdoLiteralValueCollection *literal_values,
                                FdoDataType               data_type)

// +---------------------------------------------------------------------------
// | The function returns the number of months to be added to the provided
// | date/time object.
// +---------------------------------------------------------------------------

{

    // Get the number of months to be added to the provided data/time.
    // NOTE: Although some RDBMS providers work with fractional numbers as
    //       the number of months to be added, the default implementation
    //       does not. It only works with the value ahead of the decimal
    //       point (if present).
    
    switch (data_type) {

      case FdoDataType_Byte:
        {

            FdoByte              byte_val     = 0;
            FdoPtr<FdoByteValue> byte_value;

            byte_value = (FdoByteValue *) literal_values->GetItem(1);
            if (!byte_value->IsNull())
                byte_val = byte_value->GetByte();
            return (FdoInt64) (byte_val);

        }
        break;

      case FdoDataType_Decimal:
        {

            FdoDouble               dbl_val         = 0;
            FdoPtr<FdoDecimalValue> decimal_value;

            decimal_value = (FdoDecimalValue *) literal_values->GetItem(1);
            if (!decimal_value->IsNull()) {

                dbl_val = decimal_value->GetDecimal();
                if ((dbl_val < LLONG_MIN) || (dbl_val > LLONG_MAX))
                    throw FdoException::Create(
                        FdoException::NLSGetMessage(
                        FUNCTION_DATA_VALUE_ERROR, 
                        "Expression Engine: Invalid value for execution of function '%1$ls'",
                        FDO_FUNCTION_ADDMONTHS));

            }  //  if (!decimal_value->IsNull()) ...

            return (FdoInt64) dbl_val;

        }
        break;

      case FdoDataType_Double:
        {

            FdoDouble              dbl_val          = 0;
            FdoPtr<FdoDoubleValue> double_value;

            double_value = (FdoDoubleValue *) literal_values->GetItem(1);
            if (!double_value->IsNull()) {

                dbl_val = double_value->GetDouble();
                if ((dbl_val < LLONG_MIN) || (dbl_val > LLONG_MAX))
                    throw FdoException::Create(
                        FdoException::NLSGetMessage(
                        FUNCTION_DATA_VALUE_ERROR, 
                        "Expression Engine: Invalid value for execution of function '%1$ls'",
                        FDO_FUNCTION_ADDMONTHS));

            }  //  if (!double_value->IsNull()) ...

            return (FdoInt64) dbl_val;

        }
        break;

      case FdoDataType_Int16:
        {

            FdoInt16              i16_val    = 0;
            FdoPtr<FdoInt16Value> i16_value;

            i16_value = (FdoInt16Value *) literal_values->GetItem(1);
            if (!i16_value->IsNull())
                i16_val = i16_value->GetInt16();
            return (FdoInt64) (i16_val);

        }
        break;

      case FdoDataType_Int32:
        {

            FdoInt32              i32_val    = 0;
            FdoPtr<FdoInt32Value> i32_value;

            i32_value = (FdoInt32Value *) literal_values->GetItem(1);
            if (!i32_value->IsNull())
                i32_val = i32_value->GetInt32();
            return (FdoInt64) (i32_val);

        }
        break;

      case FdoDataType_Int64:
        {

            FdoInt64              i64_val    = 0;
            FdoPtr<FdoInt64Value> i64_value;

            i64_value = (FdoInt64Value *) literal_values->GetItem(1);
            if (!i64_value->IsNull())
                i64_val = i64_value->GetInt64();
            return (i64_val);

        }
        break;

      case FdoDataType_Single:
        {

            FdoFloat               sgl_val    = 0;
            FdoPtr<FdoSingleValue> sgl_value;

            sgl_value = (FdoSingleValue *) literal_values->GetItem(1);
            if (!sgl_value->IsNull())
                sgl_val = sgl_value->GetSingle();
            return (FdoInt64) (sgl_val);

        }
        break;

      default:
        throw FdoException::Create(
                  FdoException::NLSGetMessage(
                    FUNCTION_PARAMETER_DATA_TYPE_ERROR, 
                    "Expression Engine: Invalid parameter data type for function '%1$ls'",
                    FDO_FUNCTION_ADDMONTHS));
        break;

    }  //  switch ...

}  //  GetNumberOfMonthsToAdd ()

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
        invalid_data_type = ((data_type != FdoDataType_Byte   ) &&
                             (data_type != FdoDataType_Decimal) &&
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

