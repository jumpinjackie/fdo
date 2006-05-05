#ifndef FDORDBMSPVCINSERTHANDLER_H
#define FDORDBMSPVCINSERTHANDLER_H    1
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
#include "Fdo/Pvc/FdoRdbmsPvcHandler.h"
#include <Sm/Lp/AssociationPropertyDefinition.h>


#define     QUERY_CACHE_SIZE    10   // The number of cached cursors to handle the attributes.


class FdoRdbmsConnection;

typedef  struct _insert_cache_ {
      wchar_t       tableName[GDBI_SCHEMA_ELEMENT_NAME_SIZE];
      int           qid;
      int           count;
      FdoRdbmsPvcBindDef   *bind;
} InsertQueryDef;


//
// Defines an abstract interface for the Property Value Collection handlers
class FdoRdbmsPvcInsertHandler: FdoRdbmsPvcHandler
{
private:

	wchar_t				mLastTableName[GDBI_SCHEMA_ELEMENT_NAME_SIZE];
	InsertQueryDef		mInsertQueryCache[QUERY_CACHE_SIZE];
    InsertQueryDef		*mLastInsertQuery;
	int					mNextQidToFree;

protected:

	FdoRdbmsConnection *mFdoConnection;

	bool				mInsertAutoIncrementProperties;

	bool				mBindUnsetValues;

	virtual void CreateInsertStringForColumn(
        const FdoSmPhColumn *column,
        const FdoSmLpPropertyDefinition *propertyDefinition,
        FdoPropertyValueCollection  *propValCollection,
        FdoStringP &insertString,
        FdoStringP &valueString,
        int& bindCount,
        bool& emptyBlobAdded );

	virtual void CreateInsertStringForColumn(
        const wchar_t *columnName,
        FdoStringP &insertString,
        FdoStringP &valueString,
        int& bindCount );

	virtual void CreateInsertStringWhereCriteria(
        const FdoSmLpClassDefinition *classDefinition,
        FdoPropertyValueCollection  *propValCollection,
        FdoStringP &whereString,
        int& bindCount );

	virtual void GetStartInsertString( FdoStringP& insertStartString, const wchar_t* tableName );

	virtual void SetAditionalBindVariables(const FdoSmLpClassDefinition *currentClass, 
						  const wchar_t *scope, int &bind_no, 
						  FdoPropertyValueCollection  *propValCollection, 
						  FdoRdbmsPvcBindDef *bind, int gid);

public:
	FdoRdbmsPvcInsertHandler( );

	FdoRdbmsPvcInsertHandler( FdoRdbmsConnection *connection );

	virtual ~FdoRdbmsPvcInsertHandler();

    /// <summary>Given a PVC operation, execute the handler.</summary>
    /// <returns>Returns the number of affected objects</returns> 
    virtual long Execute( const FdoSmLpClassDefinition *classDefinition, FdoPropertyValueCollection  *propValCollection, bool revisionNumberUpdate = false, bool handleForeignAutoincrementedId=false );

private:
	void SetBindValues(const FdoSmLpClassDefinition *classDefinition, 
						   FdoPropertyValueCollection  *propValCollection, InsertQueryDef *insertQuery,
                           bool handleForeignAutoincrementedId);

	void CreateInsertString(const FdoSmLpClassDefinition *currentClass,
                           FdoPropertyValueCollection  *propValCollection,
                           FdoStringP& insertString,
                           FdoStringP& valuesString,
                           int& bindCount,
                           bool scanOnly );

    

	void SetBindVariables(const FdoSmLpClassDefinition *currentClass, 
						  const wchar_t *scope, int &bind_no, FdoPropertyValueCollection  *propValCollection, FdoRdbmsPvcBindDef *bind, int gid);

	InsertQueryDef *GetInsertQuery( const wchar_t *tableName, bool alloc_new = false );

	void AssociationConstrainCheck( const FdoSmLpAssociationPropertyDefinition* propertyDefinition,
                                    FdoPropertyValueCollection  *propValCollection );
};

#endif

