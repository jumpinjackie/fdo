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

#include "Stdafx.h"
#include "FdoRdbmsSqlServerOptimizedAggregateReader.h"
#include "FdoRdbmsSchemaUtil.h"
#include <FdoCommonMiscUtil.h>

#include <float.h>

FdoRdbmsSqlServerOptimizedAggregateReader::FdoRdbmsSqlServerOptimizedAggregateReader(FdoIConnection* connection, const FdoSmLpClassDefinition* classDef, aggr_list *selAggrList) :
FdoRdbmsFeatureReader( connection, NULL, false, classDef, NULL)
{
	m_ReaderIndex = -1;
    m_SelAggrList = selAggrList;
	m_Count= 0;
 
	FdoSchemaManagerP	schemaMgr = mConnection->GetSchemaUtil()->GetSchemaManager();
	FdoSmPhMgrP			mgr = schemaMgr->GetPhysicalSchema();
	FdoSmPhOwnerP		owner = mgr->GetOwner();
	FdoString*			wdatastore = owner->GetName();

	// Get the table name
	FdoStringP	tableNameW	= mClassDefinition->GetDbObjectName();

	bool				countRequired = false;
	bool				mbrRequired = false;
	const	char *		columnName = NULL;
	int					isGeodetic = false;
    const wchar_t       *colNameW = NULL;

	for ( size_t i = 0; i < m_SelAggrList->size(); i++ )
	{ 
		AggregateElement	*id = m_SelAggrList->at(i);
		if ( id->type == FdoPropertyType_GeometricProperty )
		{
			mbrRequired = true;

			// Get the column name via property name
			const FdoSmLpFeatureClass*  feat = static_cast<const FdoSmLpFeatureClass *>( mClassDefinition );
			const FdoSmLpPropertyDefinitionCollection *props = feat->RefProperties();
			const FdoSmLpPropertyDefinition *prop = props->RefItem( id->propName );
		
			const FdoSmLpSimplePropertyDefinition* simpleProp = static_cast<const FdoSmLpSimplePropertyDefinition*>(prop);

			const FdoSmPhColumn *column = simpleProp->RefColumn();
			colNameW = column->GetName();
		}
		else
		{
			countRequired = true;
		}		
	}


	// Get the extents and count

    // Future optimization.
	//mConnection->dbi_get_geoms_ext( (char *)tableName, (char *)columnName, isGeodetic,
	//							mbrRequired? &(ba.p) : NULL, 
	//							countRequired? &count : NULL );

    FdoPtr<FdoISQLCommand> selCmd = (FdoISQLCommand*)mFdoConnection->CreateCommand( FdoCommandType_SQLCommand );

    // Apparently strait select is 3x faster.
    //    FdoStringP sql = FdoStringP::Format(L"select %ls.STEnvelope().STAsBinary() as MBR from %ls", colNameW, tableNameW);
    FdoStringP sql = FdoStringP::Format(L"select %ls.STAsBinary() as MBR from %ls", colNameW, tableNameW);

    selCmd->SetSQLStatement( (FdoString *)sql );
    FdoPtr<FdoISQLDataReader>  rdr = selCmd->ExecuteReader();

	long    count = 0;
    double  minX = 0;
    double  minY = 0;
    double  maxX = 0;
    double  maxY = 0;

    FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();

    while (rdr->ReadNext())
    {
        if (!rdr->IsNull(L"MBR"))
        {
            FdoPtr<FdoByteArray> ba = rdr->GetGeometry(L"MBR");
            FdoPtr<FdoIGeometry> geom = gf->CreateGeometryFromFgf(ba);

            FdoPtr<FdoIEnvelope>  envelope = geom->GetEnvelope();
            if ( count == 0 )
            {
                minX = envelope->GetMinX();
                minY = envelope->GetMinY();
                maxX = envelope->GetMaxX();
                maxY = envelope->GetMaxY();
            }
            else
            {
                minX = FdoCommonMin(envelope->GetMinX(), minX);
                minY = FdoCommonMin(envelope->GetMinY(), minY);
                maxX = FdoCommonMax(envelope->GetMaxX(), maxX);
                maxY = FdoCommonMax(envelope->GetMaxY(), maxY);          
            }
        }

        count++;
    }
    
    FdoIGeometry	*geom = NULL;

    if (count != 0 )
    {
        double ordinates[5 * 2]; // 5 2D points
        int     i = 0;
        ordinates[i++] = minX;
        ordinates[i++] = minY;

        ordinates[i++] = minX;
        ordinates[i++] = maxY;

        ordinates[i++] = maxX;
        ordinates[i++] = maxY;

        ordinates[i++] = maxX;
        ordinates[i++] = minY;

        ordinates[i++] = minX;
        ordinates[i]   = minY;

        FdoPtr<FdoILinearRing> ring = gf->CreateLinearRing(FdoDimensionality_XY, // geometry->GetDimensionality() ??
                                                    10, ordinates);

        geom = gf->CreatePolygon(ring, NULL);
    }

	if ( countRequired )
		m_Count = count;

	if ( mbrRequired )
		m_Extents = (FdoIPolygon *)geom;
}

