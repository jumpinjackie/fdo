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

#ifndef ARCSDEREADER_H
#define ARCSDEREADER_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

class ArcSDEReader : public virtual FdoIFeatureReader, public virtual FdoISQLDataReader, public virtual FdoIDataReader
{
protected:
    union columnUnion
    {
         SE_BLOB_INFO _blob;
         struct tm _timedate;
         LFLOAT _double;
         FLOAT _float;
         SHORT _short;
         LONG  _long;
         SE_SHAPE _shape;
         CHAR* _string;
    };

    class ColumnDefinition
    {
    public:
        /* One based column number */
        int mColumnNumber;

        /* The column name. */
        wchar_t mColumnName[SE_MAX_COLUMN_LEN];

        /* The column type (e.g. SE_SMALLINT_TYPE, SE_FLOAT_TYPE, etc) */
        LONG mColumnType;

        /* The Fdo identifier from the Select list provided by the user,
           can contain ComputedIdentifiers, may not be populated */
        FdoPtr<FdoIdentifier> mIdentifier;

        /* The corresponding property name. */
        wchar_t mPropertyName[SE_MAX_COLUMN_LEN];  //TODO: length limit is wrong for Fdo property names

        /* The property data type for this column. (-1 for geometry types) */
        FdoDataType mPropertyType;

        /* The size of data (used so far only for strings). */
        int mDataLength;

        /* Bind variable (used during select command) */
        columnUnion mBindVariable;
        SHORT mBindIsNull;
        bool mBindVariableInitialized;

        /* stream statistics (used for aggregate functions and distinct) */
        SE_STATS *mStreamStats;

        /* Storage for values. */
        void* mValuePointer;
        FdoInt32 mValuePointerSize;

        ColumnDefinition();
        ~ColumnDefinition ();

        void ClearCache(bool bReaderClosing);
    };

    SE_STREAM mStream;
    bool mReady; // true after ReadNext()
    int mColumnCount;
    ColumnDefinition** mColumnDefs;
    FdoPtr<ArcSDEConnection> mConnection;
    FdoPtr<FdoIdentifierCollection> mSelectIds;
    bool m_bBindVariablesInitialized;

    FdoPtr<FdoClassDefinition> mClassDef;
    bool mDistinct;
    bool mSelectingAggregates;
    FdoOrderingOption mOrderingOption;
    FdoPtr<FdoIdentifierCollection> mOrderingIds;
    long mStreamStatsIndex;
    bool mRowNotValidated;  // Whether or not the reader has validated the current row of data

public:
    ArcSDEReader (ArcSDEConnection *connection, FdoClassDefinition* fdoClassDef, FdoIdentifierCollection* propertiesToSelect);
    virtual ~ArcSDEReader (void);

    virtual void Dispose();

    /// <summary>Increase the reference count.</summary>
    /// <returns>Returns the new reference count (value for debugging use only).</returns> 
    FdoInt32 AddRef()
    {
        // NOTE: due to multiple inheritance, there is an ambiguity in which AddRef() method to call.
        //  Calling BOTH AddRef() methods leads to instances of this class being prematurely released.
        return FdoIFeatureReader::AddRef();
    }

    /// <summary>Decrease the reference count.</summary>
    /// <returns>Returns the new reference count (value for debugging use only).</returns> 
    FdoInt32 Release ()
    {
        // NOTE: due to multiple inheritance, there is an ambiguity in which Release() method to call.
        //  Calling BOTH Release() methods leads to instances of this class being prematurely released.
        return FdoIFeatureReader::Release();
    }



    // Prevent the use of the Assignment Operation by defining it and not implementing it.
    // DO NOT IMPLEMENT
    ArcSDEReader & operator= (const ArcSDEReader &right);

    //
    // FdoIFeatureReader interface
    //

    /// <summary>Gets the Boolean value of the specified property. No conversion is
    /// performed, thus the property must be FdoDataType_Boolean or an 
    /// exception is thrown.</summary>
    /// <param name="identifier">Input the property name.</param> 
    /// <returns>Returns the Boolean value.</returns> 
    virtual bool GetBoolean (const FdoString* identifier);

    /// <summary>Gets the byte value of the specified property. No conversion is 
    /// performed, thus the property must be FdoDataType_Byte or an 
    /// exception is thrown.</summary>
    /// <param name="identifier">Input the property name.</param> 
    /// <returns>Returns the byte value.</returns> 
    virtual FdoByte GetByte (FdoString* identifier);

    /// <summary> Gets the date and time value of the specified property. No conversion is 
    /// performed, thus the property must be FdoDataType_DateTime or an 
    /// exception is thrown.</summary>
    /// <param name="identifier">Input the property name.</param> 
    /// <returns>Returns the date and time value.</returns> 
    virtual FdoDateTime GetDateTime (FdoString* identifier);

