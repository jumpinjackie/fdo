#ifndef _SdfSelect_H_
#define _SdfSelect_H_
// 
//  Copyright (C) 2004-2006  Autodesk, Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
#include "SdfFeatureCommand.h"

class PropertyIndex;

class SdfSelect : public SdfFeatureCommand<FdoISelect>
{

    //-------------------------------------------------------
    // Constructor / destructor
    //-------------------------------------------------------

    public:
        // constructs a Select command using the specified connection
        SDF_API SdfSelect(SdfConnection* connection);

    protected:
        // default constructor
        SDF_API SdfSelect();
        // default destructor
        SDF_API virtual ~SdfSelect();

    //-------------------------------------------------------
    // FdoISelect implementation
    //-------------------------------------------------------

    public:
        // Gets the IdentifierCollection that holds the list of property names to
        // return with the result.  If empty all properties of the specified class
        // are returned.
        SDF_API virtual FdoIdentifierCollection* GetPropertyNames();

        // Gets the LockType value (see "Feature Locking Commands").
        SDF_API virtual FdoLockType GetLockType();

        // Sets the LockType value (see "Feature Locking Commands").
        SDF_API virtual void SetLockType(FdoLockType value);

        // Gets the LockStrategy value (see "Feature Locking Commands").
        SDF_API virtual FdoLockStrategy GetLockStrategy();

        // Sets the LockStrategy value (see "Feature Locking Commands").
        SDF_API virtual void SetLockStrategy(FdoLockStrategy value);

        // Executes the select command and returns a reference to an IFeatureReader.
        virtual FdoIFeatureReader* Execute();

        // Executes the select command and returns a reference to an IFeatureReader.
        SDF_API virtual FdoIFeatureReader* ExecuteWithLock();

        // When executing the operation ExecuteWithLock lock conflicts may occur.
        // Those lock conflicts are reported.  The function GetLockConflicts returns
        // a lock conflict reader providing access to the list of lock conflicts
        // that occurred during the execution of the operation.
        SDF_API virtual FdoILockConflictReader* GetLockConflicts();


        /// <summary>Gets the FdoIdentifierCollection that holds the list of order by property names. If empty no ordering is used. This list is initially
        /// empty and the caller need to add the property that the command should use as a order by criteria.</summary>
        /// <returns>Returns the list of group by property names.</returns> 
        SDF_API virtual FdoIdentifierCollection* GetOrdering(){ return NULL; }

        /// <summary>Set the ordering option of the selection. This is only used if the ordering collection is not empty.</summary>
        /// <param name="option">Is the ordering option and should be set to one of FdoOrderingOption_Ascending or FdoOrderingOption_Descending.
        /// FdoOrderingOption_Ascending is the default value.</param> 
        /// <returns>Returns nothing</returns> 
        SDF_API virtual void SetOrderingOption( FdoOrderingOption  option ) {}

        /// <summary>Gets the ordering option.</summary>
        /// <returns>Returns the ordering option.</returns> 
        SDF_API virtual FdoOrderingOption GetOrderingOption( ){ return (FdoOrderingOption)0; }

    private:

        FdoPropertyDefinitionCollection* ProcessComputedIdentifiers(PropertyIndex* pi, 
            FdoClassDefinition* clas, FdoFilter* rdrFilter, recno_list* &features);

    //-------------------------------------------------------
    // Variables
    //-------------------------------------------------------

    private:
        FdoIdentifierCollection* m_properties;

};
#endif

