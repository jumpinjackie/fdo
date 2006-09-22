/***************************************************************************
 * Copyright (C) 2004-2006  Autodesk, Inc.
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of version 2.1 of the GNU Lesser
 * General Public License as published by the Free Software Foundation.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 ***************************************************************************/

// =============================================================================
// | The file contains the implementation of the class LockUtility. The class
// | implements functions common to lock related operations.
// =============================================================================

// The links to the header(s) to be included.

#include "stdafx.h"
#include <malloc.h>

#include "FdoCommonOSUtil.h"
#include "LockUtility.h"
//#include "inc/adb/col_names.h"
#include "FdoRdbmsFilterUtil.h"
#include "FdoRdbmsFilterProcessor.h"
#include "FdoRdbmsSchemaUtil.h"
#include "FdoRdbmsException.h"
#include "Inc/ut.h"

#include "FdoCommonOSUtil.h"


// ----------------------------------------------------------------------------
// --                            Public functions                            --
// ----------------------------------------------------------------------------

LockUtility::LockUtility ()

// +---------------------------------------------------------------------------
// | The function represents a class constructor.
// +---------------------------------------------------------------------------

{

}  //  LockUtility ()

LockUtility::~LockUtility()

// +---------------------------------------------------------------------------
// | The function represents the class destructor.
// +---------------------------------------------------------------------------

{

}  //  ~LockUtility ()

bool LockUtility::ExecuteDbiCommand (FdoRdbmsConnection  *fdoConnection,
                                     char           *dbi_cmd,
                                     int            *dbi_code)

// +---------------------------------------------------------------------------
// | The function executes the given DBI command.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    int     qid                 = -1;

    bool    execution_status    = TRUE;

    // Execute the given DBI command.

    /*if (!(*dbi_code = dbi_connection->dbi_gql(dbi_cmd, &qid))) return FALSE;
    if (!(*dbi_code = dbi_connection->dbi_execute(qid, NULL, NULL)))
                                                    execution_status = FALSE;*/

    //dbi_connection->dbi_free(qid, NULL);
    return execution_status;

}  //  ExecuteDbiCommand ()

bool LockUtility::GetNumberOfEntries (FdoRdbmsConnection *fdoConnection,
                                      char          *cmd_buffer,
                                      long          *number_of_entries)

// +---------------------------------------------------------------------------
// | The function executes the given command and returns the number of rows
// | selected.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    int     qid                 = -1;

    bool    endloop             = FALSE,
            execution_status    = TRUE;

    // Execute the command. In case of an error terminate the routine.

    //if (!dbi_connection->dbi_gql(cmd_buffer, &qid)) return FALSE;
    //if (!dbi_connection->dbi_execute(qid, NULL, NULL)) {

    //    dbi_connection->dbi_free(qid, NULL);
    //    qid = -1;
    //    return FALSE;

    //}  //  !dbi_connection->dbi_execute( ...

    //// Get the count.

    //while (!endloop) {

    //  if (dbi_connection->dbi_fetch(qid, &dbi_ret_code)) {

    //      endloop = (dbi_ret_code == RDBI_END_OF_FETCH);
    //      if (!endloop) *number_of_entries += 1;

    //  }  //  if (dbi_connection->dbi_fetch( ...
    //  else {

    //    endloop          = TRUE;
    //    execution_status = FALSE;

    //  }  //  else ...

    //}  //  while ( ...

    //// Do some cleanup work.

    //if (qid != -1) dbi_connection->dbi_free(qid, NULL);
    //qid = -1;

    return execution_status;

}  //  GetNumberOfEntries ()

bool LockUtility::HasEntries (FdoRdbmsConnection *fdoConnection,
                              char          *cmd_buffer,
                              bool          *has_entries)

// +---------------------------------------------------------------------------
// | The function executes the given command and returns TRUE if the query
// | produces at least one row, FALSE otherwise.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.
/* TODO
    int     qid                 = -1,
            dbi_ret_code;
*/
    bool    execution_status    = TRUE;
/* TODO
    // Execute the command. In case of an error terminate the routine.

    if (!dbi_connection->dbi_gql(cmd_buffer, &qid)) return FALSE;
    if (!dbi_connection->dbi_execute(qid, NULL, NULL)) {

        dbi_connection->dbi_free(qid, NULL);
        qid = -1;
        return FALSE;

    }  //  !dbi_connection->dbi_execute( ...

    // Get the information.

    if (dbi_connection->dbi_fetch(qid, &dbi_ret_code)) {

        *has_entries = (!(dbi_ret_code == DBI_END_OF_FETCH));

    }  //  if (dbi_connection->dbi_fetch( ...
    else {

      execution_status = FALSE;

    }  //  else ...

    // Do some cleanup work.

    if (qid != -1) dbi_connection->dbi_free(qid, NULL);
    qid = -1;
*/

    *has_entries = false;

    return execution_status;

}  //  HasEntries ()

bool LockUtility::IsLockSupported (FdoRdbmsConnection *fdo_rdbms_connection,
                                   FdoIdentifier      *class_name)

// +---------------------------------------------------------------------------
// | The function returns TRUE back to the calling routine if the given connec-
// | tion and class supports locking, FALSE otherwise.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    const FdoSmLpClassDefinition               *class_definition;
    bool                       is_lock_supported        = FALSE;

    // Check if the connection supports locking. If the connection supports
    // locking check if the class does as well (if provided).

    FdoPtr<FdoIConnectionCapabilities>	connection_capabilities = fdo_rdbms_connection->GetConnectionCapabilities();
    if (connection_capabilities->SupportsLocking()) {

        if (class_name == NULL)
            is_lock_supported = TRUE;
        else {
			if ((class_definition =
                    GetClassDefinition(fdo_rdbms_connection, class_name     )) == NULL)
				return is_lock_supported;

			is_lock_supported = class_definition->GetCapabilities()->SupportsLocking();
        }  
    }  //  if (connection_capabilities-> ...

    // Return the result back to the calling routine.

    return is_lock_supported;

}  //  IsLockSupported ()

char *LockUtility::ConvertString (FdoIdentifier *the_identifier)

// +---------------------------------------------------------------------------
// | The function converts the given string to a UTF8 string and returns it
// | back to the calling routine.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    int  size;

    char *converted_string  = NULL;

    // Get the size of the identifier. Then allocate memory for the converted
    // string and execute the conversion. If an error is detected terminate
    // the routine.

    if ((the_identifier == NULL) ||
        ((size = (int) wcslen(the_identifier->GetText())) == 0))
          throw FdoRdbmsException::Create(GetExceptionMessage(
                                                        INVALID_INPUT_ERROR));

    if ((converted_string = new char[size*3]) == NULL)
         throw FdoRdbmsException::Create(GetExceptionMessage(
                                                MEMORY_ALLOCATION_INT_ERROR));

    if(!ut_utf8_from_unicode(the_identifier->GetText(),
                             converted_string,
                             size*3)) {

       delete[] converted_string;
       converted_string = NULL;

    }  //  if (!ut_utf8_from_unicode( ...

    return converted_string;

}  //  ConvertString ()

char *LockUtility::ConvertString (FdoRdbmsUtil *util, wchar_t *the_string)

// +---------------------------------------------------------------------------
// | The function converts the given string from a wide char to a UTF8 string
// | and returns it back to the calling routine.
// +---------------------------------------------------------------------------

{

#ifdef _WIN32

    // Declare and initialize all necessary local variables.

    int  size;

    char *converted_string  = NULL;

    // Execute the conversion and return the generated string back to the
    // calling routine.

    if ((size = WideCharToMultiByte(CP_UTF8,
                                    0,
                                    the_string,
                                    (int)wcslen (the_string),
                                    NULL,
                                    0,
                                    NULL,
                                    NULL)) != 0) {

        converted_string = new char[size + 1];
        memset(converted_string, 0, size + 1);
        WideCharToMultiByte(CP_UTF8,
                            0,
                            the_string,
                            (int)wcslen (the_string),
                            converted_string,
                            size,
                            NULL,
                            NULL);

        converted_string[size] = 0;

    }  //  if ((size = ...

    return converted_string;
#else
    const char *string = util->UnicodeToUtf8(the_string);
    char *allocated_string = new char [strlen(string)+1];
    return allocated_string;
#endif

}  //  ConvertString ()

char *LockUtility::ConvertString (FdoRdbmsUtil *util, const wchar_t *the_string)

// +---------------------------------------------------------------------------
// | The function converts the given string from a wide char to a UTF8 string
// | and returns it back to the calling routine.
// +---------------------------------------------------------------------------

{
#ifdef _WIN32

    // Declare and initialize all necessary local variables.

    int  size;

    char *converted_string  = NULL;

    // Execute the conversion and return the generated string back to the
    // calling routine.

    if ((size = WideCharToMultiByte(CP_UTF8,
                                    0,
                                    the_string,
                                    (int)wcslen (the_string),
                                    NULL,
                                    0,
                                    NULL,
                                    NULL)) != 0) {

        converted_string = new char[size + 1];
        memset(converted_string, 0, size + 1);
        WideCharToMultiByte(CP_UTF8,
                            0,
                            the_string,
                            (int)wcslen (the_string),
                            converted_string,
                            size,
                            NULL,
                            NULL);

        converted_string[size] = 0;

    }  //  if ((size = ...

    return converted_string;
#else
    const char *string = util->UnicodeToUtf8(the_string);
    char *allocated_string = new char [strlen(string)+1];
    strcpy(allocated_string, string);
    return allocated_string;
#endif

}  //  ConvertString ()

wchar_t *LockUtility::ConvertString (char *the_string)

// +---------------------------------------------------------------------------
// | The function converst the given string from a wide char to a UTF8 string
// | and returns it back to the calling routine.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    wchar_t *converted_string   = NULL;

    // Convert the given string to a wide-char one.

    if ((the_string         == NULL) ||
        (strlen(the_string) == 0   )    )
        return converted_string;

         //throw FdoRdbmsException::Create(GetExceptionMessage(
         //                                               INVALID_INPUT_ERROR));

    if ((converted_string = new wchar_t[strlen(the_string)+1]) == NULL)
         throw FdoRdbmsException::Create(GetExceptionMessage(
                                                MEMORY_ALLOCATION_INT_ERROR));

    ut_utf8_to_unicode(the_string,  converted_string, GDBI_MAXIMUM_TEXT_SIZE);
    return converted_string;

}  //  ConvertString ()

