#ifndef FDO_SqlServerOVPROPERTYDEFINITION_H
#define FDO_SqlServerOVPROPERTYDEFINITION_H
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

#include <Rdbms/Override/RdbmsOvPropertyDefinition.h>
#include <Rdbms/Override/SQLServerSpatial/SqlServerOv.h>

/// \brief
/// Abstract class defining physical schema overrides for a property definition.
class FdoSqlServerOvPropertyDefinition : public virtual FdoRdbmsOvPropertyDefinition
{
friend class FdoSqlServerOvClassDefinition;
protected:
    FdoSqlServerOvPropertyDefinition();
    FdoSqlServerOvPropertyDefinition(FdoString* name);
    virtual ~FdoSqlServerOvPropertyDefinition();
    virtual void Dispose();

};

/// \brief
/// FdoSqlServerOvPropertyP is a FdoPtr on FdoSqlServerOvPropertyDefinition, provided for convenience.
typedef FdoPtr<FdoSqlServerOvPropertyDefinition> FdoSqlServerOvPropertyP;

#endif


