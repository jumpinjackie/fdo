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
#include "Runtime\Disposable.h"
#include "Common\mgObjectFactory.h"
#include "Common\mgDictionary.h"
#include "Common\mgIStreamReaderImp.h"
#include "Common\mgDictionaryElement.h"
#include "Common\mgStringCollection.h"
#include "Common\mgStringElement.h"
#include "Common\Io\mgIoByteStreamReader.h"
#include "Common\Io\mgIoCharStreamReader.h"
#include "Common\Io\mgIoStream.h"
#include "Common\Io\mgIoFileStream.h"
#include "Common\Io\mgIoMemoryStream.h"
#include "Common\Io\mgIoTextReader.h"
#include "Common\Io\mgIoTextWriter.h"
#include "Common\Io\VirtualFdoIoStream.h"
#include "Common\Xml\mgXmlSaxHandler.h"
#include "Common\Xml\mgXmlSkipElementHandler.h"
#include "Common\Xml\VirtualSaxHandler.h"
#include "Common\Xml\mgXmlAttribute.h"
#include "Common\Xml\mgXmlAttributeCollection.h"
#include "Common\Xml\mgXmlCharDataHandler.h"
#include "Common\Xml\mgXmlCopyHandler.h"
#include "Common\Xml\mgXmlReader.h"
#include "Common\Xml\mgXmlWriter.h"
#include "Common\Xml\mgXmlSaxContext.h"
#include "Common\Xsl\mgXslTransformer.h"

template <class fdoclas, class clas> 
inline NAMESPACE_OSGEO_RUNTIME::Disposable^ CHECK(FdoIDisposable* p, FdoBoolean autoDispose)
{
    return (dynamic_cast<fdoclas *>(p) ?  gcnew clas(IntPtr(p), autoDispose) : nullptr);
}

NAMESPACE_OSGEO_COMMON_XML::XmlReader^ ObjectFactory::CreateXmlReader(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_COMMON_XML::XmlReader(ptr, autoDispose);
}

NAMESPACE_OSGEO_COMMON_XML::XmlWriter^ ObjectFactory::CreateXmlWriter(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_COMMON_XML::XmlWriter(ptr, autoDispose);
}

NAMESPACE_OSGEO_COMMON::DictionaryElementCollection^ ObjectFactory::CreateDictionaryElementCollection(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_COMMON::DictionaryElementCollection(ptr, autoDispose);
}

NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext^ ObjectFactory::CreateXmlSaxContext(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext(ptr, autoDispose);
}

NAMESPACE_OSGEO_COMMON_XML::XmlAttributeCollection^ ObjectFactory::CreateXmlAttributeCollection(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_COMMON_XML::XmlAttributeCollection(ptr, autoDispose);
}

NAMESPACE_OSGEO_COMMON_IO::IoTextWriter^ ObjectFactory::CreateIoTextWriter(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_COMMON_IO::IoTextWriter(ptr, autoDispose);
}

NAMESPACE_OSGEO_COMMON_IO::IoTextReader^ ObjectFactory::CreateIoTextReader(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_COMMON_IO::IoTextReader(ptr, autoDispose);
}

NAMESPACE_OSGEO_COMMON_XML::XmlAttribute^ ObjectFactory::CreateXmlAttribute(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_COMMON_XML::XmlAttribute(ptr, autoDispose);
}

NAMESPACE_OSGEO_COMMON::StringElement^ ObjectFactory::CreateStringElement(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_COMMON::StringElement(ptr, autoDispose);
}

NAMESPACE_OSGEO_COMMON::DictionaryElement^ ObjectFactory::CreateDictionaryElement(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_COMMON::DictionaryElement(ptr, autoDispose);
}

NAMESPACE_OSGEO_COMMON::StringCollection^ ObjectFactory::CreateStringCollection(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_COMMON::StringCollection(ptr, autoDispose);
}

NAMESPACE_OSGEO_COMMON::IStreamReader^ ObjectFactory::CreateIStreamReader(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

	FdoIStreamReader* p = static_cast<FdoIStreamReader*>(ptr.ToPointer());

	switch(p->GetType())
	{
		case FdoStreamReaderType_Byte:
		{
			return gcnew NAMESPACE_OSGEO_COMMON_IO::IoByteStreamReader(IntPtr(p), autoDispose);
		}
		case FdoStreamReaderType_Char:
		{
			return gcnew NAMESPACE_OSGEO_COMMON_IO::IoCharStreamReader(IntPtr(p), autoDispose);
		}
		default:
			return nullptr;
	}
}

NAMESPACE_OSGEO_COMMON_IO::IoStream^ ObjectFactory::CreateIoStream(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

	FdoIDisposable* p = (FdoIDisposable*)ptr.ToPointer();

	NAMESPACE_OSGEO_RUNTIME::Disposable^ wrap;

	// Note:
	// Here we need keep dynamic_cast to decide the real type of "ptr"
	if(wrap = CHECK<FdoIoMemoryStream, NAMESPACE_OSGEO_COMMON_IO::IoMemoryStream>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_COMMON_IO::IoStream^>(wrap);
	if(wrap = CHECK<FdoIoFileStream, NAMESPACE_OSGEO_COMMON_IO::IoFileStream>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_COMMON_IO::IoStream^>(wrap);

	return gcnew NAMESPACE_OSGEO_COMMON_IO::IoStream(ptr, autoDispose);
}

NAMESPACE_OSGEO_COMMON_XML::IXmlSaxHandler^ ObjectFactory::CreateIXmlSaxHandler(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

	FdoIDisposable* p = (FdoIDisposable*)ptr.ToPointer();

	NAMESPACE_OSGEO_RUNTIME::Disposable^ wrap;

	// Note:
	// Here we need keep dynamic_cast to decide the real type of "ptr"
	if(wrap = CHECK<FdoXmlCharDataHandler, NAMESPACE_OSGEO_COMMON_XML::XmlCharDataHandler>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_COMMON_XML::XmlSaxHandler^>(wrap);
	if(wrap = CHECK<FdoXmlCopyHandler, NAMESPACE_OSGEO_COMMON_XML::XmlCopyHandler>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_COMMON_XML::XmlSaxHandler^>(wrap);
	if(wrap = CHECK<FdoXmlSkipElementHandler, NAMESPACE_OSGEO_COMMON_XML::XmlSkipElementHandler>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_COMMON_XML::XmlSaxHandler^>(wrap);

	return gcnew NAMESPACE_OSGEO_COMMON_XML::XmlSaxHandler(ptr, autoDispose);
}
