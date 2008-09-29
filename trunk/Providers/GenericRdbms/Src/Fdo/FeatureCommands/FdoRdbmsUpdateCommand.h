#ifndef FDORDBMSUPDATECOMMAND_H
#define FDORDBMSUPDATECOMMAND_H     1
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
 *
 */

#include "FdoRdbms.h"
#include "FdoRdbmsFeatureCommand.h"
#include "FdoRdbmsLockConflictReader.h"


class DbiConnection;

class FdoRdbmsUpdateCommand : public FdoRdbmsFeatureCommand<FdoIUpdate>
{
    friend class FdoRdbmsConnection;

  private:

      //
      // Prevent the use of the copy constructor by definning it and not implemeting it.
      // DO NOT IMPLEMENT
      FdoRdbmsUpdateCommand(const FdoRdbmsUpdateCommand &right);

      FdoRdbmsUpdateCommand ();

      FdoRdbmsUpdateCommand (FdoIConnection *connection);

      bool CheckLocks(bool placeTransactionLock, bool checkForConflicts);
     
	  void GetDefaultLockConflictReader();

  protected:
      virtual ~FdoRdbmsUpdateCommand();

  public:

      //
      // Prevent the use of the Assignment Operation by definning it and not implemeting it.
      // DO NOT IMPLEMENT
      FdoRdbmsUpdateCommand & operator=(const FdoRdbmsUpdateCommand &right);

      //    Executes the update command and returns the number of
      //    modified instances.
      virtual FdoInt32 Execute ();

      FdoILockConflictReader* GetLockConflicts();

      virtual FdoPropertyValueCollection* GetPropertyValues();

      virtual void SetFeatureClassName(FdoIdentifier* value);

      virtual void SetFeatureClassName(const wchar_t* value);


  private:
      DbiConnection* mConnection;
      wchar_t       *mCurrentClass;

      FdoPropertyValueCollection *mPropertyValues;
      FdoILockConflictReader *mLockConflictReader;

	  FdoRdbmsPvcProcessor  *mPvcProcessor;
};

#endif // FDORDBMSUPDATECOMMAND_H
