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
#include <Functions/Conversion/FdoFunctionToString.h>


// ----------------------------------------------------------------------------
// --                         Constructors/Destructors                       --
// ----------------------------------------------------------------------------

FdoFunctionToString::FdoFunctionToString ()

// +---------------------------------------------------------------------------
// | The class constructor.
// +---------------------------------------------------------------------------

{

    // Initialize all class variables.

    function_definition = NULL;

}  //  FdoFunctionToString ()


FdoFunctionToString::~FdoFunctionToString ()

// +---------------------------------------------------------------------------
// | The class destructor.
// +---------------------------------------------------------------------------

{

    // Delete the function definition.

    FDO_SAFE_RELEASE(function_definition);

}  //  ~FdoFunctionToString ()


// ----------------------------------------------------------------------------
// --                            Public Class APIs                           --
// ----------------------------------------------------------------------------

FdoFunctionToString *FdoFunctionToString::Create ()

// +---------------------------------------------------------------------------
// | The function creates a new instance of the class.
// +---------------------------------------------------------------------------

{

    return new FdoFunctionToString();

}  //  Create ()

FdoFunctionToString *FdoFunctionToString::CreateObject ()

// +---------------------------------------------------------------------------
// | The function creates a new instance of the class.
// +---------------------------------------------------------------------------

{

    return new FdoFunctionToString();

}  //  CreateObject ()

FdoFunctionDefinition *FdoFunctionToString::GetFunctionDefinition ()

// +---------------------------------------------------------------------------
// | The function creates the supported signature list for the function
// | TOSTRING.
// +---------------------------------------------------------------------------

{

    if (function_definition == NULL)
        CreateFunctionDefinition();

    return FDO_SAFE_ADDREF(function_definition);

}  //  GetFunctionDefinition ()

FdoLiteralValue *FdoFunctionToString::Evaluate (
                                    FdoLiteralValueCollection *literal_values)

// +---------------------------------------------------------------------------
// | The function processes a call to the function TOSTRING.
// +---------------------------------------------------------------------------

{

    // NOT YET IMPLEMEMTED.

    return FdoStringValue::Create();

}  //  Evaluate ()


// ----------------------------------------------------------------------------
// --                          Supporting functions                          --
// ----------------------------------------------------------------------------

void FdoFunctionToString::CreateFunctionDefinition ()

