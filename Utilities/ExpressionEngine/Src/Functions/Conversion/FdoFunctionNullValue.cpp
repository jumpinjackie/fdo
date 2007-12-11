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
#include <Functions/Conversion/FdoFunctionNullValue.h>

#include <FdoCommonOSUtil.h>

// ----------------------------------------------------------------------------
// --                         Constructors/Destructors                       --
// ----------------------------------------------------------------------------

FdoFunctionNullValue::FdoFunctionNullValue ()

// +---------------------------------------------------------------------------
// | The class constructor.
// +---------------------------------------------------------------------------

{

    // Initialize all class variables.
    // NOTE: Due to the fact that data type enumeration misses an entry to
    //       indicate a not-set value, the variables "para1_data_type" and
    //       "para2_data_type" are set to "FdoDataType_CLOB" to indicate an
    //       invalid data type because this function does not support this
    //       type. 

    function_definition = NULL;

    para1_data_type     = FdoDataType_CLOB;
    para2_data_type     = FdoDataType_CLOB;

    first = true;

}  //  FdoFunctionNullValue ()

FdoFunctionNullValue::~FdoFunctionNullValue ()

// +---------------------------------------------------------------------------
// | The class destructor.
// +---------------------------------------------------------------------------

{

    // Delete the function definition.

    FDO_SAFE_RELEASE(function_definition);

}  //  ~FdoFunctionNullValue ()


// ----------------------------------------------------------------------------
// --                            Public Class APIs                           --
// ----------------------------------------------------------------------------

FdoFunctionNullValue *FdoFunctionNullValue::Create ()

// +---------------------------------------------------------------------------
// | The function creates a new instance of the class.
// +---------------------------------------------------------------------------

{

    return new FdoFunctionNullValue();

}  //  Create ()

FdoFunctionNullValue *FdoFunctionNullValue::CreateObject ()

// +---------------------------------------------------------------------------
// | The function creates a new instance of the class.
// +---------------------------------------------------------------------------

{

    return new FdoFunctionNullValue();

}  //  CreateObject ()

FdoFunctionDefinition *FdoFunctionNullValue::GetFunctionDefinition ()

// +---------------------------------------------------------------------------
// | The function creates the supported signature list for the function NULL-
// | VALUE.
// +---------------------------------------------------------------------------

{

    if (function_definition == NULL)
        CreateFunctionDefinition();

    return FDO_SAFE_ADDREF(function_definition);

}  //  GetFunctionDefinition ()

FdoLiteralValue *FdoFunctionNullValue::Evaluate (
                                    FdoLiteralValueCollection *literal_values)

// +---------------------------------------------------------------------------
// | The function processes a call to the function NULLVALUE.
// +---------------------------------------------------------------------------

{

    if (first)
    {

        Validate(literal_values);
        // variable 'first' will be clear after the return object is created
    }

    // The processing of this request depends on the data type of the first
    // parameter.

    switch (para1_data_type) {

      case FdoDataType_Boolean:
        return ProcessBooleanRequest(literal_values);
        break;

      case FdoDataType_Byte:
        return ProcessByteRequest(literal_values);
        break;

      case FdoDataType_DateTime:
        return ProcessDateTimeRequest(literal_values);
        break;

      case FdoDataType_Decimal:
        return ProcessDecimalRequest(literal_values);
        break;

      case FdoDataType_Double:
        return ProcessDoubleRequest(literal_values);
        break;

      case FdoDataType_Int16:
        return ProcessInt16Request(literal_values);
        break;

      case FdoDataType_Int32:
        return ProcessInt32Request(literal_values);
        break;

      case FdoDataType_Int64:
        return ProcessInt64Request(literal_values);
        break;

      case FdoDataType_Single:
        return ProcessSingleRequest(literal_values);
        break;

      case FdoDataType_String:
        return ProcessStringRequest(literal_values);
        break;

    }  //  switch ...

    // The switch statement should have processed all valid cases and hence the
    // following code should never be reached. But just in case issue an excep-
    // tion if it does.
    
    throw FdoException::Create(
            FdoException::NLSGetMessage(
              FUNCTION_PARAMETER_DATA_TYPE_ERROR, 
              "Expression Engine: Invalid parameter data type for function '%1$ls'",
              FDO_FUNCTION_NULLVALUE));

}  //  Evaluate ()


// ----------------------------------------------------------------------------
// --                          Supporting functions                          --
// ----------------------------------------------------------------------------

void FdoFunctionNullValue::CreateFunctionDefinition ()

