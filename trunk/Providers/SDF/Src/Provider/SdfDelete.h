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

class SdfSimpleFeatureReader;

class SdfDelete : public SdfFeatureCommand<FdoIDelete>
{

    //-------------------------------------------------------
    // Constructor / destructor
    //-------------------------------------------------------

    public:
        // constructs a Delete command using the specified connection
        SdfDelete(SdfConnection* connection);

    protected:
        // default destructor
        virtual ~SdfDelete();


    //-------------------------------------------------------
    // FdoIDelete implementation
    //-------------------------------------------------------

    public:
        // Executes the delete command and returns the number of instances deleted.
        SDF_API virtual FdoInt32 Execute();

        // Deleting objects might result in lock conflicts if objects to be deleted
        // are not exclusively locked for the user attempting to delete the object.
        // In this case a lock conflict report is generated.  The function
        // GetLockConflicts returns a lock conflict reader providing access to the
        // list of lock conflicts that occurred during the execution of the delete
        // operation.
        SDF_API virtual FdoILockConflictReader* GetLockConflicts();
	
	private:

		bool RequiresAssociationHandling( FdoClassDefinition* clas );

		int FollowAssociatedObjects( FdoClassDefinition* clas, SdfSimpleFeatureReader* reader );
};

