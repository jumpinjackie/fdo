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

#pragma  once

#include "FDO\Schema\mgPropertyDefinition.h"

#include "FDO\Schema\mgDeleteRule.h"
#include "FDO\Schema\mgPropertyType.h"

class FdoAssociationPropertyDefinition;

BEGIN_NAMESPACE_OSGEO_FDO_SCHEMA
public __gc class DataPropertyDefinitionCollection;
public __gc class ClassDefinition;

/// \ingroup (OSGeoFDOSchema)
/// \brief
/// AssociationPropertyDefinition class derives PropertyDefinition and represents
/// the association between two classes.
/// The class of the associated class must already be defined in the feature
/// schema and cannot be abstract.
public __gc class AssociationPropertyDefinition : public NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinition
{
public:
    /// \brief
    /// Constructs a default instance of an AssociationPropertyDefinition.
    /// 
	AssociationPropertyDefinition();

    /// \brief
    /// Constructs an instance of an AssociationPropertyDefinition using the specified
    /// arguments.
    /// 
    /// \param name 
    /// Input the property definition name
    /// \param description 
    /// Input the property definition description
    /// 
	AssociationPropertyDefinition(System::String* name, System::String* description);

    /// \brief
    /// Constructs an instance of an AssociationPropertyDefinition using the specified
    /// arguments.
    /// 
    /// \param name 
    /// Input the property definition name
    /// \param description 
    /// Input the property definition description
    /// \param system 
    /// Input true if this is a system generated property, false otherwise.
    /// <p><b>Note:</b> A client would never set system to true, only a provider.
    /// 
	AssociationPropertyDefinition(System::String* name, System::String* description, System::Boolean system);

    /// \brief
    /// Gets the concrete property type.
    /// 
    /// \return
    /// Returns the concrete property type
    /// 
	__property NAMESPACE_OSGEO_FDO_SCHEMA::PropertyType get_PropertyType();

    /// \brief
    /// Gets a reference to the associated class.
    /// 
    /// \return
    /// Returns class definition
    /// 
	__property NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition* get_AssociatedClass();

    /// \brief
    /// Sets a reference to the associated class.
    /// 
    /// \param value 
    /// Input class definition
    /// 
	__property System::Void set_AssociatedClass(NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition* value);

    /// \brief
    /// Gets a reference to an NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinitionCollection which is a collection of 
    /// properties of the current class that are used as key for this association. Initially, this 
    /// collection is empty. The user can optionally add any number of properties.  If the collection 
    /// is left empty, the identity properties of the associated class are added to the current class. 
    /// The number, order and types should match the property of the ReverseIdentityProperties collection. 
    /// All properties in the collection should already exist in the containing class. 
    /// This is needed in case the current class already has properties (foreign keys) that are used 
    /// to reference the associated feature.
    /// 
    /// \return
    /// Returns data property definition
    /// 
	__property NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinitionCollection* get_IdentityProperties();

    /// \brief
    /// Gets a reference to an NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinitionCollection used to return a collection 
    /// of properties of the associated class that are used as key for this association. 
    /// The number, order and types should match the IdentityProperties. If the reverse identity 
    /// collection is empty, then the associated class identity properties will be used. 
    /// The properties of the collection should already exist on the associated class.
    /// 
    /// \return
    /// Returns data property definition
    /// 
	__property NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinitionCollection* get_ReverseIdentityProperties();

    /// \brief
    /// Gets the reverse name of this property. See description of SetReverseName.
    /// 
    /// \return
    /// Returns the reverse name
    /// 
	__property System::String* get_ReverseName();

    /// \brief
    /// This method is used to set the name of this association as seen by the associated 
    /// class. This is an optional parameter that can be provided if the navigation back from the 
    /// associated class is needed. This property will appear as read-only property on the associated 
    /// class. For example let’s assume that we have an Accident class that associated to a Road class 
    /// by an association called “road? If we wanted to find all the accidents associated to a given 
    /// road, we will need to provide a reverse property name of the road association property. 
    /// In this case an appropriate value for such a name would be “accident? The filter that can be 
    /// used to find all the accidents would look like: “accident not null?
    /// 
    /// \param name 
    /// Input reverse name
    /// 
	__property System::Void set_ReverseName(System::String* name);

    /// \brief
    /// Gets the delete rule.
    /// 
    /// \return
    /// Returns the delete rule
    /// 
	__property NAMESPACE_OSGEO_FDO_SCHEMA::DeleteRule get_DeleteRule();

    /// \brief
    /// Sets the delete rule.
    /// 
    /// \param value 
    /// Input the delete rule
    /// 
	__property System::Void set_DeleteRule(NAMESPACE_OSGEO_FDO_SCHEMA::DeleteRule value);

    /// \brief
    /// Gets the lock cascade option. true to cascade the lock otherwise the lock is not cascaded
    /// 
    /// \return
    /// Returns the lock cascade option
    /// 
	__property System::Boolean get_LockCascade();

    /// \brief
    /// Sets the lock cascade option.
    /// 
    /// \param value 
    /// Input the lock cascade option.
    /// 
	__property System::Void set_LockCascade(System::Boolean value);

    /// \brief
    /// This method is used to verify if this association property is a read-only association 
    /// property. A read-only association property is created internally when the reverse name of an 
    /// association property is specified. The read-only association property is added automatically 
    /// to the associated class and can be used to navigate from the associated class to the 
    /// associating class
    /// 
    /// \return
    /// Returns true if the association property is read-only. otherwise false.
    /// 
	__property System::Boolean get_IsReadOnly();

    /// \brief
    /// This method is used to set the read-only status of the association property. 
    /// This method should only be called by FDO providers to set to read-only the association 
    /// property added internally to the associated class that represent the reverse association 
    /// property.
    /// 
    /// \param value 
    /// Input the read-only flag
    /// 
	__property System::Void set_IsReadOnly(System::Boolean value);

    /// \brief
    /// This method is used to return the association multiplicity from the owner class 
    /// side. See SetMultiplicity method for description of the possible values.
    /// 
    /// \return
    /// Returns a System::String that represent the multiplicity.
    /// 
	__property System::String* get_Multiplicity();

    /// \brief
    /// This method is used to set the association multiplicity from the property owner class side. 
    /// The only possible values are 1 or m. If the multiplicity is set to ?? then only one instance
    /// of the owning class can be associated to a given instance of the associated class. If the 
    /// multiplicity is set to “m? then many instances of the owning class can be associated to the 
    /// same instance of the associated class.
    /// 
    /// \param value 
    /// The multiplicity from the property owner class side. Valid values are:  ?? or “m?
    /// 
	__property System::Void set_Multiplicity(System::String* value);

    /// \brief
    /// This method is used to return the association multiplicity from the associated class 
    /// side. See SetReverseMultiplicity method for description of the possible values.
    /// 
    /// \return
    /// Returns a System::String that represent the reverse multiplicity.
    /// 
	__property System::String* get_ReverseMultiplicity();

    /// \brief
    /// This method is used to set the association multiplicity from the associated class side.
    /// The only possible values are 0 or 1. If the multiplicity is set to ?? then it is not mandatory 
    /// to initialize the association property when a new object is created. If the multiplicity is 
    /// set to ?? then the association property must be initialized when a new object is created.
    /// 
    /// \param value 
    /// The multiplicity from the associated class side. Valid values are:  ?? or ??
    /// 
	__property System::Void set_ReverseMultiplicity(System::String* value);

    /// \brief
    /// Constructs a AssociationPropertyDefinition object based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed object should be automatically deleted 
    /// once it no longer referenced.
    /// 
	AssociationPropertyDefinition(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinition(unmanaged, autoDelete)
	{

	}

public private:
	inline FdoAssociationPropertyDefinition* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_SCHEMA