// +---------------------------------------------------------------------------
// | The procedure creates the function definition for the function NULLVALUE.
// | The function definition includes the list of supported signatures. The
// | following signatures are supported:
// |
// |    NULLVALUE (bool,      bool)
// |    NULLVALUE (byte,      byte}
// |    NULLVALUE (date_time, date_time}
// |    NULLVALUE (decimal,   {decimal, double, int16, int32, int64, single}}
// |    NULLVALUE (double,    {decimal, double, int16, int32, int64, single}}
// |    NULLVALUE (int16,     {decimal, double, int16, int32, int64, single}}
// |    NULLVALUE (int32,     {decimal, double, int16, int32, int64, single}}
// |    NULLVALUE (int64,     {decimal, double, int16, int32, int64, single}}
// |    NULLVALUE (single,    {decimal, double, int16, int32, int64, single}}
// |    NULLVALUE (string,    {decimal, double, int16, int32, int64, single,
// |                                                                 string}}
// |
// | The function returns a data type depending on the the provided data types.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoString                               *desc               = NULL;

    FdoStringP                              arg1_description;
    FdoStringP                              bool_arg_literal;
    FdoStringP                              byte_arg_literal;
    FdoStringP                              dt_arg_literal;
    FdoStringP                              num_arg_literal;
    FdoStringP                              str_arg_literal;

    FdoPtr<FdoArgumentDefinition>           bool_arg;
    FdoPtr<FdoArgumentDefinition>           byte_arg;
    FdoPtr<FdoArgumentDefinition>           dt_arg;
    FdoPtr<FdoArgumentDefinition>           dcl_arg;
    FdoPtr<FdoArgumentDefinition>           dbl_arg;
    FdoPtr<FdoArgumentDefinition>           int16_arg;
    FdoPtr<FdoArgumentDefinition>           int32_arg;
    FdoPtr<FdoArgumentDefinition>           int64_arg;
    FdoPtr<FdoArgumentDefinition>           sgl_arg;
    FdoPtr<FdoArgumentDefinition>           str_arg;

    FdoPtr<FdoArgumentDefinitionCollection> bool_bool_args;
    FdoPtr<FdoArgumentDefinitionCollection> byte_byte_args;
    FdoPtr<FdoArgumentDefinitionCollection> dt_dt_args;

    FdoPtr<FdoArgumentDefinitionCollection> dcl_dcl_args;
    FdoPtr<FdoArgumentDefinitionCollection> dcl_dbl_args;
    FdoPtr<FdoArgumentDefinitionCollection> dcl_int16_args;
    FdoPtr<FdoArgumentDefinitionCollection> dcl_int32_args;
    FdoPtr<FdoArgumentDefinitionCollection> dcl_int64_args;
    FdoPtr<FdoArgumentDefinitionCollection> dcl_sgl_args;

    FdoPtr<FdoArgumentDefinitionCollection> dbl_dcl_args;
    FdoPtr<FdoArgumentDefinitionCollection> dbl_dbl_args;
    FdoPtr<FdoArgumentDefinitionCollection> dbl_int16_args;
    FdoPtr<FdoArgumentDefinitionCollection> dbl_int32_args;
    FdoPtr<FdoArgumentDefinitionCollection> dbl_int64_args;
    FdoPtr<FdoArgumentDefinitionCollection> dbl_sgl_args;

    FdoPtr<FdoArgumentDefinitionCollection> int16_dcl_args;
    FdoPtr<FdoArgumentDefinitionCollection> int16_dbl_args;
    FdoPtr<FdoArgumentDefinitionCollection> int16_int16_args;
    FdoPtr<FdoArgumentDefinitionCollection> int16_int32_args;
    FdoPtr<FdoArgumentDefinitionCollection> int16_int64_args;
    FdoPtr<FdoArgumentDefinitionCollection> int16_sgl_args;

    FdoPtr<FdoArgumentDefinitionCollection> int32_dcl_args;
    FdoPtr<FdoArgumentDefinitionCollection> int32_dbl_args;
    FdoPtr<FdoArgumentDefinitionCollection> int32_int16_args;
    FdoPtr<FdoArgumentDefinitionCollection> int32_int32_args;
    FdoPtr<FdoArgumentDefinitionCollection> int32_int64_args;
    FdoPtr<FdoArgumentDefinitionCollection> int32_sgl_args;

    FdoPtr<FdoArgumentDefinitionCollection> int64_dcl_args;
    FdoPtr<FdoArgumentDefinitionCollection> int64_dbl_args;
    FdoPtr<FdoArgumentDefinitionCollection> int64_int16_args;
    FdoPtr<FdoArgumentDefinitionCollection> int64_int32_args;
    FdoPtr<FdoArgumentDefinitionCollection> int64_int64_args;
    FdoPtr<FdoArgumentDefinitionCollection> int64_sgl_args;

    FdoPtr<FdoArgumentDefinitionCollection> sgl_dcl_args;
    FdoPtr<FdoArgumentDefinitionCollection> sgl_dbl_args;
    FdoPtr<FdoArgumentDefinitionCollection> sgl_int16_args;
    FdoPtr<FdoArgumentDefinitionCollection> sgl_int32_args;
    FdoPtr<FdoArgumentDefinitionCollection> sgl_int64_args;
    FdoPtr<FdoArgumentDefinitionCollection> sgl_sgl_args;

    FdoPtr<FdoArgumentDefinitionCollection> str_dcl_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_dbl_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_int16_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_int32_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_int64_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_sgl_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_str_args;

    FdoPtr<FdoSignatureDefinition>          signature;
    FdoSignatureDefinitionCollection        *signatures;

    // Get the general descriptions for the arguments.

    arg1_description = FdoException::NLSGetMessage(
                                        FUNCTION_GENERAL_ARG,
                                        "Argument to be processed");

    bool_arg_literal =
            FdoException::NLSGetMessage(FUNCTION_BOOL_ARG_LIT, "boolean");

    byte_arg_literal =
            FdoException::NLSGetMessage(FUNCTION_BYTE_ARG_LIT, "byte");

    dt_arg_literal =
            FdoException::NLSGetMessage(FUNCTION_DATE_ARG_LIT, "date/time");

    num_arg_literal =
            FdoException::NLSGetMessage(FUNCTION_NUMBER_ARG_LIT, "number");

    str_arg_literal =
            FdoException::NLSGetMessage(
                                    FUNCTION_STRING_ARG_LIT, "text property");

    // The following defines the different argument definition collections.

    bool_arg  = FdoArgumentDefinition::Create(
                    bool_arg_literal, arg1_description, FdoDataType_Boolean);
    byte_arg  = FdoArgumentDefinition::Create(
                    byte_arg_literal, arg1_description, FdoDataType_Byte);
    dt_arg    = FdoArgumentDefinition::Create(
                    dt_arg_literal, arg1_description, FdoDataType_DateTime);
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
    str_arg   = FdoArgumentDefinition::Create(
                    str_arg_literal, arg1_description, FdoDataType_String);

    bool_bool_args = FdoArgumentDefinitionCollection::Create();
    bool_bool_args->Add(bool_arg);
    bool_bool_args->Add(bool_arg);

    byte_byte_args = FdoArgumentDefinitionCollection::Create();
    byte_byte_args->Add(byte_arg);
    byte_byte_args->Add(byte_arg);

    dt_dt_args = FdoArgumentDefinitionCollection::Create();
    dt_dt_args->Add(dt_arg);
    dt_dt_args->Add(dt_arg);

    dcl_dcl_args = FdoArgumentDefinitionCollection::Create();
    dcl_dcl_args->Add(dcl_arg);
    dcl_dcl_args->Add(dcl_arg);

    dcl_dbl_args = FdoArgumentDefinitionCollection::Create();
    dcl_dbl_args->Add(dcl_arg);
    dcl_dbl_args->Add(dbl_arg);

    dcl_int16_args = FdoArgumentDefinitionCollection::Create();
    dcl_int16_args->Add(dcl_arg);
    dcl_int16_args->Add(int16_arg);

    dcl_int32_args = FdoArgumentDefinitionCollection::Create();
    dcl_int32_args->Add(dcl_arg);
    dcl_int32_args->Add(int32_arg);

    dcl_int64_args = FdoArgumentDefinitionCollection::Create();
    dcl_int64_args->Add(dcl_arg);
    dcl_int64_args->Add(int64_arg);

    dcl_sgl_args = FdoArgumentDefinitionCollection::Create();
    dcl_sgl_args->Add(dcl_arg);
    dcl_sgl_args->Add(sgl_arg);

    dbl_dcl_args = FdoArgumentDefinitionCollection::Create();
    dbl_dcl_args->Add(dbl_arg);
    dbl_dcl_args->Add(dcl_arg);

    dbl_dbl_args = FdoArgumentDefinitionCollection::Create();
    dbl_dbl_args->Add(dbl_arg);
    dbl_dbl_args->Add(dbl_arg);

    dbl_int16_args = FdoArgumentDefinitionCollection::Create();
    dbl_int16_args->Add(dbl_arg);
    dbl_int16_args->Add(int16_arg);

    dbl_int32_args = FdoArgumentDefinitionCollection::Create();
    dbl_int32_args->Add(dbl_arg);
    dbl_int32_args->Add(int32_arg);

    dbl_int64_args = FdoArgumentDefinitionCollection::Create();
    dbl_int64_args->Add(dbl_arg);
    dbl_int64_args->Add(int64_arg);

    dbl_sgl_args = FdoArgumentDefinitionCollection::Create();
    dbl_sgl_args->Add(dbl_arg);
    dbl_sgl_args->Add(sgl_arg);

    int16_dcl_args = FdoArgumentDefinitionCollection::Create();
    int16_dcl_args->Add(int16_arg);
    int16_dcl_args->Add(dcl_arg);

    int16_dbl_args = FdoArgumentDefinitionCollection::Create();
    int16_dbl_args->Add(int16_arg);
    int16_dbl_args->Add(dbl_arg);

    int16_int16_args = FdoArgumentDefinitionCollection::Create();
    int16_int16_args->Add(int16_arg);
    int16_int16_args->Add(int16_arg);

    int16_int32_args = FdoArgumentDefinitionCollection::Create();
    int16_int32_args->Add(int16_arg);
    int16_int32_args->Add(int32_arg);

    int16_int64_args = FdoArgumentDefinitionCollection::Create();
    int16_int64_args->Add(int16_arg);
    int16_int64_args->Add(int64_arg);

    int16_sgl_args = FdoArgumentDefinitionCollection::Create();
    int16_sgl_args->Add(int16_arg);
    int16_sgl_args->Add(sgl_arg);

    int32_dcl_args = FdoArgumentDefinitionCollection::Create();
    int32_dcl_args->Add(int32_arg);
    int32_dcl_args->Add(dcl_arg);

    int32_dbl_args = FdoArgumentDefinitionCollection::Create();
    int32_dbl_args->Add(int32_arg);
    int32_dbl_args->Add(dbl_arg);

    int32_int16_args = FdoArgumentDefinitionCollection::Create();
    int32_int16_args->Add(int32_arg);
    int32_int16_args->Add(int16_arg);

    int32_int32_args = FdoArgumentDefinitionCollection::Create();
    int32_int32_args->Add(int32_arg);
    int32_int32_args->Add(int32_arg);

    int32_int64_args = FdoArgumentDefinitionCollection::Create();
    int32_int64_args->Add(int32_arg);
    int32_int64_args->Add(int64_arg);

    int32_sgl_args = FdoArgumentDefinitionCollection::Create();
    int32_sgl_args->Add(int32_arg);
    int32_sgl_args->Add(sgl_arg);

    int64_dcl_args = FdoArgumentDefinitionCollection::Create();
    int64_dcl_args->Add(int64_arg);
    int64_dcl_args->Add(dcl_arg);

    int64_dbl_args = FdoArgumentDefinitionCollection::Create();
    int64_dbl_args->Add(int64_arg);
    int64_dbl_args->Add(dbl_arg);

    int64_int16_args = FdoArgumentDefinitionCollection::Create();
    int64_int16_args->Add(int64_arg);
    int64_int16_args->Add(int16_arg);

    int64_int32_args = FdoArgumentDefinitionCollection::Create();
    int64_int32_args->Add(int64_arg);
    int64_int32_args->Add(int32_arg);

    int64_int64_args = FdoArgumentDefinitionCollection::Create();
    int64_int64_args->Add(int64_arg);
    int64_int64_args->Add(int64_arg);

    int64_sgl_args = FdoArgumentDefinitionCollection::Create();
    int64_sgl_args->Add(int64_arg);
    int64_sgl_args->Add(sgl_arg);

    sgl_dcl_args = FdoArgumentDefinitionCollection::Create();
    sgl_dcl_args->Add(sgl_arg);
    sgl_dcl_args->Add(dcl_arg);

    sgl_dbl_args = FdoArgumentDefinitionCollection::Create();
    sgl_dbl_args->Add(sgl_arg);
    sgl_dbl_args->Add(dbl_arg);

    sgl_int16_args = FdoArgumentDefinitionCollection::Create();
    sgl_int16_args->Add(sgl_arg);
    sgl_int16_args->Add(int16_arg);

    sgl_int32_args = FdoArgumentDefinitionCollection::Create();
    sgl_int32_args->Add(sgl_arg);
    sgl_int32_args->Add(int32_arg);

    sgl_int64_args = FdoArgumentDefinitionCollection::Create();
    sgl_int64_args->Add(sgl_arg);
    sgl_int64_args->Add(int64_arg);

    sgl_sgl_args = FdoArgumentDefinitionCollection::Create();
    sgl_sgl_args->Add(sgl_arg);
    sgl_sgl_args->Add(sgl_arg);

    str_dcl_args = FdoArgumentDefinitionCollection::Create();
    str_dcl_args->Add(str_arg);
    str_dcl_args->Add(dcl_arg);

    str_dbl_args = FdoArgumentDefinitionCollection::Create();
    str_dbl_args->Add(str_arg);
    str_dbl_args->Add(dbl_arg);

    str_int16_args = FdoArgumentDefinitionCollection::Create();
    str_int16_args->Add(str_arg);
    str_int16_args->Add(int16_arg);

    str_int32_args = FdoArgumentDefinitionCollection::Create();
    str_int32_args->Add(str_arg);
    str_int32_args->Add(int32_arg);

    str_int64_args = FdoArgumentDefinitionCollection::Create();
    str_int64_args->Add(str_arg);
    str_int64_args->Add(int64_arg);

    str_sgl_args = FdoArgumentDefinitionCollection::Create();
    str_sgl_args->Add(str_arg);
    str_sgl_args->Add(sgl_arg);

    str_str_args = FdoArgumentDefinitionCollection::Create();
    str_str_args->Add(str_arg);
    str_str_args->Add(str_arg);

    // Create the signature collection.

    signatures = FdoSignatureDefinitionCollection::Create();

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Boolean, bool_bool_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Byte, byte_byte_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_DateTime, dt_dt_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Decimal, dcl_dcl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Double, dcl_dbl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Decimal, dcl_int16_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Decimal, dcl_int32_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Double, dcl_int64_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Single, dcl_sgl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Double, dbl_dcl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Double, dbl_dbl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Double, dbl_int16_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Double, dbl_int32_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Double, dbl_int64_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Double, dbl_sgl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Double, int16_dcl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Double, int16_dbl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Int16, int16_int16_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Int32, int16_int32_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Int64, int16_int64_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Single, int16_sgl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Double, int32_dcl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Double, int32_dbl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Int32, int32_int16_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Int32, int32_int32_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Int64, int32_int64_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Double, int32_sgl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Double, int64_dcl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Double, int64_dbl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Int64, int64_int16_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Int64, int64_int32_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Int64, int64_int64_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Double, int64_sgl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Double, sgl_dcl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Double, sgl_dbl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Single, sgl_int16_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Double, sgl_int32_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Double, sgl_int64_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Single, sgl_sgl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, str_dcl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, str_dbl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, str_int16_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, str_int32_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, str_int64_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, str_sgl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, str_str_args);
    signatures->Add(signature);

    // Create the function definition.

    desc =
        FdoException::NLSGetMessage(
            FUNCTION_NULLVALUE,
            "Returns the first argument if not NULL, the second otherwise");
    function_definition =
                FdoFunctionDefinition::Create(
                                        FDO_FUNCTION_NULLVALUE,
                                        desc,
                                        false,
                                        signatures,
                                        FdoFunctionCategoryType_Conversion);

}  //  CreateFunctionDefinition ()

