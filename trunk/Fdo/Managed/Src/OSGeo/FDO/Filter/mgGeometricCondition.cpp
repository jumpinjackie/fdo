/*
* 
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

#include "stdafx.h"
#include <Fdo.h>
#include <Fdo\Filter\BinaryLogicalOperations.h>

#include "FDO\Filter\mgGeometricCondition.h"
#include "FDO\Expression\mgIdentifier.h"
#include "FDO\mgObjectFactory.h"

NAMESPACE_OSGEO_FDO_FILTER::GeometricCondition::GeometricCondition(System::IntPtr unmanaged, System::Boolean autoDelete) : SearchCondition(unmanaged, autoDelete)
{

}

FdoGeometricCondition* NAMESPACE_OSGEO_FDO_FILTER::GeometricCondition::GetImpObj()
{
	return static_cast<FdoGeometricCondition*>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier* NAMESPACE_OSGEO_FDO_FILTER::GeometricCondition::get_PropertyName()
{
	FdoIdentifier* unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetPropertyName())
	return static_cast<Identifier*>(ObjectFactory::CreateExpression(unobj, true));
}

System::Void NAMESPACE_OSGEO_FDO_FILTER::GeometricCondition::set_PropertyName(NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier* value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetPropertyName((value == NULL ? NULL : value->GetImpObj())))
}