bool LockUtility::GetClassId (FdoRdbmsConnection *dbi_connection,
                              FdoIdentifier *class_name,
                              long          *class_id)

// +---------------------------------------------------------------------------
// | The function returns the class id associated with the given class name.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.
#pragma message ("ToDo: LockUtility::GetClassId")
throw "TODO";
#if 0
    int          qid                               = -1,
                 dbi_ret_code,
                 number_of_columns;

    char         *cmd_buffer,
                 *the_class_name,
                 select_seq[]                      = "select ClassId from ",
                 where_seq[]                       = " where ClassName = '",
                 end_quote_seq[]                   = "'",
                 table_name_seq[]                  = "F_ClassDefinition",
                 int_buffer[10],
                 value_buffer[DBI_ORACLE_VARCHAR];

    size_t       cmd_buffer_size;

    dbi_slct_def current_buffer;

    try {

      // It is assumed that the current schema does not contain a class identified
      // by the given class name. Based on this assumption the function's return
      // parameter is set to -1.

      *class_id = -1;

      // Construct the command to be executed.

      the_class_name  = ConvertString(class_name);
      cmd_buffer_size = strlen(select_seq)     +
                        strlen(table_name_seq) +
                        strlen(where_seq)      +
                        strlen(the_class_name) +
                        strlen(end_quote_seq);

      // Note: The function _alloca will either succeed or throw an exception.
      //       Therefore, checking for a NULL pointer being returned is not
      //       required.

      cmd_buffer = (char *)alloca(cmd_buffer_size+1);

      strcpy(cmd_buffer, select_seq);
      strcat(cmd_buffer, table_name_seq);
      strcat(cmd_buffer, where_seq);
      strcat(cmd_buffer, the_class_name);
      strcat(cmd_buffer, end_quote_seq);

      delete[] the_class_name;
      the_class_name = NULL;

      // Execute the command. In case of an error terminate the routine.

      if (!dbi_connection->dbi_gql(cmd_buffer, &qid)) return FALSE;
      if (!dbi_connection->dbi_execute(qid, NULL, NULL)) {

          dbi_connection->dbi_free(qid, NULL);
          qid = -1;
          return FALSE;

      }  //  if(!dbi_connection->dbi_execute( ...

      // Get the class id.

      if (!dbi_connection->dbi_fetch(qid, &dbi_ret_code)) {

          dbi_connection->dbi_free(qid, NULL);
          qid = -1;
          return FALSE;

      }  //  if (!dbi_connection->dbi_fetch( ...

      if (dbi_ret_code == DBI_END_OF_FETCH) {

          dbi_connection->dbi_free(qid, NULL);
          qid = -1;
          return FALSE;

      }  //  if (dbi_ret_code == ...

      if (!dbi_connection->dbi_desc_nslct(qid, &number_of_columns)) {

          dbi_connection->dbi_free(qid, NULL);
          qid = -1;
          return FALSE;

      }  //  if (!dbi_connection->dbi_desc_nslct( ...

      if (number_of_columns != 1) {

          dbi_connection->dbi_free(qid, NULL);
          qid = -1;
          return FALSE;

      }  //  if (number_of_columns != ...

      sprintf(int_buffer, "%d", 1);
      if (!dbi_connection->dbi_desc_slct(qid,
                                         int_buffer,
                                         &current_buffer,
                                         &dbi_ret_code)) {

          dbi_connection->dbi_free(qid, NULL);
          qid = -1;
          return FALSE;

      }  //  if (!dbi_connection->dbi_desc_slct( ...

      if (!dbi_connection->dbi_get_val_a(qid,
                                         int_buffer,
                                         current_buffer.size + 1,
                                         value_buffer,
                                         NULL,
                                         &dbi_ret_code)) {

          dbi_connection->dbi_free(qid, NULL);
          qid = -1;
          return FALSE;

      }  //  if (!dbi_connection->dbi_get_val_a( ...

      *class_id = atol(value_buffer);

      dbi_connection->dbi_free(qid, NULL);
      qid = -1;

      return TRUE;

    }  //  try ...

    catch ( ... ) {

      throw;

    }  //  catch ( ... ) ...
#endif
}  //  GetClassId ()

char *LockUtility::GetClassFeatIdColumn (FdoRdbmsConnection *fdoConnection,
                                         FdoIdentifier *class_name)

// +---------------------------------------------------------------------------
// | The function returns the feature class identifier for the given class. If
// | the class does not have a feature id property, a NULL pointer is returned
// | back to the calling routine.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    const FdoSmLpClassDefinition *class_definition;

    // Get hold of the class definition.

    if ((class_definition =
                    LockUtility::GetClassDefinition(fdoConnection,
                                                    class_name     )) == NULL)
         return NULL;

    // The class is found and the feature id column can be returned.

    const FdoSmLpDataPropertyDefinition* featIdProp = class_definition->RefFeatIdProperty();

    if ( featIdProp == NULL ) 
        return NULL;

    return (LockUtility::ConvertString(fdoConnection->GetDbiConnection()->GetUtility(),
                      featIdProp->GetColumnName()));

}  //  GetClassFeatIdColumn ()

char *LockUtility::GetClassIdentityString (FdoRdbmsConnection *dbi_connection,
                                           FdoIdentifier *class_name)

// +---------------------------------------------------------------------------
// | The function returns a comma separated string containing the class identi-
// | fiers for the class identified by the given class name. If the class can
// | not be located the routine returns NULL.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    const FdoSmLpClassDefinition                  *class_definition;
    const FdoSmLpPropertyDefinition               *class_identity;
    const FdoSmLpDataPropertyDefinition           *class_identity_property;
    const FdoSmLpDataPropertyDefinitionCollection *class_identities;

    int    i,
           class_identity_count;

    char   *column_name,
           *identity_string;

    size_t identity_buffer_size   = 0;

    try {

      // Find the class definition for the given class name.

      if ((class_definition = GetClassDefinition(dbi_connection,
                                                 class_name)) == NULL)
                                                                   return NULL;

      // Get a handle on the class identity collection.

      class_identities = class_definition->RefIdentityProperties();

      // If there are no identities then return a NULL pointer.

      if ((class_identity_count = class_identities->GetCount()) == 0)
                                                                   return NULL;

      // Query the collection of class identities and calculate the amount of
      // memory to be allocated.

      for (i = 0; i < class_identity_count; i++) {

           class_identity          = class_identities->RefItem(i);
           class_identity_property =
                static_cast<const FdoSmLpDataPropertyDefinition*>(
                                                               class_identity);
           column_name =
                ConvertString(dbi_connection->GetDbiConnection()->GetUtility(),
                              class_identity_property->RefColumn()->GetName());
           identity_buffer_size += strlen(column_name);
           if (i < (class_identity_count - 1))
               identity_buffer_size += strlen(", ");
           delete[] column_name;

      }  //  for (i = 0; ...

      // Allocate memory to store the identity string.
      // Note: The function _alloca will either succeed or throw an exception.
      //       Therefore, checking for a NULL pointer being returned is not
      //       required.

      identity_string = (char *)alloca(identity_buffer_size + 1);

      for (i = 0; i < class_identity_count; i++) {

           class_identity          = class_identities->RefItem(i);
           class_identity_property =
            static_cast<const FdoSmLpDataPropertyDefinition*>(class_identity);
           column_name =
                ConvertString(dbi_connection->GetDbiConnection()->GetUtility(), 
							  class_identity_property->RefColumn()->GetName());
           if (i == 0)
               strcpy(identity_string, column_name);
           else
               strcat(identity_string, column_name);
           if (i < (class_identity_count - 1)) strcat(identity_string, ", ");
           delete[] column_name;

      }  //  for (i = 0; ...

      return SetValue(identity_string);

    }  //  try ...

    catch ( ... ) {

      throw;

    }  //  catch ...

}  //  GetClassIdentityString ()

bool LockUtility::IsPropertyClassName (FdoIdentifier *class_name)

// +---------------------------------------------------------------------------
// | The function checks whether or not the given class name identifies a class
// | name with a dot separated property list. If this is the case, TRUE is re-
// | turned back to the calling routine, FALSE otherwise.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    const FdoString **scopes;

    int             length;

    // Perform the check.

    scopes = class_name->GetScope(length);
    return ((length != 0));

}  //  IsPropertyClassName ()

FdoIdentifier *LockUtility::GetClassName (FdoIdentifier *class_name,
                                          bool          *class_name_modified)

// +---------------------------------------------------------------------------
// | The function takes the given class name and modfies it if required. The
// | routine either returns the modified or original class name back to the
// | caller.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    const FdoString **scopes;

    int             length;

    // The class name can be just a class name or a class name with a dot-
    // separated property list. In the latter case the class name has to
    // be extraxted.

    scopes = class_name->GetScope(length);

	if (class_name_modified != NULL)
		*class_name_modified = (length != 0);
    return ((length != 0) ? FdoIdentifier::Create(scopes[0])
                          : class_name);

}  //  GetClassName ()

wchar_t *LockUtility::GetClassName (FdoRdbmsConnection *dbi_connection,
                                 FdoInt64         class_id,
                                 FdoString          *table_name)

// +---------------------------------------------------------------------------
// | The function returns the name of the class associated with the given class
// | id or table name.
// +---------------------------------------------------------------------------

{
	wchar_t *className = NULL;
	const FdoSmLpClassDefinition *pClass = NULL;

	//const FdoSmLpSchemasP schemas = dbi_connection->GetSchemaManager()->GetLogicalPhysicalSchemas();
	FdoSchemaManagerP      pschManager = dbi_connection->GetSchemaManager();
	const FdoSmLpSchemaCollection* schemas = pschManager->RefLogicalPhysicalSchemas();
	pClass = schemas->FindClass(class_id);
	if (pClass != NULL)
	{
		className = new wchar_t[GDBI_SCHEMA_ELEMENT_NAME_SIZE];
		wcsncpy(className, pClass->GetQName(), GDBI_SCHEMA_ELEMENT_NAME_SIZE);
	}
	return className;
}

 
char *LockUtility::GetClassTable (FdoRdbmsConnection *dbi_connection,
                                  FdoIdentifier *class_name)

