#ifndef FDORDBMSSQLSERVEROPTIMIZEDAGGREGATEREADER_H
#define FDORDBMSSQLSERVEROPTIMIZEDAGGREGATEREADER_H	1
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
#include "../../Fdo/Filter/FdoRdbmsFilterProcessor.h"

#include <FdoRdbmsFeatureReader.h>
#include "FdoRdbmsSqlServerConnection.h"


class FdoRdbmsSqlServerOptimizedAggregateReader : public FdoRdbmsFeatureReader 
{
	friend class FdoRdbmsSelectCommand;
public:

    FdoRdbmsSqlServerOptimizedAggregateReader(FdoIConnection* conn, const FdoSmLpClassDefinition* originalClass, aggr_list *selAggrList, FdoFilter* filter);
    ~FdoRdbmsSqlServerOptimizedAggregateReader();

protected:

    virtual void Dispose();

public:
	virtual bool			IsNull( FdoString *idName );
	virtual FdoInt64		GetInt64( FdoString* idName );
	virtual FdoDouble		GetDouble( FdoString* idName );
	virtual FdoByteArray*	GetGeometry( FdoString* idName);
	virtual	FdoInt32		GetPropertyCount();
	virtual	FdoString*		GetPropertyName(FdoInt32 index);
	virtual	FdoDataType		GetDataType(FdoString* idName);
	virtual	FdoPropertyType GetPropertyType(FdoString* idName);
    virtual FdoString*      GetPropertyNameForDataReader(FdoInt32 index);
    // Overriden FdoIReader methods:
    virtual bool			ReadNext();
	virtual void			Close();

private:
	FdoInt32				Validate( FdoString* idName );

private:
    FdoPtr<FdoIPolygon>		m_Extents;  // the geometry that store the spatial extents
    FdoInt32				m_ReaderIndex;  // the current row we are on (-1 == before first row, 0 == first row, etc)
 	FdoInt64				m_Count;

	aggr_list				*m_SelAggrList;
};

#endif
