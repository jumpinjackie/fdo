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
#include "BinaryWriter.h"
#include "BinaryReader.h"

class PropertyIndex;


//This class implements some common functionality for serializing
//property values for key and data records. It is currently used by DataDb
//and KeyDb.
class DataIO
{
public:

    static void WriteProperty(FdoPropertyDefinition* pd, FdoPropertyValue* pv, SdfBinaryWriter& wrt, bool forAssociation = false );
	static bool WriteAssociationProperty(FdoAssociationPropertyDefinition* pd, FdoPropertyValueCollection* pvc, SdfBinaryWriter& wrt);
    static void WriteProperty(FdoPropertyDefinition* pd, FdoIFeatureReader* reader, SdfBinaryWriter& wrt);
	static void WriteAssociationProperty(FdoAssociationPropertyDefinition* pd, FdoIFeatureReader* reader, SdfBinaryWriter& wrt);
	static void WriteProperty(FdoPropertyDefinition* pd, PropertyIndex* propIndex, SdfBinaryReader* reader, SdfBinaryWriter& wrt);

    static void MakeKey(FdoClassDefinition* fc, PropertyIndex* pi, FdoPropertyValueCollection* pvc, SdfBinaryWriter& wrtkey, REC_NO recno);
    static void MakeKey(FdoClassDefinition* fc, FdoIFeatureReader* reader, SdfBinaryWriter& wrtkey);
	static void MakeKey(FdoClassDefinition* fc, PropertyIndex* propIndex,  SdfBinaryReader* reader, SdfBinaryWriter& wrtkey, REC_NO recno = 0);
    static void UpdateKey(FdoClassDefinition* fc, FdoPropertyValueCollection* pvc, FdoIFeatureReader* reader, SdfBinaryWriter& wrtkey);

    static void MakeDataRecord(FdoClassDefinition* fc, PropertyIndex* pi, FdoPropertyValueCollection* pvc, SdfBinaryWriter& wrtdata);
	static void MakeDataRecord(FdoClassDefinition* fc, PropertyIndex* pi, FdoIFeatureReader* reader, FdoPropertyValueCollection* defaultPvc, SdfBinaryWriter& wrtdata);
    static void MakeDataRecord(PropertyIndex* srcpi, SdfBinaryReader& reader , FdoClassDefinition* destfc, SdfBinaryWriter& wrtdata);
	static void UpdateDataRecord(FdoClassDefinition* fc, PropertyIndex* pi, FdoPropertyValueCollection* pvc, FdoIFeatureReader* reader, SdfBinaryWriter& wrtdata);

    static FdoDataPropertyDefinitionCollection* FindIDProps(FdoClassDefinition* fc);

private:

    
};