// +---------------------------------------------------------------------------
// | The function returns the table associated with the class identified by the
// | given class name or a NULL pointer if the class has no table associated
// | with it.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    const FdoSmLpDbObject         *table;
    const FdoSmLpClassDefinition  *class_definition;

    // Find the class definition for the given class name.

    if ((class_definition =
                    GetClassDefinition(dbi_connection,
                                       class_name     )) == NULL) return NULL;

    // If the class does not have a table associated with it return a NULL
    // pointer otherwise the table's name.

    if ((table = class_definition->RefDbObject()) == NULL) return NULL;
    return (ConvertString(dbi_connection->GetDbiConnection()->GetUtility(), table->GetName()));

}  //  GetClassTable ()

bool LockUtility::GetClassType (FdoRdbmsConnection   *dbi_connection,
                                FdoIdentifier   *class_name,
                                ClassTypes      *class_type)

// +---------------------------------------------------------------------------
// | The function returns the class type for the class identified by the given
// | class name.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    const FdoSmLpClassDefinition  *class_definition;

    // It is assumed that the class type for the class identified by the given
    // class name cannot be determined. Based on this assumption initialize the
    // function's return parameter.

    *class_type = UNSUPPORTED_CLASS_TYPE;

    // Find the class definition for the given class name.

    if ((class_definition =
                   GetClassDefinition(dbi_connection,
                                      class_name     )) == NULL) return FALSE;

    // Get the class type and set the function's return parameter accordingly.

    *class_type = (class_definition->GetClassType() == FdoClassType_Class)
              ? CLASS
              : (class_definition->GetClassType() == FdoClassType_FeatureClass)
                  ? FEATURECLASS
                  : UNSUPPORTED_CLASS_TYPE;

    return TRUE;

}  //  GetClassType ()

bool LockUtility::GetClassType (FdoRdbmsConnection *dbi_connection,
                                FdoIdentifier *class_name,
                                FdoClassType  *class_type)

// +---------------------------------------------------------------------------
// | The function returns the class type for the class identified by the given
// | class name.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    const FdoSmLpClassDefinition  *class_definition;

    // Find the class definition for the given class name.

    if ((class_definition =
                   GetClassDefinition(dbi_connection,
                                      class_name     )) == NULL) return FALSE;

    // Get the class type and set the function's return parameter accordingly.

    *class_type = class_definition->GetClassType();
    return TRUE;

}  //  GetClassType ()

char *LockUtility::GetIdentityPropertyName (FdoRdbmsConnection *dbi_connection,
                                            char          *class_name,
                                            char          *column_name)

// +---------------------------------------------------------------------------
// | The function returns the property name for the given identity column name
// | for the identified class.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    const FdoSmLpClassDefinition                  *class_definition;
    const FdoSmLpDataPropertyDefinition           *data_property_def;
    const FdoSmLpPropertyDefinitionCollection     *property_def_col;
    const FdoSmLpDataPropertyDefinitionCollection *data_property_def_col;

    char             *property_name          = NULL;

    wchar_t          *temp_class_name        = NULL,
                     *temp_column_name       = NULL;

    FdoIdentifier    *temp_class_identifier  = NULL,
                     *temp_column_identifier = NULL;

    FdoStringP temp_column_string;

    try {

      // For further processing the given class and column names have to be
      // converted into a FdoIdentifier.

      temp_class_name        = ConvertString(class_name);
      temp_class_identifier  = FdoIdentifier::Create(temp_class_name);
      delete[] temp_class_name;
      temp_class_name = NULL;

      temp_column_name       = ConvertString(column_name);
      temp_column_identifier = FdoIdentifier::Create(temp_column_name);
      delete[] temp_column_name;
      temp_column_name = NULL;

      // Find the class definition for the given class name.

      if ((class_definition =
                     GetClassDefinition(dbi_connection,
                                        temp_class_identifier)) == NULL) {

           if (temp_class_identifier != NULL) {

               temp_class_identifier->Release();
               temp_class_identifier = NULL;

           }  //  if (temp_class_identifier != ...

           if (temp_column_identifier != NULL) {

               temp_column_identifier->Release();
               temp_column_identifier = NULL;

           }  //  if (temp_column_identifier != ...

           return NULL;

      }  //  if ((class_definition = ...

      // Get the collection of identity properties and properties (the second
      // collection is required because of the way schema manager handles a
      // request to find a property name).

      property_def_col      = class_definition->RefProperties();
      data_property_def_col = class_definition->RefIdentityProperties();

      // Use the information to identify the property name for the given column
      // name.

      temp_column_string = column_name;

      if ((data_property_def =
            data_property_def_col->ColName2Property(
                                                  property_def_col,
                                                  temp_column_string)) == NULL)
            property_name = NULL;
       else

         property_name = (data_property_def->GetIdPosition() > 0)
                       ? ConvertString(dbi_connection->GetDbiConnection()->GetUtility(), 
					     data_property_def->GetName()): NULL;

       // Clean up.

       temp_class_identifier->Release();
       temp_column_identifier->Release();

       return property_name;

    }  //  try ...

    catch ( ... ) {

      if (temp_class_name != NULL) {

          delete[] temp_class_name;
          temp_class_name = NULL;

      }  //  if (temp_class_name != ...

      if (temp_column_name != NULL) {

          delete[] temp_column_name;
          temp_column_name = NULL;

      }  //  if (temp_column_name != ...

      if (temp_class_identifier != NULL) {

          temp_class_identifier->Release();
          temp_class_identifier = NULL;

      }  //  if (temp_class_identifier != ...

      if (temp_column_identifier != NULL) {

          temp_column_identifier->Release();
          temp_column_identifier = NULL;

      }  //  if (temp_column_identifier != ...

      throw;

    }  //  catch ...

}  //  GetIdentityPropertyName ()

char *LockUtility::GetPropertyName (FdoRdbmsConnection *dbi_connection,
                                    char          *class_name,
                                    char          *column_name)

// +---------------------------------------------------------------------------
// | The function returns the property name for the given column name for the
// | identified class.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    const FdoSmLpClassDefinition                  *class_definition;
    const FdoSmLpDataPropertyDefinition           *data_property_def;
    const FdoSmLpPropertyDefinitionCollection     *property_def_col;
    const FdoSmLpDataPropertyDefinitionCollection *data_property_def_col;

    char             *property_name          = NULL;

    wchar_t          *temp_class_name        = NULL,
                     *temp_column_name       = NULL;

    FdoIdentifier    *temp_class_identifier  = NULL,
                     *temp_column_identifier = NULL;

    FdoStringP temp_column_string;

    try {

      // For further processing the given class and column names have to be
      // converted into a FdoIdentifier.

      temp_class_name        = ConvertString(class_name);
      temp_class_identifier  = FdoIdentifier::Create(temp_class_name);
      delete[] temp_class_name;
      temp_class_name = NULL;

      temp_column_name       = ConvertString(column_name);
      temp_column_identifier = FdoIdentifier::Create(temp_column_name);
      delete[] temp_column_name;
      temp_column_name = NULL;

      // Find the class definition for the given class name.

      if ((class_definition =
                     GetClassDefinition(dbi_connection,
                                        temp_class_identifier)) == NULL) {

           if (temp_class_identifier != NULL) {

               temp_class_identifier->Release();
               temp_class_identifier = NULL;

           }  //  if (temp_class_identifier != ...

           if (temp_column_identifier != NULL) {

               temp_column_identifier->Release();
               temp_column_identifier = NULL;

           }  //  if (temp_column_identifier != ...

           return NULL;

      }  //  if ((class_definition = ...

      // Get the collection of identity properties.

      data_property_def_col = class_definition->RefIdentityProperties();
      property_def_col = class_definition->RefProperties();

      // Use the information to identify the property name for the given column
      // name.

      temp_column_string = column_name;

      if ((data_property_def =
            data_property_def_col->ColName2Property(
                                                property_def_col,
                                                temp_column_string)) == NULL)
            property_name = NULL;
       else
         property_name = ConvertString(dbi_connection->GetDbiConnection()->GetUtility(),
                                       data_property_def->GetColumnName());

       // Clean up.

       temp_class_identifier->Release();
       temp_column_identifier->Release();

       return property_name;

    }  //  try ...

    catch ( ... ) {

      if (temp_class_name != NULL) {

          delete[] temp_class_name;
          temp_class_name = NULL;

      }  //  if (temp_class_name != ...

      if (temp_column_name != NULL) {

          delete[] temp_column_name;
          temp_column_name = NULL;

      }  //  if (temp_column_name != ...

      if (temp_class_identifier != NULL) {

          temp_class_identifier->Release();
          temp_class_identifier = NULL;

      }  //  if (temp_class_identifier != ...

      if (temp_column_identifier != NULL) {

          temp_column_identifier->Release();
          temp_column_identifier = NULL;

      }  //  if (temp_column_identifier != ...

      throw;

    }  //  catch ...

}  //  GetPropertyName ()

bool LockUtility::IsSystemTable(FdoRdbmsConnection *dbi_connection,
                                char          *table_name)

// +---------------------------------------------------------------------------
// | The function returns TRUE if the given table name represents a system
// | table, FALSE otherwise.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    int is_system_table = false;

    // Call the corresponding DBI function to determine whether or not the
    // given table name represents a system table and return the result back
    // to the calling routine.

    //dbi_connection->dbi_is_sys_tbl(table_name, &is_system_table);
    return ((is_system_table == 0) ? FALSE : TRUE);

}  //  IsSystemTable ()

bool LockUtility::IsValidName (FdoString *name)

// +---------------------------------------------------------------------------
// | The function verifies whether or not the given name consists of alpha-
// | numeric values only. If this is the case the routine returns "TRUE" and
// | "FALSE" otherwise.
// +---------------------------------------------------------------------------

{

    // Declare all necessary local variables.

    wchar_t *curr_char,
            us_char     = L'_';

    // Parse the given name character by character. If at any time a non alpha-
    // numeric character is dected return "FALSE", "TRUE" otherwise.

    for (curr_char = (wchar_t *)name;
         ((*curr_char           != '\0')       &&
          ((iswalnum(*curr_char) != 0   ) ||
           (*curr_char == us_char)           )    );
         *curr_char++);

    return (*curr_char == '\0');

}  //  IsValidName ()

bool LockUtility::ActivateLock (FdoRdbmsConnection   *dbi_connection,
                                const wchar_t        *lock_name,
                                wchar_t              *lock_user)

// +---------------------------------------------------------------------------
// | The function activates the lock associated with the given lock owner.
// +---------------------------------------------------------------------------

{

    /*return (((dbi_connection->dbi_lock_act(lock_name, lock_user)) == 0)
                     ? FALSE
                     : TRUE);*/
	return true;

}  //  ActivateLock ()

