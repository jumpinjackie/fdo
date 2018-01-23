#ifndef FDOSMLPPROPERTYDEFINITION_H
#define FDOSMLPPROPERTYDEFINITION_H		1
//
// Copyright (C) 2004-2006  Autodesk, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

#ifdef _WIN32
#pragma once
#endif

// Temporary: needs to be repointed at FDO.
#include <Sm/Lp/SchemaElement.h>
//#include <Sm/Lp/DataTypeMapper.h>
#include <Sm/Ph/ClassPropertyReader.h>
#include <Sm/NamedCollection.h>
#include <Sm/Lp/PropertyTypeMapper.h>
//#include <Rdbms/Override/Oracle/OracleOvPropertyDefinition.h>

class FdoSmLpClassDefinition;

// PropertyDefinition is an abstract class that derives from SchemaObject.
// PropertyDefinition is the base class of DataPropertyDefinition and
// ClassPropertyDefinition. 
class FdoSmLpPropertyDefinition : public FdoSmLpSchemaElement
{
public:
    virtual ~FdoSmLpPropertyDefinition()
    {
    }

    /// Get the type of property ( object, data, geometric, etc. ) that
    /// this object represents.
	virtual FdoPropertyType GetPropertyType() const = 0;

    /// Returns a bool value that indicates if this property is read only.
    bool GetReadOnly() const;

    /// Returns a bool value that indicates if this property is an FDO Feature Id.
	bool GetIsFeatId() const;

    void SetIsFeatId(bool value);
    /// Returns a bool value that indicates if this property is an FDO System property.
    bool GetIsSystem() const;

    /// Gets the class that defines this property. When this property
    /// is inherited then the defining and containing ( returned by
    /// GetParent() class will be different.
    /// NULL if the defining class is not in the current database.
	const FdoSmLpClassDefinition* RefDefiningClass() const;

    /// Gets the class that defines this property. When this property
    /// is inherited then the defining and containing ( returned by
    /// GetParent() class will be different.
    /// NULL if the defining class is not in the current database.
	const FdoSmLpClassDefinition* RefParentClass() const
    {
        return mpParentClass;
    }

    /// When this is an inherited property, this function returns 
    /// the property that it is based on.
    /// Returns NULL if this property is not inherited.
	const FdoSmLpPropertyDefinition* RefBaseProperty() const;

    FdoPtr<FdoSmLpPropertyDefinition> GetBaseProperty();

    /// When this is a copied property, this function returns 
    /// the property that it is based on.
    /// Returns NULL if this property was not copied from another property.
	const FdoSmLpPropertyDefinition* RefSrcProperty() const;

    FdoPtr<FdoSmLpPropertyDefinition> GetSrcProperty();

    /// When this is a copied or inherited  property, this function returns 
    /// the property that it was directly copied or inherited from. This function
    /// differs from GetBaseProperty() and GetSrcProperty() since these return
    /// the original property that it was inherited or copied from.
    /// Returns NULL if this property was not copied or inherited from another property.
	const FdoSmLpPropertyDefinition* RefPrevProperty() const;

    FdoPtr<FdoSmLpPropertyDefinition> GetPrevProperty();

    /// Get the highest level object property that contains this 
    /// property.
    /// Returns this property when it is not nested within
    /// another object property.
	const FdoSmLpPropertyDefinition* RefTopProperty() const;

    FdoPtr<FdoSmLpPropertyDefinition> GetTopProperty();

    /// Returns the name of the table where values for this 
    /// property are stored. 
	FdoString* GetContainingDbObjectName() const;

    /// Returns the table where values for this 
    /// property are stored ( in read-only form ).
    /// Returns NULL if the table is not in the current database.
	const FdoSmPhDbObject* RefContainingDbObject() const;

    /// Returns the table where values for this 
    /// property are stored ( in modifyable form ).
	FdoSmPhDbObjectP GetContainingDbObject();

    /// Get the Schema Attribute Dictionary for this property.
	virtual const FdoSmLpSAD* RefSAD() const;

    /// Gets the qualified name for this property 
    /// ( [schemaname]:[classname].[propertyname] )
	virtual FdoStringP GetQName() const;

    /// Get the nested name for this property.
    ///  [propertyname] if this property is not inside an object property
    ///  { [objectpropertyname].}*[propertyname] if it is nested inside object properties.
	virtual FdoStringP GetNestedName() const;

    /// Makes this property an inherited property.
	//
    /// Parameters:
    /// 	pBaseProp: the property to inherit from.
	virtual void SetInherited( const FdoSmLpPropertyDefinition* pBaseProp );

