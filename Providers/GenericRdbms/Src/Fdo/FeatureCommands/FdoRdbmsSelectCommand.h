#ifndef FDORDBMSSELECTCOMMAND_H
#define FDORDBMSSELECTCOMMAND_H        1
/*
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
 */

#include "FdoRdbms.h"

// FeatureCommand
#include "FdoRdbmsConnection.h"
#include "FdoRdbmsFeatureCommand.h"
#include "FdoRdbmsLockConflictReader.h"
#include <FdoExpressionEngineFunctionCollection.h>

class FeatureSet;

class FdoRdbmsSelectCommand : public FdoRdbmsFeatureCommand<FdoISelect>
{
    friend class FdoRdbmsConnection;
    friend class FdoRdbmsSelectAggregates;

    protected:
      DbiConnection* mConnection;
      FdoIConnection *mIConnection;
      FdoRdbmsConnection *mConn;
      FdoIdentifierCollection *mIdentifiers;

      FdoLockType                mLockType;
      FdoLockStrategy            mLockStrategy;
      FdoRdbmsLockConflictReader *mLockConflictReader;

      FdoOrderingOption          mOrderingOption;
      FdoIdentifierCollection   *mOrderingIdentifiers;
      FdoFilter                  *mGroupingFilter;

      FdoIdentifierCollection*   mGroupingCol;

      void**                     mBoundGeometries;
      FdoInt32                   mBoundGeometryCount;

      //
      // Prevent the use of the copy constructor by definning it and not implemeting it.
      // DO NOT IMPLEMENT
      FdoRdbmsSelectCommand(const FdoRdbmsSelectCommand &right);

      //    Constructs a default instance of a FdoRdbmsSelectCommand.
      FdoRdbmsSelectCommand ();

      //    Constructs an instance of a FdoRdbmsSelectCommand using the
      //    specified arguments.
      FdoRdbmsSelectCommand (FdoIConnection *connection);

      //    Constructs an instance of a FdoRdbmsSelectCommand using the
      //    specified arguments.
      //FdoRdbmsSelectCommand (IFeatureConnection *connection, String className);

      //    Executes the select command and returns an object that
      //    implements IFeatureReader. Only distinct properties will be returned if distinct is set to true
      FdoIFeatureReader *Execute ( bool distinct, FdoInt16 callerId = FdoCommandType_Select );

      virtual ~FdoRdbmsSelectCommand();

  private:
      bool HasLobProperty( const FdoSmLpClassDefinition *classDefinition );

	  // Internal method added in support for the expression engine 
	  FdoExpressionEngineFunctionCollection* GetUserDefinedFunctions( FdoSmLpSpatialContextCollection *scColl, FdoClassDefinition *classDef );

	  // Internal method added in support for processing spatial conditions.
	  void CheckSpatialFilters( FdoRdbmsSecondarySpatialFilterCollection * geometricConditions, vector<int> *logicalOps );

      // Binds spatial condition geometries to the query statement
      // (if provider supports bound spatial condition geometries)
      void  BindSpatialGeoms( GdbiStatement* statement, FdoRdbmsFilterProcessor::BoundGeometryCollection* geometries );

      // Frees all bound spatial condition geometries.
      void  FreeBoundSpatialGeoms();

  protected:
      // Internal method in support for select command 
      virtual FdoRdbmsFeatureReader *GetOptimizedFeatureReader( const FdoSmLpClassDefinition *classDefinition );

      // Internal method added in support for the select aggregates command
      void SetGroupingFilter( FdoFilter* filter )
      {
        FDO_SAFE_RELEASE(mGroupingFilter);
        mGroupingFilter = FDO_SAFE_ADDREF( filter );
      }

      // Internal method added in support for the select aggregates command
      FdoFilter* GetGroupingFilter( )
      {
        return FDO_SAFE_ADDREF( mGroupingFilter );
      }

      // Internal method added in support for the select aggregates command
      FdoIdentifierCollection* GetGrouping();


  public:

      //
      // Prevent the use of the Assignment Operation by definning it and not implemeting it.
      // DO NOT IMPLEMENT
      FdoRdbmsSelectCommand & operator=(const FdoRdbmsSelectCommand &right);

      //    Executes the select command and returns an object that
      //    implements IFeatureReader.
      virtual FdoIFeatureReader *Execute () { return Execute( false, FdoCommandType_Select ); }

    /// <summary>Gets the IdentifierCollection that holds the list of property names to
    /// return with the result. If empty all properties of the specified class
    /// are returned.</summary>
    /// <returns>Returns the list of property names.</returns>
    FdoIdentifierCollection* GetPropertyNames();

    /// <summary>Gets the LockType value.</summary>
    /// <returns>Returns the lock type.</returns>
    FdoLockType GetLockType();

    /// <summary>Sets the LockType value.</summary>
    /// <param name="value">Input the lock type.</param>
    /// <returns>Returns nothing</returns>
    void SetLockType(FdoLockType value);

    /// <summary>Gets the LockStrategy value (see "Feature Locking Commands").</summary>
    /// <returns>Returns the lock strategy.</returns>
    FdoLockStrategy GetLockStrategy();

    /// <summary>Sets the LockStrategy value (see "Feature Locking Commands").</summary>
    /// <param name="value">Input the lock strategy.</param>
    /// <returns>Returns nothing</returns>
    void SetLockStrategy(FdoLockStrategy value);

    /// <summary>Executes the select command and returns a reference to an
    /// IFeatureReader. During execution an attempt is made to place a lock
    /// on each feature. If there are conflicts the conflicts are reported
    /// and made available to the user via a lock conflict reader. The user
    /// retrieves the lock conflict reader when executing the function
    /// "GetLockConflicts()".</summary>
    /// <returns>Returns the feature reader.</returns>
    FdoIFeatureReader* ExecuteWithLock();

    /// <summary>The function returns the lock conflict reader containing any
    /// lock conflicts as detected during the execution of the select-and-lock
    /// command.</summary>
    /// <returns>Returns the lock conflict reader.</returns>
    FdoILockConflictReader* GetLockConflicts();

    /// <summary>Gets the FdoIdentifierCollection that holds the list of order by property names. If empty no ordering is used. This list is initially
    /// empty and the caller need to add the property that the command should use as a order by criteria.</summary>
    /// <returns>Returns the list of group by property names.</returns>
    virtual FdoIdentifierCollection* GetOrdering();

    /// <summary>Set the ordering option of the selection. This is only used if the ordering collection is not empty.</summary>
    /// <param name="option">Is the ordering option and should be set to one of FdoOrderingOption_Ascending or FdoOrderingOption_Descending.
    /// FdoOrderingOption_Ascending is the default value.</param>
    /// <returns>Returns nothing</returns>
    virtual void SetOrderingOption( FdoOrderingOption  option ) { mOrderingOption = option; }

    /// <summary>Gets the ordering option.</summary>
    /// <returns>Returns the ordering option.</returns>
    virtual FdoOrderingOption GetOrderingOption( ){ return mOrderingOption; }
};

#endif // FDORDBMSSELECTCOMMAND_H
