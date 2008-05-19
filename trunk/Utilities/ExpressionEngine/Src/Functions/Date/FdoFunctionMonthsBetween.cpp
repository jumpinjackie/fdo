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
#include <Functions/Date/FdoFunctionMonthsBetween.h>


// ----------------------------------------------------------------------------
// --                         Constructors/Destructors                       --
// ----------------------------------------------------------------------------

FdoFunctionMonthsBetween::FdoFunctionMonthsBetween ()

// +---------------------------------------------------------------------------
// | The class constructor.
// +---------------------------------------------------------------------------

{

    // Initialize all class variables.

    function_definition = NULL;

    is_validated        = false;

}  //  FdoFunctionMonthsBetween ()


FdoFunctionMonthsBetween::~FdoFunctionMonthsBetween ()

// +---------------------------------------------------------------------------
// | The class destructor.
// +---------------------------------------------------------------------------

{

    // Delete the function definition.

    FDO_SAFE_RELEASE(function_definition);

}  //  ~FdoFunctionMonthsBetween ()


// ----------------------------------------------------------------------------
// --                            Public Class APIs                           --
// ----------------------------------------------------------------------------

FdoFunctionMonthsBetween *FdoFunctionMonthsBetween::Create ()

// +---------------------------------------------------------------------------
// | The function creates a new instance of the class.
// +---------------------------------------------------------------------------

{

    return new FdoFunctionMonthsBetween();

}  //  Create ()

FdoFunctionMonthsBetween *FdoFunctionMonthsBetween::CreateObject ()

// +---------------------------------------------------------------------------
// | The function creates a new instance of the class.
// +---------------------------------------------------------------------------

{

    return new FdoFunctionMonthsBetween();

}  //  CreateObject ()

FdoFunctionDefinition *FdoFunctionMonthsBetween::GetFunctionDefinition ()

// +---------------------------------------------------------------------------
// | The function creates the supported signature list for the function ADD-
// | MONTHS.
// +---------------------------------------------------------------------------

{

    if (function_definition == NULL)
        CreateFunctionDefinition();

    return FDO_SAFE_ADDREF(function_definition);

}  //  GetFunctionDefinition ()

FdoLiteralValue *FdoFunctionMonthsBetween::Evaluate (
                                    FdoLiteralValueCollection *literal_values)

// +---------------------------------------------------------------------------
// | The function processes a call to the function MONTHSBETWEEN.
// | NOTE: Unlike native functions in different RDBMS systems, this implemen-
// |       tation of the function will soley use the year and month informa-
// |       tion that is provided. Day data is not considered.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoDouble                months_between     = 0;

    FdoDateTime              dt_end,
                             dt_start;

    FdoPtr<FdoDateTimeValue> dt_value_end,
                             dt_value_start;

    // If this is the first call to this function validate the arguments and
    // initialize some member variables used to process the request.

    if (!is_validated) {

        Validate(literal_values);
        result       = FdoDoubleValue::Create();
        is_validated = true;

    }  //  if (!is_validated) ...

    // Get the date information provided to the function. If either of the
    // provided date information is NULL. nothing further needs to be done.

    dt_value_start = (FdoDateTimeValue *) literal_values->GetItem(0);
    dt_value_end   = (FdoDateTimeValue *) literal_values->GetItem(1);

    if ((dt_value_start->IsNull()) || (dt_value_end->IsNull())) {

        result->SetNull();
        return FDO_SAFE_ADDREF(result.p);

    }  //  if (!dt_value->IsNull()) ...

    dt_start = dt_value_start->GetDateTime();
    dt_end   = dt_value_end->GetDateTime();

    // Check the year and month information for either of the provided date
    // information. If either of them is not set (indicated by the value being
    // -1), then there is nothing to do and the function can be terminated.

    if ((dt_start.year == -1) || (dt_start.month == -1) ||
        (dt_end.year   == -1) || (dt_end.month   == -1)    ) {

        result->SetNull();
        return FDO_SAFE_ADDREF(result.p);

    }  //  if ((dt_start.year == -1) || ...

    // Determine the number of months between the two given dates.

    months_between =
        (dt_end.year - dt_start.year) * 12 + dt_end.month - dt_start.month;


    // Set the value and return a reference to it.

    result->SetDouble(months_between);
    return FDO_SAFE_ADDREF(result.p);

}  //  Evaluate ()


