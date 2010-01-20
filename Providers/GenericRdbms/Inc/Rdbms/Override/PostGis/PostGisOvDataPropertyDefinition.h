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
#ifndef FDOPOSTGISOVDATAPROPERTYDEFINITION_H
#define FDOPOSTGISOVDATAPROPERTYDEFINITION_H

#include <FdoCommon.h>
#include <Rdbms/Override/RdbmsOvDataPropertyDefinition.h>
#include <Rdbms/Override/PostGis/PostGisOv.h>
#include <Rdbms/Override/PostGis/PostGisOvColumn.h>
#include <Rdbms/Override/PostGis/PostGisOvPropertyDefinition.h>

/// \brief
/// Concrete class defining physical schema overrides for a data 
/// property definition.
///
class FdoPostGISOvDataPropertyDefinition :
    public FdoRdbmsOvDataPropertyDefinition,
    public FdoPostGISOvPropertyDefinition
{
public:
 
    /// \brief
    /// Named constructor.
    /// Creates default instance of FdoPostGISOvDataPropertyDefinition.
    /// 
    /// \return
    /// Pointer to new instance of FdoPostGISOvDataPropertyDefinition.
    /// 
    FDOPOSTGIS_OV_API static FdoPostGISOvDataPropertyDefinition* Create();

    /// Named constructor for FdoPostGISOvDataPropertyDefinition.
    /// 
    /// \param
    /// name [in] - name of property definition.
    ///
    /// \return
    /// Pointer to new instance of FdoPostGISOvDataPropertyDefinition.
    /// 
    FDOPOSTGIS_OV_API static FdoPostGISOvDataPropertyDefinition* Create(FdoString* name);

    /// \brief
    /// Gets the PostGIS override column.
    /// 
    /// \return PostGIS override column.
    /// 
    FDOPOSTGIS_OV_API FdoPostGISOvColumn* GetColumn();

    /// \brief
    /// Sets the PostGIS column.
    /// 
    /// \param
    /// column [in] - pointer to column definition.
    /// 
    FDOPOSTGIS_OV_API void SetColumn(FdoPostGISOvColumn *column);

protected:
    
    /// Default constructor.
    FdoPostGISOvDataPropertyDefinition();

    /// Alternative constructor.
    /// \param name [in] - name of class definition.
    FdoPostGISOvDataPropertyDefinition(FdoString* name);

    /// Destructor
    virtual ~FdoPostGISOvDataPropertyDefinition();

    /// Named destructor.
    virtual void Dispose();

private:

};

/// \brief
/// Declaration of smart-pointer type for FdoPostGISOvDataPropertyDefinition.
/// Provided for convenience.
///
typedef FdoPtr<FdoPostGISOvDataPropertyDefinition> FdoPostGISOvDataPropertyP;

#endif // FDOPOSTGISOVDATAPROPERTYDEFINITION_H
