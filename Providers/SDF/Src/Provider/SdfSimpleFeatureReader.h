#ifndef _SdfSimpleFeatureReader_H_
#define _SdfSimpleFeatureReader_H_
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
#include <vector>

//forward declare
class SdfConnection;
class PropertyIndex;
class FdoExpressionEngine;
class BinaryReader;
class DataDb;
struct PropertyStub;

class SdfSimpleFeatureReader :public FdoIFeatureReader
{
    //-------------------------------------------------------
    // Constructor / destructor
    //-------------------------------------------------------

public:
    // constructs an SdfFeatureReader using the specified
    // connection, class definition, and Sdf reader
    SDF_API SdfSimpleFeatureReader(SdfConnection* connection, 
                                       FdoClassDefinition* classDef, 
                                       FdoFilter* filter, 
                                       recno_list* features,
                                       FdoIdentifierCollection* selectIdents,
                                       FdoPropertyDefinitionCollection* computedProps);
	
	SdfSimpleFeatureReader( SdfSimpleFeatureReader& reader );

protected:
    SDF_API SdfSimpleFeatureReader() {};
    // default destructor
    SDF_API virtual ~SdfSimpleFeatureReader();

    //-------------------------------------------------------
    // FdoIDisposable implementation
    //-------------------------------------------------------

protected:
    // dispose this object
    SDF_API virtual void Dispose();

    //-------------------------------------------------------
    // FdoIFeatureReader implementation
    //-------------------------------------------------------

public:
    // Gets the definition of the object currently being read.  If the
    // user has requested only a subset of the class properties the class
    // definition will reflect what the user asked for and not the full
    // class definition.
    SDF_API virtual FdoClassDefinition* GetClassDefinition();

    // Gets a value indicating the depth of nesting for the current reader.
    // The depth value increases each time GetFeatureObject is called and
    // a new reader is returned.  The outermost reader has a depth of 0.
    SDF_API virtual FdoInt32 GetDepth();

    // Gets the Boolean value of the specified property.  No conversion
    // is performed, therefore the property must be of type Boolean or
    // an exception is thrown.
    SDF_API virtual bool GetBoolean(FdoString* propertyName);

    // Gets the Byte value of the specified property.  No conversion
    // is performed, therefore the property must be of type Byte or
    // an exception is thrown.
    SDF_API virtual FdoByte GetByte(FdoString* propertyName);

    // Gets the DateTime value of the specified property.  No conversion
    // is performed, therefore the property must be of type DateTime or
    // an exception is thrown.
    SDF_API virtual FdoDateTime GetDateTime(FdoString* propertyName);

    // Gets the Double floating point value of the specified property.
    // No conversion is performed, therefore the property must be of
    // type Double or an exception is thrown.
    SDF_API virtual double GetDouble(FdoString* propertyName);

    // Gets the Int16 value of the specified property.  No conversion
    // is performed, therefore the property must be of type Int16 or
    // an exception is thrown.
    SDF_API virtual FdoInt16 GetInt16(FdoString* propertyName);

    // Gets the Int32 value of the specified property.  No conversion
    // is performed, therefore the property must be of type Int32 or
    // an exception is thrown.
    SDF_API virtual FdoInt32 GetInt32(FdoString* propertyName);

    // Gets the Int64 value of the specified property.  No conversion
    // is performed, therefore the property must be of type Int64 or
    // an exception is thrown.
    SDF_API virtual FdoInt64 GetInt64(FdoString* propertyName);

    // Gets the Single floating point value of the specified property.
    // No conversion is performed, therefore the property must be of
    // type Single or an exception is thrown.
    SDF_API virtual float GetSingle(FdoString* propertyName);

    // Gets the String value of the specified property.  No conversion
    // is performed, therefore the property must be of type String or
    // an exception is thrown.
    SDF_API virtual FdoString* GetString(FdoString* propertyName);

