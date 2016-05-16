//
// Copyright (C) 2006 Refractions Research, Inc.
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
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
#ifndef FDOSMPHPOSTGISCOLTYPEMAPPER_H
#define FDOSMPHPOSTGISCOLTYPEMAPPER_H

#include <Sm/NamedCollection.h>
#include <Sm/Ph/Coltype.h>

/// Defines a single entry for translating a column type between
/// enum facet and PostGIS string format.
///
class FdoSmPhPostGisColTypeMapEntry
{
public:

    /// Create a column type mapping.
    ///
    /// \param
    /// colType [in] - enum facet.
    /// \param
    /// colTypeName [in] - corresponding string representation of type.
    /// \param
    /// defaultSize [in] - type's default size.
    ///
    FdoSmPhPostGisColTypeMapEntry(FdoSmPhColType colType,
        FdoString* colTypeName,
        FdoInt32 defaultSize = 0);

    /// Destructor.
    ~FdoSmPhPostGisColTypeMapEntry();

    /// Get the Map entry in string form
    /// \return
    /// String representation of map entry.
    ///
    FdoString* GetName() const;

    // TODO: mloskot - Consider moving following data memebers to private.

    /// \todo To be documented.
    FdoSmPhColType mColType;

    /// \todo To be documented.
    FdoStringP mColTypeString;
    
    FdoInt32 mDefaultSize;
}; // class FdoSmPhPostGisColTypeMapEntry

inline FdoString* FdoSmPhPostGisColTypeMapEntry::GetName() const
{
    return mColTypeString;
}


/// Defines mapper that lists all FDO property types with their
/// corresponding enumeration facet and string representations.
///
class FdoSmPhPostGisColTypeMapper
{
public:

    /// Default constructor.
    FdoSmPhPostGisColTypeMapper();

    /// Destructor.
    ~FdoSmPhPostGisColTypeMapper();

    /// Converts string to facet
    ///
    /// \param
    /// colTypeString [in] - string of type mapped to enumerator.
    /// \param
    /// typmod [in] - native PostgreSQL size and scale indicator.
    /// \param
    /// size [out] - FDO size. derived from column type and typmod.
    /// \param
    /// scale [out] - FDO scale. derived from column type and typmod.
    ///
    static FdoSmPhColType String2Type(FdoString* colTypeString,
        int typmod,
        FdoInt32& size,
        FdoInt32& scale);

    /// Converts facet to string
    ///
    /// \param
    /// colType [in] - type enumerator mapped to string represenation.
    ///
    static FdoStringP Type2String(FdoSmPhColType colType);

private:

    /// Enum-String mappings for each property type.
    static FdoSmPhPostGisColTypeMapEntry* mMap[];

}; // class FdoSmPhPostGisColTypeMapper


#endif // FDOSMPHPOSTGISCOLTYPEMAPPER_H