bool LockUtility::GetLockId (FdoRdbmsConnection  *dbi_connection,
                             char           *lock_name,
                             long           *lock_id)

// +---------------------------------------------------------------------------
// | The function retrieves a lock id from the database for the given lock name
// | and returns it back to the calling routine.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.
#pragma message ("ToDo: LockUtility::GetLockId")
throw "TODO";
#if 0
    int          qid                               = -1,
                 dbi_ret_code,
                 number_of_columns;

    char         *cmd_buffer,
                 *lock_name_copy                   = NULL,
                 select_seq[]                      = "select lockid from ",
                 where_seq[]                       = " where name = '",
                 end_quote_seq[]                   = "'",
                 table_name_seq[]                  = "F_LockName",
                 int_buffer[10],
                 value_buffer[DBI_ORACLE_VARCHAR];

    size_t       cmd_buffer_size;

    dbi_slct_def current_buffer;

    try {

      // The lock name is stored in uppercase characters in the table F_LockName.
      // Therefore the given lock name must be converted into an uppercase string.

      lock_name_copy = SetValue(lock_name);
      ut_to_upper(lock_name_copy);

      // Construct the command to be executed.

      cmd_buffer_size = strlen(select_seq)     +
                        strlen(table_name_seq) +
                        strlen(where_seq)      +
                        strlen(lock_name_copy) +
                        strlen(end_quote_seq);

      // Note: The function _alloca will either succeed or throw an exception.
      //       Therefore, checking for a NULL pointer being returned is not
      //       required.

      cmd_buffer = (char *)alloca(cmd_buffer_size+1);

      strcpy(cmd_buffer, select_seq);
      strcat(cmd_buffer, table_name_seq);
      strcat(cmd_buffer, where_seq);
      strcat(cmd_buffer, lock_name_copy);
      strcat(cmd_buffer, end_quote_seq);

      // Free memory no longer required.

      delete[] lock_name_copy;
      lock_name_copy = NULL;

      // Execute the command. In case of an error terminate the routine.

      if (!dbi_connection->dbi_gql(cmd_buffer, &qid)) return FALSE;
      if (!dbi_connection->dbi_execute(qid, NULL, NULL)) {

          dbi_connection->dbi_free(qid, NULL);
          qid = -1;
          return FALSE;

      }  //  if(!dbi_connection->dbi_execute( ...

      // Get the lock id.

      if (!dbi_connection->dbi_fetch(qid, &dbi_ret_code)) {

          dbi_connection->dbi_free(qid, NULL);
          qid = -1;
          return FALSE;

      }  //  if (!dbi_connection->dbi_fetch( ...

      if (dbi_ret_code == DBI_END_OF_FETCH) {

          dbi_connection->dbi_free(qid, NULL);
          qid = -1;
          return FALSE;

      }  //  if (dbi_ret_code == ...

      if (!dbi_connection->dbi_desc_nslct(qid, &number_of_columns)) {

          dbi_connection->dbi_free(qid, NULL);
          qid = -1;
          return FALSE;

      }  //  if (!dbi_connection->dbi_desc_nslct( ...

      if (number_of_columns != 1) {

          dbi_connection->dbi_free(qid, NULL);
          qid = -1;
          return FALSE;

      }  //  if (number_of_columns != ...

      sprintf(int_buffer, "%d", 1);
      if (!dbi_connection->dbi_desc_slct(qid,
                                         int_buffer,
                                         &current_buffer,
                                         &dbi_ret_code)) {

          dbi_connection->dbi_free(qid, NULL);
          qid = -1;
          return FALSE;

      }  //  if (!dbi_connection->dbi_desc_slct( ...

      if (!dbi_connection->dbi_get_val_a(qid,
                                         int_buffer,
                                         current_buffer.size + 1,
                                         value_buffer,
                                         NULL,
                                         &dbi_ret_code)) {

          dbi_connection->dbi_free(qid, NULL);
          qid = -1;
          return FALSE;

      }  //  if (!dbi_connection->dbi_get_val_a( ...

      *lock_id = atol(value_buffer);

      dbi_connection->dbi_free(qid, NULL);
      qid = -1;

      return TRUE;

    }  //  try ...

    catch ( ... ) {

      throw;

    }  //  catch ( ... ) ...
#endif
}  //  GetLockId ()

char *LockUtility::GetLockOwner (FdoRdbmsConnection *dbi_connection,
                                 char          *lock_id)

// +---------------------------------------------------------------------------
// | The function returns the lock owner associated with the given lock id.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    int               found = true;

    dbi_li_id_t       lock_identifier;

    dbi_lknm_spec_def lknm_spec;

    // Convert the given id to the required type.

    lock_identifier = atol(lock_id);

    // Call the DBI function that searches for the lock identified by the given
    // id.

    //if (!dbi_connection->dbi_lock_find_by_id(lock_identifier,
    //                                         &lknm_spec,
    //                                         &found)) return NULL;
    if (!found) return NULL;
    return (SetValue(lknm_spec.owner));

}  //  GetLockOwner ()

bool LockUtility::MaintainLockData (FdoRdbmsConnection *dbi_connection,
                                    char          *lock_name)

// +---------------------------------------------------------------------------
// | The function ensures that the table F_LockName in the current data store
// | contains a lock definition for the given lock name. If the table does not
// | contain such a definition yet, the routine will add one to the table.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    bool lock_exists = FALSE;

    try {

      // Check if the table F_LockName already contains an entry for the given
      // lock name. If this is the case terminate the routine.

      if (!LockExists(dbi_connection, lock_name, &lock_exists))
             throw FdoRdbmsException::Create(GetExceptionMessage(
                                                    LOCK_MAINTENANCE_FAILURE));

      if (lock_exists) return TRUE;

      // A lock definition for the given lock name does not yet exist and needs
      // to be added to the table F_LockName.

      if (!CreateLock(dbi_connection, lock_name))
                throw FdoRdbmsException::Create(GetExceptionMessage(
                                                    LOCK_MAINTENANCE_FAILURE));

      // The lock has been created successfully. Terminate the routine.

      return TRUE;

    }  //  try ...

    catch ( ... ) {

      throw;

    }  //  catch ( ... ) ...

}  //  MaintainLockData ()

bool LockUtility::ProcessLockRequest (
                            FdoRdbmsConnection              *dbi_connection,
                            FdoRdbmsFilterProcessor    *filter_utility,
                            FdoIdentifier              *class_name,
                            FdoIdentifier              *modified_class_name,
							bool						class_name_modified,
                            FdoFilter                  *fdo_filter,
							char					   **table_name,
							char					   **sql_filter)

// +---------------------------------------------------------------------------
// | The function implements the lock related request.
// +---------------------------------------------------------------------------

{
    // Declare and initialize all necessary local variables.

    bool         execution_status             = FALSE,
                 current_user_set             = FALSE,
                 transaction_lock_step        = FALSE,
                 class_name_mod_indicator;

    char         *user_num                    = NULL,
                 *view_name                   = NULL,
                 *current_user                = NULL,
                 *tmp_sql_filter              = NULL,
                 *the_class_name              = NULL,
                 *sql_filter_extension        = NULL;

    size_t       buf_len                      = 0;

    FdoClassType class_type;

    //try {

      if ((*table_name = GetClassTable (dbi_connection,
                                       modified_class_name)) == NULL)
           return execution_status;

      if ((the_class_name =
                    ConvertString(modified_class_name)) == NULL) {

           delete[] *table_name;
           *table_name = NULL;
           return execution_status;

      }  //  if ((the_class_name = ...

      if (!GetClassType(dbi_connection, class_name, &class_type)) {

          delete[] *table_name;
          delete[] the_class_name;

          *table_name     = NULL;
          the_class_name = NULL;
          return execution_status;

      }  //  if (!GetClassType( ...

      class_name_mod_indicator =
                            ((class_name_modified)                    &&
                            (class_type == FdoClassType_FeatureClass)   )
                                ? FALSE
                                : class_name_modified;

      if ((*sql_filter = (char*)GetFilterSQL(dbi_connection,
                                     filter_utility,
                                     modified_class_name,
                                     fdo_filter,
                                     class_name_mod_indicator)) == NULL) {

          delete[] *table_name;
          delete[] the_class_name;

          *table_name     = NULL;
          the_class_name = NULL;
          return execution_status;

      }  //  if ((sql_filter = ...

      delete[] the_class_name;
      the_class_name = NULL;
	  
	  execution_status = true;

	  return execution_status;
      
}  //  ProcessLockRequest ()

LockUtility::ColumnToken LockUtility::GetColumnToken (char *column_name)

// +---------------------------------------------------------------------------
// | The routine returns the token associated with the given column name.
// +---------------------------------------------------------------------------

{

    // Compare the given string to different (expected) system columns. If the
    // string represents one of the system columns return the corresponding
    // token back to the calling routine. Otherwise it is assumed that the
    // name represents an identity column.

    /*if (stricmp(column_name, ADB_LTID_COLUMN           ) == 0)
        return LT_ID_COLUMN;
    if (stricmp(column_name, ADB_ALIAS_LT_NAME_COLUMN  ) == 0)
        return LT_NAME_COLUMN;
    if (stricmp(column_name, ADB_ROW_LOCK_COLUMN       ) == 0)
        return ROW_LOCK_COLUMN;
    if (stricmp(column_name, ADB_CLASS_ID_COLUMN       ) == 0)
        return CLASS_ID_COLUMN;
    if (stricmp(column_name, ADB_CLASS_NAME_COLUMN     ) == 0)
        return CLASS_NAME_COLUMN;
    if ((stricmp(column_name, ADB_FLN_OWNER_COLUMN     ) == 0) ||
        (stricmp(column_name, ADB_OWM_WM_USERNAME      ) == 0) ||
        (stricmp(column_name, ADB_ALIAS_OWNER_COLUMN   ) == 0)    )
        return LOCK_OWNER_COLUMN;
    if ((stricmp(column_name, ADB_ROWLOCKTYPE_COLUMN   ) == 0) ||
        (stricmp(column_name, ADB_OWM_WM_LOCKMODE      ) == 0) ||
        (stricmp(column_name, ADB_ALIAS_LOCKTYPE_COLUMN) == 0)    )
        return ROW_LOCK_TYPE_COLUMN;
    if ((stricmp(column_name, ADB_OWM_WM_INCURWORKSPACE) == 0) ||
        (stricmp(column_name, ADB_ALIAS_INCURRLT_COLUMN) == 0)    )
        return LT_IN_CURR_COLUMN;*/

    return IDENTITY_COLUMN;
}  //  GetColumnToken ()

