#ifndef FDOSMLPASSOCIATIONPROPERTYDEFINITION_H
#define FDOSMLPASSOCIATIONPROPERTYDEFINITION_H		1
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

#include <Sm/Lp/DataPropertyDefinition.h>
//#include <Sm/Lp/PropertyMappingDefinition.h>
#include <Sm/Lp/PropertyDefinitionCollection.h>
#include <Sm/Ph/ColumnList.h>

class ClassDefinition;
// AssociationPropertyDefinition derives from PropertyDefinition and represents
// the association between two classes. The associated class must already be defined in the 
// feature schema and cannot be abstract.
class FdoSmLpAssociationPropertyDefinition : public FdoSmLpPropertyDefinition
{
public:
    
    /// Given a property collection and a column, this function finds the 
    /// the association property for the column.
	//
    /// Parameters:
    /// 	pProperties: the property collection.
    /// 	columnName: name of the column to search for.
	static const FdoSmLpPropertyDefinition* ColName2Property( 
		const FdoSmLpPropertyDefinitionCollection* pProperties,
		FdoStringP columnName 
	);

    /// Get the type of property  that this property represents.
	virtual FdoPropertyType GetPropertyType() const;

    /// Gets the name of the associated class  of this property.
	FdoString* GetAssociatedClassName() const;

    /// Gets a reference to the associated ClassObject. NULL if no class is defined or the class is not in the
    /// current database.
	const FdoSmLpClassDefinition* RefAssociatedClass() const;

    FdoDeleteRule GetDeleteRule() const { 
        ((FdoSmLpAssociationPropertyDefinition*) this)->Finalize();
        return mDeleteRule; 
    }

    bool GetCascadeLock() const { 
        ((FdoSmLpAssociationPropertyDefinition*) this)->Finalize();
        return mbCascadeLock; 
    }

    const wchar_t* GetMultiplicity() const { 
        ((FdoSmLpAssociationPropertyDefinition*) this)->Finalize();
        return (const wchar_t*)mMultiplicity; 
    }


    FdoString* GetReverseMultiplicity() const {  
        ((FdoSmLpAssociationPropertyDefinition*) this)->Finalize();
        return (const wchar_t*)mReverseMultiplicity; 
    }

    FdoString* GetReverseName() const {   
        ((FdoSmLpAssociationPropertyDefinition*) this)->Finalize();
        return (const wchar_t*)mReverseName; 
    }

    /// Returns a string collection containing the name of the properties that can be used to 
    /// identify instances of the associated class.
    /// Return value is always valid (never null).
    const FdoStringsP GetIdentityProperties() const;

    /// Returns a a string collection containing the name of the columns added to the current 
    /// class that are used to navigate to the associated class.
    /// Return value is always valid (never null).
    const FdoSmPhColumnListP GetIdentityColumns() const {
    	((FdoSmLpAssociationPropertyDefinition*) this)->Finalize();
        return mpIdentColumns; 
    }

    /// Returns a a string collection containing the name of the columns added to the current 
    /// class that are used to navigate to the associated class.
    /// Return value is always valid (never null).
    const FdoSmPhColumnListP GetReverseIdentityColumns() const { 
    	((FdoSmLpAssociationPropertyDefinition*) this)->Finalize();
        return mpReverseIdentColumns; 
    }

    /// Returns a string collection containing the name of the properties 
    /// that can be used as reverse identity property(foreign key)
    /// Return value is always valid (never null).
    const FdoStringsP GetReverseIdentityProperties() const;

    /// Makes this property an inherited property.
	//
    /// Parameters:
    /// 	pBaseProp: the property to inherit from.
	virtual void SetInherited( const FdoSmLpPropertyDefinition* pBaseProp );

    /// Convenience function for casting a property to an association property.
    /// Returns NULL if the property is not an association property.
	static const FdoSmLpAssociationPropertyDefinition* Cast( const FdoSmLpPropertyDefinition* src )
	{
		return( src ? ( src->GetPropertyType() == FdoPropertyType_AssociationProperty ? 
						(const FdoSmLpAssociationPropertyDefinition*) src  : NULL
					  ) : NULL );
	}

