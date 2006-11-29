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
#include "stdafx.h"
#include "DataIO.h"
#include "PropertyIndex.h"


void DataIO::WriteProperty(FdoPropertyDefinition* pd, FdoPropertyValue* pv, BinaryWriter& wrt, bool forAssociation)
{
    FdoDataPropertyDefinition* dpd = NULL;
    FdoPtr<FdoValueExpression> expression;
    
    if (pd->GetPropertyType() == FdoPropertyType_DataProperty)
        dpd = (FdoDataPropertyDefinition*)pd;

    if (pv == NULL)
    {
        //if the property value is NULL, do not write anything to the data record
        //the default value will be used if later the user asks for the value
        //of this porperty
        //TODO: should we write the default property value instead?
        return;
    }

    expression = pv->GetValue();

    if (dpd)
    {

        FdoDataValue* dv = (FdoDataValue*)expression.p;
        if(dv == NULL || dv->IsNull())
            return;

        switch (dpd->GetDataType())
        {
        case FdoDataType_Boolean :
                wrt.WriteByte(((FdoBooleanValue*)(expression.p))->GetBoolean() ? 1 : 0);
                break;

        case FdoDataType_Byte : 
                wrt.WriteByte(((FdoByteValue*)(expression.p))->GetByte());
                break;

        case FdoDataType_DateTime : 
                wrt.WriteDateTime(((FdoDateTimeValue*)(expression.p))->GetDateTime());
                break;

        case FdoDataType_Decimal :
                wrt.WriteDouble(((FdoDecimalValue*)(expression.p))->GetDecimal());
                break;

        case FdoDataType_Double :
                wrt.WriteDouble(((FdoDoubleValue*)(expression.p))->GetDouble());
                break;

        case FdoDataType_Int16 : 
                wrt.WriteInt16(((FdoInt16Value*)(expression.p))->GetInt16());
                break;

        case FdoDataType_Int32 : 
                wrt.WriteInt32(((FdoInt32Value*)(expression.p))->GetInt32());
                break;

        case FdoDataType_Int64 : 
                wrt.WriteInt64(((FdoInt64Value*)(expression.p))->GetInt64());
                break;

        case FdoDataType_Single : 
                wrt.WriteSingle(((FdoSingleValue*)(expression.p))->GetSingle());
                break;

        case FdoDataType_String : 
			if( ! forAssociation )
                wrt.WriteRawString(((FdoStringValue*)(expression.p))->GetString());
			else
				wrt.WriteString(((FdoStringValue*)(expression.p))->GetString());
                break;

        case FdoDataType_BLOB : 
                throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_13_LOBS_NOT_SUPPORTED)));

        case FdoDataType_CLOB : 
                throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_13_LOBS_NOT_SUPPORTED)));

        default: 
                throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_14_UNKNOWN_DATA_TYPE)));
        }
    }
    else
    {
        //we have a geometric property
        FdoPtr<FdoByteArray> byteArray = ((FdoGeometryValue*)(expression.p))->GetGeometry();

        //Note we do not need to write the length of a byte array since we know it
        //by subtracting the offsets into property values in the data record
		if (byteArray)
			wrt.WriteBytes(byteArray->GetData(), byteArray->GetCount());
    }

}

