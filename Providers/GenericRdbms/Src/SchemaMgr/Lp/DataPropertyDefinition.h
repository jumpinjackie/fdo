#ifndef FDOSMLPGRDDATAPROPERTYDEFINITION_H
#define FDOSMLPGRDDATAPROPERTYDEFINITION_H		1
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

#include <Sm/Lp/DataPropertyDefinition.h>

// DataPropertyDefinition derives from PropertyDefinition and represents simple
// values or collections of simple values. DataPropertyDefinitions can take on
// any of the data types listed in the DataType enumeration.
class FdoSmLpGrdDataPropertyDefinition : public FdoSmLpDataPropertyDefinition
{
public:
    
	// Update from generic class overrides
    //
    // Parameters
    //      pFdoProp: the FDO element.
    //      elementState: the modification state for this data property.
    //      pClassOverrides: the class overrides
    //      bIgnoreStates: true if the element state on the FDO element is 
    //          to be ignored.
	virtual void Update(
        FdoPropertyDefinition* pFdoProp,
        FdoSchemaElementState elementState,
        FdoPhysicalClassMapping* pClassOverrides,
        bool bIgnoreStates
    ); 

	// Update from generic property overrides
    //
    // Parameters
    //      pFdoProp: the FDO element.
    //      elementState: the modification state for this data property.
    //      pPropOverrides: the property overrides
    //      bIgnoreStates: true if the element state on the FDO element is 
    //          to be ignored.
	virtual void Update(
        FdoPropertyDefinition* pFdoProp,
        FdoSchemaElementState elementState,
        FdoPhysicalPropertyMapping* pPropOverrides,
        bool bIgnoreStates
    ); 

	// Update from generic property overrides
    //
    // Parameters
    //      pPropOverrides: the property overrides
	virtual void Update(
        FdoPhysicalPropertyMapping* pPropOverrides
    ); 

    // FDO Long Transaction mode system property names
  	static const FdoStringP LtIdName;
	static const FdoStringP NextLtIdName;

    // FDO Persistent Locking mode system property names
	static const FdoStringP LockIdName;
	static const FdoStringP LockTypeName;

protected:
    // unused constructor needed only to build Linux
    FdoSmLpGrdDataPropertyDefinition() {}
    FdoSmLpGrdDataPropertyDefinition(FdoSmPhClassPropertyReaderP propReader, FdoSmLpClassDefinition* parent);

    FdoSmLpGrdDataPropertyDefinition(
        FdoDataPropertyDefinition* pFdoProp, 
        bool bIgnoreStates,
        FdoSmLpClassDefinition* parent
    );

    FdoSmLpGrdDataPropertyDefinition(
        FdoSmLpDataPropertyP pBaseProperty,
        FdoSmLpClassDefinition* pTargetClass,
        FdoStringP logicalName,
        FdoStringP physicalName,
        bool bInherit,
        FdoPhysicalPropertyMapping* pPropOverrides
    );

private:
};

typedef FdoPtr<FdoSmLpGrdDataPropertyDefinition> FdoSmLpGrdDataPropertyP;

#endif