FdoRdbmsSqlServerOptimizedAggregateReader::~FdoRdbmsSqlServerOptimizedAggregateReader()
{
	// Clean up
	for ( size_t i = 0; i < m_SelAggrList->size(); i++ )
		delete m_SelAggrList->at(i);
	delete m_SelAggrList;
}

void FdoRdbmsSqlServerOptimizedAggregateReader::Dispose()
{
    delete this;
}

bool FdoRdbmsSqlServerOptimizedAggregateReader::ReadNext()
{
    m_ReaderIndex++;
    return (m_ReaderIndex==0);
}

void FdoRdbmsSqlServerOptimizedAggregateReader::Close( )
{
}

bool FdoRdbmsSqlServerOptimizedAggregateReader::IsNull( FdoString *idName )
{
	bool	ret = false;

	FdoInt32  index = Validate( idName );

	if (m_SelAggrList->at(index)->type == FdoPropertyType_GeometricProperty )
		ret = ( m_Extents == NULL );

	return ret;
}

// Retrieve data values; these are guaranteed to only be called once per row of data:
FdoByteArray * FdoRdbmsSqlServerOptimizedAggregateReader::GetGeometry( FdoString *idName )
{
	FdoInt32  index = Validate( idName );

	if (m_SelAggrList->at(index)->type != FdoPropertyType_GeometricProperty )
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_INVALID_INPUT_ON_CLASS_FUNCTION),
                                                        L"FdoRdbmsSqlServerOptimizedAggregateReader::GetInt64",
														L"idName"));

    FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
    FdoByteArray* geomFGF = !m_Extents ? NULL : gf->GetFgf(m_Extents);

	return geomFGF;
}

FdoInt64 FdoRdbmsSqlServerOptimizedAggregateReader::GetInt64( FdoString* idName )
{
	FdoInt32  index = Validate( idName );

	if (m_SelAggrList->at(index)->type == FdoPropertyType_GeometricProperty )
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_INVALID_INPUT_ON_CLASS_FUNCTION),
                                                        L"FdoRdbmsSqlServerOptimizedAggregateReader::GetInt64",
														L"idName"));
    return m_Count;
}

FdoDouble FdoRdbmsSqlServerOptimizedAggregateReader::GetDouble( FdoString* idName )
{
    return (FdoDouble)GetInt64( idName );
}

FdoInt32 FdoRdbmsSqlServerOptimizedAggregateReader::Validate( FdoString* idName )  // throw exception if name is invalid
{
	bool		found = false;
	FdoInt32	index;

	for ( FdoInt32 i = 0; i < (FdoInt32)m_SelAggrList->size() && !found; i++ )
	{
		found = ( wcscmp(m_SelAggrList->at(i)->name, idName ) == 0 );
		index = i;
	}

	if ( !found )
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_INVALID_INPUT_ON_CLASS_FUNCTION),
                                                        L"FdoRdbmsSqlServerOptimizedAggregateReader::Validate",
														L"idName"));
	return index;
}

FdoInt32 FdoRdbmsSqlServerOptimizedAggregateReader::GetPropertyCount()
{
	return (FdoInt32)m_SelAggrList->size();
}

FdoString* FdoRdbmsSqlServerOptimizedAggregateReader::GetPropertyName(FdoInt32 index)
{
	return m_SelAggrList->at(index)->name;
}

FdoDataType FdoRdbmsSqlServerOptimizedAggregateReader::GetDataType(FdoString* idName)
{
	FdoInt32  index = Validate( idName );

	FdoPropertyType propType = m_SelAggrList->at(index)->type; 

	if ( propType == FdoPropertyType_GeometricProperty )
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_INVALID_INPUT_ON_CLASS_FUNCTION),
                                                        L"FdoRdbmsSqlServerOptimizedAggregateReader::GetDataType",
														L"idName"));

	// Count()
	return FdoDataType_Double;
}

FdoPropertyType FdoRdbmsSqlServerOptimizedAggregateReader::GetPropertyType(FdoString* idName)
{
	FdoInt32  index = Validate( idName );

	return m_SelAggrList->at(index)->type; 
}