bool DataIO::WriteAssociationProperty(FdoAssociationPropertyDefinition* apd, FdoPropertyValueCollection* pvc, BinaryWriter& wrt)
{
	bool  errorIfSet = apd->GetIsReadOnly();
    bool  oneIdentIsNull = false;
	bool  written = false;

	FdoPtr<FdoDataPropertyDefinitionCollection> idents = apd->GetIdentityProperties();
	if( idents->GetCount() == 0 )
	{
		// Search for property values with names build from the association property name and the
		// associated class identity properties. For example if the associated class identifier is "Id" and the
		// association property name is "AssocProp", then we should search for a property value with
		// name: "AssocProp.Id". If that property value is found and set, then that means an association
		// exists between the new object(we are about to insert) and the object identified by the value
		// of the property value(AssocProp.Id)
		FdoPtr<FdoClassDefinition>cls = apd->GetAssociatedClass();
		idents = cls->GetIdentityProperties();
	}
		
	for(int i=0; i<idents->GetCount(); i++ )
	{
		FdoPtr<FdoDataPropertyDefinition>prop = idents->GetItem( i );
		std::wstring wstr = apd->GetName();
		wstr += L".";
		wstr += prop->GetName();
		FdoPtr<FdoPropertyValue> pv = pvc->FindItem( wstr.c_str() );
        if(pv != NULL)
		{
			if( errorIfSet )
				throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_62_READONLY_ASSOCIATION), apd->GetName() ));

			if( oneIdentIsNull )
				throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_63_ASSOCIATION_MISSING_VALUES)));

			DataIO::WriteProperty(prop, pv, wrt, true);
			written = true;
			oneIdentIsNull = false;
		}
		else 
		{
			if( written ) // we already written one or more identity and this one is null
				throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_63_ASSOCIATION_MISSING_VALUES)));
			oneIdentIsNull = true;
		}
	}
	
	return written;
}

void DataIO::WriteAssociationProperty(FdoAssociationPropertyDefinition* apd, FdoIFeatureReader* reader, BinaryWriter& wrt)
{
	if( apd->GetIsReadOnly() )
		return;

	FdoPtr<FdoDataPropertyDefinitionCollection> idents = apd->GetIdentityProperties();
	if( idents->GetCount() == 0 )
	{
		// Search for property values with names build from the association property name and the
		// associated class identity properties. For example if the associated class identifier is "Id" and the
		// association property name is "AssocProp", then we should search for a property value with
		// name: "AssocProp.Id". If that property value is found and set, then that means an association
		// exists between the new object(we are about to insert) and the object identified by the value
		// of the property value(AssocProp.Id)
		FdoPtr<FdoClassDefinition>cls = apd->GetAssociatedClass();
		idents = cls->GetIdentityProperties();
	}
	if( reader->IsNull( apd->GetName() ) )
		return;

	FdoPtr<FdoIFeatureReader>loc_reader = reader->GetFeatureObject( apd->GetName() );
	if( ! loc_reader->ReadNext() )
		return;
	for(int i=0; i<idents->GetCount(); i++ )
	{
		FdoPtr<FdoDataPropertyDefinition>prop = idents->GetItem( i );
		if( ! loc_reader->IsNull( prop->GetName() ) )
			WriteProperty( prop, loc_reader, wrt );
	}
}

void DataIO::WriteProperty(FdoPropertyDefinition* pd, FdoIFeatureReader* reader, BinaryWriter& wrt)
{
    FdoDataPropertyDefinition* dpd = NULL;
    
    if (pd->GetPropertyType() == FdoPropertyType_DataProperty)
        dpd = (FdoDataPropertyDefinition*)pd;

    FdoString* name = pd->GetName();

    if (reader->IsNull(name))
    {
        //do not write anything if the reader does not have
        //data for this property
        //TODO: should we write the default property value instead?
        return;
    }

    if (dpd)
    {
        switch (dpd->GetDataType())
        {
        case FdoDataType_Boolean : 
                wrt.WriteByte(reader->GetBoolean(name) ? 1 : 0);
                break;

        case FdoDataType_Byte : 
                wrt.WriteByte(reader->GetByte(name));
                break;

        case FdoDataType_DateTime : 
                wrt.WriteDateTime(reader->GetDateTime(name));
                break;

        case FdoDataType_Decimal :
                wrt.WriteDouble(reader->GetDouble(name));
                break;

        case FdoDataType_Double :
                wrt.WriteDouble(reader->GetDouble(name));
                break;

        case FdoDataType_Int16 : 
                wrt.WriteInt16(reader->GetInt16(name));
                break;

        case FdoDataType_Int32 : 
                wrt.WriteInt32(reader->GetInt32(name));
                break;

        case FdoDataType_Int64 : 
                wrt.WriteInt64(reader->GetInt64(name));
                break;

        case FdoDataType_Single : 
                wrt.WriteSingle(reader->GetSingle(name));
                break;

        case FdoDataType_String : 
                wrt.WriteRawString(reader->GetString(name));
                break;

        case FdoDataType_BLOB : 
                throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_13_LOBS_NOT_SUPPORTED)));

        case FdoDataType_CLOB : 
                throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_13_LOBS_NOT_SUPPORTED)));

        default: 
                throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_14_UNKNOWN_DATA_TYPE)));
        }
    }
    else
    {
        
        //we have a geometric property
        FdoPtr<FdoByteArray> byteArray = reader->GetGeometry(name);

        //Note we do not need to write the length of a byte array since we know it
        //by subtracting the offsets into property values in the data record
        wrt.WriteBytes(byteArray->GetData(), byteArray->GetCount());
    }
}

