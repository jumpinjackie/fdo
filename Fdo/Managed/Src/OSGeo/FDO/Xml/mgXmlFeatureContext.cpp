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
#include <Fdo\Xml\FeatureContext.h>
#include <Fdo\Commands\CommandType.h>

#include "FDO\Xml\mgXmlFeatureContext.h"
#include "FDO\mgObjectFactory.h"
#include "FDO\Xml\mgXmlFeaturePropertyReader.h"

NAMESPACE_OSGEO_FDO_XML::XmlFeatureContext::XmlFeatureContext(NAMESPACE_OSGEO_FDO_XML::XmlFeaturePropertyReader^ value) : NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoXmlFeatureContext::Create((value == nullptr ? nullptr : value->GetImpObj()))), true))
}

FdoXmlFeatureContext* NAMESPACE_OSGEO_FDO_XML::XmlFeatureContext::GetImpObj()
{
    return static_cast<FdoXmlFeatureContext*>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_FDO_XML::XmlFeaturePropertyReader^ NAMESPACE_OSGEO_FDO_XML::XmlFeatureContext::GetFeaturePropertyReader()
{
	FdoXmlFeaturePropertyReader* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetFeaturePropertyReader())

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateXmlFeaturePropertyReader(IntPtr(result), true);
}