FdoLiteralValue *FdoFunctionNullValue::ProcessBooleanRequest (
                                    FdoLiteralValueCollection *literal_values)

// +---------------------------------------------------------------------------
// | The function executes the request in case the provided parameters are of
// | type BOOLEAN and returns the result back to the calling routine.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoPtr<FdoBooleanValue> boolean_value;

    if (first)
    {
        return_data_value = FdoBooleanValue::Create();
        first = false;
    }

    // Get the first parameter. If this is not NULL, use it as the function re-
    // turn value. Otherwise, get the second parameter and return this one.

    boolean_value = (FdoBooleanValue *) literal_values->GetItem(0);
    if (!boolean_value->IsNull())
    {
        (static_cast<FdoBooleanValue *> (return_data_value.p))->SetBoolean(boolean_value->GetBoolean());
        return FDO_SAFE_ADDREF(return_data_value.p);
    }

    boolean_value = (FdoBooleanValue *) literal_values->GetItem(1);
    if (!boolean_value->IsNull())
    {
        (static_cast<FdoBooleanValue *> (return_data_value.p))->SetBoolean(boolean_value->GetBoolean());
        return FDO_SAFE_ADDREF(return_data_value.p);
    }

    return_data_value->SetNull();
    return FDO_SAFE_ADDREF(return_data_value.p);
    
}  //  ProcessBooleanRequest ()

FdoLiteralValue *FdoFunctionNullValue::ProcessByteRequest (
                                    FdoLiteralValueCollection *literal_values)

// +---------------------------------------------------------------------------
// | The function executes the request in case the provided parameters are of
// | type BYTE and returns the result back to the calling procedure.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoPtr<FdoByteValue> byte_value;

    if (first)
    {
        return_data_value = FdoByteValue::Create();
        first = false;
    }

    // Get the first parameter. If this is not NULL, use it as the function re-
    // turn value. Otherwise, get the second parameter and return this one.

    byte_value = (FdoByteValue *) literal_values->GetItem(0);
    if (!byte_value->IsNull())
    {
        (static_cast<FdoByteValue *> (return_data_value.p))->SetByte(byte_value->GetByte());
        return FDO_SAFE_ADDREF(return_data_value.p);
    }

    byte_value = (FdoByteValue *) literal_values->GetItem(1);
    if (!byte_value->IsNull())
    {
        (static_cast<FdoByteValue *> (return_data_value.p))->SetByte(byte_value->GetByte());
        return FDO_SAFE_ADDREF(return_data_value.p);
    }

    return_data_value->SetNull();
    return FDO_SAFE_ADDREF(return_data_value.p);
    
}  //  ProcessByteRequest ()

FdoLiteralValue *FdoFunctionNullValue::ProcessDateTimeRequest (
                                    FdoLiteralValueCollection *literal_values)

// +---------------------------------------------------------------------------
// | The function executes the request in case the provided parameters are of
// | type DATETIME and returns the result back to the calling routine.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoPtr<FdoDateTimeValue> dt_value;

    if (first)
    {
        return_data_value = FdoDateTimeValue::Create();
        first = false;
    }
    // Get the first parameter. If this is not NULL, use it as the function re-
    // turn value. Otherwise, get the second parameter and return this one.

    dt_value = (FdoDateTimeValue *) literal_values->GetItem(0);
    if (!dt_value->IsNull())
    {
        (static_cast<FdoDateTimeValue *> (return_data_value.p))->SetDateTime(dt_value->GetDateTime());
        return FDO_SAFE_ADDREF(return_data_value.p);
    }

    dt_value = (FdoDateTimeValue *) literal_values->GetItem(1);
    if (!dt_value->IsNull())
    {
        (static_cast<FdoDateTimeValue *> (return_data_value.p))->SetDateTime(dt_value->GetDateTime());
        return FDO_SAFE_ADDREF(return_data_value.p);
    }

    return_data_value->SetNull();
    return FDO_SAFE_ADDREF(return_data_value.p);
    
}  //  ProcessDateTimeRequest ()

FdoLiteralValue *FdoFunctionNullValue::ProcessDecimalRequest (
                                    FdoLiteralValueCollection *literal_values)

