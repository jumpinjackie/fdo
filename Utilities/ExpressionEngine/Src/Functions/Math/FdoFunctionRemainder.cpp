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
#include <Functions/Math/FdoFunctionRemainder.h>


// ----------------------------------------------------------------------------
// --                         Constructors/Destructors                       --
// ----------------------------------------------------------------------------

FdoFunctionRemainder::FdoFunctionRemainder ()

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

}  //  FdoFunctionRemainder ()


FdoFunctionRemainder::~FdoFunctionRemainder ()

// +---------------------------------------------------------------------------
// | The class destructor.
// +---------------------------------------------------------------------------

{

    // Delete the function definition.

    FDO_SAFE_RELEASE(function_definition);

}  //  ~FdoFunctionRemainder ()


// ----------------------------------------------------------------------------
// --                            Public Class APIs                           --
// ----------------------------------------------------------------------------

FdoFunctionRemainder *FdoFunctionRemainder::Create ()

// +---------------------------------------------------------------------------
// | The function creates a new instance of the class.
// +---------------------------------------------------------------------------

{

    return new FdoFunctionRemainder();

}  //  Create ()

FdoFunctionRemainder *FdoFunctionRemainder::CreateObject ()

// +---------------------------------------------------------------------------
// | The function creates a new instance of the class.
// +---------------------------------------------------------------------------

{

    return new FdoFunctionRemainder();

}  //  CreateObject ()

FdoFunctionDefinition *FdoFunctionRemainder::GetFunctionDefinition ()

// +---------------------------------------------------------------------------
// | The function creates the supported signature list for the function RE-
// | MAINDER.
// +---------------------------------------------------------------------------

{

    if (function_definition == NULL)
        CreateFunctionDefinition();

    return FDO_SAFE_ADDREF(function_definition);

}  //  GetFunctionDefinition ()

FdoLiteralValue *FdoFunctionRemainder::Evaluate (
                                    FdoLiteralValueCollection *literal_values)

