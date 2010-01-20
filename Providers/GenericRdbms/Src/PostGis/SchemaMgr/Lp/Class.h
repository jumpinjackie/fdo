//
// Copyright (C) 2006 Refractions Research, Inc.
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
//
#ifndef FDOSMLPPOSTGISCLASS_H
#define FDOSMLPPOSTGISCLASS_H

#include <Sm/Lp/Class.h>
#include "ClassDefinition.h"

/// Defines non-spatial feature class.
/// This class is used by PostGIS provider.
///
class FdoSmLpPostGisClass :
    public FdoSmLpClass,
    public FdoSmLpPostGisClassDefinition
{
public:
   
    /// Construct from a class reader.
    ///
    /// \param
    /// classReader [in] - pointer to class reader instance.
    ///
    /// \param
    /// parent [in] - pointer to instance of parent schema element.
    ///
    FdoSmLpPostGisClass(FdoSmPhClassReaderP classReader,
        FdoSmLpSchemaElement* parent);

	/// Construct from FDO base class
    ///
    /// \param
    /// fdoClass [in] - pointer to instance of the FDO class
    ///
    /// \param
    /// ignoreStates [in] - 
    ///
    /// \param
    /// parent [in] - pointer to instance of parent schema element.
    ///
    FdoSmLpPostGisClass(FdoClass* fdoClass, bool ignoreStates,
        FdoSmLpSchemaElement* parent);

    /// Apply updates to this non-feature class
    ///
    /// \param
    /// fdoClass [in] - pointer to instance of the FDO class
    ///
    /// \param
    /// state [in] - 
    ///
    /// \param
    /// classOverrides [in] - 
    ///
    /// \param
    /// ignoreStates [in] - 
    ///
    virtual void Update(FdoClassDefinition* fdoClass,
        FdoSchemaElementState state,
        FdoPhysicalClassMapping* classOverrides,
        bool ignoreStates);

protected:

    /// Destructor.
    virtual ~FdoSmLpPostGisClass();

    /// \return
    /// Class writer object.
    FdoSmPhClassWriterP GetPhysicalAddWriter();

    /// 
    void PostFinalize();

};

#endif // FDOSMLPPOSTGISCLASS_H