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
#include <Functions/String/FdoFunctionSoundex.h>


// ----------------------------------------------------------------------------
// --                         Constructors/Destructors                       --
// ----------------------------------------------------------------------------

FdoFunctionSoundex::FdoFunctionSoundex ()

// +---------------------------------------------------------------------------
// | The class constructor.
// +---------------------------------------------------------------------------

{

    // Initialize all class variables.

    function_definition = NULL;

}  //  FdoFunctionSoundex ()


FdoFunctionSoundex::~FdoFunctionSoundex ()

// +---------------------------------------------------------------------------
// | The class destructor.
// +---------------------------------------------------------------------------

{

    // Delete the function definition.

    FDO_SAFE_RELEASE(function_definition);

}  //  ~FdoFunctionSoundex ()


// ----------------------------------------------------------------------------
// --                            Public Class APIs                           --
// ----------------------------------------------------------------------------

FdoFunctionSoundex *FdoFunctionSoundex::Create ()

// +---------------------------------------------------------------------------
// | The function creates a new instance of the class.
// +---------------------------------------------------------------------------

{

    return new FdoFunctionSoundex();

}  //  Create ()

FdoFunctionSoundex *FdoFunctionSoundex::CreateObject ()

// +---------------------------------------------------------------------------
// | The function creates a new instance of the class.
// +---------------------------------------------------------------------------

{

    return new FdoFunctionSoundex();

}  //  CreateObject ()

FdoFunctionDefinition *FdoFunctionSoundex::GetFunctionDefinition ()

// +---------------------------------------------------------------------------
// | The function creates the supported signature list for the function
// | SOUNDEX.
// +---------------------------------------------------------------------------

{

    if (function_definition == NULL)
        CreateFunctionDefinition();

    return FDO_SAFE_ADDREF(function_definition);

}  //  GetFunctionDefinition ()

FdoLiteralValue *FdoFunctionSoundex::Evaluate (
                                    FdoLiteralValueCollection *literal_values)

// +---------------------------------------------------------------------------
// | The function processes a call to the function SOUNDEX.
// | NOTE:
// | The implementation of the function utilizes the following algorithm:
// |
// |   Step 1: Capitalize all letters in the word and remove all punctation
// |           marks and digits.
// |   Step 2: Retain the first letter of the word.
// |   Step 3: Replace any letter from the set {A, E, I, O, U, H, W, Y} with
// |           a 0 (zero).
// |   Step 4: Replace any letter from the set {B, F, P, V} with a 1.
// |           Replace any letter from the set {C, G, J, K, Q, S, X, Z) with
// |           a 2
// |           Replace any letter from the set {D, T) with a 3
// |           Replace any letter from the set {L) with a 4
// |           Replace any letter from the set {M, N) with a 5
// |           Replace any letter from the set {R) with a 6
// |   Step 5: Remove all pairs of digits which occur beside each other from
// |           the that resulted from step 4.
// |   Step 6: Remove all the zeros from the string that resulted from step 5.
// |   Step 7: Pad the string resulting from step 6 with trailing zeros and
// |           return the first 4 positions (resulting in a string of the 
// |           structure <letter><number><number><number>).
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoInt64               str_length;

    FdoStringP             base_string,
                           work_string;

    FdoPtr<FdoStringValue> string_value;

    // Validate the function call.

    Validate(literal_values);

    // Get the string that needs to be processed. If no value is provided,
    // terminate the function.

    string_value = (FdoStringValue *) literal_values->GetItem(0);
    if (string_value->IsNull())
        return FdoStringValue::Create();

    // Make a work-copy of the provided string.

    work_string = string_value->GetString();

    // Get the length of the provided string. If it is 0, then there is nothing
    // to do and the functin can terminate.

    str_length = (FdoInt64)work_string.GetLength();
    if (str_length == 0)
        return FdoStringValue::Create();

    // Check if the string contains numbers only. If this is the case, nothing
    // else needs to be done and the function can terminate.

    if (work_string.IsNumber())
        return FdoStringValue::Create();

    // Execute the first step of the algorithm and remove all punctation marks
    // and numbers. For the resulting string, check the length. If it is 0,
    // nothing else needs to be done and the function can terminate.

    work_string = EliminateNonAlphaChars(work_string, str_length);
    str_length  = (FdoInt64)work_string.GetLength();
    if (str_length == 0)
        return FdoStringValue::Create();

    // If there is just one letter left, add three trailing zeros to the string
    // and return it back to the calling routine.

    if (str_length == 1) {

        base_string = work_string.Mid(0, 1);
        base_string = base_string.Upper();
        base_string = base_string + L"000";
        return FdoStringValue::Create((FdoString *)base_string);

    }  //  if (str_length == 1) ...

    // Capitalize the remaining string. Retain the first letter (step 2) and
    // substitute letters with numbers as outlined in the algorithm (step 3,
    // 4) for the remaining string.

    work_string = work_string.Upper();
    base_string = work_string.Mid(0, 1);

    work_string = work_string.Mid(1, (size_t)str_length+1);
    work_string = work_string.Replace(L"A", L"0");
    work_string = work_string.Replace(L"E", L"0");
    work_string = work_string.Replace(L"I", L"0");
    work_string = work_string.Replace(L"O", L"0");
    work_string = work_string.Replace(L"U", L"0");
    work_string = work_string.Replace(L"H", L"0");
    work_string = work_string.Replace(L"W", L"0");
    work_string = work_string.Replace(L"Z", L"0");
    work_string = work_string.Replace(L"B", L"1");
    work_string = work_string.Replace(L"F", L"1");
    work_string = work_string.Replace(L"P", L"1");
    work_string = work_string.Replace(L"V", L"1");
    work_string = work_string.Replace(L"C", L"2");
    work_string = work_string.Replace(L"G", L"2");
    work_string = work_string.Replace(L"J", L"2");
    work_string = work_string.Replace(L"K", L"2");
    work_string = work_string.Replace(L"Q", L"2");
    work_string = work_string.Replace(L"S", L"2");
    work_string = work_string.Replace(L"X", L"2");
    work_string = work_string.Replace(L"Z", L"2");
    work_string = work_string.Replace(L"D", L"3");
    work_string = work_string.Replace(L"T", L"3");
    work_string = work_string.Replace(L"L", L"4");
    work_string = work_string.Replace(L"M", L"5");
    work_string = work_string.Replace(L"N", L"5");
    work_string = work_string.Replace(L"R", L"6");

    // Eliminate all duplicate numbers if they are besides each other (step 6).

    str_length  = work_string.GetLength();
    work_string = EliminateDuplicateNumbers(work_string, str_length);

    // Eliminate all zeros.

    str_length  = work_string.GetLength();
    work_string = EliminateZeros (work_string, str_length);

    // Construct the resulting string and return the value back to the calling
    // routine.

    base_string = base_string + work_string.Mid(0, 3);
    while (base_string.GetLength() < 4)
      base_string = base_string + L"0";

    return FdoStringValue::Create((FdoString *)base_string);

}  //  Evaluate ()


