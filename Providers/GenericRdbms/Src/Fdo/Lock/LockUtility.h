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
// | The file contains the definition of the class LockUtility. The class im-
// | plements functions common to lock related operations.
// =============================================================================

#ifndef LockUtility_h
#define LockUtility_h 1

// The links to the header files to be included.

#include <wctype.h>
#include "FdoRdbms.h"
#include "FdoRdbmsConnection.h"
#include "FdoRdbmsLockConflictReader.h"

#define		PROCESS_NAME_LENGTH			255

#define		DBI_LOCK_CONFLICT            0xffff0000
#define     DBI_IS_LOCKED               (DBI_LOCK_CONFLICT+1)  /* for backward compatibility            */
#define     DBI_LOCK_N_L                (DBI_LOCK_CONFLICT+2)  /* no lock                               */
#define     DBI_LOCK_N_S                (DBI_LOCK_CONFLICT+3)  /* no shared lock                        */
#define     DBI_LOCK_U                  (DBI_LOCK_CONFLICT+4)  /* update exclusive transaction lock     */
#define     DBI_LOCK_E                  (DBI_LOCK_CONFLICT+5)  /* exclusive row lock                    */
#define     DBI_LOCK_V_E                (DBI_LOCK_CONFLICT+6)  /* version exclusive row lock            */
#define     DBI_LOCK_S                  (DBI_LOCK_CONFLICT+7)  /* shared row lock                       */
#define     DBI_LOCK_S_E                (DBI_LOCK_CONFLICT+8)  /* shared to exclusive lock escalation   */
#define     DBI_LOCK_S_V_E              (DBI_LOCK_CONFLICT+9)  /* shared to version ex lock escalation  */
#define     DBI_UNLOCK                  (DBI_LOCK_CONFLICT+10) /* remove row lock                       */
#define     DBI_UNLOCK_U                (DBI_LOCK_CONFLICT+11) /* remove update ex transaction lock     */
#define     DBI_UNLOCK_S                (DBI_LOCK_CONFLICT+12) /* remove shared lock+ report conflicts  */
#define     DBI_LOCK_COMMIT             (DBI_LOCK_CONFLICT+13) /* commit, rollback lock                 */
#define     DBI_LOCK_E_S                (DBI_LOCK_CONFLICT+14) /* exclusive to shared                   */
#define     DBI_LOCK_V_E_S              (DBI_LOCK_CONFLICT+15) /* version exclusive to shared           */
#define     DBI_LOCK_NO_PRIMARY_KEY     (DBI_LOCK_CONFLICT+16) /* no primary key defined for table      */
#define     DBI_LOCK_INV_TYPE           (DBI_LOCK_CONFLICT+17) /* invalid lock type                     */
#define     DBI_LOCK_NO_ACT_LOCK        (DBI_LOCK_CONFLICT+18) /* no active lock                        */
#define     DBI_LOCK_AE                 (DBI_LOCK_CONFLICT+19) /* Workspace Manager lock type E         */

/************************************************************************
*  Define FDO Operation constants.                                      *
************************************************************************/

#define DBI_FDO_ACQUIRE_LOCK           0
#define DBI_FDO_RELEASE_LOCK           1
#define DBI_FDO_DELETE                 2
#define DBI_FDO_INSERT                 3
#define DBI_FDO_SELECT                 4
#define DBI_FDO_UPDATE                 5
#define DBI_FDO_COMMIT_LT              6
#define DBI_FDO_ROLLBACK_LT            7

/************************************************************************
*  Define lock related constants.                                       *
************************************************************************/

#define DBI_LOCK_STRATEGY_ALL           0
#define DBI_LOCK_STRATEGY_PARTIAL       1

class LockUtility
{

  friend class FdoRdbmsConnection;

  public:

    // Class Enumerations:
    //   ClassType      : The collection contains the different types of base
    //                    classes a class can belong to. This enumeration is
    //                    replaced ones the SchemaManager is in place.
    //   ColumnToken    : The enumeration contains tokens to identify the
    //                    role of different column names.
    //   ExceptionIds   : The enumeration contains execption ids.

