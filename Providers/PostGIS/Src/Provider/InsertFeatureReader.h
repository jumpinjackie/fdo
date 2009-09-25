//
// Copyright (C) 2008 Mateusz Loskot <mateusz@loskot.net>
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
//
#ifndef FDOPOSTGIS_INSERTFEATUREREADER_H_INCLUDED
#define FDOPOSTGIS_INSERTFEATUREREADER_H_INCLUDED

#include <Fdo/Commands/Feature/IFeatureReader.h>
#include <Fdo/Commands/PropertyValueCollection.h>
#include <Fdo/Schema/ClassDefinition.h>
#include "FdoDefaultFeatureReader.h"

namespace fdo { namespace postgis {


/// Implementation of forward-only and read-only iterator for reading
/// features inserted with FdoIInsert command.
/// This reader works against collection of properties and it 
/// does not query PostGIS datastore.
///
class InsertFeatureReader : public FdoDefaultFeatureReader
{
public:

    /// Type of FDO smart pointer for the class.
    typedef FdoPtr<InsertFeatureReader> Ptr;

    /// Default constructor.
    InsertFeatureReader(FdoPropertyValueCollection* props, FdoClassDefinition* classDef);

    //
    // FdoIReader interface
    //

    /// Get Boolean value of the specified property.
    virtual bool GetBoolean(FdoString* propertyName);

    /// Get byte value of the specified property.
    virtual FdoByte GetByte(FdoString* propertyName);

    /// Get date and time value of the specified property.
    virtual FdoDateTime GetDateTime(FdoString* propertyName);

    /// Get double-precision floating point value of the specified property.
    virtual double GetDouble(FdoString* propertyName);

    /// Get 16-bit integer value of the specified property.
    virtual FdoInt16 GetInt16(FdoString* propertyName);

    /// Get 32-bit integer value of the specified property.
    virtual FdoInt32 GetInt32(FdoString* propertyName);

    /// Get 64-bit integer value of the specified property.
    virtual FdoInt64 GetInt64(FdoString* propertyName);

    /// Get Single floating point value of the specified property.
    virtual float GetSingle(FdoString* propertyName);

    /// Get string value of the specified property.
    virtual FdoString* GetString(FdoString* propertyName);

    /// Gets LOBValue reference.
    virtual FdoLOBValue* GetLOB(FdoString* propertyName);

    /// Get reference of the specified LOB property as of type of
    /// FdoBLOBStreamReader or FdoCLOBStreamReader etc.
    virtual FdoIStreamReader* GetLOBStreamReader(FdoString* propertyName);

    /// Get raster object of the specified property.
    /// No conversion is performed, so the property must be of Raster type;
    /// otherwise, an exception is thrown.
    virtual FdoIRaster* GetRaster(FdoString* propertyName);

    /// Check if the value of the specified property is null.
    virtual bool IsNull(FdoString* propertyName);

    /// Advance the reader iterator to next item.
    /// \return true if there is another object to read, false if reading is complete.
    virtual bool ReadNext();

    /// Close the reader object and release any of its resources.
    virtual void Close();

    //
    // FdoIFeatureReader interface
    //

    /// Get definition of the object currently being read.
    virtual FdoClassDefinition* GetClassDefinition();
 	
    /// Get value indicating depth of nesting for the current reader.
    /// \todo What is readers nesting?
    ///
    virtual FdoInt32 GetDepth();
 	
    /// Get geometry value of the specified property as a byte array in FGF format.
    virtual const FdoByte* GetGeometry(FdoString* propertyName, FdoInt32* count);
 	
    /// Get geometry value of the specified property as a byte array in FGF format.
    virtual FdoByteArray* GetGeometry(FdoString* propertyName);
 	
    /// Get reference to instance of FdoIFeatureReader to read the data contained
    /// in the object or object collection property. 
    virtual FdoIFeatureReader* GetFeatureObject(FdoString* propertyName);
 	
protected:

    /// Destructor
    virtual ~InsertFeatureReader();

    //
    // FdoIDisposable
    //

    virtual void Dispose();

private:

    //
    // Private data members
    //

    typedef FdoDefaultFeatureReader Base;
	
    FdoPtr<FdoPropertyValueCollection> mProps;
    FdoPtr<FdoClassDefinition> mClassDef;
    bool mIsFirst;

    //
    // Private operations
    //
    FdoDataValue* GetDataValue(FdoString* propertyName);
};

}} // namespace fdo::postgis

#endif // FDOPOSTGIS_INSERTFEATUREREADER_H_INCLUDED

