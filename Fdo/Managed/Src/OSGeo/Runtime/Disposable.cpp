// Copyright (C) 2004-2006  Autodesk, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

#include "stdafx.h"
#include "Disposable.h"
#include <stdio.h>

#include "Common\mgException.h"

System::IntPtr NAMESPACE_OSGEO_RUNTIME::Disposable::UnmanagedObject::get()
{
	return m_imp;
}

System::Boolean NAMESPACE_OSGEO_RUNTIME::Disposable::Disposed::get()
{
	return m_imp == System::IntPtr::Zero;
}

System::Boolean NAMESPACE_OSGEO_RUNTIME::Disposable::AutoDelete::get()
{
	return (m_bAutoDelete != 0);
}

System::Void NAMESPACE_OSGEO_RUNTIME::Disposable::AutoDelete::set(System::Boolean value)
{
	m_bAutoDelete = value;
		return;
}

NAMESPACE_OSGEO_RUNTIME::Disposable::Disposable()
{
    m_bAutoDelete = 0;
    m_imp = IntPtr::Zero;
}

NAMESPACE_OSGEO_RUNTIME::Disposable::Disposable(System::IntPtr unmanagedPointer, System::Boolean autoDelete)
{
    m_bAutoDelete = 0;
    m_imp = IntPtr::Zero;
	
    if (unmanagedPointer == IntPtr::Zero)
		return;
    m_imp = unmanagedPointer;
    m_bAutoDelete = (autoDelete ? 1 : 0);
}

NAMESPACE_OSGEO_RUNTIME::Disposable::!Disposable()
{
    ReleaseUnmanagedObject();
}

NAMESPACE_OSGEO_RUNTIME::Disposable::~Disposable()
{
	this->!Disposable();
}

System::Void NAMESPACE_OSGEO_RUNTIME::Disposable::ReleaseUnmanagedObject()
{
	if (m_bAutoDelete && m_imp != IntPtr::Zero)
    	EXCEPTION_HANDLER(GetImpObj()->Release())
    
    m_bAutoDelete = false;
    m_imp = IntPtr::Zero;
}

System::Void NAMESPACE_OSGEO_RUNTIME::Disposable::Detach()
{
	m_imp = System::IntPtr::Zero;
	m_bAutoDelete = false;
}

System::Void NAMESPACE_OSGEO_RUNTIME::Disposable::Attach(System::IntPtr unmanagedPointer, System::Boolean autoDelete)
{
	if (unmanagedPointer == System::IntPtr::Zero || 
		m_imp == unmanagedPointer || 
		m_imp!= System::IntPtr::Zero )
		throw gcnew System::InvalidOperationException();

    ReleaseUnmanagedObject();
	
    m_imp = unmanagedPointer;
	m_bAutoDelete = autoDelete;
}

System::Boolean NAMESPACE_OSGEO_RUNTIME::Disposable::Equals(System::Object^ obj)
{
	Disposable^ other = dynamic_cast<Disposable^>(obj);
	if ((System::Object^)other == nullptr)
		return false;
	return m_imp == other->m_imp;
}

System::Boolean NAMESPACE_OSGEO_RUNTIME::Disposable::operator!=(Disposable^ leftObject, Disposable^ rightObject)
{
    return !Disposable::operator==(leftObject, rightObject);
}

System::Boolean NAMESPACE_OSGEO_RUNTIME::Disposable::operator==(Disposable^ leftObject, Disposable^ rightObject)
{
    if ((System::Object^)leftObject == nullptr)
	{
		if ((System::Object^)rightObject == nullptr)
			return true;
		return false;
	}
	return leftObject->Equals(rightObject);
}

System::Int32 NAMESPACE_OSGEO_RUNTIME::Disposable::GetHashCode()
{
	return m_imp.ToInt32();
}

IntPtr NAMESPACE_OSGEO_RUNTIME::Disposable::GetDisposableObject()
{
	return m_imp;
}

FdoIDisposable* NAMESPACE_OSGEO_RUNTIME::Disposable::GetImpObj()
{
    return static_cast<FdoIDisposable*>(GetDisposableObject().ToPointer());
}

System::Int32 NAMESPACE_OSGEO_RUNTIME::Disposable::RefCount::get()
{
	System::Int32 result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetRefCount())

	return result;
}
