#ifndef FDOSMSCHEMAELEMENT_H
#define FDOSMSCHEMAELEMENT_H		1
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


#ifdef _WIN32
#pragma once
#endif

#include <Sm/ObjectState.h>
#include <Sm/Disposable.h>
#include <Sm/Base.h>

class FdoSmSchema;
class FdoSmErrorCollection;

// SchemaElement is an abstract class that acts as a base class for all classes
// for LogicalPhysical or Physical schema elements.
class FdoSmSchemaElement : public FdoSmDisposable, public FdoSmBase
{
  public:

    /// Gets the parent of this SchemaObject or null if this object has not been
    /// added to a parent object.
    const FdoSmSchemaElement* GetParent() const;

    /// Returns the name of the schema object.
    FdoString* GetName() const;

    /// Get the fully qualified name for this element.
    /// This varies between element type so some types override this
    /// functions
	virtual FdoStringP GetQName() const;

    /// Returns the description of the schema object.
    virtual FdoString* GetDescription() const;

    /// Returns the errors in the definition for this schema object.
    const FdoSmErrorCollection* RefErrors() const;
    FdoPtr<FdoSmErrorCollection> GetErrors();

    /// Get the modification state ( adding, deleting, modifying, no change )
    /// for this element,
    FdoSchemaElementState GetElementState() const;

    /// Returns the object state of this schema object.
    /// The object state tells whether the element has been initialized or 
    /// is currently being initialized.
	FdoSmObjectState GetState() const;

    /// Post outstanding modifications to the database.
    /// Base function does nothing by default.
	virtual void Commit( bool fromParent = false ) {}

	virtual FdoSchemaExceptionP Errors2Exception( FdoSchemaException* pFirstException = NULL ) const;

	void SetDescription(FdoString* description)
	{
		mDescription = description;
	}

    /// Set the modification state.
	virtual void SetElementState(FdoSchemaElementState elementState);

    /// Mainly for Unit Testing.
    /// Serializes this schema element to the given XML file.
	virtual void XMLSerialize( FILE* xmlFp, int ref ) const;

    FdoBoolean CanSetName()
    {
        return false;
    };

protected:
    /// unused constructor needed only to build on Linux
    FdoSmSchemaElement();

    /// Constructs an instance of a SchemaElement using the specified arguments.
    FdoSmSchemaElement(FdoString* name, FdoString* description, const FdoSmSchemaElement* parent = NULL );

    ~FdoSmSchemaElement(void);

    /// Update the state of this element.
	void SetState(FdoSmObjectState objectState);

  	void AddFinalizeLoopError( void );

	FdoPtr<FdoSmErrorCollection> mErrors;

private:

    /// Parent element
    const FdoSmSchemaElement* mpParent;

    /// The name of the schema object. This name is used to reference the schema
    /// object within the scope of the containing schema object.
    FdoStringP mName;

    /// Optional description of the schema object.
    FdoStringP mDescription;

	FdoSchemaElementState mElementState;

    /// The state of this element.
	FdoSmObjectState mObjectState;

};

#endif


