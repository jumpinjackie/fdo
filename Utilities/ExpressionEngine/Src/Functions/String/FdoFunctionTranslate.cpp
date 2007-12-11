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
#include <Functions/String/FdoFunctionTranslate.h>


// ----------------------------------------------------------------------------
// --                         Constructors/Destructors                       --
// ----------------------------------------------------------------------------

FdoFunctionTranslate::FdoFunctionTranslate ()

// +---------------------------------------------------------------------------
// | The class constructor.
// +---------------------------------------------------------------------------

{

    // Initialize all class variables.

    function_definition = NULL;

    first = true;
    tmp_buffer = NULL;

}  //  FdoFunctionTranslate ()


FdoFunctionTranslate::~FdoFunctionTranslate ()

// +---------------------------------------------------------------------------
// | The class destructor.
// +---------------------------------------------------------------------------

{

    // Delete the function definition.

    FDO_SAFE_RELEASE(function_definition);
    delete [] tmp_buffer;

}  //  ~FdoFunctionTranslate ()


// ----------------------------------------------------------------------------
// --                            Public Class APIs                           --
// ----------------------------------------------------------------------------

FdoFunctionTranslate *FdoFunctionTranslate::Create ()

// +---------------------------------------------------------------------------
// | The function creates a new instance of the class.
// +---------------------------------------------------------------------------

{

    return new FdoFunctionTranslate();

}  //  Create ()

FdoFunctionTranslate *FdoFunctionTranslate::CreateObject ()

// +---------------------------------------------------------------------------
// | The function creates a new instance of the class.
// +---------------------------------------------------------------------------

{

    return new FdoFunctionTranslate();

}  //  CreateObject ()

FdoFunctionDefinition *FdoFunctionTranslate::GetFunctionDefinition ()

// +---------------------------------------------------------------------------
// | The function creates the supported signature list for the function TRANS-
// | LATE.
// +---------------------------------------------------------------------------

{

    if (function_definition == NULL)
        CreateFunctionDefinition();

    return FDO_SAFE_ADDREF(function_definition);

}  //  GetFunctionDefinition ()

FdoLiteralValue *FdoFunctionTranslate::Evaluate (
                                    FdoLiteralValueCollection *literal_values)

// +---------------------------------------------------------------------------
// | The function processes a call to the function TRANSLATE.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoInt32               i,
                           pos,
                           to_set_length;
    size_t                 from_set_length,
                           source_string_length;
    wchar_t                curr_char[2];
    FdoString              *source_string,
                           *from_char_set,
                           *to_char_set;

    FdoPtr<FdoStringValue> string_value;

    if (first)
    {
        Validate(literal_values);
        return_string_value = FdoStringValue::Create();
        tmp_buffer      = new wchar_t[INIT_ALLOCATE_SIZE+1];
        tmp_buffer_size = INIT_ALLOCATE_SIZE;
        first = false;
    }

    // Get the source string and the two character sets involved in the opera-
    // tion. If any of the provided strings is empty, return an empty string
    // as the function result.

    for (i = 0; i < 3; i++) {

      string_value = (FdoStringValue *) literal_values->GetItem(i);
      if (string_value->IsNull())
          return FdoStringValue::Create();
      else
        switch (i) {

          case 0:
            source_string = string_value->GetString();
            break;

          case 1:
            from_char_set = string_value->GetString();
            break;

          case 2:
            to_char_set = string_value->GetString();
            break;

        }  //  switch ...

    }  //  for (i = 0; i < 3; i++) ...

    // Get the length of the character sets involved.

    to_set_length = (FdoInt32) wcslen(to_char_set);
    from_set_length = (FdoInt32) wcslen(from_char_set);
    source_string_length = (FdoInt32) wcslen(source_string);

    if (from_set_length > tmp_buffer_size) {

        delete [] tmp_buffer;
        tmp_buffer_size = from_set_length;
        tmp_buffer      = new wchar_t[tmp_buffer_size + 1];

    }

    // Navigate through the source string and execute the replacement. The
    // following rules apply:
    //
    //  - any character in the from-set is replaced with the character in
    //    the to-set at the same position.
    //  - if the character in the from-set does not have a replacement
    //    character in the to-set at the same position, the character is
    //    deleted.
    //
    // NOTE: It is not possible to use the function REPLACE offered with the
    //       class FdoStringP because this may result in incorrect results.
    //       For example, if the call is TRANSLATE('abcd', 'ae', 'eS'), the
    //       result should be 'ebcd', not 'Sbcd' which would be the case if
    //       the mentioned function is used to do the job.

    for (i = 0; i < (int) source_string_length; i++) {

      curr_char[0] = source_string[i];
      curr_char[1] = '\0';
      if (wcsstr(from_char_set, curr_char) != NULL) {

          pos = GetFromSetPosition(from_char_set, (FdoInt32) from_set_length, curr_char);
          if (pos <= to_set_length)
              tmp_buffer[i] = to_char_set[pos];

      }  //  if (from_char_set.Contains( ...
      else
        tmp_buffer[i] = curr_char[0];

    }  //  for (i = 0; ...
     tmp_buffer[i] = '\0';

    // Return the resulting string back to the calling routine.

    
    return_string_value->SetString(tmp_buffer);
    return FDO_SAFE_ADDREF(return_string_value.p);

}  //  Evaluate ()


// ----------------------------------------------------------------------------
// --                          Supporting functions                          --
// ----------------------------------------------------------------------------

void FdoFunctionTranslate::CreateFunctionDefinition ()

