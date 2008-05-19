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
// | The file contains the implementation of the class FdoRdbmsLongTransaction-
// | ConflictDirectiveEnumerator. The class implements functions for the long
// | transaction conflict resolution.
// =============================================================================

// The links to the header files to be included.

#include "stdafx.h"
#include <malloc.h>

#include "FdoCommonOSUtil.h"
#include "FdoRdbmsLongTransactionConflictDirectiveEnumerator.h"
#include "LockConflict.h"
#include "FdoRdbmsException.h"


// ----------------------------------------------------------------------------
// --                         Constructors/Destructors                       --
// ----------------------------------------------------------------------------

FdoRdbmsLongTransactionConflictDirectiveEnumerator::
                           FdoRdbmsLongTransactionConflictDirectiveEnumerator()

// +---------------------------------------------------------------------------
// | The function represents a class constructor.
// +---------------------------------------------------------------------------

{

    // Initialize the member variables.

    SetToZero();

}  //  FdoRdbmsLongTransactionConflictDirectiveEnumerator ()

FdoRdbmsLongTransactionConflictDirectiveEnumerator::
          FdoRdbmsLongTransactionConflictDirectiveEnumerator(
                            FdoRdbmsConnection               *connection,
                            FdoRdbmsLongTransactionConflicts *lt_conflicts)

// +---------------------------------------------------------------------------
// | The function represents a class constructor.
// +---------------------------------------------------------------------------

{

    // Initialize the member variables.

    SetToZero();

    // Set some member variables.

    lt_conflicts_root    = lt_conflicts;
    fdo_rdbms_connection = connection;
    fdo_rdbms_connection->AddRef();

    // Create the identity collection.

    lt_cf_identities = CreateIdentityCollection();

    // Determine the number of long transaction conflicts.

    DetermineCflCount();

}  //  FdoRdbmsLongTransactionConflictDirectiveEnumerator ()

FdoRdbmsLongTransactionConflictDirectiveEnumerator::
                          ~FdoRdbmsLongTransactionConflictDirectiveEnumerator()

// +---------------------------------------------------------------------------
// | The function represents the class destructor.
// +---------------------------------------------------------------------------

{

    // Release the memory occupied by the current class variables..

    ClearMemory();
    fdo_rdbms_connection->Release();

}  //  ~FdoRdbmsLongTransactionConflictDirectiveEnumerator ()


// ----------------------------------------------------------------------------
// --                            Public Interfaces                           --
// ----------------------------------------------------------------------------

FdoString *FdoRdbmsLongTransactionConflictDirectiveEnumerator::
                                                         GetFeatureClassName ()

// +---------------------------------------------------------------------------
// | The function returns the class name for the feature currently read.
// +---------------------------------------------------------------------------

{

    // If the reader is not positioned correctly issue an exception. Otherwise
    // provide the requested information.

    if (!valid_reader_position)
        throw FdoCommandException::Create(
                NlsMsgGet1(FDORDBMS_383,
                          "Reader '%1$ls' is not positioned correctly",
                          L"FdoILongTransactionConflictDirectiveEnumerator"));

    return (lt_cfl_class_name);

}  //  GetFeatureClassName ()

FdoPropertyValueCollection
            *FdoRdbmsLongTransactionConflictDirectiveEnumerator::GetIdentity ()

