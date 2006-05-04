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

#include <Fdo.h>
#include <Fdo/Xml/FeatureSerializer.h>
#include <Fdo/Xml/FeatureWriter.h>
#include <assert.h>

void _writeFeature(FdoString* featureName, FdoIFeatureReader* reader, FdoXmlFeatureWriter* writer,
                   FdoXmlFeatureFlags* flags) {
    FdoPtr<FdoClassDefinition> classDef = reader->GetClassDefinition();
    FdoPtr<FdoPropertyDefinitionCollection> props = classDef->GetProperties();
    FdoPtr<FdoClassDefinition> oldClassDef = writer->GetClassDefinition();
    writer->SetClassDefinition(classDef);

    FdoInt32 count = props->GetCount();
    for (int i = 0; i < count; i++) {
        FdoPtr<FdoPropertyDefinition> prop = props->GetItem(i);
        FdoPropertyType type = prop->GetPropertyType();
        FdoString* propName = prop->GetName();
        if (reader->IsNull(propName))
            continue;
        switch (type) {
            case FdoPropertyType_DataProperty:
                {
                    FdoPtr<FdoDataPropertyDefinition> dataProp = static_cast<FdoDataPropertyDefinition*>(FDO_SAFE_ADDREF(prop.p));
                    FdoDataType dataType = dataProp->GetDataType();
                    FdoPtr<FdoValueExpression> propValue;
                    switch (dataType) {
                        case FdoDataType_Boolean:
                            propValue = FdoBooleanValue::Create(reader->GetBoolean(propName));
                            break;
                        case FdoDataType_Byte:
                            propValue = FdoByteValue::Create(reader->GetByte(propName));
                            break;
                        case FdoDataType_DateTime:
                            propValue = FdoDateTimeValue::Create(reader->GetDateTime(propName));
                            break;
                        case FdoDataType_Decimal:
                            propValue = FdoDecimalValue::Create(reader->GetDouble(propName));
                            break;
                        case FdoDataType_Double:
                            propValue = FdoDoubleValue::Create(reader->GetDouble(propName));
                            break;
                        case FdoDataType_Int16:
                            propValue = FdoInt16Value::Create(reader->GetInt16(propName));
                            break;
                        case FdoDataType_Int32:
                            propValue = FdoInt32Value::Create(reader->GetInt32(propName));
                            break;
                        case FdoDataType_Int64:
                            propValue = FdoInt64Value::Create(reader->GetInt64(propName));
                            break;
                        case FdoDataType_Single:
                            propValue = FdoSingleValue::Create(reader->GetSingle(propName));
                            break;
                        case FdoDataType_String:
                            propValue = FdoStringValue::Create(reader->GetString(propName));
                            break;
                        case FdoDataType_BLOB:
                        case FdoDataType_CLOB:
                            propValue = reader->GetLOB(propName);
                            break;
                        default:
                            assert(false);
                            break;
                    }
                    if (propValue != NULL) {
                        FdoPtr<FdoPropertyValue> nameValue = FdoPropertyValue::Create(propName, propValue);
                        writer->SetProperty(nameValue);
                    }
                }
                break;
            case FdoPropertyType_GeometricProperty:
                {
                    FdoPtr<FdoByteArray> geoValue = reader->GetGeometry(propName);
                    FdoPtr<FdoValueExpression> propValue = FdoGeometryValue::Create(geoValue);
                    FdoPtr<FdoPropertyValue> nameValue = FdoPropertyValue::Create(propName, propValue);
                    writer->SetProperty(nameValue);
                }
                break;
            case FdoPropertyType_ObjectProperty:
                {
                    FdoPtr<FdoIFeatureReader> subReader = reader->GetFeatureObject(propName);
                    FdoPtr<FdoXmlFeatureWriter> subWriter = writer->GetObjectWriter(propName);
                    while (subReader->ReadNext()) {
                        _writeFeature(propName, subReader, subWriter, flags);
                    }
                }
                break;
            case FdoPropertyType_AssociationProperty:
                {
                    FdoPtr<FdoIFeatureReader> subReader = reader->GetFeatureObject(propName);
                    FdoPtr<FdoXmlFeatureWriter> subWriter = writer->GetAssociationWriter(propName);
                    while (subReader->ReadNext()) {
                        _writeFeature(propName, subReader, subWriter, flags);
                    }
                }
                break;
            case FdoPropertyType_RasterProperty:
                {
                    // TODO: Implement it later
                }
                break;
            default:
                assert(false);
                break;
        }
    }

    // Write the current feature to the underlying writer
    // or cache it for sub-writer
    writer->WriteFeature(featureName);

    // restore the old class definition
    writer->SetClassDefinition(oldClassDef);


}