void DataIO::WriteProperty(FdoPropertyDefinition* pd, PropertyIndex* propIndex, BinaryReader* reader, BinaryWriter& wrt)
{
    FdoDataPropertyDefinition* dpd = NULL;
    
    if (pd->GetPropertyType() == FdoPropertyType_DataProperty)
        dpd = (FdoDataPropertyDefinition*)pd;

    PropertyStub* ps = propIndex->GetPropInfo(pd->GetName());
	if( ps == NULL ) // source reader does not have this property
		return;

	reader->SetPosition(sizeof(FCID_STORAGE) + ps->m_recordIndex * sizeof(int));

    int offset = reader->ReadInt32();
    int endoffset = 0;
    if (ps->m_recordIndex < propIndex->GetNumProps() - 1)
        endoffset = reader->ReadInt32();
    else
        endoffset = reader->GetDataLen();

    //skip to the beginning of the data value
    reader->SetPosition(offset);

    int len = endoffset - offset;

	if( dpd == NULL && len != 0 )
	{
		// We have a geometry property
        unsigned char* ptr = reader->GetDataAtCurrentPosition();
		if (ptr)
			wrt.WriteBytes(ptr, len);
	}

    else if ( len > 0 )
    {
        switch (dpd->GetDataType())
        {
        case FdoDataType_Boolean : 
                wrt.WriteByte(reader->ReadByte());
                break;

        case FdoDataType_Byte : 
                wrt.WriteByte(reader->ReadByte());
                break;

        case FdoDataType_DateTime : 
                wrt.WriteDateTime(reader->ReadDateTime());
                break;

        case FdoDataType_Decimal :
                wrt.WriteDouble(reader->ReadDouble());
                break;

        case FdoDataType_Double :
                wrt.WriteDouble(reader->ReadDouble());
                break;

        case FdoDataType_Int16 : 
                wrt.WriteInt16(reader->ReadInt16());
                break;

        case FdoDataType_Int32 : 
                wrt.WriteInt32(reader->ReadInt32());
                break;

        case FdoDataType_Int64 : 
                wrt.WriteInt64(reader->ReadInt64());
                break;

        case FdoDataType_Single : 
                wrt.WriteSingle(reader->ReadSingle());
                break;

        case FdoDataType_String : 

                
            wrt.WriteRawString(reader->ReadRawString(len));
                break;
        }
    }
}