char *LockUtility::GetFilterExtension (FdoRdbmsConnection *dbi_connection,
                                       FdoRdbmsFilterProcessor *filter_utility,
                                       FdoIdentifier *class_name,
                                       FdoClassType  class_type)

// +---------------------------------------------------------------------------
// | The function retrieves a filter extension depending on the given class
// | type.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    char   *filter_extension     = NULL,
           *tmp_filter_extension = NULL;

    size_t filter_extension_size = 0;

    // The extension is either the feature id column or the class identity
    // properties depending on the given class type.

    tmp_filter_extension = (class_type == FdoClassType_Class)
                     ? LockUtility::GetClassIdentityString(dbi_connection,
                                                           class_name)
                     : LockUtility::GetClassFeatIdColumn (dbi_connection,
                                                          class_name);

    if (tmp_filter_extension == NULL) return NULL;

    // The filter extension has to be structured according to the following
    // description:
    //
    // class type is FdoClassType_Class:
    //   (<filter_extension>)
    //
    // class type is FdoClassType_FeatureClass:
    //   <filter_extension>

    filter_extension_size = ((class_type == FdoClassType_Class)
                                ? strlen("(")
                                : 0                            ) +
                             strlen(tmp_filter_extension)        +
                            ((class_type == FdoClassType_Class)
                                ? strlen (")")
                                : 0                            );
    filter_extension = new char[filter_extension_size + 1];
    if (class_type == FdoClassType_Class)
        strcpy(filter_extension, "(");
    if (class_type == FdoClassType_Class)
        strcat(filter_extension, tmp_filter_extension);
    else
        strcpy(filter_extension, tmp_filter_extension);
    if (class_type == FdoClassType_Class)
        strcat(filter_extension, ")");

    delete[] tmp_filter_extension;
    tmp_filter_extension = NULL;

    return filter_extension;

}  //  GetFilterExtension ()

const char *LockUtility::GetFilterSQL (FdoRdbmsConnection              *dbi_connection,
                                 FdoRdbmsFilterProcessor    *filter_utility,
                                 FdoIdentifier *class_name,
                                 FdoFilter     *fdo_filter,
                                 bool          class_name_modified)

// +---------------------------------------------------------------------------
// | The function takes a FDO filter object and returns its SQL equivalent back
// | to the calling routine.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    wchar_t                            *filter_sql       = NULL;

    FdoPtr<FdoIdentifierCollection> sql_filter_props;

    FdoRdbmsFilterUtilConstrainDef  filter_constrain;

    try {


      // The following sets up an identifier list containing the feature id
      // property, if the class is a feature class. For feature classes, the
      // sqlFilter is embedded in an IN clause that must select the feature id.
      // For non-feature classes, selecting the identity properties is fine.
		
      sql_filter_props = 
		  FdoRdbmsFilterUtil::GetIdentPropList(dbi_connection->GetDbiConnection()->GetSchemaUtil()->GetClass(class_name->GetText()));
      filter_constrain.selectedProperties = sql_filter_props;
      filter_sql = SetValue(filter_utility->FilterToSql(
                                                    fdo_filter,
                                                    class_name->GetText(),
													SqlCommandType_Select,
                                                    FdoCommandType_Select,
                                                    &filter_constrain, false));
      return( dbi_connection->GetDbiConnection()->GetUtility()->UnicodeToUtf8( filter_sql ) );

    }  //  try ...

    catch ( ... ) {

      throw;

    }  //  catch ...

}  //  GetFilterSQL ()

char *LockUtility::HandleFilter (FdoRdbmsConnection              *dbi_connection,
                                 FdoRdbmsFilterProcessor  *filterProcessor,
                                 FdoIdentifier *class_name,
                                 FdoIdentifier *modified_class_name,
                                 FdoFilter     *fdo_filter,
                                 bool          class_name_modified)

// +---------------------------------------------------------------------------
// | The function converts the given filter into the corresponding SQL state-
// | ment.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    bool         class_name_mod_indicator     = FALSE;

    char         *sql_filter                  = NULL,
                 *tmp_sql_filter              = NULL,
                 *sql_filter_extension        = NULL;

    size_t       buf_len                      = 0;

    FdoClassType class_type;

    // Get the class type for the given class.

    if (!GetClassType(dbi_connection, modified_class_name, &class_type))
        return sql_filter;

    // Set the variable indicating whether or not the class name was modified.

    class_name_mod_indicator = ((class_name_modified)                    &&
                                (class_type == FdoClassType_FeatureClass)   )
                             ? FALSE
                             : class_name_modified;

    // Get the SQL representation for the given FDO filter.

    sql_filter = (char*)GetFilterSQL(dbi_connection,
                                   filterProcessor,
                                   modified_class_name,
                                   fdo_filter,
                                   class_name_mod_indicator);
    return sql_filter;

}  //  HandleFilter ()

char *LockUtility::SetValue (char *value_buffer)

// +---------------------------------------------------------------------------
// | The function allocates memory for a string and initializes it with the
// | given value.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    char *value = NULL;

    // Allocate memory for the given buffer. If this fails terminate the
    // routine.

    if ((value = new char[strlen(value_buffer) + 1]) == NULL) return value;
    strcpy(value, value_buffer);
    return value;

}  //  SetValue ()

char *LockUtility::SetValue (const char *value_buffer)

// +---------------------------------------------------------------------------
// | The function allocates memory for a string and initializes it with the
// | given value.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    char *value = NULL;

    // Allocate memory for the given buffer. If this fails terminate the
    // routine.

    if ((value = new char[strlen(value_buffer) + 1]) == NULL) return value;
    strcpy(value, value_buffer);
    return value;

}  //  SetValue ()

wchar_t *LockUtility::SetValue (const wchar_t *value_buffer)

// +---------------------------------------------------------------------------
// | The function allocates memory for a string and initializes it with the
// | given value.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    wchar_t *value = NULL;

    // Allocate memory for the given buffer. If this fails terminate the
    // routine.

    if ((value = new wchar_t[wcslen(value_buffer) + 1]) == NULL) return value;
    wcscpy(value, value_buffer);
    return value;

}  //  SetValue ()

const FdoSmLpDataPropertyDefinition
                            *LockUtility::GetIdentityPropertyDefinition (
                                                 FdoRdbmsConnection *dbi_connection,
                                                 FdoString     *class_name,
                                                 FdoString         *column_name)

// +---------------------------------------------------------------------------
// | The function determines the identity property definition for the named
// | column and class name and returns it back to the calling routine. If such
// | a definition does not exist a NULL pointer is returned.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    const FdoSmLpClassDefinition                  *class_definition;
    const FdoSmLpPropertyDefinition               *class_identity;
    const FdoSmLpDataPropertyDefinition           *class_identity_property;
    const FdoSmLpDataPropertyDefinitionCollection *class_identities;

    int       i,
              class_identity_count;

    FdoString *the_column_name       = NULL;

    // Find the class identified by the given class name. If the class is not
    // found return a NULL pointer back to the calling routine.

    if ((class_definition = GetClassDefinition(dbi_connection,
                                               class_name     )) == NULL)
         return NULL;

    // Get hold of the class identity. If there is no class identity return a
    // NULL pointer back to the calling routine.

    class_identities = class_definition->RefIdentityProperties();
    if ((class_identity_count = class_identities->GetCount()) == 0)
                                                                   return NULL;

    // Query the collection of class identities to find the one that is
    // identified by the given column name. If the identity is found re-
    // turn it back to the calling routine.

    for (i = 0; i < class_identity_count; i++) {

         class_identity          = class_identities->RefItem(i);
         class_identity_property =
                    static_cast<const FdoSmLpDataPropertyDefinition*>(
                                                               class_identity);
         if (dbi_connection->GetDbiConnection()->GetUtility()->StrCmp(
                    column_name,
                    class_identity_property->RefColumn()->GetName()) == 0) {

             return class_identity_property;

         }  //  if (wcscmp(the_column_name, ...

    }  //  for (i = 0; ...

    // No matching identity has been found. Return a NULL pointer back to the
    // calling routine.

    return NULL;

}  //  GetIdentityPropertyDefinition ()

FdoDataValue *LockUtility::ProcessIdentityValue (
                  const FdoSmLpDataPropertyDefinition *property_definition,
                  FdoString                                   *column_value)

// +---------------------------------------------------------------------------
// | The function processes the given identity value.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.


    FdoDataValue *data_value = NULL;

    // Convert the given value into the type that corresponds to the identity
    // property type.

    switch (property_definition->GetDataType()) {

      case FdoDataType_Int16:
        data_value = FdoDataValue::Create((FdoInt16) FdoCommonOSUtil::wtoi(column_value));
        break;

      case FdoDataType_Int32:
        data_value = FdoDataValue::Create((FdoInt32) FdoCommonOSUtil::wtoi(column_value));
        break;

      case FdoDataType_Int64:
        data_value = FdoDataValue::Create((FdoInt64) FdoCommonStringUtil::StringToInt64(column_value));
        break;

      case FdoDataType_String:
      default:
		  if (FdoCommonOSUtil::wcsicmp(column_value, L"") == 0)
            data_value = FdoDataValue::Create(L"");
        else 
          data_value = FdoDataValue::Create(column_value);

        break;

    }  //  switch ...

    return data_value;

}  //  ProcessIdentityValue ()

FdoPropertyValue *LockUtility::CreateIdentity (FdoRdbmsConnection *dbi_connection,
                                               FdoString          *column_name,
                                               FdoString          *column_value,
                                               FdoString          *class_name)

