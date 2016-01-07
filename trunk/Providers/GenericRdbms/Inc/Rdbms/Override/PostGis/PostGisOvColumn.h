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
#ifndef FDOPOSTGISOVCOLUMN_H
#define FDOPOSTGISOVCOLUMN_H

#include <Rdbms/Override/PostGis/PostGisOv.h>
#include <Rdbms/Override/RdbmsOvColumn.h>

/// \brief
/// Concrete class defining physical schema overrides for column.
/// This class is used in PostGIS schema overrides.
///
class FdoPostGISOvColumn : public FdoRdbmsOvColumn
{
public:

    /// Named constructor for FdoPostGISOvColumn.
    /// 
    /// \return
    /// Pointer to new instance of FdoPostGISOvColumn.
    /// 
    FDOPOSTGIS_OV_API static FdoPostGISOvColumn* Create();

    /// Named constructor for FdoPostGISOvColumn.
    /// 
    /// \param
    /// name [in] - name of column.
    ///
    /// \return
    /// Pointer to new instance of FdoPostGISOvColumn.
    /// 
    FDOPOSTGIS_OV_API static FdoPostGISOvColumn* Create(FdoString* name);

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

/// \endcond

protected:

    /// Default constructor.
    FdoPostGISOvColumn();

    /// Alternative constructor.
    /// \param name [in] - name of class definition. 
    FdoPostGISOvColumn(FdoString* name);
    
    /// Destructor
    virtual ~FdoPostGISOvColumn();
    
    /// Named destructor.
    /// Responsible for final destruction calling
    /// \code delete this \endcode.
    virtual void Dispose();

    /// Serialize this property to XML.
    ///
    /// \param
    /// xmlWriter [in] - 
    ///
    /// \param
    /// flags [in] - 
    ///
    virtual void _writeXmlContents(FdoXmlWriter* xmlWriter,
        const FdoXmlFlags* flags);

private:

};

/// \brief
/// Declaration of smart-pointer type for FdoPostGISOvColumn.
/// Provided for convenience.
///
typedef FdoPtr<FdoPostGISOvColumn> FdoPostGISOvColumnP;

#endif // FDOPOSTGISOVCOLUMN_H
