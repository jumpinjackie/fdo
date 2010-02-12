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
#include <Functions/String/FdoFunctionConcat.h>
#include "../../../../../Fdo/Unmanaged/Src/Common/StringUtility.h"


// ----------------------------------------------------------------------------
// --                         Constructors/Destructors                       --
// ----------------------------------------------------------------------------

FdoFunctionConcat::FdoFunctionConcat ()

// +---------------------------------------------------------------------------
// | The class constructor.
// +---------------------------------------------------------------------------

{

    // Initialize all class variables.

    function_definition = NULL;
    tmp_buffer          = NULL;
    arg_values          = NULL;
	dt_index            = NULL;
	size                = NULL;
    buffer              = NULL;

    is_validated        = false;

}  //  FdoFunctionConcat ()

FdoFunctionConcat::~FdoFunctionConcat ()

// +---------------------------------------------------------------------------
// | The class destructor.
// +---------------------------------------------------------------------------

{

    // Delete the function definition.

    FDO_SAFE_RELEASE(function_definition);

    delete[]    tmp_buffer;
	delete[]	arg_values;
	delete[]	dt_index;
	delete[]	size;
    delete[]    buffer;

}  //  ~FdoFunctionConcat ()


// ----------------------------------------------------------------------------
// --                            Public Class APIs                           --
// ----------------------------------------------------------------------------

FdoFunctionConcat *FdoFunctionConcat::Create ()

// +---------------------------------------------------------------------------
// | The function creates a new instance of the class.
// +---------------------------------------------------------------------------

{

    return new FdoFunctionConcat();

}  //  Create ()

FdoFunctionConcat *FdoFunctionConcat::CreateObject ()

// +---------------------------------------------------------------------------
// | The function creates a new instance of the class.
// +---------------------------------------------------------------------------

{

    return new FdoFunctionConcat();

}  //  CreateObject ()

FdoFunctionDefinition *FdoFunctionConcat::GetFunctionDefinition ()

// +---------------------------------------------------------------------------
// | The function creates the supported signature list for the function CONCAT.
// +---------------------------------------------------------------------------

{

    if (function_definition == NULL)
        CreateFunctionDefinition();

    return FDO_SAFE_ADDREF(function_definition);

}  //  GetFunctionDefinition ()

FdoLiteralValue *FdoFunctionConcat::Evaluate (
                                    FdoLiteralValueCollection *literal_values)