// +---------------------------------------------------------------------------
// | The function creates a property value object based on the given data and
// | returns the object back the calling routine.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    const FdoSmLpDataPropertyDefinition *class_identity_property;

    FdoString                              *col_name                  = NULL,
                                           *col_value                 = NULL;

    FdoDataValue                           *data_value                = NULL;

    FdoPropertyValue                       *property_value_obj        = NULL;

    try {

      // If no class name has been given the data is still processed by using a
      // default handling. Otherwise the handling depends on the class's iden-
      // tity definition.

      if (class_name == NULL) {

          // Create the new property object and set its properties.

          if ((property_value_obj = FdoPropertyValue::Create()) == NULL)
               return property_value_obj;

          property_value_obj->SetName(column_name);
          property_value_obj->SetValue(column_value);

          // Free used memory and return the newly created identity object.

          return property_value_obj;

      }  //  if (class_name == NULL) ...

      // A class name has been specified. Get hold of the property definition
      // identified by the given column and class name.

      if ((class_identity_property =
                          GetIdentityPropertyDefinition(dbi_connection,
                                                        class_name,
                                                        column_name)) == NULL)
           return NULL;

      // Process the given column value.

      if ((data_value = ProcessIdentityValue(class_identity_property,
                                             column_value            )) == NULL)
            return NULL;

      // Create the new property object. If this fails terminate the routine.

      if ((property_value_obj = FdoPropertyValue::Create()) == NULL)
           return property_value_obj;

      // Set the object's properties and return a handle to it back to the
      // calling routine.

      property_value_obj->SetName(class_identity_property->GetName());
      property_value_obj->SetValue(data_value);
      data_value->Release();
      data_value = NULL;

      return property_value_obj;

    }  //  try

    catch ( ... ) {

      if (data_value != NULL) {

          data_value->Release();
          data_value = NULL;

      }  //  if (data_value != ...

      if (property_value_obj != NULL) {

          property_value_obj->Release();
          property_value_obj = NULL;

      }  //  if (property_value_obj != ...

      throw;

    }  //  catch ...

}  //  CreateIdentity ()

FdoPropertyValueCollection *LockUtility::CreateIdentityCollection ()

// +---------------------------------------------------------------------------
// | The function creates a property value collection object and returns a
// | handle to it back to the calling routine.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoPropertyValueCollection  *property_value_collection  = NULL;

    try {

      // Create the new property collection.

      property_value_collection = FdoPropertyValueCollection::Create();
      return property_value_collection;

    }  //  try ...

    catch ( ... ) {

      throw;

    }  //  catch ...

}  //  CreateIdentityCollection ()

void LockUtility::SetActiveLock (FdoRdbmsConnection    *dbi_connection,
                                 char             *lock_name)

// +---------------------------------------------------------------------------
// | The function activates the given lock. The function is used from the
// | different commands (FdoIDelete, FdoIUpdate) where it is not necessary to
// | separate the maintaining of the lock data and the activation of a lock
// | like it is necessary when locking objects (there it is required because
// | a transaction lock has to be placed on the lock itself).
// +---------------------------------------------------------------------------

{
#if 0
    // Declare and initialize all necessary local variables.

    char lock_user[PROCESS_NAME_LENGTH+1];

    // Ensure that the lock to be set active is present.

    MaintainLockData(dbi_connection, lock_name);

    // Activate the lock.

    if (!ActivateLock(dbi_connection, lock_name, lock_user))
        throw FdoCommandException::Create(
                            LockUtility::GetExceptionMessage(
                                               LockUtility::DBI_LOCK_FAILURE));
#endif
}  //  SetActiveLock ()

FdoILockConflictReader *LockUtility::GetDefaultLockConflictReader (
                                      FdoRdbmsConnection *fdo_rdbms_connection,
                                      FdoIdentifier      *class_name,
                                      bool               *execution_status)

// +---------------------------------------------------------------------------
// | Some FDO commands like FdoIDelete and FdoIUpdate may need a default lock
// | conflict reader. A default lock conflict reader is a conflict reader with
// | no lock conflicts. This routine creates one and returns it back to the
// | calling routine.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    bool                   class_name_modified   = FALSE;

    FdoIdentifier          *modified_class_name  = NULL;

    FdoILockConflictReader *lk_conflict_reader   = NULL;

    try {

      // It is assumed that this operation cannot be completed successfully.
      // Therefore, the function's return parameter indicating the execution
      // status is set to reflect this assumption.

      *execution_status = FALSE;

      // Analyze the class name. The class name can have any of the following
      // structures:
      //
      //  (1) just a class name (like "testClass")
      //  (2) <class_name>.<property>[.<property]
      //  (3) <schema>:<class_name>
      //
      // At the end of the process a usable class name is available.

      modified_class_name = GetClassName(class_name, &class_name_modified);

      // Create the lock conflict reader.

      if ((lk_conflict_reader =
           new FdoRdbmsLockConflictReader(fdo_rdbms_connection,
										  NULL,
                                          modified_class_name)) == NULL)
           throw FdoCommandException::Create(
                      LockUtility::GetExceptionMessage(
                          LockUtility::LOCK_CONFLICT_REPORT_GENERATION_ERROR));

      // Clean up.

      if ((class_name_modified) && (modified_class_name != NULL))
           modified_class_name->Release();
      modified_class_name = NULL;

      // Set the status variable and terminate the routine.

      *execution_status = TRUE;
      return lk_conflict_reader;

    }  //  try ...

    catch ( ... ) {

      // Clean up.

      if ((class_name_modified) && (modified_class_name != NULL))
          modified_class_name->Release();
      modified_class_name = NULL;

      throw;

    }  //  catch ( ... ) ...

}  //  GetDefaultLockConflictReader ()

FdoILockConflictReader *LockUtility::HandleLocks (
                                      FdoRdbmsConnection *fdo_rdbms_connection,
                                      FdoIdentifier      *class_name,
                                      FdoFilter          *fdo_filter,
                                      bool               place_transaction_lock,
                                      bool               *lock_conflict_found,
                                      bool               *execution_status)

// +---------------------------------------------------------------------------
// | Some FDO commands like FdoIDelete and FdoIUpdate only work in an all-or-
// | nothing approach. This means that the command is not executed if the pro-
// | cess cannot get exclusive access to all features involved in the action or
// | if some features are locked with a persistent lock by a different user.
// | This function implements all lock related operations for those commands.
// | It applies a transaction lock on all objects and checks for persistent
// | lock conflicts.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    bool                            filter_modified       = FALSE,
                                    class_name_modified   = FALSE;

    char                            *table_name           = NULL,
                                    *filter_sql           = NULL;

	wchar_t							*table_nameW		  = NULL,
									*filter_sqlW		  = NULL;

    FdoIdentifier                   *tmp_class_name       = NULL,
                                    *modified_class_name  = NULL;

    FdoILockConflictReader          *lk_conflict_reader   = NULL;
	

    FdoPtr<FdoRdbmsFilterProcessor>filterProcessor = fdo_rdbms_connection->GetFilterProcessor();
    const FdoSmLpClassDefinition *class_definition     = NULL;
	int lexecution_status;

    try {

		// Check if the connection supports locking. If the connection supports
		// locking check if the class does as well (if provided).
		*lock_conflict_found = false;
		FdoPtr<FdoIConnectionCapabilities> connection_capabilities = fdo_rdbms_connection->GetConnectionCapabilities();
		FdoRdbmsLockManagerP lockManager = FdoPtr<FdoRdbmsLockManager>(fdo_rdbms_connection->GetLockManager());
		if (lockManager) 
		{
			// It is assumed that this operation cannot be completed successfully.
			// Therefore, the function's return parameter indicating the execution
			// status is set to reflect this assumption.

			*execution_status = FALSE;

			// Analyze the class name. The class name can have any of the following
			// structures:
			//
			//  (1) just a class name (like "testClass")
			//  (2) <class_name>.<property>[.<property]
			//  (3) <schema>:<class_name>
			//
			// At the end of the process a usable class name is available.

			modified_class_name = GetClassName(class_name, &class_name_modified);

			// Modify the filter if the class name has been changed and a filter is
			// provided.

			if ((class_name_modified) && (fdo_filter != NULL)) {

			  tmp_class_name =
				  FdoRdbmsFilterUtil::ConvertFilterToMainClass(class_name,
															 fdo_filter);
			  tmp_class_name->Release();
			  filter_modified = TRUE;

			}  //  if ((class_name_modified) ...

			// Get the class definition. If it cannot be retrieved clean up and
			// terminate the routine.

			if ((class_definition =
					GetClassDefinition(fdo_rdbms_connection,
									   modified_class_name)) == NULL) {

			   if (filter_modified) {

				   FdoRdbmsFilterUtil::ConvertFilterToObjectClass(class_name,
															  fdo_filter);
				   filter_modified = FALSE;

			   }  //  if (filter_modified) ...

			   if ((class_name_modified) && (modified_class_name != NULL))
					modified_class_name->Release();

			   modified_class_name = NULL;
			   return lk_conflict_reader;

			}  //  if ((class_definition = ...

			// Get the class table.

			if ((table_name =
				  LockUtility::ConvertString(fdo_rdbms_connection->GetDbiConnection()->GetUtility(),
									  class_definition->GetDbObjectName())) == NULL) {

			   if (filter_modified) {

				   FdoRdbmsFilterUtil::ConvertFilterToObjectClass(class_name,
															  fdo_filter);
				   filter_modified = FALSE;

			   }  //  if (filter_modified) ...

			   if ((class_name_modified) && (modified_class_name != NULL))
					modified_class_name->Release();
			   modified_class_name = NULL;
			   return lk_conflict_reader;

			}  //  if ((table_name = ...

			// If a filter is given, get the corresponding SQL statement.

			if (fdo_filter != NULL)
			  filter_sqlW = ConvertString(HandleFilter(fdo_rdbms_connection,
										filterProcessor,
										class_name,
										modified_class_name,
										fdo_filter,
										class_name_modified));

			// If requested, apply a transaction lock on the qualified objects. If
			// this fails terminate the routine.

			table_nameW = ConvertString(table_name);

			if (place_transaction_lock)
			{
				lexecution_status = 
				  FdoPtr<FdoRdbmsLockManager>(fdo_rdbms_connection->GetLockManager())->AcquireLock(table_nameW, 
							  filter_sqlW,(FdoString *)NULL, FdoLockType_Transaction);
			}
			FdoRdbmsLockConflictQueryHandler *lockConflictQuery = NULL;
			// check if the class supports locking
			if (LockUtility::IsLockSupported(fdo_rdbms_connection, modified_class_name))
			{
				lockConflictQuery = 
				FdoPtr<FdoRdbmsLockManager>(fdo_rdbms_connection->GetLockManager())->LockConflictQuery(table_nameW,
								filter_sqlW );
			}
			
			if (lockConflictQuery)	
			{
				lk_conflict_reader = new FdoRdbmsLockConflictReader(fdo_rdbms_connection, 
																lockConflictQuery, class_name);
				*lock_conflict_found = (lockConflictQuery->IsTableEmpty() == false);
			}
			else
			{
				lk_conflict_reader = GetDefaultLockConflictReader (fdo_rdbms_connection,
																   class_name,
																   execution_status);
			}
			if (filter_modified) {

			  FdoRdbmsFilterUtil::ConvertFilterToObjectClass(class_name, fdo_filter);
			  filter_modified = FALSE;

			}  //  if (filter_modified) ...

			// Clean up.

			if ((class_name_modified) && (modified_class_name != NULL))
			  modified_class_name->Release();
			modified_class_name = NULL;

			delete[] filter_sql;
			delete[] table_name;
			delete[] filter_sqlW;
			delete[] table_nameW;

			filter_sql = NULL;
			table_name = NULL;
			filter_sqlW = NULL;
			table_nameW = NULL;

			*execution_status = TRUE;
	  }
	  else
	  {
		  lk_conflict_reader = GetDefaultLockConflictReader (fdo_rdbms_connection,
														  class_name,
														  execution_status);
	  }
	  return lk_conflict_reader;

    }  //  try ...

    catch ( ... ) {

      // Clean up.

      if (filter_modified) {

          FdoRdbmsFilterUtil::ConvertFilterToObjectClass(class_name, fdo_filter);
          filter_modified = FALSE;

      }  //  if (filter_modified) ...

      if ((class_name_modified) && (modified_class_name != NULL))
          modified_class_name->Release();
      modified_class_name = NULL;

      if (filter_sql != NULL) { delete[] filter_sql; filter_sql = NULL; }
      if (table_name != NULL) { delete[] table_name; table_name = NULL; }
	  if (filter_sqlW != NULL) { delete[] filter_sqlW; filter_sqlW = NULL; }
      if (table_nameW != NULL) { delete[] table_nameW; table_nameW = NULL; }

      throw;

    }  //  catch ( ... ) ...

}  //  HandleLocks ()


