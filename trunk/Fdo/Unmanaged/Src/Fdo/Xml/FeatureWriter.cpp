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
#include <Fdo/Xml/FeatureWriter.h>
#include <vector>
#include <StringUtility.h>

//
// definition and implementation for class FdoXmlFeatureWriterCollection
//
class FdoXmlFeatureWriterCollection : public FdoCollection<FdoXmlFeatureWriter, FdoException> {
public:
    static FdoXmlFeatureWriterCollection* Create() {
        return new FdoXmlFeatureWriterCollection();
    }
protected:
    FdoXmlFeatureWriterCollection() {}
    virtual ~FdoXmlFeatureWriterCollection() {}
    virtual void Dispose() {
        delete this;
    }
};

//
// define the sub-writer for object/association properties
//
class FdoXmlFeatureSubWriter : public FdoXmlFeatureWriter {
public:
	static FdoXmlFeatureSubWriter* Create( 
		 FdoXmlFeaturePropertyWriter*    writer,
    	 FdoXmlFeatureFlags*             flags = NULL,
         bool                            association = false);

    virtual void WriteFeature(FdoString* elementTag);

    void SetCaching(bool caching);
    FdoSize GetNumOfCachedFeatures();

protected:
    FdoXmlFeatureSubWriter();
    FdoXmlFeatureSubWriter(FdoXmlFeaturePropertyWriter* writer, FdoXmlFeatureFlags* flags, bool association);
    virtual ~FdoXmlFeatureSubWriter();

private:
    bool        mCaching;
    bool        mAssociation; // if it represents an association property, 
                                // we have to wrap each features with "gml:featureMember"

    FdoPtr<FdoStringCollection> mElementTagCached;
    std::vector<FdoClassDefinition*> mClassDefCached;

    std::vector<FdoPropertyValueCollection*> mPropertyValuesCached;
    
    std::vector<FdoStringCollection*> mObjectPropertyNamesCached;
    std::vector<FdoXmlFeatureWriterCollection*> mObjectPropertyWritersCached;
    
    std::vector<FdoStringCollection*> mAssociationPropertyNamesCached;
    std::vector<FdoXmlFeatureWriterCollection*> mAssociationPropertyWritersCached;

};

FdoXmlFeatureSubWriter* FdoXmlFeatureSubWriter::Create( 
	 FdoXmlFeaturePropertyWriter*    writer,
	 FdoXmlFeatureFlags*             flags,
     bool                            association) 
{
     return new FdoXmlFeatureSubWriter(writer, flags, association);
}

FdoXmlFeatureSubWriter::FdoXmlFeatureSubWriter(FdoXmlFeaturePropertyWriter *writer, FdoXmlFeatureFlags *flags, bool association) :
FdoXmlFeatureWriter(writer, flags), mCaching(true), mAssociation(association) {
    mElementTagCached = FdoStringCollection::Create();
}

FdoXmlFeatureSubWriter::~FdoXmlFeatureSubWriter() {
}

void FdoXmlFeatureSubWriter::SetCaching(bool caching) {
    mCaching = caching;
}

FdoSize FdoXmlFeatureSubWriter::GetNumOfCachedFeatures() {
    return mPropertyValuesCached.size();
}