// +---------------------------------------------------------------------------
// | The function returns the identity for the feature currently read.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    int                        i,
                               count;

    FdoPropertyValue           *property_value             = NULL;

    FdoCommandException        *fdo_cmd_ex                 = NULL;

    FdoPropertyValueCollection *property_value_collection  = NULL;

    // If the reader is not positioned correctly issue an exception.

    if (!valid_reader_position)
        throw FdoCommandException::Create(
                NlsMsgGet1(FDORDBMS_383,
                           "Reader '%1$ls' is not positioned correctly",
                           L"FdoILongTransactionConflictDirectiveEnumerator"));

    try {

      // Create a property value collection object.

      if ((property_value_collection = CreateIdentityCollection()) == NULL) {

           ClearMemory();
           throw FdoCommandException::Create(
                        NlsMsgGet(FDORDBMS_82, "Failed to retrieve identity"));

      }  //  if ((property_value_collection = ...

      // Copy all the entries from the internal identity collection to this
      // newly created one.

      if (lt_cf_identities == NULL) return property_value_collection;

      count = lt_cf_identities->GetCount();
      for (i = 0; i < count; i++) {

        property_value = lt_cf_identities->GetItem(i);
        property_value_collection->Add(property_value);
        property_value->Release();

      }  //  for (i = 0; ...

      return property_value_collection;

    }  //  try ...

    catch (FdoException *ex) {

      ClearMemory();
      fdo_cmd_ex = FdoCommandException::Create(ex->GetExceptionMessage(), ex);
      ex->Release();
      throw fdo_cmd_ex;

    }  //  catch (FdoException *ex)

    catch ( ... ) {

      ClearMemory();
      throw;

    }  //  catch ( ... ) ...

}  //  GetIdentity ()

FdoLongTransactionConflictResolution
          FdoRdbmsLongTransactionConflictDirectiveEnumerator::GetResolution ()

// +---------------------------------------------------------------------------
// | The function returns the resolution set for the current conflict.
// +---------------------------------------------------------------------------

{

    // Issue an exception if the user attempts to access the enumerator data
    // without setting it first. Otherwise, map the internal value of the flag
    // to the required return parameter.

    if (!valid_reader_position)
        throw FdoCommandException::Create(
                NlsMsgGet1(FDORDBMS_383,
                           "Reader '%1$ls' is not positioned correctly",
                           L"FdoILongTransactionConflictDirectiveEnumerator"));

    switch (curr_lt_conflict_info->GetConflictSolution()) {

      case FdoRdbmsLongTransactionConflictSolution_Unresolved:
        return (FdoLongTransactionConflictResolution_Unresolved);
        break;

      case FdoRdbmsLongTransactionConflictSolution_Parents:
        return (FdoLongTransactionConflictResolution_Parent);
        break;

      case FdoRdbmsLongTransactionConflictSolution_Child:
        return (FdoLongTransactionConflictResolution_Child);
        break;

      default:
        return (FdoLongTransactionConflictResolution_Unresolved);
        break;

    }  //  switch ...

}  //  GetResolution ()

void FdoRdbmsLongTransactionConflictDirectiveEnumerator::SetResolution                                      (
                                   FdoLongTransactionConflictResolution value)

// +---------------------------------------------------------------------------
// | The function sets the resolution for the current conflict.
// +---------------------------------------------------------------------------

{

    // Issue an exception if the user attempts to access the enumerator data
    // without setting it first. Otherwise, map the given value to the internal
    // counterpart and set the flag for the current conflict.

    if (!valid_reader_position)
        throw FdoCommandException::Create(
                NlsMsgGet1(FDORDBMS_383,
                           "Reader '%1$ls' is not positioned correctly",
                           L"FdoILongTransactionConflictDirectiveEnumerator"));

    switch (value) {

      case FdoLongTransactionConflictResolution_Unresolved:
        curr_lt_conflict_info->SetConflictSolution(
                        FdoRdbmsLongTransactionConflictSolution_Unresolved);
        break;

      case FdoLongTransactionConflictResolution_Parent:
        curr_lt_conflict_info->SetConflictSolution(
                            FdoRdbmsLongTransactionConflictSolution_Parents);
        break;

      case FdoLongTransactionConflictResolution_Child:
        curr_lt_conflict_info->SetConflictSolution(
                            FdoRdbmsLongTransactionConflictSolution_Child);
        break;

    }  //  switch ...

}  //  SetResolution ()

bool FdoRdbmsLongTransactionConflictDirectiveEnumerator::ReadNext ()

// +---------------------------------------------------------------------------
// | The function moves the cursor to the next entry in the enumerator.
// +---------------------------------------------------------------------------

