#ifndef FDO_SqlServerOVPROPERTYMAPPINGCLASS_H
#define FDO_SqlServerOVPROPERTYMAPPINGCLASS_H
/*
 * Copyright (C) 2004-2008  Autodesk, Inc.
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

#include <Rdbms/Override/SQLServerSpatial/SqlServerOv.h>
#include <Rdbms/Override/SQLServerSpatial/SqlServerOvPropertyMappingRelation.h>
#include <Rdbms/Override/RdbmsOvPropertyMappingClass.h>

/// \cond DOXYGEN-IGNORE

/// \brief
/// Concrete class defining the physical schema overrides for the Class
/// type object property mapping.
class FdoSqlServerOvPropertyMappingClass : public FdoSqlServerOvPropertyMappingRelation, public FdoRdbmsOvPropertyMappingClass
{
public:
    /// \brief
    /// Constructs a default of an FdoSqlServerOvPropertyMappingClass
    /// 
    /// \return
    /// Returns FdoSqlServerOvPropertyMappingClass
    /// 
    FDOSQLSERVER_OV_API static FdoSqlServerOvPropertyMappingClass* Create();

protected:
    FdoSqlServerOvPropertyMappingClass();
    virtual ~FdoSqlServerOvPropertyMappingClass();
    virtual void Dispose();
};

/// \brief
/// FdoSqlServerOvPropertyMappingClassP is a FdoPtr on FdoSqlServerOvPropertyMappingClass, provided for convenience.
typedef FdoPtr<FdoSqlServerOvPropertyMappingClass> FdoSqlServerOvPropertyMappingClassP;

/// \endcond

#endif


