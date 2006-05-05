#ifndef FDOSMLPFEATURECLASS_H
#define FDOSMLPFEATURECLASS_H		1
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

#include <Sm/Lp/ClassDefinition.h>
#include <Sm/Lp/GeometricPropertyDefinition.h>

// A FeatureClass can be used to represent spatial or 
// non-spatial features. Spatial features must have at 
// least one geometric property.

class FdoSmLpFeatureClass : public FdoSmLpClassDefinition
{
public:
    /// Returns the concrete class type, ClassType.FeatureClass.
    virtual FdoClassType GetClassType() const;

    /// Returns the geometric property that defines the geometry for 
    /// objects of this class.
    /// Returns NULL when this class is not a spatial feature class.
	const FdoSmLpGeometricPropertyDefinition* RefGeometryProperty() const;
	FdoSmLpGeometricPropertyP GetGeometryProperty();

    /// Feature classes can have geometry.
	virtual bool SupportsGeometry() const { return(true);} 

    /// Convenience function for casting a ClassDefinition to a Feature Class.
    /// Returns NULL if the ClassDefinition is not a Feature Class.
	static const FdoSmLpFeatureClass* Cast( const FdoSmLpClassDefinition* src )
	{
		return( (src  && (src->GetClassType() == FdoClassType_FeatureClass)) ? 
					(const FdoSmLpFeatureClass*) src : NULL
		);
 	}

	//Copy updates from corresponding FDO class. 
    //
    /// Parameters:
    ///      pFdoClass: the FDO class.
    ///      elementState: the modification state for this feature class.
    ///      pClassOverrides: Fdo to physical mapping overrides.
    ///      bIgnoreStates: true if the element state on the FDO class is 
    ///          to be ignored.
	virtual void Update(
        FdoClassDefinition* pFdoClass,
        FdoSchemaElementState elementState,
        FdoPhysicalClassMapping* pClassOverrides,
        bool bIgnoreStates
    ); 

    /// Post outstanding modifications to the current database.
	virtual void Commit( bool fromParent = false );

    /// Allows sub-classes to insert class-specific sub-elements into the
    /// XML file. 
    /// Mainly for unit testing.
	virtual void XMLSerializeSubElements(  FILE* xmlFp, int ref ) const;

protected:
    //Unused constructor needed only to build on Linux
    FdoSmLpFeatureClass() {}

    /// Constructs a LogicalPhysical feature class from a class reader
	FdoSmLpFeatureClass(FdoSmPhClassReaderP classReader, FdoSmLpSchemaElement* parent);

    /// Constructs a LogicalPhysical feature class from an FDO feature class.
	FdoSmLpFeatureClass(
        FdoFeatureClass* pFeatClass, 
        bool bIgnoreStates,
        FdoSmLpSchemaElement* parent
    );

    /// Get a class writer for adding feature class definitions.
    /// Returns a class writer with fields already set.
    virtual FdoSmPhClassWriterP GetPhysicalAddWriter();

    /// Get a class writer for modifying feature class definitions.
    /// Returns a class writer with fields already set.
    virtual FdoSmPhClassWriterP GetPhysicalModifyWriter();
/*
    /// Create a new class table of the given name.
 	virtual FdoSmPhTable* NewTable( 
        const wchar_t* tableName, 
        const wchar_t* pkeyName,
        const wchar_t* tablespace
    );
 */
    /// Finalize this class. Find the Geometry property if
    /// any.
	virtual void PostFinalize();
/*
    /// Set the Geometry property schema mappings.
	//
    /// Parameters:
    /// 	classMapping: add the schema mappings to this class mapping.
    /// 	bIncludeDefaults - 
    ///          true: include all schema mappings.
    ///          false: include only non-default mappings.
    /// Returns:
    ///      true: if mappings were added to classMappings
    ///      false: no mappings were added.
    virtual bool _setSchemaMappings( FdoOracleOvClassP classMapping, bool bIncludeDefaults ) const;
*/

private:

    bool IsGeomInMetaschema();

	void AddMultiGeomPropError( 
		const FdoSmLpGeometricPropertyDefinition* pProp1,
		const FdoSmLpGeometricPropertyDefinition* pProp2
	);
	void FdoSmLpFeatureClass::AddGeomPropNotFoundError( const wchar_t* propName );
	void FdoSmLpFeatureClass::AddSecondaryGeomError( const wchar_t* propName );
	void AddGeomOverrideError( const wchar_t* baseClassName );
	void AddGeomRemoveError( const wchar_t* basePropName );
	void AddGeomDeletedError();
	void AddNoIdError();

	FdoSmLpGeometricPropertyP mGeometryProperty;

	FdoStringP mGeometryPropertyName;
};

typedef FdoPtr<FdoSmLpFeatureClass> FdoSmLpFeatureClassP;

#endif


