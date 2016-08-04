#ifndef FDOSMLPSCHEMAELEMENT_H
#define FDOSMLPSCHEMAELEMENT_H		1
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

#include <Sm/Lp/SAD.h>
#include <Sm/SchemaElement.h>
#include <Sm/Ph/SADReader.h>

class FdoSmLpSchema;

// SchemaElement is an abstract class that acts as a base class for all classes
// in the LogicalPhysical Schema package.
class FdoSmLpSchemaElement : public FdoSmSchemaElement
{
  public:

    /// Constructs an instance of a SchemaElement using the specified arguments.
    FdoSmLpSchemaElement(
        FdoString* name, 
        FdoString* description, 
        FdoSmLpSchemaElement* parent = NULL,
        bool fromFdo = false                    // true if this object came directly from an FDO Feature Schema.
    );

    /// Returns the LogicalPhysicalSchema this object is a part of. Returns null if this
    /// object has not been added to a LogicalPhysical Schema.
    const FdoSmLpSchema* RefLogicalPhysicalSchema() const;

    /// Returns true if this object was generated directly from an FDO Feature Schema object
    /// (Either from doing an ApplySchema or loading from a config doc).
    bool GetIsFromFdo() const
    {
        return mFromFdo;
    }

    /// Returns true if this object was loaded from a config doc.
    bool GetIsFromConfig() const
    {
        return GetIsFromFdo() && (GetElementState() == FdoSchemaElementState_Unchanged);
    }

    // Returns true if the current datastore has MetaSchema, false otherwise.
    bool GetHasMetaSchema() const;

    /// Copy updates from corresponding FDO element. 
    //
    /// Parameters
    ///      pFdoElement: the FDO element.
    ///      elementState: the modification state for this element
    ///      bIgnoreStates: true if the element state on the FDO element is 
    ///          to be ignored.
	virtual void Update(
        FdoSchemaElement* pFdoElement, 
        FdoSchemaElementState elementState,
        bool bIgnoreStates
    ); 

    /// Synchronize this element with its physical database object(s). The physical schema
    /// is synchronized to match this object.
    //
    /// Parameters:
    //
    ///      bRollbackOnly - 
    ///          true - synchronize only if a change to this element has been rolled back since
    ///              the previous synchronization.
    ///          false - always synchronize this element.
	virtual void SynchPhysical(bool bRollbackOnly = true) {}

    /// Get the Schema Attribute Dictionary for this element.
	virtual const FdoSmLpSAD* RefSAD() const;

    /// Get the Schema Attribute Dictionary for this element.
	virtual FdoSmLpSADP GetSAD();

	void CommitSAD(FdoString* elementType);

    /// Mainly for Unit Testing.
    /// Serializes this schema element to the given XML file.
	virtual void XMLSerialize( FILE* xmlFp, int ref ) const;

protected:
    /// Unused constructor needed only to build on Linux
    FdoSmLpSchemaElement() {}

	virtual ~FdoSmLpSchemaElement(void);

    /// Returns the LogicalPhysicalSchema in modifyable form.
    FdoPtr<FdoSmLpSchema> GetLogicalPhysicalSchema();

    /// Set the LogicalPhysical schema that contains this element.
	void SetLogicalPhysicalSchema(FdoPtr<FdoSmLpSchema> schema);

    /// Clean out the SAD
	void DeleteSAD();
	
    /// (re)load the SAD using the given SAD reader.
	void LoadSAD( FdoSmPhISADReader* pSADReader );

    /// (re)load the SAD from FDO.
	void LoadSAD( FdoSchemaAttributeDictionary* pFdoSAD );

    /// merge the SAD from FDO.
	void MergeSAD( FdoSchemaAttributeDictionary* pFdoSAD );

    /// Checks that the given string will fit in the given string column. The length of
    /// of the UTF8 version of pString is checked. An error is added to the error list
    /// if the string is too long
    //
    /// Parameters:
    ///      pString - the string to check.
    ///      pTable - table containing column where string will be stored
    ///      pColumn - column where string will be stored (must be a string type column)
    ///          An error is logged if the string will not fit in this column.
    ///          No error is logged if this column does not exist. In this case it is
    ///          assumed that this string will not go into the RDBMS.
    ///      elementNlsNum, dfltElementName - the name of the Schema Element that has 
    ///          this value (used in the error message).
    ///          elementNlsNum is the message number from the NLS message catalog.
    ///          dfltElementName is the element name to use if the message catalog is 
    ///          not accessible.
    ///      itemNlsNum, dfltItemName - the name of the Schema Element attribute that 
    ///          has this value (used in the error message).
    ///          itemNlsNum is the message number from the NLS message catalog.
    ///          dfltItemName is the attribute name to use if the message catalog is 
    ///          not accessible.
    void ValidateStringLength( 
        FdoString* pString, 
        FdoString* pTable,
        FdoString* pColumn,
        FdoInt32   elementNlsNum,
        const char* dfltElementName,
        FdoInt32   itemNlsNum,
        const char* dfltItemName
    );

private:
    void AddSADNoMetaError( FdoSmPhOwnerP owner );

    /// Containing schema.
	FdoSmLpSchema* mSchema;

	FdoSmLpSADP mSAD;

    bool mFromFdo;
};

#endif


