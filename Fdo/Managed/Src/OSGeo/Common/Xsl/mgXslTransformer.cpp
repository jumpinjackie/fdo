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
#include "Common\mgException.h"
#include "Common\mgDictionary.h"
#include "Common\mgObjectFactory.h"
#include "Common\Xml\mgXmlWriter.h"
#include "Common\Xml\mgXmlReader.h"
#include "Common\Xsl\mgXslTransformer.h"

NAMESPACE_OSGEO_COMMON_XSL::XslTransformer::XslTransformer() : NAMESPACE_OSGEO_RUNTIME::Disposable(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoXslTransformer::Create()), true))
}

NAMESPACE_OSGEO_COMMON_XSL::XslTransformer::XslTransformer(NAMESPACE_OSGEO_COMMON_XML::XmlReader^ inDoc): NAMESPACE_OSGEO_RUNTIME::Disposable(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoXslTransformer::Create(inDoc->GetImpObj())), true))
}

NAMESPACE_OSGEO_COMMON_XSL::XslTransformer::XslTransformer(NAMESPACE_OSGEO_COMMON_XML::XmlReader^ inDoc, NAMESPACE_OSGEO_COMMON_XML::XmlReader^ stylesheet): NAMESPACE_OSGEO_RUNTIME::Disposable(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoXslTransformer::Create(inDoc->GetImpObj(), stylesheet->GetImpObj())), true))
}

NAMESPACE_OSGEO_COMMON_XSL::XslTransformer::XslTransformer(NAMESPACE_OSGEO_COMMON_XML::XmlReader^ inDoc, NAMESPACE_OSGEO_COMMON_XML::XmlReader^ stylesheet, NAMESPACE_OSGEO_COMMON_XML::XmlWriter^ outDoc): NAMESPACE_OSGEO_RUNTIME::Disposable(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoXslTransformer::Create(inDoc->GetImpObj(), stylesheet->GetImpObj(), outDoc->GetImpObj())), true))
}	

NAMESPACE_OSGEO_COMMON_XSL::XslTransformer::XslTransformer(NAMESPACE_OSGEO_COMMON_XML::XmlReader^ inDoc, NAMESPACE_OSGEO_COMMON_XML::XmlReader^ stylesheet, NAMESPACE_OSGEO_COMMON_XML::XmlWriter^ outDoc, NAMESPACE_OSGEO_COMMON_IO::IoTextWriter^ log): NAMESPACE_OSGEO_RUNTIME::Disposable(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoXslTransformer::Create(inDoc->GetImpObj(), stylesheet->GetImpObj(), outDoc->GetImpObj(), log->GetImpObj())), true))
}

FdoXslTransformer* NAMESPACE_OSGEO_COMMON_XSL::XslTransformer::GetImpObj()
{
	return static_cast<FdoXslTransformer*>(UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_COMMON_XML::XmlReader^ NAMESPACE_OSGEO_COMMON_XSL::XslTransformer::InDoc::get()
{
	FdoXmlReader* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetInDoc())

	return ObjectFactory::CreateXmlReader(IntPtr(result), true);
}

System::Void NAMESPACE_OSGEO_COMMON_XSL::XslTransformer::InDoc::set(NAMESPACE_OSGEO_COMMON_XML::XmlReader^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetInDoc((value == nullptr ? nullptr : value->GetImpObj())))
}

NAMESPACE_OSGEO_COMMON_XML::XmlReader^ NAMESPACE_OSGEO_COMMON_XSL::XslTransformer::Stylesheet::get()
{
	FdoXmlReader* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetStylesheet())

	return ObjectFactory::CreateXmlReader(IntPtr(result), true);
}

System::Void NAMESPACE_OSGEO_COMMON_XSL::XslTransformer::Stylesheet::set(NAMESPACE_OSGEO_COMMON_XML::XmlReader^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetInDoc((value == nullptr ? nullptr : value->GetImpObj())))
}

NAMESPACE_OSGEO_COMMON_XML::XmlWriter^ NAMESPACE_OSGEO_COMMON_XSL::XslTransformer::OutDoc::get()
{
	FdoXmlWriter* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetOutDoc())

	return ObjectFactory::CreateXmlWriter(IntPtr(result), true);
}

System::Void NAMESPACE_OSGEO_COMMON_XSL::XslTransformer::OutDoc::set(NAMESPACE_OSGEO_COMMON_XML::XmlWriter^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetOutDoc((value == nullptr ? nullptr : value->GetImpObj())))
}

NAMESPACE_OSGEO_COMMON_IO::IoTextWriter^ NAMESPACE_OSGEO_COMMON_XSL::XslTransformer::Log::get()
{
	FdoIoTextWriter* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetLog())

    return ObjectFactory::CreateIoTextWriter(IntPtr(result), true);
}

System::Void NAMESPACE_OSGEO_COMMON_XSL::XslTransformer::Log::set(NAMESPACE_OSGEO_COMMON_IO::IoTextWriter^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetLog((value == nullptr ? nullptr : value->GetImpObj())))
}

NAMESPACE_OSGEO_COMMON::DictionaryElementCollection^ NAMESPACE_OSGEO_COMMON_XSL::XslTransformer::Parameters::get()
{
	FdoDictionary* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetParameters())

	return ObjectFactory::CreateDictionaryElementCollection(IntPtr(result), true);
}

System::Void NAMESPACE_OSGEO_COMMON_XSL::XslTransformer::Transform()
{
	EXCEPTION_HANDLER(GetImpObj()->Transform())
}
