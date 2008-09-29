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
#include <Fdo\Xml\Context.h>
#include <Fdo\Commands\CommandType.h>

#include "FDO\Xml\mgXmlContext.h"
#include "FDO\mgObjectFactory.h"
#include "FDO\Xml\mgXmlFlags.h"

NAMESPACE_OSGEO_FDO_XML::XmlContext::XmlContext(NAMESPACE_OSGEO_FDO_XML::XmlFlags* flags, NAMESPACE_OSGEO_COMMON_XML::XmlReader* xmlReader) : NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoXmlContext::Create(flags->GetImpObj(), static_cast<FdoXmlReader*>(xmlReader->UnmanagedObject.ToPointer())), true))
}

FdoXmlContext* NAMESPACE_OSGEO_FDO_XML::XmlContext::GetImpObj()
{
    return static_cast<FdoXmlContext*>(__super::UnmanagedObject.ToPointer());
}

System::String* NAMESPACE_OSGEO_FDO_XML::XmlContext::DecodeName (System::String* name)
{
	FdoStringP result;

	EXCEPTION_HANDLER(result = GetImpObj()->DecodeName(FdoStringP(StringToUni(name))))

	return (FdoString*) result;
}

NAMESPACE_OSGEO_FDO_XML::XmlFlags* NAMESPACE_OSGEO_FDO_XML::XmlContext::GetFlags()
{
	FdoXmlFlags* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetFlags())

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateXmlFlags(result, true);
}
