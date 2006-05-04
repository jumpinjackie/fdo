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

#include "FDO\Schema\mgPropertyValueConstraintType.h"

class FdoPropertyValueConstraint;

BEGIN_NAMESPACE_OSGEO_FDO_SCHEMA

/// \brief
/// FdoPropertyValueConstraint is an abstract class that derives from FdoIDisposable.
/// FdoPropertyValueConstraint is the base class of FdoPropertyValueConstraintRange 
/// and FdoPropertyValueConstraintList.
public __gc class PropertyValueConstraint : public NAMESPACE_OSGEO_RUNTIME::Disposable
{
public private:
	PropertyValueConstraint(System::IntPtr unmanaged, System::Boolean autoDelete);

	inline FdoPropertyValueConstraint* GetImpObj();

protected:
	System::Void Dispose(System::Boolean disposing);

public:
    /// \brief
    /// Abstract operation that must be implemented by derived classes to return the constraint type.
    /// 
    /// \return
    /// Returns the constraint type
    /// 
	__property NAMESPACE_OSGEO_FDO_SCHEMA::PropertyValueConstraintType get_ConstraintType();
};

END_NAMESPACE_OSGEO_FDO_SCHEMA


