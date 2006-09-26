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
#include "Common\Io\mgIoStream.h"
#include "Common\Io\mgIoTextReader.h"
#include "Common\Xml\mgXmlReader.h"
#include "Common\Xml\mgXmlSaxHandler.h"
#include "Common\Xml\mgXmlSaxContext.h"

using namespace NAMESPACE_OSGEO_COMMON_IO;

NAMESPACE_OSGEO_COMMON_XML::XmlReader::XmlReader( String* fileName) : Disposable(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoXmlReader::Create(StringToUni(fileName)), true))
}

NAMESPACE_OSGEO_COMMON_XML::XmlReader::XmlReader(IoStream* stream) : Disposable(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoXmlReader::Create(stream->GetImpObj()), true))
}

NAMESPACE_OSGEO_COMMON_XML::XmlReader::XmlReader(IoTextReader* reader ) : Disposable(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoXmlReader::Create(reader->GetImpObj()), true))
}

NAMESPACE_OSGEO_COMMON_XML::XmlReader::XmlReader(IntPtr unmanaged, Boolean autoDelete) : Disposable(unmanaged, autoDelete)
{
	
}

FdoXmlReader* NAMESPACE_OSGEO_COMMON_XML::XmlReader::GetImpObj()
{
	return static_cast<FdoXmlReader*>(__super::UnmanagedObject.ToPointer());
}

System::Void NAMESPACE_OSGEO_COMMON_XML::XmlReader::ReleaseUnmanagedObject()
{
	if (get_AutoDelete()) 
        EXCEPTION_HANDLER(GetImpObj()->Release())
	Detach();
}

IoTextReader* NAMESPACE_OSGEO_COMMON_XML::XmlReader::get_TextReader()
{
	IoTextReader* reader;
	EXCEPTION_HANDLER(reader = ObjectFactory::CreateIoTextReader(GetImpObj()->GetTextReader(), true))
	return reader;
}

IoStream* NAMESPACE_OSGEO_COMMON_XML::XmlReader::get_Stream()
{
	IoStream* mgstream;
	EXCEPTION_HANDLER(mgstream= ObjectFactory::CreateIoStream(GetImpObj()->GetStream(), true))
	return mgstream;
}

System::Boolean NAMESPACE_OSGEO_COMMON_XML::XmlReader::Parse()
{
	FdoBoolean rv;
	EXCEPTION_HANDLER(rv = !!GetImpObj()->Parse(NULL, NULL, false))
	return rv;
}

System::Boolean NAMESPACE_OSGEO_COMMON_XML::XmlReader::Parse(NAMESPACE_OSGEO_COMMON_XML::IXmlSaxHandler* saxHandler)
{
	FdoBoolean rv;
	EXCEPTION_HANDLER(rv = !!GetImpObj()->Parse((NULL == saxHandler ? NULL : static_cast<FdoXmlSaxHandler*>(static_cast<NAMESPACE_OSGEO_RUNTIME::Disposable*>(static_cast<System::IDisposable*>(saxHandler))->UnmanagedObject.ToPointer()), NULL, false)))
	return rv;
}

System::Boolean NAMESPACE_OSGEO_COMMON_XML::XmlReader::Parse(NAMESPACE_OSGEO_COMMON_XML::IXmlSaxHandler* saxHandler, NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext* saxContext)
{
	FdoBoolean rv;
	EXCEPTION_HANDLER(rv = !!GetImpObj()->Parse((static_cast<FdoXmlSaxHandler*>(static_cast<NAMESPACE_OSGEO_RUNTIME::Disposable*>(static_cast<System::IDisposable*>(saxHandler))->UnmanagedObject.ToPointer()), saxContext->GetImpObj(), false)))
	return rv;
}

System::Boolean NAMESPACE_OSGEO_COMMON_XML::XmlReader::Parse(NAMESPACE_OSGEO_COMMON_XML::IXmlSaxHandler* saxHandler, NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext* saxContext, System::Boolean incremental)
{
	FdoBoolean rv;
	EXCEPTION_HANDLER(rv = !!GetImpObj()->Parse(static_cast<FdoXmlSaxHandler*>(static_cast<NAMESPACE_OSGEO_RUNTIME::Disposable*>(static_cast<System::IDisposable*>(saxHandler))->UnmanagedObject.ToPointer()), saxContext->GetImpObj(), incremental))
	return rv;
}


Boolean NAMESPACE_OSGEO_COMMON_XML::XmlReader::get_EOD()
{
	FdoBoolean rv;
	EXCEPTION_HANDLER(rv = !!GetImpObj()->GetEOD())
	return rv;
}

System::String* NAMESPACE_OSGEO_COMMON_XML::XmlReader::DecodeName(System::String* name)
{
    FdoStringP result;
    EXCEPTION_HANDLER(result = GetImpObj()->DecodeName(FdoStringP(StringToUni(name))))
    return (FdoString*) result;
}

DictionaryElementCollection* NAMESPACE_OSGEO_COMMON_XML::XmlReader::get_Namespaces()
{
	DictionaryElementCollection* mgD;
	EXCEPTION_HANDLER(mgD = ObjectFactory::CreateDictionaryElementCollection(GetImpObj()->GetNamespaces(), true))
	return mgD;
}

String* NAMESPACE_OSGEO_COMMON_XML::XmlReader::PrefixToUri( String* prefix )
{
	FdoString* uurl;
	EXCEPTION_HANDLER(uurl= GetImpObj()->PrefixToUri(StringToUni(prefix)))
	return uurl;
}

