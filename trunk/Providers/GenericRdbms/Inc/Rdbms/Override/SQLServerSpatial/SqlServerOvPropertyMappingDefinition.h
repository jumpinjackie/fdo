#ifndef FDO_SqlServerOVPROPERTYMAPPINGDEFINITION_H
#define FDO_SqlServerOVPROPERTYMAPPINGDEFINITION_H
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

#include <FdoCommon.h>
#include <Fdo/Xml/Flags.h>
#include <Fdo/Commands/Schema/PhysicalElementMapping.h>
#include <Rdbms/Override/RdbmsOvPropertyMappingDefinition.h>
#include <Rdbms/Override/SQLServerSpatial/SqlServerOv.h>

/// \brief
/// Abstract class defining the physical schema overrides for object property table
/// mappings.
class FdoSqlServerOvPropertyMappingDefinition : public virtual FdoRdbmsOvPropertyMappingDefinition
{
public:

protected:
    FdoSqlServerOvPropertyMappingDefinition();
    virtual ~FdoSqlServerOvPropertyMappingDefinition();
};

/// \brief
/// FdoSqlServerOvPropertyMappingP is a FdoPtr on FdoSqlServerOvPropertyMappingDefinition, provided for convenience.
typedef FdoPtr<FdoSqlServerOvPropertyMappingDefinition> FdoSqlServerOvPropertyMappingP;

#endif