{

    // If there are no long transaction conflicts to be processed terminate the
    // function.

    if (lt_cfl_count == 0) return false;

    // There are long transaction conflicts to be processed. Set the pointers
    // the next conflict. The returned boolean value will indicate whether or
    // not all conflicts have already been processed.

    valid_reader_position = GetNextConflictData();

    return (valid_reader_position);

}  //  ReadNext ()

void FdoRdbmsLongTransactionConflictDirectiveEnumerator::Reset ()

// +---------------------------------------------------------------------------
// | The function resets the enumerator.
// +---------------------------------------------------------------------------

{

    // To reset the enumerator, reset the indexes indicating the position of
    // the pointers within the various collections. Then reset the pointers
    // itself.

    curr_lt_conflict_index      = -1;
    curr_lt_conflict_info_index = -1;

    if (curr_lt_conflict_info != NULL) {

        curr_lt_conflict_info->Release();
        curr_lt_conflict_info = NULL;

    }  //  if (curr_lt_conflict_info != NULL) ...

    if (curr_lt_conflict_info_root != NULL) {

        curr_lt_conflict_info_root->Release();
        curr_lt_conflict_info_root = NULL;

    }  //  if (curr_lt_conflict_info_root != NULL) ...

    if (curr_lt_conflict != NULL) {

        curr_lt_conflict->Release();
        curr_lt_conflict = NULL;

    }  //  if (curr_lt_conflict != NULL) ...

}  //  Reset ()

void FdoRdbmsLongTransactionConflictDirectiveEnumerator::Invalidate ()

// +---------------------------------------------------------------------------
// | The function invalidates the enumerator.
// +---------------------------------------------------------------------------

{

    ClearMemory();

}  //  Invalidate ()

FdoInt32 FdoRdbmsLongTransactionConflictDirectiveEnumerator::GetCount ()

// +---------------------------------------------------------------------------
// | The function returns the number of conflicts stored in the enumerator.
// +---------------------------------------------------------------------------

{

    return (lt_cfl_count);

}  //  GetCount ()


// ----------------------------------------------------------------------------
// --                          Supporting functions                          --
// ----------------------------------------------------------------------------

void FdoRdbmsLongTransactionConflictDirectiveEnumerator::ClearMemory ()

// +---------------------------------------------------------------------------
// | The function frees memory.
// +---------------------------------------------------------------------------

{

    if (curr_lt_conflict != NULL) {

        curr_lt_conflict->Release();
        curr_lt_conflict = NULL;

    }  //  if (curr_lt_conflict != NULL) ...

    if (curr_lt_conflict_info != NULL) {

        curr_lt_conflict_info->Release();
        curr_lt_conflict_info = NULL;

    }  //  if (curr_lt_conflict_info != NULL) ...

    if (curr_lt_conflict_info_root != NULL) {

        curr_lt_conflict_info_root->Release();
        curr_lt_conflict_info_root = NULL;

    }  //  if (curr_lt_conflict_info_root != NULL) ...

    if (lt_cfl_class_name != NULL) delete[] lt_cfl_class_name;

    if (lt_cf_identities != NULL) {

        lt_cf_identities->Clear();
        lt_cf_identities->Release();

    };  //  if (lt_cf_identities != NULL);

    SetToZero();

}  //  ClearMemory ()

FdoPropertyValue 
    *FdoRdbmsLongTransactionConflictDirectiveEnumerator::
                        CreateIdentity (
                                FdoString               *column_name,
                                FdoString               *column_value,
                                FdoString               *class_name,
                                FdoSmLpQClassDefinition *q_class_definition)

// +---------------------------------------------------------------------------
// | The function creates a property value object based on the given data and
// | returns the object back the calling routine.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    const FdoSmLpDataPropertyDefinition *class_identity_property;

    FdoDataValue                        *data_value                = NULL;

    FdoPropertyValue                    *property_value_obj        = NULL;

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

          return property_value_obj;

      }  //  if (class_name == NULL) ...

      // A class name has been specified. Get hold of the property definition
      // identified by the given column and class name.

      if ((class_identity_property =
                      GetIdentityPropertyDefinition(
                                                class_name,
                                                column_name,
                                                q_class_definition)) == NULL)
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

