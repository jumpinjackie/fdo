
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
//

#ifndef ARCSDECLASSMAPPING_H
#define ARCSDECLASSMAPPING_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

class ArcSDEClassMapping;

#include <ArcSDE/Override/FdoArcSDEPropertyMapping.h>
#include <ArcSDE/Override/FdoArcSDEIndexMapping.h>


/// \brief
/// The storage type to use when storing ArcSDE geometries in this class.
typedef enum ArcSDEGeometryStorageType
{
    ArcSDEGeometryStorageType_Spatial,
    ArcSDEGeometryStorageType_SdeBinary,
    ArcSDEGeometryStorageType_LOB,
    ArcSDEGeometryStorageType_WKB,
    ArcSDEGeometryStorageType_SQL,
    ArcSDEGeometryStorageType_Normalized
};


class ArcSDEClassMapping : public FdoPhysicalClassMapping
{
protected:
    /// \brief
    /// The constructor for the ArcSDEClassMapping class.
    /// 
    /// \return
    /// Returns nothing.
    /// 
    FDOSDE_API ArcSDEClassMapping(void);

public:
    /// \brief
    /// The destructor for the ArcSDEClassMapping class.
    /// 
    /// \return
    /// Returns nothing.
    /// 
    FDOSDE_API virtual ~ArcSDEClassMapping(void);

    /// \brief
    /// Creates a new instance of the ArcSDEClassMapping class.
    /// 
    /// \return
    /// Returns a new instance of the ArcSDEClassMapping class.
    /// 
    static FDOSDE_API ArcSDEClassMapping* Create(void);

    /// \brief
    /// Dispose this object.
    /// 
    /// \return
    /// Returns nothing.
    /// 
    FDOSDE_API void Dispose(void)  { delete this; }

    ///////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////
    /// Non-inherited public methods:

    /// \brief
    /// Returns the current table name override for this class.
    /// 
    /// \return
    /// Returns the current table name override for this class.
    /// 
    FDOSDE_API FdoString* GetTableName();

    /// \brief
    /// Sets the current table name override for this class.
    /// 
    /// \return
    /// Returns nothing.
    /// 
    FDOSDE_API void SetTableName(FdoString*);

    /// \brief
    /// Returns the current owner name override for this class.
    /// 
    /// \return
    /// Returns the current owner name override for this class.
    /// 
    FDOSDE_API FdoString* GetOwnerName();

    /// \brief
    /// Sets the current owner name override for this class.
    /// 
    /// \return
    /// Returns nothing.
    /// 
    FDOSDE_API void SetOwnerName(FdoString*);

    /// \brief
    /// Returns the current database name override for this class.
    /// 
    /// \return
    /// Returns the current database name override for this class.
    /// 
    FDOSDE_API FdoString* GetDatabaseName();

    /// \brief
    /// Sets the current database name override for this class.
    /// 
    /// \return
    /// Returns nothing.
    /// 
    FDOSDE_API void SetDatabaseName(FdoString*);

    /// \brief
    /// Returns the geometry storage type override for this class.
    /// 
    /// \return
    /// Returns the geometry storage type override for this class.
    /// 
    FDOSDE_API ArcSDEGeometryStorageType GetGeometryStorageType();

    /// \brief
    /// Sets the geometry storage type override for this class.
    /// 
    /// \return
    /// Returns nothing.
    /// 
    FDOSDE_API void SetGeometryStorageType(ArcSDEGeometryStorageType type);

    /// \brief
    /// Returns the property mappings for this class.
    /// 
    /// \return
    /// Returns the property mappings for this class.
    /// 
    FDOSDE_API ArcSDEPropertyMappingCollection* GetProperties();

    /// \brief
    /// Returns the index mappings for this class.
    /// 
    /// \return
    /// Returns the index mappings for this class.
    /// 
    FDOSDE_API ArcSDEIndexMappingCollection* GetIndexes();


    ///////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////
    /// Functions for XML support:

    /// \brief
    /// Initialize this element from its XML attributes.
    /// 
    /// \return
    /// Returns nothing.
    /// 
    FDOSDE_API virtual void InitFromXml(FdoXmlSaxContext* pContext, FdoXmlAttributeCollection* attrs);

    /// \brief
    /// Handle the start of a sub-element.
    /// 
    /// \return
    /// Returns nothing.
    /// 
    FDOSDE_API virtual FdoXmlSaxHandler* XmlStartElement(
        FdoXmlSaxContext* context, 
        FdoString* uri, 
        FdoString* name, 
        FdoString* qname, 
        FdoXmlAttributeCollection* atts
    );

    /// \brief
    /// Handle the end of a sub-element.
    /// 
    /// \return
    /// Returns nothing.
    /// 
    FDOSDE_API virtual FdoBoolean XmlEndElement(
        FdoXmlSaxContext* context, 
        FdoString* uri, 
        FdoString* name, 
        FdoString* qname
    );

protected:
    /// \brief
    /// The geometry storage type for this class.
    /// 
    ArcSDEGeometryStorageType  m_GeometryStorageType;

    /// \brief
    /// The property mapping collection for this class.
    /// 
    FdoPtr<ArcSDEPropertyMappingCollection>  m_PropertyMappings;

    /// \brief
    /// The index mapping collection for this class.
    /// 
    FdoPtr<ArcSDEIndexMappingCollection>  m_IndexMappings;

    /// \brief
    /// The ArcSDE table name for this class.
    /// 
    FdoStringP m_TableName;

    /// \brief
    /// The ArcSDE owner name for this class.
    /// 
    FdoStringP m_OwnerName;

    /// \brief
    /// The ArcSDE database name for this class.
    /// 
    FdoStringP m_DatabaseName;
};


class ArcSDEClassMappingCollection : public FdoPhysicalElementMappingCollection<ArcSDEClassMapping>
{
protected:
    /// 0-argument constructor to please FdoPtr::operator->() :
    ArcSDEClassMappingCollection () { ArcSDEClassMappingCollection(NULL); }

    /// \brief
    /// The constructor for the ArcSDEClassMappingCollection class.
    /// 
    /// \return
    /// Returns nothing.
    /// 
    FDOSDE_API ArcSDEClassMappingCollection(ArcSDESchemaMapping* parent) : FdoPhysicalElementMappingCollection<ArcSDEClassMapping>((FdoPhysicalElementMapping*)parent) {}

public:
    /// \brief
    /// Dispose this object.
    /// 
    /// \return
    /// Returns nothing.
    /// 
    FDOSDE_API void Dispose(void)  { delete this; }

    /// \brief
    /// Creates an instance of the ArcSDEClassMappingCollection class.
    /// 
    /// \return
    /// Returns nothing.
    /// 
    static FDOSDE_API ArcSDEClassMappingCollection* Create(ArcSDESchemaMapping* parent);
};


#endif //ARCSDECLASSMAPPING_H


