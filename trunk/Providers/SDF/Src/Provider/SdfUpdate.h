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

class SdfUpdate : public SdfFeatureCommand<FdoIUpdate>
{

    //-------------------------------------------------------
    // Constructor / destructor
    //-------------------------------------------------------

public:
    // constructs an Update command using the specified connection
    SDF_API SdfUpdate(SdfConnection* connection);

protected:
    // default destructor
    SDF_API virtual ~SdfUpdate();


public:
    // Gets the StringDictionary that specifies the names and values of the
    // properties to be updated.
    SDF_API virtual FdoPropertyValueCollection* GetPropertyValues();

    // Executes the update command and returns the number of modified instances.
    SDF_API virtual FdoInt32 Execute();

    // Updating objects might result in lock conflicts if objects to be updated
    // are not exclusively locked for the user attempting to update the object.
    // In this case a lock conflict report is generated.  The function
    // GetLockConflicts returns a lock conflict reader providing access to the
    // list of lock conflicts that occurred during the execution of the update
    // operation.
    SDF_API virtual FdoILockConflictReader* GetLockConflicts();


    //-------------------------------------------------------
    // Variables
    //-------------------------------------------------------

private:
    FdoPropertyValueCollection* m_properties;


};