    enum ClassTypes       {CLASS,
                           FEATURECLASS,
                           UNSUPPORTED_CLASS_TYPE};

    enum ColumnToken      {LT_ID_COLUMN,
                           LT_NAME_COLUMN,
                           IDENTITY_COLUMN,
                           ROW_LOCK_COLUMN,
                           CLASS_ID_COLUMN,
                           CLASS_NAME_COLUMN,
                           LOCK_OWNER_COLUMN,
                           LT_IN_CURR_COLUMN,
                           ROW_LOCK_TYPE_COLUMN};

    enum ExceptionIds     {ACTIVE_LT_ERROR,
                           ADMINISTRATOR_ROLE_REQUIRED,
                           CLASS_NAME_RETRIEVAL_FAILURE,
                           CLEAR_LOCK_REPORT_FAILURE,
                           CLOSING_LOCK_CONFLICT_READER_FAILURE,
                           CLOSING_LOCK_INFO_READER_FAILURE,
                           CLOSING_LOCK_OWNERS_READER_FAILURE,
                           CREATE_IDENTITY_FAILURE,
                           DBI_LOCK_FAILURE,
                           DBI_UNLOCK_FAILURE,
                           ERROR_LOADING_CONFLICT_INFORMATION,
                           ERROR_LOADING_LOCK_INFO_DATA,
                           ERROR_LOADING_LT_INFO_DATA,
                           ERROR_LOADING_OWNER_INFORMATION,
                           FAILED_TO_CREATE_LKOR,
                           GET_LOCK_INFO_CMD_FAILURE,
                           GET_LOCK_INFO_EXECUTION_FAILURE,
                           IDENTITY_RETRIEVAL_FAILURE,
                           INVALID_CONTEXT,
                           INVALID_CLASS_NAME,
                           INVALID_DBI_CONNECTION,
                           INVALID_LOCK_OWNER,
                           INVALID_LT_NAME,
                           LOCK_CONFLICT_GENERATION_ERROR,
                           LOCK_CONFLICT_INITIALIZATION_ERROR,
                           LOCK_CONFLICT_REPORT_GENERATION_ERROR,
                           LOCK_EXECUTION_FAILURE,
                           LOCK_INFO_READER_GENERATION_ERROR,
                           LOCK_LOCK_CONFL_ERROR,
                           LOCK_LT_RETRIEVAL_FAILURE,
                           LOCK_OWNER_READER_GENERATION_ERROR,
                           LOCK_OWNER_RETRIEVAL_FAILURE,
                           LOCK_SUPPORT_ERROR,
						   LOCK_SUPPORT_DATASTORE_ERROR,
                           LOCK_TYPE_INPUT_ERROR,
                           LOCK_TYPE_RETRIEVAL_FAILURE,
                           LOCK_VERSION_CONFL_ERROR,
						   LOCK_ACTIVE_TRANSACTION_ERROR,
						   LOCK_REQUEST_PROCESSING_ERROR,
                           LOCKED_OBJECTS_READ_ERROR,
                           LT_ACTIVE_SESSION_ERROR,
                           LT_CONFLICT_ENUM_GENERATION_ERROR,
                           LT_CONFLICT_ENUM_NOT_SET,
                           LT_CONFLICT_ENV_ERROR,
                           LT_CONFLICT_LOAD_ERROR,
                           LT_DESCRIPTION_LENGTH_ERROR,
                           LT_DOES_NOT_EXIST,
                           LT_LEAF_ERROR,
                           LT_NAME_LENGTH_ERROR,
                           LT_NAME_NOT_UNIQUE,
                           LT_NAME_NULL_ERROR,
                           MEMORY_ALLOCATION_ERROR,
                           NO_CONFLICT_VIEW_NAME,
                           PROCESS_SET_LOCK_INFO_REQUEST_CMD_ERROR,
                           READ_NEXT_CONFLICT_FAILURE,
                           READ_NEXT_LOCK_INFO_FAILURE,
                           READ_NEXT_LT_INFO_FAILURE,
                           READ_NEXT_OWNER_FAILURE,
                           READER_POSITIONING_ERROR,
                           ROOT_LT_ERROR,
                           TRANSACTION_LOCK_FAILURE,
                           UNLOCK_EXECUTION_FAILURE,
                           UNSUPPORTED_LOCK_TYPE,
                           UNSUPPORTED_ORACLE_VERSION,
                           WORKSPACE_MANAGER_GET_LOCK_INFO_CMD_FAILURE,
                           WORKSPACE_MANAGER_LOCK_FAILURE,
                           WORKSPACE_MANAGER_UNLOCK_FAILURE};


