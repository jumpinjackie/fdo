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
 *
 */

#ifndef FDOWFSFEATUREREADER_H
#define FDOWFSFEATUREREADER_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include <Fdo/Xml/FeatureReader.h>

class FdoWfsFeatureReader : public FdoIFeatureReader	
{
    typedef FdoWfsReader<FdoIFeatureReader> superclass;	

public:
    FdoWfsFeatureReader ();	
    virtual ~FdoWfsFeatureReader (void);

	virtual void Dispose();

    // Prevent the use of the Assignment Operation by definning it and not implemeting it.
    // DO NOT IMPLEMENT
    FdoWfsFeatureReader & operator= (const FdoWfsFeatureReader &right);

	///////////////////////////////////////
	// Overrides from FdoIFeatureReader
	///////////////////////////////////////

    /// <summary>Gets the definition of the object currently being read. If the user
    /// has requested only a subset of the class properties, the class 
    /// definition reflects what the user has asked, rather than the full class 
    /// definition.</summary>
    /// <returns>Returns the class definition object.</returns> 
    virtual FdoClassDefinition* GetClassDefinition ();

    /// <summary>Gets a value indicating the depth of nesting for the current reader.
    /// The depth value increases each time GetFeatureObject is called and a new 
    /// reader is returned. The outermost reader has a depth of 0.</summary>
    /// <returns>Returns the depth</returns> 
    virtual FdoInt32 GetDepth ();

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

    /// <summary>Gets the geometry value of the specified property as a byte array in 
    /// FGF format. Because no conversion is performed, the property must be
    /// of Geometric type; otherwise, an exception is thrown.</summary>
    /// <param name="propertyName">Input the property name.</param> 
    /// <returns>Returns the byte array in FGF format.</returns> 
    virtual FdoByteArray* GetGeometry(FdoString* propertyName);

    /// <summary>Gets a reference to an FdoIFeatureReader to read the data contained in
    /// the object or object collection property. If the property is not an
    /// object property, an exception is thrown.</summary>
    /// <param name="propertyName">Input the property name.</param> 
    /// <returns>Returns the nested feature reader</returns> 
    virtual FdoIFeatureReader* GetFeatureObject (FdoString* propertyName);

	/////////////////////////////////////////////////////////////
	// Overrides from FdoIReader
	/////////////////////////////////////////////////////////////
	/// <summary>Gets the Boolean value of the specified property. No conversion is
    /// performed, thus the property must be FdoDataType_Boolean or an 
    /// exception is thrown.</summary>
    /// <param name="propertyName">Input the property name.</param> 
    /// <returns>Returns the Boolean value.</returns> 
    virtual bool GetBoolean(FdoString* propertyName);

    /// <summary>Gets the byte value of the specified property. No conversion is 
    /// performed, thus the property must be FdoDataType_Byte or an 
    /// exception is thrown.</summary>
    /// <param name="propertyName">Input the property name.</param> 
    /// <returns>Returns the byte value.</returns> 
    virtual FdoByte GetByte(FdoString* propertyName);

    /// <summary> Gets the date and time value of the specified property. No conversion is 
    /// performed, thus the property must be FdoDataType_DateTime or an 
    /// exception is thrown.</summary>
    /// <param name="propertyName">Input the property name.</param> 
    /// <returns>Returns the date and time value.</returns> 
    virtual FdoDateTime GetDateTime(FdoString* propertyName);

    /// <summary>Gets the double-precision floating point value of the specified property. No
    /// conversion is performed, thus the property must be FdoDataType_Double
    /// or an exception is thrown.</summary>
    /// <param name="propertyName">Input the property name.</param> 
    /// <returns>Returns the double floating point value</returns> 
    virtual double GetDouble(FdoString* propertyName);

    /// <summary>Gets the 16-bit integer value of the specified property. No conversion is
    /// performed, thus the property must be FdoDataType_Int16 or an exception
    /// is thrown.</summary>
    /// <param name="propertyName">Input the property name.</param> 
    /// <returns>Returns the FdoInt16 value.</returns> 
    virtual FdoInt16 GetInt16(FdoString* propertyName);

    /// <summary>Gets the 32-bit integer value of the specified property. No conversion is
    /// performed, thus the property must be FdoDataType_Int32 or an exception
    /// is thrown.</summary>
    /// <param name="propertyName">Input the property name.</param> 
    /// <returns>Returns the FdoInt32 value</returns> 
    virtual FdoInt32 GetInt32(FdoString* propertyName);

