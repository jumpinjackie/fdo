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
	bool	isFeatIdQuery = false;
	bool	doSorting = ( m_orderingProperties->GetCount() > 0 );

	// Special case, optimized: FeatId as ordering property
	bool	isFeatidQuery = m_orderingProperties->GetCount() == 1 && ( wcscmp(m_orderingProperties->GetItem(0)->GetName(), L"FeatId") == 0 );

	// Do regular select in order to collect data
	FdoPtr<ShpFeatureReader>	reader = (ShpFeatureReader*)ShpSelectCommand::Execute();

	// We do fetches also in the case of a filter
	bool	doFetches = doSorting || (this->GetFilter() != NULL );

	// Set a flag on the reader not to read the SHP file. Unless the reader tells otherwise.
	if ( isFeatidQuery && reader->mIsFeatIdQuery )
		reader->SetFetchGeometry( false );

	// Set a flag on the reader to return deleted row as well
	reader->SetFetchDeletes( true );

	if( reader == NULL || !reader->ReadNext() )
		return NULL;
	
	// Optimize memory in the presence of a filter: use the reader to get the size of the featid list
	int maxsize = reader->mMaxNumObjects;

	// Not available when not a feaid query 
	if ( maxsize == 0 )
		maxsize = reader->mFileSet->GetShapeIndexFile ()->GetNumObjects ();

	PropertyStub		*propStubs = NULL;

	// Create and initialize a sorting context.
	SortContextDef		*ctx = new SortContextDef;
	memset (ctx, '\0', sizeof(SortContextDef));

	// It may be needed regardless no ordering option
	ctx->compareHandler = m_compareHandler;

	if ( doFetches )
	{
		ctx->compareHandler = m_compareHandler;

		// Count the number of properties
		if ( m_orderingProperties->GetCount() != 0 )
			ctx->propCount = m_orderingProperties->GetCount();
		else if (this->GetFilter() != NULL )
			ctx->propCount = 1;

		ctx->options = new FdoOrderingOption[ctx->propCount];
		ctx->names = new FdoString*[ctx->propCount];

		// Create properties stubs to save processing time 
		ColumnInfo* infoCols = reader->mData->GetColumnInfo ();
		int	numCols = infoCols->GetNumColumns ();

		// Get the names of the ordered properties. Create properties stubs.
		propStubs = new PropertyStub[ctx->propCount];

		if ( m_orderingProperties->GetCount() != 0 )
		{
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
		}
		else if (this->GetFilter() != NULL )
		{
			// Populate the table elements with FeatId as property.
			propStubs[0].m_name = new wchar_t[wcslen(L"FeatId")+1];
			wcscpy( (wchar_t*)propStubs[0].m_name, L"FeatId" );
			propStubs[0].m_dataType = FdoDataType_Int32;

			// Populate the ctx with sane values
			ctx->options[0] = GetOrderingOption( L"FeatId" );
			ctx->names[0] = new wchar_t[wcslen(L"FeatId")+1];
			wcscpy( (wchar_t*)ctx->names[0], L"FeatId" );

			isFeatIdQuery = reader->mIsFeatIdQuery;
		}
	}

	ctx->propStubs = propStubs;

	SortElementDef  *sortedTable = NULL;

	// Allow for an optimization: in case there is no filtering and the ordering property is just "FeatId",
	// then we don't need reading the files since the Featids are consecutive (including the deletes).
	if ( !doFetches && isFeatidQuery )
	{
		doSorting = false;	
	}

	// Optimize Case 1: no ordering property, no filter: No need to fetch, return a reader
	if ( !doFetches )
	{
		// The sorting context will be freed by the reader
		return new ShpImpScrollableFeatureReader<ShpScrollableFeatureReader>(new ShpScrollableFeatureReader( 
																				mConnection, mClassName->GetText(), GetFilter(),
																				ctx, NULL, false, maxsize, 0 ) );
	}

	// Optimize Case 2: Featid ordering property, no filter: No need to fetch, return a reader
	if ( isFeatidQuery && (this->GetFilter() == NULL) )
	{
		// The sorting context will be freed by the reader
		return new ShpImpScrollableFeatureReader<ShpScrollableFeatureReader>(new ShpScrollableFeatureReader( 
																				mConnection, mClassName->GetText(), GetFilter(),
																				ctx, NULL, true, maxsize, 0 ) );
	}

	sortedTable = new SortElementDef[maxsize];

	// Initialize the ordering property cache (first read done above).
	i = 0;

	do
	{		
		SortElementDef  *pRow = &sortedTable[i];

		pRow->propCache = new DataPropertyDef*[ctx->propCount];

		// This works both for filtered and not filtered
		pRow->index = reader->GetInt32(L"FeatId") - 1;

		// This is skipped if !doSorting
		for( int j = 0; j < ctx->propCount; j++ )
		{
			PropertyStub  ps = propStubs[j];

			// Initialize a property 
			pRow->propCache[j] = new DataPropertyDef;
			DataPropertyDef *pProp = pRow->propCache[j];

			pProp->type = ps.m_dataType;
			FdoString *pName = ps.m_name;

			if ( reader->IsNull( pName ) )
			{
				pProp->type = SHP_NULL_VALUE_TYPE;
				continue;
			}

			switch( ps.m_dataType )
			{
				case FdoDataType_Boolean : 
				case FdoDataType_Byte : 
					pProp->value.intVal = (int)reader->GetByte( pName );
					break;

				case FdoDataType_DateTime :
					pProp->value.dateVal = new FdoDateTime();
					*pProp->value.dateVal = reader->GetDateTime( pName );
					break;

				case FdoDataType_Decimal :		  
				case FdoDataType_Double :
					pProp->value.fltVal = (float)reader->GetDouble( pName );
					break;

				case FdoDataType_Int16 : 
					pProp->value.intVal = reader->GetInt16( pName );
					break;

				case FdoDataType_Int32 : 
					pProp->value.intVal = reader->GetInt32( pName );
					break;

				case FdoDataType_Single :
					pProp->value.fltVal = reader->GetSingle( pName );
					break;

				case FdoDataType_String : 
				{
					FdoString*	tmpStr = reader->GetString( pName );
					pProp->value.strVal = new wchar_t[wcslen(tmpStr)+1];
					wcscpy( pProp->value.strVal , tmpStr );
					break;
				}
			
			  default:
				  throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_71_DATA_TYPE_NOT_SUPPORTED), FdoCommonMiscUtil::FdoDataTypeToString(ps.m_dataType)));
				  break;
			}
		}
		
        i++;

		// get the next record
	} while (reader->ReadNext());

	// Adjust the size (account for deleted records)
	maxsize = i;

	// Reset the flags.
	reader->SetFetchGeometry( true );
	reader->SetFetchDeletes( false );

	// Build the sorted list
	if ( doSorting ) {
#ifdef _WIN32
		qsort_s( (void*)sortedTable, maxsize, sizeof(SortElementDef), compare, (void *)ctx);
#else
	    SortMutex.Enter();

		GlobalSortCtx = ctx;
		qsort( (void*)sortedTable, maxsize, sizeof(SortElementDef), compare);

		SortMutex.Leave();
#endif
	}

	bool	useTableIndex = doFetches;
							
	// The sorted table and sorting context will be freed by the reader
	return new ShpImpScrollableFeatureReader<ShpScrollableFeatureReader>(new ShpScrollableFeatureReader( 
																				mConnection, mClassName->GetText(), GetFilter(),
																				ctx, sortedTable, 
																				useTableIndex, 
																				maxsize,
																				m_orderingProperties->GetCount()) );
}


