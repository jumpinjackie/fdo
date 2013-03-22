#ifndef FDO_SqlServerOVPROPERTYMAPPINGRELATION_H
#define FDO_SqlServerOVPROPERTYMAPPINGRELATION_H
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

#include <Rdbms/Override/RdbmsOvPropertyMappingConcrete.h>
#include <Rdbms/Override/SQLServerSpatial/SqlServerOv.h>
#include <Rdbms/Override/SQLServerSpatial/SqlServerOvPropertyMappingDefinition.h>

class FdoSqlServerOvClassDefinition;

/// \brief
/// Abstract class defining the physical schema overrides for relation type object property
/// table mappings.
class FdoSqlServerOvPropertyMappingRelation : public FdoSqlServerOvPropertyMappingDefinition, public virtual FdoRdbmsOvPropertyMappingRelation
{

public:

    /// \brief
    /// Gets the internal class
    /// 
    /// \return
    /// Returns the internal class
    /// 
    FDOSQLSERVER_OV_API FdoSqlServerOvClassDefinition* GetInternalClass();

    /// \brief
    /// Sets the internal class
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDOSQLSERVER_OV_API void SetInternalClass(FdoSqlServerOvClassDefinition* classDefinition);

protected:
    FdoSqlServerOvPropertyMappingRelation();
    virtual ~FdoSqlServerOvPropertyMappingRelation();

    /// Creates and optionally attaches an internal class override object.
    virtual FdoRdbmsOvClassDefinition* CreateInternalClass(
        bool attach
    );
};

/// \brief
/// FdoSqlServerOvPropertyMappingRelationP is a FdoPtr on FdoSqlServerOvPropertyMappingRelation, provided for convenience.
typedef FdoPtr<FdoSqlServerOvPropertyMappingRelation> FdoSqlServerOvPropertyMappingRelationP;

#endif