// +---------------------------------------------------------------------------
// | The function executes the request in case the provided first parameter is
// | of type DECIMAL and returns the result back to the calling routine.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoPtr<FdoDecimalValue> decimal_value;
    FdoPtr<FdoDoubleValue>  double_value;
    FdoPtr<FdoInt16Value>   int16_value;
    FdoPtr<FdoInt32Value>   int32_value;
    FdoPtr<FdoInt64Value>   int64_value;
    FdoPtr<FdoSingleValue>  single_value;

    // Get the first parameter. If it is not NULL, the value is used for the
    // return parameter. The type of the return parameter, however, depends
    // on the data type of the second parameter.

    decimal_value = (FdoDecimalValue *) literal_values->GetItem(0);
    switch (para2_data_type) {

      case FdoDataType_Decimal:
      case FdoDataType_Int16:
      case FdoDataType_Int32:
        if (!decimal_value->IsNull())
        {
            if (first)
            {
                return_data_value = FdoDecimalValue::Create();
                first = false;
            }
            (static_cast<FdoDecimalValue *> (return_data_value.p))->SetDecimal(decimal_value->GetDecimal());
            return FDO_SAFE_ADDREF(return_data_value.p);
        }
        break;

      case FdoDataType_Double:
      case FdoDataType_Int64:
      case FdoDataType_Single:
        if (!decimal_value->IsNull())
        {
            if (first)
            {
                return_data_value = FdoDoubleValue::Create();
                first = false;
            }
            (static_cast<FdoDoubleValue *> (return_data_value.p))->SetDouble(decimal_value->GetDecimal());
            return FDO_SAFE_ADDREF(return_data_value.p);
        }
        break;

      default:
        throw FdoException::Create(
                FdoException::NLSGetMessage(
                FUNCTION_PARAMETER_DATA_TYPE_ERROR, 
                "Expression Engine: Invalid parameter data type for function '%1$ls'",
                FDO_FUNCTION_NULLVALUE));
        break;

    }  //  switch ...

    // At this point, the first parameter is NULL and hence the second para-
    // meter has to be retrieved and analysed. It serves as the value for the
    // return parameter. The return parameter's type depends on the type of
    // the second parameter.

    switch (para2_data_type) {

      case FdoDataType_Decimal:
        if (first)
        {
            return_data_value = FdoDecimalValue::Create();
            first = false;
        }
        decimal_value = (FdoDecimalValue *) literal_values->GetItem(1);
        if (!decimal_value->IsNull())
            (static_cast<FdoDecimalValue *> (return_data_value.p))->SetDecimal(decimal_value->GetDecimal());
        else
            return_data_value->SetNull();
        return FDO_SAFE_ADDREF(return_data_value.p);
        break;

      case FdoDataType_Double:
        if (first)
        {
            return_data_value = FdoDoubleValue::Create();
            first = false;
        }
        double_value = (FdoDoubleValue *) literal_values->GetItem(1);
        if (!double_value->IsNull())
            (static_cast<FdoDoubleValue *> (return_data_value.p))->SetDouble(double_value->GetDouble());
        else
            return_data_value->SetNull();
        return FDO_SAFE_ADDREF(return_data_value.p);
        break;

      case FdoDataType_Int16:
        if (first)
        {
            return_data_value = FdoDecimalValue::Create();
            first = false;
        }
        int16_value = (FdoInt16Value *) literal_values->GetItem(1);
        if (!int16_value->IsNull())
            (static_cast<FdoDecimalValue *> (return_data_value.p))->SetDecimal(int16_value->GetInt16());
        else
            return_data_value->SetNull();
        return FDO_SAFE_ADDREF(return_data_value.p);
        break;

      case FdoDataType_Int32:
        if (first)
        {
            return_data_value = FdoDecimalValue::Create();
            first = false;
        }
        int32_value = (FdoInt32Value *) literal_values->GetItem(1);
        if (!int32_value->IsNull())
            (static_cast<FdoDecimalValue *> (return_data_value.p))->SetDecimal(int32_value->GetInt32());
        else
            return_data_value->SetNull();
        return FDO_SAFE_ADDREF(return_data_value.p);
        break;

      case FdoDataType_Int64:
        if (first)
        {
            return_data_value = FdoDoubleValue::Create();
            first = false;
        }
        int64_value = (FdoInt64Value *) literal_values->GetItem(1);
        if (!int64_value->IsNull())
            (static_cast<FdoDoubleValue *> (return_data_value.p))->SetDouble((double) int64_value->GetInt64());
        else
            return_data_value->SetNull();
        return FDO_SAFE_ADDREF(return_data_value.p);
        break;

      case FdoDataType_Single:
        if (first)
        {
            return_data_value = FdoDoubleValue::Create();
            first = false;
        }
        single_value = (FdoSingleValue *) literal_values->GetItem(1);
        if (!single_value->IsNull())
            (static_cast<FdoDoubleValue *> (return_data_value.p))->SetDouble(single_value->GetSingle());
        else
            return_data_value->SetNull();
        return FDO_SAFE_ADDREF(return_data_value.p);
        break;

      default:
        throw FdoException::Create(
                FdoException::NLSGetMessage(
                FUNCTION_PARAMETER_DATA_TYPE_ERROR, 
                "Expression Engine: Invalid parameter data type for function '%1$ls'",
                FDO_FUNCTION_NULLVALUE));
        break;

    }  //  switch ...

    // The previous switch statement should have processed all valid cases
    // and hence the following part of the code should not be reached. If
    // it is reached issue an exception.

    throw FdoException::Create(
            FdoException::NLSGetMessage(
                FUNCTION_UNEXPECTED_RESULT_ERROR, 
                "Expression Engine: Unexpected result for function '%1$ls'",
                FDO_FUNCTION_NULLVALUE));
    
}  //  ProcessDecimalRequest ()

FdoLiteralValue *FdoFunctionNullValue::ProcessDoubleRequest (
                                    FdoLiteralValueCollection *literal_values)

// +---------------------------------------------------------------------------
// | The function executes the request in case the provided first parameter is
// | of type DOUBLE and returns the result back to the calling routine.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    bool                    is_NULL         = false,
                            is_processed    = false;

    FdoDouble               curr_value;

    FdoPtr<FdoDecimalValue> decimal_value;
    FdoPtr<FdoDoubleValue>  double_value;
    FdoPtr<FdoInt16Value>   int16_value;
    FdoPtr<FdoInt32Value>   int32_value;
    FdoPtr<FdoInt64Value>   int64_value;
    FdoPtr<FdoSingleValue>  single_value;

    if (first)
    {
        return_data_value = FdoDoubleValue::Create();
        first = false;
    }

    // Get the first parameter. If it is not NULL, the value is used for the
    // return parameter. The type of the return parameter is independent of
    // the type of the second parameter always a DOUBLE.

    double_value = (FdoDoubleValue *) literal_values->GetItem(0);
    if (!double_value->IsNull())
    {
        (static_cast<FdoDoubleValue *> (return_data_value.p))->SetDouble(double_value->GetDouble());
        return FDO_SAFE_ADDREF(return_data_value.p);
    }

    // At this point, the first parameter is NULL and hence the second para-
    // meter has to be retrieved and analysed. It serves as the value for the
    // return parameter. Again the return parameter is independent of the
    // data type of the second parameter and is always a double.

    switch (para2_data_type) {

      case FdoDataType_Decimal:
        is_processed  = true;
        decimal_value = (FdoDecimalValue *) literal_values->GetItem(1);
        is_NULL       = decimal_value->IsNull();
        if (!is_NULL)
            curr_value = decimal_value->GetDecimal();
        break;

      case FdoDataType_Double:
        is_processed = true;
        double_value = (FdoDoubleValue *) literal_values->GetItem(1);
        is_NULL      = double_value->IsNull();
        if (!is_NULL)
            curr_value = double_value->GetDouble();
        break;

      case FdoDataType_Int16:
        is_processed = true;
        int16_value  = (FdoInt16Value *) literal_values->GetItem(1);
        is_NULL      = int16_value->IsNull();
        if (!is_NULL)
            curr_value = (FdoDouble) int16_value->GetInt16();
        break;

      case FdoDataType_Int32:
        is_processed = true;
        int32_value  = (FdoInt32Value *) literal_values->GetItem(1);
        is_NULL      = int32_value->IsNull();
        if (!is_NULL)
            curr_value = (FdoDouble) int32_value->GetInt32();
        break;

      case FdoDataType_Int64:
        is_processed = true;
        int64_value  = (FdoInt64Value *) literal_values->GetItem(1);
        is_NULL      = int64_value->IsNull();
        if (!is_NULL)
            curr_value = (FdoDouble) int64_value->GetInt64();
        break;

      case FdoDataType_Single:
        is_processed  = true;
        single_value  = (FdoSingleValue *) literal_values->GetItem(1);
        is_NULL       = single_value->IsNull();
        if (!is_NULL)
            curr_value = (FdoDouble) single_value->GetSingle();
        break;

      default:
        throw FdoException::Create(
                FdoException::NLSGetMessage(
                FUNCTION_PARAMETER_DATA_TYPE_ERROR, 
                "Expression Engine: Invalid parameter data type for function '%1$ls'",
                FDO_FUNCTION_NULLVALUE));
        break;

    }  //  switch ...

    if (is_processed) {

        if (!is_NULL)
            (static_cast<FdoDoubleValue *> (return_data_value.p))->SetDouble(curr_value);
        else
            return_data_value->SetNull();
        return FDO_SAFE_ADDREF(return_data_value.p);

    }  //  if (is_processed) ...

    // The previous switch statement should have processed all valid cases
    // and hence the following part of the code should not be reached. If
    // it is reached issue an exception.

    throw FdoException::Create(
            FdoException::NLSGetMessage(
                FUNCTION_UNEXPECTED_RESULT_ERROR, 
                "Expression Engine: Unexpected result for function '%1$ls'",
                FDO_FUNCTION_NULLVALUE));
    
}  //  ProcessDoubleRequest ()

