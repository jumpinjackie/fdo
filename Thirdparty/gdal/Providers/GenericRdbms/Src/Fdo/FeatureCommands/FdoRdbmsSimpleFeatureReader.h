#ifndef FDORDBMSSIMPLEFEATUREREADER_H
#define FDORDBMSSIMPLEFEATUREREADER_H  1
/*
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
 */

#include "FdoRdbms.h"
#include "FdoRdbmsUtil.h"
#include "../../Gdbi/GdbiQueryResult.h"
#include <Sm/Lp/ObjectPropertyDefinition.h>
#include <Sm/Lp/AssociationPropertyDefinition.h>
#include "../../Gdbi/GdbiStatement.h"
#include <Sm/Lp/PropertyMappingConcrete.h>
#include <Sm/Lp/ObjectPropertyClass.h>
#include "../Filter/FdoRdbmsFilterProcessor.h"
#include "../Fdo/Other/FdoRdbmsSQLDataReader.h"

class FdoRdbmsConnection;
class ISchemaHandler;
class DbiConnection;

typedef struct
{
    GdbiColumnDesc col;
    int   idxStmt;
} GdbiColumnDetail;

typedef std::vector<GdbiColumnDetail*> LinkColumnList;
typedef std::map<FdoString*, std::pair<GdbiColumnDetail*, int>, wstring_less> GdbiColumnDetailToIdx;

class FdoRdbmsSimpleFeatureReader : public FdoIFeatureReader
{
protected:
    FdoRdbmsSimpleFeatureReader()
    {
    }

    FdoRdbmsSimpleFeatureReader( FdoIConnection *connection, GdbiQueryResult *queryResult, bool isFeatureQuery, 
        const FdoSmLpClassDefinition *classDef, FdoFeatureSchemaCollection *schmCol, 
        FdoIdentifierCollection *properties = NULL);

    FdoRdbmsSimpleFeatureReader(const FdoRdbmsSimpleFeatureReader &right);
    FdoRdbmsSimpleFeatureReader & operator=(const FdoRdbmsSimpleFeatureReader &right);

    inline void ValidateIndex(int index)
    {    
        if (index < 0 || index >= mColCount)
            throw FdoCommandException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_73_PROPERTY_INDEXOUTOFBOUNDS), index));
    }

    int NameToIndex(FdoString* name);

    const void* GetGeometry(FdoInt32 index, FdoInt32* len, bool noExcOnInvalid = false);
    FdoClassDefinition* FilterClassDefinition(FdoClassDefinition* classDef);
    void GenerateInternalMapping();
    FdoPropertyDefinition* GetPropertyFromComputed(FdoClassDefinition* classDef, GdbiColumnDesc* colDesc, FdoComputedIdentifier* cidf);
    FdoPropertyDefinition* GetClonePropertyByName(FdoClassDefinition* classDef, FdoPropertyDefinitionCollection* pcoll, GdbiColumnDesc* colDesc, FdoIdentifier* idf = NULL);
    void AddToPkIfNeeded(FdoDataPropertyDefinitionCollection* idProps, FdoDataPropertyDefinition* dp, FdoClassDefinition* classDef);

protected:
    virtual ~FdoRdbmsSimpleFeatureReader();
    virtual void Dispose();