void DataIO::MakeKey(FdoClassDefinition* fc, PropertyIndex* pi, FdoPropertyValueCollection* pvc, BinaryWriter& wrtkey, REC_NO recno)
{
    FdoPtr<FdoDataPropertyDefinitionCollection> idpdc = FindIDProps(fc);

    //find number of properties we will store into the data record
    //we will use this number to save an offset into the data records for each property
    //at the beginning of the data record
    int numIdProps = idpdc->GetCount();

    //reserve space for offsets into id property values in key record
    //nothing if there is just one of them...
    if (numIdProps > 1)
    {
        for (int i=0; i<numIdProps; i++)
            wrtkey.WriteInt32(0);
    }

    //<sigh> Generate key for the property...
    for (int i=0; i<numIdProps; i++)
    {
        FdoPtr<FdoDataPropertyDefinition> dpd = idpdc->GetItem(i);

        //save offset into beginning of the property data
        //TODO: endian
        if (numIdProps > 1)
            ((int*)wrtkey.GetData())[i] = wrtkey.GetPosition();

        //Note that if pi is passed as NULL, the property value collection
        //might contain valid value for the id property, e.g. in the case
        //of optimizing a filter expression 
        if (pi && pi->IsPropAutoGen(dpd->GetName()))
        {
            _ASSERT(recno != 0);
            //for autogenerated property values, use feature record number
            wrtkey.WriteInt32(recno);                        
        }
        else
        {
            FdoPtr<FdoPropertyValue> dpv = pvc->GetItem(dpd->GetName());
            DataIO::WriteProperty(dpd, dpv, wrtkey);
        }
    }
}


void DataIO::MakeKey(FdoClassDefinition* fc, FdoIFeatureReader* reader, BinaryWriter& wrtkey)
{
    FdoPtr<FdoDataPropertyDefinitionCollection> idpdc = FindIDProps(fc);

    //find number of properties we will store into the data record
    //we will use this number to save an offset into the data records for each property
    //at the beginning of the data record
    int numIdProps = idpdc->GetCount();

    //TODO: write feature class ID in key
    //TODO: we do not really need to index things in the key -- it
    //reserve space for offsets into id property values in key record
    //nothing if there is just one of them...
    if (numIdProps > 1)
    {
        for (int i=0; i<numIdProps; i++)
            wrtkey.WriteInt32(0);
    }

    
    //<sigh> Generate key for the property...
    for (int i=0; i<numIdProps; i++)
    {
        FdoPtr<FdoDataPropertyDefinition> dpd = idpdc->GetItem(i);
        
        //TODO: write feature class ID in key
        //TODO: we do not really need to index things in the key -- it
        //will be unique anyway
        //save offset into beginning of the property data
        //TODO: endian
        if (numIdProps > 1)
            ((int*)wrtkey.GetData())[i] = wrtkey.GetPosition();

        DataIO::WriteProperty(dpd, reader, wrtkey);
    }
}

void DataIO::MakeKey(FdoClassDefinition* fc, PropertyIndex* propIndex,  BinaryReader* reader, BinaryWriter& wrtkey, REC_NO recno )
{
    FdoPtr<FdoDataPropertyDefinitionCollection> idpdc = FindIDProps(fc);

    //find number of properties we will store into the data record
    //we will use this number to save an offset into the data records for each property
    //at the beginning of the data record
    int numIdProps = idpdc->GetCount();

    //reserve space for offsets into id property values in key record
    //nothing if there is just one of them...
    if (numIdProps > 1)
    {
        for (int i=0; i<numIdProps; i++)
            wrtkey.WriteInt32(0);
    }

    
    //<sigh> Generate key for the property...
    for (int i=0; i<numIdProps; i++)
    {
        FdoPtr<FdoDataPropertyDefinition> dpd = idpdc->GetItem(i);
        
        //TODO: write feature class ID in key
        //TODO: we do not really need to index things in the key -- it
        //will be unique anyway
        //save offset into beginning of the property data
        //TODO: endian
        if (numIdProps > 1)
            ((int*)wrtkey.GetData())[i] = wrtkey.GetPosition();

		if (propIndex && propIndex->IsPropAutoGen(dpd->GetName()) && recno != 0 )
        {
            wrtkey.WriteInt32(recno);                        
        }
        else
        {
			DataIO::WriteProperty(dpd, propIndex, reader, wrtkey);
		}
    }
}

