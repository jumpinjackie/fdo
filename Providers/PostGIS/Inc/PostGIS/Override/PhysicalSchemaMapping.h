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
#ifndef FDOPOSTGIS_PHYSICALSCHEMAMAPPING_H_INCLUDED
#define FDOPOSTGIS_PHYSICALSCHEMAMAPPING_H_INCLUDED

#include <PostGIS/Override/ClassCollection.h>

namespace fdo { namespace postgis { namespace ov {

// Forward declarations
class ClassDefinition;
class ClassCollection;
    
/// \todo To be documented
class PhysicalSchemaMapping : public FdoPhysicalSchemaMapping
{
public:

    typedef FdoPtr<PhysicalSchemaMapping> Ptr;

    FDOPOSTGIS_API static PhysicalSchemaMapping* Create();

    FDOPOSTGIS_API virtual FdoString* GetProvider();
    
    FDOPOSTGIS_API ClassCollection* GetClasses() const; 

    /// Find a feature class by name
    FDOPOSTGIS_API ClassDefinition* FindByClassName(FdoStringP const& name);

protected:

    PhysicalSchemaMapping();
    virtual ~PhysicalSchemaMapping();

    //
    // FdoIDisposable interface
    //

    virtual void Dispose();

private:

    ClassCollection::Ptr mClasses;

    typedef FdoPhysicalSchemaMapping BaseType;
};

}}} // namespace fdo::postgis::ov

#endif // FDOPOSTGIS_PHYSICALSCHEMAMAPPING_H_INCLUDED