    /// Sets the read-only status for this property.
	void SetReadOnly( bool readOnly )
	{
		mReadOnly = readOnly;
	}

    /// Set the highest level object property that contains this 
    /// object property.
	void SetTopProperty( FdoPtr<FdoSmLpPropertyDefinition> pTopProp );

    /// Set the source property for this property.
    /// Called when this is a nested (object sub-property) property
    /// read from the datastore. The source is set to the 
    /// object property's class's property that this property was
    /// originally generated from.
	void SetSrcProperty( FdoPtr<FdoSmLpPropertyDefinition> pSrcProp );
   
    /// Inherit this property to a subclass. Creates a new instance of this 
    /// property, which can be attached to the subclass.
	//
    /// Parameters:
    /// 	pSubClass: inherit to this subclass. Must be a subclass of this
    /// 		property's defining class.
	
    FdoPtr<FdoSmLpPropertyDefinition> CreateInherited( FdoSmLpClassDefinition* pSubClass ) const;

    /// Create a non-inherited copy of this property. 
	//
    /// Parameters:
    /// 	pTargetClass: The class that will contain the new property.
    /// 	logicalName: The name for the new property.
    /// 		If L"" then the new property has the same name as this
    /// 		property.
    /// 	physicalName: the name of the property's physical object. Actual
    /// 		meaning depends in the property type.
    /// 		If L"" then the default physical name is used.
    ///      pPropOverrides: Fdo to physical mapping overrides for created property.
	FdoPtr<FdoSmLpPropertyDefinition> CreateCopy( 
        FdoSmLpClassDefinition* pTargetClass, 
        FdoStringP logicalName, 
        FdoStringP physicalName,
        FdoPhysicalPropertyMapping* pPropOverrides = NULL
    ) const;

    /// Get the Schema Mappings for this property
	//
    /// Parameters:
    /// 	bIncludeDefaults - 
    ///          true: include all schema mappings.
    ///          false: include only non-default mappings.
    /// Returns:
    ///      returns NULL. Sub-classes override this function to fill in the
    ///      schema mappings specific to the property type.
    virtual FdoPhysicalPropertyMappingP GetSchemaMappings( bool bIncludeDefaults ) const
    {
    /// No mappings by default.
        return FdoPhysicalPropertyMappingP();
    }
    
    /// Adds the physical mappings for this property to the given class mappings.
	//
    /// Parameters:
    ///      schemaMapping - the class mappings.
    /// 	bIncludeDefaults - 
    ///          true: include all schema mappings.
    ///          false: include only non-default mappings.
    virtual bool AddSchemaMappings( FdoPhysicalClassMappingP schemaMapping, bool bIncludeDefaults ) const
    {
        return false;
    }

    /// Copy updates from corresponding FDO property. 
    /// Provider's must override to extract property overrides from the
    /// given class overrides.
    //
    /// Parameters
    ///      pFdoProp: the FDO element.
    ///      elementState: the modification state for this property
    ///      pClassOverrides: Fdo to physical mapping overrides.
    ///      bIgnoreStates: true if the element state on the FDO element is 
    ///          to be ignored.
	virtual void Update(
        FdoPropertyDefinition* pFdoProp,
        FdoSchemaElementState elementState,
        FdoPhysicalClassMapping* pClassOverrides,
        bool bIgnoreStates
    ) = 0; 

    /// Copy updates from corresponding FDO property. 
    //
    /// Parameters
    ///      pFdoProp: the FDO element.
    ///      elementState: the modification state for this property
    ///      pPropOverrides: Fdo to physical mapping overrides.
    ///      bIgnoreStates: true if the element state on the FDO element is 
    ///          to be ignored.
	virtual void Update(
        FdoPropertyDefinition* pFdoProp,
        FdoSchemaElementState elementState,
        FdoPhysicalPropertyMapping* pPropOverrides,
        bool bIgnoreStates
    ); 

    /// Posts modifications to this element to the FDO data store.
	virtual void Commit( bool fromParent = false );

	virtual FdoSchemaExceptionP Errors2Exception( FdoSchemaException* pFirstException = NULL ) const;

    /// Output this property to an XML file.
    /// Mainly for unit testing.
	virtual void XMLSerialize( FILE* xmlFp, int ref ) const = 0;

protected:
    /// Unused constructor needed only to build on Linux
    FdoSmLpPropertyDefinition() {}


    /// Loads an instance of a PropertyDefinition from the FDO data store.
    //
    /// Parameters:
    /// 	propReader: reader for obtaining the DataPropertyDefinition attributes.
    /// 	parent: the containing class.
    FdoSmLpPropertyDefinition(FdoSmPhClassPropertyReaderP propReader, FdoSmLpClassDefinition* parent);
    
