/*
* Copyright (C) 2007  Haris Kurtagic
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

#ifndef _c_KgInfGetSpatialContextsCommand_h
#define _c_KgInfGetSpatialContextsCommand_h


class c_KgInfGetSpatialContextsCommand :
    public c_KgInfFdoCommand<FdoIGetSpatialContexts>
{


public:
    c_KgInfGetSpatialContextsCommand ();

    c_KgInfGetSpatialContextsCommand (c_KgInfConnection* Connection);
    virtual ~c_KgInfGetSpatialContextsCommand (void);

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
    FDOKGINF_API virtual const bool GetActiveOnly() ;

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
    FDOKGINF_API virtual void SetActiveOnly(const bool value) ;

    /// \brief
    /// Executes the GetSpatialContexts command returning an FdoISpatialContextReader.
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDOKGINF_API virtual FdoISpatialContextReader* Execute() ;
};


#endif 

