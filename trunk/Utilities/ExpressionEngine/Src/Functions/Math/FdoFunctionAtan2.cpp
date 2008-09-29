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
#include <Functions/Math/FdoFunctionAtan2.h>


// ----------------------------------------------------------------------------
// --                         Constructors/Destructors                       --
// ----------------------------------------------------------------------------

FdoFunctionAtan2::FdoFunctionAtan2 ()

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

}  //  FdoFunctionAtan2 ()


FdoFunctionAtan2::~FdoFunctionAtan2 ()

// +---------------------------------------------------------------------------
// | The class destructor.
// +---------------------------------------------------------------------------

{

    // Delete the function definition.

    FDO_SAFE_RELEASE(function_definition);

}  //  ~FdoFunctionAtan2 ()


// ----------------------------------------------------------------------------
// --                            Public Class APIs                           --
// ----------------------------------------------------------------------------

FdoFunctionAtan2 *FdoFunctionAtan2::Create ()

// +---------------------------------------------------------------------------
// | The function creates a new instance of the class.
// +---------------------------------------------------------------------------

{

    return new FdoFunctionAtan2();

}  //  Create ()

FdoFunctionAtan2 *FdoFunctionAtan2::CreateObject ()

// +---------------------------------------------------------------------------
// | The function creates a new instance of the class.
// +---------------------------------------------------------------------------

{

    return new FdoFunctionAtan2();

}  //  CreateObject ()

FdoFunctionDefinition *FdoFunctionAtan2::GetFunctionDefinition ()

// +---------------------------------------------------------------------------
// | The function creates the supported signature list for the function ATAN2.
// +---------------------------------------------------------------------------

{

    if (function_definition == NULL)
        CreateFunctionDefinition();

    return FDO_SAFE_ADDREF(function_definition);

}  //  GetFunctionDefinition ()

FdoLiteralValue *FdoFunctionAtan2::Evaluate (
                                    FdoLiteralValueCollection *literal_values)

// +---------------------------------------------------------------------------
// | The function processes a call to the function ATAN2.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    bool      is_NULL_value = false;

    FdoDouble p1,
              p2;

    if (first)
    {
        Validate(literal_values);
        return_double_value = FdoDoubleValue::Create();
        first = false;
    }

    // Process the request and return the result back to the calling routine.

    p1 = GetParameterValue(literal_values, 0, para1_data_type, &is_NULL_value);
    if (is_NULL_value)
    {
        return_double_value->SetNull();
        return FDO_SAFE_ADDREF(return_double_value.p);
    }

    p2 = GetParameterValue(literal_values, 1, para2_data_type, &is_NULL_value);
    if (is_NULL_value)
    {
        return_double_value->SetNull();
        return FDO_SAFE_ADDREF(return_double_value.p);
    }

    return_double_value->SetDouble(atan2(p1, p2));
    return FDO_SAFE_ADDREF(return_double_value.p);

}  //  Evaluate ()


// ----------------------------------------------------------------------------
// --                          Supporting functions                          --
// ----------------------------------------------------------------------------

void FdoFunctionAtan2::CreateFunctionDefinition ()