FdoLiteralValue *FdoFunctionNullValue::ProcessInt16Request (
                                    FdoLiteralValueCollection *literal_values)

// +---------------------------------------------------------------------------
// | The function executes the request in case the provided first parameter is
// | of type INT16 and returns the result back to the calling routine.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoPtr<FdoDecimalValue> decimal_value;
    FdoPtr<FdoDoubleValue>  double_value;
    FdoPtr<FdoInt16Value>   int16_value;
    FdoPtr<FdoInt32Value>   int32_value;
    FdoPtr<FdoInt64Value>   int64_value;
    FdoPtr<FdoSingleValue>  single_value;

    // Get the first parameter. If it is not NULL, the value is used for the
    // return parameter. The type of the return parameter, however, depends
    // on the data type of the second parameter.

    int16_value = (FdoInt16Value *) literal_values->GetItem(0);
    switch (para2_data_type) {

      case FdoDataType_Decimal:
      case FdoDataType_Double:
        if (!int16_value->IsNull())
        {
            if (first)
            {
                return_data_value = FdoDoubleValue::Create();
                first = false;
            }
            (static_cast<FdoDoubleValue *> (return_data_value.p))->SetDouble(int16_value->GetInt16());
            return FDO_SAFE_ADDREF(return_data_value.p);
        }
        break;

      case FdoDataType_Int16:
        if (!int16_value->IsNull())
        {
            if (first)
            {
                return_data_value = FdoInt16Value::Create();
                first = false;
            }
            (static_cast<FdoInt16Value *> (return_data_value.p))->SetInt16(int16_value->GetInt16());
            return FDO_SAFE_ADDREF(return_data_value.p);
        }
        break;

      case FdoDataType_Int32:
        if (!int16_value->IsNull())
        {
            if (first)
            {
                return_data_value = FdoInt32Value::Create();
                first = false;
            }
            (static_cast<FdoInt32Value *> (return_data_value.p))->SetInt32(int16_value->GetInt16());
            return FDO_SAFE_ADDREF(return_data_value.p);
        }
        break;

      case FdoDataType_Int64:
        if (!int16_value->IsNull())
        {
            if (first)
            {
                return_data_value = FdoInt64Value::Create();
                first = false;
            }
            (static_cast<FdoInt64Value *> (return_data_value.p))->SetInt64(int16_value->GetInt16());
            return FDO_SAFE_ADDREF(return_data_value.p);
        }
        break;

      case FdoDataType_Single:
        if (!int16_value->IsNull())
        {
            if (first)
            {
                return_data_value = FdoSingleValue::Create();
                first = false;
            }
            (static_cast<FdoSingleValue *> (return_data_value.p))->SetSingle(int16_value->GetInt16());
            return FDO_SAFE_ADDREF(return_data_value.p);
        }
        break;

      default:
        throw FdoException::Create(
                FdoException::NLSGetMessage(
                FUNCTION_PARAMETER_DATA_TYPE_ERROR, 
                "Expression Engine: Invalid parameter data type for function '%1$ls'",
                FDO_FUNCTION_NULLVALUE));
        break;

    }  //  switch ...

    // At this point, the first parameter is NULL and hence the second para-
    // meter has to be retrieved and analysed. It serves as the value for the
    // return parameter. The return parameter's type depends on the type of
    // the second parameter.

    switch (para2_data_type) {

      case FdoDataType_Decimal:
        if (first)
        {
            return_data_value = FdoDoubleValue::Create();
            first = false;
        }
        decimal_value = (FdoDecimalValue *) literal_values->GetItem(1);
        if (!decimal_value->IsNull())
            (static_cast<FdoDoubleValue *> (return_data_value.p))->SetDouble(decimal_value->GetDecimal());
        else
            return_data_value->SetNull();
        return FDO_SAFE_ADDREF(return_data_value.p);
        break;

      case FdoDataType_Double:
        if (first)
        {
            return_data_value = FdoDoubleValue::Create();
            first = false;
        }
        double_value = (FdoDoubleValue *) literal_values->GetItem(1);
        if (!double_value->IsNull())
            (static_cast<FdoDoubleValue *> (return_data_value.p))->SetDouble(double_value->GetDouble());
        else
            return_data_value->SetNull();
        return FDO_SAFE_ADDREF(return_data_value.p);
        break;

      case FdoDataType_Int16:
        if (first)
        {
            return_data_value = FdoInt16Value::Create();
            first = false;
        }

        int16_value = (FdoInt16Value *) literal_values->GetItem(1);
        if (!int16_value->IsNull())
            (static_cast<FdoInt16Value *> (return_data_value.p))->SetInt16(int16_value->GetInt16());
        else
            return_data_value->SetNull();
        return FDO_SAFE_ADDREF(return_data_value.p);
        break;

      case FdoDataType_Int32:
        if (first)
        {
            return_data_value = FdoInt32Value::Create();
            first = false;
        }
        int32_value = (FdoInt32Value *) literal_values->GetItem(1);
        if (!int32_value->IsNull())
            (static_cast<FdoInt32Value *> (return_data_value.p))->SetInt32(int32_value->GetInt32());
        else
            return_data_value->SetNull();
        return FDO_SAFE_ADDREF(return_data_value.p);
        break;

      case FdoDataType_Int64:
        if (first)
        {
            return_data_value = FdoInt64Value::Create();
            first = false;
        }
        int64_value = (FdoInt64Value *) literal_values->GetItem(1);
        if (!int64_value->IsNull())
            (static_cast<FdoInt64Value *> (return_data_value.p))->SetInt64(int64_value->GetInt64());
        else
            return_data_value->SetNull();
        return FDO_SAFE_ADDREF(return_data_value.p);
        break;

      case FdoDataType_Single:
        if (first)
        {
            return_data_value = FdoSingleValue::Create();
            first = false;
        }
        single_value = (FdoSingleValue *) literal_values->GetItem(1);
        if (!single_value->IsNull())
            (static_cast<FdoSingleValue *> (return_data_value.p))->SetSingle(single_value->GetSingle());
        else
            return_data_value->SetNull();
        return FDO_SAFE_ADDREF(return_data_value.p);
        break;

      default:
        throw FdoException::Create(
                FdoException::NLSGetMessage(
                FUNCTION_PARAMETER_DATA_TYPE_ERROR, 
                "Expression Engine: Invalid parameter data type for function '%1$ls'",
                FDO_FUNCTION_NULLVALUE));
        break;

    }  //  switch ...

    // The previous switch statement should have processed all valid cases
    // and hence the following part of the code should not be reached. If
    // it is reached issue an exception.

    throw FdoException::Create(
            FdoException::NLSGetMessage(
                FUNCTION_UNEXPECTED_RESULT_ERROR, 
                "Expression Engine: Unexpected result for function '%1$ls'",
                FDO_FUNCTION_NULLVALUE));
    
}  //  ProcessInt16Request ()

FdoLiteralValue *FdoFunctionNullValue::ProcessInt32Request (
                                    FdoLiteralValueCollection *literal_values)

