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
#ifndef FDOSMPHPOSTGISDBCOLUMN_H
#define FDOSMPHPOSTGISDBCOLUMN_H

#include <Sm/Ph/Column.h>
#include <Sm/Ph/Rd/ColumnReader.h>

/// Definition of column object for PostGIS provider.
///
class FdoSmPhPostGisColumn : virtual public FdoSmPhColumn
{
public:
    FdoSmPhColumnP GetBaseColumn();

   	virtual void SetElementState(FdoSchemaElementState elementState);

protected:
    
    /// \todo To be documented.
    FdoSmPhPostGisColumn(FdoSmPhRdColumnReader* reader);
    
    /// \todo To be documented.
    virtual ~FdoSmPhPostGisColumn();

    FdoSmPhColumnP GetBaseColumn( FdoSmPhDbObjectP dbObject );

protected:
    
    /// Destructor.
    /// The following constructor is to satisfy the compiler,
    /// and should never actually be called.
    FdoSmPhPostGisColumn();

private:
    FdoSmPhColumnP mBaseColumn;

};

/// \brief
/// Declaration of smart-pointer type.
/// Provided for convenience.
///
typedef FdoPtr<FdoSmPhPostGisColumn> FdoSmPhPostGisColumnP;

#endif // FDOSMPHPOSTGISDBCOLUMN_H
