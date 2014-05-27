// 
//  
//  Copyright (C) 2004-2014  Autodesk, Inc.
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
//  

#ifndef SHPGETCLASSNAMESCOMMAND_H
#define SHPGETCLASSNAMESCOMMAND_H

class ShpGetClassNamesCommand :
    public FdoCommonCommand<FdoIGetClassNames, ShpConnection>
{
    friend class ShpConnection;

private:
    ShpGetClassNamesCommand (FdoIConnection *connection);
    virtual ~ShpGetClassNamesCommand (void);
    FdoStringP mSchemaName;

public:
    //
    // Prevent the use of the Assignment Operation by defining it and not implementing it.
    // DO NOT IMPLEMENT
    ShpGetClassNamesCommand & operator= (const ShpGetClassNamesCommand &right);

    /// \brief
    /// Gets the name of the schema for enumeration. This function is optional;
    /// if not specified, execution of the command will enumerate the classes in all schemas.
    /// 
    /// \return
    /// Returns the schema name
    /// 
    virtual FdoString* GetSchemaName();

    /// \brief
    /// Sets the name of the schema for the enumeration. This function is optional; if not
    /// specified execution of the command will enumerate the classes in all schemas.
    /// 
    /// \param value 
    /// Input the schema name
    /// 
    /// \return
    /// Returns nothing
    /// 
    virtual void SetSchemaName(FdoString* value);

    /// \brief
    /// Executes the GetClassNames command and returns a 
    /// FdoStringCollection. If the specified schema name does not exist,
    /// the Execute method throws an exception.
    /// 
    /// \return
    /// Returns the string collection of the fully qualified class names for the specified schema.
    virtual FdoStringCollection* Execute();
};

#endif //SHPGETCLASSNAMESCOMMAND_H