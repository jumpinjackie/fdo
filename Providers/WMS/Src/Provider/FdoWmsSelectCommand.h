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

#ifndef FDOWMSSELECTCOMMAND_H
#define FDOWMSSELECTCOMMAND_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "FdoWmsFeatureCommand.h"
#include "FdoWmsDelegate.h"
#include "WMS/Override/FdoWmsOvFormatType.h"

class FdoWmsSelectCommand : public FdoWmsFeatureCommand<FdoISelect>
{
    friend class FdoWmsConnection;

protected:
    // <summary>Constructs an instance of a command for the given connection. </summary>
    // <returns>Returns nothing</returns> 
    FdoWmsSelectCommand (FdoWmsConnection* connection);
    
    // <summary>Virtual Destructor.</summary>
    // <returns>Returns nothing</returns> 
    virtual ~FdoWmsSelectCommand (void);
    
    // <summary>Dispose this object.</summary>
    // <returns>Returns nothing</returns> 
    virtual void Dispose ();

public:
    //
    // Prevent the use of the Assignment Operation by defining it and not implemeting it.
    // DO NOT IMPLEMENT
    FdoWmsSelectCommand & operator= (const FdoWmsSelectCommand &right);

    /// <summary>Gets the FdoIdentifierCollection that holds the list of property names to 
    /// return with the result. If empty all properties of the specified class
    /// are returned.</summary>
    /// <returns>Returns the list of property names.</returns> 
    virtual FdoIdentifierCollection* GetPropertyNames ();

    /// <summary>Gets the FdoLockType value (see "Locking Commands").</summary>
    /// <returns>Returns the lock type.</returns> 
    virtual FdoLockType GetLockType ();

    /// <summary>Sets the FdoLockType value (see "Locking Commands").</summary>
    /// <param name="value">Input the lock type.</param> 
    /// <returns>Returns nothing</returns> 
    virtual void SetLockType (FdoLockType value);

    /// <summary>Gets the FdoLockStrategy value (see "Locking Commands").</summary>
    /// <returns>Returns the lock strategy.</returns> 
    virtual FdoLockStrategy GetLockStrategy ();

    /// <summary>Sets the FdoLockStrategy value (see "Locking Commands").</summary>
    /// <param name="value">Input the lock strategy.</param> 
    /// <returns>Returns nothing</returns> 
    virtual void SetLockStrategy (FdoLockStrategy value);

    /// <summary>Executes the select command and returns a reference to an FdoIFeatureReader.</summary>
    /// <returns>Returns the feature reader.</returns> 
    virtual FdoIFeatureReader* Execute ();

    /// <summary>Executes the select command and returns a reference to an
    /// IFeatureReader.</summary> 
    /// <returns>Returns the feature reader.</returns> 
    virtual FdoIFeatureReader* ExecuteWithLock ();

    /// <summary> When executing the operation ExecuteWithLock lock 
    /// conflicts may occur. Those lock conflicts are reported. The 
    /// function GetLockConflicts returns a lock conflict reader 
    /// providing access to the list of lock conflicts that occurred 
    /// during the execution of the operation.</summary>
    /// <returns>Returns a lock conflict reader.</returns> 
    virtual FdoILockConflictReader* GetLockConflicts ();

    /// <summary>Gets the FdoIdentifierCollection that holds the list of order by property names. If empty no ordering is used. This list is initially
    /// empty and the caller need to add the property that the command should use as a order by criteria.</summary>
    /// <returns>Returns the list of group by property names.</returns> 
    virtual FdoIdentifierCollection* GetOrdering(){ return NULL; }

    /// <summary>Set the ordering option of the selection. This is only used if the ordering collection is not empty.</summary>
    /// <param name="option">Is the ordering option and should be set to one of FdoOrderingOption_Ascending or FdoOrderingOption_Descending.
    /// FdoOrderingOption_Ascending is the default value.</param> 
    /// <returns>Returns nothing</returns> 
    virtual void SetOrderingOption( FdoOrderingOption  option ) {}

    /// <summary>Gets the ordering option.</summary>
    /// <returns>Returns the ordering option.</returns> 
    virtual FdoOrderingOption GetOrderingOption( ){ return (FdoOrderingOption)0; }

private:
	// Get the pruned class definition. If the user does
	// not select any proerties, the whole class will be 
	// returned; otherwise only the selected proerties will
	// be included in the class definition.
	FdoClassDefinition* _getPrunedClassDefinition ();

	// associate the spatial context to the raster property of the feature class
	void _setSpatialContextAssociation (FdoFeatureClass* featClass, FdoString* spatialContext);
	FdoStringP _getSpatialContextAssociation (FdoFeatureClass* featClass);

	// Get the default image height which is specified by the user in the connection string.
	// if it's not set, use 600 pixels arbitarily as default height value.
	FdoSize _getDefaultImageHeight ();
	
	// create a WMS delegate to get the map
	FdoWmsDelegate* _getWmsDelegate ();

    // Analyze the properties to check whether the RESAMPLE or CLIP function is invoked.
	void _analysisIdentifier (FdoString* srsName, FdoWmsBoundingBox* bbox, FdoStringP& propertyName, FdoSize& height, FdoSize& width);

    // Some layers may contain characters ":" or "." which are not allowed in FDO class name.
    // When the layer is mapped to an FDO class, it's name will be mangled: the invalid characters
    // will be filled with blank. The "mapping" between original and mangled names are kept in a
    // dictionary data structure.
    // This method is used to get the original name of the layer.
    FdoString* _getOriginalLayerName (FdoString* mangledLayerName);

	// Lock support
    FdoLockType mType;
    FdoLockStrategy mStrategy;

	// The properties to be selected
    FdoPtr<FdoIdentifierCollection> mPropertiesToSelect;
};

#endif // FDOWMSSELECTCOMMAND_H