// +---------------------------------------------------------------------------
// | The function executes the request in case the provided first parameter is
// | of type INT32 and returns the result back to the calling routine.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoPtr<FdoDecimalValue> decimal_value;
    FdoPtr<FdoDoubleValue>  double_value;
    FdoPtr<FdoInt16Value>   int16_value;
    FdoPtr<FdoInt32Value>   int32_value;
    FdoPtr<FdoInt64Value>   int64_value;
    FdoPtr<FdoSingleValue>  single_value;

    // Get the first parameter. If it is not NULL, the value is used for the
    // return parameter. The type of the return parameter, however, depends
    // on the data type of the second parameter.

    int32_value = (FdoInt32Value *) literal_values->GetItem(0);
    switch (para2_data_type) {

      case FdoDataType_Decimal:
      case FdoDataType_Double:
      case FdoDataType_Single:
        if (!int32_value->IsNull())
        {
            if (first)
            {
                return_data_value = FdoDoubleValue::Create();
                first = false;
            }
            (static_cast<FdoDoubleValue *> (return_data_value.p))->SetDouble(int32_value->GetInt32());
            return FDO_SAFE_ADDREF(return_data_value.p);
        }
        break;

      case FdoDataType_Int16:
      case FdoDataType_Int32:
        if (!int32_value->IsNull())
        {
            if (first)
            {
                return_data_value = FdoInt32Value::Create();
                first = false;
            }
            (static_cast<FdoInt32Value *> (return_data_value.p))->SetInt32(int32_value->GetInt32());
            return FDO_SAFE_ADDREF(return_data_value.p);
        }
        break;

      case FdoDataType_Int64:
        if (!int32_value->IsNull())
        {
            if (first)
            {
                return_data_value = FdoInt64Value::Create();
                first = false;
            }
            (static_cast<FdoInt64Value *> (return_data_value.p))->SetInt64(int32_value->GetInt32());
            return FDO_SAFE_ADDREF(return_data_value.p);
        }
        break;

      default:
        throw FdoException::Create(
                FdoException::NLSGetMessage(
                FUNCTION_PARAMETER_DATA_TYPE_ERROR, 
                "Expression Engine: Invalid parameter data type for function '%1$ls'",
                FDO_FUNCTION_NULLVALUE));
        break;

    }  //  switch ...

    // At this point, the first parameter is NULL and hence the second para-
    // meter has to be retrieved and analysed. It serves as the value for the
    // return parameter. The return parameter's type depends on the type of
    // the second parameter.

    switch (para2_data_type) {

      case FdoDataType_Decimal:

        if (first)
        {
            return_data_value = FdoDoubleValue::Create();
            first = false;
        }
        decimal_value = (FdoDecimalValue *) literal_values->GetItem(1);
        if (!decimal_value->IsNull())
            (static_cast<FdoDoubleValue *> (return_data_value.p))->SetDouble(decimal_value->GetDecimal());
        else
            return_data_value->SetNull();
        return FDO_SAFE_ADDREF(return_data_value.p);
        break;

      case FdoDataType_Double:
        if (first)
        {
            return_data_value = FdoDoubleValue::Create();
            first = false;
        }
        double_value = (FdoDoubleValue *) literal_values->GetItem(1);
        if (!double_value->IsNull())
            (static_cast<FdoDoubleValue *> (return_data_value.p))->SetDouble(double_value->GetDouble());
        else
            return_data_value->SetNull();
        return FDO_SAFE_ADDREF(return_data_value.p);
        break;

      case FdoDataType_Int16:
        if (first)
        {
            return_data_value = FdoInt32Value::Create();
            first = false;
        }
        int16_value = (FdoInt16Value *) literal_values->GetItem(1);
        if (!int16_value->IsNull())
            (static_cast<FdoInt32Value *> (return_data_value.p))->SetInt32(int16_value->GetInt16());
        else
            return_data_value->SetNull();
        return FDO_SAFE_ADDREF(return_data_value.p);
        break;

      case FdoDataType_Int32:
        if (first)
        {
            return_data_value = FdoInt32Value::Create();
            first = false;
        }
        int32_value = (FdoInt32Value *) literal_values->GetItem(1);
        if (!int32_value->IsNull())
            (static_cast<FdoInt32Value *> (return_data_value.p))->SetInt32(int32_value->GetInt32());
        else
            return_data_value->SetNull();
        return FDO_SAFE_ADDREF(return_data_value.p);
        break;

      case FdoDataType_Int64:
        if (first)
        {
            return_data_value = FdoInt64Value::Create();
            first = false;
        }
        int64_value = (FdoInt64Value *) literal_values->GetItem(1);
        if (!int64_value->IsNull())
            (static_cast<FdoInt64Value *> (return_data_value.p))->SetInt64(int64_value->GetInt64());
        else
            return_data_value->SetNull();
        return FDO_SAFE_ADDREF(return_data_value.p);
        break;

      case FdoDataType_Single:
        if (first)
        {
            return_data_value = FdoDoubleValue::Create();
            first = false;
        }
        single_value = (FdoSingleValue *) literal_values->GetItem(1);
        if (!single_value->IsNull())
            (static_cast<FdoDoubleValue *> (return_data_value.p))->SetDouble(single_value->GetSingle());
        else
            return_data_value->SetNull();
        return FDO_SAFE_ADDREF(return_data_value.p);
        break;

      default:
        throw FdoException::Create(
                FdoException::NLSGetMessage(
                FUNCTION_PARAMETER_DATA_TYPE_ERROR, 
                "Expression Engine: Invalid parameter data type for function '%1$ls'",
                FDO_FUNCTION_NULLVALUE));
        break;

    }  //  switch ...

    // The previous switch statement should have processed all valid cases
    // and hence the following part of the code should not be reached. If
    // it is reached issue an exception.

    throw FdoException::Create(
            FdoException::NLSGetMessage(
                FUNCTION_UNEXPECTED_RESULT_ERROR, 
                "Expression Engine: Unexpected result for function '%1$ls'",
                FDO_FUNCTION_NULLVALUE));
    
}  //  ProcessInt32Request ()

FdoLiteralValue *FdoFunctionNullValue::ProcessInt64Request (
                                    FdoLiteralValueCollection *literal_values)

// +---------------------------------------------------------------------------
// | The function executes the request in case the provided first parameter is
// | of type INT64 and returns the result back to the calling routine.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoPtr<FdoDecimalValue> decimal_value;
    FdoPtr<FdoDoubleValue>  double_value;
    FdoPtr<FdoInt16Value>   int16_value;
    FdoPtr<FdoInt32Value>   int32_value;
    FdoPtr<FdoInt64Value>   int64_value;
    FdoPtr<FdoSingleValue>  single_value;

    // Get the first parameter. If it is not NULL, the value is used for the
    // return parameter. The type of the return parameter, however, depends
    // on the data type of the second parameter.

    int64_value = (FdoInt64Value *) literal_values->GetItem(0);
    switch (para2_data_type) {

      case FdoDataType_Decimal:
      case FdoDataType_Double:
      case FdoDataType_Single:
        if (!int64_value->IsNull())
        {
            if (first)
            {
                return_data_value = FdoDoubleValue::Create();
                first = false;
            }
            (static_cast<FdoDoubleValue *> (return_data_value.p))->SetDouble((FdoDouble) int64_value->GetInt64());
            return FDO_SAFE_ADDREF(return_data_value.p);
        }
        break;

      case FdoDataType_Int16:
      case FdoDataType_Int32:
      case FdoDataType_Int64:
        if (!int64_value->IsNull())
        {
            if (first)
            {
                return_data_value = FdoInt64Value::Create();
                first = false;
            }
            (static_cast<FdoInt64Value *> (return_data_value.p))->SetInt64(int64_value->GetInt64());
            return FDO_SAFE_ADDREF(return_data_value.p);
        }
        break;

      default:
        throw FdoException::Create(
                FdoException::NLSGetMessage(
                FUNCTION_PARAMETER_DATA_TYPE_ERROR, 
                "Expression Engine: Invalid parameter data type for function '%1$ls'",
                FDO_FUNCTION_NULLVALUE));
        break;

    }  //  switch ...

    // At this point, the first parameter is NULL and hence the second para-
    // meter has to be retrieved and analysed. It serves as the value for the
    // return parameter. The return parameter's type depends on the type of
    // the second parameter.

    switch (para2_data_type) {

      case FdoDataType_Decimal:
        if (first)
        {
            return_data_value = FdoDoubleValue::Create();
            first = false;
        }
        decimal_value = (FdoDecimalValue *) literal_values->GetItem(1);
        if (!decimal_value->IsNull())
            (static_cast<FdoDoubleValue *> (return_data_value.p))->SetDouble(decimal_value->GetDecimal());
        else
            return_data_value->SetNull();
        return FDO_SAFE_ADDREF(return_data_value.p);
        break;

      case FdoDataType_Double:
        if (first)
        {
            return_data_value = FdoDoubleValue::Create();
            first = false;
        }
        double_value = (FdoDoubleValue *) literal_values->GetItem(1);
        if (!double_value->IsNull())
            (static_cast<FdoDoubleValue *> (return_data_value.p))->SetDouble(double_value->GetDouble());
        else
            return_data_value->SetNull();
        return FDO_SAFE_ADDREF(return_data_value.p);
        break;

      case FdoDataType_Int16:
        if (first)
        {
            return_data_value = FdoInt64Value::Create();
            first = false;
        }
        int16_value = (FdoInt16Value *) literal_values->GetItem(1);
        if (!int16_value->IsNull())
            (static_cast<FdoInt64Value *> (return_data_value.p))->SetInt64(int16_value->GetInt16());
        else
            return_data_value->SetNull();
        return FDO_SAFE_ADDREF(return_data_value.p);
        break;

      case FdoDataType_Int32:
        if (first)
        {
            return_data_value = FdoInt64Value::Create();
            first = false;
        }
        int32_value = (FdoInt32Value *) literal_values->GetItem(1);
        if (!int32_value->IsNull())
            (static_cast<FdoInt64Value *> (return_data_value.p))->SetInt64(int32_value->GetInt32());
        else
            return_data_value->SetNull();
        return FDO_SAFE_ADDREF(return_data_value.p);
        break;

      case FdoDataType_Int64:
        if (first)
        {
            return_data_value = FdoInt64Value::Create();
            first = false;
        }
        int64_value = (FdoInt64Value *) literal_values->GetItem(1);
        if (!int64_value->IsNull())
            (static_cast<FdoInt64Value *> (return_data_value.p))->SetInt64(int64_value->GetInt64());
        else
            return_data_value->SetNull();
        return FDO_SAFE_ADDREF(return_data_value.p);
        break;

      case FdoDataType_Single:
        if (first)
        {
            return_data_value = FdoDoubleValue::Create();
            first = false;
        }
        single_value = (FdoSingleValue *) literal_values->GetItem(1);
        if (!single_value->IsNull())
            (static_cast<FdoDoubleValue *> (return_data_value.p))->SetDouble(single_value->GetSingle());
        else
            return_data_value->SetNull();
        return FDO_SAFE_ADDREF(return_data_value.p);
        break;

      default:
        throw FdoException::Create(
                FdoException::NLSGetMessage(
                FUNCTION_PARAMETER_DATA_TYPE_ERROR, 
                "Expression Engine: Invalid parameter data type for function '%1$ls'",
                FDO_FUNCTION_NULLVALUE));
        break;

    }  //  switch ...

    // The previous switch statement should have processed all valid cases
    // and hence the following part of the code should not be reached. If
    // it is reached issue an exception.

    throw FdoException::Create(
            FdoException::NLSGetMessage(
                FUNCTION_UNEXPECTED_RESULT_ERROR, 
                "Expression Engine: Unexpected result for function '%1$ls'",
                FDO_FUNCTION_NULLVALUE));
    
}  //  ProcessInt64Request ()

