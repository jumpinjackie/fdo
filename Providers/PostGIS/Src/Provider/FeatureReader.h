//
// Copyright (C) 2007 Refractions Research, Inc. 
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
#ifndef FDOPOSTGIS_FEATUREREADER_H_INCLUDED
#define FDOPOSTGIS_FEATUREREADER_H_INCLUDED

#include "Reader.h"

//
// Forward declarations
//
class fdo::postgis::Connection;


namespace fdo { namespace postgis {

/// Implementation of forward-only and read-only iterator for reading
/// feature data.
/// This reader works against PostGIS datastore.
///
class FeatureReader : public Reader<FdoIFeatureReader>
{
public:

    /// Default constructor.
    FeatureReader(Connection* conn);

    //
    // FdoIFeatureReader interface
    //

    /// Get definition of the object currently being read.
    virtual FdoClassDefinition* GetClassDefinition();
 	
    /// Get value indicating depth of nesting for the current reader.
    ///
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
    virtual ~FeatureReader();

    //
    // FdoIDisposable
    //

    virtual void Dispose();

private:

    typedef Reader<FdoIFeatureReader> Base;
	
};

}} // namespace fdo::postgis

#endif // FDOPOSTGIS_FEATUREREADER_H_INCLUDED