//serializes a collection of (identity) data property values into a byte array
//This function differs from MakeKey() in that
//it will merge property values specified in the given PropertyValueCollection
//with property values in the given FeatureReader. The PropertyValueCollection
//represents properties to be updated in an old feature record represented by
//the given FeatureReader.
void DataIO::UpdateKey(FdoClassDefinition* fc, FdoPropertyValueCollection* pvc, FdoIFeatureReader* reader, BinaryWriter& wrtkey)
{
    FdoPtr<FdoDataPropertyDefinitionCollection> idpdc = FindIDProps(fc);

    //find number of properties we will store into the data record
    //we will use this number to save an offset into the data records for each property
    //at the beginning of the data record
    int numIdProps = idpdc->GetCount();

    //reserve space for offsets into id property values in key record
    //nothing if there is just one of them...
    if (numIdProps > 1)
    {
        for (int i=0; i<numIdProps; i++)
            wrtkey.WriteInt32(0);
    }

    //<sigh> Generate key for the property...
    for (int i=0; i<numIdProps; i++)
    {
        FdoPtr<FdoDataPropertyDefinition> dpd = idpdc->GetItem(i);

        //save offset into beginning of the property data
        if (numIdProps > 1)
            ((int*)wrtkey.GetData())[i] = wrtkey.GetPosition();

        FdoPtr<FdoPropertyValue> dpv = pvc->FindItem(dpd->GetName());
        if(dpv != NULL)
            DataIO::WriteProperty(dpd, dpv, wrtkey);
        else
            DataIO::WriteProperty(dpd, reader, wrtkey);

    }
}


//serializes a collection of feature properties into a byte array
void DataIO::MakeDataRecord(FdoClassDefinition* fc, PropertyIndex* pi, FdoPropertyValueCollection* pvc, BinaryWriter& wrtdata)
{
    FdoPtr<FdoReadOnlyPropertyDefinitionCollection> bpdc = fc->GetBaseProperties();
    FdoPtr<FdoPropertyDefinitionCollection> pdc = fc->GetProperties();

    //find number of properties we will store into the data record
    //we will use this number to save an offset into the data records for each property
    //at the beginning of the data record
    int numProps = bpdc->GetCount() + pdc->GetCount();

    //write feature class ID
    wrtdata.WriteUInt16((FCID_STORAGE)pi->GetFCID());

    //now generate the data value -- write all property values, except for the ones
    //we already wrote to the key -- we have to check if each one is in the ID prop
    //collection

    //reserve space for offsets into property values in data record
    for (int i=0; i<numProps; i++)
        wrtdata.WriteInt32(0);

    int index = 0;

    //base properties first
    for (int i=0; i<bpdc->GetCount(); i++)
    {
        FdoPtr<FdoPropertyDefinition> pd = (FdoPropertyDefinition*)bpdc->GetItem(i);

        //save offset of property data to the reserved position at the
        //beginning of the record
        //TODO: endian
        ((int*)(wrtdata.GetData() + sizeof(FCID_STORAGE)))[index++] = wrtdata.GetPosition();

        //if property is autogenerated, do not write
        //anything. We use the record number as autogen property value
        //so we can obtain it at read time
        if ( (pi==NULL) || !pi->IsPropAutoGen(pd->GetName()))
        {
            if (pvc)
            {
				if( pd->GetPropertyType() == FdoPropertyType_AssociationProperty )
					DataIO::WriteAssociationProperty((FdoAssociationPropertyDefinition*)pd.p, pvc, wrtdata);
				else
				{
					FdoPtr<FdoPropertyValue> pv = pvc->FindItem(pd->GetName());
					if(pv != NULL)
						DataIO::WriteProperty(pd, pv, wrtdata);
					else
						DataIO::WriteProperty(pd, (FdoPropertyValue*)NULL, wrtdata);
				}
            }
            else
            {
                DataIO::WriteProperty(pd, (FdoPropertyValue*)NULL, wrtdata);
            }
        }

    }

    //class properties
    for (int i=0; i<pdc->GetCount(); i++)
    {
        FdoPtr<FdoPropertyDefinition> pd = (FdoPropertyDefinition*)pdc->GetItem(i);

        //save offset of property data to the reserved position at the
        //beginning of the record
        //TODO: endian
        ((int*)(wrtdata.GetData() + sizeof(FCID_STORAGE)))[index++] = wrtdata.GetPosition();

        //if property is autogenerated, do not write
        //anything. We use the record number as autogen property value
        //so we can obtain it at read time
        if ( (pi == NULL) || !pi->IsPropAutoGen(pd->GetName()))
        {
			if (pvc)
			{
				if( pd->GetPropertyType() == FdoPropertyType_AssociationProperty )
					DataIO::WriteAssociationProperty((FdoAssociationPropertyDefinition*)pd.p, pvc, wrtdata);
				else
				{
					FdoPtr<FdoPropertyValue> pv = pvc->FindItem(pd->GetName());
					if(pv != NULL)
						DataIO::WriteProperty(pd, pv, wrtdata);
					else
						DataIO::WriteProperty(pd, (FdoPropertyValue*)NULL, wrtdata);
				}
			}
            else
            {
                DataIO::WriteProperty(pd, (FdoPropertyValue*)NULL, wrtdata);
            }
        }
    }
}