    /// Copy updates from corresponding FDO class. 
    //
    /// Parameters
    ///      pFdoProp: the FDO element.
    ///      elementState: the modification state for this association property.
    ///      pPropOverrides: Fdo to physical mapping overrides.
    ///      bIgnoreStates: true if the element state on the FDO element is 
    ///          to be ignored.
	virtual void Update(
        FdoPropertyDefinition* pFdoProp,
        FdoSchemaElementState elementState,
        FdoPhysicalClassMapping* pClassOverrides,
        bool bIgnoreStates
    );

    virtual void Update(
        FdoPropertyDefinition* pFdoProp,
        FdoSchemaElementState elementState,
        FdoPhysicalPropertyMapping* pPropOverrides,
        bool bIgnoreStates
    ); 

    /// Synchronize this association property and its table. The table is synchronized
    /// to match the association property.
    //
    /// Parameters:
    //
    ///      bRollbackOnly - 
    ///          true - synchronize only if a change to the class for this association property
    ///              has been rolled back since the previous synchronization.
    ///          false - always synchronize this association property.
	virtual void SynchPhysical(bool bRollbackOnly = true);

    /// Post updates to the database.
	virtual void Commit( bool fromParent = false );

	virtual FdoSchemaExceptionP Errors2Exception( FdoSchemaException* pFirstException = NULL ) const;

    /// Serialize this property to an XML file.
    /// Mainly for unit testing.
	virtual void XMLSerialize( FILE* xmlFp, int ref ) const;

protected:
    //Unused constructor needed only to build on Linux
    FdoSmLpAssociationPropertyDefinition() {}

    FdoSmLpAssociationPropertyDefinition(FdoSmPhClassPropertyReaderP propReader, FdoSmLpClassDefinition* parent);

    virtual ~FdoSmLpAssociationPropertyDefinition(void);


    FdoSmLpAssociationPropertyDefinition(
        FdoAssociationPropertyDefinition* pFdoProp, 
        bool bIgnoreStates,
        FdoSmLpClassDefinition* parent
    );

    /// Constructs an instance of an AssociationPropertyDefinition from a 
    /// base property.
    /// Used mainly by CreateInherited() and CreateCopy().
    //
    /// Parameters:
    /// 	pBaseProperty: initialize the new property from this base property.
    /// 	pTargetClass: the class that will contain the new property.
    /// 	logicalName: The name for the new property.
    /// 		If L"" then the new property has the same name as this
    /// 		property.
    /// 	physicalName: not applicable to association properties
    /// 	bInherit:
    /// 		True: the new property inherits from the base property.
    /// 		False: the new property is just a copy of the base property.
    ///      pPropOverrides: Fdo to physical mapping overrides for created property.
    FdoSmLpAssociationPropertyDefinition(
        FdoPtr<FdoSmLpAssociationPropertyDefinition> pBaseProperty, 
        FdoSmLpClassDefinition* pTargetClass, 
        FdoStringP logicalName, 
        FdoStringP physicalName, 
        bool bInherit,
        FdoPhysicalPropertyMapping* pPropOverrides = NULL
    );

    /// Check the datastore MetaData for a dependency from the table for the containing class
    /// to the containing table for this property.
	void LoadAssociationDefinition( const FdoSmLpClassDefinition* pParent);

    /// Finalize this association property. 
	void Finalize();

private:

    /// Helper method to return the  Sql value to the Delete rule
    int  DeleteRuleSqlValue();

    /// Helper method to convert from Sql delete rule value to FdoDeleteRule
    FdoDeleteRule  DeleteRuleValue( int sqlVal );

    void InitIdentyProperties();

	FdoStringP mAssociatedClassName;

	const FdoSmLpClassDefinition* mpClass;

    FdoDeleteRule           mDeleteRule;

    bool                    mbCascadeLock;

    bool                    mbColumnAdded;

    FdoStringP        mReverseName;

    FdoStringP        mMultiplicity;

    FdoStringP        mReverseMultiplicity;

    FdoStringP        mPseudoColumnName;

    FdoStringsP mpIndProperties;

    FdoStringsP mpReverseIndProperties;

    FdoSmPhColumnListP mpIdentColumns;

    FdoSmPhColumnListP mpReverseIdentColumns;

};

typedef FdoPtr<FdoSmLpAssociationPropertyDefinition> FdoSmLpAssociationPropertyP;

#endif