wchar_t *LockUtility::GetExceptionMessage (ExceptionIds exception_id)

// +---------------------------------------------------------------------------
// | The function returns the message associated with the given exception id.
// +---------------------------------------------------------------------------

{

    switch (exception_id) {

      case ADMINISTRATOR_ROLE_REQUIRED:
        return NlsMsgGet(FDORDBMS_68, "Operation requires administrator role");
        break;

      case CLASS_NAME_RETRIEVAL_FAILURE:
        return NlsMsgGet(FDORDBMS_69, "Failed to retrieve class name");
        break;

      case CLEAR_LOCK_REPORT_FAILURE:
        return NlsMsgGet(FDORDBMS_70, "Failed to clear lock report");
        break;

      case CLOSING_LOCK_CONFLICT_READER_FAILURE:
        return NlsMsgGet(FDORDBMS_71, "Failed to close lock conflict reader");
        break;

      case CLOSING_LOCK_INFO_READER_FAILURE:
        return NlsMsgGet(FDORDBMS_72, "Failed to close lock object reader");
        break;

      case CLOSING_LOCK_OWNERS_READER_FAILURE:
        return NlsMsgGet(FDORDBMS_73, "Failed to close lock owners reader");
        break;

      case CREATE_IDENTITY_FAILURE:
        return NlsMsgGet(FDORDBMS_74, "Failed to create an identity object");
        break;

      case DBI_LOCK_FAILURE:
        return NlsMsgGet(FDORDBMS_75, "DBI lock error");
        break;

      case DBI_UNLOCK_FAILURE:
        return NlsMsgGet(FDORDBMS_77, "DBI unlock error");
        break;

      case ERROR_LOADING_CONFLICT_INFORMATION:
        return NlsMsgGet(
                    FDORDBMS_78,
                    "Error during initial load of lock conflict information");
        break;

      case ERROR_LOADING_LOCK_INFO_DATA:
        return NlsMsgGet(FDORDBMS_79,
                         "Error during initial load of lock info data");
        break;

      case ERROR_LOADING_LT_INFO_DATA:
        return NlsMsgGet(
                    FDORDBMS_356,
                    "Error during initial load of long transaction info data");
        break;

      case ERROR_LOADING_OWNER_INFORMATION:
        return NlsMsgGet(FDORDBMS_80,
                         "Error during initial load of lock owner information");
        break;

      case FAILED_TO_CREATE_LKOR:
        return NlsMsgGet(FDORDBMS_379,
                         "Failed to create a FdoILockedObjectsReader object");
        break;

      case GET_LOCK_INFO_CMD_FAILURE:
        return NlsMsgGet(
                 FDORDBMS_76,
                 "Failed to construct command to retrieve lock information in DBI");
        break;

      case GET_LOCK_INFO_EXECUTION_FAILURE:
        return NlsMsgGet(FDORDBMS_81, "Failed to retrieve lock info data");
        break;

      case IDENTITY_RETRIEVAL_FAILURE:
        return NlsMsgGet(FDORDBMS_82, "Failed to retrieve identity");
        break;

      case INVALID_CLASS_NAME:
        return NlsMsgGet(FDORDBMS_83, "Invalid class name");
        break;

      case INVALID_DBI_CONNECTION:
        return NlsMsgGet(FDORDBMS_84, "Invalid DBI connection");
        break;

      case INVALID_LOCK_OWNER:
        return NlsMsgGet(FDORDBMS_380, "Invalid lock owner specification");
        break;

      case LOCK_CONFLICT_GENERATION_ERROR:
        return NlsMsgGet(FDORDBMS_112,
                         "Failed to create a lock conflict object");
        break;

      case LOCK_CONFLICT_INITIALIZATION_ERROR:
        return NlsMsgGet(FDORDBMS_113,
                         "Failed to initialize a lock conflict object");
        break;

      case LOCK_CONFLICT_REPORT_GENERATION_ERROR:
        return NlsMsgGet(FDORDBMS_85, "Failed to create lock conflict report");
        break;

      case LOCK_EXECUTION_FAILURE:
        return NlsMsgGet(FDORDBMS_86, "Lock execution error");
        break;

      case LOCK_INFO_READER_GENERATION_ERROR:
        return NlsMsgGet(FDORDBMS_87, "Failed to create a lock object reader");
        break;

      case LOCK_LOCK_CONFL_ERROR:
        return NlsMsgGet(FDORDBMS_399,
                         "Failed to lock object(s) due to a lock conflict");
        break;

      case LOCK_LT_RETRIEVAL_FAILURE:
        return NlsMsgGet(FDORDBMS_394,
                         "Failed to retrieve long transaction information");
        break;

      case LOCK_OWNER_READER_GENERATION_ERROR:
        return NlsMsgGet(FDORDBMS_88, "Failed to create a lock owner reader");
        break;

      case LOCK_OWNER_RETRIEVAL_FAILURE:
        return NlsMsgGet(FDORDBMS_89, "Failed to retrieve lock owner");
        break;

      case LOCK_TYPE_RETRIEVAL_FAILURE:
        return NlsMsgGet(FDORDBMS_90, "Failed to retrieve lock type");
        break;

      case LOCK_VERSION_CONFL_ERROR:
        // See xxx
        return NlsMsgGet(FDORDBMS_112,
                         "Failed to lock object(s) due to a version conflict");
        break;

      case LOCKED_OBJECTS_READ_ERROR:
        return NlsMsgGet(
                FDORDBMS_381,
                "Failed execute read request on FdoIGetLockedObjectsReader");
        break;

      case LT_CONFLICT_ENUM_GENERATION_ERROR:
        return NlsMsgGet(
                      FDORDBMS_375,
                      "Failed to create long transaction conflict enumerator");
        break;

      case LT_CONFLICT_ENUM_NOT_SET:
        return NlsMsgGet(
                FDORDBMS_376,
                "Invalid conflict solution flags in long transaction conflict enumerator");
        break;

      case LT_DESCRIPTION_LENGTH_ERROR:
        return NlsMsgGet(FDORDBMS_357,
                         "Long transaction description too long (max: 255)");
        break;

      case LT_NAME_LENGTH_ERROR:
        return NlsMsgGet(FDORDBMS_358,
                         "Long transaction name too long (max: 30)");
        break;

      case LT_NAME_NULL_ERROR:
        return NlsMsgGet(FDORDBMS_359, "Long transaction name cannot be null");
        break;

      case MEMORY_ALLOCATION_ERROR:
        return NlsMsgGet(FDORDBMS_91, "Failed to allocate memory");
        break;

      case NO_CONFLICT_VIEW_NAME:
        return NlsMsgGet(FDORDBMS_331, "No conflict view name available");
        break;

      case PROCESS_SET_LOCK_INFO_REQUEST_CMD_ERROR:
        return NlsMsgGet(FDORDBMS_92, "Failed to set the lock request command");
        break;

      case READ_NEXT_CONFLICT_FAILURE:
        return NlsMsgGet(FDORDBMS_93, "Failed to read next lock conflict data set");
        break;

      case READ_NEXT_LOCK_INFO_FAILURE:
        return NlsMsgGet(FDORDBMS_94, "Failed to read next lock info data set");
        break;

      case READ_NEXT_LT_INFO_FAILURE:
        return NlsMsgGet(FDORDBMS_360,
                         "Failed to read next long transaction info data set");
        break;

      case READ_NEXT_OWNER_FAILURE:
        return NlsMsgGet(FDORDBMS_95, "Failed to read next owner data set");
        break;

      case TRANSACTION_LOCK_FAILURE:
        return NlsMsgGet(FDORDBMS_332,
                         "Failed to get exclusive access to specified objects");
        break;

      case UNLOCK_EXECUTION_FAILURE:
        return NlsMsgGet(FDORDBMS_96, "Unlock execution error");
        break;

      case WORKSPACE_MANAGER_GET_LOCK_INFO_CMD_FAILURE:
        return NlsMsgGet(
                FDORDBMS_97,
                "Failed to construct command to retrieve lock information in Oracle Workspace Manager");
        break;

      case WORKSPACE_MANAGER_LOCK_FAILURE:
        return NlsMsgGet(FDORDBMS_98, "Oracle Workspace Manager lock error");
        break;

      case WORKSPACE_MANAGER_UNLOCK_FAILURE:
        return NlsMsgGet(FDORDBMS_99, "Oracle Workspace Manager unlock error");
        break;

	  case LOCK_ACTIVE_TRANSACTION_ERROR:
		return NlsMsgGet(FDORDBMS_458, "Transaction must be started before requesting transaction lock");
		break;

	  case LOCK_REQUEST_PROCESSING_ERROR:
		  return NlsMsgGet(FDORDBMS_457, "Lock request processing error");
		  break;

      default:
        return NlsMsgGet(FDORDBMS_100, "Unknown error code");
        break;

    }  //  switch ...

}  //  GetExceptionMessage ()