//serializes a collection of property values into a byte array
//This function differs from MakeDataRecord() in that
//it will merge property values specified in the given PropertyValueCollection
//with property values in the given FeatureReader. The PropertyValueCollection
//represents properties to be updated in an old feature record represented by
//the given FeatureReader.
void DataIO::UpdateDataRecord(FdoClassDefinition* fc, PropertyIndex* pi, FdoPropertyValueCollection* pvc, FdoIFeatureReader* reader, BinaryWriter& wrtdata)
{
    FdoPtr<FdoReadOnlyPropertyDefinitionCollection> bpdc = fc->GetBaseProperties();
    FdoPtr<FdoPropertyDefinitionCollection> pdc = fc->GetProperties();

    //find number of properties we will store into the data record
    //we will use this number to save an offset into the data records for each property
    //at the beginning of the data record
    int numProps = bpdc->GetCount() + pdc->GetCount();// - idpdc->GetCount();

    //now generate the data value -- write all property values, except for the ones
    //we already wrote to the key -- we have to check if each one is in the ID prop
    //collection

    //write feature class ID
    wrtdata.WriteUInt16((FCID_STORAGE)pi->GetFCID());

    //reserve space for offsets into property values in data record
    for (int i=0; i<numProps; i++)
        wrtdata.WriteInt32(0);

    int index = 0;

    //base properties first
    for (int i=0; i<bpdc->GetCount(); i++)
    {
        FdoPtr<FdoPropertyDefinition> pd = (FdoPropertyDefinition*)bpdc->GetItem(i);

        //save offset of property data to the reserved position at the
        //beginning of the record
        //TODO: endian
        ((int*)(wrtdata.GetData() + sizeof(FCID_STORAGE)))[index++] = wrtdata.GetPosition();

        //if property is autogenerated, do not write
        //anything. We use the record number as autogen property value
        //so we can obtain it at read time
        if (!pi->IsPropAutoGen(pd->GetName()))
        {
            if (pvc)
            {
				if( pd->GetPropertyType() == FdoPropertyType_AssociationProperty )
				{
					if( ! DataIO::WriteAssociationProperty((FdoAssociationPropertyDefinition*)pd.p, pvc, wrtdata) )
						DataIO::WriteAssociationProperty((FdoAssociationPropertyDefinition*)pd.p, reader, wrtdata);
				}
				else
				{
					FdoPtr<FdoPropertyValue> pv = (FdoPropertyValue*)pvc->FindItem(pd->GetName());
					if( pv != NULL )
						DataIO::WriteProperty(pd, pv, wrtdata);
					else
						DataIO::WriteProperty(pd, reader, wrtdata);
				}
            }
            else
            {
                DataIO::WriteProperty(pd, reader, wrtdata);
            }
        }
        
    }

    //class properties
    for (int i=0; i<pdc->GetCount(); i++)
    {
        FdoPtr<FdoPropertyDefinition> pd = (FdoPropertyDefinition*)pdc->GetItem(i);

        //save offset of property data to the reserved position at the
        //beginning of the record
        //TODO: endian
        ((int*)(wrtdata.GetData() + sizeof(FCID_STORAGE)))[index++] = wrtdata.GetPosition();
    
       
        //if property is autogenerated, do not write
        //anything. We use the record number as autogen property value
        //so we can obtain it at read time
        if (!pi->IsPropAutoGen(pd->GetName()))
        {
            if (pvc)
            {
				if( pd->GetPropertyType() == FdoPropertyType_AssociationProperty )
				{
					if( ! DataIO::WriteAssociationProperty((FdoAssociationPropertyDefinition*)pd.p, pvc, wrtdata) )
						DataIO::WriteAssociationProperty((FdoAssociationPropertyDefinition*)pd.p, reader, wrtdata);
				}
				else
				{
					
						FdoPtr<FdoPropertyValue> pv((FdoPropertyValue*)pvc->FindItem(pd->GetName()));
						if( pv != NULL )
							DataIO::WriteProperty(pd, pv, wrtdata);
						else //property value did not change -- use old one from reader
							DataIO::WriteProperty(pd, reader, wrtdata);
					
				}
            }
            else
            {
                DataIO::WriteProperty(pd, reader, wrtdata);
            }
        }
    }
}