// +---------------------------------------------------------------------------
// | The procedure creates the function definition for the function ATAN2. The
// | function definition includes the list of supported signatures. The follow-
// | ing signatures are supported:
// |
// |    ATAN2 ({byte, decimal, double, int16, int32, int64, single},
// |           {byte, decimal, double, int16, int32, int64, single} )
// |
// | The function always returns a DOUBLE.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoString *desc = NULL;

    FdoStringP arg1_description;
    FdoStringP num_arg_literal;

    FdoPtr<FdoArgumentDefinition> byte_arg;
    FdoPtr<FdoArgumentDefinition> dcl_arg;
    FdoPtr<FdoArgumentDefinition> dbl_arg;
    FdoPtr<FdoArgumentDefinition> int16_arg;
    FdoPtr<FdoArgumentDefinition> int32_arg;
    FdoPtr<FdoArgumentDefinition> int64_arg;
    FdoPtr<FdoArgumentDefinition> sgl_arg;

    FdoPtr<FdoArgumentDefinitionCollection> byte_byte_args;
    FdoPtr<FdoArgumentDefinitionCollection> byte_dcl_args;
    FdoPtr<FdoArgumentDefinitionCollection> byte_dbl_args;
    FdoPtr<FdoArgumentDefinitionCollection> byte_int16_args;
    FdoPtr<FdoArgumentDefinitionCollection> byte_int32_args;
    FdoPtr<FdoArgumentDefinitionCollection> byte_int64_args;
    FdoPtr<FdoArgumentDefinitionCollection> byte_sgl_args;

    FdoPtr<FdoArgumentDefinitionCollection> dcl_byte_args;
    FdoPtr<FdoArgumentDefinitionCollection> dcl_dcl_args;
    FdoPtr<FdoArgumentDefinitionCollection> dcl_dbl_args;
    FdoPtr<FdoArgumentDefinitionCollection> dcl_int16_args;
    FdoPtr<FdoArgumentDefinitionCollection> dcl_int32_args;
    FdoPtr<FdoArgumentDefinitionCollection> dcl_int64_args;
    FdoPtr<FdoArgumentDefinitionCollection> dcl_sgl_args;

    FdoPtr<FdoArgumentDefinitionCollection> dbl_byte_args;
    FdoPtr<FdoArgumentDefinitionCollection> dbl_dcl_args;
    FdoPtr<FdoArgumentDefinitionCollection> dbl_dbl_args;
    FdoPtr<FdoArgumentDefinitionCollection> dbl_int16_args;
    FdoPtr<FdoArgumentDefinitionCollection> dbl_int32_args;
    FdoPtr<FdoArgumentDefinitionCollection> dbl_int64_args;
    FdoPtr<FdoArgumentDefinitionCollection> dbl_sgl_args;

    FdoPtr<FdoArgumentDefinitionCollection> int16_byte_args;
    FdoPtr<FdoArgumentDefinitionCollection> int16_dcl_args;
    FdoPtr<FdoArgumentDefinitionCollection> int16_dbl_args;
    FdoPtr<FdoArgumentDefinitionCollection> int16_int16_args;
    FdoPtr<FdoArgumentDefinitionCollection> int16_int32_args;
    FdoPtr<FdoArgumentDefinitionCollection> int16_int64_args;
    FdoPtr<FdoArgumentDefinitionCollection> int16_sgl_args;

    FdoPtr<FdoArgumentDefinitionCollection> int32_byte_args;
    FdoPtr<FdoArgumentDefinitionCollection> int32_dcl_args;
    FdoPtr<FdoArgumentDefinitionCollection> int32_dbl_args;
    FdoPtr<FdoArgumentDefinitionCollection> int32_int16_args;
    FdoPtr<FdoArgumentDefinitionCollection> int32_int32_args;
    FdoPtr<FdoArgumentDefinitionCollection> int32_int64_args;
    FdoPtr<FdoArgumentDefinitionCollection> int32_sgl_args;

    FdoPtr<FdoArgumentDefinitionCollection> int64_byte_args;
    FdoPtr<FdoArgumentDefinitionCollection> int64_dcl_args;
    FdoPtr<FdoArgumentDefinitionCollection> int64_dbl_args;
    FdoPtr<FdoArgumentDefinitionCollection> int64_int16_args;
    FdoPtr<FdoArgumentDefinitionCollection> int64_int32_args;
    FdoPtr<FdoArgumentDefinitionCollection> int64_int64_args;
    FdoPtr<FdoArgumentDefinitionCollection> int64_sgl_args;

    FdoPtr<FdoArgumentDefinitionCollection> sgl_byte_args;
    FdoPtr<FdoArgumentDefinitionCollection> sgl_dcl_args;
    FdoPtr<FdoArgumentDefinitionCollection> sgl_dbl_args;
    FdoPtr<FdoArgumentDefinitionCollection> sgl_int16_args;
    FdoPtr<FdoArgumentDefinitionCollection> sgl_int32_args;
    FdoPtr<FdoArgumentDefinitionCollection> sgl_int64_args;
    FdoPtr<FdoArgumentDefinitionCollection> sgl_sgl_args;

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

    byte_byte_args = FdoArgumentDefinitionCollection::Create();
    byte_byte_args->Add(byte_arg);
    byte_byte_args->Add(byte_arg);

    byte_dcl_args = FdoArgumentDefinitionCollection::Create();
    byte_dcl_args->Add(byte_arg);
    byte_dcl_args->Add(dcl_arg);

    byte_dbl_args = FdoArgumentDefinitionCollection::Create();
    byte_dbl_args->Add(byte_arg);
    byte_dbl_args->Add(dbl_arg);

    byte_int16_args = FdoArgumentDefinitionCollection::Create();
    byte_int16_args->Add(byte_arg);
    byte_int16_args->Add(int16_arg);

    byte_int32_args = FdoArgumentDefinitionCollection::Create();
    byte_int32_args->Add(byte_arg);
    byte_int32_args->Add(int32_arg);

    byte_int64_args = FdoArgumentDefinitionCollection::Create();
    byte_int64_args->Add(byte_arg);
    byte_int64_args->Add(int64_arg);

    byte_sgl_args = FdoArgumentDefinitionCollection::Create();
    byte_sgl_args->Add(byte_arg);
    byte_sgl_args->Add(sgl_arg);

    dcl_byte_args = FdoArgumentDefinitionCollection::Create();
    dcl_byte_args->Add(dcl_arg);
    dcl_byte_args->Add(byte_arg);

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

    dbl_byte_args = FdoArgumentDefinitionCollection::Create();
    dbl_byte_args->Add(dbl_arg);
    dbl_byte_args->Add(byte_arg);

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

    int16_byte_args = FdoArgumentDefinitionCollection::Create();
    int16_byte_args->Add(int16_arg);
    int16_byte_args->Add(byte_arg);

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

    int32_byte_args = FdoArgumentDefinitionCollection::Create();
    int32_byte_args->Add(int32_arg);
    int32_byte_args->Add(byte_arg);

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

    int64_byte_args = FdoArgumentDefinitionCollection::Create();
    int64_byte_args->Add(int64_arg);
    int64_byte_args->Add(byte_arg);

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

    sgl_byte_args = FdoArgumentDefinitionCollection::Create();
    sgl_byte_args->Add(sgl_arg);
    sgl_byte_args->Add(byte_arg);

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

    // Create the signature collection.

    signatures = FdoSignatureDefinitionCollection::Create();

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Double, byte_byte_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Double, byte_dcl_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Double, byte_dbl_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Double, byte_int16_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Double, byte_int32_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Double, byte_int64_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Double, byte_sgl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Double, dcl_byte_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Double, dcl_dcl_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Double, dcl_dbl_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Double, dcl_int16_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Double, dcl_int32_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Double, dcl_int64_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Double, dcl_sgl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Double, dbl_byte_args);
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
                                        FdoDataType_Double, int16_byte_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Double, int16_dcl_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Double, int16_dbl_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Double, int16_int16_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Double, int16_int32_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Double, int16_int64_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Double, int16_sgl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Double, int32_byte_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Double, int32_dcl_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Double, int32_dbl_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Double, int32_int16_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Double, int32_int32_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Double, int32_int64_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Double, int32_sgl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Double, int64_byte_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Double, int64_dcl_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Double, int64_dbl_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Double, int64_int16_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Double, int64_int32_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Double, int64_int64_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Double, int64_sgl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Double, sgl_byte_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Double, sgl_dcl_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Double, sgl_dbl_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Double, sgl_int16_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Double, sgl_int32_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Double, sgl_int64_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Double, sgl_sgl_args);
    signatures->Add(signature);

    // Create the function definition.

    desc =
        FdoException::NLSGetMessage(
                    FUNCTION_ATAN2,
                    "Determines the arc tangent of two numeric expressions");
    function_definition =
                FdoFunctionDefinition::Create(
                                        FDO_FUNCTION_ATAN2,
                                        desc,
                                        false,
                                        signatures,
                                        FdoFunctionCategoryType_Math);

}  //  CreateFunctionDefinition ()

