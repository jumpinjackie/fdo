/*
* Copyright (C) 2006  SL-King d.o.o
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
*/


#ifndef _c_KgKmlInsert_h
#define _c_KgKmlInsert_h

class c_KgKmlInsert : public c_KgKmlFdoCommand<FdoIInsert>
{
public:
  c_KgKmlInsert(c_KgKmlConnection *Conn);
public:
  ~c_KgKmlInsert(void);
  
protected:  
    FdoPtr<FdoIdentifier>  m_ClassId;
    FdoPtr<FdoPropertyValueCollection> m_PropertyValues;
    FdoPtr<FdoBatchParameterValueCollection> m_BatchParameterValues;
    
public:  
  /// \brief
    /// Gets the name of the class to be operated upon as an FdoIdentifier.
    /// 
    /// \return
    /// Returns the class name.
    /// 
    FDOKGKML_API virtual FdoIdentifier* GetFeatureClassName();

    /// \brief
    /// Sets the name of the class to be operated upon as an FdoIdentifier.
    /// 
    /// \param value 
    /// Input the identifier for the class.
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDOKGKML_API virtual void SetFeatureClassName(FdoIdentifier* ClassId);

    /// \brief
    /// Sets the name of the class to be operated upon as an FdoIdentifier.
    /// 
    /// \param value 
    /// Input the class name.
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDOKGKML_API virtual void SetFeatureClassName(FdoString* ClassName);

    /// \brief
    /// Gets the FdoPropertyValueCollection that specifies the names and values
    /// of the properties for the instance to be inserted.
    /// 
    /// \return
    /// Returns the list of properties and their values.
    /// 
    FDOKGKML_API virtual FdoPropertyValueCollection* GetPropertyValues();

    /// \brief
    /// Gets the FdoBatchParameterValueCollection that can be used for optimized
    /// batch inserts of multiple features with a single insert command. Batch
    /// inserts can be performed by using Parameters for each of the property
    /// values, then adding collections of parameter values to the
    /// FdoBatchParameterValueCollection. Each FdoParameterValueCollection in the
    /// FdoBatchParameterValueCollection should contain one FdoParameterValue for each
    /// of the parameters specified for property values.
    /// 
    /// \return
    /// Returns FdoBatchParameterValueCollection
    /// 
    FDOKGKML_API virtual FdoBatchParameterValueCollection* GetBatchParameterValues();

    /// \brief
    /// Executes the insert command and returns a reference to an
    /// FdoIFeatureReader. Some feature providers can generate automatic identity
    /// values for features. This will happen automatically as the features are
    /// inserted. The returned FdoIFeatureReader allows the client to obtain the
    /// automatic identity property value(s) of newly inserted object(s). The
    /// returned feature reader at a minimum will read the unique identity
    /// properties of the objects just inserted. Multiple objects will be
    /// returned through the reader in the case of a batch insert.
    /// 
    /// \return
    /// Returns an FdoIFeatureReader
    /// 
    FDOKGKML_API virtual FdoIFeatureReader* Execute();
};

#endif