// 
//  
//  Copyright (C) 2004-2006  Autodesk, Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//  
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN	// Exclude rarely-used stuff from Windows headers
#include <windows.h>
#endif

#include <Fdo.h>
#include <Fdo/Xml/FeaturePropertyWriter.h>
#include <xercesc/util/Base64.hpp>
#include <xercesc/util/XMLString.hpp>
#include "GeometrySerializer.h"

FdoXmlFeaturePropertyWriter * FdoXmlFeaturePropertyWriter::Create( 
	FdoXmlWriter*               writer,
    FdoXmlFlags*                flags) {
    return new FdoXmlFeaturePropertyWriter(writer, flags);
}

FdoXmlFeaturePropertyWriter::FdoXmlFeaturePropertyWriter(FdoXmlWriter* writer, FdoXmlFlags* flags) :
m_writer(writer), m_flags(flags) {
    FDO_SAFE_ADDREF(writer);
    FDO_SAFE_ADDREF(flags);
}
FdoXmlFeaturePropertyWriter::~FdoXmlFeaturePropertyWriter() {
}

void FdoXmlFeaturePropertyWriter::Dispose() {
    delete this;
}


FdoClassDefinition* FdoXmlFeaturePropertyWriter::GetClassDefinition() {
    return FDO_SAFE_ADDREF(m_classDef.p);
}

FdoXmlWriter* FdoXmlFeaturePropertyWriter::GetXmlWriter() {
    return FDO_SAFE_ADDREF(m_writer.p);
}


void FdoXmlFeaturePropertyWriter::SetClassDefinition(FdoClassDefinition* classDefinition) {
    m_classDef = FDO_SAFE_ADDREF(classDefinition);
}

void FdoXmlFeaturePropertyWriter::WriteFeatureStart(FdoString* startTag) {
    if (startTag == NULL)
        startTag = m_classDef->GetName();
    m_writer->WriteStartElement(startTag);
}

void FdoXmlFeaturePropertyWriter::WriteFeatureEnd() {
    m_writer->WriteEndElement();
}

void FdoXmlFeaturePropertyWriter::WriteProperty( FdoString* name, FdoPropertyValue* value, FdoBoolean valueOnly) {
    FdoPtr<FdoValueExpression> propValue = value->GetValue();
    // Is there a way other than using dynamic_cast to
    // get the exact subclass type here?
    if (dynamic_cast<FdoDataValue*>(propValue.p) != NULL) {
        FdoDataValue* dataValue = static_cast<FdoDataValue*>(propValue.p);
        FdoDataType dataType = dataValue->GetDataType();
        FdoStringP valueBuff;
        switch (dataType) {
            case FdoDataType_DateTime:
                {
                    FdoDateTime value = static_cast<FdoDateTimeValue*>(dataValue)->GetDateTime();
                    if (value.year != -1) {
                        valueBuff = FdoStringP::Format(L"%4d-%02d-%02d", value.year, value.month, value.day);
                    }
                    if (value.hour != -1) {
                        if (valueBuff.GetLength() > 0)
                            valueBuff += L"T";
                        valueBuff += FdoStringP::Format(L"%02d:%02d:", value.hour, value.minute);
                        if (value.seconds == (float)(int)value.seconds)
                            valueBuff += FdoStringP::Format(L"%02d", (int)value.seconds);
                        else
                            valueBuff += FdoStringP::Format(L"%.3f", value.seconds);
                    }
                }
                break;
            case FdoDataType_BLOB:
            case FdoDataType_CLOB:
                WriteProperty(name, static_cast<FdoLOBValue*>(dataValue), valueOnly);
                break;
			case FdoDataType_String:
				valueBuff = static_cast<FdoStringValue*>(dataValue)->GetString();
				break;
            default:
                valueBuff = dataValue->ToString();
                break;
        }
        if (valueBuff.GetLength() > 0)
            WriteProperty(name, valueBuff, valueOnly);

    } else { // geometry property
        FdoPtr<FdoByteArray> geo = static_cast<FdoGeometryValue*>(propValue.p)->GetGeometry();
        FdoInt32 count = geo->GetCount();
        const FdoByte* geoValue = geo->GetData();
        WriteGeometricProperty(name, const_cast<FdoByte*>(geoValue), count, valueOnly);
    }

}

