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
#ifndef _c_KgInfSQLCommand_h
#define _c_KgInfSQLCommand_h

class c_KgInfSQLCommand : public c_KgInfFdoCommand<FdoISQLCommand>
{
public:
  c_KgInfSQLCommand(c_KgInfConnection* Connection);
public:
  ~c_KgInfSQLCommand(void);


protected:
  FdoStringP  m_SqlStr;
  
public:
    /// \brief
    /// Gets the SQL statement to be executed as a string.
    /// 
    /// \return
    /// Returns the SQL statement.
    /// 
    FDOKGINF_API virtual FdoString* GetSQLStatement();

    /// \brief
    /// Sets the SQL statement to be executed as a string.
    /// 
    /// \param value 
    /// Input the SQL statement.
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDOKGINF_API virtual void SetSQLStatement(FdoString* SqlStr);

    /// \brief
    /// Executes the SQL statement against the connection object and returns
    /// the number of rows affected.
    /// 
    /// \return
    /// Returns the number of rows affected.
    /// 
    FDOKGINF_API virtual FdoInt32 ExecuteNonQuery();

    /// \brief
    /// Executes the SQL statement against the connection object and returns
    /// an FdoISQLDataReader.
    /// 
    /// \return
    /// Returns the data reader.
    /// 
    FDOKGINF_API virtual FdoISQLDataReader* ExecuteReader();
};

#endif