FdoPropertyValueCollection 
        *FdoRdbmsLongTransactionConflictDirectiveEnumerator::
                                                CreateIdentityCollection ()

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

void FdoRdbmsLongTransactionConflictDirectiveEnumerator::DetermineCflCount ()

// +---------------------------------------------------------------------------
// | The function reads the next conflict data set and processes it.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoInt32                             i,
                                         cfl_table_count;

    FdoRdbmsLongTransactionConflict      *curr_cfl           = NULL;

    FdoRdbmsLongTransactionConflictInfo  *curr_cfl_info      = NULL;

    FdoRdbmsLongTransactionConflictInfos *curr_cfl_info_root = NULL;

    // If the conflict list is empty, terminate the routine.

    if ((lt_conflicts_root == NULL         ) ||
        (lt_conflicts_root->GetCount() == 0)    )
        return;

    // Long transaction conflicts for different tables exist. To get the
    // number of all conflicts loop through the list of tables causing a
    // conflict and add the number of conflicts for each table. 

    cfl_table_count = lt_conflicts_root->GetCount();

    for (i = 0; i < cfl_table_count; i++ ) {

      curr_cfl            = lt_conflicts_root->GetItem(i);
      curr_cfl_info_root  = curr_cfl->GetConflicts();
      lt_cfl_count       += curr_cfl_info_root->GetCount();

      curr_cfl_info_root->Release();
      curr_cfl->Release();

      curr_cfl_info_root = NULL;
      curr_cfl           = NULL;

    }  //  for (i = 0; ...

}  //  DetermineCflCount ()

const FdoSmLpDataPropertyDefinition
        *FdoRdbmsLongTransactionConflictDirectiveEnumerator::
                        GetIdentityPropertyDefinition (
                                FdoString               *class_name,
                                FdoString               *column_name,
                                FdoSmLpQClassDefinition *q_class_definition)

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

    FdoRdbmsUtil *utility = NULL;

    // Set the class definition.

    class_definition = q_class_definition->RefClassDefinition();

    // Get hold of the class identity. If there is no class identity return a
    // NULL pointer back to the calling routine.

    class_identities = class_definition->RefIdentityProperties();
    if ((class_identity_count = class_identities->GetCount()) == 0)
                                                                   return NULL;

    // Query the collection of class identities to find the one that is
    // identified by the given column name. If the identity is found re-
    // turn it back to the calling routine.

    utility = fdo_rdbms_connection->GetDbiConnection()->GetUtility();

    for (i = 0; i < class_identity_count; i++) {

         class_identity          = class_identities->RefItem(i);
         class_identity_property =
                    static_cast<const FdoSmLpDataPropertyDefinition*>(
                                                               class_identity);
         if (utility->StrCmp(
                    column_name,
                    class_identity_property->RefColumn()->GetName()) == 0)
             return class_identity_property;

    }  //  for (i = 0; ...

    // No matching identity has been found. Return a NULL pointer back to the
    // calling routine.

    return NULL;

}  //  GetIdentityPropertyDefinition ()

bool FdoRdbmsLongTransactionConflictDirectiveEnumerator::GetNextConflictData ()

