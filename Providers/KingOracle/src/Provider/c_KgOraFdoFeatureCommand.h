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

#ifndef _c_KgOraFdoFeatureCommand_h
#define _c_KgOraFdoFeatureCommand_h


#include "c_KgOraFdoCommand.h"

template <class FDO_COMMAND> class c_KgOraFdoFeatureCommand : public c_KgOraFdoCommand<FDO_COMMAND>
{
    

protected:
    FdoPtr<FdoFilter> m_Filter;         
    FdoPtr<FdoIdentifier> m_ClassName;  
    FdoPtr<FdoPropertyValueCollection> m_PropValues;  

protected:

    /// Constructs an instance of a command for the given connection.    
    c_KgOraFdoFeatureCommand (c_KgOraConnection* Connection) :
        c_KgOraFdoCommand<FDO_COMMAND>(Connection)  
    {
    }

    /// Default destructor for a command.
    virtual ~c_KgOraFdoFeatureCommand() 
    { 
    }

    virtual void Dispose()
    {
        delete this;
    }

    /// \brief
    /// Gets the FdoPropertyValueCollection that specifies the names and values
    /// of the properties for the instance to be inserted OR updated.
    /// 
    /// \return
    /// Returns the list of properties and their values.
    /// 
    virtual FdoPropertyValueCollection* GetPropertyValues ()
    {
    /// allocate a new property value collection if not yet done
        if (m_PropValues == NULL)
            m_PropValues = FdoPropertyValueCollection::Create ();

        return (FDO_SAFE_ADDREF(m_PropValues.p));
    }

public:

    /// \brief
    /// Gets the filter as a filter tree.
    /// 
    /// \return
    /// Returns the filter object
    /// 
    virtual FdoFilter* GetFilter ();

    /// \brief
    /// Sets the filter as a Filter tree.
    /// 
    /// \param value 
    /// Input the filter object
    /// 
    /// \return
    /// Returns nothing
    /// 
    virtual void SetFilter (FdoFilter* value);

    /// \brief
    /// Sets the filter as a Filter tree.
    /// 
    /// \param value 
    /// Input the filter expression string
    /// 
    /// \return
    /// Returns nothing
    /// 
    virtual void SetFilter (GisString* value);

    /// \brief
    /// Gets the name of the class to be operated upon as an identifier.
    /// 
    /// \return
    /// Returns the class identifier.
    /// 
    virtual FdoIdentifier* GetFeatureClassName ();

    /// \brief
    /// Sets the name of the class to be operated upon as an identifier.
    /// 
    /// \param value 
    /// Input the class identifier
    /// 
    /// \return
    /// Returns nothing.
    /// 
    virtual void SetFeatureClassName (FdoIdentifier* value);

    /// \brief
    /// Sets the name of the class to be operated upon as an identifier.
    /// 
    /// \param value 
    /// Input the class name
    /// 
    /// \return
    /// Returns nothing
    /// 
    virtual void SetFeatureClassName (GisString* value);
};



/// \brief
/// Gets the filter as a filter tree.
/// \return
/// Returns the filter object
template <class FDO_COMMAND> 
FdoFilter* c_KgOraFdoFeatureCommand<FDO_COMMAND>::GetFilter ()
{
    return (FDO_SAFE_ADDREF(m_Filter.p));
}

/// \brief
/// Sets the filter as a Filter tree.
/// <param name="value">
/// Input the filter object
/// </param>
/// \return
/// Returns nothing
template <class FDO_COMMAND> 
void c_KgOraFdoFeatureCommand<FDO_COMMAND>::SetFilter (FdoFilter* Filter)
{
    m_Filter = FDO_SAFE_ADDREF(Filter);
}

/// \brief
/// Sets the filter as a Filter tree.
/// <param name="value">
/// Input the filter expression string
/// </param>
/// \return
/// Returns nothing
template <class FDO_COMMAND> 
void c_KgOraFdoFeatureCommand<FDO_COMMAND>::SetFilter (GisString* value)
{
    FdoPtr<FdoFilter> filter = FdoFilter::Parse (value);
    m_Filter = FDO_SAFE_ADDREF(filter.p);
}


/// \brief
/// Gets the name of the class to be operated upon as an identifier.
/// \return
/// Returns the class identifier.
template <class FDO_COMMAND> 
FdoIdentifier* c_KgOraFdoFeatureCommand<FDO_COMMAND>::GetFeatureClassName ()
{
    /// addref the class name because we own it
    return (FDO_SAFE_ADDREF(m_ClassName.p));
}


/// \brief
/// Sets the name of the class to be operated upon as an identifier.
/// <param name="value">
/// Input the class identifier
/// </param>
/// \return
/// Returns nothing.
template <class FDO_COMMAND> 
void c_KgOraFdoFeatureCommand<FDO_COMMAND>::SetFeatureClassName (FdoIdentifier* ClassName)
{
    /// Store the given feature class name:
    m_ClassName = FDO_SAFE_ADDREF(ClassName);  // does automatic release
}


/// \brief
/// Sets the name of the class to be operated upon as an identifier.
/// <param name="value">
/// Input the class name
/// </param>
/// \return
/// Returns nothing
template <class FDO_COMMAND> 
void c_KgOraFdoFeatureCommand<FDO_COMMAND>::SetFeatureClassName (GisString* ClassName)
{
    FdoPtr<FdoIdentifier> cname;

    if (ClassName != NULL)
        cname = FdoIdentifier::Create (ClassName);
    else
        cname = NULL;
    SetFeatureClassName (cname);
}


#endif 


