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


#ifndef _c_KgOraCreateDataStore_h
#define _c_KgOraCreateDataStore_h

class c_KgOraCreateDataStore : public c_KgOraFdoCommand<FdoICreateDataStore>
{
public:
  c_KgOraCreateDataStore(c_KgOraConnection *Conn);
public:
  ~c_KgOraCreateDataStore(void);
  
    
public:  
  
   /// \brief
    /// Gets the FdoIDataStorePropertyDictionary interface that	
    /// can be used to dynamically query	and	set	the	properties required	
    /// to create a new datastore.
    /// 
    /// \return
    /// Returns the property dictionary
    /// 
	FDOKGORA_API	virtual	FdoIDataStorePropertyDictionary* GetDataStoreProperties();

    /// \brief
    /// Executes the FdoICreateDataStore command.
    /// 
    /// \return
    /// Returns nothing
    /// 
	FDOKGORA_API	virtual	void Execute();
};

#endif
