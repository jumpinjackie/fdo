#ifndef _FDORDBMSFILTERPROCESSOR_
#define _FDORDBMSFILTERPROCESSOR_ 1
/*
 * Copyright (C) 2004-2006  Autodesk, Inc.
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
#ifdef _WIN32
#pragma once
#endif
// Filter includes
#include "Fdo/Filter/BinaryLogicalOperator.h"
#include "Fdo/Filter/ComparisonCondition.h"
#include "Fdo/Filter/DistanceCondition.h"
#include "Fdo/Filter/InCondition.h"
#include "Fdo/Filter/NullCondition.h"
#include "Fdo/Filter/SpatialCondition.h"
#include "Fdo/Filter/UnaryLogicalOperator.h"

// Expression includes
#include "Fdo/Expression/BinaryExpression.h"
#include "Fdo/Expression/UnaryExpression.h"
#include "Fdo/Expression/Function.h"
#include "Fdo/Expression/Identifier.h"
#include "Fdo/Expression/Parameter.h"
#include "Fdo/Expression/DataValue.h"
#include "Fdo/Expression/GeometryValue.h"
#include <vector>

#include <Geometry/GeometryStd.h>

using namespace std ;

#include "DbiConnection.h"
#include "FdoRdbmsBaseFilterProcessor.h"

class FdoRdbmsConnection;
class FdoRdbmsSpatialSecondaryFilter;


// To bracket various condition and expressions
#define             OPEN_PARENTH                    L" ( "
#define             CLOSE_PARENTH                   L"  ) "

// For logical operations
#define             LOGICAL_AND                     L" AND "
#define             LOGICAL_OR                      L" OR "
#define             LOGICAL_NOT                     L" NOT "

// For comparison operation
#define             EQUAL_OP                        L" = "
#define             NOT_EQUAL_OP                    L" <> "
#define             GREATER_THAN_OP                 L" > "
#define             GREATER_OR_EQUAL_OP             L" >= "
#define             LESS_THAN_OP                    L" < "
#define             LESS_OR_EQUAL_OP                L" <= "
#define             LIKE_OP                         L" LIKE "

// For arthmetic operation
#define             ARITHMETIC_PLUS                 L" + "
#define             ARITHMETIC_MINUS                L" - "
#define             ARITHMETIC_MULT                 L" * "
#define             ARITHMETIC_DIV                  L" / "
#define             ARITHMETIC_MOD                  L" % "

#define     MEM_BLOCK_ALLOC_SIZE    128

enum SqlCommandType
{
    SqlCommandType_Select,
    SqlCommandType_Delete,
    SqlCommandType_Update
};

//
// Used to provide additional filter constrains such as distinct orderby ...
class FdoRdbmsFilterUtilConstrainDef {
public:
    FdoRdbmsFilterUtilConstrainDef()
    {
        distinct = false;
        orderingOption = FdoOrderingOption_Ascending;
        selectedProperties = orderByProperties = groupByProperties = NULL;
    }
    bool                    distinct;
    FdoOrderingOption       orderingOption;
    FdoIdentifierCollection *selectedProperties;
    FdoIdentifierCollection *orderByProperties;
    FdoIdentifierCollection *groupByProperties;
};

class FdoRdbmsSecondarySpatialFilterCollection : public FdoCollection<FdoRdbmsSpatialSecondaryFilter, FdoException>
{
public:
    static FdoRdbmsSecondarySpatialFilterCollection* Create()
    {
        FdoRdbmsSecondarySpatialFilterCollection * newCollection =
            new FdoRdbmsSecondarySpatialFilterCollection();
	    if (NULL == newCollection)
		    throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));
        return newCollection;
    }
protected:
    virtual void Dispose()  { delete this; }
};
typedef FdoPtr<FdoRdbmsSecondarySpatialFilterCollection> FdoRdbmsSecondarySpatialFilters;


class FdoRdbmsFilterProcessor:
    public FdoRdbmsBaseFilterProcessor
{
   friend class DbiConnection;

public:
    class BoundGeometry : public FdoIDisposable
    {
    public:
        BoundGeometry(
            FdoIGeometry* geometry,
            FdoInt64 srid
        );

        // Returns the name of this schema's owning user.
	    FdoIGeometry* GetGeometry();

        // Returns true if this is a system schema.
        FdoInt64 GetSrid();

    protected:
        /// unused constructor needed only to build on Linux
        BoundGeometry()
        {
        }

	    virtual ~BoundGeometry(void);

        virtual void Dispose()  { delete this; }

    private:
        FdoPtr<FdoIGeometry> mGeometry;
        FdoInt64 mSrid;
    };

    class BoundGeometryCollection : public FdoCollection<BoundGeometry, FdoException>
    {
    public:
	    BoundGeometryCollection()
	    {}
	    ~BoundGeometryCollection(void) {}

    protected:
        virtual void Dispose()  { delete this; }
    };

private:

    wchar_t*            mSqlFilterText;  // Buffer used to encode the filter SQL conversion
    size_t              mSqlTextSize;    // The size of the SQL buffer
    size_t              mFirstTxtIndex;  // The index of the start of the sql string
    size_t              mNextTxtIndex;   // The index of the next empty string
	short				mNextTabAliasId;
	bool				mUseTableAliases;

    // List of geometry values that are bound in spatial filters.
    FdoPtr<BoundGeometryCollection>       mBoundGeometryValues;

protected:
    bool                mRequiresDistinct; // Used in case a distinct clause is needed for the sql select string
    bool                mProcessingOrOperator;
    SqlCommandType      mCurrentCmdType; // Used to build the right sql command(select, delete, update)
    wchar_t*            mCurrentClassName; // Used to fetch the class properties


    typedef struct _filter_tabs_  {
        wchar_t   pk_TableName[GDBI_TABLE_NAME_SIZE];
		wchar_t	  pk_TabAlias[2];
        wchar_t   pk_ColumnName[GDBI_COLUMN_NAME_SIZE];
        wchar_t   fk_TableName[GDBI_TABLE_NAME_SIZE];
		wchar_t	  fk_TabAlias[2];
        wchar_t   fk_ColumnName[GDBI_COLUMN_NAME_SIZE];
        bool   useOuterJoin;
        bool   duplicatefkTable; // used to avoid adding the same table of the select multiple time as: select tab1.*,tab1.* ...
    } FilterTableRelationDef;

    vector<FilterTableRelationDef>  mCurrentTableRelationArray;

	vector<const FdoSmLpClassDefinition *>  mClassArray;

    FdoRdbmsConnection*  mFdoConnection;  // Dbi connection used by this filter

    // List of secondary spatial filters that apply to this filter.  
    FdoRdbmsSecondarySpatialFilters     mSecondarySpatialFilters;
	vector<int>							mFilterLogicalOps;

public:
    FdoRdbmsFilterProcessor();

    FdoRdbmsFilterProcessor(FdoRdbmsConnection *connection);
    virtual ~FdoRdbmsFilterProcessor(void);

private:

    void ProcessIdentifier(FdoIdentifier& expr, bool useOuterJoin );

    // Checks an identifier collection for the existance of aggregate functions.
    bool ContainsAggregateFunctions( FdoIdentifierCollection *identifiers );


protected:

    // This method is used to follow a value type object property or an m:1 association
    // and add the necessary column spec and table mappings for joining them later.
    void FollowRelation( FdoStringP    &relationColumns, const FdoSmLpPropertyDefinition* propertyDefinition, FdoIdentifierCollection *selectedProperties );

    const FdoSmLpDataPropertyDefinitionCollection* GetIdentityProperties(const wchar_t *className, const FdoSmLpClassDefinition **identClass );

    void PrependProperty( FdoIdentifier* property, bool scanForTableOnly=false );

    void PrependTables();

    // Add the order by clause if it's required
    void AppendOrderBy( FdoRdbmsFilterUtilConstrainDef *filterConstrain );

    // Add the group by clause if it's required
    void AppendGroupBy( FdoRdbmsFilterUtilConstrainDef *filterConstrain );

    virtual void ResetBuffer( SqlCommandType cmdType );

    void ReallocBuffer( size_t  extraSize , bool atEnd );

    const wchar_t* GetBuffer()
    {
        return &mSqlFilterText[mFirstTxtIndex];
    }

    void AppendString(const char *str);

    void PrependString(const char *str);

	void AppendString(const wchar_t *str);

    void PrependString(const wchar_t *str);

    virtual void AppendForUpdate() { AppendString(" FOR UPDATE"); } ;

    virtual void AppendTablesHints( SqlCommandType cmdType, bool forUpdate ) { };

    virtual void PrependSelectStar( const wchar_t *tableName);

    void AddNewTableRelation( const wchar_t *pkTab, const wchar_t *pkCol, const wchar_t *fkTab, const wchar_t *fkCol, bool userOuterJoin=false );

	void AddNewClass( const FdoSmLpClassDefinition *smLpClass );

    const wchar_t * PropertyNameToColumnName( const wchar_t *propName );


//public:

    //
    // Process expressions
    //
    virtual void ProcessBinaryExpression(FdoBinaryExpression& expr);

    virtual void ProcessUnaryExpression(FdoUnaryExpression& expr);

    virtual void ProcessFunction(FdoFunction& expr);
    virtual void ProcessFunctionName(FdoFunction& expr);

    virtual void ProcessComputedIdentifier(FdoComputedIdentifier& expr);

    virtual void ProcessIdentifier(FdoIdentifier& expr)
    {
        return ProcessIdentifier( expr, false );
    }

    virtual void ProcessParameter(FdoParameter& expr);

    // Data value processors
    virtual void ProcessBooleanValue(FdoBooleanValue& expr);

    virtual void ProcessByteValue(FdoByteValue& expr);

    virtual void ProcessDateTimeValue(FdoDateTimeValue& expr);

    virtual void ProcessDoubleValue(FdoDoubleValue& expr);

    virtual void ProcessDecimalValue(FdoDecimalValue& expr);

    virtual void ProcessInt16Value(FdoInt16Value& expr);

    virtual void ProcessInt32Value(FdoInt32Value& expr);

    virtual void ProcessInt64Value(FdoInt64Value& expr);

    virtual void ProcessSingleValue(FdoSingleValue& expr);

    virtual void ProcessStringValue(FdoStringValue& expr);

    virtual void ProcessBLOBValue(FdoBLOBValue& expr);

    virtual void ProcessCLOBValue(FdoCLOBValue& expr);

    virtual void ProcessGeometryValue(FdoGeometryValue& expr);


    //
    // Process Filters
    //
    virtual void ProcessBinaryLogicalOperator(FdoBinaryLogicalOperator& filter);

    virtual void ProcessComparisonCondition(FdoComparisonCondition& filter);

    virtual void ProcessInCondition(FdoInCondition& filter);

    virtual void ProcessNullCondition(FdoNullCondition& filter);

    virtual void ProcessUnaryLogicalOperator(FdoUnaryLogicalOperator& filter);

    virtual bool IsAggregateFunctionName(FdoString* wFunctionName) const = 0;
    virtual bool IsNotNativeSupportedFunction(FdoString* wFunctionName) const = 0;
    virtual bool HasNativeSupportedFunctionArguments(FdoFunction& expr) const = 0;
    virtual FdoStringP GetGeometryString( FdoString* columnName );

public:
	virtual void GetLtTableExpression( const FdoSmLpClassDefinition *classDefinition, FdoStringP &ltJoin, FdoStringP &ltTableExp, FdoCommandType callerFdoCommand );
	virtual void GetLtQualificationClause( const FdoSmLpClassDefinition *classDefinition, FdoStringP &ltQualificationClause );
    bool IsValidExpression( FdoFilter *filter );
    bool IsValidExpression( FdoIdentifierCollection *identifiers );

    bool  GetUseTableAlias() 
    {
        return mUseTableAliases;
    }

	void  SetUseTableAlias( bool useFlag ) { mUseTableAliases = useFlag; };

	const wchar_t* GetTableAlias( const wchar_t* tabName );

    bool CanOptimizeRelationQuery( const FdoSmLpClassDefinition* pClass, const FdoSmLpPropertyDefinition* propertyDefinition );

    virtual const wchar_t* FilterToSql( FdoFilter *filter,
                                        const wchar_t *className,
                                        SqlCommandType cmdType,
                                        FdoCommandType callerFdoCommand,
                                        FdoRdbmsFilterUtilConstrainDef *filterConstrain = NULL,
                                        bool forUpdate = false,
                                        FdoInt16 callerId = FdoCommandType_Select );
    virtual const wchar_t* FilterToSql( FdoFilter *filter,
                                        const wchar_t *className );

    FdoRdbmsSecondarySpatialFilterCollection * GetGeometricConditions() { return FDO_SAFE_ADDREF(mSecondarySpatialFilters.p); }
	
    vector<int> * GetFilterLogicalOps() { return &mFilterLogicalOps; }

    BoundGeometryCollection * GetBoundGeometryValues() { 
        if (mBoundGeometryValues == NULL)
            mBoundGeometryValues = new BoundGeometryCollection();

        return FDO_SAFE_ADDREF(mBoundGeometryValues.p); 
    }


};

#endif //_FDORDBMSFILTERPROCESSOR_