// +---------------------------------------------------------------------------
// | The function reads the next conflict data set and processes it.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoInt32                      i                   = 0,
                                  pk_count            = 0;

    const FdoSmLpSchemaCollection *schemas            = NULL;
    FdoSmLpQClassesP              classes;
    FdoSchemaManagerP             schema_manager;
    FdoSmLpQClassDefinition       *q_class_definition = NULL;

    FdoRdbmsPrimaryKey            *curr_pk            = NULL;
    FdoRdbmsPrimaryKeyColumn      *curr_pk_col        = NULL;

    FdoPropertyValue              *identity           = NULL;

    try {

      // Set the pointers to the current long transaction conflict data set
      // that needs to be processed.

      if (curr_lt_conflict_index == -1) {

          // The pointers have not been initially set. This is done next.

          curr_lt_conflict_index      = 0;
          curr_lt_conflict_info_index = 0;

          curr_lt_conflict            =
            lt_conflicts_root->GetItem(curr_lt_conflict_index);
          curr_lt_conflict_info_root  = curr_lt_conflict->GetConflicts();
          curr_lt_conflict_info       = 
            curr_lt_conflict_info_root->GetItem(curr_lt_conflict_info_index);

      }  //  if (curr_lt_conflict_index == -1) ...
      else {

        // All the pointers are set. Check if there is another element in the
        // currently processed long transaction conflict info list. If this is
        // the case reference it as the next conflict to be processed.

        curr_lt_conflict_info_index++;
        if (curr_lt_conflict_info_index <
                                    curr_lt_conflict_info_root->GetCount()) {

            curr_lt_conflict_info->Release();
            curr_lt_conflict_info = NULL;

            curr_lt_conflict_info = 
              curr_lt_conflict_info_root->GetItem(curr_lt_conflict_info_index);

        }  //  if (curr_lt_conflict_info_index < ...
        else {

          // There is no more long transaction conflict info data set to be
          // processed in the current long transaction conflict info list.
          // Check if there are more long transaction conflict objects. If this
          // is not the case all long transaction conflicts are processed and
          // the procedure can be terminated indicating that all conflicts have
          // been processed. Otherwise, set the pointer to the next long trans-
          // action conflict and the resulting long transaction info set.

          curr_lt_conflict_index++;
          curr_lt_conflict_info_index = 0;

          curr_lt_conflict_info->Release();
          curr_lt_conflict_info = NULL;

          curr_lt_conflict_info_root->Release();
          curr_lt_conflict_info_root = NULL;
        
          curr_lt_conflict->Release();
          curr_lt_conflict = NULL;

          if (curr_lt_conflict_index < lt_conflicts_root->GetCount()) {

              curr_lt_conflict           =
                        lt_conflicts_root->GetItem(curr_lt_conflict_index);
              curr_lt_conflict_info_root = curr_lt_conflict->GetConflicts();
              curr_lt_conflict_info      = 
                        curr_lt_conflict_info_root->GetItem(
                                                curr_lt_conflict_info_index);

          }  //  if (curr_lt_conflict_index < ...
          else
            return false;

        }  //  else ...

      }  //  else ...

      // For the current long transaction conflict information data set deter-
      // mine the class name and identity. This requires the release of the
      // currently held data in the corresponding variables.

      if (lt_cfl_class_name != NULL) {

          delete[] lt_cfl_class_name;
          lt_cfl_class_name = NULL;

      }  //  if (lt_cfl_class_name != NULL) ...

      if (lt_cf_identities != NULL)
          lt_cf_identities->Clear();

      // To determine the class name and identity for the current long trans-
      // action conflict get hold of the Schema Manager.

      schema_manager = fdo_rdbms_connection->GetSchemaManager();
      schemas        = schema_manager->RefLogicalPhysicalSchemas();
      classes        = schemas->TableToClasses(
                                            curr_lt_conflict->GetTableName(),
                                            curr_lt_conflict->GetDataStore(),
                                            L"");

      // Any further steps depend on the number of classes associated with the
      // current table:
      //
      //  No class returned:
      //      In this case, the class name and identity cannot be set. However,
      //      to document the long transaction conflict, the identity will be
      //      set using the column names rather than the identity names.
      //
      //  One class returned:
      //      In this case, the class name and identity can be set properly.
      //
      //  More than one class returned:
      //      In this case, the class name cannot be set as it is impossible
      //      to identify the class indicated by the row causing the conflict.
      //      Because all classes write to the same table the assumption could
      //      be made that they all share the same identity properties. How-
      //      ever, although this may be a valid assumption, it cannot be gen-
      //      eralized. Hence, the code uses the column names rather than the
      //      identity properties to document the conflict.

      if (classes->GetCount() == 1) {

          q_class_definition = classes->GetItem(0);
          if ((lt_cfl_class_name =
                            SetValue(q_class_definition->GetName())) == NULL)
              throw FdoCommandException::Create(
                        NlsMsgGet(FDORDBMS_91, "Failed to allocate memory"));

      }  //  if (classes->GetCount() == 1) ...

      curr_pk  = curr_lt_conflict_info->GetPk();
      pk_count = curr_pk->GetCount();
  
      for (i = 0; i < pk_count; i++) {

        curr_pk_col = curr_pk->GetItem(i);
        if ((identity = CreateIdentity(
                                    curr_pk_col->GetColumnName(),
                                    curr_pk_col->GetColumnValue(),
                                    lt_cfl_class_name,
                                    q_class_definition)           ) == NULL)
            throw FdoRdbmsException::Create(
                    NlsMsgGet(FDORDBMS_82, "Failed to retrieve identity"));

        lt_cf_identities->Add(identity);
        identity->Release();

        curr_pk_col->Release();
        curr_pk_col = NULL;

      }  //  for ((identity = ...

      curr_pk = NULL;

      q_class_definition->Release();
      q_class_definition = NULL;

      return (true);

    }  //  try ...

    catch ( ... ) {

      // Clean up.

      if (curr_pk_col != NULL) {

          curr_pk_col->Release();
          curr_pk_col = NULL;

      }  //  if (curr_pk_col != NULL) ...

      if (identity != NULL) {

        identity->Release();
        identity = NULL;

      }  //  if (identity != NULL) ...

      if (lt_cf_identities->GetCount())
          lt_cf_identities->Clear();

      // Rethrow the exception.

      throw;

    }  //  catch ...

}  //  GetNextConflictData ()