void FdoXmlFeaturePropertyWriter::WriteProperty(FdoString* name, FdoString* value, FdoBoolean valueOnly) {
    if (!valueOnly)
        m_writer->WriteStartElement(name);
    m_writer->WriteCharacters(value);
    if (!valueOnly)
        m_writer->WriteEndElement();
}

void FdoXmlFeaturePropertyWriter::WriteProperty(FdoString* name, FdoIRaster* value, FdoBoolean valueOnly) {

}

void FdoXmlFeaturePropertyWriter::WriteProperty(FdoString* name, FdoLOBValue* value, FdoBoolean valueOnly) {
    if (!valueOnly)
        m_writer->WriteStartElement(name);
    
    FdoPtr<FdoByteArray> v = value->GetData();
    XMLSize_t encodedLen;
    const XMLByte* const inputData = v->GetData();
    const unsigned int   inputLength = v->GetCount();
    XMLByte* encoded = XERCES_CPP_NAMESPACE::Base64::encode(inputData, inputLength, &encodedLen);
    std::string encoded1((char*)encoded, encodedLen);
    m_writer->WriteCharacters((FdoString*)FdoStringP((const char*)encoded1.c_str()));
	delete encoded;

    if (!valueOnly)
        m_writer->WriteEndElement();
}

template<typename T> void Stream2Base64(FdoIStreamReaderTmpl<T>* stream, std::basic_string<T>& b64) {
    T buffer[1024];
    FdoInt32 read = 0;
    do {
        read = stream->ReadNext(buffer, 0, 1024);
        if (read == 0)
            break;
        b64.append(buffer, read);
    } while (true);
}

void FdoXmlFeaturePropertyWriter::WriteProperty(FdoString* name, FdoIStreamReader* lobReader, FdoBoolean valueOnly) {

    if (!valueOnly)
        m_writer->WriteStartElement(name);

    FdoStreamReaderType type = lobReader->GetType();
    const XMLByte* value;
    size_t valueLen;
    std::string v;
    std::wstring v1;
    if (type == FdoStreamReaderType_Byte ||
        type == FdoStreamReaderType_Char) {
        Stream2Base64(static_cast<FdoIStreamReaderTmpl<char>*>(lobReader), v);
        value = (const XMLByte*)v.data();
        valueLen = v.size();
    } else {
        Stream2Base64(static_cast<FdoIStreamReaderTmpl<wchar_t>*>(lobReader), v1);
        value = (const XMLByte*)v1.data();
        valueLen = v1.size() * sizeof(wchar_t);
    }

    XMLSize_t encodedLen;
    XMLByte* encoded = XERCES_CPP_NAMESPACE::Base64::encode(value, (const unsigned int)valueLen, &encodedLen);
    std::string encoded1((char*)encoded, encodedLen);
    m_writer->WriteCharacters((FdoString*)FdoStringP(encoded1.c_str()));
	delete encoded;

    if (!valueOnly)
        m_writer->WriteEndElement();

}

void FdoXmlFeaturePropertyWriter::WriteGeometricProperty(
    FdoString* name, 
    FdoByte* value, 
    FdoInt32 count, 
    FdoBoolean valueOnly) {

    if (!valueOnly)
        m_writer->WriteStartElement(name);

	FdoPtr<FdoFgfGeometryFactory> geoFactory = FdoFgfGeometryFactory::GetInstance();
	FdoPtr<FdoIGeometry> geometry = geoFactory->CreateGeometryFromFgf(value, count);
	FdoGeometrySerializer::SerializeGeometry(geometry, m_writer, L"EPSG:4326");	

    if (!valueOnly)
        m_writer->WriteEndElement();
    
}


void FdoXmlFeaturePropertyWriter::WriteObjectPropertyStart(FdoString* name) {
    m_writer->WriteStartElement(name);
}

void FdoXmlFeaturePropertyWriter::WriteObjectPropertyEnd() {
    m_writer->WriteEndElement();
}