    /// <summary>Gets the double-precision floating point value of the specified property. No
    /// conversion is performed, thus the property must be FdoDataType_Double
    /// or an exception is thrown.</summary>
    /// <param name="identifier">Input the property name.</param> 
    /// <returns>Returns the double floating point value</returns> 
    virtual double GetDouble (FdoString* identifier);

    /// <summary>Gets the 16-bit integer value of the specified property. No conversion is
    /// performed, thus the property must be FdoDataType_Int16 or an exception
    /// is thrown.</summary>
    /// <param name="identifier">Input the property name.</param> 
    /// <returns>Returns the FdoInt16 value.</returns> 
    virtual FdoInt16 GetInt16 (FdoString* identifier);

    /// <summary>Gets the 32-bit integer value of the specified property. No conversion is
    /// performed, thus the property must be FdoDataType_Int32 or an exception
    /// is thrown.</summary>
    /// <param name="identifier">Input the property name.</param> 
    /// <returns>Returns the FdoInt32 value</returns> 
    virtual FdoInt32 GetInt32 (FdoString* identifier);

    /// <summary>Gets the 64-bit integer value of the specified property. No conversion is
    /// performed, thus the property must be FdoDataType_Int64 or an exception
    /// is thrown.</summary>
    /// <param name="identifier">Input the property name.</param> 
    /// <returns>Returns the FdoInt64 value.</returns> 
    virtual FdoInt64 GetInt64 (FdoString* identifier);

    /// <summary>Gets the Single floating point value of the specified property. No
    /// conversion is performed, thus the property must be FdoDataType_Single
    /// or an exception is thrown.</summary>
    /// <param name="identifier">Input the property name.</param> 
    /// <returns>Returns the single value</returns> 
    virtual float GetSingle (FdoString* identifier);

    /// <summary>Gets the string value of the specified property. No conversion is
    /// performed, thus the property must be FdoDataType_String or an exception
    /// is thrown.</summary>
    /// <param name="identifier">Input the property name.</param> 
    /// <returns>Returns the string value</returns> 
    virtual FdoString* GetString (FdoString* identifier);


    /// <summary>Gets a LOBValue reference. The LOB is fully read in and data available.
    /// Because no conversion is performed, the property must be FdoDataType_BLOB or
    /// FdoDataType_CLOB etc. (a LOB type)
    /// <param name="propertyName">Input the property name.</param> 
    /// <returns>Returns the reference to LOBValue</returns> 
    virtual FdoLOBValue* GetLOB(FdoString* identifier);

    /// <summary>Gets a reference of the specified LOB property as a FdoBLOBStreamReader or
    /// FdoCLOBStreamReader etc. to allow reading in blocks of data.
    /// Because no conversion is performed, the property must be FdoDataType_BLOB 
    /// or FdoDataType_CLOB etc. (a LOB type)
    /// Cast the FdoIStreamReader to the appropiate LOB Stream Reader.
    /// <param name="propertyName">Input the property name.</param> 
    /// <returns>Returns a reference to a LOB stream reader</returns> 
    virtual FdoIStreamReader* GetLOBStreamReader(FdoString* identifier);

    /// <summary>Returns true if the value of the specified property is null.</summary>
    /// <param name="identifier">Input the property name.</param> 
    /// <returns>Returns true if the value is null.</returns> 
    virtual bool IsNull (FdoString* identifier);

    /// <summary>Gets the geometry value of the specified property as a byte array in 
    /// FGF format. Because no conversion is performed, the property must be
    /// of Geometric type; otherwise, an exception is thrown.</summary>
    /// <param name="identifier">Input the property name.</param> 
    /// <returns>Returns the byte array in FGF format.</returns> 
    virtual FdoByteArray* GetGeometry (FdoString* identifier);

    /// <summary>Gets the geometry value of the specified property as a byte array in 
    /// FGF format. Because no conversion is performed, the property must be
    /// of Geometric type; otherwise, an exception is thrown. 
    /// This method is a language-specific performance optimization that returns a
    /// pointer to the array data, rather than to an object that encapsulates
    /// the array.  The array's memory area is only guaranteed to be valid
    /// until a call to ReadNext() or Close(), or the disposal of this reader
    /// object.</summary>
    /// <param name="propertyName">Input the property name.</param> 
    /// <param name="count">Output the number of bytes in the array.</param> 
    /// <returns>Returns a pointer to the byte array in FGF format.</returns> 
    virtual const FdoByte * GetGeometry(FdoString* propertyName, FdoInt32 * count);

