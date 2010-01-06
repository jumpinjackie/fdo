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

    first = true;
    tmp_buffer = NULL;


}  //  FdoFunctionSoundex ()


FdoFunctionSoundex::~FdoFunctionSoundex ()

// +---------------------------------------------------------------------------
// | The class destructor.
// +---------------------------------------------------------------------------

{

    // Delete the function definition.

    FDO_SAFE_RELEASE(function_definition);

    delete [] tmp_buffer;
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

    FdoString              *work_string;

    FdoPtr<FdoStringValue> string_value;

    if (first)
    {
        Validate(literal_values);
        return_string_value = FdoStringValue::Create();
        tmp_buffer      = new wchar_t[INIT_ALLOCATE_SIZE+1];
        tmp_buffer_size = INIT_ALLOCATE_SIZE;
        first = false;
    }

    // Get the string that needs to be processed. If no value is provided,
    // terminate the function.

    string_value = (FdoStringValue *) literal_values->GetItem(0);
    if (string_value->IsNull())
    {
        return_string_value->SetNull();
        return FDO_SAFE_ADDREF(return_string_value.p);
    }

    // Make a work-copy of the provided string.

    work_string = string_value->GetString();

    // Get the length of the provided string. If it is 0, then there is nothing
    // to do and the functin can terminate.

    str_length = (FdoInt64)wcslen(work_string);
    if (str_length == 0)
    {
        return_string_value->SetNull();
        return FDO_SAFE_ADDREF(return_string_value.p);
    }

    if (str_length > tmp_buffer_size)
    {
        delete [] tmp_buffer;
        tmp_buffer_size = (size_t) str_length;
        tmp_buffer      = new wchar_t[tmp_buffer_size + 1];

    }
    
    wcscpy(tmp_buffer, work_string);

    // Execute the first step of the algorithm and remove all punctation marks
    // and numbers. For the resulting string, check the length. If it is 0,
    // nothing else needs to be done and the function can terminate.

    EliminateNonAlphaChars(tmp_buffer, str_length);
    str_length  = (FdoInt64)wcslen(tmp_buffer);
    if (str_length == 0)
    {
        return_string_value->SetNull();
        return FDO_SAFE_ADDREF(return_string_value.p);
    }

    // If there is just one letter left, add three trailing zeros to the string
    // and return it back to the calling routine.

    if (str_length == 1) {

        FdoCommonOSUtil::wcsupr(tmp_buffer);
        wcscat(tmp_buffer, L"000");
        return_string_value->SetString(tmp_buffer);
        return FDO_SAFE_ADDREF(return_string_value.p);

    }  //  if (str_length == 1) ...

    // Capitalize the remaining string. Retain the first letter (step 2) and
    // substitute letters with numbers as outlined in the algorithm (step 3,
    // 4) for the remaining string.

    FdoCommonOSUtil::wcsupr(tmp_buffer);

    for (int i=1; i<(int) wcslen(tmp_buffer); i++)
    {
        switch (tmp_buffer[i])
        {
        case 'A':
        case 'E':
        case 'I':
        case 'O':
        case 'U':
        case 'H':
        case 'W':
            tmp_buffer[i] = '0';
            break;
        case 'B':
        case 'F':
        case 'P':
        case 'V':
            tmp_buffer[i] = '1';
            break;
        case 'C':
        case 'G':
        case 'J':
        case 'K':
        case 'Q':
        case 'S':
        case 'X':
        case 'Z':
            tmp_buffer[i] = '2';
            break;
        case 'D':
        case 'T':
            tmp_buffer[i] = '3';
            break;
        case 'L':
            tmp_buffer[i] = '4';
            break;
        case 'M':
        case 'N':
            tmp_buffer[i] = '5';
            break;
        case 'R':
            tmp_buffer[i] = '6';
            break;

        }
    }


    // Eliminate all duplicate numbers if they are besides each other (step 6).

    str_length  = wcslen(tmp_buffer);
    EliminateDuplicateNumbers(tmp_buffer+1, str_length-1);

    // Eliminate all zeros.

    str_length  = wcslen(tmp_buffer);
    EliminateZeros (tmp_buffer+1, str_length-1);

    // Construct the resulting string and return the value back to the calling
    // routine.

    size_t size = wcslen(tmp_buffer);
    if (size < 4)
    {
        for (int i=(int)size; i<4; i++)
        {
            tmp_buffer[i] = L'0';
        }
    }
    tmp_buffer[4] = '\0';
    
    return_string_value->SetString(tmp_buffer);
    return FDO_SAFE_ADDREF(return_string_value.p);
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
    FdoPtr<FdoSignatureDefinitionCollection> signatures;

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

void FdoFunctionSoundex::EliminateDuplicateNumbers (wchar_t* value,
                                                          FdoInt64   length
                                                          )

// +---------------------------------------------------------------------------
// | The function executes the sixth step as outlined in the algorithm for the
// | function SOUNDEX and eliminates all duplicate numbers.
// +---------------------------------------------------------------------------

{
    // Declare and initialize all necessary local variables.

    FdoInt64   loop_count         = 0;

    wchar_t curr_char;

    // Loop through each of the characters in the given string and remove any
    // duplicate numbers that are adjacent to each other (the first occurance
    // of the number is kept.

    wchar_t comp_char = value[0];
    wchar_t* ptr = value;
    ptr++;
    for (loop_count = 1; loop_count < length; loop_count++) {

        curr_char = value[loop_count];
        if (curr_char != comp_char) {
            *ptr = curr_char;
            ptr++;
            comp_char        = value[loop_count];

        }  //  if (curr_char != comp_char) ...

    }  //  for (loop_count = 0; loop_count < length; loop_count++) ...
    *ptr = '\0';
}  //  EliminateDuplicateNumbers ()

void FdoFunctionSoundex::EliminateNonAlphaChars (wchar_t* value,
                                                       FdoInt64   length)

// +---------------------------------------------------------------------------
// | The function executes the first step as outlined in the algorithm for the
// | function SOUNDEX and eliminates all characters that are not letters.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoInt64      loop_count         = 0;

    // Loop through each of the characters in the given string and remove any
    // characters that is not a letter.

    wchar_t *ptr = value;
    for (loop_count = 0; loop_count < length; loop_count++) {

      if (isalpha(value[loop_count]))
      {
          *ptr = value[loop_count];
          ptr++;
      }
    }  //  for (loop_count = 0; loop_count < length; loop_count++) ...
    *ptr = '\0';

}  //  EliminateNonAlphaChars ()

void FdoFunctionSoundex::EliminateZeros (wchar_t* value,
                                               FdoInt64   length)

// +---------------------------------------------------------------------------
// | The function executes the sixth step as outlined in the algorithm for the
// | function SOUNDEX and eliminates all zeros.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoInt64   loop_count         = 0;

    // Loop through each of the characters in the given string and remove any
    // zeros.

    wchar_t *ptr = value;
    for (loop_count = 0; loop_count < length; loop_count++) {
        if (value[loop_count] != L'0')
        {
            *ptr = value[loop_count];
            ptr++;
        }

    }  //  for (loop_count = 0; loop_count < length; loop_count++) ...
    *ptr = '\0';
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