FdoLiteralValue *FdoFunctionNullValue::ProcessSingleRequest (
                                    FdoLiteralValueCollection *literal_values)

// +---------------------------------------------------------------------------
// | The function executes the request in case the provided first parameter is
// | of type SINGLE and returns the result back to the calling routine.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoPtr<FdoDecimalValue> decimal_value;
    FdoPtr<FdoDoubleValue>  double_value;
    FdoPtr<FdoInt16Value>   int16_value;
    FdoPtr<FdoInt32Value>   int32_value;
    FdoPtr<FdoInt64Value>   int64_value;
    FdoPtr<FdoSingleValue>  single_value;

    // Get the first parameter. If it is not NULL, the value is used for the
    // return parameter. The type of the return parameter, however, depends
    // on the data type of the second parameter.

    single_value = (FdoSingleValue *) literal_values->GetItem(0);
    switch (para2_data_type) {

      case FdoDataType_Decimal:
      case FdoDataType_Double:
      case FdoDataType_Int32:
      case FdoDataType_Int64:
        if (!single_value->IsNull())
        {
            if (first)
            {
                return_data_value = FdoDoubleValue::Create();
                first = false;
            }
            (static_cast<FdoDoubleValue *> (return_data_value.p))->SetDouble(single_value->GetSingle());
            return FDO_SAFE_ADDREF(return_data_value.p);
        }
        break;

      case FdoDataType_Int16:
      case FdoDataType_Single:
        if (!single_value->IsNull())
        {
            if (first)
            {
                return_data_value = FdoSingleValue::Create();
                first = false;
            }
            (static_cast<FdoSingleValue *> (return_data_value.p))->SetSingle(single_value->GetSingle());
            return FDO_SAFE_ADDREF(return_data_value.p);
        }
        break;

      default:
        throw FdoException::Create(
                FdoException::NLSGetMessage(
                FUNCTION_PARAMETER_DATA_TYPE_ERROR, 
                "Expression Engine: Invalid parameter data type for function '%1$ls'",
                FDO_FUNCTION_NULLVALUE));
        break;

    }  //  switch ...

    // At this point, the first parameter is NULL and hence the second para-
    // meter has to be retrieved and analysed. It serves as the value for the
    // return parameter. The return parameter's type depends on the type of
    // the second parameter.

    switch (para2_data_type) {

      case FdoDataType_Decimal:
        if (first)
        {
            return_data_value = FdoDoubleValue::Create();
            first = false;
        }
        decimal_value = (FdoDecimalValue *) literal_values->GetItem(1);
        if (!decimal_value->IsNull())
            (static_cast<FdoDoubleValue *> (return_data_value.p))->SetDouble(decimal_value->GetDecimal());
        else
            return_data_value->SetNull();
        return FDO_SAFE_ADDREF(return_data_value.p);
        break;

      case FdoDataType_Double:
        if (first)
        {
            return_data_value = FdoDoubleValue::Create();
            first = false;
        }
        double_value = (FdoDoubleValue *) literal_values->GetItem(1);
        if (!double_value->IsNull())
            (static_cast<FdoDoubleValue *> (return_data_value.p))->SetDouble(double_value->GetDouble());
        else
            return_data_value->SetNull();
            return FDO_SAFE_ADDREF(return_data_value.p);
        break;

      case FdoDataType_Int16:
        if (first)
        {
            return_data_value = FdoSingleValue::Create();
            first = false;
        }
        int16_value = (FdoInt16Value *) literal_values->GetItem(1);
        if (!int16_value->IsNull())
            (static_cast<FdoSingleValue *> (return_data_value.p))->SetSingle(int16_value->GetInt16());
        else
            return_data_value->SetNull();
        return FDO_SAFE_ADDREF(return_data_value.p);
        break;

      case FdoDataType_Int32:
        if (first)
        {
            return_data_value = FdoDoubleValue::Create();
            first = false;
        }
        int32_value = (FdoInt32Value *) literal_values->GetItem(1);
        if (!int32_value->IsNull())
            (static_cast<FdoDoubleValue *> (return_data_value.p))->SetDouble(int32_value->GetInt32());
        else
            return_data_value->SetNull();
        return FDO_SAFE_ADDREF(return_data_value.p);
        break;

      case FdoDataType_Int64:
        if (first)
        {
            return_data_value = FdoDoubleValue::Create();
            first = false;
        }
        int64_value = (FdoInt64Value *) literal_values->GetItem(1);
        if (!int64_value->IsNull())
            (static_cast<FdoDoubleValue *> (return_data_value.p))->SetDouble((FdoDouble) int64_value->GetInt64());
        else
            return_data_value->SetNull();
        return FDO_SAFE_ADDREF(return_data_value.p);
        break;

      case FdoDataType_Single:
        if (first)
        {
            return_data_value = FdoSingleValue::Create();
            first = false;
        }
        single_value = (FdoSingleValue *) literal_values->GetItem(1);
        if (!single_value->IsNull())
            (static_cast<FdoSingleValue *> (return_data_value.p))->SetSingle(single_value->GetSingle());
        else
            return_data_value->SetNull();
        return FDO_SAFE_ADDREF(return_data_value.p);
        break;

      default:
        throw FdoException::Create(
                FdoException::NLSGetMessage(
                FUNCTION_PARAMETER_DATA_TYPE_ERROR, 
                "Expression Engine: Invalid parameter data type for function '%1$ls'",
                FDO_FUNCTION_NULLVALUE));
        break;

    }  //  switch ...

    // The previous switch statement should have processed all valid cases
    // and hence the following part of the code should not be reached. If
    // it is reached issue an exception.

    throw FdoException::Create(
            FdoException::NLSGetMessage(
                FUNCTION_UNEXPECTED_RESULT_ERROR, 
                "Expression Engine: Unexpected result for function '%1$ls'",
                FDO_FUNCTION_NULLVALUE));
    
}  //  ProcessSingleRequest ()

FdoLiteralValue *FdoFunctionNullValue::ProcessStringRequest (
                                    FdoLiteralValueCollection *literal_values)