    /// <summary>Gets a LOBValue reference. The LOB is fully read in and data available.
    /// Because no conversion is performed, the property must be FdoDataType_BLOB or
    /// FdoDataType_CLOB etc. (a LOB type)
    /// <param name="propertyName">Input the property name.</param> 
    /// <returns>Returns the reference to LOBValue</returns> 
    SDF_API virtual FdoLOBValue* GetLOB(FdoString* propertyName);

    /// <summary>Gets a reference of the specified LOB property as a FdoBLOBStreamReader or
    /// FdoCLOBStreamReader etc. to allow reading in blocks of data.
    /// Because no conversion is performed, the property must be FdoDataType_BLOB 
    /// or FdoDataType_CLOB etc. (a LOB type)
    /// Cast the FdoIStreamReader to the appropiate LOB Stream Reader.
    /// <param name="propertyName">Input the property name.</param> 
    /// <returns>Returns a reference to a LOB stream reader</returns> 
    SDF_API virtual FdoIStreamReader* GetLOBStreamReader(FdoString* propertyName );

    // Returns true if the value of the specified property is null.
    SDF_API virtual bool IsNull(FdoString* propertyName);

    // Gets a reference to an IFeatureReader to read the data contained
    // in the object or object collection property.  If the property is
    // not an object property an exception is thrown.
    SDF_API virtual FdoIFeatureReader* GetFeatureObject(FdoString* propertyName);

    // Gets the Geometry value of the specified property as a byte array
    // in FGF format.  No conversion is performed, therefore the property
    // must be of Geometric type or an exception is thrown.
    SDF_API virtual FdoByteArray* GetGeometry(FdoString* propertyName);

    // Gets the Geometry value of the specified property as a byte array
    // in FGF format.  No conversion is performed, therefore the property
    // must be of Geometric type or an exception is thrown.
    SDF_API virtual const FdoByte* GetGeometry(FdoString* propertyName, FdoInt32* len);


    /// <summary>Gets the raster object of the specified property.
    /// Because no conversion is performed, the property must be
    /// of Raster type; otherwise, an exception is thrown.</summary>
    /// <param name="propertyName">Input the property name.</param> 
    /// <returns>Returns the raster object.</returns> 
    SDF_API virtual FdoIRaster* GetRaster(FdoString* propertyName);

    // Advances the reader to the next item and returns true if there is
    // another object to read or false if reading is complete.  The default
    // position of the reader is prior to the first item.  Therefore you
    // must call ReadNext to begin accessing any data.
    SDF_API virtual bool ReadNext();

    // Closes the IFeatureReader object freeing any resources it may be
    // holding.
    SDF_API virtual void Close();

	//
	// Internal function used to quickly get the data type of a given property.
	virtual FdoDataType GetDataType( FdoString* propertyName ); 

	// Internal getter for the binary data
	virtual SQLiteData* GetRawData();

	// Internal getter for the binary key
	virtual SQLiteData* GetRawKey();

protected:

    FdoClassDefinition* m_classDefPruned; 
    
    FdoClassDefinition* m_class;
    SdfConnection* m_connection;
    PropertyIndex* m_propIndex;
    PropertyIndex* m_basePropIndex;

    SQLiteData* m_currentKey;
    SQLiteData* m_currentData;

    //used when iterating over all features in the db
    DataDb* m_dbData;

    FdoFilter* m_filter;
	FdoExpressionEngine *m_filterExec;

    REC_NO m_currentFeatureRecno;

    unsigned int m_currentFCID;

	BinaryReader* m_dataReader;

	std::map<std::wstring, wchar_t*> m_stringPropsCache;   
  
    FdoClassDefinitionP m_lastClassDefinition;

private:
    int PositionReader(int recordIndex);

    bool TestFeatureClassHierarchy();

    void CheckIfPropExists(FdoString* name);

	void RefreshData();

    bool IsNullObject( PropertyStub* ps );

	FdoValueExpression* GetValue( FdoDataType type );

    FdoClassDefinition* CloneAndPruneClass(FdoClassDefinition *fdoClassDef, FdoIdentifierCollection *idCollection, FdoPropertyDefinitionCollection* computedProps);

    recno_list::iterator m_currentFeatureRecnoIter;
    
    recno_list* m_features;

};

#endif