// +---------------------------------------------------------------------------
// | The function processes a call to the function REMAINDER.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    bool                    is_NULL_input     = true,
                            is_zero_divisor   = false,
                            is_zero_dividend  = false;

    FdoDouble               abs_int64_p1,
                            abs_int64_p2,
                            f_dbl_result,
                            p1_dbl_value,
                            p2_dbl_value,
                            nearest_int_val;

    FdoFloat                f_flt_result,
                            p1_flt_value,
                            p2_flt_value;

    FdoInt16                p1_sign,
                            f_int16_result;

    FdoInt32                i,
                            f_int32_result;

    FdoInt64                p1_int64_value,
                            p2_int64_value,
                            f_int64_result;

    FdoDataType             ret_data_type,
                            curr_data_type;

    FdoPtr<FdoByteValue>    byte_value;
    FdoPtr<FdoDecimalValue> decimal_value;
    FdoPtr<FdoDoubleValue>  double_value;
    FdoPtr<FdoInt16Value>   int16_value;
    FdoPtr<FdoInt32Value>   int32_value;
    FdoPtr<FdoInt64Value>   int64_value;
    FdoPtr<FdoSingleValue>  single_value;

    // Validate the function call.

    if (first)
    {
        Validate(literal_values);
        // first will be clear after the return object is created
    }

    // Process the request and return the result back to the calling routine.

    for (i = 0; i < 2; i++) {

      is_NULL_input  = true;
      curr_data_type = (i == 0) ? para1_data_type : para2_data_type;
      switch (curr_data_type) {

        case FdoDataType_Byte:
          byte_value = (FdoByteValue *) literal_values->GetItem(i);
          if (!byte_value->IsNull()) {

              is_NULL_input = false;

              if (i == 0) {

                  p1_int64_value   = byte_value->GetByte();
                  p1_sign          = 1;
                  is_zero_dividend = (p1_int64_value == 0);

              }  //  if (i == 0) ...
              else {

                p2_int64_value  = byte_value->GetByte();
                is_zero_divisor = (p2_int64_value == 0);

              }  //  else ...

          }  //  if (!byte_value->IsNull()) ...

          break;

        case FdoDataType_Decimal:
          decimal_value =(FdoDecimalValue *) literal_values->GetItem(i);
          if (!decimal_value->IsNull()) {

              is_NULL_input = false;

              if (i == 0) {

                  p1_dbl_value     = decimal_value->GetDecimal();
                  p1_sign          = (p1_dbl_value < 0) ? -1 : 1;
                  is_zero_dividend = (p1_dbl_value == 0);

              }  //  if (i == 0) ...
              else {

                p2_dbl_value    = decimal_value->GetDecimal();
                is_zero_divisor = (p2_dbl_value == 0);

              }  //  else ...

          }  //  if (!decimal_value->IsNull()) ...

          break;

        case FdoDataType_Double:
          double_value = (FdoDoubleValue *) literal_values->GetItem(i);
          if (!double_value->IsNull()) {

              is_NULL_input = false;

              if (i == 0) {

                  p1_dbl_value     = double_value->GetDouble();
                  p1_sign          = (p1_dbl_value < 0) ? -1 : 1;
                  is_zero_dividend = (p1_dbl_value == 0);

              }  //  if (i == 0) ...
              else {

                p2_dbl_value    = double_value->GetDouble();
                is_zero_divisor = (p2_dbl_value == 0);

              }  //  else ...

          }  //  if (!double_value->IsNull()) ...

          break;

        case FdoDataType_Int16:
          int16_value = (FdoInt16Value *) literal_values->GetItem(i);
          if (!int16_value->IsNull()) {

              is_NULL_input = false;

              if (i == 0) {

                  p1_int64_value   = int16_value->GetInt16();
                  p1_sign          = (p1_int64_value < 0) ? -1 : 1;
                  is_zero_dividend = (p1_int64_value == 0);

              }  //  if (i == 0) ...
              else {

                p2_int64_value  = int16_value->GetInt16();
                is_zero_divisor = (p2_int64_value == 0);

              }  //  else ...

          }  //  if (!int16_value->IsNull()) ...

          break;

        case FdoDataType_Int32:
          int32_value = (FdoInt32Value *) literal_values->GetItem(i);
          if (!int32_value->IsNull()) {

              is_NULL_input = false;

              if (i == 0) {

                  p1_int64_value   = int32_value->GetInt32();
                  p1_sign          = (p1_int64_value < 0) ? -1 : 1;
                  is_zero_dividend = (p1_int64_value == 0);

              }  //  if (i == 0) ...
              else {

                p2_int64_value  = int32_value->GetInt32();
                is_zero_divisor = (p2_int64_value == 0);

              }  //  else ...

          }  //  if (!int32_value->IsNull()) ...

          break;

        case FdoDataType_Int64:
          int64_value = (FdoInt64Value *) literal_values->GetItem(i);
          if (!int64_value->IsNull()) {

              is_NULL_input = false;

              if (i == 0) {

                  p1_int64_value   = int64_value->GetInt64();
                  p1_sign          = (p1_int64_value < 0) ? -1 : 1;
                  is_zero_dividend = (p1_int64_value == 0);

              }  //  if (i == 0) ...
              else {

                p2_int64_value  = int64_value->GetInt64();
                is_zero_divisor = (p2_int64_value == 0);

              }  //  else ...

          }  //  if (!int64_value->IsNull()) ...

          break;

        case FdoDataType_Single:
          single_value = (FdoSingleValue *) literal_values->GetItem(i);
          if (!single_value->IsNull()) {

              is_NULL_input = false;

              if (i == 0) {

                  p1_flt_value     = single_value->GetSingle();
                  p1_sign          = (p1_flt_value < 0) ? -1 : 1;
                  is_zero_dividend = (p1_flt_value == 0);

              }  //  if (i == 0) ...
              else {

                p2_flt_value    = single_value->GetSingle();
                is_zero_divisor = (p2_flt_value == 0);

              }  //  else ...

          }  //  if (!single_value->IsNull()) ...

          break;

      }  //  switch ...

      // If the provided value is NULL, terminate the loop.

      if (is_NULL_input)
          break;

    }  //  for (i = 0; i < 2; i++) ...

    // Determine the data type to be used to return the function result.

    ret_data_type = GetReturnDataType(para1_data_type, para2_data_type);

    // If a NULL value has been identified return an empty result object.

    if (is_NULL_input) {

        switch (ret_data_type) {

          case FdoDataType_Byte:
              if (first)
              {
                return_data_value = FdoByteValue::Create();
                first = false;
              }
              return_data_value->SetNull();
              return FDO_SAFE_ADDREF(return_data_value.p);

            break;

          case FdoDataType_Decimal:
              if (first)
              {
                return_data_value = FdoDecimalValue::Create();
                first = false;
              }
              return_data_value->SetNull();
              return FDO_SAFE_ADDREF(return_data_value.p);
            break;

          case FdoDataType_Double:
              if (first)
              {
                return_data_value = FdoDoubleValue::Create();
                first = false;
              }
              return_data_value->SetNull();
              return FDO_SAFE_ADDREF(return_data_value.p);
            break;

          case FdoDataType_Int16:
              if (first)
              {
                return_data_value = FdoInt16Value::Create();
                first = false;
              }
              return_data_value->SetNull();
              return FDO_SAFE_ADDREF(return_data_value.p);
            break;

          case FdoDataType_Int32:
              if (first)
              {
                return_data_value = FdoInt32Value::Create();
                first = false;
              }
              return_data_value->SetNull();
              return FDO_SAFE_ADDREF(return_data_value.p);
            break;

          case FdoDataType_Int64:
              if (first)
              {
                return_data_value = FdoInt64Value::Create();
                first = false;
              }
              return_data_value->SetNull();
              return FDO_SAFE_ADDREF(return_data_value.p);
            break;

          case FdoDataType_Single:
              if (first)
              {
                return_data_value = FdoSingleValue::Create();
                first = false;
              }
              return_data_value->SetNull();
              return FDO_SAFE_ADDREF(return_data_value.p);

            break;

        }  //  switch ...

    }  //  if (is_NULL_input) ...

    // If the divisor is 0, issue an exception.

    if (is_zero_divisor)
        throw FdoException::Create(
                    FdoException::NLSGetMessage(
                        FUNCTION_DIV_BY_0_ERROR, 
                        "Expression Engine: Attempt to divide by 0 ('%1$ls')",
                        FDO_FUNCTION_REMAINDER));

    // If the dividend is 0, return 0.

    if (is_zero_dividend) {

        switch (ret_data_type) {

          case FdoDataType_Byte:
              if (first)
              {
                return_data_value = FdoByteValue::Create();
                first = false;
              }
              (static_cast<FdoByteValue *> (return_data_value.p))->SetByte((FdoByte)0);
              return FDO_SAFE_ADDREF(return_data_value.p);
            break;

          case FdoDataType_Decimal:
              if (first)
              {
                return_data_value = FdoDecimalValue::Create();
                first = false;
              }
              (static_cast<FdoDecimalValue *> (return_data_value.p))->SetDecimal((double)0);
              return FDO_SAFE_ADDREF(return_data_value.p);
            break;

          case FdoDataType_Double:
              if (first)
              {
                return_data_value = FdoDoubleValue::Create();
                first = false;
              }
              (static_cast<FdoDoubleValue *> (return_data_value.p))->SetDouble((double)0);
              return FDO_SAFE_ADDREF(return_data_value.p);
            break;

          case FdoDataType_Int16:
              if (first)
              {
                return_data_value = FdoInt16Value::Create();
                first = false;
              }
              (static_cast<FdoInt16Value *> (return_data_value.p))->SetInt16((FdoInt16)0);
              return FDO_SAFE_ADDREF(return_data_value.p);
            break;

          case FdoDataType_Int32:
              if (first)
              {
                return_data_value = FdoInt32Value::Create();
                first = false;
              }
              (static_cast<FdoInt32Value *> (return_data_value.p))->SetInt32((FdoInt32)0);
              return FDO_SAFE_ADDREF(return_data_value.p);
            break;

          case FdoDataType_Int64:
              if (first)
              {
                return_data_value = FdoInt64Value::Create();
                first = false;
              }
              (static_cast<FdoInt64Value *> (return_data_value.p))->SetInt64((FdoInt64)0);
              return FDO_SAFE_ADDREF(return_data_value.p);
            break;

          case FdoDataType_Single:
              if (first)
              {
                return_data_value = FdoSingleValue::Create();
                first = false;
              }
              (static_cast<FdoSingleValue *> (return_data_value.p))->SetSingle((float)0);
              return FDO_SAFE_ADDREF(return_data_value.p);
            break;

        }  //  switch ...

    }  //  if (is_zero_dividend) ...

    // Calculate the function result.

    if ((para1_data_type == FdoDataType_Byte       ) &&
        ((para2_data_type == FdoDataType_Byte ) ||
         (para2_data_type == FdoDataType_Int16)    )    ){

         abs_int64_p1    = GetInt64AbsValue(p1_int64_value);
         abs_int64_p2    = GetInt64AbsValue(p2_int64_value); 
         nearest_int_val = Round((abs_int64_p1 / abs_int64_p2));
         f_int16_result  =
            (FdoInt16) (p1_sign * (abs_int64_p1 -
                                        (abs_int64_p2 * nearest_int_val)));
         if (first)
         {
            return_data_value = FdoInt16Value::Create();
            first = false;
         }
         (static_cast<FdoInt16Value *> (return_data_value.p))->SetInt16(f_int16_result);
         return FDO_SAFE_ADDREF(return_data_value.p);

    }  //  if ((para1_data_type == FdoDataType_Byte) ...

    if ((para1_data_type == FdoDataType_Byte ) &&
        (para2_data_type == FdoDataType_Int32)    ) {

         abs_int64_p1    = GetInt64AbsValue(p1_int64_value);
         abs_int64_p2    = GetInt64AbsValue(p2_int64_value);
         nearest_int_val = Round((abs_int64_p1 / abs_int64_p2));
         f_int32_result  =
            (FdoInt32) (p1_sign * (abs_int64_p1 -
                                        (abs_int64_p2 * nearest_int_val)));
         if (first)
         {
            return_data_value = FdoInt32Value::Create();
            first = false;
         }
         (static_cast<FdoInt32Value *> (return_data_value.p))->SetInt32(f_int32_result);
         return FDO_SAFE_ADDREF(return_data_value.p);

    }  //  if ((para1_data_type == FdoDataType_Byte) ...

    if ((para1_data_type == FdoDataType_Byte ) &&
        (para2_data_type == FdoDataType_Int64)    ) {

         abs_int64_p1    = GetInt64AbsValue(p1_int64_value);
         abs_int64_p2    = GetInt64AbsValue(p2_int64_value); 
         nearest_int_val = Round((abs_int64_p1 / abs_int64_p2));
         f_int64_result  =
            (FdoInt64) (p1_sign * (abs_int64_p1 -
                                        (abs_int64_p2 * nearest_int_val)));
         if (first)
         {
            return_data_value = FdoInt64Value::Create();
            first = false;
         }
         (static_cast<FdoInt64Value *> (return_data_value.p))->SetInt64(f_int64_result);
         return FDO_SAFE_ADDREF(return_data_value.p);

    }  //  if ((para1_data_type == FdoDataType_Byte) ...

    if (((para1_data_type == FdoDataType_Decimal) &&
         (para2_data_type == FdoDataType_Decimal)    ) ||
        ((para1_data_type == FdoDataType_Decimal) &&
         (para2_data_type == FdoDataType_Double )    ) ||
        ((para1_data_type == FdoDataType_Double) &&
         (para2_data_type == FdoDataType_Decimal)    ) ||
        ((para1_data_type == FdoDataType_Double ) &&
         (para2_data_type == FdoDataType_Double )    )    ) {

         nearest_int_val = Round(fabs(p1_dbl_value) / fabs(p2_dbl_value));
         f_dbl_result    =
            p1_sign * (fabs(p1_dbl_value) -
                                    (fabs(p2_dbl_value) * nearest_int_val));

         if (first)
         {
            return_data_value = FdoDoubleValue::Create();
            first = false;
         }
         (static_cast<FdoDoubleValue *> (return_data_value.p))->SetDouble(f_dbl_result);
         return FDO_SAFE_ADDREF(return_data_value.p);

    }  //  if (((para1_data_type == FdoDataType_Decimal) ...

    if ((para1_data_type == FdoDataType_Decimal    ) &&
        ((para2_data_type == FdoDataType_Byte ) ||
         (para2_data_type == FdoDataType_Int16) ||
         (para2_data_type == FdoDataType_Int32) ||
         (para2_data_type == FdoDataType_Int64)    )    ) {

         abs_int64_p2    = GetInt64AbsValue(p2_int64_value);
         nearest_int_val = Round(fabs(p1_dbl_value) / abs_int64_p2);
         f_dbl_result    =
                p1_sign * (fabs(p1_dbl_value) -
                                        (abs_int64_p2 * nearest_int_val));

         if (first)
         {
            return_data_value = FdoDoubleValue::Create();
            first = false;
         }
         (static_cast<FdoDoubleValue *> (return_data_value.p))->SetDouble(f_dbl_result);
         return FDO_SAFE_ADDREF(return_data_value.p);

    }  //  if ((para1_data_type == FdoDataType_Decimal) ...

    if ((para1_data_type == FdoDataType_Decimal) &&
        (para2_data_type == FdoDataType_Single )    ) {

         nearest_int_val = Round(fabs(p1_dbl_value) / fabs(p2_flt_value));
         f_dbl_result    =
            p1_sign * (fabs(p1_dbl_value) -
                                    (fabs(p2_flt_value) * nearest_int_val));
         if (first)
         {
            return_data_value = FdoDoubleValue::Create();
            first = false;
         }
         (static_cast<FdoDoubleValue *> (return_data_value.p))->SetDouble(f_dbl_result);
         return FDO_SAFE_ADDREF(return_data_value.p);

    }  //  if ((para1_data_type == FdoDataType_Decimal) ...

    if ((para1_data_type == FdoDataType_Double     ) &&
        ((para2_data_type == FdoDataType_Byte ) ||
         (para2_data_type == FdoDataType_Int16) ||
         (para2_data_type == FdoDataType_Int32) ||
         (para2_data_type == FdoDataType_Int64)    )    ) {

         abs_int64_p2    = GetInt64AbsValue(p2_int64_value);
         nearest_int_val = Round(fabs(p1_dbl_value) / abs_int64_p2);
         f_dbl_result    =
                p1_sign * (fabs(p1_dbl_value) -
                                        (abs_int64_p2 * nearest_int_val));

         if (first)
         {
            return_data_value = FdoDoubleValue::Create();
            first = false;
         }
         (static_cast<FdoDoubleValue *> (return_data_value.p))->SetDouble(f_dbl_result);
         return FDO_SAFE_ADDREF(return_data_value.p);

    }  //  if ((para1_data_type == FdoDataType_Double) ...

    if ((para1_data_type == FdoDataType_Double) &&
        (para2_data_type == FdoDataType_Single )    ) {

         nearest_int_val = Round(fabs(p1_dbl_value) / fabs(p2_flt_value));
         f_dbl_result    =
            p1_sign * (fabs(p1_dbl_value) -
                                    (fabs(p2_flt_value) * nearest_int_val));

         if (first)
         {
            return_data_value = FdoDoubleValue::Create();
            first = false;
         }
         (static_cast<FdoDoubleValue *> (return_data_value.p))->SetDouble(f_dbl_result);
         return FDO_SAFE_ADDREF(return_data_value.p);

    }  //  if ((para1_data_type == FdoDataType_Double) ...

    if (((para1_data_type == FdoDataType_Byte   ) ||
         (para1_data_type == FdoDataType_Int16  ) ||
         (para1_data_type == FdoDataType_Int32  ) ||
         (para1_data_type == FdoDataType_Int64  )    ) &&
        ((para2_data_type == FdoDataType_Decimal) ||
         (para2_data_type == FdoDataType_Double )    )    ) {

         abs_int64_p1    = GetInt64AbsValue(p1_int64_value);
         nearest_int_val = Round(abs_int64_p1 / fabs(p2_dbl_value));
         f_dbl_result    =
                    p1_sign * (abs_int64_p1 -
                                    (fabs(p2_dbl_value) * nearest_int_val));

         if (first)
         {
            return_data_value = FdoDoubleValue::Create();
            first = false;
         }
         (static_cast<FdoDoubleValue *> (return_data_value.p))->SetDouble(f_dbl_result);
         return FDO_SAFE_ADDREF(return_data_value.p);

    }  //  if (((para1_data_type == FdoDataType_Int16) ...

    if ((para1_data_type == FdoDataType_Int16 )      &&
        ((para2_data_type == FdoDataType_Byte ) ||
         (para2_data_type == FdoDataType_Int16) ||
         (para2_data_type == FdoDataType_Int32) ||
         (para2_data_type == FdoDataType_Int64)    )    ) {

         abs_int64_p1    = GetInt64AbsValue(p1_int64_value);
         abs_int64_p2    = GetInt64AbsValue(p2_int64_value);
         nearest_int_val = Round((abs_int64_p1 / abs_int64_p2));
         f_int16_result  =
            (FdoInt16) (p1_sign * (abs_int64_p1 -
                                        (abs_int64_p2 * nearest_int_val)));

         if (first)
         {
            return_data_value = FdoInt16Value::Create();
            first = false;
         }
         (static_cast<FdoInt16Value *> (return_data_value.p))->SetInt16(f_int16_result);
         return FDO_SAFE_ADDREF(return_data_value.p);

    }  //  if (((para1_data_type == FdoDataType_Int16) ...

    if (((para1_data_type == FdoDataType_Byte  ) ||
         (para1_data_type == FdoDataType_Int16 ) ||
         (para1_data_type == FdoDataType_Int32 ) ||
         (para1_data_type == FdoDataType_Int64 )    ) &&
        (para2_data_type == FdoDataType_Single      )    ) {

         abs_int64_p1    = GetInt64AbsValue(p1_int64_value);
         nearest_int_val = Round(abs_int64_p1 / fabs(p2_flt_value));
         f_flt_result    =
           (FdoFloat) (p1_sign * (abs_int64_p1 -
                                    (fabs(p2_flt_value) * nearest_int_val)));

         if (first)
         {
            return_data_value = FdoSingleValue::Create();
            first = false;
         }
         (static_cast<FdoSingleValue *> (return_data_value.p))->SetSingle(f_flt_result);
         return FDO_SAFE_ADDREF(return_data_value.p);

    }  //  if (((para1_data_type == FdoDataType_Int16) ...

    if (((para1_data_type == FdoDataType_Int32) ||
         (para1_data_type == FdoDataType_Int64)    ) &&
        (para2_data_type == FdoDataType_Int16      )    ) {

         abs_int64_p1    = GetInt64AbsValue(p1_int64_value);
         abs_int64_p2    = GetInt64AbsValue(p2_int64_value);
         nearest_int_val = Round(abs_int64_p1 / abs_int64_p2);
         f_int16_result  =
            (FdoInt16) (p1_sign * (abs_int64_p1 -
                                        (abs_int64_p2 * nearest_int_val)));

         if (first)
         {
            return_data_value = FdoInt16Value::Create();
            first = false;
         }
         (static_cast<FdoInt16Value *> (return_data_value.p))->SetInt16(f_int16_result);
         return FDO_SAFE_ADDREF(return_data_value.p);

    }  //  if (((para1_data_type == FdoDataType_Int32) ...

    if ((para1_data_type == FdoDataType_Int32      ) &&
        ((para2_data_type == FdoDataType_Byte ) ||
         (para2_data_type == FdoDataType_Int64)    )    ) {

         abs_int64_p1    = GetInt64AbsValue(p1_int64_value);
         abs_int64_p2    = GetInt64AbsValue(p2_int64_value);
         nearest_int_val = Round(abs_int64_p1 / abs_int64_p2);
         f_int32_result  = 
            (FdoInt32) (p1_sign * (abs_int64_p1 -
                                        (abs_int64_p2 * nearest_int_val)));

         if (first)
         {
            return_data_value = FdoInt32Value::Create();
            first = false;
         }
         (static_cast<FdoInt32Value *> (return_data_value.p))->SetInt32(f_int32_result);
         return FDO_SAFE_ADDREF(return_data_value.p);

    }  //  if (((para1_data_type == FdoDataType_Int32) ...

    if (((para1_data_type == FdoDataType_Int32) ||
         (para1_data_type == FdoDataType_Int64)    ) &&
        (para2_data_type == FdoDataType_Int32      )    ) {

         abs_int64_p1    = GetInt64AbsValue(p1_int64_value);
         abs_int64_p2    = GetInt64AbsValue(p2_int64_value);
         nearest_int_val = Round(abs_int64_p1 / abs_int64_p2);
         f_int32_result  =
            (FdoInt32) (p1_sign * (abs_int64_p1 -
                                        (abs_int64_p2 * nearest_int_val)));

         if (first)
         {
            return_data_value = FdoInt32Value::Create();
            first = false;
         }
         (static_cast<FdoInt32Value *> (return_data_value.p))->SetInt32(f_int32_result);
         return FDO_SAFE_ADDREF(return_data_value.p);


    }  //  if (((para1_data_type == FdoDataType_Int32) ...

    if ((para1_data_type == FdoDataType_Int64      ) &&
        ((para2_data_type == FdoDataType_Byte ) ||
         (para2_data_type == FdoDataType_Int64)    )    ) {

         abs_int64_p1    = GetInt64AbsValue(p1_int64_value);
         abs_int64_p2    = GetInt64AbsValue(p2_int64_value);
         nearest_int_val = Round(abs_int64_p1 / abs_int64_p2);
         f_int64_result  =
            (FdoInt64) (p1_sign * (abs_int64_p1 -
                                        (abs_int64_p2 * nearest_int_val)));

         if (first)
         {
            return_data_value = FdoInt64Value::Create();
            first = false;
         }
         (static_cast<FdoInt64Value *> (return_data_value.p))->SetInt64(f_int64_result);
         return FDO_SAFE_ADDREF(return_data_value.p);

    }  //  if (((para1_data_type == FdoDataType_Int32) ...

    if ((para1_data_type == FdoDataType_Single       ) &&
        ((para2_data_type == FdoDataType_Decimal) ||
         (para2_data_type == FdoDataType_Double )    )    ) {

         nearest_int_val = Round(fabs(p1_flt_value) / fabs(p2_dbl_value));
         f_dbl_result    =
            p1_sign * (fabs(p1_flt_value) -
                                    (fabs(p2_dbl_value) * nearest_int_val));

         if (first)
         {
            return_data_value = FdoDoubleValue::Create();
            first = false;
         }
         (static_cast<FdoDoubleValue *> (return_data_value.p))->SetDouble(f_dbl_result);
         return FDO_SAFE_ADDREF(return_data_value.p);

    }  //  if (((para1_data_type == FdoDataType_Single) ...

    if ((para1_data_type == FdoDataType_Single       ) &&
        ((para2_data_type == FdoDataType_Byte ) ||
         (para2_data_type == FdoDataType_Int16) ||
         (para2_data_type == FdoDataType_Int32) ||
         (para2_data_type == FdoDataType_Int32)    )    ) {

         abs_int64_p2    = GetInt64AbsValue(p2_int64_value);
         nearest_int_val = Round(fabs(p1_flt_value) / abs_int64_p2);
         f_flt_result    =
              (FdoFloat) (p1_sign * (fabs(p1_flt_value) -
                                        (abs_int64_p2 * nearest_int_val)));

         if (first)
         {
            return_data_value = FdoSingleValue::Create();
            first = false;
         }
         (static_cast<FdoSingleValue *> (return_data_value.p))->SetSingle(f_flt_result);
         return FDO_SAFE_ADDREF(return_data_value.p);

    }  //  if (((para1_data_type == FdoDataType_Single) ...

    if ((para1_data_type == FdoDataType_Single) &&
        (para2_data_type == FdoDataType_Single)    ) {

         nearest_int_val = Round(fabs(p1_flt_value) / fabs(p2_flt_value));
         f_flt_result    =
            (FdoFloat) (p1_sign * (fabs(p1_flt_value) -
                                    (fabs(p2_flt_value) * nearest_int_val)));

         if (first)
         {
            return_data_value = FdoSingleValue::Create();
            first = false;
         }
         (static_cast<FdoSingleValue *> (return_data_value.p))->SetSingle(f_flt_result);
         return FDO_SAFE_ADDREF(return_data_value.p);

    }  //  if (((para1_data_type == FdoDataType_Single) ...

    // The validation at the top of the function ensures that any invalid call
    // is handled correctly. Therefore, the above statements should have pro-
    // cessed the request correctly and the function result should have been
    // returned. Just in case there is an issue, the following exception is
    // thrown if this part of the function implementation is reached.

    throw FdoException::Create(
            FdoException::NLSGetMessage(
              FUNCTION_UNEXPECTED_RESULT_ERROR, 
              "Expression Engine: Unexpected result for function '%1$ls'",
              FDO_FUNCTION_REMAINDER));

}  //  Evaluate ()


