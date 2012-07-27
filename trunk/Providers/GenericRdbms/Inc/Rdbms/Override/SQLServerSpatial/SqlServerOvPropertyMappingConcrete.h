#ifndef FDO_SqlServerOVPROPERTYMAPPINGRCONCRETE_H
#define FDO_SqlServerOVPROPERTYMAPPINGRCONCRETE_H
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

/// \brief
/// Concrete class defining the physical schema overrides for the Concrete type object
/// property mapping. Under the Concrete mapping type, each object property is kept in its own
/// table.
class FdoSqlServerOvPropertyMappingConcrete : public FdoSqlServerOvPropertyMappingRelation, public FdoRdbmsOvPropertyMappingConcrete
{

public:
    /// \brief
    /// Constructs a default of an FdoSqlServerOvPropertyMappingConcrete
    /// 
    /// \return
    /// Returns FdoSqlServerOvPropertyMappingConcrete
    /// 
    FDOSQLSERVER_OV_API static FdoSqlServerOvPropertyMappingConcrete* Create();

protected:
    FdoSqlServerOvPropertyMappingConcrete();
    virtual ~FdoSqlServerOvPropertyMappingConcrete();
    virtual void Dispose();

private:
};

/// \brief
/// FdoSqlServerOvPropertyMappingConcreteP is a FdoPtr on FdoSqlServerOvPropertyMappingConcrete, provided for convenience.
typedef FdoPtr<FdoSqlServerOvPropertyMappingConcrete> FdoSqlServerOvPropertyMappingConcreteP;

#endif