public:
    static FdoRdbmsSimpleFeatureReader* Create(FdoIConnection *connection, GdbiQueryResult *queryResult, bool isFeatureQuery, 
        const FdoSmLpClassDefinition *classDef, FdoFeatureSchemaCollection *schmCol, FdoIdentifierCollection *properties = NULL);

    virtual FdoClassDefinition* GetClassDefinition();

    virtual FdoInt32 GetDepth();

    virtual FdoString* GetPropertyName(FdoInt32 index);

    virtual FdoInt32 GetPropertyIndex(FdoString* propertyName);

    /// <summary>Gets the FDO property type of the column with the specified name. This is used
    //  to indicate if a given column is a geometric property or a data property. If the column is
    //  a FdoPropertyType_Data, then GetColumnType can be used to find the data type of the column.</summary>
    /// <param name="propertyName">Input the column name.</param>
    /// <returns>Returns the FDO property type of the column.</returns>
    virtual FdoPropertyType GetPropertyType(FdoString* propertyName);
    virtual FdoPropertyType GetPropertyType(FdoInt32 index);

    // Gets the Boolean value of the specified column. No conversion is
    // performed, therefore the column must be of type Boolean or an
    // exception is thrown.
    virtual bool GetBoolean(FdoString* propertyName);
    virtual FdoBoolean GetBoolean(FdoInt32 index);

    // Gets the Byte value of the specified column. No conversion is
    // performed, therefore the column must be of type Byte or an
    // exception is thrown.
    virtual FdoByte GetByte(FdoString* propertyName);
    virtual FdoByte GetByte(FdoInt32 index);

    // Gets the DateTime value of the specified column. No conversion
    // is performed, therefore the column must be of type DateTime or
    // an exception is thrown.
    virtual FdoDateTime GetDateTime(FdoString* propertyName);
    virtual FdoDateTime GetDateTime(FdoInt32 index);

    // Gets the Double floating point value of the specified column.
    // No conversion is performed, therefore the column must be of type
    // Double or an exception is thrown.
    virtual double GetDouble(FdoString* propertyName);
    virtual double GetDouble(FdoInt32 index);

    // Gets the Int16 value of the specified column. No conversion is
    // performed, therefore the column must be of type Int16 or an
    // exception is thrown.
    virtual FdoInt16 GetInt16(FdoString* propertyName);
    virtual FdoInt16 GetInt16(FdoInt32 index);

    // Gets the Int32 value of the specified column. No conversion is
    // performed, therefore the column must be of type Int32 or an
    // exception is thrown.
    virtual FdoInt32 GetInt32(FdoString* propertyName);
    virtual FdoInt32 GetInt32(FdoInt32 index);

    // Gets the Int64 value of the specified column. No conversion
    // is performed, therefore the column must be of type Int64 or an
    // exception is thrown.
    virtual FdoInt64 GetInt64(FdoString* propertyName);
    virtual FdoInt64 GetInt64(FdoInt32 index);

    // Gets the Single floating point value of the specified column.
    // No conversion is performed, therefore the column must be of type
    // Single or an exception is thrown.
    virtual float GetSingle(FdoString* propertyName);
    virtual float GetSingle(FdoInt32 index);

    // Gets the String value of the specified column. No conversion is
    // performed, therefore the column must be of type String or an
    // exception is thrown.
    virtual FdoString* GetString(FdoString* propertyName);
    virtual FdoString* GetString(FdoInt32 index);

    // Gets the LOB value of the specified column as a LOBValue.
    // No conversion is performed, therefore the column must be of
    // type LOB (BLOB/CLOB etc) or an exception is thrown.
    virtual FdoLOBValue* GetLOB(FdoString* propertyName);
    virtual FdoLOBValue* GetLOB(FdoInt32 index);

    // Gets a reference of the specified LOB property as a FdoBLOBStreamReader or
    // FdoCLOBStreamReader etc. to allow reading in blocks of data.
    // Because no conversion is performed, the property must be FdoDataType_BLOB
    // or FdoDataType_CLOB etc. (a LOB type)
    // Cast the FdoIStreamReader to the appropiate LOB Stream Reader.
    virtual FdoIStreamReader* GetLOBStreamReader(FdoString* propertyName);
    virtual FdoIStreamReader* GetLOBStreamReader(FdoInt32 index);

    // Returns true if the value of the specified column is null.
    virtual bool IsNull(FdoString* propertyName);
    virtual bool IsNull(FdoInt32 index);

    // Gets the Geometry value of the specified column as a byte array
    // in FGF format. No conversion is performed, therefore the column
    // must be of Geometric type or an exception is thrown.
    virtual FdoByteArray* GetGeometry(FdoString* propertyName);
    virtual FdoByteArray* GetGeometry(FdoInt32 index);

    virtual const FdoByte* GetGeometry(FdoString* propertyName, FdoInt32 * count);
    virtual const FdoByte* GetGeometry(FdoInt32 index, FdoInt32* count);

    virtual FdoIFeatureReader* GetFeatureObject(FdoString* propertyName);
    virtual FdoIFeatureReader* GetFeatureObject(FdoInt32 index);

    virtual FdoIRaster* GetRaster(FdoString* propertyName);
    virtual FdoIRaster* GetRaster(FdoInt32 index);

    // Advances the reader to the next item. The default position of the
    // reader is prior to the first item. Therefore you must call ReadNext
    // to begin accessing any data.
    virtual bool ReadNext();

    // Closes the ISQLDataReader object freeing any resources it may be holding.
    virtual void Close();
private:
        GdbiQueryResult*    mQueryResult;
        DbiConnection*      mConnection; // The dbi connection that should be used for any dbi level access
        FdoRdbmsConnection* mFdoConnection;
        bool                mHasMoreRows;
        int                 mColCount;
        LinkColumnList      mColList;
		StringRec*          mSprops; //cache of unicode string values of columns in the current row
        GdbiColumnDetailToIdx mColMap;
        int                 mGeomIdx; //indicates index of cached geometry
        unsigned char*      mWkbBuffer; //geometry buffer
        int                 mWkbBufferLen;
        int                 mWkbGeomLen;
        FdoFeatureSchemaCollection  *mSchemaCollection; // Used to cach the schema definition
        const FdoSmLpClassDefinition *mClassDefinition;
        const FdoSmLpClassDefinition *mMainClassDefinition;
        FdoClassDefinition *mFdoClassDefinition;
        FdoIdentifierCollection *mProperties; // list of properties to be returned
        bool                mIsFeatureQuery;   // Used to indicate that this reader is for a feature query
};

#endif // FDORDBMSSIMPLEFEATUREREADER_H