// +---------------------------------------------------------------------------
// | The function processes a call to the function CONCAT.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    size_t                   exp_buffer_size = 0;
    FdoDataType              curr_data_type;

    FdoPtr<FdoBooleanValue>  bool_value;
    FdoPtr<FdoByteValue>     byte_value;
    FdoPtr<FdoDateTimeValue> dt_value;
    FdoPtr<FdoDecimalValue>  decimal_value;
    FdoPtr<FdoDoubleValue>   double_value;
    FdoPtr<FdoInt16Value>    int16_value;
    FdoPtr<FdoInt32Value>    int32_value;
    FdoPtr<FdoInt64Value>    int64_value;
    FdoPtr<FdoSingleValue>   single_value;
    FdoPtr<FdoStringValue>   string_value;

    // If this is the first call to this function validate the arguments and
    // initialize some member variables used to process the request.

    if (!is_validated) {

        Validate(literal_values);
        result          = FdoStringValue::Create();
        tmp_buffer      = new wchar_t[INIT_ALLOCATE_SIZE+1];
        tmp_buffer_size = INIT_ALLOCATE_SIZE;
        numArgsMax      = 0;
    }  
    
    // Allocate buffers
    numArgs = literal_values->GetCount();
    if (!is_validated || (numArgs > numArgsMax))
    {
        // Reallocate the buffers
        if (is_validated)
        {    	
            delete[]	arg_values;
            delete[]	dt_index;
            delete[]	size;
            delete[]    buffer;
        }

   	    buffer          = new wchar_t[numArgs * BUFFER_SIZE];
	    arg_values      = new FdoString*[numArgs];
	    dt_index        = new FdoInt32[numArgs];
	    size            = new size_t[numArgs];
        numArgsMax      = numArgs; // save the current buffers size
        is_validated    = true;
    }

    // Initialize
	for (size_t i = 0; i < numArgs; i++)
	{
		arg_values[i] = NULL;
		dt_index[i]   = -1;
	}

    // Navigate through the provided arguments, transform the values into the
    // corresponding string results and collect them for further processing.

    for (size_t i = 0; i < numArgs; i++) {
      size[i] = 0;
	  FdoPtr<FdoLiteralValue> literal_value = literal_values->GetItem(i);
	  FdoDataValue *data_value = static_cast<FdoDataValue *>(literal_value.p);
      curr_data_type = data_value->GetDataType(); 

      switch (curr_data_type) {

        case FdoDataType_Boolean:
          bool_value = (FdoBooleanValue *) literal_values->GetItem(i);
          if (!bool_value->IsNull())
          {
              if (bool_value->GetBoolean())
                arg_values[i] = L"1";
              else
                arg_values[i] = L"0";
          }

          break;

        case FdoDataType_Byte:
          byte_value = (FdoByteValue *) literal_values->GetItem(i);
          if (!byte_value->IsNull())
          {
#ifdef _WIN32
            _itow(byte_value->GetByte(), &buffer[i * BUFFER_SIZE], 10);
#else
	        swprintf(&buffer[i * BUFFER_SIZE], BUFFER_SIZE, L"%d", (FdoInt32)byte_value->GetByte());
#endif	
            arg_values[i] = &buffer[i * BUFFER_SIZE];
          }

          break;

        case FdoDataType_DateTime:
          dt_value = (FdoDateTimeValue *) literal_values->GetItem(i);
          if (!dt_value->IsNull()) {

              arg_values[i] = (wchar_t *)ProcessArgument(dt_value);
              dt_index[i]   = 1;

          }  //  if (!dt_value->IsNull()) ...
          break;

        case FdoDataType_Decimal:
          decimal_value = (FdoDecimalValue *) literal_values->GetItem(i);
          if (!decimal_value->IsNull())
          {
              FdoStringUtility::FormatDouble(decimal_value->GetDecimal(), &buffer[i * BUFFER_SIZE], BUFFER_SIZE);
              arg_values[i] = &buffer[i * BUFFER_SIZE];
          }

          break;

        case FdoDataType_Double:
          double_value = (FdoDoubleValue *) literal_values->GetItem(i);
          if (!double_value->IsNull())
          {
              FdoStringUtility::FormatDouble(double_value->GetDouble(), &buffer[i * BUFFER_SIZE], BUFFER_SIZE);
              arg_values[i] = &buffer[i * BUFFER_SIZE];
          }

          break;

        case FdoDataType_Int16:
          int16_value = (FdoInt16Value *) literal_values->GetItem(i);
          if (!int16_value->IsNull())
          {
#ifdef _WIN32
            _itow(int16_value->GetInt16(), &buffer[i * BUFFER_SIZE], 10);
#else
	        swprintf(&buffer[i * BUFFER_SIZE], BUFFER_SIZE, L"%d", (FdoInt32)int16_value->GetInt16());
#endif	
            arg_values[i] = &buffer[i * BUFFER_SIZE];

          }
          break;

        case FdoDataType_Int32:
          int32_value = (FdoInt32Value *) literal_values->GetItem(i);
          if (!int32_value->IsNull())
          {
#ifdef _WIN32
            _itow(int32_value->GetInt32(), &buffer[i * BUFFER_SIZE], 10);
#else
	        swprintf(&buffer[i * BUFFER_SIZE], BUFFER_SIZE, L"%d", int32_value->GetInt32());
#endif
            arg_values[i] = &buffer[i * BUFFER_SIZE];
          }
          break;

        case FdoDataType_Int64:
          int64_value = (FdoInt64Value *) literal_values->GetItem(i);
          if (!int64_value->IsNull())
          {

#ifdef _WIN32
                _i64tow(int64_value->GetInt64(), &buffer[i * BUFFER_SIZE], 10);
#else
	            swprintf(&buffer[i * BUFFER_SIZE], BUFFER_SIZE, L"%lld", int64_value->GetInt64());
#endif
                arg_values[i] = &buffer[i * BUFFER_SIZE];
          }
          break;

        case FdoDataType_Single:
          single_value = (FdoSingleValue *) literal_values->GetItem(i);
          if (!single_value->IsNull())
          {
            FdoStringUtility::FormatSingle(single_value->GetSingle(), &buffer[i * BUFFER_SIZE], BUFFER_SIZE);
            arg_values[i] = &buffer[i * BUFFER_SIZE];
          }
          break;

        case FdoDataType_String:
          string_value = (FdoStringValue *) literal_values->GetItem(i);
          if (!string_value->IsNull())
              arg_values[i] = string_value->GetString();
          break;

      }  //  switch (curr_data_type) ...

      if (arg_values[i] != NULL)
      {
          size[i] = wcslen(arg_values[i]);
          exp_buffer_size += size[i];
      }

    }  //  for (i = 0; i < numArgs; i++) ...

    if (exp_buffer_size > tmp_buffer_size) {

        delete [] tmp_buffer;
        tmp_buffer_size = exp_buffer_size;
        tmp_buffer      = new wchar_t[tmp_buffer_size + 1];

    }  //  if (exp_buffer_size > tmp_buffer_size) ...

    tmp_buffer[0] = '\0';

    size_t index = 0;
    for (size_t i = 0; i < numArgs; i++) {
      if (arg_values[i] != NULL) {

          if (dt_index[i] != -1)
          {
              memcpy(tmp_buffer+index, arg_values[i], (size[i] - 1) *sizeof(wchar_t));
              index = index + size[i] - 1;
          }
          else
          {
              memcpy(tmp_buffer+index, arg_values[i], size[i]*sizeof(wchar_t));
              index += size[i];
          }

      }  //  if (arg_values[0] != NULL) ...
    }  //  for (i = 0; i < numArgs; i++) 

    tmp_buffer[index] = '\0';
    result->SetString(tmp_buffer);

    return FDO_SAFE_ADDREF(result.p);

}  //  Evaluate ()


// ----------------------------------------------------------------------------
// --                          Supporting functions                          --
// ----------------------------------------------------------------------------

void FdoFunctionConcat::CreateFunctionDefinition ()

