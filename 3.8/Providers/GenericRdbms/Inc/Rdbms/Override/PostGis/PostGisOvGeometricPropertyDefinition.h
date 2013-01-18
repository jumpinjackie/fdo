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
#ifndef FDOPOSTGISOVGEOMETRICPROPERTYDEFINITION_H
#define FDOPOSTGISOVGEOMETRICPROPERTYDEFINITION_H

#include <Rdbms/Override/RdbmsOvGeometricPropertyDefinition.h>
#include <Rdbms/Override/PostGis/PostGisOv.h>
#include <Rdbms/Override/PostGis/PostGisOvGeometricColumn.h>
#include <Rdbms/Override/PostGis/PostGisOvPropertyDefinition.h>

/// \brief
/// Concrete class defining physical schema overrides for a geometric
/// property definition.
/// This class is used in PostGIS schema overrides.
///
class FdoPostGISOvGeometricPropertyDefinition :
    public FdoRdbmsOvGeometricPropertyDefinition,
    public FdoPostGISOvPropertyDefinition
{
public:

    /// \brief
    /// Constructs a default of an FdoPostGISOvGeometricPropertyDefinition
    /// 
    /// \return
    /// Returns FdoPostGISOvGeometricPropertyDefinition
    /// 
    FDOPOSTGIS_OV_API static FdoPostGISOvGeometricPropertyDefinition* Create();

    /// \brief
    /// Constructs an instance of an FdoPostGISOvGeometricPropertyDefinition
    /// using the specified arguments
    /// 
    /// \param name 
    /// Input name
    /// 
    /// \return
    /// Returns FdoPostGISOvGeometricPropertyDefinition
    /// 
    FDOPOSTGIS_OV_API static FdoPostGISOvGeometricPropertyDefinition* Create(
        FdoString* name);

    /// \brief
    /// Gets the column
    /// 
    /// \return
    /// Returns the column
    /// 
    FDOPOSTGIS_OV_API FdoPostGISOvGeometricColumn* GetColumn();

    /// \brief
    /// Sets the column
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDOPOSTGIS_OV_API void SetColumn(FdoPostGISOvGeometricColumn* column);


protected:

/// \cond DOXYGEN-IGNORE

    FdoPostGISOvGeometricPropertyDefinition();
    FdoPostGISOvGeometricPropertyDefinition(FdoString* name);
    virtual ~FdoPostGISOvGeometricPropertyDefinition();
    virtual void Dispose();

/// \endcond

private:

};

/// \brief
/// Declaration of smart-pointer type for FdoPostGISOvGeometricPropertyDefinition.
/// Provided for convenience.
///
typedef FdoPtr<FdoPostGISOvGeometricPropertyDefinition> FdoPostGISOvGeometricPropertyP;

#endif // FDOPOSTGISOVGEOMETRICPROPERTYDEFINITION_H
