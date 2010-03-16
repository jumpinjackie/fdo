#ifndef FDORDBMSDELETECOMMAND_H
#define FDORDBMSDELETECOMMAND_H     1
/*
 * Copyright (C) 2004-2006  Autodesk, Inc.
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

#include "FdoRdbms.h"
#include "FdoRdbmsConnection.h"
#include "FdoRdbmsFeatureCommand.h"
#include "../Lock/FdoRdbmsLockConflictReader.h"


class FdoRdbmsDeleteCommand : public FdoRdbmsFeatureCommand<FdoIDelete>
{
  friend class FdoRdbmsConnection;

  private:
      //
      // Prevent the use of the copy constructor by definning it and not implemeting it.
      // DO NOT IMPLEMENT
      FdoRdbmsDeleteCommand(const FdoRdbmsDeleteCommand &right);

      // Perform lock checks needed before delete performed
      virtual bool PreCheckLocks(bool placeTransactionLock, bool checkForConflicts, bool* lockConflictsChecked);
      // Perform lock checks
      virtual bool CheckLocks(bool placeTransactionLock, bool checkForConflicts);
      void GetDefaultLockConflictReader();
      FdoInt32  InternalExecute ();

      bool CheckAssociationDependancies( const wchar_t* scope, const FdoSmLpClassDefinition* currentClass, FdoFilter *filter  );

      FdoInt32 DeleteRelatedObjects( const wchar_t* scope, const FdoSmLpClassDefinition* currentClass,
                    const FdoSmLpDataPropertyDefinitionCollection *fkProps, bool followRelOnly=false );

 protected:
	  FdoRdbmsDeleteCommand ();

      FdoRdbmsDeleteCommand (FdoIConnection *connection);

      virtual ~FdoRdbmsDeleteCommand();

  public:

      FdoRdbmsDeleteCommand & operator=(const FdoRdbmsDeleteCommand &right);

      FdoInt32 virtual Execute ();

      virtual void GetLtQualification ( const FdoSmLpClassDefinition *classDefinition, FdoStringP &ltQualificationClause );

      FdoILockConflictReader* GetLockConflicts();

	  void SetLockConflictReader(FdoILockConflictReader* lockConflictReader);

  protected:
      DbiConnection* mConnection;
      FdoRdbmsConnection *mConn;

      FdoILockConflictReader *mLockConflictReader;

  private:

  private: //## implementation

};

#endif // FDORDBMSDELETECOMMAND_H

