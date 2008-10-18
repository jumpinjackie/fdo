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
// | The file contains the implementation of the class FdoRdbmsGetLockInfo. The
// | class retrieves lock information for objects identified by the user. The
// | retrieved information is made available via the ILockInfoReader interface.
// =============================================================================

// The links to the header file(s) to be included.

#include "stdafx.h"
#include "FdoRdbmsSchemaUtil.h"
#include <malloc.h>

#include "FdoRdbmsGetLockInfo.h"
#include "LockUtility.h"
#include "FdoRdbmsException.h"
#include "FdoRdbmsFilterUtil.h"
#include "FdoRdbmsFilterProcessor.h"

#include "FdoCommonOSUtil.h"


FdoRdbmsGetLockInfo::FdoRdbmsGetLockInfo ()

// +---------------------------------------------------------------------------
// | The function represents a class constructor.
// +---------------------------------------------------------------------------

{

    // Initialize the class variables.

    //dbi_connection = NULL;
    SetToZero();

}  //  FdoRdbmsGetLockInfo ()

FdoRdbmsGetLockInfo::FdoRdbmsGetLockInfo (FdoIConnection *connection)
    : FdoRdbmsFeatureCommand<FdoIGetLockInfo>(connection)

// +---------------------------------------------------------------------------
// | The function represents a class constructor.
// +---------------------------------------------------------------------------

{

    // Configure the RDBMS and DBI connection.

    fdo_rdbms_connection = static_cast<FdoRdbmsConnection*>(connection);

    // Initialize the class variables.

    SetToZero();

}  //  FdoRdbmsGetLockInfo ()

FdoRdbmsGetLockInfo::~FdoRdbmsGetLockInfo ()

// +---------------------------------------------------------------------------
// | The function represents the class destructor.
// +---------------------------------------------------------------------------

{

    // Reset the connection pointers.

    fdo_rdbms_connection = NULL;

    // Release used memory.

    FreeMemory();

}  //  ~FdoRdbmsGetLockInfo ()

FdoILockedObjectReader *FdoRdbmsGetLockInfo::Execute ()

// +---------------------------------------------------------------------------
// | The function executes a request to retrieve the lock related data for the
// | specified objets.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoCommandException    *fdo_cmd_ex       = NULL;

    FdoRdbmsLockInfoReader *lock_info_reader = NULL;

    try {

      // Ensure that the connection is established. Otherwise raise an
      // exception.

      if (fdo_rdbms_connection->GetDbiConnection() == NULL)
          throw FdoCommandException::Create(
                    LockUtility::GetExceptionMessage(
                                         LockUtility::INVALID_DBI_CONNECTION));

      // Check if the class name is set. If this is not the case terminate the
      // routine.

      if (this->GetClassNameRef() == NULL)
          throw FdoCommandException::Create(
                        LockUtility::GetExceptionMessage(
                                             LockUtility::INVALID_CLASS_NAME));

      // The user requested lock information on identified objects. The request
      // itself is only executed when the reader this command provides back to
      // the user is accessed. The purpose of this command is to get the neces-
      // sary SQL statement, create the reader and provide the SQL statement to
      // that reader.

	  // establish filter and class to be processed
      if (GetSQLLockInfoCmd() == false)
		  throw FdoCommandException::Create(
                        LockUtility::GetExceptionMessage(
                                      LockUtility::GET_LOCK_INFO_CMD_FAILURE));

      // Create the return object and pass it back to the caller.

      if ((lock_info_reader =
                    new FdoRdbmsLockInfoReader(
                        fdo_rdbms_connection,
						req_class_name,
                        sqlFilter,
                        tableName                          )) == NULL)
           throw FdoCommandException::Create(
                    LockUtility::GetExceptionMessage(
                              LockUtility::LOCK_INFO_READER_GENERATION_ERROR));

      return lock_info_reader;

    }  //  try ...

    catch (FdoException *ex) {

      FreeMemory();
      fdo_cmd_ex = FdoCommandException::Create(ex->GetExceptionMessage(), ex);
      ex->Release();
      throw fdo_cmd_ex;

    }  //  catch (FdoException *ex)

    catch ( ... ) {

      FreeMemory();
      throw;

    }  //  catch ( ... ) ...

}  //  Execute ()