    /// <summary>Gets the 64-bit integer value of the specified property. No conversion is
    /// performed, thus the property must be FdoDataType_Int64 or an exception
    /// is thrown.</summary>
    /// <param name="propertyName">Input the property name.</param> 
    /// <returns>Returns the FdoInt64 value.</returns> 
    virtual FdoInt64 GetInt64(FdoString* propertyName);

    /// <summary>Gets the Single floating point value of the specified property. No
    /// conversion is performed, thus the property must be FdoDataType_Single
    /// or an exception is thrown.</summary>
    /// <param name="propertyName">Input the property name.</param> 
    /// <returns>Returns the single value</returns> 
    virtual float GetSingle(FdoString* propertyName);

    /// <summary>Gets the string value of the specified property. No conversion is
    /// performed, thus the property must be FdoDataType_String or an exception
    /// is thrown.</summary>
    /// <param name="propertyName">Input the property name.</param> 
    /// <returns>Returns the string value</returns> 
    virtual FdoString* GetString(FdoString* propertyName);

    /// <summary>Gets a LOBValue reference. The LOB is fully read in and data available.
    /// Because no conversion is performed, the property must be FdoDataType_BLOB or
    /// FdoDataType_CLOB etc. (a LOB type)
    /// <param name="propertyName">Input the property name.</param> 
    /// <returns>Returns the reference to LOBValue</returns> 
    virtual FdoLOBValue* GetLOB(FdoString* propertyName);

    /// <summary>Gets a reference of the specified LOB property as a FdoBLOBStreamReader or
    /// FdoCLOBStreamReader etc. to allow reading in blocks of data.
    /// Because no conversion is performed, the property must be FdoDataType_BLOB 
    /// or FdoDataType_CLOB etc. (a LOB type)
    /// Cast the FdoIStreamReader to the appropiate LOB Stream Reader.
    /// <param name="propertyName">Input the property name.</param> 
    /// <returns>Returns a reference to a LOB stream reader</returns> 
    virtual FdoIStreamReader* GetLOBStreamReader(const wchar_t* propertyName );

    /// <summary>Returns true if the value of the specified property is null.</summary>
    /// <param name="propertyName">Input the property name.</param> 
    /// <returns>Returns true if the value is null.</returns> 
    virtual bool IsNull(FdoString* propertyName);

    /// <summary>Gets the raster object of the specified property.
    /// Because no conversion is performed, the property must be
    /// of Raster type; otherwise, an exception is thrown.</summary>
    /// <param name="propertyName">Input the property name.</param> 
    /// <returns>Returns the raster object.</returns> 
    virtual FdoIRaster* GetRaster(FdoString* propertyName);

    /// <summary>Advances the reader to the next item and returns true if there is
    /// another object to read or false if reading is complete. The default
    /// position of the reader is prior to the first item. Thus you must
    /// call ReadNext to begin accessing any data.</summary>
    /// <returns>Returns true if there is a next item.</returns> 
    virtual bool ReadNext();

    /// <summary>Closes the FdoIFeatureReader object, freeing any resources it may be holding.</summary>
    /// <returns>Returns nothing</returns> 
    virtual void Close();

	///////////////////////////////////////////////////
	// Helper methods
	///////////////////////////////////////////////////

	/// <summary>Sets the class to query and the properties to select. This method is used to 
	/// cache the class and properties which will be used when user call "GetClassDefinition".
	/// </summary>
	/// <param name="originClassDef">The class definition to be queried against.</param>
	/// <param name="propsToSelect">The properties to be selected by the user.</param>
	void SetClassDefinition (FdoClassDefinition* originClassDef, FdoIdentifierCollection* propsToSelect);

	void SetXmlFeatureReader(FdoXmlFeatureReader* xmlFeatureReader) { m_featureReader = xmlFeatureReader; }

private:
	FdoClassDefinition* CloneAndPruneClass(FdoClassDefinition*, FdoIdentifierCollection*, FdoPropertyDefinitionCollection*);
	FdoStringP _decodeName(FdoString* name);
	FdoPtr<FdoXmlFeatureReader> m_featureReader;
	FdoPtr<FdoClassDefinition> m_classDefPruned;
};

#endif // FDOWFSFEATUREREADER_H
