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
#ifndef FDOPOSTGIS_CLASSCOLLECTION_H_INCLUDED
#define FDOPOSTGIS_CLASSCOLLECTION_H_INCLUDED

#include <Fdo.h>
#include <PostGIS/Override/ClassDefinition.h>

namespace fdo { namespace postgis { namespace ov {

/// \todo To be documented
class ClassCollection : public FdoPhysicalElementMappingCollection<ClassDefinition>
{
public:
    
    /// Type of FDO smart pointer for the class.
    typedef FdoPtr<ClassCollection> Ptr;

    /// Factory creates collection without parent specified.
    FDOPOSTGIS_API static ClassCollection* Create();

    /// Factory creates collection with parent specified.
    FDOPOSTGIS_API static ClassCollection* Create(FdoPhysicalElementMapping* parent);

protected:

    /// Default constructor creates class collection with no parent.
    ClassCollection();

    /// Constructor creates class collection with specified parent.
    ClassCollection(FdoPhysicalElementMapping* parent);

    /// Destructor
    virtual ~ClassCollection();

    //
    // FdoIDisposable interface
    //

    virtual void Dispose();

private:

    typedef FdoPhysicalElementMappingCollection<ClassDefinition> Base;

};

}}} // namespace fdo::postgis::ov

#endif // FDOPOSTGIS_CLASSCOLLECTION_H_INCLUDED