    // Class Functions:
    //   SetValue                  : Allocates memory and initializes it.
    //   GetClassName              : Retrieves a class name based on either a
    //                               given class id or table name.
    //   GetFilterSQL              : Returns the SQL equivilent for a filter.
    //   GetLockOwner              : Returns the owner of the lock identified
    //                               by a lock id.
    //   GetClassTable             : Returns the name of the table associated
    //                               with the given class name.
    //   ConvertString             : Various functions to convert a string.
    //   GetPropertyName           : Returns the property name for the given
    //                               column name for the identified class.
    //   GetFilterExtension        : Retrieves a filter extension depending on
    //                               the given class type and returns it back
    //                               to the calling routine.
    //   GetIdentityPropertyName   : Returns the property name for the given
    //                               identity column name for the identified
    //                               class.
    //   GetExceptionMessage       : Returns a string associated with the
    //                               given exception id.
    //   GetClassIdentityString    : Returns a comma-separated string contain-
    //                               ing the identity columns for the class
    //                               identified by the given class name.
    // ------------------------------------------------------------------------
    //   ActivateLock              : Activates a lock.
    //   ExecuteDbiCommand         : Executes a DBI command.
    //   GetClassId                : Returns the class id for a given class name.
    //   GetClassType              : Returns the class type for the class
    //                               identified by the given class name.
    //   GetLockId                 : Returns a lock id for thegiven lock name.
    //   GetNumberOfEntries        : Executes a command and returns the number
    //                               of rows returned by the query.
    //   HasEntries                : Executes a given command and returns TRUE
    //                               if the query produces at least one row,
    //                               FALSE otherwise.
    //   IsLockSupported           : Returns TRUE, if the given connection and
    //                               class supports locking, FALSE otherwise.
    //   IsSystemTable             : Returns TRUE if the given table name
    //                               represents a system table, FALSE otherwise.
    //   IsValidName               : Verifies that the given name does not
    //                               contain of spaces only, has leading and/or
    //                               trailing blanks or blanks as part of the
    //                               name.
    //   MaintainLockData          : If necessary creates a lock and activates
    //                               it.
    //   ProcessLockRequest        : Executes a lock related operation.
    // ------------------------------------------------------------------------
    //   SetActiveLock             : Activates the given lock name.
    // ------------------------------------------------------------------------
    //   GetColumnToken            : Returns the token associated with the
    //                               given column name.
    // ------------------------------------------------------------------------
    //   CreateIdentity            : Creates an object of class FdoPropertyValue
    //                               and initializes it.
    // ------------------------------------------------------------------------
    //   CreateIdentityCollection  : Creates a property value collection object
    //                               object and returns a handle to it.
    // ------------------------------------------------------------------------
    //   GetClassDefinition        : Returns the class definition for the given
    //                               class name.


