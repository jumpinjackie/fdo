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
#ifndef SHPSCROLLABLEFEATUREREADER_H
#define SHPSCROLLABLEFEATUREREADER_H

#include "SHP/ShpCompareHandler.h"
#include <FdoCommonThreadMutex.h>

// Some local types used for sorting
typedef struct _DataProperty_ {
	FdoByte			type;  //FdoDataType and allow for -1
	union {
		FdoInt32	intVal;
		FdoFloat	fltVal;
		FdoDateTime* dateVal;
		wchar_t*	strVal;
	} value;
} DataPropertyDef;

typedef struct _PropertyStub
{
    wchar_t*		m_name;
    FdoDataType		m_dataType;
} PropertyStub;

typedef struct _SortContext_ {
	int					propCount;
	FdoOrderingOption	*options;
	FdoString			**names;
	ShpCompareHandler	*compareHandler;
	PropertyStub		*propStubs;
} SortContextDef;

typedef struct _SortElement_ {
	REC_NO			index;	// featid zero-based
	DataPropertyDef	**propCache;
} SortElementDef;


#define	SHP_NULL_VALUE_TYPE		255 // greater than any FdoDataType

class ShpScrollableFeatureReader : public ShpFeatureReader
{
	friend class ShpImpExtendedSelect;

public:
    // constructs an ShpFeatureReader using the specified
    // connection, class definition, and Shp reader
    FDOSHP_API ShpScrollableFeatureReader( ShpConnection* connection, FdoString* className, FdoFilter* filter, SortContextDef *ctx, 
											SortElementDef  *sortedTable, bool useTableIndex, unsigned int tableSize, int numProps );

public:

	FDOSHP_API virtual int Count();

	FDOSHP_API virtual bool ReadFirst();

	FDOSHP_API virtual bool ReadLast();

    // Advances the reader to the next item and returns true if there is
    // another object to read or false if reading is complete.  The default
    // position of the reader is prior to the first item.  Therefore you
    // must call ReadFirst or ReadLast to begin accessing any data.
    FDOSHP_API virtual bool ReadNext();

	// Advances the reader to the previous item and returns true if there is
    // another object to read or false if reading is complete.  The default
    // position of the reader is prior to the first item.  Therefore you
    // must call ReadFirst or ReadLast to begin accessing any data.
    FDOSHP_API virtual bool ReadPrevious();

	FDOSHP_API virtual bool ReadAt(FdoPropertyValueCollection* key);

	FDOSHP_API virtual bool  ReadAtIndex( REC_NO recordindex );

	FDOSHP_API REC_NO IndexOf(FdoPropertyValueCollection* key);

protected:
    // default destructor
    virtual ~ShpScrollableFeatureReader();

    // dispose this object
    virtual void Dispose();

private:
	bool	GetData();
	REC_NO	FindRowidByFeatNum( REC_NO featid );
	REC_NO	SearchSortedTable( SortContextDef *ctx, SortElementDef *sortedTable, unsigned int tableSize, PropertyStub* propStubs, int numProps, FdoPropertyValueCollection* keyVals, REC_NO *featid);
	REC_NO	SearchNewSortedTable( FdoPropertyValueCollection *keyVals, REC_NO *featid );

private:
	FdoFilter*				m_Filter;
	SortContextDef			*m_Ctx;
	SortElementDef			*m_SortedTable;
	bool					m_UseTableIndex;
	bool					m_IsFeatidQuery;
	FdoOrderingOption		m_FeatidQueryOpt;
	unsigned int			m_TableSize;
	int						m_CurrentIndex;

	PropertyStub			*m_PropStubs;
	int						m_NumProps;

};

// Static context needed by qsort Compare() on Linux
#ifndef _WIN32
static	FdoCommonThreadMutex SortMutex;
static	SortContextDef	*GlobalSortCtx;
#endif

// qsort compare function
#ifdef _WIN32
static int compare(void *ctxCompare, const void * lhs, const void * rhs )
#else
static int compare(const void * lhs, const void * rhs )
#endif
{
	int retcode = 0;

	SortElementDef  *elem1 = (SortElementDef*)lhs;
	SortElementDef  *elem2 = (SortElementDef*)rhs;
	SortContextDef  *ctx;
#ifndef _WIN32
	ctx = GlobalSortCtx;
#else
	ctx = (SortContextDef *)ctxCompare;
#endif

	for(int i=0; i<ctx->propCount && retcode==0; i++ )
	{
		// Deal with null values (they come last in ascending ordered collection  
		if( elem1->propCache[i]->type == SHP_NULL_VALUE_TYPE || elem2->propCache[i]->type == SHP_NULL_VALUE_TYPE )
		{
			if( elem1->propCache[i]->type == SHP_NULL_VALUE_TYPE && elem2->propCache[i]->type == SHP_NULL_VALUE_TYPE )
				retcode = 0;
			else if ( elem1->propCache[i]->type == SHP_NULL_VALUE_TYPE )
				retcode = 1;
			else
				retcode = -1;

			if( ctx->options[i] == FdoOrderingOption_Descending )
				retcode *= -1; 

			continue;
		}

		FdoDataType	dataType =  ctx->propStubs[i].m_dataType;
		switch( dataType )
		{
			case FdoDataType_DateTime :
				retcode = ctx->compareHandler->Compare( ctx->names[i], *elem1->propCache[i]->value.dateVal, *elem2->propCache[i]->value.dateVal);
				break;
		  
			case FdoDataType_Decimal :
			case FdoDataType_Single : 
			case FdoDataType_Double :
				retcode = ctx->compareHandler->Compare( ctx->names[i], elem1->propCache[i]->value.fltVal, elem2->propCache[i]->value.fltVal);
				break;

			case FdoDataType_Boolean : 
			case FdoDataType_Byte :
			case FdoDataType_Int16 : 
			case FdoDataType_Int32 : 
				retcode = ctx->compareHandler->Compare( ctx->names[i], elem1->propCache[i]->value.intVal, elem2->propCache[i]->value.intVal);
				break;
			
			case FdoDataType_String : 
				retcode = ctx->compareHandler->Compare( ctx->names[i], elem1->propCache[i]->value.strVal, elem2->propCache[i]->value.strVal);
				break;

			default:
				throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_71_DATA_TYPE_NOT_SUPPORTED), FdoCommonMiscUtil::FdoDataTypeToString(dataType)));
				break;
		}
		if( ctx->options[i] == FdoOrderingOption_Descending )
			retcode *= -1; 
	}

    return retcode;
}


#endif
