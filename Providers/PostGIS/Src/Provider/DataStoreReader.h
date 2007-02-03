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
#ifndef FDOPOSTGIS_DATASTOREREADER_H_INCLUDED
#define FDOPOSTGIS_DATASTOREREADER_H_INCLUDED

namespace fdo { namespace postgis {

/// Implementation of forward-only and read-only iterator for
/// reading feature data from a datastore.
///
class DataStoreReader : public FdoIDataStoreReader
{
public:

    DataStoreReader();
    virtual ~DataStoreReader();

    //
    // FdoIDataStoreReader
    //

    /// Get name of the datastore being read.
    FdoString* GetName();

    /// Get description of the datastore being read.
    FdoString* GetDescription();

 	/// Get flag that indicates whether the datastore is FDO enabled or not.
    bool GetIsFdoEnabled();

 	/// Get dictionary of properties set during datastore creation.
    /// Password property is not included.
    FdoIDataStorePropertyDictionary* GetDataStoreProperties();

 	/// Advances the reader iterator to next item.
    /// It returns true if there is any object to read or false if reading is complete.
    bool ReadNext();

 	// Close the reader object, freeing any resources it may be holding.
    void Close();

private:

};

}} // namespace fdo::postgis

#endif // FDOPOSTGIS_DATASTOREREADER_H_INCLUDED