FdoDouble FdoFunctionAtan2::GetParameterValue (
                                FdoLiteralValueCollection *literal_values,
                                FdoInt32                  parameter_count,
                                FdoDataType               data_type,
                                bool                      *is_NULL_value)

// +---------------------------------------------------------------------------
// | The function returns the value for the provided parameter.
// +---------------------------------------------------------------------------

{

    // Declare all necessary local variables and initialize them.

    FdoPtr<FdoByteValue>    byte_value;
    FdoPtr<FdoDecimalValue> decimal_value;
    FdoPtr<FdoDoubleValue>  double_value;
    FdoPtr<FdoInt16Value>   int16_value;
    FdoPtr<FdoInt32Value>   int32_value;
    FdoPtr<FdoInt64Value>   int64_value;
    FdoPtr<FdoSingleValue>  single_value;

    // Process the identified parameter and return its value back to the
    // calling routine.

    *is_NULL_value = false;

    switch (data_type) {

      case FdoDataType_Byte:
        byte_value =(FdoByteValue *) literal_values->GetItem(
                                                            parameter_count);
        if (!byte_value->IsNull())
            return (FdoDouble) byte_value->GetByte();
        else {

          *is_NULL_value = true;
          return 0;

        }  //  else ...
        break;

      case FdoDataType_Decimal:
        decimal_value =(FdoDecimalValue *) literal_values->GetItem(
                                                            parameter_count);
        if (!decimal_value->IsNull())
            return decimal_value->GetDecimal();
        else {

          *is_NULL_value = true;
          return 0;

        }  //  else ...
        break;

      case FdoDataType_Double:
        double_value = (FdoDoubleValue *) literal_values->GetItem(
                                                            parameter_count);
        if (!double_value->IsNull())
            return double_value->GetDouble();
        else {

          *is_NULL_value = true;
          return 0;

        }  //  else ...
        break;

      case FdoDataType_Int16:
        int16_value = (FdoInt16Value *) literal_values->GetItem(
                                                            parameter_count);
        if (!int16_value->IsNull())
            return (FdoDouble) int16_value->GetInt16();
        else {

          *is_NULL_value = true;
          return 0;

        }  //  else ...
        break;

      case FdoDataType_Int32:
        int32_value = (FdoInt32Value *) literal_values->GetItem(
                                                            parameter_count);
        if (!int32_value->IsNull())
            return (FdoDouble) int32_value->GetInt32();
        else {

          *is_NULL_value = true;
          return 0;

        }  //  else ...
        break;

      case FdoDataType_Int64:
        int64_value = (FdoInt64Value *) literal_values->GetItem(
                                                            parameter_count);
        if (!int64_value->IsNull())
            return (FdoDouble) int64_value->GetInt64();
        else {

          *is_NULL_value = true;
          return 0;

        }  //  else ...
        break;

      case FdoDataType_Single:
        single_value = (FdoSingleValue *) literal_values->GetItem(
                                                            parameter_count);
        if (!single_value->IsNull())
            return single_value->GetSingle();
        else {

          *is_NULL_value = true;
          return 0;

        }  //  else ...
        break;

    }  //  switch ...

    // The switch statement should handle all valid scenarios. Therefore, the
    // above statements should have returned valid information to the calling
    // routine. Just in case there is an issue, the following exception is
    // thrown if this part of the function implementation is reached.

    throw FdoException::Create(
            FdoException::NLSGetMessage(
              FUNCTION_UNEXPECTED_RESULT_ERROR, 
              "Expression Engine: Unexpected result for function '%1$ls'",
              FDO_FUNCTION_ATAN2));

}  //  GetParameterValue ()

