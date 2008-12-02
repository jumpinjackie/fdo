#ifndef FdoRdbmsSchemaUtil_H
#define FdoRdbmsSchemaUtil_H    1
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

using namespace std ;

#include "DbiConnection.h"
#include "Fdo/Pvc/FdoRdbmsPvcOperation.h"
#include <Collection.h>

#include <Sm/Lp/ObjectPropertyClass.h>
#include <Sm/Lp/ObjectPropertyDefinition.h>

class FdoRdbmsSchemaUtil
{
    friend class DbiConnection;

private:

    FdoSchemaManagerP       mSchemaMgr;

    FdoRdbmsUtil*           mUtility;

    wchar_t*                mBuffer;
    size_t                  mBufferSize;
	bool					mSkipCheck;


    virtual void AppendProperties( const wchar_t* scope, const FdoSmLpClassDefinition* currentClass, bool bAddPrimaryKeys, bool bGetAutoGen, FdoPropertyValueCollection* propertyValues );

    bool IsCurveStringValid( FdoICurveString *curve, double tolerance );
    bool IsCurvePolygonValid( FdoICurvePolygon *curve, double tolerance );
    bool IsRingValid( FdoIRing *curve, double tolerance );

protected:
    FdoRdbmsSchemaUtil();

    FdoRdbmsSchemaUtil( DbiConnection *connection );

    ~FdoRdbmsSchemaUtil();

    FdoRdbmsUtil* GetUtility()
    {
        return mUtility;
    }

public:

    FdoSchemaManagerP GetSchemaManager() { return mSchemaMgr; }
    void SetSchemaManager( FdoSchemaManagerP schemaMgr )
    {
        mSchemaMgr = schemaMgr;
    }

	void SetSkipCheck( bool val ) { mSkipCheck = val; }

    wchar_t* makePropName( const wchar_t* scope, const wchar_t* name );
    const wchar_t *Property2ColName(const wchar_t *className, const wchar_t *attribute);

    virtual const wchar_t *ColName2Property(const wchar_t *className, const wchar_t *column);

    const wchar_t *GetTable(const wchar_t *className);

    virtual const char *MakeDBValidName(const wchar_t *name);

    const FdoSmLpSchema* GetSchema(const wchar_t *className);

    const FdoSmLpClassDefinition* GetClass(const wchar_t* className);

    FdoPropertyValueCollection* GetPropertyValues( const wchar_t* className, bool bGetAutoGen = false );

    void CheckClass(const wchar_t* className);

    // Given a class, returns its table name formated for a SQL statement
    // (Adds enclosing ANSI quotes if RDBMS supports them).
    // Throws exception if table is not in the RDBMS.
    virtual FdoStringP GetDbObjectSqlName( const FdoSmLpClassDefinition *classDefinition );

    // Given an object property, returns its table name formated for a SQL statement
    // (Adds enclosing ANSI quotes if RDBMS supports them.
    // Throws exception if table is not in the RDBMS.
    FdoStringP GetDbObjectSqlName( const FdoSmLpObjectPropertyDefinition *propertyDefinition );

    // Given a property, returns its column name formated for a SQL statement
    // (Adds enclosing ANSI quotes if RDBMS supports them.
    // Throws exception if column is not in the RDBMS.
    FdoStringP GetColumnSqlName( const FdoSmLpSimplePropertyDefinition *propertyDefinition );

    // Check the geometry property dimensionality against a geometry value
    void CheckGeomPropOrdDimensionality( const FdoSmLpClassDefinition *classDefinition,
                                      const wchar_t*    geomPropName,
                                      FdoIGeometry      *pGeomValue );

    // Check the geometry property shape type against a geometry value
    void CheckGeomPropShapeType( const FdoSmLpClassDefinition *classDefinition,
                                      const wchar_t*    geomPropName,
                                      FdoIGeometry      *pGeomValue );

    // Check the geometry property value for validity (e.g. non-collinear points in arcs)
    void CheckGeomPropValidity( const FdoSmLpClassDefinition *classDefinition,
                                      const wchar_t*    geomPropName,
                                      FdoIGeometry      *pGeomValue );

    // Check the geometric property associated spatial context against active spatial context
    virtual void AssociatedSpatialContextCheck( const FdoSmLpClassDefinition *classDefinition,
                                        const wchar_t *geomPropName );

    // Make sure the property value collection does not contain system or autogenerated properties
    // Used by the insert and update command for parameter validation
    virtual void SanitizePropertyValues( const FdoSmLpClassDefinition *classDefinition,
                                 FdoPropertyValueCollection      *propValCollection,
                                 bool                            *containsObjectProperties );


};

#endif // FdoRdbmsSchemaUtil_H
