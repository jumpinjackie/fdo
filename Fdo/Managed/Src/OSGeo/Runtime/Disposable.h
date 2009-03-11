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

/// \ingroup (OSGeoFDORuntime)
/// \brief
///  
///     This class should be used as a base class for the managed classes
///     we define in this project since we need to manage the lifetime of 
///     unmanaged C++ FDO classes. For more information on why this is 
///     necessary see "Implementing Finalize and Dispose to Clean Up 
///     Unmanaged Resources" in the .Net Framework Reference Help
public ref class Disposable abstract : public System::IDisposable, public System::MarshalByRefObject
{
    System::IntPtr m_imp;
    System::Byte m_bAutoDelete;

public:
        property System::IntPtr UnmanagedObject
        {
            System::IntPtr get(void);
        }
        property System::Boolean Disposed
        {
            System::Boolean get(void);
        }
        property System::Boolean AutoDelete
        {
            System::Boolean get(void);
            void set(System::Boolean value);
        }
        property System::Int32 RefCount
        {
            System::Int32 get(void);
        }

protected:
    Disposable();
    Disposable(System::IntPtr unmanagedPointer, System::Boolean autoDelete);
    !Disposable();
    virtual ~Disposable();
    virtual System::Void ReleaseUnmanagedObject();

public:
    System::Void Detach();
    static System::Boolean operator!=(Disposable^ leftObject, Disposable^ rightObject);
    static System::Boolean operator==(Disposable^ leftObject, Disposable^ rightObject);
    System::Void Attach(System::IntPtr unmanagedPointer, System::Boolean autoDelete);
    virtual System::Boolean Equals(System::Object^ obj) override;
    virtual System::Int32 GetHashCode() override;

internal:
	inline FdoIDisposable* GetImpObj();
};

END_NAMESPACE_OSGEO_RUNTIME


