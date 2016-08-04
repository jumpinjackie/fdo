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
#ifndef FDOPOSTGISOVPHYSICALSCHEMAMAPPING_H
#define FDOPOSTGISOVPHYSICALSCHEMAMAPPING_H

#include <Rdbms/Override/RdbmsOvPhysicalSchemaMapping.h>
#include <Rdbms/Override/PostGis/PostGisOv.h>
#include <Rdbms/Override/PostGis/PostGisOvClassCollection.h>

/// \brief
/// Concrete class offering physical schema mapping overrides.
/// This class is used in PostGIS schema overrides.
///
class FdoPostGISOvPhysicalSchemaMapping :
    public FdoRdbmsOvPhysicalSchemaMapping
{
public:

    /// \brief
    /// Constructs a default of an FdoPostGISOvPhysicalSchemaMapping.
    /// 
    /// \return
    /// Returns FdoPostGISOvPhysicalSchemaMapping
    /// 
    FDOPOSTGIS_OV_API static FdoPostGISOvPhysicalSchemaMapping* Create();

    /// \brief
    /// Constructs an instance of an FdoPostGISOvPhysicalSchemaMapping
    /// using the specified arguments.
    /// 
    /// \param name 
    /// Input name
    /// 
    /// \return
    /// Returns FdoPostGISOvPhysicalSchemaMapping
    /// 
    FDOPOSTGIS_OV_API static FdoPostGISOvPhysicalSchemaMapping* Create(
        FdoString* name);

    /// \brief
    /// Gets the classes
    /// 
    /// \return
    /// Returns the classes
    /// 
    FDOPOSTGIS_OV_API FdoPostGISOvClassCollection* GetClasses();

    /// \brief
    /// Gets the owner
    /// 
    /// \return
    /// Returns the database
    /// 
    FDOPOSTGIS_OV_API FdoString* GetOwner();

    /// \brief
    /// Sets the owner
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDOPOSTGIS_OV_API void SetOwner(FdoString* owner);

    /// \brief
    /// Gets the provider name
    /// 
    /// \return
    /// Returns the provider name
    /// 
    FDOPOSTGIS_OV_API virtual FdoString* GetProvider(void);

/// \cond DOXYGEN-IGNORE

    /// XML Serialization functions, not part of API.

    /// Serialize this property to XML.
    virtual void _writeXmlAttributes(FdoXmlWriter* xmlWriter,
        const FdoXmlFlags* flags);

    /// Initialize this element from its XML attributes
    virtual void InitFromXml(FdoXmlSaxContext* saxContext,
        FdoXmlAttributeCollection* attrs);

/// \endcond

protected:

    FdoPostGISOvPhysicalSchemaMapping();
    FdoPostGISOvPhysicalSchemaMapping(FdoString* name);
    virtual ~FdoPostGISOvPhysicalSchemaMapping();
    virtual void Dispose();

    /// Create a class override from XML attributes
    virtual FdoRdbmsOvClassDefinition* CreateClass(
        FdoXmlSaxContext* saxContext,
        FdoXmlAttributeCollection* atts);

    /// Add the given class override to this schema override set
    virtual void AddClass(FdoRdbmsOvClassDefinition* classDef);

    /// common initializer for all constructors:
    void Init();

private:

    FdoPostGISOvClassesP mClasses;

    FdoStringP mOwner;
};

/// \brief
/// Declaration of smart-pointer type for FdoPostGISOvGeometricColumn.
/// Provided for convenience.
///
typedef FdoPtr<FdoPostGISOvPhysicalSchemaMapping> FdoPostGISOvSchemaMappingP;

#endif // FDOPOSTGISOVPHYSICALSCHEMAMAPPING_H