void FdoXmlFeatureSubWriter::WriteFeature(FdoString* elementTag) {
    if (mCaching) {
        // cache elementTag
        mElementTagCached->Add(elementTag);
        // cache class definition
        mClassDefCached.push_back(FDO_SAFE_ADDREF(mClassDef.p));
        FdoInt32 count, i;
        // cache property values
        FdoPropertyValueCollection* propertyValues = FdoPropertyValueCollection::Create();
        count = mPropertyValues->GetCount();
        for (i = 0; i < count; i++) {
            FdoPtr<FdoPropertyValue> propValue = mPropertyValues->GetItem(i);
            propertyValues->Add(propValue);
        }
        mPropertyValuesCached.push_back(propertyValues);
        // cache object property names
        FdoStringCollection* objectPropertyNames = FdoStringCollection::Create();
        count = mObjectPropertyNames->GetCount();
        for (i = 0; i < count; i++) {
            objectPropertyNames->Add(mObjectPropertyNames->GetString(i));
        }
        mObjectPropertyNamesCached.push_back(objectPropertyNames);
        // cache object property writers
        FdoXmlFeatureWriterCollection* objectPropertyWriters = FdoXmlFeatureWriterCollection::Create();
        count = mObjectPropertyWriters->GetCount();
        for (i = 0; i < count; i++) {
            FdoPtr<FdoXmlFeatureWriter> featureWriter = mObjectPropertyWriters->GetItem(i);
            objectPropertyWriters->Add(featureWriter);
        }
        mObjectPropertyWritersCached.push_back(objectPropertyWriters);

        // cache association property names
        FdoStringCollection* associationPropertyNames = FdoStringCollection::Create();
        count = mAssociationPropertyNames->GetCount();
        for (i = 0; i < count; i++) {
            associationPropertyNames->Add(mAssociationPropertyNames->GetString(i));
        }
        mAssociationPropertyNamesCached.push_back(associationPropertyNames);
        // cache association property writers
        FdoXmlFeatureWriterCollection* associationPropertyWriters = FdoXmlFeatureWriterCollection::Create();
        count = mAssociationPropertyWriters->GetCount();
        for (i = 0; i < count; i++) {
            FdoPtr<FdoXmlFeatureWriter> featureWriter = mAssociationPropertyWriters->GetItem(i);
            associationPropertyWriters->Add(featureWriter);
        }
        mAssociationPropertyWritersCached.push_back(associationPropertyWriters);

    } else {
        // Flush all cached features to the output
        FdoInt32 count = (FdoInt32)mPropertyValuesCached.size();
        for (int i = 0; i < count; i++) {
            if (count > 1 && mAssociation) { // wrap feature with "gml:featureMember"
                FdoPtr<FdoXmlWriter> _writer = mPropertyWriter->GetXmlWriter();
                _writer->WriteStartElement(L"gml:featureMember");
            }
            // for association property, use class name as its element tag
            FdoString* elementTag = NULL;
            if (!mAssociation)
                elementTag = mElementTagCached->GetString(i);
            _writeFeature(elementTag, mClassDefCached[i],
                mPropertyValuesCached[i], mObjectPropertyNamesCached[i],
                mObjectPropertyWritersCached[i], mAssociationPropertyNamesCached[i],
                mAssociationPropertyWritersCached[i]);

            FDO_SAFE_RELEASE(mClassDefCached[i]);
            FDO_SAFE_RELEASE(mPropertyValuesCached[i]);
            FDO_SAFE_RELEASE(mObjectPropertyNamesCached[i]);
            FDO_SAFE_RELEASE(mObjectPropertyWritersCached[i]);
            FDO_SAFE_RELEASE(mAssociationPropertyNamesCached[i]);
            FDO_SAFE_RELEASE(mAssociationPropertyWritersCached[i]);

            if (count > 1 && mAssociation) { // wrap feature with "gml:featureMember"
                FdoPtr<FdoXmlWriter> _writer = mPropertyWriter->GetXmlWriter();
                _writer->WriteEndElement(); // end of "gml:featureMember"
            }
        }
        // clear all cached data
        mElementTagCached->Clear();
        mClassDefCached.clear();
        mPropertyValuesCached.clear();
        mObjectPropertyNamesCached.clear();
        mObjectPropertyWritersCached.clear();
        mAssociationPropertyNamesCached.clear();
        mAssociationPropertyWritersCached.clear();

    }
}

//
// Implemetation for FdoXmlFeatureWriter
//

FdoXmlFeatureWriter::FdoXmlFeatureWriter(FdoXmlFeaturePropertyWriter* writer, FdoXmlFeatureFlags* flags) :
                    mPropertyWriter(writer), mFlags(flags) {
    FDO_SAFE_ADDREF(writer);
    FDO_SAFE_ADDREF(flags);
    
    mPropertyValues = FdoPropertyValueCollection::Create();
    
    mObjectPropertyNames = FdoStringCollection::Create();
    mObjectPropertyWriters = FdoXmlFeatureWriterCollection::Create();

    mAssociationPropertyNames = FdoStringCollection::Create();
    mAssociationPropertyWriters = FdoXmlFeatureWriterCollection::Create();



}