// +---------------------------------------------------------------------------
// | The function executes the request in case the provided first parameter is
// | of type STRING and returns the result back to the calling routine.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    bool                    is_NULL         = false,
                            is_processed    = false;

    FdoPtr<FdoDecimalValue> decimal_value;
    FdoPtr<FdoDoubleValue>  double_value;
    FdoPtr<FdoInt16Value>   int16_value;
    FdoPtr<FdoInt32Value>   int32_value;
    FdoPtr<FdoInt64Value>   int64_value;
    FdoPtr<FdoSingleValue>  single_value;
    FdoPtr<FdoStringValue>  string_value;
    wchar_t                 curr_value[64];

    if (first)
    {
        return_data_value = FdoStringValue::Create();
        first = false;
    }

    // Get the first parameter. If it is not NULL, the value is used for the
    // return parameter. The type of the return parameter is - independent of
    // the type of the second parameter - always a STRING.

    string_value = (FdoStringValue *) literal_values->GetItem(0);
    if (!string_value->IsNull())
    {
        (static_cast<FdoStringValue *> (return_data_value.p))->SetString(string_value->GetString());
        return FDO_SAFE_ADDREF(return_data_value.p);
    }

    // At this point, the first parameter is NULL and hence the second para-
    // meter has to be retrieved and analysed. It serves as the value for the
    // return parameter. Again the return parameter is independent of the
    // data type of the second parameter and is always a STRING.

    switch (para2_data_type) {

      case FdoDataType_Decimal:
        is_processed  = true;
        decimal_value = (FdoDecimalValue *) literal_values->GetItem(1);
        is_NULL       = decimal_value->IsNull();
        if (!is_NULL)
            FdoCommonOSUtil::swprintf(curr_value, sizeof(curr_value)/sizeof(wchar_t)-1, L"%f", decimal_value->GetDecimal());
        break;

      case FdoDataType_Double:
        is_processed = true;
        double_value = (FdoDoubleValue *) literal_values->GetItem(1);
        is_NULL      = double_value->IsNull();
        if (!is_NULL)
            FdoCommonOSUtil::swprintf(curr_value, sizeof(curr_value)/sizeof(wchar_t)-1, L"%f", double_value->GetDouble());
        break;

      case FdoDataType_Int16:
        is_processed = true;
        int16_value  = (FdoInt16Value *) literal_values->GetItem(1);
        is_NULL      = int16_value->IsNull();
        if (!is_NULL)
            FdoCommonOSUtil::swprintf(curr_value, sizeof(curr_value)/sizeof(wchar_t)-1, L"%d", int16_value->GetInt16());
        break;

      case FdoDataType_Int32:
        is_processed = true;
        int32_value  = (FdoInt32Value *) literal_values->GetItem(1);
        is_NULL      = int32_value->IsNull();
        if (!is_NULL)
            FdoCommonOSUtil::swprintf(curr_value, sizeof(curr_value)/sizeof(wchar_t)-1, L"%d", int32_value->GetInt32());
        break;

      case FdoDataType_Int64:
        is_processed = true;
        int64_value  = (FdoInt64Value *) literal_values->GetItem(1);
        is_NULL      = int64_value->IsNull();
        if (!is_NULL)
#ifdef _WIN32
            FdoCommonOSUtil::swprintf(curr_value, sizeof(curr_value)/sizeof(wchar_t)-1, L"%I64d", int64_value->GetInt64());
#else
            FdoCommonOSUtil::swprintf(curr_value, sizeof(curr_value)/sizeof(wchar_t)-1, L"%lld", int64_value->GetInt64());
#endif
        break;

      case FdoDataType_Single:
        is_processed  = true;
        single_value  = (FdoSingleValue *) literal_values->GetItem(1);
        is_NULL       = single_value->IsNull();
        if (!is_NULL)
            FdoCommonOSUtil::swprintf(curr_value, sizeof(curr_value)/sizeof(wchar_t)-1, L"%f", single_value->GetSingle());
        break;

      case FdoDataType_String:
        is_processed  = true;
        string_value  = (FdoStringValue *) literal_values->GetItem(1);
        is_NULL       = string_value->IsNull();
        if (!is_NULL)
        {
            (static_cast<FdoStringValue *> (return_data_value.p))->SetString(string_value->GetString());
            return FDO_SAFE_ADDREF(return_data_value.p);
        }
        break;

      default:
        throw FdoException::Create(
                FdoException::NLSGetMessage(
                FUNCTION_PARAMETER_DATA_TYPE_ERROR, 
                "Expression Engine: Invalid parameter data type for function '%1$ls'",
                FDO_FUNCTION_NULLVALUE));
        break;

    }  //  switch ...

    if (is_processed) {

        if (!is_NULL)
            (static_cast<FdoStringValue *> (return_data_value.p))->SetString(curr_value);
        else
          return_data_value->SetNull();

        return FDO_SAFE_ADDREF(return_data_value.p);

    }  //  if (is_processed) ...

    // The previous switch statement should have processed all valid cases
    // and hence the following part of the code should not be reached. If
    // it is reached issue an exception.

    throw FdoException::Create(
            FdoException::NLSGetMessage(
                FUNCTION_UNEXPECTED_RESULT_ERROR, 
                "Expression Engine: Unexpected result for function '%1$ls'",
                FDO_FUNCTION_NULLVALUE));
    
}  //  ProcessStringRequest ()

void FdoFunctionNullValue::Validate (FdoLiteralValueCollection *literal_values)

// +---------------------------------------------------------------------------
// | The function validates the argument list that was passed in.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    bool                    invalid_data_type = true;

    FdoInt32                i,
                            count             = literal_values->GetCount();

    FdoDataValue            *data_value       = NULL;

    FdoPtr<FdoLiteralValue> literal_value;

    // Check the number of arguments. NULLVALUE accepts two parameters. If the
    // number of parameters is not correct issue an exception.

    if (count != 2) 
        throw FdoException::Create(
                FdoException::NLSGetMessage(
                  FUNCTION_PARAMETER_NUMBER_ERROR, 
                  "Expression Engine: Invalid number of parameters for function '%1$ls'",
                  FDO_FUNCTION_NULLVALUE));

    // Navigate through the list of arguments and check whether or not the pro-
    // vided data types match the specification in the supported signatures. If
    // this is not the case issue an exception.

    for (i = 0; i < count; i++) {

      literal_value = literal_values->GetItem(i);
      if (literal_value->GetLiteralValueType() != FdoLiteralValueType_Data)
          throw FdoException::Create(
                  FdoException::NLSGetMessage(
                    FUNCTION_PARAMETER_ERROR, 
                    "Expression Engine: Invalid parameters for function '%1$ls'",
                    FDO_FUNCTION_NULLVALUE));

      data_value = static_cast<FdoDataValue *>(literal_value.p);
      if (i == 0)
          para1_data_type = data_value->GetDataType();
      else
        para2_data_type = data_value->GetDataType();

    }  //  for (i = 0; i < count; i++) ...

    switch (para1_data_type) {

      case FdoDataType_Boolean:
        invalid_data_type = (para2_data_type != FdoDataType_Boolean);
        break;

      case FdoDataType_Byte:
        invalid_data_type = (para2_data_type != FdoDataType_Byte);
        break;

      case FdoDataType_DateTime:
        invalid_data_type = (para2_data_type != FdoDataType_DateTime);
        break;

      case FdoDataType_Decimal:
      case FdoDataType_Double:
      case FdoDataType_Int16:
      case FdoDataType_Int32:
      case FdoDataType_Int64:
      case FdoDataType_Single:
        invalid_data_type = ((para2_data_type != FdoDataType_Decimal) &&
                             (para2_data_type != FdoDataType_Double ) &&
                             (para2_data_type != FdoDataType_Int16  ) &&
                             (para2_data_type != FdoDataType_Int32  ) &&
                             (para2_data_type != FdoDataType_Int64  ) &&
                             (para2_data_type != FdoDataType_Single )    );
        break;

      case FdoDataType_String:
        invalid_data_type = ((para2_data_type != FdoDataType_Decimal) &&
                             (para2_data_type != FdoDataType_Double ) &&
                             (para2_data_type != FdoDataType_Int16  ) &&
                             (para2_data_type != FdoDataType_Int32  ) &&
                             (para2_data_type != FdoDataType_Int64  ) &&
                             (para2_data_type != FdoDataType_Single ) &&
                             (para2_data_type != FdoDataType_String )    );
        break;

    }  //  switch (para1_data_type) ...

    if (invalid_data_type)
        throw FdoException::Create(
                FdoException::NLSGetMessage(
                  FUNCTION_PARAMETER_DATA_TYPE_ERROR, 
                  "Expression Engine: Invalid parameter data type for function '%1$ls'",
                  FDO_FUNCTION_NULLVALUE));

}  //  Validate ()

