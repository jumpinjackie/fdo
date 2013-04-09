/*
* Copyright (C) 2010  SL-King d.o.o
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
*/


#ifndef _c_KgKmlDelete_h
#define _c_KgKmlDelete_h

/// \brief
/// The FdoIDelete interface defines the Delete command, which deletes instances of
/// a given class that match the specified criteria. Input to the delete command
/// includes the name of the class, and filter criteria by which to identify the
/// instances to be deleted. The filter may be specified either as text or as an
/// expression tree (most likely produced by a query builder). The delete
/// command can delete instances at global scope or instances/values nested
/// within an object collection property. Instances at global scope are referred
/// to simply by the class name. Instances at a nested scope (i.e. instances
/// within a object collection property) are referred to by the containing class
/// name, followed by a '.', followed by the object collection property name.
class c_KgKmlDelete : public c_KgKmlFdoFeatureCommand<FdoIDelete>
{
public:
  c_KgKmlDelete(c_KgKmlConnection *Conn);
public:
  ~c_KgKmlDelete(void);
  

    
    
public:  
   

    /// \brief
    /// Executes the update command and returns the number of modified 
    /// instances.
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDOKGKML_API virtual FdoInt32 Execute();

    /// \brief
    ///  Updating objects might result in lock conflicts if objects
    /// to be updated are not exclusively locked for the user attempting to
    /// update the object. If objects to be updated are not exclusively locked for the 
    /// user attempting to update the object, a lock conflict report is generated.
    /// The function GetLockConflicts returns a lock conflict reader providing
    /// access to the list of lock conflicts that occurred during the execution
    /// of the update operation.
    /// 
    /// \return
    /// Returns a lock conflict reader.
    /// 
    FDOKGKML_API virtual FdoILockConflictReader* GetLockConflicts();
};

#endif