wchar_t *LockUtility::GetExceptionMessage (ExceptionIds  exception_id,
                                           const wchar_t *parameter_1)

// +---------------------------------------------------------------------------
// | The function returns the message associated with the given exception id
// | using the given parameters.
// +---------------------------------------------------------------------------

{

    switch (exception_id) {

      case ACTIVE_LT_ERROR:
        return NlsMsgGet1(FDORDBMS_366,
                          "%1$ls: Cannot process active long transaction",
                          parameter_1);
        break;

      case INVALID_CONTEXT:
        return NlsMsgGet1(FDORDBMS_353,
                          "Invalid context for command '%1$ls'",
                          parameter_1);
        break;

      case LT_NAME_NOT_UNIQUE:
        return NlsMsgGet1(
                      FDORDBMS_362,
                      "Long transaction name '%1$ls' not unique; cannot create",
                      parameter_1);
        break;

      case LT_CONFLICT_LOAD_ERROR:
        return NlsMsgGet1(
                      FDORDBMS_402,
                      "%1$ls: Failed to load version conflicts",
                      parameter_1);
        break;

      case READER_POSITIONING_ERROR:
        return NlsMsgGet1(FDORDBMS_383,
                          "Reader '%1$ls' is not positioned correctly",
                          parameter_1);
        break;

      case ROOT_LT_ERROR:
        return NlsMsgGet1(FDORDBMS_367,
                          "%1$ls: Cannot process root long transaction",
                          parameter_1);
        break;

      case UNSUPPORTED_ORACLE_VERSION:
        return NlsMsgGet1(
                FDORDBMS_354,
                "Unsupported Oracle version (<9i) detected for command '%1$ls'",
                parameter_1);
        break;

      default:
        return NlsMsgGet(FDORDBMS_100, "Unknown error code");
        break;

    }  //  switch ...

}  //  GetExceptionMessage ()

wchar_t *LockUtility::GetExceptionMessage (ExceptionIds  exception_id,
                                           const wchar_t *parameter_1,
                                           const wchar_t *parameter_2)

// +---------------------------------------------------------------------------
// | The function returns the message associated with the given exception id
// | using the given parameters.
// +---------------------------------------------------------------------------

{

    switch (exception_id) {

      case INVALID_LT_NAME:
        return NlsMsgGet2(
                FDORDBMS_363,
                "Invalid long transaction name ('%1$ls') for command '%2$ls'",
                parameter_1,
                parameter_2);
        break;

      case LOCK_SUPPORT_ERROR:
        return NlsMsgGet2(
                FDORDBMS_384,
                "%1$ls: Locking is not supported for either the connection or class (%2$ls)",
                parameter_2,
                parameter_1);
        break;
	
	  case LOCK_SUPPORT_DATASTORE_ERROR:
		return NlsMsgGet(
				FDORDBMS_452,
				"Locking is not supported for this datastore");
		break;

      case LOCK_TYPE_INPUT_ERROR:
        return NlsMsgGet2(FDORDBMS_239,
                          "Invalid lock type '%1$ls' for command '%2$ls'",
                          parameter_1,
                          parameter_2);
        break;

      case LT_ACTIVE_SESSION_ERROR:
        return NlsMsgGet2(
                FDORDBMS_368,
                "%1$ls: Long transaction '%2$ls' or descendent active in a session",
                parameter_1,
                parameter_2);
        break;

      case LT_DOES_NOT_EXIST:
        return NlsMsgGet2(FDORDBMS_361,
                          "%1$ls: Long transaction '%2$ls' does not exist",
                          parameter_1,
                          parameter_2);
        break;

      case LT_LEAF_ERROR:
        return NlsMsgGet2(
                FDORDBMS_377,
                "%1$ls: Long transaction '%2$ls' is not a leaf long transaction",
                parameter_1,
                parameter_2);
        break;

      case LT_CONFLICT_ENV_ERROR:
        return NlsMsgGet2(FDORDBMS_403,
                          "%1$ls: Version conflicts caused by Oracle data from the following Oracle users outside current data store: %2$ls",
                          parameter_1,
                          parameter_2);
        break;

      case UNSUPPORTED_LOCK_TYPE:
        return NlsMsgGet2(FDORDBMS_355,
                          "Unsupported lock type '%1$ls' for command '%2$ls'",
                          parameter_1,
                          parameter_2);
        break;

	  

      default:
        return NlsMsgGet(FDORDBMS_100, "Unknown error code");
        break;

    }  //  switch ...

}  //  GetExceptionMessage ()



// ----------------------------------------------------------------------------
// --                          Supporting functions                          --
// ----------------------------------------------------------------------------

bool LockUtility::CreateLock (FdoRdbmsConnection *dbi_connection,
                              char          *lock_name)

// +---------------------------------------------------------------------------
// | The function creates a new lock map entry for the given lock name.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.
/*
    int  lock_exists                         = 0;

    char generated_lock_name[DBI_LKNM_SIZE];

    // Create the lock and return the execution status back to the calling
    // routine. It is not an error if the lock already exists.

    if (!dbi_connection->dbi_lock_create(lock_name,
                                         0,
                                         NULL,
                                         generated_lock_name,
                                         &lock_exists)) return FALSE;
*/
    return TRUE;

}  //  CreateLock ()

const FdoSmLpClassDefinition *LockUtility::GetClassDefinition (
                                                FdoRdbmsConnection *dbi_connection,
                                                FdoString     *class_name)

// +---------------------------------------------------------------------------
// | The function returns the class definition for the class identified by the
// | given class name. If the class cannot be located the routine returns a
// | NULL pointer.
// +---------------------------------------------------------------------------

{

    return dbi_connection->GetDbiConnection()->GetSchemaUtil()->GetClass(class_name);

}  //  GetClassDefinition ()

const FdoSmLpClassDefinition *LockUtility::GetClassDefinition (
												FdoRdbmsConnection *dbi_connection,
                                                FdoIdentifier *class_name)

// +---------------------------------------------------------------------------
// | The function returns the class definition for the class identified by the
// | given class name. If the class cannot be located the routine returns a
// | NULL pointer.
// +---------------------------------------------------------------------------

{

    return dbi_connection->GetDbiConnection()->GetSchemaUtil()->GetClass(class_name->GetText());

}  //  GetClassDefinition ()

bool LockUtility::LockExists (FdoRdbmsConnection *dbi_connection,
                              char          *lock_name,
                              bool          *lock_exists)

// +---------------------------------------------------------------------------
// | The function checks whether or not the given user has an entry in the
// | table F_LockName.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    char   *cmd_buffer,
           *lock_name_copy,
           select_seq[]     = "select Name from ",
           where_seq[]      = " where Name = '",
           end_quote_seq[]  = "'",
           table_name_seq[] = "F_LockName";

    size_t cmd_buffer_size;

    try {

      // The lock name is stored in uppercase characters in the table F_LockName.
      // Therefore the given lock name must be converted into an uppercase string.

      lock_name_copy = LockUtility::SetValue(lock_name);
      ut_to_upper(lock_name_copy);

      // Construct the command to be executed.

      cmd_buffer_size = strlen(select_seq)     +
                        strlen(table_name_seq) +
                        strlen(where_seq)      +
                        strlen(lock_name_copy) +
                        strlen(end_quote_seq);

      // Note: The function _alloca will either succeed or throw an exception.
      //       Therefore, checking for a NULL pointer being returned is not
      //       required.

      cmd_buffer = (char *)alloca(cmd_buffer_size+1);

      strcpy(cmd_buffer, select_seq);
      strcat(cmd_buffer, table_name_seq);
      strcat(cmd_buffer, where_seq);
      strcat(cmd_buffer, lock_name_copy);
      strcat(cmd_buffer, end_quote_seq);

      // Release allocated  memory.

      delete[] lock_name_copy;

      // Execute the command and return the result back to the calling routine.

      return(HasEntries(dbi_connection, cmd_buffer, lock_exists));

    }  //  try ...

    catch ( ... ) {

      throw;

    }  //  catch ( ... ) ...

}  //  LockExists ()

bool LockUtility::ExecuteDbiLockCommands (FdoRdbmsConnection  *dbi_connection,
                                          char           *dbi_cmd,
                                          int            *dbi_code)

// +---------------------------------------------------------------------------
// | The function executes the given DBI command.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    int     qid                 = -1;

    bool    endloop             = FALSE,
            execution_status    = TRUE;

    // Execute the given DBI command.

    /*if (!(*dbi_code = dbi_connection->dbi_gql(dbi_cmd, &qid))) return FALSE;
    if (execution_status)
        if (!(*dbi_code = dbi_connection->dbi_execute(qid, NULL, NULL)))
                                                    execution_status = FALSE;*/

    // Fetch the result. This will lock/unlock the objects.

    //while ((execution_status) && (!endloop)) {

    //  if (!dbi_connection->dbi_fetch(qid, dbi_code)) execution_status = FALSE;
    //  if (*dbi_code == RDBI_END_OF_FETCH) endloop = TRUE;

    //}  //  while ...

    //dbi_connection->dbi_free(qid, NULL);
    return execution_status;

}  //  ExecuteDbiLockCommands ()

wchar_t *LockUtility::GetExceptionMessage (InternalExceptionIds exception_id)

// +---------------------------------------------------------------------------
// | The function returns the message associated with the given exception id.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    switch (exception_id) {

      case DBI_EXECUTION:
        return NlsMsgGet(FDORDBMS_101,"Failed to execute a DBI command");
        break;

      case CONVERSION_ERROR:
        return NlsMsgGet(FDORDBMS_102, "Conversion error");
        break;

      case INVALID_INPUT_ERROR:
        return NlsMsgGet(FDORDBMS_103, "Invalid parameter");
        break;

      case PROCESS_CLASS_REQUEST:
        return NlsMsgGet(FDORDBMS_104, "Process class request error");
        break;

      case LOCK_MAINTENANCE_FAILURE:
        return NlsMsgGet(FDORDBMS_105, "Failed to maintain lock data");
        break;

      case MEMORY_ALLOCATION_INT_ERROR:
        return NlsMsgGet(FDORDBMS_91, "Failed to allocate memory");
        break;

      case PROCESS_FEATURE_CLASS_REQUEST:
        return NlsMsgGet(FDORDBMS_106, "Process feature class request error");
        break;

      default:
        return NlsMsgGet(FDORDBMS_100, "Unknown error code");
        break;

    }  //  switch ...

}  //  GetExceptionMessage ()
