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
#ifndef FDOPOSTGISOVCLASSDEFINITION_H
#define FDOPOSTGISOVCLASSDEFINITION_H

#include <Rdbms/Override/RdbmsOvClassDefinition.h>
#include <Rdbms/Override/PostGis/PostGisOv.h>
#include <Rdbms/Override/PostGis/PostGisOvDataPropertyDefinition.h>
#include <Rdbms/Override/PostGis/PostGisOvGeometricPropertyDefinition.h>
#include <Rdbms/Override/PostGis/PostGisOvObjectPropertyDefinition.h>
#include <Rdbms/Override/PostGis/PostGisOvPropertyDefinitionCollection.h>
#include <Rdbms/Override/PostGis/PostGisOvTable.h>
#include <Rdbms/Override/PostGis/PostGisOvGeometricColumn.h>

/// \brief
/// Concrete class defining physical schema overrides for a class definition.
///
class FdoPostGISOvClassDefinition : public FdoRdbmsOvClassDefinition
{
public:

    /// Named constructor for FdoPostGISOvClassDefinition.
    /// 
    /// \return
    /// Pointer to new instance of FdoPostGISOvClassDefinition.
    /// 
    FDOPOSTGIS_OV_API static FdoPostGISOvClassDefinition* Create();

    /// Named constructor for FdoPostGISOvClassDefinition.
    /// 
    /// \param
    /// name [in] - name of class definition.
    ///
    /// \return
    /// Pointer to new instance of FdoPostGISOvClassDefinition.
    /// 
    FDOPOSTGIS_OV_API static FdoPostGISOvClassDefinition* Create(FdoString* name);

    /// Gets collection of overrides properties.
    /// 
    /// \return
    /// Collection of PostGIS property overrides
    /// 
    FDOPOSTGIS_OV_API FdoPostGISOvPropertyDefinitionCollection* GetProperties();

    /// Gets table override for this class definition.
    /// 
    /// \return
    /// PostGIS table override for class definition.
    /// 
    FDOPOSTGIS_OV_API FdoPostGISOvTable* GetTable();

    /// Sets the PostGIS table override for this class override.
    /// 
    /// \param
    /// name [in] - input PostGIS table override.
    /// 
    FDOPOSTGIS_OV_API void SetTable(FdoPostGISOvTable* table);

    /// Sets the parent of this object. Not part of the API.
    ///
    /// \param
    /// parent [in] - 
    ///
    void SetParent(FdoPhysicalElementMapping* parent);

/// \cond DOXYGEN-IGNORE

    /// XML serialization functions, not part of the API.

    /// Initialize this element from its XML attributes
    ///
    /// \param
    /// saxContext [in] - 
    ///
    /// \param
    /// attrs [in] - 
    ///
    virtual void InitFromXml(FdoXmlSaxContext* saxContext,
        FdoXmlAttributeCollection* attrs);

    /// Serialize the XML attributes of this class.
    ///
    /// \param
    /// xmlWriter [in] - 
    ///
    /// \param
    /// flags [in] - 
    ///
    virtual void _writeXmlAttributes(FdoXmlWriter* xmlWriter,
        const FdoXmlFlags* flags);

/// \endcond

protected:

    /// Default constructor.
    FdoPostGISOvClassDefinition();

    /// Alternative constructor.
    /// \param name [in] - name of class definition.
    FdoPostGISOvClassDefinition(FdoString *name);

    /// Destructor
    virtual ~FdoPostGISOvClassDefinition();

    /// Named constructor.
    /// Called from class constructors.
    void Init();

    /// Named destructor.
    virtual void Dispose();

    /// Create table override from XML attributes
    virtual void SetTable(
        FdoXmlSaxContext* saxContext,
        FdoXmlAttributeCollection* atts);

    /// Create data property override from XML attributes
    virtual FdoRdbmsOvDataPropertyDefinition* CreateDataProperty( 
        FdoXmlSaxContext* saxContext,
        FdoXmlAttributeCollection* propAtts,
        FdoXmlAttributeCollection* colAtts);

    /// Create geometric property override from XML attributes
    virtual FdoRdbmsOvGeometricPropertyDefinition* CreateGeometricProperty( 
        FdoXmlSaxContext* saxContext,
        FdoXmlAttributeCollection* propAtts,
        FdoXmlAttributeCollection* colAtts
    );

    /// Create object property override from XML attributes
    virtual FdoRdbmsOvObjectPropertyDefinition* CreateObjectProperty( 
        FdoXmlSaxContext* saxContext,
        FdoXmlAttributeCollection* propAtts,
        FdoString* mappingType,
        FdoXmlAttributeCollection* mappingAtts
    );

    /// Add the given property override to this class override
    virtual void AddProperty(FdoRdbmsOvPropertyDefinition* propDef);

private:

    /// Collection with properties definition.
    FdoPostGISOvPropertiesP mPropertyDefinitions;
};

/// \brief
/// Declaration of smart-pointer type for FdoPostGISOvClassDefinition.
/// Provided for convenience.
typedef FdoPtr<FdoPostGISOvClassDefinition> FdoPostGISOvClassP;

#endif // FDOPOSTGISOVCLASSDEFINITION_H
