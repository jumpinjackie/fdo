#ifndef FDOSMLPSQSCLASS_H
#define FDOSMLPSQSCLASS_H       1
/*
 * Copyright (C) 2004-2008  Autodesk, Inc.
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

#include <Sm/Lp/Class.h>
#include "../../../SchemaMgr/Lp/ClassDefinition.h"
#include "ClassDefinition.h"

// SqlServer Provider's non-feature class definition
class FdoSmLpSqsClass : public FdoSmLpClass, public FdoSmLpSqsClassDefinition
{
public:
    // Construct from a class reader.
    FdoSmLpSqsClass(FdoSmPhClassReaderP classReader, FdoSmLpSchemaElement* parent);

	// Construct from an FDO class
    FdoSmLpSqsClass(
        FdoClass* pFdoClass, 
        bool bIgnoreStates,
        FdoSmLpSchemaElement* parent
    );

    // Apply updates to this non-feature class
    virtual void Update(
        FdoClassDefinition* pFdoClass,
        FdoSchemaElementState elementState,
        FdoPhysicalClassMapping* pClassOverrides,
        bool bIgnoreStates
    );

protected:
    virtual ~FdoSmLpSqsClass();

    FdoSmPhClassWriterP GetPhysicalAddWriter();

    virtual void PostFinalize();

};

#endif


