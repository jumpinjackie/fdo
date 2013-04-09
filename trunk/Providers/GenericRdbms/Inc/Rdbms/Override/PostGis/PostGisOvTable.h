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
#ifndef FDOPOSTGISOVTABLE_H
#define FDOPOSTGISOVTABLE_H

#include <Rdbms/Override/RdbmsOvTable.h>
#include <Rdbms/Override/PostGis/PostGisOv.h>

/// \brief
/// Concrete class defining physical schema overrides for a table.
/// This class is used in PostGIS schema overrides.
///
class FdoPostGISOvTable : public FdoRdbmsOvTable
{
public:

    /// Named constructor for FdoPostGISOvColumn.
    /// 
    /// \return
    /// Pointer to new instance of FdoPostGISOvColumn.
    /// 
    FDOPOSTGIS_OV_API static FdoPostGISOvTable* Create();

    /// Named constructor for FdoPostGISOvTable.
    /// 
    /// \param
    /// name [in] - name of table.
    ///
    /// \return
    /// Pointer to new instance of FdoPostGISOvTable.
    /// 
    FDOPOSTGIS_OV_API static FdoPostGISOvTable* Create(FdoString* name);

    /// \brief
    /// Gets the owner
    /// 
    /// \return
    /// Returns the owner
    /// 
    FDOPOSTGIS_OV_API FdoString* GetOwner();

    /// \brief
    /// Sets the owner
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDOPOSTGIS_OV_API void SetOwner(FdoString* owner);


/// \cond DOXYGEN-IGNORE

    /// XML Serialization functions, not part of the API.

    /// Serialize this property to XML.
    virtual void _writeXmlContents(FdoXmlWriter* xmlWriter,
        const FdoXmlFlags* flags);

    /// Initialize this element from its XML attributes
    virtual void InitFromXml(FdoXmlSaxContext* saxContext,
        FdoXmlAttributeCollection* attrs);

    /// Sets the parent of this object. Not part of the API.
    void SetParent(FdoPhysicalElementMapping* value);

/// \endcond

protected:

    FdoPostGISOvTable();
    FdoPostGISOvTable(FdoString* name);
    virtual ~FdoPostGISOvTable();
    virtual void Dispose();

    /// Named constructor.
    /// Called from class constructors.
    void Init();

private:
    FdoStringP mOwner;
};

/// \brief
/// Declaration of smart-pointer type for FdoPostGISOvTable.
/// Provided for convenience.
typedef FdoPtr<FdoPostGISOvTable> FdoPostGISOvTableP;

#endif // FDOPOSTGISOVTABLE_H
