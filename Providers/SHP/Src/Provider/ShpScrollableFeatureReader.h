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
	int				type; // FdoDataType and allow for -1
	union {
		FdoInt32	intVal;
		FdoInt64	int64Val;
		double		dblVal;
		FdoDateTime* dateVal;
		wchar_t*	strVal;
	} value;
} DataPropertyDef;

typedef struct _SortContext_ {
	REC_NO				*featIds;
	int					propCount;
	FdoOrderingOption	*options;
	FdoString			**names;
	ShpCompareHandler	*compareHandler;
} SortContextDef;

typedef struct _PropertyStub
{
    wchar_t*		m_name;
    int				m_recordIndex;
    FdoDataType		m_dataType;
	FdoPropertyType m_propertyType;
    bool			m_isAutoGen;
} PropertyStub;

typedef struct _SortElement_ {
	REC_NO			index;
	DataPropertyDef	**propCache;
} SortElementDef;



class ShpScrollableFeatureReader : public ShpFeatureReader
{
	friend class ShpImpExtendedSelect;

public:
    // constructs an ShpFeatureReader using the specified
    // connection, class definition, and Shp reader
    FDOSHP_API ShpScrollableFeatureReader( ShpConnection* connection, FdoString* className, FdoFilter* filter, SortContextDef *ctx, 
											SortElementDef  *sortedTable, unsigned int tableSize, 
											PropertyStub*	props, int numProps );

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

	FDOSHP_API virtual bool  ReadAtIndex( unsigned int recordindex );

	FDOSHP_API unsigned int IndexOf(FdoPropertyValueCollection* key);

protected:
    // default destructor
    virtual ~ShpScrollableFeatureReader();

    // dispose this object
    virtual void Dispose();

	//static int compare(const void * lhs, const void * rhs );

private:
	void	GetData();
	REC_NO	FindRowidByFeatNum( REC_NO featid );
	REC_NO	SearchSortedTable( SortContextDef *ctx, SortElementDef *sortedTable, unsigned int tableSize, PropertyStub* propStubs, int numProps, FdoPropertyValueCollection* keyVals);
	REC_NO	SearchNewSortedTable( FdoPropertyValueCollection *keyVals );

private:
	FdoFilter*				m_Filter;
	SortContextDef			*m_Ctx;
	SortElementDef			*m_SortedTable;
	unsigned int			m_TableSize;
	int						m_CurrentIndex;

	PropertyStub			*m_PropStubs;
	int						m_NumProps;

};

// Static context needed by qsort Compare()
static	FdoCommonThreadMutex SortMutex;
static	SortContextDef	*GlobalSortCtx;

// qsort compare function
static int compare(const void * lhs, const void * rhs )
{
	int retcode = 0;

	SortElementDef  *elem1 = (SortElementDef*)lhs;
	SortElementDef  *elem2 = (SortElementDef*)rhs;

	SortContextDef  *ctx = GlobalSortCtx;

	for(int i=0; i<ctx->propCount && retcode==0; i++ )
	{
		// Deal with null values (they come last in ascending ordered collection  
		if( elem1->propCache[i]->type == -1 || elem2->propCache[i]->type == -1 )
		{
			if( elem1->propCache[i]->type == -1 && elem2->propCache[i]->type == -1 )
				retcode = 0;
			else if ( elem1->propCache[i]->type == -1 )
				retcode = 1;
			else
				retcode = -1;

			if( ctx->options[i] == FdoOrderingOption_Descending )
				retcode *= -1; 

			continue;
		}

		FdoDataType	dataType = (FdoDataType)elem2->propCache[i]->type;
		switch( dataType )
		{
			case FdoDataType_DateTime :
				retcode = ctx->compareHandler->Compare( ctx->names[i], *elem1->propCache[i]->value.dateVal, *elem2->propCache[i]->value.dateVal);
				break;
		  
			case FdoDataType_Decimal :
			case FdoDataType_Single : 
			case FdoDataType_Double :
				retcode = ctx->compareHandler->Compare( ctx->names[i], elem1->propCache[i]->value.dblVal, elem2->propCache[i]->value.dblVal);
				break;

			case FdoDataType_Boolean : 
			case FdoDataType_Byte :
			case FdoDataType_Int16 : 
			case FdoDataType_Int32 : 
				retcode = ctx->compareHandler->Compare( ctx->names[i], elem1->propCache[i]->value.intVal, elem2->propCache[i]->value.intVal);
				break;

			case FdoDataType_Int64 : 
				retcode = ctx->compareHandler->Compare( ctx->names[i], elem1->propCache[i]->value.int64Val, elem2->propCache[i]->value.int64Val);
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