// ----------------------------------------------------------------------------
// --                          Supporting functions                          --
// ----------------------------------------------------------------------------

void FdoFunctionSoundex::CreateFunctionDefinition ()

// +---------------------------------------------------------------------------
// | The procedure creates the function definition for the function SOUNDEX.
// | The function definition includes the list of supported signatures. The
// | following signatures are supported:
// |
// |    SOUNDEX (string)
// |
// | The function always returns a STRING.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoString                               *desc               = NULL;

    FdoStringP                              arg1_description;
    FdoStringP                              str_arg_literal;

    FdoPtr<FdoArgumentDefinition>           str_arg;

    FdoPtr<FdoArgumentDefinitionCollection> str_args;

    FdoPtr<FdoSignatureDefinition>          signature;
    FdoSignatureDefinitionCollection        *signatures;

    // Get the general descriptions for the arguments.

    arg1_description =
        FdoException::NLSGetMessage(
                    FUNCTION_SOUNDEX_STRING_ARG,
                    "String for which to identify phonetic representation");

    // The following defines the different argument definition collections.

    str_arg_literal = FdoException::NLSGetMessage(FUNCTION_STRING_ARG_LIT,
                                                  "text property");

    str_arg   = FdoArgumentDefinition::Create(
                    str_arg_literal, arg1_description, FdoDataType_String);

    str_args = FdoArgumentDefinitionCollection::Create();
    str_args->Add(str_arg);

    // Create the signature collection.

    signatures = FdoSignatureDefinitionCollection::Create();

    signature = FdoSignatureDefinition::Create(FdoDataType_String, str_args);
    signatures->Add(signature);

    // Create the function definition.

    desc =
        FdoException::NLSGetMessage(
                FUNCTION_SOUNDEX,
                "Returns the phonetic representation of a string expression");
    function_definition =
                FdoFunctionDefinition::Create(
                                        FDO_FUNCTION_SOUNDEX,
                                        desc,
                                        false,
                                        signatures,
                                        FdoFunctionCategoryType_String);

}  //  CreateFunctionDefinition ()