    static char    *SetValue (
                            char                       *value_buffer);
    static char    *SetValue (
                            const char                 *value_buffer);
    static wchar_t *SetValue (
                            const wchar_t              *value_buffer);
    static char    *GetLockOwner (
                            FdoRdbmsConnection              *fdoConnection,
                            char                       *lock_id);
    static wchar_t    *GetClassName (
                            FdoRdbmsConnection              *dbi_connection,
                            FdoInt64						class_id,
                            FdoString                       *table_name);
    static const char    *GetFilterSQL (
                            FdoRdbmsConnection              *dbi_connection,
                            FdoRdbmsFilterProcessor    *filter_utility,
                            FdoIdentifier              *class_name,
                            FdoFilter                  *fdo_filter,
                            bool                       class_name_modified);
    static char    *GetClassTable (
                            FdoRdbmsConnection              *dbi_connection,
                            FdoIdentifier              *class_name);
    static char    *GetPropertyName (
                            FdoRdbmsConnection              *dbi_connection,
                            char                       *class_name,
                            char                       *column_name);
    static char    *GetFilterExtension (
                            FdoRdbmsConnection              *dbi_connection,
                            FdoRdbmsFilterProcessor    *filter_utility,
                            FdoIdentifier              *class_name,
                            FdoClassType               class_type);
    static char    *GetClassFeatIdColumn (
                            FdoRdbmsConnection              *dbi_connection,
                            FdoIdentifier              *class_name);
    static char    *GetIdentityPropertyName (
                            FdoRdbmsConnection              *dbi_connection,
                            char                       *class_name,
                            char                       *column_name);
    static char    *GetClassIdentityString (
                            FdoRdbmsConnection              *dbi_connection,
                            FdoIdentifier              *class_name);
    static char    *HandleFilter (
                            FdoRdbmsConnection              *dbi_connection,
                            FdoRdbmsFilterProcessor    *filter_utility,
                            FdoIdentifier              *class_name,
                            FdoIdentifier              *modified_class_name,
                            FdoFilter                  *fdo_filter,
                            bool                       class_name_modified);
    static char    *ConvertString (
                            FdoIdentifier              *the_identifier);
    static char    *ConvertString (
                            FdoRdbmsUtil               *util,
                            wchar_t                    *the_string);
    static char    *ConvertString (
                            FdoRdbmsUtil               *util,
                            const wchar_t              *the_string);
    static wchar_t *ConvertString (
                            char                       *the_string);
    static wchar_t *GetExceptionMessage (
                            ExceptionIds               exception_id);
    static wchar_t *GetExceptionMessage (
                            ExceptionIds               exception_id,
                            const wchar_t              *parameter_1);
    static wchar_t *GetExceptionMessage (
                            ExceptionIds               exception_id,
                            const wchar_t              *parameter_1,
                            const wchar_t              *parameter_2);

    static bool ActivateLock (
                            FdoRdbmsConnection              *dbi_connection,
                            const wchar_t                   *lock_name,
                            wchar_t                         *lock_user);
    static bool ExecuteDbiCommand (
                            FdoRdbmsConnection              *dbi_connection,
                            char                       *dbi_cmd,
                            int                        *dbi_code);
    static bool GetClassId (FdoRdbmsConnection              *dbi_connection,
                            FdoIdentifier              *class_name,
                            long                       *class_id);
    static bool GetClassType (
                            FdoRdbmsConnection              *dbi_connection,
                            FdoIdentifier              *class_name,
                            ClassTypes                 *class_type);
    static bool GetClassType (
                            FdoRdbmsConnection              *dbi_connection,
                            FdoIdentifier              *class_name,
                            FdoClassType               *class_type);
    static bool GetLockId ( FdoRdbmsConnection              *dbi_connection,
                            char                       *lock_name,
                            long                       *lock_id);
    static bool GetNumberOfEntries (
                            FdoRdbmsConnection              *dbi_connection,
                            char                       *cmd_buffer,
                            long                       *number_of_entries);
    static bool HasEntries (FdoRdbmsConnection              *dbi_connection,
                            char                       *cmd_buffer,
                            bool                       *has_entries);
    static bool IsLockSupported (
                            FdoRdbmsConnection         *fdo_rdbms_connection,
                            FdoIdentifier              *class_name);
    static bool IsSystemTable (
                            FdoRdbmsConnection              *dbi_connection,
                            char                       *table_name);
    static bool IsPropertyClassName (
                            FdoIdentifier              *class_name);
    static bool IsValidName (
                            FdoString                  *name);
    static bool MaintainLockData (
                            FdoRdbmsConnection              *dbi_connection,
                            char                       *lock_name);
    static bool ProcessLockRequest (
                            FdoRdbmsConnection              *dbi_connection,
                            FdoRdbmsFilterProcessor    *filter_utility,
                            FdoIdentifier              *class_name,
                            FdoIdentifier              *modified_class_name,
							bool						class_name_modified,
                            FdoFilter                  *fdo_filter,
							char					   **tableName,
							char					   **sqlFilter);

