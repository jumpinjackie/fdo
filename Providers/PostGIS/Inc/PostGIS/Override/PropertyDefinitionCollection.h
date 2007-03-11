//
// Copyright (C) 2007 Refractions Research, Inc. 
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
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
//
#ifndef FDOPOSTGIS_PROPERTYDEFINITIONCOLLECTION_H_INCLUDED
#define FDOPOSTGIS_PROPERTYDEFINITIONCOLLECTION_H_INCLUDED

#include <Fdo.h>
#include <PostGIS/Override/PropertyDefinition.h>

namespace fdo { namespace postgis { namespace ov {

/// \todo To be documented
///
class PropertyDefinitionCollection :
    public FdoPhysicalElementMappingCollection<PropertyDefinition>
{
public:
    
    typedef FdoPtr<PropertyDefinitionCollection> Ptr;

    //
    // FdoPhysicalElementMappingCollection interface
    //
    
    FDOPOSTGIS_API static PropertyDefinitionCollection* Create();
    FDOPOSTGIS_API static PropertyDefinitionCollection* Create(
        FdoPhysicalElementMapping* parent);

protected:

    /// Default constructor.
    PropertyDefinitionCollection();

    /// Create collection with parent element assigned.
    PropertyDefinitionCollection(FdoPhysicalElementMapping* parent);
    
    /// Destructor.
    virtual ~PropertyDefinitionCollection();

    //
    // FdoIDisposable
    //

    void Dispose();

private:

    typedef FdoPhysicalElementMappingCollection<PropertyDefinition> Base;
};

}}} // namespace fdo::postgis::ov

#endif // FDOPOSTGIS_PROPERTYDEFINITIONCOLLECTION_H_INCLUDED
