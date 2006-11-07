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
#ifndef _c_KgOraSQLCommand_h
#define _c_KgOraSQLCommand_h

class c_KgOraSQLCommand : public c_KgOraFdoCommand<FdoISQLCommand>
{
public:
  c_KgOraSQLCommand(c_KgOraConnection* Connection);
public:
  ~c_KgOraSQLCommand(void);


protected:
  FdoStringP  m_SqlStr;
  
public:
    /// \brief
    /// Gets the SQL statement to be executed as a string.
    /// 
    /// \return
    /// Returns the SQL statement.
    /// 
    FDOKGORA_API virtual FdoString* GetSQLStatement();

    /// \brief
    /// Sets the SQL statement to be executed as a string.
    /// 
    /// \param value 
    /// Input the SQL statement.
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDOKGORA_API virtual void SetSQLStatement(FdoString* SqlStr);

    /// \brief
    /// Executes the SQL statement against the connection object and returns
    /// the number of rows affected.
    /// 
    /// \return
    /// Returns the number of rows affected.
    /// 
    FDOKGORA_API virtual GisInt32 ExecuteNonQuery();

    /// \brief
    /// Executes the SQL statement against the connection object and returns
    /// an FdoISQLDataReader.
    /// 
    /// \return
    /// Returns the data reader.
    /// 
    FDOKGORA_API virtual FdoISQLDataReader* ExecuteReader();
};

#endif
