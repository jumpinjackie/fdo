#ifndef FDORDBMSLONGTRANSACTIONPVCINSERTSELECTHANDLER_H
#define FDORDBMSLONGTRANSACTIONPVCINSERTSELECTHANDLER_H    1
/*
 * (C) Copyright 2005 by Autodesk, Inc. All Rights Reserved.
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
 */
#include "Fdo/Pvc/FdoRdbmsPvcInsertHandler.h"

#define		PREVIOUS_LTID_NAME		L"PreviousLtId"   // Used to save the previous LtId in the property value collection
#define		IDENTY_NAME_EXTESION	L"_ForLT"		  // Used to modify the name of the identities values in the property value collection 

class FdoRdbmsConnection;

//
// Defines an abstract interface for the Property Value Collection handlers
class FdoRdbmsLongTransactionPvcInsertSelectHandler: public FdoRdbmsPvcInsertHandler
{
private:

	void BindProperty(const FdoSmLpDataPropertyDefinition *prop, int &bind_no, 
						  FdoRdbmsPvcBindDef *bind, int gid, const wchar_t* overrideName );
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

	virtual void GetStartInsertString( FdoStringP& insertStartString, const wchar_t* tableName );

	virtual void SetAditionalBindVariables(const FdoSmLpClassDefinition *currentClass, 
						  const wchar_t *scope, int &bind_no, 
						  FdoPropertyValueCollection  *propValCollection, 
						  FdoRdbmsPvcBindDef *bind, int gid);

public:
	FdoRdbmsLongTransactionPvcInsertSelectHandler( );

	FdoRdbmsLongTransactionPvcInsertSelectHandler( FdoRdbmsConnection *connection );

	virtual ~FdoRdbmsLongTransactionPvcInsertSelectHandler();
    /// <summary>Given a PVC operation, execute the handler.</summary>
    /// <returns>Returns the number of affected objects</returns> 
    virtual long Execute( const FdoSmLpClassDefinition *classDefinition, FdoPropertyValueCollection  *propValCollection, bool revisionNumberUpdate = false);
};

#endif