// ----------------------------------------------------------------------------
// --                          Supporting functions                          --
// ----------------------------------------------------------------------------

void FdoFunctionRemainder::CreateFunctionDefinition ()

// +---------------------------------------------------------------------------
// | The procedure creates the function definition for the function REMAINDER.
// | The function definition includes the list of supported signatures. The
// | following signatures are supported:
// |
// |    REMAINDER ({byte, decimal, double, int16, int32, int64, single},
// |               {byte, decimal, double, int16, int32, int64, single} )
// |
// | The function's return value data type depends on the provided input para-
// | meter types.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoString                               *desc               = NULL;

    FdoStringP                              arg1_description;
    FdoStringP                              arg2_description;
    FdoStringP                              num_arg_literal;
    FdoStringP                              div_arg_literal;

    FdoPtr<FdoArgumentDefinition>           byte_arg;
    FdoPtr<FdoArgumentDefinition>           dcl_arg;
    FdoPtr<FdoArgumentDefinition>           dbl_arg;
    FdoPtr<FdoArgumentDefinition>           int16_arg;
    FdoPtr<FdoArgumentDefinition>           int32_arg;
    FdoPtr<FdoArgumentDefinition>           int64_arg;
    FdoPtr<FdoArgumentDefinition>           sgl_arg;

    FdoPtr<FdoArgumentDefinition>           byte_div_arg;
    FdoPtr<FdoArgumentDefinition>           dcl_div_arg;
    FdoPtr<FdoArgumentDefinition>           dbl_div_arg;
    FdoPtr<FdoArgumentDefinition>           int16_div_arg;
    FdoPtr<FdoArgumentDefinition>           int32_div_arg;
    FdoPtr<FdoArgumentDefinition>           int64_div_arg;
    FdoPtr<FdoArgumentDefinition>           sgl_div_arg;

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
    FdoSignatureDefinitionCollection        *signatures;

    // Get the general descriptions for the arguments.

    arg1_description = FdoException::NLSGetMessage(
                                        FUNCTION_GENERAL_ARG,
                                        "Argument to be processed");

    arg2_description = FdoException::NLSGetMessage(
                                        FUNCTION_DIVISOR_ARG,
                                        "The divisor");

    // The following defines the different argument definition collections.

    div_arg_literal =
            FdoException::NLSGetMessage(FUNCTION_DIV_ARG_LIT, "divisor");

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

    byte_div_arg  = FdoArgumentDefinition::Create(
                        div_arg_literal, arg2_description, FdoDataType_Byte);
    dcl_div_arg   = FdoArgumentDefinition::Create(
                        div_arg_literal, arg2_description, FdoDataType_Decimal);
    dbl_div_arg   = FdoArgumentDefinition::Create(
                        div_arg_literal, arg2_description, FdoDataType_Double);
    int16_div_arg = FdoArgumentDefinition::Create(
                        div_arg_literal, arg2_description, FdoDataType_Int16);
    int32_div_arg = FdoArgumentDefinition::Create(
                        div_arg_literal, arg2_description, FdoDataType_Int32);
    int64_div_arg = FdoArgumentDefinition::Create(
                        div_arg_literal, arg2_description, FdoDataType_Int64);
    sgl_div_arg   = FdoArgumentDefinition::Create(
                        div_arg_literal, arg2_description, FdoDataType_Single);

    byte_byte_args = FdoArgumentDefinitionCollection::Create();
    byte_byte_args->Add(byte_arg);
    byte_byte_args->Add(byte_div_arg);

    byte_dcl_args = FdoArgumentDefinitionCollection::Create();
    byte_dcl_args->Add(byte_arg);
    byte_dcl_args->Add(dcl_div_arg);

    byte_dbl_args = FdoArgumentDefinitionCollection::Create();
    byte_dbl_args->Add(byte_arg);
    byte_dbl_args->Add(dbl_div_arg);

    byte_int16_args = FdoArgumentDefinitionCollection::Create();
    byte_int16_args->Add(byte_arg);
    byte_int16_args->Add(int16_div_arg);

    byte_int32_args = FdoArgumentDefinitionCollection::Create();
    byte_int32_args->Add(byte_arg);
    byte_int32_args->Add(int32_div_arg);

    byte_int64_args = FdoArgumentDefinitionCollection::Create();
    byte_int64_args->Add(byte_arg);
    byte_int64_args->Add(int64_div_arg);

    byte_sgl_args = FdoArgumentDefinitionCollection::Create();
    byte_sgl_args->Add(byte_arg);
    byte_sgl_args->Add(sgl_div_arg);

    dcl_byte_args = FdoArgumentDefinitionCollection::Create();
    dcl_byte_args->Add(dcl_arg);
    dcl_byte_args->Add(byte_div_arg);

    dcl_dcl_args = FdoArgumentDefinitionCollection::Create();
    dcl_dcl_args->Add(dcl_arg);
    dcl_dcl_args->Add(dcl_div_arg);

    dcl_dbl_args = FdoArgumentDefinitionCollection::Create();
    dcl_dbl_args->Add(dcl_arg);
    dcl_dbl_args->Add(dbl_div_arg);

    dcl_int16_args = FdoArgumentDefinitionCollection::Create();
    dcl_int16_args->Add(dcl_arg);
    dcl_int16_args->Add(int16_div_arg);

    dcl_int32_args = FdoArgumentDefinitionCollection::Create();
    dcl_int32_args->Add(dcl_arg);
    dcl_int32_args->Add(int32_div_arg);

    dcl_int64_args = FdoArgumentDefinitionCollection::Create();
    dcl_int64_args->Add(dcl_arg);
    dcl_int64_args->Add(int64_div_arg);

    dcl_sgl_args = FdoArgumentDefinitionCollection::Create();
    dcl_sgl_args->Add(dcl_arg);
    dcl_sgl_args->Add(sgl_div_arg);

    dbl_byte_args = FdoArgumentDefinitionCollection::Create();
    dbl_byte_args->Add(dbl_arg);
    dbl_byte_args->Add(byte_div_arg);

    dbl_dcl_args = FdoArgumentDefinitionCollection::Create();
    dbl_dcl_args->Add(dbl_arg);
    dbl_dcl_args->Add(dcl_div_arg);

    dbl_dbl_args = FdoArgumentDefinitionCollection::Create();
    dbl_dbl_args->Add(dbl_arg);
    dbl_dbl_args->Add(dbl_div_arg);

    dbl_int16_args = FdoArgumentDefinitionCollection::Create();
    dbl_int16_args->Add(dbl_arg);
    dbl_int16_args->Add(int16_div_arg);

    dbl_int32_args = FdoArgumentDefinitionCollection::Create();
    dbl_int32_args->Add(dbl_arg);
    dbl_int32_args->Add(int32_div_arg);

    dbl_int64_args = FdoArgumentDefinitionCollection::Create();
    dbl_int64_args->Add(dbl_arg);
    dbl_int64_args->Add(int64_div_arg);

    dbl_sgl_args = FdoArgumentDefinitionCollection::Create();
    dbl_sgl_args->Add(dbl_arg);
    dbl_sgl_args->Add(sgl_div_arg);

    int16_byte_args = FdoArgumentDefinitionCollection::Create();
    int16_byte_args->Add(int16_arg);
    int16_byte_args->Add(byte_div_arg);

    int16_dcl_args = FdoArgumentDefinitionCollection::Create();
    int16_dcl_args->Add(int16_arg);
    int16_dcl_args->Add(dcl_div_arg);

    int16_dbl_args = FdoArgumentDefinitionCollection::Create();
    int16_dbl_args->Add(int16_arg);
    int16_dbl_args->Add(dbl_div_arg);

    int16_int16_args = FdoArgumentDefinitionCollection::Create();
    int16_int16_args->Add(int16_arg);
    int16_int16_args->Add(int16_div_arg);

    int16_int32_args = FdoArgumentDefinitionCollection::Create();
    int16_int32_args->Add(int16_arg);
    int16_int32_args->Add(int32_div_arg);

    int16_int64_args = FdoArgumentDefinitionCollection::Create();
    int16_int64_args->Add(int16_arg);
    int16_int64_args->Add(int64_div_arg);

    int16_sgl_args = FdoArgumentDefinitionCollection::Create();
    int16_sgl_args->Add(int16_arg);
    int16_sgl_args->Add(sgl_div_arg);

    int32_byte_args = FdoArgumentDefinitionCollection::Create();
    int32_byte_args->Add(int32_arg);
    int32_byte_args->Add(byte_div_arg);

    int32_dcl_args = FdoArgumentDefinitionCollection::Create();
    int32_dcl_args->Add(int32_arg);
    int32_dcl_args->Add(dcl_div_arg);

    int32_dbl_args = FdoArgumentDefinitionCollection::Create();
    int32_dbl_args->Add(int32_arg);
    int32_dbl_args->Add(dbl_div_arg);

    int32_int16_args = FdoArgumentDefinitionCollection::Create();
    int32_int16_args->Add(int32_arg);
    int32_int16_args->Add(int16_div_arg);

    int32_int32_args = FdoArgumentDefinitionCollection::Create();
    int32_int32_args->Add(int32_arg);
    int32_int32_args->Add(int32_div_arg);

    int32_int64_args = FdoArgumentDefinitionCollection::Create();
    int32_int64_args->Add(int32_arg);
    int32_int64_args->Add(int64_div_arg);

    int32_sgl_args = FdoArgumentDefinitionCollection::Create();
    int32_sgl_args->Add(int32_arg);
    int32_sgl_args->Add(sgl_div_arg);

    int64_byte_args = FdoArgumentDefinitionCollection::Create();
    int64_byte_args->Add(int64_arg);
    int64_byte_args->Add(byte_div_arg);

    int64_dcl_args = FdoArgumentDefinitionCollection::Create();
    int64_dcl_args->Add(int64_arg);
    int64_dcl_args->Add(dcl_div_arg);

    int64_dbl_args = FdoArgumentDefinitionCollection::Create();
    int64_dbl_args->Add(int64_arg);
    int64_dbl_args->Add(dbl_div_arg);

    int64_int16_args = FdoArgumentDefinitionCollection::Create();
    int64_int16_args->Add(int64_arg);
    int64_int16_args->Add(int16_div_arg);

    int64_int32_args = FdoArgumentDefinitionCollection::Create();
    int64_int32_args->Add(int64_arg);
    int64_int32_args->Add(int32_div_arg);

    int64_int64_args = FdoArgumentDefinitionCollection::Create();
    int64_int64_args->Add(int64_arg);
    int64_int64_args->Add(int64_div_arg);

    int64_sgl_args = FdoArgumentDefinitionCollection::Create();
    int64_sgl_args->Add(int64_arg);
    int64_sgl_args->Add(sgl_div_arg);

    sgl_byte_args = FdoArgumentDefinitionCollection::Create();
    sgl_byte_args->Add(sgl_arg);
    sgl_byte_args->Add(byte_div_arg);

    sgl_dcl_args = FdoArgumentDefinitionCollection::Create();
    sgl_dcl_args->Add(sgl_arg);
    sgl_dcl_args->Add(dcl_div_arg);

    sgl_dbl_args = FdoArgumentDefinitionCollection::Create();
    sgl_dbl_args->Add(sgl_arg);
    sgl_dbl_args->Add(dbl_div_arg);

    sgl_int16_args = FdoArgumentDefinitionCollection::Create();
    sgl_int16_args->Add(sgl_arg);
    sgl_int16_args->Add(int16_div_arg);

    sgl_int32_args = FdoArgumentDefinitionCollection::Create();
    sgl_int32_args->Add(sgl_arg);
    sgl_int32_args->Add(int32_div_arg);

    sgl_int64_args = FdoArgumentDefinitionCollection::Create();
    sgl_int64_args->Add(sgl_arg);
    sgl_int64_args->Add(int64_div_arg);

    sgl_sgl_args = FdoArgumentDefinitionCollection::Create();
    sgl_sgl_args->Add(sgl_arg);
    sgl_sgl_args->Add(sgl_div_arg);

    // Create the signature collection.

    signatures = FdoSignatureDefinitionCollection::Create();

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Int16, byte_byte_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Double, byte_dcl_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Double, byte_dbl_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Int16, byte_int16_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Int32, byte_int32_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Int64, byte_int64_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Single, byte_sgl_args);
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
                                        FdoDataType_Int16, int16_byte_args);
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
                                        FdoDataType_Int16, int16_int32_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Int16, int16_int64_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Single, int16_sgl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Int32, int32_byte_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Double, int32_dcl_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Double, int32_dbl_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Int16, int32_int16_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Int32, int32_int32_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Int32, int32_int64_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Single, int32_sgl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Int64, int64_byte_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Double, int64_dcl_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Double, int64_dbl_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Int16, int64_int16_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Int32, int64_int32_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Int64, int64_int64_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Single, int64_sgl_args);
    signatures->Add(signature);

    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Single, sgl_byte_args);
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
                                        FdoDataType_Single, sgl_int32_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Single, sgl_int64_args);
    signatures->Add(signature);
    signature = FdoSignatureDefinition::Create(
                                        FdoDataType_Single, sgl_sgl_args);
    signatures->Add(signature);

    // Create the function definition.

    desc =
        FdoException::NLSGetMessage(
                    FUNCTION_REMAINDER,
                    "Determines the remainder of the division of two numeric expressions");
    function_definition =
                FdoFunctionDefinition::Create(
                                        FDO_FUNCTION_REMAINDER,
                                        desc,
                                        false,
                                        signatures,
                                        FdoFunctionCategoryType_Math);

}  //  CreateFunctionDefinition ()