FdoXmlFeatureWriter::~FdoXmlFeatureWriter() {
}

void FdoXmlFeatureWriter::Dispose() {
    delete this;
}

FdoXmlFeatureWriter * FdoXmlFeatureWriter::Create( 
	 FdoXmlFeaturePropertyWriter*    writer,
     FdoXmlFeatureFlags*             flags ) {
    return new FdoXmlFeatureWriter(writer, flags);
}

FdoXmlFeatureWriter* FdoXmlFeatureWriter::Create( 
	 FdoXmlWriter*                   writer,
     FdoXmlFeatureFlags*             flags ) {
    FdoPtr<FdoXmlFeaturePropertyWriter> propertyWriter = FdoXmlFeaturePropertyWriter::Create(writer, flags);
    return new FdoXmlFeatureWriter(propertyWriter, flags);
}

FdoXmlFeaturePropertyWriter* FdoXmlFeatureWriter::GetFeaturePropertyWriter() {
    return FDO_SAFE_ADDREF(mPropertyWriter.p);
}

FdoClassDefinition* FdoXmlFeatureWriter::GetClassDefinition() {
    return FDO_SAFE_ADDREF(mClassDef.p);
}

void FdoXmlFeatureWriter::SetClassDefinition(FdoClassDefinition* classDefinition) {
    mClassDef = FDO_SAFE_ADDREF(classDefinition);
}

void FdoXmlFeatureWriter::SetProperty(FdoPropertyValue* propertyValue) {
    FdoPtr<FdoIdentifier> idName = propertyValue->GetName();
    FdoPtr<FdoPropertyValue> propertyValueStored = mPropertyValues->FindItem(idName->GetName());
    if (propertyValueStored != NULL)
        mPropertyValues->Remove(propertyValueStored);
    mPropertyValues->Add(propertyValue);
}

void FdoXmlFeatureWriter::ClearProperties() {

	mPropertyValues->Clear();
}

FdoXmlFeatureWriter* FdoXmlFeatureWriter::GetObjectWriter(FdoString* propertyName) {
    // first check if it already exists
    FdoInt32 count = mObjectPropertyNames->GetCount();
    FdoInt32 i;
    for (i = 0; i < count; i++) {
        if (wcscmp(mObjectPropertyNames->GetString(i), propertyName) == 0)
            break;
    }
    FdoPtr<FdoXmlFeatureWriter> rv;
    if (i < count) {
        rv = mObjectPropertyWriters->GetItem(i);
    } else {
        // We have to check if there is an object property that matches propertyName
        bool bFound = false;
        FdoPtr<FdoClassDefinition> classDef = mClassDef;
        FdoPtr<FdoClassDefinition> objPropClass;
        while (classDef != NULL) {
            FdoPtr<FdoPropertyDefinitionCollection> props = classDef->GetProperties();
            FdoInt32 count = props->GetCount();
            for (int i = 0; i < count; i++) {
                FdoPtr<FdoPropertyDefinition> prop = props->GetItem(i);
                FdoPropertyType propType = prop->GetPropertyType();
                if (wcscmp(prop->GetName(), propertyName) == 0 && 
                    propType == FdoPropertyType_ObjectProperty) {
                    bFound = true;
                    objPropClass = static_cast<FdoObjectPropertyDefinition*>(prop.p)->GetClass();
                    break;
                }
            }
            if (bFound) 
                break;
            classDef = classDef->GetBaseClass();
        }
        if (bFound) {
            rv = FdoXmlFeatureSubWriter::Create(mPropertyWriter, mFlags);
            rv->SetClassDefinition(objPropClass);
            mObjectPropertyNames->Add(propertyName);
            mObjectPropertyWriters->Add(rv);
        }
    }
    return FDO_SAFE_ADDREF(rv.p);
}

