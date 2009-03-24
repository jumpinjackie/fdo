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
#include "Common\mgObjectFactory.h"
#include "Common\Xml\mgXmlWriter.h"
#include "Common\Io\mgIoStream.h"
#include "Common\Io\mgIoTextWriter.h"

NAMESPACE_OSGEO_COMMON_XML::XmlWriter::XmlWriter(System::String^ fileName, System::Boolean defaultRoot, NAMESPACE_OSGEO_COMMON_XML::XmlWriter::LineFormat lineFormat, System::UInt32 lineLength) : NAMESPACE_OSGEO_RUNTIME::Disposable(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoXmlWriter::Create(StringToUni(fileName), defaultRoot, static_cast<FdoXmlWriter::LineFormat>(lineFormat), lineLength)), true))
}

NAMESPACE_OSGEO_COMMON_XML::XmlWriter::XmlWriter(System::String^ fileName, System::Boolean defaultRoot, NAMESPACE_OSGEO_COMMON_XML::XmlWriter::LineFormat lineFormat) : NAMESPACE_OSGEO_RUNTIME::Disposable(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoXmlWriter::Create(StringToUni(fileName), defaultRoot, static_cast<FdoXmlWriter::LineFormat>(lineFormat))), true))
}

NAMESPACE_OSGEO_COMMON_XML::XmlWriter::XmlWriter(System::String^ fileName, System::Boolean defaultRoot) : NAMESPACE_OSGEO_RUNTIME::Disposable(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoXmlWriter::Create(StringToUni(fileName), defaultRoot)), true))
}

NAMESPACE_OSGEO_COMMON_XML::XmlWriter::XmlWriter(String^ fileName) : NAMESPACE_OSGEO_RUNTIME::Disposable(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoXmlWriter::Create(StringToUni(fileName))), true))
}

NAMESPACE_OSGEO_COMMON_XML::XmlWriter::XmlWriter(NAMESPACE_OSGEO_COMMON_IO::IoStream^ stream, System::Boolean defaultRoot, NAMESPACE_OSGEO_COMMON_XML::XmlWriter::LineFormat lineFormat, System::UInt32 lineLength) : NAMESPACE_OSGEO_RUNTIME::Disposable(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoXmlWriter::Create(stream->GetImpObj(), defaultRoot, static_cast<FdoXmlWriter::LineFormat>(lineFormat), lineLength)), true))
}

NAMESPACE_OSGEO_COMMON_XML::XmlWriter::XmlWriter(NAMESPACE_OSGEO_COMMON_IO::IoStream^ stream, System::Boolean defaultRoot, NAMESPACE_OSGEO_COMMON_XML::XmlWriter::LineFormat lineFormat) : NAMESPACE_OSGEO_RUNTIME::Disposable(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoXmlWriter::Create(stream->GetImpObj(), defaultRoot, static_cast<FdoXmlWriter::LineFormat>(lineFormat))), true))
}

NAMESPACE_OSGEO_COMMON_XML::XmlWriter::XmlWriter(NAMESPACE_OSGEO_COMMON_IO::IoStream^ stream, System::Boolean defaultRoot) : NAMESPACE_OSGEO_RUNTIME::Disposable(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoXmlWriter::Create(stream->GetImpObj(), defaultRoot)), true))
}

NAMESPACE_OSGEO_COMMON_XML::XmlWriter::XmlWriter(NAMESPACE_OSGEO_COMMON_IO::IoStream^ stream) : NAMESPACE_OSGEO_RUNTIME::Disposable(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoXmlWriter::Create(stream->GetImpObj())), true))
}

NAMESPACE_OSGEO_COMMON_XML::XmlWriter::XmlWriter(NAMESPACE_OSGEO_COMMON_IO::IoTextWriter^ writer, System::Boolean defaultRoot, NAMESPACE_OSGEO_COMMON_XML::XmlWriter::LineFormat lineFormat, System::UInt32 lineLength) : NAMESPACE_OSGEO_RUNTIME::Disposable(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoXmlWriter::Create(writer->GetImpObj(), defaultRoot, static_cast<FdoXmlWriter::LineFormat>(lineFormat), lineLength)), true))
}

NAMESPACE_OSGEO_COMMON_XML::XmlWriter::XmlWriter(NAMESPACE_OSGEO_COMMON_IO::IoTextWriter^ writer, System::Boolean defaultRoot, NAMESPACE_OSGEO_COMMON_XML::XmlWriter::LineFormat lineFormat) : NAMESPACE_OSGEO_RUNTIME::Disposable(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoXmlWriter::Create(writer->GetImpObj(), defaultRoot, static_cast<FdoXmlWriter::LineFormat>(lineFormat))), true))
}

NAMESPACE_OSGEO_COMMON_XML::XmlWriter::XmlWriter(NAMESPACE_OSGEO_COMMON_IO::IoTextWriter^ writer, System::Boolean defaultRoot) : NAMESPACE_OSGEO_RUNTIME::Disposable(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoXmlWriter::Create(writer->GetImpObj(), defaultRoot)), true))
}

