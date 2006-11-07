#include "StdAfx.h"
#include "c_KgOraCreateDataStore.h"

c_KgOraCreateDataStore::c_KgOraCreateDataStore(c_KgOraConnection *Conn)
  : c_KgOraFdoCommand(Conn)
{
}

c_KgOraCreateDataStore::~c_KgOraCreateDataStore(void)
{
}

 /// \brief
  /// Gets the FdoIDataStorePropertyDictionary interface that	
  /// can be used to dynamically query	and	set	the	properties required	
  /// to create a new datastore.
  /// 
  /// \return
  /// Returns the property dictionary
  /// 
FdoIDataStorePropertyDictionary* c_KgOraCreateDataStore::GetDataStoreProperties()
{
  return NULL;
}

  /// \brief
  /// Executes the FdoICreateDataStore command.
  /// 
  /// \return
  /// Returns nothing
  /// 
void c_KgOraCreateDataStore::Execute()
{
}