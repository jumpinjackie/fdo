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

#pragma once

class FdoIDisposable;

BEGIN_NAMESPACE_OSGEO_RUNTIME

/// \brief
///  
///     This class should be used as a base class for the managed classes
///     we define in this project since we need to manage the lifetime of 
///     unmanaged C++ FDO classes. For more information on why this is 
///     necessary see "Implementing Finalize and Dispose to Clean Up 
///     Unmanaged Resources" in the .Net Framework Reference Help
public __gc __abstract class Disposable : public System::IDisposable, public System::MarshalByRefObject
{
    System::IntPtr m_imp;
    System::Boolean m_bAutoDelete;

public:
    __property System::IntPtr get_UnmanagedObject();
    __property System::Boolean get_Disposed();
    __property System::Boolean get_AutoDelete();
    __property System::Void set_AutoDelete(System::Boolean value);
	__property System::Int32 get_RefCount();

protected:
    Disposable();

    //[System::Security::Permissions::SecurityPermissionAttribute(System::Security::Permissions::SecurityAction::LinkDemand , Flags = System::Security::Permissions::SecurityPermissionFlag::UnmanagedCode)]
    Disposable(System::IntPtr unmanagedPointer, System::Boolean autoDelete);
    virtual ~Disposable();

public:
    System::Void Dispose();

public:
    System::Void Detach();
    System::Void Attach(System::IntPtr unmanagedPointer, System::Boolean autoDelete);
    virtual System::Boolean Equals(System::Object* obj);
    static System::Boolean op_Inequality(Disposable* leftObject, Disposable* rightObject);
    static System::Boolean op_Equality(Disposable* leftObject, Disposable* rightObject);
    System::Int32 GetHashCode();

public private:
	inline FdoIDisposable* GetImpObj();

protected:
    virtual System::Void Dispose(System::Boolean disposing);
};

END_NAMESPACE_OSGEO_RUNTIME


