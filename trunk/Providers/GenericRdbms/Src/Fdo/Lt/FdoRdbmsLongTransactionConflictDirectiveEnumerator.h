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
// | The file contains the definition of the class FdoRdbmsLongTransactionCon-
// | flictDirectiveEnumerator. The class implements functions for the long
// | transaction conflict resolution.
// =============================================================================

#ifndef FdoRdbmsLongTransactionConflictDirectiveEnumerator_h
#define FdoRdbmsLongTransactionConflictDirectiveEnumerator_h 1

// The links to the header files to be included.

#include "FdoRdbms.h"
#include "FdoRdbmsConnection.h"
#include "LongTransactionConflictInfos.h"


class FdoRdbmsLongTransactionConflictDirectiveEnumerator :
                        public FdoILongTransactionConflictDirectiveEnumerator
{

  friend class FdoRdbmsConnection;

  public:

    // ************************************************************************
    // *                           Public Interfaces                          *
    // ************************************************************************

    //  FdoRdbmsLongTransactionConflictDirectiveEnumerator:
    //      The class constructors.

    FdoRdbmsLongTransactionConflictDirectiveEnumerator ();
    FdoRdbmsLongTransactionConflictDirectiveEnumerator (
                        FdoRdbmsConnection               *connection,
                        FdoRdbmsLongTransactionConflicts *lt_conflicts);

    //  GetCount:
    //      The function returns the number of conflicts.

    FdoInt32 GetCount ();

    //  GetFeatureClassName:
    //      Returns the class name for the feature currently being read.

    FdoString *GetFeatureClassName ();

    //  GetIdentity:
    //      Returns the identity for the feature currently being read.

    FdoPropertyValueCollection *GetIdentity ();

    //  GetResolution:
    //      Returns the value for the conflict resolution parameter set for the
    //      feature currently being read.

    FdoLongTransactionConflictResolution GetResolution ();

    //  Invalidate:
    //      The function invalidates the reader.

    void Invalidate();

    //  ReadNext:
    //      Advances the reader to the next item. Since the default position of
    //      the reader is prior to the first item, "ReadNext" must be called to
    //      access any data.

    bool ReadNext ();

    //  Reset:
    //      Sets the enumerator to its initial position, which is before the
    //      first conflict.

    void Reset ();

    //  SetResolution:
    //      Sets the value for the conflict resolution parameter for the feature
    //      currently being read.

    void SetResolution (FdoLongTransactionConflictResolution value);


  protected:

    // ************************************************************************
    // *                         Protected Interfaces                         *
    // ************************************************************************

    //  Dispose:
    //      The function to delete an instance of this class.

    virtual void Dispose () { delete this; }

    //  ~FdoRdbmsLongTransactionConflictDirectiveEnumerator:
    //      The class destructor.

    virtual  ~FdoRdbmsLongTransactionConflictDirectiveEnumerator ();


  private:

    // ************************************************************************
    // *                          Private Interfaces                          *
    // ************************************************************************

    //  ClearMemory:
    //      Frees all memory allocated by the member variables.

    void ClearMemory ();

    //  CreateIdentity:
    //      The function creates an identity property data set using the pro-
    //      vided parameters.

    FdoPropertyValue *CreateIdentity (
                                FdoString               *column_name,
                                FdoString               *column_value,
                                FdoString               *class_name,
                                FdoSmLpQClassDefinition *q_class_definition);

    //  CreateIdentityCollection:
    //      The function creates a property value collection object and returns
    //      a handle to it back to the calling routine.

    FdoPropertyValueCollection *CreateIdentityCollection ();

    //  DetermineCflCount:
    //      Determines the number of long transaction conflicts.

    void DetermineCflCount ();

    //  GetIdentityPropertyDefinition:
    //      The function returns a data property definition.

    const FdoSmLpDataPropertyDefinition
                    *GetIdentityPropertyDefinition (
                                FdoString               *class_name,
                                FdoString               *column_name,
                                FdoSmLpQClassDefinition *q_class_definition);

    //  GetNextConflictData:
    //      Reads the next conflict information set.

    bool GetNextConflictData ();

    //  ProcessIdentityValue:
    //      The function processes the given identity column value.

    FdoDataValue *ProcessIdentityValue (
                  const FdoSmLpDataPropertyDefinition *property_definition,
                  FdoString                           *column_value);

    //  SetToZero:
    //      Initializes all class member variables.

    void SetToZero ();

    //  SetValue:
    //      The function allocates memory for a string and initializes it with
    //      the given value.

    wchar_t *SetValue (FdoString *value_buffer);


    // ************************************************************************
    // *                        Private Member Variables                      *
    // ************************************************************************

    //  curr_lt_conflict:
    //      References the currently processed long transaction conflict. The
    //      long transaction conflict identifies the currently processed table
    //      and provides a link to the actual conflict list.

    FdoRdbmsLongTransactionConflict *curr_lt_conflict;

    //  curr_lt_conflict_index:
    //      Indicates the position within the collection of long transaction
    //      conflicts.

    FdoInt32 curr_lt_conflict_index;

    //  curr_lt_conflict_info:
    //      References the currently processed long transaction conflict info
    //      data set.

    FdoRdbmsLongTransactionConflictInfo *curr_lt_conflict_info;

    //  curr_lt_conflict_info_index:
    //      Indicates the position within the collection of long transaction
    //      conflict infos.

    FdoInt32 curr_lt_conflict_info_index;

    //  curr_lt_conflict_info_root:
    //      The pointer references the currently processed long transaction
    //      conflict information list.

    FdoRdbmsLongTransactionConflictInfos *curr_lt_conflict_info_root;

    //  fdo_rdbms_connection:
    //      The variable references the FDO RDBMS connection provided when the
    //      enumerator is created.

    FdoRdbmsConnection *fdo_rdbms_connection;

    //  lt_cf_identities:
    //      The variable keeps a reference to the conflict identity properties..

    FdoPropertyValueCollection *lt_cf_identities;

    //  lt_cfl_class_name:
    //      The variable keeps a reference to the conflict class name.

    FdoString *lt_cfl_class_name;

    //  lt_cfl_count:
    //      The variable holds the number of long transaction conflicts.

    FdoInt32 lt_cfl_count;

    //  lt_conflicts_root:
    //      References the root of the list containing detected long transac-
    //      tion conflicts.

    FdoRdbmsLongTransactionConflicts *lt_conflicts_root;

    //  valid_reader_position:
    //      The variable indicates whether or not the reader is correctly
    //      positioned to read data.

    bool valid_reader_position;

};  //  class FdoRdbmsLongTransactionConflictDirectiveEnumerator

#endif
