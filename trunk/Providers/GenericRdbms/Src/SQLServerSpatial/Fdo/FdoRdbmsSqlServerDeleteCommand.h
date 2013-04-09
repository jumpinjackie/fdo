#ifndef FDORDBMSSQLSERVERDELETECOMMAND_H
#define FDORDBMSSQLSERVERDELETECOMMAND_H     1
/*
 * Copyright (C) 2004-2008  Autodesk, Inc.
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
 *
 */

#include "FdoRdbmsSchemaUtil.h"
#include "FdoRdbmsSqlServerConnection.h"
#include "FdoRdbmsDeleteCommand.h"

class FdoRdbmsSqlServerDeleteCommand : public FdoRdbmsDeleteCommand
{
  friend class FdoRdbmsSqlServerConnection;

  private:
      //
      // Prevent the use of the copy constructor by definning it and not implemeting it.
      // DO NOT IMPLEMENT
      FdoRdbmsSqlServerDeleteCommand(const FdoRdbmsSqlServerDeleteCommand &right);

      FdoRdbmsSqlServerDeleteCommand ();

      FdoRdbmsSqlServerDeleteCommand (FdoIConnection *connection);

	  void AddLtProperty( const wchar_t *scope, const FdoSmLpClassDefinition *classDefinition, 
								FdoPropertyValueCollection *propertyValues, FdoDataValue *dataValue );

 protected:
      virtual ~FdoRdbmsSqlServerDeleteCommand();

  public:

      FdoRdbmsSqlServerDeleteCommand & operator=(const FdoRdbmsSqlServerDeleteCommand &right);

      FdoInt32 virtual Execute ();
};

#endif // FDORDBMSSQLSERVERDELETECOMMAND_H
