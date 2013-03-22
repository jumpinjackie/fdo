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
#ifndef FDOPOSTGISOVGEOMETRICCOLUMN_H
#define FDOPOSTGISOVGEOMETRICCOLUMN_H

#include <Rdbms/Override/PostGis/PostGisOv.h>
#include <Rdbms/Override/RdbmsOvGeometricColumn.h>


/// \brief
/// Concrete class defining physical schema overrides for geometric column.
/// This class is used in PostGIS schema overrides.
///
class FdoPostGISOvGeometricColumn : public FdoRdbmsOvGeometricColumn
{
public:

    /// Named constructor for FdoPostGISOvGeometricColumn.
    /// 
    /// \return
    /// Pointer to new instance of FdoPostGISOvGeometricColumn.
    /// 
    FDOPOSTGIS_OV_API static FdoPostGISOvGeometricColumn* Create();

    /// Named constructor for FdoPostGISOvGeometricColumn.
    /// 
    /// \param
    /// name [in] - name of column.
    ///
    /// \return
    /// Pointer to new instance of FdoPostGISOvGeometricColumn.
    /// 
    FDOPOSTGIS_OV_API static FdoPostGISOvGeometricColumn* Create(
        FdoString* name);


/// \cond DOXYGEN-IGNORE

    /// XML serialization functions, not part of the API.

    /// Initialize this element from its XML attributes
    ///
    /// \param
    /// saxContext [in] - 
    ///
    /// \param
    /// attrs [in] - 
    ///
    virtual void InitFromXml(FdoXmlSaxContext* saxContext,
        FdoXmlAttributeCollection* attrs);

    /// Serialize provider-specific parts of this override to XML
    ///
    /// \param
    /// xmlWriter [in] - 
    ///
    /// \param
    /// flags [in] - 
    ///
    virtual void _writeXmlContents(FdoXmlWriter* xmlWriter,
        const FdoXmlFlags* flags);

/// \endcond

protected:

    /// Default constructor.
    FdoPostGISOvGeometricColumn();

    /// Alternative constructor.
    /// \param name [in] - name of class definition. 
    FdoPostGISOvGeometricColumn(FdoString* name);

    /// Destructor
    virtual ~FdoPostGISOvGeometricColumn();

    /// Named destructor.
    /// Responsible for final destruction calling
    /// \code delete this \endcode.
    virtual void Dispose();

private:

};

/// \brief
/// Declaration of smart-pointer type
/// for FdoPostGISOvGeometricColumn.
/// Provided for convenience.
///
typedef FdoPtr<FdoPostGISOvGeometricColumn>
    FdoPostGISOvGeometricColumnP;

#endif // FDOPOSTGISOVGEOMETRICCOLUMN_H