// +---------------------------------------------------------------------------
// | The procedure creates the function definition for the function TOSTRING.
// | The function definition includes the list of supported signatures. The
// | following signatures are supported:
// |
// |    TOSTRING (date [, string])
// |    TOSTRING ({decimal, double, int16, int32, int64, single})
// |
// | The function always returns a STRING.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoString                               *desc               = NULL;

    FdoStringP                              arg1_description;
    FdoStringP                              arg2_description;
    FdoStringP                              dt_arg_literal;
    FdoStringP                              form_arg_literal;
    FdoStringP                              num_arg_literal;

    FdoPtr<FdoArgumentDefinition>           dt_arg;
    FdoPtr<FdoArgumentDefinition>           dcl_arg;
    FdoPtr<FdoArgumentDefinition>           dbl_arg;
    FdoPtr<FdoArgumentDefinition>           int16_arg;
    FdoPtr<FdoArgumentDefinition>           int32_arg;
    FdoPtr<FdoArgumentDefinition>           int64_arg;
    FdoPtr<FdoArgumentDefinition>           sgl_arg;
    FdoPtr<FdoArgumentDefinition>           form_arg;

    FdoPtr<FdoArgumentDefinitionCollection> dt_args;
    FdoPtr<FdoArgumentDefinitionCollection> dt_form_args;

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

    arg2_description = FdoException::NLSGetMessage(
                                                FUNCTION_GENERAL_FORMAT_ARG,
                                                "Format specification");

    dt_arg_literal = FdoException::NLSGetMessage(FUNCTION_DATE_ARG_LIT,
                                                 "date/time");

    form_arg_literal = FdoException::NLSGetMessage(FUNCTION_FORMAT_ARG_LIT,
                                                   "optional format");

    num_arg_literal  = FdoException::NLSGetMessage(FUNCTION_NUMBER_ARG_LIT,
                                                   "number");

    // The following defines the different argument definition collections.

    dt_arg    = FdoArgumentDefinition::Create(
                    dt_arg_literal, arg1_description, FdoDataType_DateTime);

    form_arg  = FdoArgumentDefinition::Create(
                    form_arg_literal, arg2_description, FdoDataType_String);

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

    dt_args = FdoArgumentDefinitionCollection::Create();
    dt_args->Add(dt_arg);

    dt_form_args = FdoArgumentDefinitionCollection::Create();
    dt_form_args->Add(dt_arg);
    dt_form_args->Add(form_arg);

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

    signature = FdoSignatureDefinition::Create(FdoDataType_String, dt_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                            FdoDataType_String, dt_form_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(FdoDataType_String, dcl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(FdoDataType_String, dbl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(FdoDataType_String, int16_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(FdoDataType_String, int32_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(FdoDataType_String, int64_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(FdoDataType_String, sgl_args);
    signatures->Add(signature);

    // Create the function definition.

    desc =
        FdoException::NLSGetMessage(FUNCTION_TOSTRING,
                                    "Converts a number or date to a string");
    function_definition =
                FdoFunctionDefinition::Create(
                                        FDO_FUNCTION_TOSTRING,
                                        desc,
                                        false,
                                        signatures,
                                        FdoFunctionCategoryType_Conversion);

}  //  CreateFunctionDefinition ()

void FdoFunctionToString::Validate (FdoLiteralValueCollection *literal_values)

// +---------------------------------------------------------------------------
// | The function validates the argument list that was passed in.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoInt32                i,
                            count           = literal_values->GetCount();

    FdoDataType             p1_data_type    = FdoDataType_CLOB,
                            p2_data_type    = FdoDataType_CLOB;

    FdoDataValue            *data_value     = NULL;

    FdoPtr<FdoLiteralValue> literal_value;

    // Check the number of arguments. TOSTRING accepts one or two parameters.
    // If the number of parameters is not correct issue an exception.

    if ((count < 1) || (count > 2)) 
        throw FdoException::Create(
                FdoException::NLSGetMessage(
                  FUNCTION_PARAMETER_NUMBER_ERROR, 
                  "Expression Engine: Invalid number of parameters for function '%1$ls'",
                  FDO_FUNCTION_TOSTRING));

    // Next, check whether or not the parameters have a valid data type. If
    // this is not the case issue an exception.

    for (i = 0; i < count; i++) {

      literal_value = literal_values->GetItem(i);
      if (literal_value->GetLiteralValueType() != FdoLiteralValueType_Data)
          throw FdoException::Create(
                  FdoException::NLSGetMessage(
                    FUNCTION_PARAMETER_ERROR, 
                    "Expression Engine: Invalid parameters for function '%1$ls'",
                    FDO_FUNCTION_TOSTRING));

      data_value = static_cast<FdoDataValue *>(literal_value.p);
      if (i == 0)
          p1_data_type  = data_value->GetDataType();
      else
        p2_data_type  = data_value->GetDataType();

    }  //  for (i = 0; i < count; i++) ...

    if ((p1_data_type != FdoDataType_Decimal ) &&
        (p1_data_type != FdoDataType_Double  ) &&
        (p1_data_type != FdoDataType_Int16   ) &&
        (p1_data_type != FdoDataType_Int32   ) &&
        (p1_data_type != FdoDataType_Int64   ) &&
        (p1_data_type != FdoDataType_Single  ) &&
        (p1_data_type != FdoDataType_DateTime)    )
        throw FdoException::Create(
                FdoException::NLSGetMessage(
                  FUNCTION_PARAMETER_DATA_TYPE_ERROR, 
                  "Expression Engine: Invalid parameter data type for function '%1$ls'",
                  FDO_FUNCTION_TOSTRING));

    if (count == 2)
        if (p2_data_type != FdoDataType_String)
            throw FdoException::Create(
                    FdoException::NLSGetMessage(
                    FUNCTION_PARAMETER_DATA_TYPE_ERROR, 
                    "Expression Engine: Invalid parameter data type for function '%1$ls'",
                    FDO_FUNCTION_TOSTRING));

}  //  Validate ()