FdoDataValue
    *FdoRdbmsLongTransactionConflictDirectiveEnumerator::
            ProcessIdentityValue (
                  const FdoSmLpDataPropertyDefinition *property_definition,
                  FdoString                           *column_value)

// +---------------------------------------------------------------------------
// | The function processes the given identity value.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoDataValue *data_value = NULL;

    FdoRdbmsUtil *utility    = NULL;

    // Set a shortcut.

    utility = fdo_rdbms_connection->GetDbiConnection()->GetUtility();

    // Convert the given value into the type that corresponds to the identity
    // property type.

    switch (property_definition->GetDataType()) {

      case FdoDataType_Int16:
        data_value = FdoDataValue::Create(
                            (FdoInt16) FdoCommonOSUtil::wtoi(column_value));
        break;

      case FdoDataType_Int32:
        data_value = FdoDataValue::Create(
                            (FdoInt32) FdoCommonOSUtil::wtoi(column_value));
        break;

      case FdoDataType_Int64:
        data_value = FdoDataValue::Create(
                            (FdoInt64) FdoCommonOSUtil::wtoi(column_value));
        break;

      case FdoDataType_String:
      default:
        if (utility->StrCmp(column_value, L"") == 0)
            data_value = FdoDataValue::Create(L"");
        else 
          data_value = FdoDataValue::Create(column_value);
        break;

    }  //  switch ...

    return data_value;

}  //  ProcessIdentityValue ()

void FdoRdbmsLongTransactionConflictDirectiveEnumerator::SetToZero ()

// +---------------------------------------------------------------------------
// | The function initializes the member variables.
// +---------------------------------------------------------------------------

{

    valid_reader_position       = false;

    lt_cfl_count                = 0;
    curr_lt_conflict_index      = -1;
    curr_lt_conflict_info_index = -1;

    curr_lt_conflict            = NULL;
    lt_conflicts_root           = NULL;
    curr_lt_conflict_info       = NULL;
    curr_lt_conflict_info_root  = NULL;

    lt_cf_identities            = NULL;
    lt_cfl_class_name           = NULL;

}  //  SetToZero ()

wchar_t *FdoRdbmsLongTransactionConflictDirectiveEnumerator::
                                            SetValue (FdoString *value_buffer)

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