//go up the class hierarchy and find the base class which has the identity properties
FdoDataPropertyDefinitionCollection* DataIO::FindIDProps(FdoClassDefinition* fc)
{
    FdoPtr <FdoDataPropertyDefinitionCollection> idpdc = fc->GetIdentityProperties();
    FdoPtr<FdoClassDefinition> base = FDO_SAFE_ADDREF(fc);

    //go up class hierarchy to find base class (it has the identity properties)
    while ((base = base->GetBaseClass()) != NULL)
        idpdc = base->GetIdentityProperties();

    if (idpdc->GetCount() == 0)
        throw FdoException::Create(
            NlsMsgGetMain(
                FDO_NLSID(SDFPROVIDER_15_NO_IDENTITY_PROPS),
                (FdoString*) fc->GetQualifiedName()
            )
        );

    return (FDO_SAFE_ADDREF (idpdc.p));
}

void DataIO::MakeDataRecord(FdoClassDefinition* fc, PropertyIndex* pi, FdoIFeatureReader* reader, FdoPropertyValueCollection* defaultPvc, BinaryWriter& wrtdata)
{
    FdoPtr<FdoReadOnlyPropertyDefinitionCollection> bpdc = fc->GetBaseProperties();
    FdoPtr<FdoPropertyDefinitionCollection> pdc = fc->GetProperties();

    //find number of properties we will store into the data record
    //we will use this number to save an offset into the data records for each property
    //at the beginning of the data record
    int numProps = bpdc->GetCount() + pdc->GetCount();

    //write feature class ID
    wrtdata.WriteUInt16((FCID_STORAGE)pi->GetFCID());

    //now generate the data value -- write all property values, except for the ones
    //we already wrote to the key -- we have to check if each one is in the ID prop
    //collection

    //reserve space for offsets into property values in data record
    for (int i=0; i<numProps; i++)
        wrtdata.WriteInt32(0);

    int index = 0;

    //base properties first
    for (int i=0; i<bpdc->GetCount(); i++)
    {
        FdoPtr<FdoPropertyDefinition> pd = (FdoPropertyDefinition*)bpdc->GetItem(i);

        //save offset of property data to the reserved position at the
        //beginning of the record
        //TODO: endian
        ((int*)(wrtdata.GetData() + sizeof(FCID_STORAGE)))[index++] = wrtdata.GetPosition();

        //if property is autogenerated, do not write
        //anything. We use the record number as autogen property value
        //so we can obtain it at read time
        if ( (pi==NULL) || !pi->IsPropAutoGen(pd->GetName()))
        {
			FdoPtr<FdoPropertyValue> pv;
			// May be an extended property
			if( defaultPvc != NULL && defaultPvc->GetCount() != 0 )
				pv = defaultPvc->FindItem( pd->GetName() );
			if( pv != NULL )
				DataIO::WriteProperty(pd, pv, wrtdata);
			else
                DataIO::WriteProperty(pd, reader, wrtdata);
        }
    }

    //class properties
    for (int i=0; i<pdc->GetCount(); i++)
    {
        FdoPtr<FdoPropertyDefinition> pd = (FdoPropertyDefinition*)pdc->GetItem(i);

        //save offset of property data to the reserved position at the
        //beginning of the record
        //TODO: endian
        ((int*)(wrtdata.GetData() + sizeof(FCID_STORAGE)))[index++] = wrtdata.GetPosition();

        //if property is autogenerated, do not write
        //anything. We use the record number as autogen property value
        //so we can obtain it at read time
        if ( (pi == NULL) || !pi->IsPropAutoGen(pd->GetName()))
        {
			FdoPtr<FdoPropertyValue> pv;
			// May be an extended property
			if( defaultPvc != NULL && defaultPvc->GetCount() != 0 )
				pv = defaultPvc->FindItem( pd->GetName() );
			if( pv != NULL )
				DataIO::WriteProperty(pd, pv, wrtdata);
			else
				DataIO::WriteProperty(pd, reader, wrtdata);
        }
    }
}