FdoStringP FdoFunctionSoundex::EliminateDuplicateNumbers (FdoStringP value,
                                                          FdoInt64   length)

// +---------------------------------------------------------------------------
// | The function executes the sixth step as outlined in the algorithm for the
// | function SOUNDEX and eliminates all duplicate numbers.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoInt64   loop_count         = 0;

    FdoStringP curr_char,
               comp_char,
               resulting_string;

    // Loop through each of the characters in the given string and remove any
    // duplicate numbers that are adjacent to each other (the first occurance
    // of the number is kept.

    comp_char        = value.Mid(0, 1);
    resulting_string = resulting_string + comp_char;

    for (loop_count = 1; loop_count < length; loop_count++) {

      curr_char = value.Mid((size_t)loop_count, 1);
      if (curr_char != comp_char) {

          resulting_string = resulting_string + curr_char;
          comp_char        = value.Mid((size_t)loop_count, 1);

      }  //  if (curr_char != comp_char) ...

    }  //  for (loop_count = 0; loop_count < length; loop_count++) ...

    return resulting_string;

}  //  EliminateDuplicateNumbers ()

FdoStringP FdoFunctionSoundex::EliminateNonAlphaChars (FdoStringP value,
                                                       FdoInt64   length)

// +---------------------------------------------------------------------------
// | The function executes the first step as outlined in the algorithm for the
// | function SOUNDEX and eliminates all characters that are not letters.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoInt64      loop_count         = 0;

    FdoStringP    curr_char,
                  resulting_string;

    unsigned char *work_char         = NULL;

    // Loop through each of the characters in the given string and remove any
    // characters that is not a letter.

    for (loop_count = 0; loop_count < length; loop_count++) {

      curr_char = value.Mid((size_t)loop_count, 1);
      work_char = (unsigned char*)((const char *)curr_char);

      if (isalpha(work_char[0]))
          resulting_string = resulting_string + curr_char;

    }  //  for (loop_count = 0; loop_count < length; loop_count++) ...

    return resulting_string;

}  //  EliminateNonAlphaChars ()

FdoStringP FdoFunctionSoundex::EliminateZeros (FdoStringP value,
                                               FdoInt64   length)

// +---------------------------------------------------------------------------
// | The function executes the sixth step as outlined in the algorithm for the
// | function SOUNDEX and eliminates all zeros.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoInt64   loop_count         = 0;

    FdoStringP curr_char,
               resulting_string;

    // Loop through each of the characters in the given string and remove any
    // zeros.

    for (loop_count = 0; loop_count < length; loop_count++) {

      curr_char = value.Mid((size_t)loop_count, 1);
      if (curr_char != L"0")
          resulting_string = resulting_string + curr_char;

    }  //  for (loop_count = 0; loop_count < length; loop_count++) ...

    return resulting_string;

}  //  EliminateZeros ()

void FdoFunctionSoundex::Validate (FdoLiteralValueCollection *literal_values)

// +---------------------------------------------------------------------------
// | The function validates the argument list that was passed in.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoInt32                count           = literal_values->GetCount();

    FdoDataType             data_type;

    FdoDataValue            *data_value     = NULL;

    FdoPtr<FdoLiteralValue> literal_value;

    // Check the number of arguments. SOUNDEX accepts one parameter only. If
    // the number of parameters is not correct issue an exception.

    count = literal_values->GetCount();

    if (count != 1) 
        throw FdoException::Create(
               FdoException::NLSGetMessage(
                  FUNCTION_PARAMETER_NUMBER_ERROR, 
                  "Expression Engine: Invalid number of parameters for function '%1$ls'",
                  FDO_FUNCTION_SOUNDEX));

    // Next, identify the data type associated with the value to be processed.
    // An exception is issued if the data type does not match any of the ones
    // the function supports

    literal_value = literal_values->GetItem(0);
    if (literal_value->GetLiteralValueType() != FdoLiteralValueType_Data)
        throw FdoException::Create(
                FdoException::NLSGetMessage(
                  FUNCTION_PARAMETER_ERROR, 
                  "Expression Engine: Invalid parameters for function '%1$ls'",
                  FDO_FUNCTION_SOUNDEX));

    data_value = static_cast<FdoDataValue *>(literal_value.p);
    data_type = data_value->GetDataType();
    if (data_type != FdoDataType_String)
        throw FdoException::Create(
                FdoException::NLSGetMessage(
                  FUNCTION_PARAMETER_DATA_TYPE_ERROR, 
                  "Expression Engine: Invalid parameter data type for function '%1$ls'",
                  FDO_FUNCTION_SOUNDEX));

}  //  Validate ()