FdoXmlFeatureWriter* FdoXmlFeatureWriter::GetAssociationWriter(FdoString* propertyName) {
    // first check if it already exists
    FdoInt32 count = mAssociationPropertyNames->GetCount();
    FdoInt32 i;
    for (i = 0; i < count; i++) {
        if (wcscmp(mAssociationPropertyNames->GetString(i), propertyName) == 0)
            break;
    }
    FdoPtr<FdoXmlFeatureWriter> rv;
    if (i < count) {
        rv = mAssociationPropertyWriters->GetItem(i);
    } else {
        // We have to check if there is an Association property that matches propertyName
        bool bFound = false;
        FdoPtr<FdoClassDefinition> classDef = mClassDef;
        FdoPtr<FdoClassDefinition> objPropClass;
        while (classDef != NULL) {
            FdoPtr<FdoPropertyDefinitionCollection> props = classDef->GetProperties();
            FdoInt32 count = props->GetCount();
            for (int i = 0; i < count; i++) {
                FdoPtr<FdoPropertyDefinition> prop = props->GetItem(i);
                FdoPropertyType propType = prop->GetPropertyType();
                if (wcscmp(prop->GetName(), propertyName) == 0 && 
                    propType == FdoPropertyType_AssociationProperty) {
                    bFound = true;
                    objPropClass = static_cast<FdoAssociationPropertyDefinition*>(prop.p)->GetAssociatedClass();
                    break;
                }
            }
            if (bFound) 
                break;
            classDef = classDef->GetBaseClass();
        }
        if (bFound) {
            rv = FdoXmlFeatureSubWriter::Create(mPropertyWriter, mFlags, true);
            rv->SetClassDefinition(objPropClass);
            mAssociationPropertyNames->Add(propertyName);
            mAssociationPropertyWriters->Add(rv);
        }
    }
    return FDO_SAFE_ADDREF(rv.p);
}
void FdoXmlFeatureWriter::_writeFeature(FdoString* elementTag, 
                    FdoClassDefinition* classDef,
                    FdoPropertyValueCollection* propertyValues,
                    FdoStringCollection* objectPropertyNames,
                    FdoXmlFeatureWriterCollection* objectPropertyWriters,
                    FdoStringCollection* associationPropertyNames,
                    FdoXmlFeatureWriterCollection* associationPropertyWriters) 
{
    FdoStringP prefixName;
    FdoPtr<FdoStringCollection> idProperties;
    FdoStringP gmlNameRelatePropName;
    FdoStringP gmlDescriptionRelatePropName;
    // Try to get the customized flag information.
    if (mFlags != NULL)
    {
        prefixName = mFlags->GetDefaultNamespacePrefix();
        idProperties = mFlags->GetGmlIDRelatePropertyNames();
        gmlNameRelatePropName = mFlags->GetGmlNameRelatePropertyName();
        gmlDescriptionRelatePropName = mFlags->GetGmlDescriptionRelatePropertyName();
    }

	if (prefixName == NULL) // use schema name as default prefix
	{
		FdoPtr<FdoFeatureSchema>  schema = classDef->GetFeatureSchema();
		if (schema != NULL)
		{
			FdoPtr<FdoXmlWriter> _writer = mPropertyWriter->GetXmlWriter();
			FdoStringP schemaName = _writer->EncodeName(schema->GetName());
			prefixName = schemaName;
		}
	}
	
	if (prefixName != NULL)
		prefixName += L":";

    if (elementTag == NULL)
        mPropertyWriter->WriteFeatureStart(prefixName + classDef->GetName());
	else if (FdoStringUtility::FindCharacter(elementTag,L':') == NULL)
        mPropertyWriter->WriteFeatureStart(prefixName + elementTag);
	else // have prefix already!
		mPropertyWriter->WriteFeatureStart(elementTag);

	mPropertyWriter->SetClassDefinition(classDef);

    FdoInt32 count = propertyValues->GetCount();
    int i;

	//pre-process the gml:id attribute
	if (idProperties != NULL)
	{
		FdoStringP value;
		for (i = 0; i < count; i ++) {
			FdoPtr<FdoPropertyValue> propValue = propertyValues->GetItem(i);
			FdoPtr<FdoIdentifier> name = propValue->GetName();
			if (idProperties->IndexOf(name->GetName())!= -1)
			{
				//use it as part of the gml:id values
				FdoPtr<FdoValueExpression> valueExpression = propValue->GetValue();
				FdoDataValue* dataValue = static_cast<FdoDataValue*>(valueExpression.p);
				FdoStringP temp;
				if (dataValue && !dataValue->IsNull())
				{
					FdoDataType dataType = dataValue->GetDataType();

					if (dataType == FdoDataType_String)
						temp =static_cast<FdoStringValue*>(dataValue)->GetString();
					else
						temp = dataValue->ToString();
				}

				// sperate the values with "\_" to keep its uniqueness as gml:id
				// like: x\_y\_z, no need to add for first value
				// use the value directly if there is only one
				if (value.GetLength() != 0) 
				{
					value += L"_";	
					// scan the character and replace the reserved "_" with "\_"(escape character) to make sure uniqueness
					// note: replace "\" with "\\" first
					temp = temp.Replace(L"\\",L"\\\\").Replace(L"_",L"\\_");
				}
				value += temp;
			}
		}
		if (value != NULL) // set gml:id attribute only when we got values
		{
			FdoStringP qValue = classDef->GetName();
			qValue += L".";
			qValue += value;

			mPropertyWriter->WriteAtribute(L"gml:id",qValue);
		}
	}

    for (i = 0; i < count; i ++) {
        FdoPtr<FdoPropertyValue> propValue = propertyValues->GetItem(i);
        FdoPtr<FdoIdentifier> name = propValue->GetName();
		if (idProperties != NULL)
			if (idProperties->IndexOf(name->GetName())!= -1)
				continue;
		FdoStringP qName = prefixName + name->GetName();
		//handle gml:name/description as special cases
		if (wcscmp(gmlNameRelatePropName,name->GetName()) ==0)
			qName = L"gml:name";
		if (wcscmp(gmlDescriptionRelatePropName,name->GetName()) ==0)
			qName = L"gml:description";
		mPropertyWriter->WriteProperty(qName, propValue);
    }
    // For object properties
    count = objectPropertyWriters->GetCount();
    for (i = 0; i < count; i++) {
        FdoPtr<FdoXmlFeatureWriter> objectWriter = objectPropertyWriters->GetItem(i);
        static_cast<FdoXmlFeatureSubWriter*>(objectWriter.p)->SetCaching(false);
        try {
            objectWriter->WriteFeature(objectPropertyNames->GetString(i));
            static_cast<FdoXmlFeatureSubWriter*>(objectWriter.p)->SetCaching(true);
        }
        catch (...) {
            static_cast<FdoXmlFeatureSubWriter*>(objectWriter.p)->SetCaching(true);
            throw;
        }
    }

    // For association properties
    count = associationPropertyWriters->GetCount();
    for (i = 0; i < count; i++) {
        FdoPtr<FdoXmlFeatureWriter> associationWriter = associationPropertyWriters->GetItem(i);
        static_cast<FdoXmlFeatureSubWriter*>(associationWriter.p)->SetCaching(false);
        try {
            FdoSize numOfCachedFeatures = static_cast<FdoXmlFeatureSubWriter*>(associationWriter.p)->GetNumOfCachedFeatures();
            if (numOfCachedFeatures > 0) {
                FdoPtr<FdoXmlWriter> _writer = mPropertyWriter->GetXmlWriter();
                _writer->WriteStartElement(associationPropertyNames->GetString(i));
                // if there are more than one features to be written
                // we have to wrap all the features within "wfs:FeatureCollection"
                if (numOfCachedFeatures > 1) {
                    _writer->WriteStartElement(L"wfs:FeatureCollection");
                }
                associationWriter->WriteFeature();
                if (numOfCachedFeatures > 1) {
                    _writer->WriteEndElement(); // end of wfs:FeatureCollection
                }
                _writer->WriteEndElement(); // end of the association property
            }
            static_cast<FdoXmlFeatureSubWriter*>(associationWriter.p)->SetCaching(true);
        }
        catch (...) {
            static_cast<FdoXmlFeatureSubWriter*>(associationWriter.p)->SetCaching(true);
            throw;
        }
    }

    mPropertyWriter->WriteFeatureEnd();    
}

void FdoXmlFeatureWriter::WriteFeature(FdoString* elementTag) {
    _writeFeature(elementTag, mClassDef, mPropertyValues, mObjectPropertyNames,
        mObjectPropertyWriters, mAssociationPropertyNames, mAssociationPropertyWriters);
}