// +---------------------------------------------------------------------------
// | The procedure creates the function definition for the function CONCAT. The
// | function definition includes the list of supported signatures. The follow-
// | ing signatures are supported:
// |
// |    CONCAT ({bool, date_time, byte, decimal, double, int16, int32, int64,
// |             single, string}, {bool, date_time, byte, decimal, double,
// |             int16, int32, int64, single, string})
// |
// | The function always returns a STRING.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoString                               *desc               = NULL;

    FdoStringP                              arg1_description;
    FdoStringP                              arg2_description;
    FdoStringP                              bool_arg_literal;
    FdoStringP                              dt_arg_literal;
    FdoStringP                              num_arg_literal;
    FdoStringP                              str_arg_literal;

    FdoPtr<FdoArgumentDefinition>           bool_1_arg;
    FdoPtr<FdoArgumentDefinition>           bool_2_arg;
    FdoPtr<FdoArgumentDefinition>           byte_1_arg;
    FdoPtr<FdoArgumentDefinition>           byte_2_arg;
    FdoPtr<FdoArgumentDefinition>           dt_1_arg;
    FdoPtr<FdoArgumentDefinition>           dt_2_arg;
    FdoPtr<FdoArgumentDefinition>           dcl_1_arg;
    FdoPtr<FdoArgumentDefinition>           dcl_2_arg;
    FdoPtr<FdoArgumentDefinition>           dbl_1_arg;
    FdoPtr<FdoArgumentDefinition>           dbl_2_arg;
    FdoPtr<FdoArgumentDefinition>           int16_1_arg;
    FdoPtr<FdoArgumentDefinition>           int16_2_arg;
    FdoPtr<FdoArgumentDefinition>           int32_1_arg;
    FdoPtr<FdoArgumentDefinition>           int32_2_arg;
    FdoPtr<FdoArgumentDefinition>           int64_1_arg;
    FdoPtr<FdoArgumentDefinition>           int64_2_arg;
    FdoPtr<FdoArgumentDefinition>           sgl_1_arg;
    FdoPtr<FdoArgumentDefinition>           sgl_2_arg;
    FdoPtr<FdoArgumentDefinition>           str_1_arg;
    FdoPtr<FdoArgumentDefinition>           str_2_arg;

    FdoPtr<FdoArgumentDefinitionCollection> bool_bool_args;
    FdoPtr<FdoArgumentDefinitionCollection> bool_byte_args;
    FdoPtr<FdoArgumentDefinitionCollection> bool_dt_args;
    FdoPtr<FdoArgumentDefinitionCollection> bool_dcl_args;
    FdoPtr<FdoArgumentDefinitionCollection> bool_dbl_args;
    FdoPtr<FdoArgumentDefinitionCollection> bool_int16_args;
    FdoPtr<FdoArgumentDefinitionCollection> bool_int32_args;
    FdoPtr<FdoArgumentDefinitionCollection> bool_int64_args;
    FdoPtr<FdoArgumentDefinitionCollection> bool_sgl_args;
    FdoPtr<FdoArgumentDefinitionCollection> bool_str_args;

    FdoPtr<FdoArgumentDefinitionCollection> byte_bool_args;
    FdoPtr<FdoArgumentDefinitionCollection> byte_byte_args;
    FdoPtr<FdoArgumentDefinitionCollection> byte_dt_args;
    FdoPtr<FdoArgumentDefinitionCollection> byte_dcl_args;
    FdoPtr<FdoArgumentDefinitionCollection> byte_dbl_args;
    FdoPtr<FdoArgumentDefinitionCollection> byte_int16_args;
    FdoPtr<FdoArgumentDefinitionCollection> byte_int32_args;
    FdoPtr<FdoArgumentDefinitionCollection> byte_int64_args;
    FdoPtr<FdoArgumentDefinitionCollection> byte_sgl_args;
    FdoPtr<FdoArgumentDefinitionCollection> byte_str_args;

    FdoPtr<FdoArgumentDefinitionCollection> dt_bool_args;
    FdoPtr<FdoArgumentDefinitionCollection> dt_byte_args;
    FdoPtr<FdoArgumentDefinitionCollection> dt_dt_args;
    FdoPtr<FdoArgumentDefinitionCollection> dt_dcl_args;
    FdoPtr<FdoArgumentDefinitionCollection> dt_dbl_args;
    FdoPtr<FdoArgumentDefinitionCollection> dt_int16_args;
    FdoPtr<FdoArgumentDefinitionCollection> dt_int32_args;
    FdoPtr<FdoArgumentDefinitionCollection> dt_int64_args;
    FdoPtr<FdoArgumentDefinitionCollection> dt_sgl_args;
    FdoPtr<FdoArgumentDefinitionCollection> dt_str_args;

    FdoPtr<FdoArgumentDefinitionCollection> dcl_bool_args;
    FdoPtr<FdoArgumentDefinitionCollection> dcl_byte_args;
    FdoPtr<FdoArgumentDefinitionCollection> dcl_dt_args;
    FdoPtr<FdoArgumentDefinitionCollection> dcl_dcl_args;
    FdoPtr<FdoArgumentDefinitionCollection> dcl_dbl_args;
    FdoPtr<FdoArgumentDefinitionCollection> dcl_int16_args;
    FdoPtr<FdoArgumentDefinitionCollection> dcl_int32_args;
    FdoPtr<FdoArgumentDefinitionCollection> dcl_int64_args;
    FdoPtr<FdoArgumentDefinitionCollection> dcl_sgl_args;
    FdoPtr<FdoArgumentDefinitionCollection> dcl_str_args;

    FdoPtr<FdoArgumentDefinitionCollection> dbl_bool_args;
    FdoPtr<FdoArgumentDefinitionCollection> dbl_byte_args;
    FdoPtr<FdoArgumentDefinitionCollection> dbl_dt_args;
    FdoPtr<FdoArgumentDefinitionCollection> dbl_dcl_args;
    FdoPtr<FdoArgumentDefinitionCollection> dbl_dbl_args;
    FdoPtr<FdoArgumentDefinitionCollection> dbl_int16_args;
    FdoPtr<FdoArgumentDefinitionCollection> dbl_int32_args;
    FdoPtr<FdoArgumentDefinitionCollection> dbl_int64_args;
    FdoPtr<FdoArgumentDefinitionCollection> dbl_sgl_args;
    FdoPtr<FdoArgumentDefinitionCollection> dbl_str_args;

    FdoPtr<FdoArgumentDefinitionCollection> int16_bool_args;
    FdoPtr<FdoArgumentDefinitionCollection> int16_byte_args;
    FdoPtr<FdoArgumentDefinitionCollection> int16_dt_args;
    FdoPtr<FdoArgumentDefinitionCollection> int16_dcl_args;
    FdoPtr<FdoArgumentDefinitionCollection> int16_dbl_args;
    FdoPtr<FdoArgumentDefinitionCollection> int16_int16_args;
    FdoPtr<FdoArgumentDefinitionCollection> int16_int32_args;
    FdoPtr<FdoArgumentDefinitionCollection> int16_int64_args;
    FdoPtr<FdoArgumentDefinitionCollection> int16_sgl_args;
    FdoPtr<FdoArgumentDefinitionCollection> int16_str_args;

    FdoPtr<FdoArgumentDefinitionCollection> int32_bool_args;
    FdoPtr<FdoArgumentDefinitionCollection> int32_byte_args;
    FdoPtr<FdoArgumentDefinitionCollection> int32_dt_args;
    FdoPtr<FdoArgumentDefinitionCollection> int32_dcl_args;
    FdoPtr<FdoArgumentDefinitionCollection> int32_dbl_args;
    FdoPtr<FdoArgumentDefinitionCollection> int32_int16_args;
    FdoPtr<FdoArgumentDefinitionCollection> int32_int32_args;
    FdoPtr<FdoArgumentDefinitionCollection> int32_int64_args;
    FdoPtr<FdoArgumentDefinitionCollection> int32_sgl_args;
    FdoPtr<FdoArgumentDefinitionCollection> int32_str_args;

    FdoPtr<FdoArgumentDefinitionCollection> int64_bool_args;
    FdoPtr<FdoArgumentDefinitionCollection> int64_byte_args;
    FdoPtr<FdoArgumentDefinitionCollection> int64_dt_args;
    FdoPtr<FdoArgumentDefinitionCollection> int64_dcl_args;
    FdoPtr<FdoArgumentDefinitionCollection> int64_dbl_args;
    FdoPtr<FdoArgumentDefinitionCollection> int64_int16_args;
    FdoPtr<FdoArgumentDefinitionCollection> int64_int32_args;
    FdoPtr<FdoArgumentDefinitionCollection> int64_int64_args;
    FdoPtr<FdoArgumentDefinitionCollection> int64_sgl_args;
    FdoPtr<FdoArgumentDefinitionCollection> int64_str_args;

    FdoPtr<FdoArgumentDefinitionCollection> sgl_bool_args;
    FdoPtr<FdoArgumentDefinitionCollection> sgl_byte_args;
    FdoPtr<FdoArgumentDefinitionCollection> sgl_dt_args;
    FdoPtr<FdoArgumentDefinitionCollection> sgl_dcl_args;
    FdoPtr<FdoArgumentDefinitionCollection> sgl_dbl_args;
    FdoPtr<FdoArgumentDefinitionCollection> sgl_int16_args;
    FdoPtr<FdoArgumentDefinitionCollection> sgl_int32_args;
    FdoPtr<FdoArgumentDefinitionCollection> sgl_int64_args;
    FdoPtr<FdoArgumentDefinitionCollection> sgl_sgl_args;
    FdoPtr<FdoArgumentDefinitionCollection> sgl_str_args;

    FdoPtr<FdoArgumentDefinitionCollection> str_bool_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_byte_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_dt_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_dcl_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_dbl_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_int16_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_int32_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_int64_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_sgl_args;
    FdoPtr<FdoArgumentDefinitionCollection> str_str_args;

    FdoPtr<FdoSignatureDefinition>          signature;
    FdoPtr<FdoSignatureDefinitionCollection> signatures;

    // Get the general descriptions for the arguments.

    arg1_description = FdoException::NLSGetMessage(
                                            FUNCTION_CONCAT_STRING1_ARG,
                                            "First value to concatenate");
    arg2_description = FdoException::NLSGetMessage(
                                            FUNCTION_CONCAT_STRING2_ARG,
                                            "Second value to concatenate");
    bool_arg_literal = FdoException::NLSGetMessage(
                                            FUNCTION_BOOL_ARG_LIT,
                                            "boolean");
    dt_arg_literal   = FdoException::NLSGetMessage(
                                            FUNCTION_DATE_ARG_LIT,
                                            "date");
    num_arg_literal  = FdoException::NLSGetMessage(
                                            FUNCTION_NUMBER_ARG_LIT,
                                            "number");
    str_arg_literal  = FdoException::NLSGetMessage(
                                            FUNCTION_STRING_ARG_LIT,
                                            "text property");

    // The following defines the different arguments.

    bool_1_arg  = FdoArgumentDefinition::Create(
                    bool_arg_literal, arg1_description, FdoDataType_Boolean);
    bool_2_arg  = FdoArgumentDefinition::Create(
                    bool_arg_literal, arg2_description, FdoDataType_Boolean);
    dt_1_arg    = FdoArgumentDefinition::Create(
                    dt_arg_literal, arg1_description, FdoDataType_DateTime);
    dt_2_arg    = FdoArgumentDefinition::Create(
                    dt_arg_literal, arg2_description, FdoDataType_DateTime);
    byte_1_arg  = FdoArgumentDefinition::Create(
                    num_arg_literal, arg1_description, FdoDataType_Byte);
    byte_2_arg  = FdoArgumentDefinition::Create(
                    num_arg_literal, arg2_description, FdoDataType_Byte);
    dcl_1_arg   = FdoArgumentDefinition::Create(
                    num_arg_literal, arg1_description, FdoDataType_Decimal);
    dcl_2_arg   = FdoArgumentDefinition::Create(
                    num_arg_literal, arg2_description, FdoDataType_Decimal);
    dbl_1_arg   = FdoArgumentDefinition::Create(
                    num_arg_literal, arg1_description, FdoDataType_Double);
    dbl_2_arg   = FdoArgumentDefinition::Create(
                    num_arg_literal, arg2_description, FdoDataType_Double);
    int16_1_arg = FdoArgumentDefinition::Create(
                    num_arg_literal, arg1_description, FdoDataType_Int16);
    int16_2_arg = FdoArgumentDefinition::Create(
                    num_arg_literal, arg2_description, FdoDataType_Int16);
    int32_1_arg = FdoArgumentDefinition::Create(
                    num_arg_literal, arg1_description, FdoDataType_Int32);
    int32_2_arg = FdoArgumentDefinition::Create(
                    num_arg_literal, arg2_description, FdoDataType_Int32);
    int64_1_arg = FdoArgumentDefinition::Create(
                    num_arg_literal, arg1_description, FdoDataType_Int64);
    int64_2_arg = FdoArgumentDefinition::Create(
                    num_arg_literal, arg2_description, FdoDataType_Int64);
    sgl_1_arg   = FdoArgumentDefinition::Create(
                    num_arg_literal, arg1_description, FdoDataType_Single);
    sgl_2_arg   = FdoArgumentDefinition::Create(
                    num_arg_literal, arg2_description, FdoDataType_Single);
    str_1_arg   = FdoArgumentDefinition::Create(
                    str_arg_literal, arg1_description, FdoDataType_String);
    str_2_arg   = FdoArgumentDefinition::Create(
                    str_arg_literal, arg2_description, FdoDataType_String);

    // The following defines the different argument collections.

    bool_bool_args = FdoArgumentDefinitionCollection::Create();
    bool_bool_args->Add(bool_1_arg);
    bool_bool_args->Add(bool_2_arg);

    bool_byte_args = FdoArgumentDefinitionCollection::Create();
    bool_byte_args->Add(bool_1_arg);
    bool_byte_args->Add(byte_2_arg);

    bool_dt_args = FdoArgumentDefinitionCollection::Create();
    bool_dt_args->Add(bool_1_arg);
    bool_dt_args->Add(dt_2_arg);

    bool_dcl_args = FdoArgumentDefinitionCollection::Create();
    bool_dcl_args->Add(bool_1_arg);
    bool_dcl_args->Add(dcl_2_arg);

    bool_dbl_args = FdoArgumentDefinitionCollection::Create();
    bool_dbl_args->Add(bool_1_arg);
    bool_dbl_args->Add(dbl_2_arg);

    bool_int16_args = FdoArgumentDefinitionCollection::Create();
    bool_int16_args->Add(bool_1_arg);
    bool_int16_args->Add(int16_2_arg);

    bool_int32_args = FdoArgumentDefinitionCollection::Create();
    bool_int32_args->Add(bool_1_arg);
    bool_int32_args->Add(int32_2_arg);

    bool_int64_args = FdoArgumentDefinitionCollection::Create();
    bool_int64_args->Add(bool_1_arg);
    bool_int64_args->Add(int64_2_arg);

    bool_sgl_args = FdoArgumentDefinitionCollection::Create();
    bool_sgl_args->Add(bool_1_arg);
    bool_sgl_args->Add(sgl_2_arg);

    bool_str_args = FdoArgumentDefinitionCollection::Create();
    bool_str_args->Add(bool_1_arg);
    bool_str_args->Add(str_2_arg);

    byte_bool_args = FdoArgumentDefinitionCollection::Create();
    byte_bool_args->Add(byte_1_arg);
    byte_bool_args->Add(bool_2_arg);

    byte_byte_args = FdoArgumentDefinitionCollection::Create();
    byte_byte_args->Add(byte_1_arg);
    byte_byte_args->Add(byte_2_arg);

    byte_dt_args = FdoArgumentDefinitionCollection::Create();
    byte_dt_args->Add(byte_1_arg);
    byte_dt_args->Add(dt_2_arg);

    byte_dcl_args = FdoArgumentDefinitionCollection::Create();
    byte_dcl_args->Add(byte_1_arg);
    byte_dcl_args->Add(dcl_2_arg);

    byte_dbl_args = FdoArgumentDefinitionCollection::Create();
    byte_dbl_args->Add(byte_1_arg);
    byte_dbl_args->Add(dbl_2_arg);

    byte_int16_args = FdoArgumentDefinitionCollection::Create();
    byte_int16_args->Add(byte_1_arg);
    byte_int16_args->Add(int16_2_arg);

    byte_int32_args = FdoArgumentDefinitionCollection::Create();
    byte_int32_args->Add(byte_1_arg);
    byte_int32_args->Add(int32_2_arg);

    byte_int64_args = FdoArgumentDefinitionCollection::Create();
    byte_int64_args->Add(byte_1_arg);
    byte_int64_args->Add(int64_2_arg);

    byte_sgl_args = FdoArgumentDefinitionCollection::Create();
    byte_sgl_args->Add(byte_1_arg);
    byte_sgl_args->Add(sgl_2_arg);

    byte_str_args = FdoArgumentDefinitionCollection::Create();
    byte_str_args->Add(byte_1_arg);
    byte_str_args->Add(str_2_arg);

    dt_bool_args = FdoArgumentDefinitionCollection::Create();
    dt_bool_args->Add(dt_1_arg);
    dt_bool_args->Add(bool_2_arg);

    dt_byte_args = FdoArgumentDefinitionCollection::Create();
    dt_byte_args->Add(dt_1_arg);
    dt_byte_args->Add(byte_2_arg);

    dt_dt_args = FdoArgumentDefinitionCollection::Create();
    dt_dt_args->Add(dt_1_arg);
    dt_dt_args->Add(dt_2_arg);

    dt_dcl_args = FdoArgumentDefinitionCollection::Create();
    dt_dcl_args->Add(dt_1_arg);
    dt_dcl_args->Add(dcl_2_arg);

    dt_dbl_args = FdoArgumentDefinitionCollection::Create();
    dt_dbl_args->Add(dt_1_arg);
    dt_dbl_args->Add(dbl_2_arg);

    dt_int16_args = FdoArgumentDefinitionCollection::Create();
    dt_int16_args->Add(dt_1_arg);
    dt_int16_args->Add(int16_2_arg);

    dt_int32_args = FdoArgumentDefinitionCollection::Create();
    dt_int32_args->Add(dt_1_arg);
    dt_int32_args->Add(int32_2_arg);

    dt_int64_args = FdoArgumentDefinitionCollection::Create();
    dt_int64_args->Add(dt_1_arg);
    dt_int64_args->Add(int64_2_arg);

    dt_sgl_args = FdoArgumentDefinitionCollection::Create();
    dt_sgl_args->Add(dt_1_arg);
    dt_sgl_args->Add(sgl_2_arg);

    dt_str_args = FdoArgumentDefinitionCollection::Create();
    dt_str_args->Add(dt_1_arg);
    dt_str_args->Add(str_2_arg);

    dt_bool_args = FdoArgumentDefinitionCollection::Create();
    dt_bool_args->Add(dt_1_arg);
    dt_bool_args->Add(bool_2_arg);

    dt_byte_args = FdoArgumentDefinitionCollection::Create();
    dt_byte_args->Add(dt_1_arg);
    dt_byte_args->Add(byte_2_arg);

    dt_dt_args = FdoArgumentDefinitionCollection::Create();
    dt_dt_args->Add(dt_1_arg);
    dt_dt_args->Add(dt_2_arg);

    dt_dcl_args = FdoArgumentDefinitionCollection::Create();
    dt_dcl_args->Add(dt_1_arg);
    dt_dcl_args->Add(dcl_2_arg);

    dt_dbl_args = FdoArgumentDefinitionCollection::Create();
    dt_dbl_args->Add(dt_1_arg);
    dt_dbl_args->Add(dbl_2_arg);

    dt_int16_args = FdoArgumentDefinitionCollection::Create();
    dt_int16_args->Add(dt_1_arg);
    dt_int16_args->Add(int16_2_arg);

    dt_int32_args = FdoArgumentDefinitionCollection::Create();
    dt_int32_args->Add(dt_1_arg);
    dt_int32_args->Add(int32_2_arg);

    dt_int64_args = FdoArgumentDefinitionCollection::Create();
    dt_int64_args->Add(dt_1_arg);
    dt_int64_args->Add(int64_2_arg);

    dt_sgl_args = FdoArgumentDefinitionCollection::Create();
    dt_sgl_args->Add(dt_1_arg);
    dt_sgl_args->Add(sgl_2_arg);

    dt_str_args = FdoArgumentDefinitionCollection::Create();
    dt_str_args->Add(dt_1_arg);
    dt_str_args->Add(str_2_arg);

    dcl_bool_args = FdoArgumentDefinitionCollection::Create();
    dcl_bool_args->Add(dcl_1_arg);
    dcl_bool_args->Add(bool_2_arg);

    dcl_byte_args = FdoArgumentDefinitionCollection::Create();
    dcl_byte_args->Add(dcl_1_arg);
    dcl_byte_args->Add(byte_2_arg);

    dcl_dt_args = FdoArgumentDefinitionCollection::Create();
    dcl_dt_args->Add(dcl_1_arg);
    dcl_dt_args->Add(dt_2_arg);

    dcl_dcl_args = FdoArgumentDefinitionCollection::Create();
    dcl_dcl_args->Add(dcl_1_arg);
    dcl_dcl_args->Add(dcl_2_arg);

    dcl_dbl_args = FdoArgumentDefinitionCollection::Create();
    dcl_dbl_args->Add(dcl_1_arg);
    dcl_dbl_args->Add(dbl_2_arg);

    dcl_int16_args = FdoArgumentDefinitionCollection::Create();
    dcl_int16_args->Add(dcl_1_arg);
    dcl_int16_args->Add(int16_2_arg);

    dcl_int32_args = FdoArgumentDefinitionCollection::Create();
    dcl_int32_args->Add(dcl_1_arg);
    dcl_int32_args->Add(int32_2_arg);

    dcl_int64_args = FdoArgumentDefinitionCollection::Create();
    dcl_int64_args->Add(dcl_1_arg);
    dcl_int64_args->Add(int64_2_arg);

    dcl_sgl_args = FdoArgumentDefinitionCollection::Create();
    dcl_sgl_args->Add(dcl_1_arg);
    dcl_sgl_args->Add(sgl_2_arg);

    dcl_str_args = FdoArgumentDefinitionCollection::Create();
    dcl_str_args->Add(dcl_1_arg);
    dcl_str_args->Add(str_2_arg);

    dbl_bool_args = FdoArgumentDefinitionCollection::Create();
    dbl_bool_args->Add(dbl_1_arg);
    dbl_bool_args->Add(bool_2_arg);

    dbl_byte_args = FdoArgumentDefinitionCollection::Create();
    dbl_byte_args->Add(dbl_1_arg);
    dbl_byte_args->Add(byte_2_arg);

    dbl_dt_args = FdoArgumentDefinitionCollection::Create();
    dbl_dt_args->Add(dbl_1_arg);
    dbl_dt_args->Add(dt_2_arg);

    dbl_dcl_args = FdoArgumentDefinitionCollection::Create();
    dbl_dcl_args->Add(dbl_1_arg);
    dbl_dcl_args->Add(dcl_2_arg);

    dbl_dbl_args = FdoArgumentDefinitionCollection::Create();
    dbl_dbl_args->Add(dbl_1_arg);
    dbl_dbl_args->Add(dbl_2_arg);

    dbl_int16_args = FdoArgumentDefinitionCollection::Create();
    dbl_int16_args->Add(dbl_1_arg);
    dbl_int16_args->Add(int16_2_arg);

    dbl_int32_args = FdoArgumentDefinitionCollection::Create();
    dbl_int32_args->Add(dbl_1_arg);
    dbl_int32_args->Add(int32_2_arg);

    dbl_int64_args = FdoArgumentDefinitionCollection::Create();
    dbl_int64_args->Add(dbl_1_arg);
    dbl_int64_args->Add(int64_2_arg);

    dbl_sgl_args = FdoArgumentDefinitionCollection::Create();
    dbl_sgl_args->Add(dbl_1_arg);
    dbl_sgl_args->Add(sgl_2_arg);

    dbl_str_args = FdoArgumentDefinitionCollection::Create();
    dbl_str_args->Add(dbl_1_arg);
    dbl_str_args->Add(str_2_arg);

    int16_bool_args = FdoArgumentDefinitionCollection::Create();
    int16_bool_args->Add(int16_1_arg);
    int16_bool_args->Add(bool_2_arg);

    int16_byte_args = FdoArgumentDefinitionCollection::Create();
    int16_byte_args->Add(int16_1_arg);
    int16_byte_args->Add(byte_2_arg);

    int16_dt_args = FdoArgumentDefinitionCollection::Create();
    int16_dt_args->Add(int16_1_arg);
    int16_dt_args->Add(dt_2_arg);

    int16_dcl_args = FdoArgumentDefinitionCollection::Create();
    int16_dcl_args->Add(int16_1_arg);
    int16_dcl_args->Add(dcl_2_arg);

    int16_dbl_args = FdoArgumentDefinitionCollection::Create();
    int16_dbl_args->Add(int16_1_arg);
    int16_dbl_args->Add(dbl_2_arg);

    int16_int16_args = FdoArgumentDefinitionCollection::Create();
    int16_int16_args->Add(int16_1_arg);
    int16_int16_args->Add(int16_2_arg);

    int16_int32_args = FdoArgumentDefinitionCollection::Create();
    int16_int32_args->Add(int16_1_arg);
    int16_int32_args->Add(int32_2_arg);

    int16_int64_args = FdoArgumentDefinitionCollection::Create();
    int16_int64_args->Add(int16_1_arg);
    int16_int64_args->Add(int64_2_arg);

    int16_sgl_args = FdoArgumentDefinitionCollection::Create();
    int16_sgl_args->Add(int16_1_arg);
    int16_sgl_args->Add(sgl_2_arg);

    int16_str_args = FdoArgumentDefinitionCollection::Create();
    int16_str_args->Add(int16_1_arg);
    int16_str_args->Add(str_2_arg);

    int32_bool_args = FdoArgumentDefinitionCollection::Create();
    int32_bool_args->Add(int32_1_arg);
    int32_bool_args->Add(bool_2_arg);

    int32_byte_args = FdoArgumentDefinitionCollection::Create();
    int32_byte_args->Add(int32_1_arg);
    int32_byte_args->Add(byte_2_arg);

    int32_dt_args = FdoArgumentDefinitionCollection::Create();
    int32_dt_args->Add(int32_1_arg);
    int32_dt_args->Add(dt_2_arg);

    int32_dcl_args = FdoArgumentDefinitionCollection::Create();
    int32_dcl_args->Add(int32_1_arg);
    int32_dcl_args->Add(dcl_2_arg);

    int32_dbl_args = FdoArgumentDefinitionCollection::Create();
    int32_dbl_args->Add(int32_1_arg);
    int32_dbl_args->Add(dbl_2_arg);

    int32_int16_args = FdoArgumentDefinitionCollection::Create();
    int32_int16_args->Add(int32_1_arg);
    int32_int16_args->Add(int16_2_arg);

    int32_int32_args = FdoArgumentDefinitionCollection::Create();
    int32_int32_args->Add(int32_1_arg);
    int32_int32_args->Add(int32_2_arg);

    int32_int64_args = FdoArgumentDefinitionCollection::Create();
    int32_int64_args->Add(int32_1_arg);
    int32_int64_args->Add(int64_2_arg);

    int32_sgl_args = FdoArgumentDefinitionCollection::Create();
    int32_sgl_args->Add(int32_1_arg);
    int32_sgl_args->Add(sgl_2_arg);

    int32_str_args = FdoArgumentDefinitionCollection::Create();
    int32_str_args->Add(int32_1_arg);
    int32_str_args->Add(str_2_arg);

    int64_bool_args = FdoArgumentDefinitionCollection::Create();
    int64_bool_args->Add(int64_1_arg);
    int64_bool_args->Add(bool_2_arg);

    int64_byte_args = FdoArgumentDefinitionCollection::Create();
    int64_byte_args->Add(int64_1_arg);
    int64_byte_args->Add(byte_2_arg);

    int64_dt_args = FdoArgumentDefinitionCollection::Create();
    int64_dt_args->Add(int64_1_arg);
    int64_dt_args->Add(dt_2_arg);

    int64_dcl_args = FdoArgumentDefinitionCollection::Create();
    int64_dcl_args->Add(int64_1_arg);
    int64_dcl_args->Add(dcl_2_arg);

    int64_dbl_args = FdoArgumentDefinitionCollection::Create();
    int64_dbl_args->Add(int64_1_arg);
    int64_dbl_args->Add(dbl_2_arg);

    int64_int16_args = FdoArgumentDefinitionCollection::Create();
    int64_int16_args->Add(int64_1_arg);
    int64_int16_args->Add(int16_2_arg);

    int64_int32_args = FdoArgumentDefinitionCollection::Create();
    int64_int32_args->Add(int64_1_arg);
    int64_int32_args->Add(int32_2_arg);

    int64_int64_args = FdoArgumentDefinitionCollection::Create();
    int64_int64_args->Add(int64_1_arg);
    int64_int64_args->Add(int64_2_arg);

    int64_sgl_args = FdoArgumentDefinitionCollection::Create();
    int64_sgl_args->Add(int64_1_arg);
    int64_sgl_args->Add(sgl_2_arg);

    int64_str_args = FdoArgumentDefinitionCollection::Create();
    int64_str_args->Add(int64_1_arg);
    int64_str_args->Add(str_2_arg);

    sgl_bool_args = FdoArgumentDefinitionCollection::Create();
    sgl_bool_args->Add(sgl_1_arg);
    sgl_bool_args->Add(bool_2_arg);

    sgl_byte_args = FdoArgumentDefinitionCollection::Create();
    sgl_byte_args->Add(sgl_1_arg);
    sgl_byte_args->Add(byte_2_arg);

    sgl_dt_args = FdoArgumentDefinitionCollection::Create();
    sgl_dt_args->Add(sgl_1_arg);
    sgl_dt_args->Add(dt_2_arg);

    sgl_dcl_args = FdoArgumentDefinitionCollection::Create();
    sgl_dcl_args->Add(sgl_1_arg);
    sgl_dcl_args->Add(dcl_2_arg);

    sgl_dbl_args = FdoArgumentDefinitionCollection::Create();
    sgl_dbl_args->Add(sgl_1_arg);
    sgl_dbl_args->Add(dbl_2_arg);

    sgl_int16_args = FdoArgumentDefinitionCollection::Create();
    sgl_int16_args->Add(sgl_1_arg);
    sgl_int16_args->Add(int16_2_arg);

    sgl_int32_args = FdoArgumentDefinitionCollection::Create();
    sgl_int32_args->Add(sgl_1_arg);
    sgl_int32_args->Add(int32_2_arg);

    sgl_int64_args = FdoArgumentDefinitionCollection::Create();
    sgl_int64_args->Add(sgl_1_arg);
    sgl_int64_args->Add(int64_2_arg);

    sgl_sgl_args = FdoArgumentDefinitionCollection::Create();
    sgl_sgl_args->Add(sgl_1_arg);
    sgl_sgl_args->Add(sgl_2_arg);

    sgl_str_args = FdoArgumentDefinitionCollection::Create();
    sgl_str_args->Add(sgl_1_arg);
    sgl_str_args->Add(str_2_arg);

    str_bool_args = FdoArgumentDefinitionCollection::Create();
    str_bool_args->Add(str_1_arg);
    str_bool_args->Add(bool_2_arg);

    str_byte_args = FdoArgumentDefinitionCollection::Create();
    str_byte_args->Add(str_1_arg);
    str_byte_args->Add(byte_2_arg);

    str_dt_args = FdoArgumentDefinitionCollection::Create();
    str_dt_args->Add(str_1_arg);
    str_dt_args->Add(dt_2_arg);

    str_dcl_args = FdoArgumentDefinitionCollection::Create();
    str_dcl_args->Add(str_1_arg);
    str_dcl_args->Add(dcl_2_arg);

    str_dbl_args = FdoArgumentDefinitionCollection::Create();
    str_dbl_args->Add(str_1_arg);
    str_dbl_args->Add(dbl_2_arg);

    str_int16_args = FdoArgumentDefinitionCollection::Create();
    str_int16_args->Add(str_1_arg);
    str_int16_args->Add(int16_2_arg);

    str_int32_args = FdoArgumentDefinitionCollection::Create();
    str_int32_args->Add(str_1_arg);
    str_int32_args->Add(int32_2_arg);

    str_int64_args = FdoArgumentDefinitionCollection::Create();
    str_int64_args->Add(str_1_arg);
    str_int64_args->Add(int64_2_arg);

    str_sgl_args = FdoArgumentDefinitionCollection::Create();
    str_sgl_args->Add(str_1_arg);
    str_sgl_args->Add(sgl_2_arg);

    str_str_args = FdoArgumentDefinitionCollection::Create();
    str_str_args->Add(str_1_arg);
    str_str_args->Add(str_2_arg);

    // Create the signature collection.

    signatures = FdoSignatureDefinitionCollection::Create();

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, bool_bool_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, bool_byte_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, bool_dt_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, bool_dcl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, bool_dbl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, bool_int16_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, bool_int32_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, bool_int64_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, bool_sgl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, bool_str_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, byte_bool_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, byte_byte_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, byte_dt_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, byte_dcl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, byte_dbl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, byte_int16_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, byte_int32_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, byte_int64_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, byte_sgl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, byte_str_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, dt_bool_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, dt_byte_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, dt_dt_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, dt_dcl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, dt_dbl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, dt_int16_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, dt_int32_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, dt_int64_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, dt_sgl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, dt_str_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, dcl_bool_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, dcl_byte_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, dcl_dt_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, dcl_dcl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, dcl_dbl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, dcl_int16_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, dcl_int32_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, dcl_int64_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, dcl_sgl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, dcl_str_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, dbl_bool_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, dcl_byte_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, dbl_dt_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, dbl_dcl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, dbl_dbl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, dbl_int16_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, dbl_int32_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, dbl_int64_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, dbl_sgl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, dbl_str_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, int16_bool_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, int16_byte_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, int16_dt_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, int16_dcl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, int16_dbl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, int16_int16_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, int16_int32_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, int16_int64_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, int16_sgl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, int16_str_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, int32_bool_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, int32_byte_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, int32_dt_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, int32_dcl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, int32_dbl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, int32_int16_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, int32_int32_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, int32_int64_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, int32_sgl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, int32_str_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, int64_bool_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, int64_byte_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, int64_dt_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, int64_dcl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, int64_dbl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, int64_int16_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, int64_int32_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, int64_int64_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, int64_sgl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, int64_str_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, sgl_bool_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, sgl_byte_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, sgl_dt_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, sgl_dcl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, sgl_dbl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, sgl_int16_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, sgl_int32_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, sgl_int64_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, sgl_sgl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, sgl_str_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, str_bool_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, str_byte_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_String, str_dt_args);
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

    desc = FdoException::NLSGetMessage(
                        FUNCTION_CONCAT,
                        "Returns the string concatenation of 2 expressions");
    function_definition =
                FdoFunctionDefinition::Create(
                                        FDO_FUNCTION_CONCAT,
                                        desc,
                                        false,
                                        signatures,
                                        FdoFunctionCategoryType_String,
                                        true);

}  //  CreateFunctionDefinition ()