    /// Creates an instance of a PropertyDefinition from an FDO Feature Schema
    /// element.
	//
    /// Parameters:
    /// 	pFdoProp: the FDO element.
    ///      pPropOverrides: Fdo to physical mapping overrides.
    ///      bIgnoreStates: true if the element state on the FDO element is 
    ///          to be ignored.
    /// 	parent: the containing class.
	FdoSmLpPropertyDefinition(
        FdoPropertyDefinition* pFdoProp, 
//        FdoOracleOvPropertyDefinition* pPropOverrides,
        bool bIgnoreStates,
        FdoSmLpClassDefinition* parent
    );

    /// Constructs an instance of a PropertyDefinition from the given parameters.
    /// Used mainly by CreateInherited() and CreateCopy().
    //
    /// Parameters:
    /// 	pBaseProperty: initialize the new property from this base property.
    /// 	pTargetClass: the class that will contain the new property.
    /// 	logicalName: The name for the new property.
    /// 		If L"" then the new property has the same name as this
    /// 		property.
    /// 	physicalName: the name of the property's physical object. Actual
    /// 		meaning depends in the property type.
    /// 		If L"" then the default physical name is used.
    /// 	bInherit:
    /// 		True: the new property inherits from the base property.
    /// 		False: the new property is just a copy of the base property.
    ///      pPropOverrides: Fdo to physical mapping overrides for created property.
	FdoSmLpPropertyDefinition(
		FdoPtr<FdoSmLpPropertyDefinition> baseProperty, 
		FdoSmLpClassDefinition* pTargetClass, 
		FdoStringP logicalName,
		FdoStringP physicalName,
		bool bInherit,
        FdoPhysicalPropertyMapping* pPropOverrides = NULL
	);

    virtual FdoPtr<FdoSmLpPropertyDefinition> NewInherited( FdoSmLpClassDefinition* pSubClass ) const = 0;

    /// Create a non-inherited copy of this property. 
	//
    /// Parameters:
    /// 	pTargetClass: The class that will contain the new property.
    /// 	logicalName: The name for the new property.
    /// 		If L"" then the new property has the same name as this
    /// 		property.
    /// 	physicalName: the name of the property's physical object. Actual
    /// 		meaning depends in the property type.
    /// 		If L"" then the default physical name is used.
    ///      pPropOverrides: Fdo to physical mapping overrides for created property.
	virtual FdoPtr<FdoSmLpPropertyDefinition> NewCopy( 
        FdoSmLpClassDefinition* pTargetClass, 
        FdoStringP logicalName, 
        FdoStringP physicalName,
        FdoPhysicalPropertyMapping* pPropOverrides = NULL
    ) const = 0;

    /// Sets the table that stores values for this property
	void SetContainingDbObject( FdoSmPhDbObjectP dbObject, FdoString* pDbObjectName = L"" ) 
	{
		mContainingDbObject = dbObject;
		if ( dbObject ) 
			mContainingDbObjectName = dbObject->GetName();
		else
			mContainingDbObjectName = pDbObjectName;
	}

    virtual	void Finalize();

	void XmlSerializeProperty( wchar_t* propertyType, FILE* xmlFp, int ref );

	void AddRedefinedError( const FdoSmLpPropertyDefinition* pBaseProp );
	void AddWrongOverrideTypeError();
	virtual void AddReferenceLoopError();	
   	void AddGenericError( FdoStringP specificError );

private:

	void InheritElementState( const FdoSmLpPropertyDefinition* pBaseProp );

	void AddPropTypeChangeError( FdoPropertyType newType );
	void AddBasePropError( const FdoSmLpPropertyDefinition* pBaseProp );

	bool mReadOnly;

	bool mIsFeatId;

	bool mIsSystem;

	const FdoSmLpClassDefinition* mpParentClass;

    const FdoSmLpClassDefinition* mpDefiningClass;

    /// base property for inherited property
	FdoPtr<FdoSmLpPropertyDefinition> mBaseProperty;

    /// original property for copied property.
	FdoPtr<FdoSmLpPropertyDefinition> mSrcProperty;

	FdoPtr<FdoSmLpPropertyDefinition> mPrevProperty;

    /// Top level property, when property is nested. 
    /// Back pointer, no reference held.
	FdoSmLpPropertyDefinition* mTopProperty;

	FdoStringP mContainingDbObjectName;

	FdoSmPhDbObjectP mContainingDbObject;

};

typedef FdoPtr<FdoSmLpPropertyDefinition> FdoSmLpPropertyP;

#endif