// +---------------------------------------------------------------------------
// | The procedure creates the function definition for the function TRANSLATE.
// | The function definition includes the list of supported signatures. The
// | following signatures are supported:
// |
// |    TRANSLATE (string, string)
// |
// | The function always returns a STRING.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoString                               *desc               = NULL;

    FdoStringP                              arg1_description;
    FdoStringP                              arg2_description;
    FdoStringP                              arg3_description;
    FdoStringP                              arg1_literal;
    FdoStringP                              arg2_literal;
    FdoStringP                              arg3_literal;

    FdoPtr<FdoArgumentDefinition>           str1_arg;
    FdoPtr<FdoArgumentDefinition>           str2_arg;
    FdoPtr<FdoArgumentDefinition>           str3_arg;

    FdoPtr<FdoArgumentDefinitionCollection> str_args;

    FdoPtr<FdoSignatureDefinition>          signature;
    FdoSignatureDefinitionCollection        *signatures;

    // Get the general descriptions for the arguments.

    arg1_description = FdoException::NLSGetMessage(
                            FUNCTION_TRANSLATE_STRING1_ARG,
                            "String where character replacement is executed");

    arg2_description =
        FdoException::NLSGetMessage(
                FUNCTION_TRANSLATE_STRING2_ARG,
                "String identifying individual characters to be replaced");

    arg3_description =
        FdoException::NLSGetMessage(
                    FUNCTION_TRANSLATE_STRING3_ARG,
                    "String identifying individual replacement characters");

    // The following defines the different argument definition collections.

    arg1_literal = FdoException::NLSGetMessage(FUNCTION_STRING_ARG_LIT,
                                               "text property");

    arg2_literal = FdoException::NLSGetMessage(FUNCTION_TRANSLATE_FROM_ARG_LIT,
                                               "from-characterset");

    arg3_literal = FdoException::NLSGetMessage(FUNCTION_TRANSLATE_TO_ARG_LIT,
                                               "to-characterset");

    str1_arg = FdoArgumentDefinition::Create(
                    arg1_literal, arg1_description, FdoDataType_String);
    str2_arg = FdoArgumentDefinition::Create(
                    arg2_literal, arg2_description, FdoDataType_String);
    str3_arg = FdoArgumentDefinition::Create(
                    arg3_literal, arg3_description, FdoDataType_String);

    str_args = FdoArgumentDefinitionCollection::Create();
    str_args->Add(str1_arg);
    str_args->Add(str2_arg);
    str_args->Add(str3_arg);

    // Create the signature collection.

    signatures = FdoSignatureDefinitionCollection::Create();

    signature = FdoSignatureDefinition::Create(FdoDataType_String, str_args);
    signatures->Add(signature);

    // Create the function definition.

    desc = FdoException::NLSGetMessage(
                FUNCTION_TRANSLATE,
                "Replaces a set of individual characters in a string with replacements");
    function_definition =
                FdoFunctionDefinition::Create(
                                        FDO_FUNCTION_TRANSLATE,
                                        desc,
                                        false,
                                        signatures,
                                        FdoFunctionCategoryType_String);

}  //  CreateFunctionDefinition ()

FdoInt32 FdoFunctionTranslate::GetFromSetPosition (FdoString* char_set,
                                                   FdoInt32   set_length,
                                                   FdoString* curr_char)

// +---------------------------------------------------------------------------
// | The function returns the position of the provided character in the set of
// | characters that need to be replaced.
// +---------------------------------------------------------------------------

{


    // Declare and initialize all necessary local variables.

    FdoInt32   pos = 0;

    wchar_t tmp_char;

    // Navigate the given string from the left, find the position of the pro-
    // vided character and return the position back to the calling routine.

    while (pos < set_length) {

      tmp_char = char_set[pos];
      if (tmp_char == *curr_char)
          break;
      pos++;

    }  //  while (pos < string_length) ...

    return pos;

}  //  GetFromSetPosition ()

void FdoFunctionTranslate::Validate (FdoLiteralValueCollection *literal_values)

// +---------------------------------------------------------------------------
// | The function validates the argument list that was passed in.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoInt32                i,
                            count           = literal_values->GetCount();

    FdoDataType             data_type;

    FdoDataValue            *data_value     = NULL;

    FdoPtr<FdoLiteralValue> literal_value;

    // Check the number of arguments. TRANSLATE accepts three parameters. If the
    // number of parameters is not correct issue an exception.

    if (count != 3) 
        throw FdoException::Create(
                FdoException::NLSGetMessage(
                  FUNCTION_PARAMETER_NUMBER_ERROR, 
                  "Expression Engine: Invalid number of parameters for function '%1$ls'",
                  FDO_FUNCTION_TRANSLATE));

    // Next query the list of arguments and verify that the provided types
    //  match those defined in the signatures. If there is an error issue an
    // exception.

    for (i = 0; i < count; i++) {

      literal_value = literal_values->GetItem(i);
      if (literal_value->GetLiteralValueType() != FdoLiteralValueType_Data)
          throw FdoException::Create(
                 FdoException::NLSGetMessage(
                    FUNCTION_PARAMETER_ERROR, 
                    "Expression Engine: Invalid parameters for function '%1$ls'",
                    FDO_FUNCTION_TRANSLATE));

      data_value = static_cast<FdoDataValue *>(literal_value.p);
      data_type = data_value->GetDataType();
      if (data_type != FdoDataType_String)
          throw FdoException::Create(
                 FdoException::NLSGetMessage(
                   FUNCTION_PARAMETER_DATA_TYPE_ERROR, 
                   "Expression Engine: Invalid parameter data type for function '%1$ls'",
                   FDO_FUNCTION_TRANSLATE));

    }  //  for (i = 0; i < count; i++) ...

}  //  Validate ()