void FdoFunctionAtan2::Validate (FdoLiteralValueCollection *literal_values)

// +---------------------------------------------------------------------------
// | The function validates the argument list that was passed in.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoInt32                i               = 0,
                            count           = literal_values->GetCount();

    FdoDataValue            *data_value     = NULL;

    FdoPtr<FdoLiteralValue> literal_value;

    // Check the number of arguments. ATAN2 accepts two numeric parameters.
    // If the number of parameters is not correct issue an exception.

    if (count != 2) 
        throw FdoException::Create(
                FdoException::NLSGetMessage(
                  FUNCTION_PARAMETER_NUMBER_ERROR, 
                  "Expression Engine: Invalid number of parameters for function '%1$ls'",
                  FDO_FUNCTION_ATAN2));

    // Next, identify the data type associated with the value to be processed.
    // An exception is issued if the data type does not match any of the ones
    // the function supports

    for (i = 0; i < count; i++) {

      literal_value = literal_values->GetItem(i);
      if (literal_value->GetLiteralValueType() != FdoLiteralValueType_Data)
          throw FdoException::Create(
                  FdoException::NLSGetMessage(
                      FUNCTION_PARAMETER_ERROR, 
                      "Expression Engine: Invalid parameters for function '%1$ls'",
                      FDO_FUNCTION_ATAN2));

      data_value = static_cast<FdoDataValue *>(literal_value.p);
      if (i == 0)
          para1_data_type = data_value->GetDataType();
      else
        para2_data_type = data_value->GetDataType();

    }  //  for (i = 0; i < count; i++) ...

    if (((para1_data_type != FdoDataType_Byte   ) &&
         (para1_data_type != FdoDataType_Decimal) &&
         (para1_data_type != FdoDataType_Double ) &&
         (para1_data_type != FdoDataType_Int16  ) &&
         (para1_data_type != FdoDataType_Int32  ) &&
         (para1_data_type != FdoDataType_Int64  ) &&
         (para1_data_type != FdoDataType_Single )    ) ||
        ((para2_data_type != FdoDataType_Byte   ) &&
         (para2_data_type != FdoDataType_Decimal) &&
         (para2_data_type != FdoDataType_Double ) &&
         (para2_data_type != FdoDataType_Int16  ) &&
         (para2_data_type != FdoDataType_Int32  ) &&
         (para2_data_type != FdoDataType_Int64  ) &&
         (para2_data_type != FdoDataType_Single )    )    )
        throw FdoException::Create(
                FdoException::NLSGetMessage(
                  FUNCTION_PARAMETER_DATA_TYPE_ERROR, 
                  "Expression Engine: Invalid parameter data type for function '%1$ls'",
                  FDO_FUNCTION_ATAN2));

}  //  Validate ()

