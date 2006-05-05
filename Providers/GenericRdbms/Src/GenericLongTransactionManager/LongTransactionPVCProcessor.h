/******************************************************************************
 * (C) Copyright 2003 by Autodesk, Inc. All Rights Reserved.
 *
 * By using this code, you are agreeing to the terms and conditions of
 * the License Agreement included in the documentation for this code.
 *
 * AUTODESK MAKES NO WARRANTIES, EXPRESS OR IMPLIED, AS TO THE
 * CORRECTNESS OF THIS CODE OR ANY DERIVATIVE WORKS WHICH INCORPORATE
 * IT. AUTODESK PROVIDES THE CODE ON AN "AS-IS" BASIS AND EXPLICITLY
 * DISCLAIMS ANY LIABILITY, INCLUDING CONSEQUENTIAL AND INCIDENTAL
 * DAMAGES FOR ERRORS, OMISSIONS, AND OTHER PROBLEMS IN THE CODE.
 *
 * Use, duplication, or disclosure by the U.S. Government is subject
 * to restrictions set forth in FAR 52.227-19 (Commercial Computer
 * Software Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
 * (Rights in Technical Data and Computer Software), as applicable.
 * 
  *
 ******************************************************************************/

#ifndef FDORDBMSLONGPVCPROCESSOR_H
#define FDORDBMSLONGPVCPROCESSOR_H

#ifdef _WIN32
#pragma once
#endif

#include <Inc/Nls/fdordbms_msg.h>
#include "FdoRdbmsException.h"
#include "Fdo/Pvc/FdoRdbmsPvcProcessor.h"
#include "Fdo/Pvc/FdoRdbmsPvcUpdateHandler.h"
#include "LongTransactionPvcInsertSelectHandler.h"

class FdoRdbmsLongTransactionManager;

enum FdoRdbmsLTUpdateOpType{
	FdoRdbmsLTUpdateOpType_Insert,
	FdoRdbmsLTUpdateOpType_Update,
	FdoRdbmsLTUpdateOpType_InsertSelect
};

//
// Helper class used by the long transaction manager only
//
class FdoRdbmsLongTransactionPVCProcessor: public FdoRdbmsPvcProcessor
{
private:
	class InsertWithIdHandler: public FdoRdbmsLongTransactionPvcInsertSelectHandler
	{
	protected:
		virtual void CreateInsertStringForColumn(
			const FdoSmPhColumn *column,
			const FdoSmLpPropertyDefinition *propertyDefinition,
			FdoPropertyValueCollection  *propValCollection,
			FdoStringP &insertString,
			FdoStringP &valueString,
			int& bindCount,
			bool& emptyBlobAdded );

		virtual void CreateInsertStringWhereCriteria(
			const FdoSmLpClassDefinition *classDefinition,
			FdoPropertyValueCollection  *propValCollection,
			FdoStringP &whereString,
			int& bindCount );

		virtual void SetAditionalBindVariables(const FdoSmLpClassDefinition *currentClass, 
						  const wchar_t *scope, int &bind_no, 
						  FdoPropertyValueCollection  *propValCollection, 
						  FdoRdbmsPvcBindDef *bind, int gid);

	public:
		InsertWithIdHandler( );

		InsertWithIdHandler( FdoRdbmsConnection *connection );

	} mInsertWithIdHandler;

	class UpdateWithIdHandler: public FdoRdbmsPvcUpdateHandler
	{
	protected:
		virtual void AdditionalCriteria( FdoPropertyValueCollection  *propValCollection, FdoStringP & updateString, int & bindIndex );

		virtual void AdditionalBinds( GdbiStatement *statement, FdoRdbmsPvcBindDef  *values, int & bindIndex  );

	public:
		public:
		UpdateWithIdHandler( );

		UpdateWithIdHandler( FdoRdbmsConnection *connection );
	} mUpdateWithIdHandler;

	FdoRdbmsLongTransactionPvcInsertSelectHandler  *mInsertSelectHandler;
	FdoRdbmsLongTransactionManager		    	   *mLtManager;

	FdoPropertyValueCollection* BuildPvcForUpdateNextLt( FdoPropertyValueCollection*  values, const FdoSmLpClassDefinition* smLpClassDef, FdoInt64 oldLtId ) const;

	FdoPropertyValueCollection* BuildPvcForSentinelRow( FdoPropertyValueCollection*  values, const FdoSmLpClassDefinition* smLpClassDef ) const;

	FdoPropertyValueCollection* BuildPvcWithLtInfo( FdoPropertyValueCollection*  values, const FdoSmLpClassDefinition* smLpClassDef ) const;

	FdoPropertyValueCollection* BuildPvcWithLtInfo( FdoPropertyValueCollection*  values, FdoInt64 ltId ) const;

	FdoRdbmsLTUpdateOpType  ResolveUpdateOpType( const FdoSmLpClassDefinition* smLpClassDef, FdoPropertyValueCollection* values, FdoInt64& objLdIt ) const;

	void BindDataValue( int bindIdx, FdoDataValue *dataVal, FdoRdbmsPvcBindDef& bindValue, GdbiStatement	*statement ) const;

	void FreeBinds( FdoRdbmsPvcBindDef     *bindValues, int size  ) const;
    
    FdoStringP BuildValueListErrorMessage ( FdoPropertyValueCollection*  values ) const;

public:
	FdoRdbmsLongTransactionPVCProcessor ( );

    FdoRdbmsLongTransactionPVCProcessor ( FdoRdbmsLongTransactionManager *ltManager, FdoRdbmsConnection *conn );

    virtual ~FdoRdbmsLongTransactionPVCProcessor ();

	virtual FdoRdbmsCollection<FdoRdbmsPvcOperation>* RefactorPvc(FdoPropertyValueCollection* values, const FdoSmLpClassDefinition* smLpClassDef, bool forUpdate ) const;
}; 

#endif

