// 
//  
//  Copyright (C) 2004-2006  Autodesk, Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//  
#include <iostream>
#include <stdlib.h> 
#include <algorithm>
#include "stdafx.h"
#include "ShpImpExtendedSelect.h"
#include "ShpConnection.h"
#include "ShpQueryOptimizer.h"
#include "ShpScrollableFeatureReader.h"
#include "FdoCommonSchemaUtil.h"
#include "ShpImpScrollableFeatureReader.h"


//-------------------------------------------------------
// Constructor / destructor
//-------------------------------------------------------
// constructs a Select command using the specified connection
ShpImpExtendedSelect::ShpImpExtendedSelect(ShpConnection* connection)
: ShpSelectCommand(connection)
{
    m_orderingOption = FdoOrderingOption_Ascending;
	m_orderingProperties = FdoIdentifierCollection::Create();
	m_compareHandler = ShpCompareHandler::Create();
	m_orderingOptions = new std::map<std::wstring, int>();
}

// default constructor
ShpImpExtendedSelect::ShpImpExtendedSelect()
: ShpSelectCommand(NULL)
{
	m_orderingOption = FdoOrderingOption_Ascending;
	m_orderingOptions = new std::map<std::wstring, int>();
}

// default destructor
ShpImpExtendedSelect::~ShpImpExtendedSelect()
{
	if( m_orderingOptions )
		delete m_orderingOptions;
}

void ShpImpExtendedSelect::SetOrderingOption( FdoString* propertyName, FdoOrderingOption  option )
{
	(*m_orderingOptions)[propertyName] = (int)option;
}

FdoOrderingOption ShpImpExtendedSelect::GetOrderingOption( FdoString* propertyName )
{
	std::map<std::wstring, int>::iterator iter = m_orderingOptions->find( propertyName );
	if( iter == m_orderingOptions->end() )
		return FdoOrderingOption_Ascending; // default
	return (FdoOrderingOption) iter->second;
}

void ShpImpExtendedSelect::ClearOrderingOptions()
{
	m_orderingOptions->clear();
}

FdoIdentifierCollection* ShpImpExtendedSelect::GetOrdering()
{
	return FDO_SAFE_ADDREF(m_orderingProperties.p);
}

void ShpImpExtendedSelect::SetCompareHandler( ShpCompareHandler*  handler )
{
	m_compareHandler = FDO_SAFE_ADDREF(handler);
}