// ----------------------------------------------------------------------------
// --                          Supporting functions                          --
// ----------------------------------------------------------------------------

void FdoFunctionMonthsBetween::CreateFunctionDefinition ()

// +---------------------------------------------------------------------------
// | The procedure creates the function definition for the function MONTHSBE-
// | TWEEN. The function definition includes the list of supported signatures.
// | The following signatures are supported:
// |
// |    MONTHSBETWEEN (date_time, date_time)
// |
// | The function always returns a DOUBLE.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoString                               *desc               = NULL;

    FdoStringP                              arg1_description;
    FdoStringP                              dt1_arg_literal;
    FdoStringP                              dt2_arg_literal;

    FdoPtr<FdoArgumentDefinition>           dt1_arg;
    FdoPtr<FdoArgumentDefinition>           dt2_arg;

    FdoPtr<FdoArgumentDefinitionCollection> args;

    FdoPtr<FdoSignatureDefinition>          signature;
    FdoPtr<FdoSignatureDefinitionCollection> signatures;

    // Get the general descriptions for the arguments.

    arg1_description = FdoException::NLSGetMessage(
                                                FUNCTION_GENERAL_ARG,
                                                "Argument to be processed");

    // The following defines the different argument definition collections.

    dt1_arg_literal =
            FdoException::NLSGetMessage(FUNCTION_DATE1_ARG_LIT, "date1");

    dt2_arg_literal =
            FdoException::NLSGetMessage(FUNCTION_DATE2_ARG_LIT, "date2");

    dt1_arg   = FdoArgumentDefinition::Create(
                    dt1_arg_literal, arg1_description, FdoDataType_DateTime);

    dt2_arg   = FdoArgumentDefinition::Create(
                    dt2_arg_literal, arg1_description, FdoDataType_DateTime);

    args = FdoArgumentDefinitionCollection::Create();
    args->Add(dt1_arg);
    args->Add(dt2_arg);

    // Create the signature collection.

    signatures = FdoSignatureDefinitionCollection::Create();

    signature = FdoSignatureDefinition::Create(FdoDataType_Double, args);
    signatures->Add(signature);

    // Create the function definition.

    desc = FdoException::NLSGetMessage(
                            FUNCTION_MONTHSBETWEEN,
                            "Returns the number of months between two dates");
    function_definition =
                FdoFunctionDefinition::Create(
                                        FDO_FUNCTION_MONTHSBETWEEN,
                                        desc,
                                        false,
                                        signatures,
                                        FdoFunctionCategoryType_Date);

}  //  CreateFunctionDefinition ()

void FdoFunctionMonthsBetween::Validate (FdoLiteralValueCollection *literal_values)

// +---------------------------------------------------------------------------
// | The function validates the argument list that was passed in.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoInt32                i,
                            count               = literal_values->GetCount();

    FdoDataValue            *data_value         = NULL;

    FdoPtr<FdoLiteralValue> literal_value;

    // Check the number of arguments. MONTHSBETWEEN accepts two parameters. If the
    // number of parameters is not correct issue an exception.

    if (count != 2) 
        throw FdoException::Create(
                FdoException::NLSGetMessage(
                  FUNCTION_PARAMETER_NUMBER_ERROR, 
                  "Expression Engine: Invalid number of parameters for function '%1$ls'",
                  FDO_FUNCTION_MONTHSBETWEEN));

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
                    FDO_FUNCTION_MONTHSBETWEEN));

      data_value = static_cast<FdoDataValue *>(literal_value.p);

      if (data_value->GetDataType() != FdoDataType_DateTime)
          throw FdoException::Create(
                  FdoException::NLSGetMessage(
                    FUNCTION_PARAMETER_DATA_TYPE_ERROR, 
                    "Expression Engine: Invalid parameter data type for function '%1$ls'",
                    FDO_FUNCTION_MONTHSBETWEEN));

    }  //  for (i = 0; i < count; i++) ...

}  //  Validate ()