    static void SetActiveLock (
                            FdoRdbmsConnection              *dbi_connection,
                            char                       *lock_name);

    static ColumnToken GetColumnToken (
                            char                       *column_name);

    static FdoIdentifier *GetClassName (
                            FdoIdentifier              *class_name,
                            bool                       *class_name_modified);

    static FdoDataValue *ProcessIdentityValue (
                const FdoSmLpDataPropertyDefinition *property_definition,
                FdoString                                   *column_value);

    static FdoPropertyValue *CreateIdentity (
                            FdoRdbmsConnection              *dbi_connection,
                            FdoString                  *column_name,
                            FdoString                  *column_value,
                            FdoString                  *class_name);

    static FdoILockConflictReader *GetDefaultLockConflictReader (
                            FdoRdbmsConnection         *fdo_rdbms_connection,
                            FdoIdentifier              *class_name,
                            bool                       *execution_status);
    static FdoILockConflictReader *HandleLocks (
                            FdoRdbmsConnection         *fdo_rdbms_connection,
                            FdoIdentifier              *class_name,
                            FdoFilter                  *fdo_filter,
                            bool                       place_transaction_lock,
                            bool                       *lock_conflict_count,
                            bool                       *execution_status);

    static FdoPropertyValueCollection *CreateIdentityCollection ();

    static const FdoSmLpClassDefinition *GetClassDefinition (
                           FdoRdbmsConnection               *dbi_connection,
                           FdoString                   *class_name);

    static const FdoSmLpClassDefinition *GetClassDefinition (
                           FdoRdbmsConnection               *dbi_connection,
                           FdoIdentifier               *class_name);

    static const FdoSmLpDataPropertyDefinition
                    *GetIdentityPropertyDefinition (
                           FdoRdbmsConnection               *dbi_connection,
                           FdoString                   *class_name,
                           FdoString                        *column_name);

  protected:

    // Class Functions:

  private:

    // Class Enumerations:
    //   InternalExceptionIds : The collection contains exception codes used by the
    //                          functions of this class.

    enum InternalExceptionIds {DBI_EXECUTION,
                               CONVERSION_ERROR,
                               INVALID_INPUT_ERROR,
                               PROCESS_CLASS_REQUEST,
                               LOCK_MAINTENANCE_FAILURE,
                               MEMORY_ALLOCATION_INT_ERROR,
                               PROCESS_FEATURE_CLASS_REQUEST};

    // Class Functions:
    //   CreateLock            : Creates a lock definition for the given lock
    //                           name.
    //   LockExists            : Returns TRUE if a lock definition for the given
    //                           lock name exists, FALSE otherwise.
    //   ProcessRequest        : The function implements the execution of a lock
    //                           related request.
    //   ExecuteDbiLockCommands: Executes a DBI lock command.
    //
    //   GetExceptionMessage   : The function returns a string associated with
    //                           the given exception id.

                LockUtility             ();
                ~LockUtility            ();

    static bool CreateLock              (FdoRdbmsConnection        *dbi_connection,
                                         char                 *lock_name);
    static bool LockExists              (FdoRdbmsConnection        *dbi_connection,
                                         char                 *lock_name,
                                         bool                 *lock_exists);
    static bool ExecuteDbiLockCommands  (FdoRdbmsConnection        *dbi_connection,
                                         char                 *dbi_cmd,
                                         int                  *dbi_code);


    static wchar_t *GetExceptionMessage (InternalExceptionIds exception_id);

};

#endif
