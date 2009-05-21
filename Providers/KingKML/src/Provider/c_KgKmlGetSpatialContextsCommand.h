/*
* Copyright (C) 2008  SL-King d.o.o
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

#ifndef _c_KgKmlGetSpatialContextsCommand_h
#define _c_KgKmlGetSpatialContextsCommand_h


class c_KgKmlGetSpatialContextsCommand :
    public c_KgKmlFdoCommand<FdoIGetSpatialContexts>
{


public:
    c_KgKmlGetSpatialContextsCommand ();

    c_KgKmlGetSpatialContextsCommand (c_KgKmlConnection* Connection);
    virtual ~c_KgKmlGetSpatialContextsCommand (void);

protected:
    bool m_ActiveOnly;
    
public:

    /// \brief
    ///  Gets a Boolean flag that indicates if the GetSpatialContexts command
    /// will return only the active spatial context or all spatial contexts. The
    /// default value of this flag is false, return all spatial contexts.
    /// 
    /// \return
    /// Returns Boolean value
    /// 
    FDOKGKML_API virtual const bool GetActiveOnly() ;

    /// \brief
    ///  Sets a Boolean flag that indicates if the GetSpatialContexts command
    /// will return only the active spatial context or all spatial contexts. The
    /// default value of this flag is false, return all spatial contexts.
    /// 
    /// \param value 
    /// Input the Boolean flag
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDOKGKML_API virtual void SetActiveOnly(const bool value) ;

    /// \brief
    /// Executes the GetSpatialContexts command returning an FdoISpatialContextReader.
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDOKGKML_API virtual FdoISpatialContextReader* Execute() ;
};


#endif 