bool FdoFunctionConcat::IsValidDataType (FdoDataType data_type)

// +---------------------------------------------------------------------------
// | The function returns true if the provided data type is valid for this
// | function, false otherwise.
// +---------------------------------------------------------------------------

{

    return (!((data_type != FdoDataType_Boolean ) &&
              (data_type != FdoDataType_Byte    ) &&
              (data_type != FdoDataType_DateTime) &&
              (data_type != FdoDataType_Decimal ) &&
              (data_type != FdoDataType_Double  ) &&
              (data_type != FdoDataType_Int16   ) &&
              (data_type != FdoDataType_Int32   ) &&
              (data_type != FdoDataType_Int64   ) &&
              (data_type != FdoDataType_Single  ) &&
              (data_type != FdoDataType_String  )    ));

}  //  IsValidDataType ()

FdoString *FdoFunctionConcat::ProcessArgument (FdoDateTimeValue *value)

// +---------------------------------------------------------------------------
// | The function converts the given argument value into a string and returns
// | it back to the calling routine.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoString *curr_char   = NULL,
              *base_string = NULL;

    // Get the string representation of the argument value. The representation
    // may include one of the keyword 'TIMESTAMP' or 'DATE' at the beginning
    // of the string. If present, the date/time information is also surrounded
    // by single quotes. Both, the keyword and the single quotes need to be
    // removed. Note that to avoid unnecessary allocation of memory, the trail-
    // ing quote is removed when the value is copied into the temporary buffer
    // in the calling routine.

    base_string = value->ToString();

    if (wcsstr(base_string, L"TIMESTAMP '") != NULL)
        curr_char = base_string + 11;
    else
      if (wcsstr(base_string, L"DATE '") != NULL)
          curr_char = base_string + 6;

    return ((curr_char == NULL) ? base_string : curr_char);

}    //  ProcessArgument ()