// ----------------------------------------------------------------------------
// --                          Supporting functions                          --
// ----------------------------------------------------------------------------

bool FdoRdbmsGetLockInfo::GetSQLLockInfoCmd ()

// +---------------------------------------------------------------------------
// | The function controls the creation of the SQL command required to retrieve
// | the requested lock info data from a DBI environment.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    bool               filter_modified        = FALSE,
                       class_name_modified    = FALSE;

    char               *sql_filter            = NULL,
                       *the_class_name        = NULL,
                       *sql_filter_extension  = NULL;

    size_t             buf_len                = 0;

    FdoFilter          *fdo_filter            = NULL;

    FdoClassType       class_type;

    FdoIdentifier      *class_name            = NULL,
                       *temp_class_name       = NULL,
                       *modified_class_name   = NULL;
   

	// Get the current class name. The class name can come in any of the
	// following structures:
	//
	//   just a class name (like "testClass")
	//   <class_name>.<property>[.<property>]
	//   <schema>:<class_name> (like "testDb:testClass")
	//
	// At the end of the process a usable class name is available.

	class_name = GetFeatureClassName();
	modified_class_name = LockUtility::GetClassName(class_name,
												  &class_name_modified);

	// The request is executed only, if the connection and the class support
	// locking. If this is not the case issue an error.

	if (!LockUtility::IsLockSupported(fdo_rdbms_connection,
									modified_class_name))	{
		if ((class_name_modified) && (modified_class_name != NULL))
			modified_class_name->Release();
		modified_class_name = NULL;
		class_name->Release();
		class_name = NULL;
		return false;
	}
			
	// Modify the filter if the class name has been changed.

	if (class_name_modified) {

	  temp_class_name =
		  FdoRdbmsFilterUtil::ConvertFilterToMainClass(class_name,
													 this->GetFilterRef());
	  temp_class_name->Release();
	  filter_modified = TRUE;

	}  //  if (class_name_modified) ...

	// Convert the class name to a data type DBI can handle. If this fails
	// terminate the routine. The calling procedure will handle the error
	// case.

	if ((the_class_name =
		  LockUtility::ConvertString(modified_class_name)) == NULL) {

		if ((class_name_modified         ) &&
		   (filter_modified             ) &&
		   (this->GetFilterRef() != NULL)    )
			FdoRdbmsFilterUtil::ConvertFilterToObjectClass(
													class_name,
													this->GetFilterRef());
		if ((class_name_modified) && (modified_class_name != NULL))
			modified_class_name->Release();
		modified_class_name = NULL;
		class_name->Release();
		class_name = NULL;
		return false;

	}  //  if ((the_class_name = ...

	// Set the class variable identifying the current class name. This value
	// is required later when the lock information reader is set up.

	req_class_name = LockUtility::SetValue(the_class_name);

	// Identify the table for the given class. If the class name is "FEATURE"
	// the table name is "F_Feature".
	// NOTE: The class name being set to "Feature" establishes a special case
	//       that still needs to be handled. In this version, the function is
	//       terminated.

	if ((tableName =
	          (LockUtility::GetClassTable(fdo_rdbms_connection,
	                                      modified_class_name))) == NULL) {

	     if ((class_name_modified         ) &&
	         (filter_modified             ) &&
	         (this->GetFilterRef() != NULL)    )
	          FdoRdbmsFilterUtil::ConvertFilterToObjectClass(
	                                                  class_name,
	                                                  this->GetFilterRef());
	     if ((class_name_modified) && (modified_class_name != NULL))
	          modified_class_name->Release();
	     modified_class_name = NULL;

	     delete[] the_class_name;
	     the_class_name = NULL;
	     class_name->Release();
	     class_name = NULL;
	     return false;

	}  //  if ((tableName = ...

	// Free used memory.

	delete[] the_class_name;
	the_class_name = NULL;

	// If there is a filter defined for the class get its SQL equivalent.

	fdo_filter = GetFilter();
	if (fdo_filter != NULL) {

		if ((sqlFilter =
				(char*)LockUtility::GetFilterSQL(fdo_rdbms_connection,
										  FdoPtr<FdoRdbmsFilterProcessor>(FdoPtr<FdoRdbmsConnection>((FdoRdbmsConnection*)GetConnection())->GetFilterProcessor()),
										  modified_class_name,
										  fdo_filter,
										  class_name_modified)) == NULL) {

			if ((class_name_modified         ) &&
			   (filter_modified             ) &&
			   (this->GetFilterRef() != NULL)    )
				FdoRdbmsFilterUtil::ConvertFilterToObjectClass(
													class_name,
													this->GetFilterRef());
			if ((class_name_modified) && (modified_class_name != NULL))
			   modified_class_name->Release();
			modified_class_name = NULL;

			class_name->Release();
			class_name = NULL;

			fdo_filter->Release();
			fdo_filter = NULL;

			return false;

		}  //  if ((sql_filter =  ...

		// Get the class type for the current class. The class type is im-
		// portant as the returned SQL statement may need to be modified.

		LockUtility::GetClassType(fdo_rdbms_connection,
							modified_class_name,
							&class_type);

		// If the class name has not been modified, the generated select
		// statement has the following structure:
		//
		// For a Feature Class:
		//      select  <table_name>.<featid_column>
		//        from  <table_name>
		//        where <where_clause>
		//
		// For a Class
		//      select  <table_name>.<ids>
		//        from  <table_name>
		//        where <where_clause>
		//
		// In both cases, the statement has to be altered as documented next:
		//
		// For a Feature Class:
		//      <featid_col> in
		//            select  <table_name>.<featid_column>
		//              from  <table_name>
		//              where <where_clause>
		//
		// For a Class
		//      (<ids>) in
		//            select  <table_name>.<ids>
		//              from  <table_name>
		//              where <where_clause>

		if (!class_name_modified) {

		// Get the required filter extension.

			if ((sql_filter_extension =
						 LockUtility::GetFilterExtension(
										  fdo_rdbms_connection,
										  FdoPtr<FdoRdbmsFilterProcessor>(FdoPtr<FdoRdbmsConnection>((FdoRdbmsConnection*)GetConnection())->GetFilterProcessor()),
										  modified_class_name,
										  class_type          )) == NULL) {

			   if ((class_name_modified         ) &&
				   (filter_modified             ) &&
				   (this->GetFilterRef() != NULL)    )
					FdoRdbmsFilterUtil::ConvertFilterToObjectClass(
													class_name,
													this->GetFilterRef());
			   if ((class_name_modified) && (modified_class_name != NULL))
					modified_class_name->Release();
			   modified_class_name = NULL;

			   class_name->Release();
			   class_name = NULL;

			   fdo_filter->Release();
			   fdo_filter = NULL;

			   return false;

			}  //  if ((sql_filter_extension = ...

			sql_filter = sqlFilter;
			buf_len = strlen(sql_filter_extension) +
					strlen (" in (")             +
					strlen(sqlFilter)       +
					strlen (") ");
			sqlFilter = new char[buf_len + 1];
			sprintf(sqlFilter,"%s in (%s)", sql_filter_extension, sql_filter);

			// delete[] sql_filter; <-- causes crash - investigate

		}  //  if (!class_name_modified) ...

	}  // if (fdo_filter != ...

      // Clean up.

	if ((class_name_modified         ) &&
	  (filter_modified             ) &&
	  (this->GetFilterRef() != NULL)    )
	  FdoRdbmsFilterUtil::ConvertFilterToObjectClass(class_name,
												 this->GetFilterRef());
	if ((class_name_modified) && (modified_class_name != NULL))
	   modified_class_name->Release();
	modified_class_name = NULL;

	class_name->Release();
	class_name = NULL;
	
	if (sql_filter_extension != NULL)
		delete[] sql_filter_extension;

	if (fdo_filter != NULL) { fdo_filter->Release(); fdo_filter = NULL; }

	return true;

}  //  GetSQLLockInfoCmd ()

void FdoRdbmsGetLockInfo::FreeMemory ()

// +---------------------------------------------------------------------------
// | The function frees memory.
// +---------------------------------------------------------------------------

{

    if (req_class_name != NULL) delete[] req_class_name;

    SetToZero();

}  //  FreeMemory ()

void FdoRdbmsGetLockInfo::SetToZero ()

// +---------------------------------------------------------------------------
// | The function initializes the member variables.
// +---------------------------------------------------------------------------

{

    req_class_name = NULL;
	sqlFilter = NULL;
	tableName = NULL;

}  //  SetToZero ()