    /// <summary>Gets the raster object of the specified property.
    /// Because no conversion is performed, the property must be
    /// of Raster type; otherwise, an exception is thrown.</summary>
    /// <param name="propertyName">Input the property name.</param> 
    /// <returns>Returns the raster object.</returns> 
    virtual FdoIRaster* GetRaster(FdoString* identifier);

    /// <summary>Advances the reader to the next item and returns true if there is
    /// another object to read or false if reading is complete. The default
    /// position of the reader is prior to the first item. Thus you must
    /// call ReadNext to begin accessing any data.</summary>
    /// <returns>Returns true if there is a next item.</returns> 
    virtual bool ReadNext ();

    /// <summary>Closes the Reader object, freeing any resources it may be holding.</summary>
    /// <returns>Returns nothing</returns> 
    virtual void Close ();


    //
    // FdoISQLDataReader interface specific  (dummy implementation here; true implementations in ArcSDESQLDataReader)
    //

    /// <summary>Gets the number of columns in the result set.</summary>
    /// <returns>Returns the number of columns.</returns> 
    virtual FdoInt32 GetColumnCount ()  {return 0L;};

    /// <summary>Gets the name of the column at the given ordinal position.</summary>
    /// <param name="index">Input the position of the column.</param> 
    /// <returns>Returns the column name</returns> 
    virtual FdoString* GetColumnName (FdoInt32 index) {return NULL;};

    /// <summary>Gets the data type of the column with the specified name.</summary>
    /// <param name="columnName">Input the column name.</param> 
    /// <returns>Returns the type of the column.</returns> 
    virtual FdoDataType GetColumnType (FdoString* columnName) {return (FdoDataType)0;};


    //
    // FdoIFeatureReader interface specific  (dummy implementation here; true implementations in ArcSDEFeatureReader)
    //

    /// <summary>Gets the definition of the object currently being read. If the user
    /// has requested only a subset of the class properties, the class 
    /// definition reflects what the user has asked, rather than the full class 
    /// definition.</summary>
    /// <returns>Returns the class definition object.</returns> 
    virtual FdoClassDefinition* GetClassDefinition () {return NULL;};

    /// <summary>Gets a value indicating the depth of nesting for the current reader.
    /// The depth value increases each time GetFeatureObject is called and a new 
    /// reader is returned. The outermost reader has a depth of 0.</summary>
    /// <returns>Returns the depth</returns> 
    virtual FdoInt32 GetDepth () {return 0L;}

    /// <summary>Gets a reference to an FdoIFeatureReader to read the data contained in
    /// the object or object collection property. If the property is not an
    /// object property, an exception is thrown.</summary>
    /// <param name="propertyName">Input the property name.</param> 
    /// <returns>Returns the nested feature reader</returns> 
    virtual FdoIFeatureReader* GetFeatureObject (FdoString* propertyName) {return NULL;};


    //
    // FdoIDataReader interface specific  (dummy implementation here; true implementations in ArcSDEDataReader)
    //

    /// <summary>Gets the number of propertys in the result set.</summary>
    /// <returns>Returns the number of propertys.</returns> 
    virtual FdoInt32 GetPropertyCount() { return 0; };

    /// <summary>Gets the name of the property at the given ordinal position.</summary>
    /// <param name="index">Input the position of the property.</param> 
    /// <returns>Returns the property name</returns> 
    virtual FdoString* GetPropertyName(FdoInt32 index) { return L""; };

    /// <summary>Gets the data type of the property with the specified name.</summary>
    /// <param name="propertyName">Input the property name.</param> 
    /// <returns>Returns the type of the property.</returns> 
    virtual FdoDataType GetDataType(FdoString* propertyName) { return (FdoDataType)0; };

    /// <summary>Gets the FDO property type of a given property. This is used
    //  to indicate if a given property is a geometric property or a data property. If the property is
    //  a FdoPropertyType_Data, then GetDataType can be used to to find the data type of the property.</summary>
    /// <param name="propertyName">Input the property name.</param> 
    /// <returns>Returns the FDO property type.</returns> 
    virtual FdoPropertyType GetPropertyType(FdoString* propertyName){ return (FdoPropertyType)0; };

protected:
	ArcSDEReader() {};
    // check for a correct state
    virtual void validate ();
    // get the column definitions from the stream
    virtual void getColumnDefs () = 0;
    // find a column definition by Fdo property name or arcsde column index
    virtual ColumnDefinition* getColumnDef (const wchar_t* identifier) = 0;
    virtual ColumnDefinition* getColumnDef (int arcSdeColumnIndex);

    // Clears the locally-cached values on each column:
    void ClearCachedColumnValues(bool bReaderClosing);
};

#endif // ARCSDEREADER_H