FdoDouble FdoFunctionRemainder::GetInt64AbsValue (FdoInt64 value)

// +---------------------------------------------------------------------------
// | The function determines the absolute value of the given Int64 value.
// +---------------------------------------------------------------------------

{

    return (FdoDouble)((value > 0) ? value : (-1 * value)); 

}  //  GetInt64AbsValue ()

FdoDataType FdoFunctionRemainder::GetReturnDataType (
                                                FdoDataType inp1_data_type,
                                                FdoDataType inp2_data_type)

// +---------------------------------------------------------------------------
// | Based on the provided data types of the input parameters, the function
// | determines the data type to be used to communicate the function result
// | back to the caller.
// +---------------------------------------------------------------------------

{

    if ((inp1_data_type == FdoDataType_Byte       ) &&
        ((inp2_data_type == FdoDataType_Byte ) ||
         (inp2_data_type == FdoDataType_Int16)    )    )
        return FdoDataType_Int16;

    if ((inp1_data_type == FdoDataType_Byte ) &&
        (inp2_data_type == FdoDataType_Int32)    )
        return FdoDataType_Int32;

    if ((inp1_data_type == FdoDataType_Byte ) &&
        (inp2_data_type == FdoDataType_Int64)    )
        return FdoDataType_Byte;

    if ((inp1_data_type == FdoDataType_Decimal) || 
        (inp1_data_type == FdoDataType_Double )    )
        return FdoDataType_Double;

    if ((inp2_data_type == FdoDataType_Decimal) ||
        (inp2_data_type == FdoDataType_Double )    )
        return FdoDataType_Double;

    if (inp2_data_type == FdoDataType_Single)
        return FdoDataType_Single;

    if (inp1_data_type == FdoDataType_Int16)
        return FdoDataType_Int16;

    if ((inp1_data_type == FdoDataType_Int32)       &&
        ((inp2_data_type == FdoDataType_Byte ) ||
         (inp2_data_type == FdoDataType_Int32) || 
         (inp2_data_type == FdoDataType_Int64)    )    )
        return FdoDataType_Int32;

    if ((inp1_data_type == FdoDataType_Int64) &&
        (inp2_data_type == FdoDataType_Int32)    )
        return FdoDataType_Int32;

    if ((inp1_data_type == FdoDataType_Int64) &&
        (inp2_data_type == FdoDataType_Int64)    )
        return FdoDataType_Int64;

    // The last combination to handle is the case where the first parameter
    // data type is a SINGLE (basically, the cases that are not yet covered
    // by the previous cases via the data type of the second parameter). To
    // avoid compiler warnings, the function just returns the necessary data
    // type without specific checking.
 
    return FdoDataType_Single;

}  //  GetReturnDataType ()

FdoDouble FdoFunctionRemainder::Round (FdoDouble value)

// +---------------------------------------------------------------------------
// | The function rounds the provided value.
// +---------------------------------------------------------------------------

{

    if ((value - floor(value)) <= 0.5)
        return floor(value);
    else
      return ceil(value); 

}  //  Round ()

void FdoFunctionRemainder::Validate (FdoLiteralValueCollection *literal_values)

// +---------------------------------------------------------------------------
// | The function validates the argument list that was passed in.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoInt32                i               = 0,
                            count           = literal_values->GetCount();

    FdoDataValue            *data_value     = NULL;

    FdoPtr<FdoLiteralValue> literal_value;

    // Check the number of arguments. REMAINDER accepts two numeric para-
    // meters. If the number of parameters is not correct issue an exception.

    if (count != 2) 
        throw FdoException::Create(
                FdoException::NLSGetMessage(
                  FUNCTION_PARAMETER_NUMBER_ERROR, 
                  "Expression Engine: Invalid number of parameters for function '%1$ls'",
                  FDO_FUNCTION_REMAINDER));

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
                      FDO_FUNCTION_REMAINDER));

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
                  FDO_FUNCTION_REMAINDER));

}  //  Validate ()