NAMESPACE_OSGEO_COMMON_XML::XmlWriter::XmlWriter(NAMESPACE_OSGEO_COMMON_IO::IoTextWriter^ writer) : NAMESPACE_OSGEO_RUNTIME::Disposable(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoXmlWriter::Create(writer->GetImpObj())), true))
}

NAMESPACE_OSGEO_COMMON_XML::XmlWriter::XmlWriter(System::IntPtr unmanaged, System::Boolean autoDelete)  : NAMESPACE_OSGEO_RUNTIME::Disposable(unmanaged, autoDelete)
{

}

FdoXmlWriter* NAMESPACE_OSGEO_COMMON_XML::XmlWriter::GetImpObj()
{
	return static_cast<FdoXmlWriter*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_COMMON_XML::XmlWriter::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

NAMESPACE_OSGEO_COMMON_IO::IoTextWriter^ NAMESPACE_OSGEO_COMMON_XML::XmlWriter::TextWriter::get()
{
	NAMESPACE_OSGEO_COMMON_IO::IoTextWriter^ writer;
    EXCEPTION_HANDLER(writer = ObjectFactory::CreateIoTextWriter(IntPtr(GetImpObj()->GetTextWriter()), true))
	return writer;
}

NAMESPACE_OSGEO_COMMON_IO::IoStream^ NAMESPACE_OSGEO_COMMON_XML::XmlWriter::Stream::get()
{
	NAMESPACE_OSGEO_COMMON_IO::IoStream^ iostream;
	EXCEPTION_HANDLER(iostream = ObjectFactory::CreateIoStream(IntPtr(GetImpObj()->GetStream()), true))
	return iostream;
}

Boolean NAMESPACE_OSGEO_COMMON_XML::XmlWriter::DefaultRoot::get()
{
	Boolean rv;
	EXCEPTION_HANDLER(rv = !!GetImpObj()->GetDefaultRoot())
	return rv;
}

System::Void NAMESPACE_OSGEO_COMMON_XML::XmlWriter::DefaultRoot::set(Boolean defaultRoot )
{
	EXCEPTION_HANDLER(GetImpObj()->SetDefaultRoot(defaultRoot))
}

System::Void NAMESPACE_OSGEO_COMMON_XML::XmlWriter::Close()
{
	EXCEPTION_HANDLER(GetImpObj()->Close())
}

System::Void NAMESPACE_OSGEO_COMMON_XML::XmlWriter::WriteStartElement(String^ elementName )
{
	EXCEPTION_HANDLER(GetImpObj()->WriteStartElement( StringToUni(elementName)))
}

System::Void NAMESPACE_OSGEO_COMMON_XML::XmlWriter::WriteEndElement()
{
	EXCEPTION_HANDLER(GetImpObj()->WriteEndElement())
}

System::Void NAMESPACE_OSGEO_COMMON_XML::XmlWriter::WriteAttribute( String^ attributeName, String^ attributeValue )
{
	EXCEPTION_HANDLER(GetImpObj()->WriteAttribute( StringToUni(attributeName),  StringToUni(attributeValue)))
}

System::Void NAMESPACE_OSGEO_COMMON_XML::XmlWriter::WriteCharacters( String^ characters )
{
	EXCEPTION_HANDLER(GetImpObj()->WriteCharacters( StringToUni(characters)))
}

System::Void NAMESPACE_OSGEO_COMMON_XML::XmlWriter::WriteBytes( array<System::Byte>^ bytes,  System::UInt32 count )
{
	pin_ptr<FdoByte> unbyte = &bytes[0];
	EXCEPTION_HANDLER(GetImpObj()->WriteBytes(unbyte, count))
}

System::String^ NAMESPACE_OSGEO_COMMON_XML::XmlWriter::EncodeName(System::String^ name)
{
    FdoStringP result;
    EXCEPTION_HANDLER(result = GetImpObj()->EncodeName(FdoStringP(StringToUni(name))))
    return CHECK_STRING((FdoString*)result);
}

System::Boolean NAMESPACE_OSGEO_COMMON_XML::XmlWriter::IsValidName(System::String^ name)
{
	Boolean rv;
	EXCEPTION_HANDLER(rv = !!GetImpObj()->IsValidName(FdoStringP(StringToUni(name))))
	return rv;
}

System::String^ NAMESPACE_OSGEO_COMMON_XML::XmlWriter::UriToQName( System::String^ location, System::String^ localName, System::Boolean isElement)
{
	FdoStringP str;
	EXCEPTION_HANDLER(str = GetImpObj()->UriToQName(StringToUni(location), StringToUni(localName), isElement))
	return CHECK_STRING((FdoString*)str);
}

System::String^ NAMESPACE_OSGEO_COMMON_XML::XmlWriter::UriToQName( System::String^ location, System::String^ localName)
{
	FdoStringP str;
	EXCEPTION_HANDLER(str = GetImpObj()->UriToQName(StringToUni(location), StringToUni(localName), true))
	return CHECK_STRING((FdoString*)str);
}
