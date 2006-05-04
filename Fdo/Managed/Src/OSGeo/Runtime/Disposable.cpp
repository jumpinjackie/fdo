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

#include "Common\mgException.h"

System::IntPtr NAMESPACE_OSGEO_RUNTIME::Disposable::get_UnmanagedObject()
{
	System::GC::KeepAlive(this); //make fxcop happy
	return m_imp;
}

System::Boolean NAMESPACE_OSGEO_RUNTIME::Disposable::get_Disposed()
{
	System::GC::KeepAlive(this); //make fxcop happy
	return m_imp == System::IntPtr::Zero;
}

System::Boolean NAMESPACE_OSGEO_RUNTIME::Disposable::get_AutoDelete()
{
	return m_bAutoDelete;
}

System::Void NAMESPACE_OSGEO_RUNTIME::Disposable::set_AutoDelete(System::Boolean value)
{
	if (m_bAutoDelete == value)
		return;
	System::IntPtr tmp = m_imp;
	Detach();
	Attach(tmp,value);
	System::GC::KeepAlive(this); //make fxcop happy
}

NAMESPACE_OSGEO_RUNTIME::Disposable::Disposable() : m_bAutoDelete(true)
{
	Detach();
}

NAMESPACE_OSGEO_RUNTIME::Disposable::Disposable(System::IntPtr unmanagedPointer, System::Boolean autoDelete)
{
	if (unmanagedPointer == IntPtr::Zero)
		return;

	m_bAutoDelete = true;
	Attach(unmanagedPointer,autoDelete);
}

NAMESPACE_OSGEO_RUNTIME::Disposable::~Disposable()
{
	Dispose(false);
}

System::Void NAMESPACE_OSGEO_RUNTIME::Disposable::Dispose()
{
	Dispose(true);
	GC::SuppressFinalize(this);
}

System::Void NAMESPACE_OSGEO_RUNTIME::Disposable::Detach()
{
	m_imp = System::IntPtr::Zero;
	if (m_bAutoDelete)
		System::GC::SuppressFinalize(this);
	m_bAutoDelete = false;
	System::GC::KeepAlive(this); //make fxcop happy
}

System::Void NAMESPACE_OSGEO_RUNTIME::Disposable::Attach(System::IntPtr unmanagedPointer, System::Boolean autoDelete)
{
	if (unmanagedPointer == System::IntPtr::Zero || 
		m_imp == unmanagedPointer || 
		m_imp!= System::IntPtr::Zero )
		throw new System::InvalidOperationException();
	m_imp = unmanagedPointer;
	if (m_bAutoDelete && !autoDelete)
		System::GC::SuppressFinalize(this);
	else if (!m_bAutoDelete && autoDelete)
		System::GC::ReRegisterForFinalize(this);
	m_bAutoDelete = autoDelete;
	System::GC::KeepAlive(this); //make fxcop happy
}

System::Boolean NAMESPACE_OSGEO_RUNTIME::Disposable::Equals(System::Object *obj)
{
	Disposable* other = dynamic_cast<Disposable*>(obj);
	if (other==NULL)
		return false;
	System::GC::KeepAlive(this); //make fxcop happy
	return m_imp == other->m_imp;
}

System::Boolean NAMESPACE_OSGEO_RUNTIME::Disposable::op_Inequality(Disposable* leftObject, Disposable* rightObject)
{
    return !Disposable::op_Equality(leftObject, rightObject);
}

System::Boolean NAMESPACE_OSGEO_RUNTIME::Disposable::op_Equality(Disposable* leftObject, Disposable* rightObject)
{
	if (leftObject == NULL)
	{
		if (rightObject == NULL)
			return true;
		return false;
	}
	return leftObject->Equals(rightObject);
}

System::Int32 NAMESPACE_OSGEO_RUNTIME::Disposable::GetHashCode()
{
	System::GC::KeepAlive(this); //make fxcop happy
	return m_imp.ToInt32();
}

System::Void NAMESPACE_OSGEO_RUNTIME::Disposable::Dispose(bool disposing)
{
	if(!Disposed)
	{
		// Release unmanaged resources
	}

	if (disposing)
	{
		// Release managed resources
	}
}

FdoIDisposable* NAMESPACE_OSGEO_RUNTIME::Disposable::GetImpObj()
{
	return static_cast<FdoIDisposable*>(m_imp.ToPointer());
}

System::Int32 NAMESPACE_OSGEO_RUNTIME::Disposable::get_RefCount()
{
	FdoInt32 result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetRefCount())

	return result;
}