void FdoXmlFeatureSerializer::XmlSerialize( 
	FdoIFeatureReader*              reader, 
	FdoXmlFeatureWriter*            writer,
    FdoXmlFeatureFlags*             flags) {

    FdoPtr<FdoXmlFeaturePropertyWriter> propertyWriter = writer->GetFeaturePropertyWriter();
    FdoPtr<FdoXmlWriter> _writer = propertyWriter->GetXmlWriter();
    FdoString* nsPrefFeatureCollection = L"wfs";
    FdoString* nsPrefFeatureMember = L"gml1";
    if (flags != NULL && flags->GetWriteCollection()) {
        if (wcscmp(flags->GetCollectionUri(), FdoXml::mGmlUri) == 0)
            nsPrefFeatureCollection = FdoXml::mGmlNs;
        FdoStringP tagFeatureCollection = nsPrefFeatureCollection;
        tagFeatureCollection += L":";
        tagFeatureCollection += flags->GetCollectionName();
        _writer->WriteStartElement(tagFeatureCollection);
        // write the namespace and schemaLocation
        // gml namespace
        FdoStringP ns = FdoXml::mXmlnsPref;
        ns += L":";
        ns += FdoXml::mGmlNs;
        _writer->WriteAttribute(ns, FdoXml::mGmlUri); 

        // alias of namespace that includes the element "FeatureCollection", 
        // most likely it's standard wfs namespace
        if (wcscmp(flags->GetCollectionUri(), FdoXml::mGmlUri) != 0) {
            ns = FdoXml::mXmlnsPref;
            ns += L":";
            ns += nsPrefFeatureCollection;
            _writer->WriteAttribute(ns, flags->GetCollectionUri()); 
        }

        // alias of namespace that includes the element "featureMember"
        // most likely it's standard gml namespace
        if (flags->GetWriteMember()) {
            if (wcscmp(flags->GetMemberUri(), flags->GetCollectionUri()) == 0)
                nsPrefFeatureMember = nsPrefFeatureCollection;
            else if (wcscmp(flags->GetMemberUri(), FdoXml::mGmlUri) == 0)
                nsPrefFeatureMember = FdoXml::mGmlNs;
            else {
                ns = FdoXml::mXmlnsPref;
                ns += L":";
                ns += nsPrefFeatureMember;
                _writer->WriteAttribute(ns, flags->GetMemberUri()); 
            }
        }

        // default namespace
        _writer->WriteAttribute(FdoXml::mXmlnsPref, flags->GetDefaultNamespace());

        // Write the attribute "schemaLocation"
        if (flags != NULL) {
            // first define namespace xsi as http://www.w3.org/2001/XMLSchema-instance
            ns = FdoXml::mXmlnsPref;
            ns += L":";
            ns += FdoXml::mXsiNs;
            _writer->WriteAttribute(ns, FdoXml::mXsiUri);

            FdoPtr<FdoStringCollection> namespaces = flags->GetNamespaces();
            FdoStringP schemaLocation;
            FdoInt32 count = namespaces->GetCount();
            for (int i = 0; i < count; i++) {
                FdoString* nameSpace = namespaces->GetString(i);
                FdoString* location = flags->GetSchemaLocation(nameSpace);
                schemaLocation += nameSpace;
                schemaLocation += L" "; // space as delimiter
                schemaLocation += location;
                if (i != count - 1)
                    schemaLocation += L" ";
            }
            if (schemaLocation.GetLength() > 0) {
                ns = FdoXml::mXsiNs;
                ns += L":schemaLocation";
                _writer->WriteAttribute(ns, schemaLocation);
            }
        }
    }

    while (reader->ReadNext()) {
        // start of "featureMember"
        if (flags != NULL && flags->GetWriteMember()) {
            FdoStringP tagFeatureMember = nsPrefFeatureMember;
            tagFeatureMember += L":";
            tagFeatureMember += flags->GetMemberName();
            _writer->WriteStartElement(tagFeatureMember);
        }
        _writeFeature(NULL, reader, writer, flags);
        // end of "gml:featureMember"
        if (flags != NULL && flags->GetWriteMember()) {
            _writer->WriteEndElement();
        }
    }

    if (flags != NULL && flags->GetWriteCollection()) {
        // end of "wfs:FeatureCollection"
        _writer->WriteEndElement();
    }
}