void DataIO::MakeDataRecord(PropertyIndex* srcpi, BinaryReader& reader , FdoClassDefinition* destfc, BinaryWriter& wrtdata)
{
    FdoPtr<FdoReadOnlyPropertyDefinitionCollection> bpdc = destfc->GetBaseProperties();
    FdoPtr<FdoPropertyDefinitionCollection> pdc = destfc->GetProperties();

    //find number of properties we will store into the data record
    //we will use this number to save an offset into the data records for each property
    //at the beginning of the data record
    int numProps = bpdc->GetCount() + pdc->GetCount();

    //write feature class ID
    wrtdata.WriteUInt16((FCID_STORAGE)srcpi->GetFCID());

    //now generate the data value -- write all property values, except for the ones
    //we already wrote to the key -- we have to check if each one is in the ID prop
    //collection

    //reserve space for offsets into property values in data record
    for (int i=0; i<numProps; i++)
        wrtdata.WriteInt32(0);

    int index = 0;

    //base properties first
    for (int i=0; i<bpdc->GetCount(); i++)
    {
        FdoPtr<FdoPropertyDefinition> pd = (FdoPropertyDefinition*)bpdc->GetItem(i);

        //save offset of property data to the reserved position at the
        //beginning of the record
        ((int*)(wrtdata.GetData() + sizeof(FCID_STORAGE)))[index++] = wrtdata.GetPosition();

        DataIO::WriteProperty(pd, srcpi, &reader, wrtdata);
    }

    //class properties
    for (int i=0; i<pdc->GetCount(); i++)
    {
        FdoPtr<FdoPropertyDefinition> pd = (FdoPropertyDefinition*)pdc->GetItem(i);

        //save offset of property data to the reserved position at the
        //beginning of the record
        ((int*)(wrtdata.GetData() + sizeof(FCID_STORAGE)))[index++] = wrtdata.GetPosition();

        DataIO::WriteProperty(pd, srcpi, &reader, wrtdata);
    }
}
