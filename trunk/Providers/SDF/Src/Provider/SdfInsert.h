// 
//  Copyright (C) 2004-2006  Autodesk, Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
#include "SdfCommand.h"

class SdfInsert : public SdfCommand<FdoIInsert>
{

    //-------------------------------------------------------
    // Constructor / destructor
    //-------------------------------------------------------

    public:
        // constructs an Insert command using the specified connection
        SDF_API SdfInsert(SdfConnection* connection);

    protected:
        // default destructor
        SDF_API virtual ~SdfInsert();


    //-------------------------------------------------------
    // FdoIInsert implementation
    //-------------------------------------------------------

    public:
        // Gets the name of the class to be operated upon as an Identifier.
        SDF_API virtual FdoIdentifier* GetFeatureClassName();

        // Sets the name of the class to be operated upon as an Identifier.
        SDF_API virtual void SetFeatureClassName(FdoIdentifier* value);

        // Sets the name of the class to be operated upon as an Identifier.
        SDF_API virtual void SetFeatureClassName(FdoString* value);

        // Gets the PropertyValueCollection that specifies the names and values
        // of the properties for the instance to be inserted.  Returns the list
        // of properties and their values.
        SDF_API virtual FdoPropertyValueCollection* GetPropertyValues();

        // Gets the BatchParameterValueCollection that can be used for optimized
        // batch inserts of multiple features with a single insert command.  Batch
        // inserts can be performed by using Parameters for each of the property
        // values, then adding collections of parameter values to the
        // BatchParameterValueCollection.  Each ParameterValueCollection in the
        // BatchParameterValueCollection should contain one ParameterValue for
        // each of the parameters specified for property values.
        SDF_API virtual FdoBatchParameterValueCollection* GetBatchParameterValues();

        // Executes the insert command and returns a reference to an IFeatureReader.
        // Some feature providers can generate automatic identity values for features.
        // This will happen automatically as the features are inserted.  The returned
        // IFeatureReader allows the client to obtain the automatic identity property
        // value(s) of newly inserted object(s).  The returned feature reader at a
        // minimum will read the unique identity properties of the objects just
        // inserted.  Multiple objects will be returned through the reader in the
        // case of a batch insert.
        SDF_API virtual FdoIFeatureReader* Execute();

    //-------------------------------------------------------
    // Variables
    //-------------------------------------------------------

    private:
        FdoIdentifier* m_ClassName;
        FdoPropertyValueCollection* m_Properties;
		int			   m_ValidationFlag;
		bool		   m_NeedSync;

};