ShpIScrollableFeatureReader* ShpImpExtendedSelect::ExecuteScrollable()
{
	int		i = 0;
	bool	doSorting = ( m_orderingProperties->GetCount() > 0 );

	// Do regular select in order to collect data
	FdoPtr<ShpFeatureReader>	reader = (ShpFeatureReader*)ShpSelectCommand::Execute();

	// Set a flag on the reader not to read the SHP file.
	reader->SetFetchGeometry( false );
	if( reader == NULL || !reader->ReadNext() )
		return NULL;
	
	// TODO: Optimize memory, use reader->mFeatIdFilterExecutor to get the size of the featid list
	int maxsize = reader->mFileSet->GetShapeIndexFile ()->GetNumObjects ();
	
	PropertyStub		*propStubs = NULL;

	// Create and initialize a sorting context.
	SortContextDef		*ctx = new SortContextDef;
	memset (ctx, '\0', sizeof(SortContextDef));

	// It may be needed regardless no ordering option
	ctx->compareHandler = m_compareHandler;

	if ( doSorting )
	{
		ctx->compareHandler = m_compareHandler;
		ctx->propCount = m_orderingProperties->GetCount();
		ctx->options = new FdoOrderingOption[ctx->propCount];
		ctx->names = new FdoString*[ctx->propCount];

		// Create properties stubs to save processing time 
		ColumnInfo* infoCols = reader->mData->GetColumnInfo ();
		int	numCols = infoCols->GetNumColumns ();

		// Get the names of the ordered properties. Create properties stubs.
		propStubs = new PropertyStub[ctx->propCount];

		for( i = 0; i < ctx->propCount; i++ )
		{
			FdoPtr<FdoIdentifier> id = m_orderingProperties->GetItem( i );

			// Need the property type. Get it from the reader.
			propStubs[i].m_name = new wchar_t[wcslen(id->GetName())+1];
			wcscpy( (wchar_t*)propStubs[i].m_name, id->GetName() );

			if ( wcscmp( id->GetName(), reader->mLogicalIdentityPropertyName ) == 0 )
			{
				propStubs[i].m_dataType = FdoDataType_Int32;
			}
			else
			{
				for ( int j = 0; j < numCols; j++ )
				{
					if ( wcscmp( infoCols->GetColumnNameAt(j), id->GetName() ) == 0 )
					{
						propStubs[i].m_dataType = ShpSchemaUtilities::DbfTypeToFdoType(infoCols->GetColumnTypeAt(j));
						break;
					}
				}
			}

			ctx->options[i] = GetOrderingOption( id->GetName() );
			ctx->names[i] = new wchar_t[wcslen(id->GetName())+1];
			wcscpy( (wchar_t*)ctx->names[i], id->GetName() );
		}		
		
		// initialize the ordering property cache
		ctx->propCache = new DataPropertyDef*[maxsize];
	}

	// Populate the array to be sorted with actual data. 
	ctx->featIds = new REC_NO[maxsize];
	SortElementDef  *sortedTable = new SortElementDef[maxsize];

	// First read done above
	i = 0;
	bool	hasMore = true;

	while( hasMore )
	{
		REC_NO featid = reader->mFeatureNumber; // Zero-based

	    sortedTable[i].index = i;
	    sortedTable[i].ctx = ctx;
	    ctx->featIds[i]= featid;

		if ( doSorting )
			ctx->propCache[i] = new DataPropertyDef[ctx->propCount];

		// This is skipped if !doSorting
	    for( int j = 0; j < ctx->propCount; j++ )
	    {
		    PropertyStub  ps = propStubs[j];

		    ctx->propCache[i][j].type = ps.m_dataType;
			FdoString *pName = ps.m_name;

			if ( reader->IsNull( pName ) )
			{
				ctx->propCache[i][j].type = -1;
				continue;
			}

		    switch( ps.m_dataType )
		    {
				case FdoDataType_Boolean : 
				case FdoDataType_Byte : 
					ctx->propCache[i][j].value.intVal = (int)reader->GetByte( pName );
					break;

				case FdoDataType_DateTime :
					ctx->propCache[i][j].value.dateVal = new FdoDateTime();
					*ctx->propCache[i][j].value.dateVal = reader->GetDateTime( pName );
					break;

				case FdoDataType_Decimal :		  
				case FdoDataType_Double :
					ctx->propCache[i][j].value.dblVal = reader->GetDouble( pName );
					break;

				case FdoDataType_Int16 : 
					ctx->propCache[i][j].value.intVal = reader->GetInt16( pName );
					break;

				case FdoDataType_Int32 : 
					ctx->propCache[i][j].value.intVal = reader->GetInt32( pName );
					break;

				case FdoDataType_Int64 : 
					ctx->propCache[i][j].value.int64Val = reader->GetInt64( pName );
					break;

				case FdoDataType_Single :
					ctx->propCache[i][j].value.dblVal = reader->GetSingle( pName );
					break;

				case FdoDataType_String : 
				{
					FdoString*	tmpStr = reader->GetString( pName );
					ctx->propCache[i][j].value.strVal = new wchar_t[wcslen(tmpStr)+1];
					wcscpy( ctx->propCache[i][j].value.strVal , tmpStr );
					break;
				}
			
		      default:
                  throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_71_DATA_TYPE_NOT_SUPPORTED), FdoCommonMiscUtil::FdoDataTypeToString(ps.m_dataType)));
			      break;
		    }
	    }
        i++;

		// get the next record
		hasMore = reader->ReadNext();
	}
	maxsize = i;

	// Reset the flag.
	reader->SetFetchGeometry( true );

	// Build the sorted list
	if ( doSorting )
		qsort( (void*)sortedTable, maxsize, sizeof(SortElementDef), compare );

	// The sorted table and sorting context will be freed by the reader
	return new ShpImpScrollableFeatureReader<ShpScrollableFeatureReader>(new ShpScrollableFeatureReader( 
																				mConnection, mClassName->GetText(), GetFilter(),
																				sortedTable, maxsize, 
																				propStubs, m_orderingProperties->GetCount()) );
}


