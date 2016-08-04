#ifndef FDO_SqlServerOVCLASSCOLLECTION_H
#define FDO_SqlServerOVCLASSCOLLECTION_H
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
#include <Rdbms/Override/SQLServerSpatial/SqlServerOvClassDefinition.h>
#include <Rdbms/Override/RdbmsOvClassCollection.h>
#include <Fdo/Commands/Schema/PhysicalElementMappingCollection.h>

/// \brief
/// Concrete class defining physical schema overrides for a collection of classes.
class FdoSqlServerOvClassCollection : public FdoRestrictedNamedCollection<FdoRdbmsOvClassDefinition,FdoSqlServerOvClassDefinition,FdoRdbmsOvClassCollection>
{
public:
    /// \brief
    /// Constructs a default of an FdoSqlServerOvClassCollection
    /// 
    /// \return
    /// Returns FdoSqlServerOvClassCollection
    /// 
    FDOSQLSERVER_OV_API static FdoSqlServerOvClassCollection* Create();

    /// \brief
    /// Constructs an instance of an FdoSqlServerOvClassCollection using the specified arguments
    /// 
    /// \param parent 
    /// Input parent
    /// 
    /// \return
    /// Returns FdoSqlServerOvClassCollection
    /// 
    FDOSQLSERVER_OV_API static FdoSqlServerOvClassCollection* Create(FdoRdbmsOvClassCollection* baseCollection);

protected:
    FdoSqlServerOvClassCollection();
    FdoSqlServerOvClassCollection(FdoRdbmsOvClassCollection* baseCollection);
    virtual ~FdoSqlServerOvClassCollection();
    virtual void Dispose();

    virtual FdoSqlServerOvClassDefinition* DownCast( FdoRdbmsOvClassDefinition* value ) const;

private:

};

/// \brief
/// FdoSqlServerOvClassesP is a FdoPtr on FdoSqlServerOvClassCollection, provided for convenience.
typedef FdoPtr<FdoSqlServerOvClassCollection> FdoSqlServerOvClassesP;

#endif