void FdoFunctionConcat::Validate (FdoLiteralValueCollection *literal_values)

// +---------------------------------------------------------------------------
// | The function validates the argument list that was passed in.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoInt32                i,
                            count           = literal_values->GetCount();

    FdoDataValue            *data_value     = NULL;

    FdoPtr<FdoLiteralValue> literal_value;

    // Check the number of arguments. CONCAT accepts two parameters. If the
    // number of parameters is not correct issue an exception.

    if (count < 2) 
        throw FdoException::Create(
                FdoException::NLSGetMessage(
                  FUNCTION_PARAMETER_NUMBER_ERROR, 
                  "Expression Engine: Invalid number of parameters for function '%1$ls'",
                  FDO_FUNCTION_CONCAT));

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
			        FDO_FUNCTION_CONCAT));

        data_value = static_cast<FdoDataValue *>(literal_value.p);

		if (!IsValidDataType(data_value->GetDataType()))
			 throw FdoException::Create(
					 FdoException::NLSGetMessage(
					   FUNCTION_PARAMETER_DATA_TYPE_ERROR, 
					   "Expression Engine: Invalid parameter data type for function '%1$ls'",
					   FDO_FUNCTION_CONCAT));

    }  //  for (i = 0; i < count; i++) ...

}  //  Validate ()

