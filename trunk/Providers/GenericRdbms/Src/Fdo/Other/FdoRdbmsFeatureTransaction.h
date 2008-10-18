#ifndef FDORDBMSFEATURETRANSACTION_H
#define FDORDBMSFEATURETRANSACTION_H    1
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
#include "FdoRdbmsConnection.h"

class DbiConnection;

class  FdoRdbmsFeatureTransaction : public FdoITransaction
{
    friend class FdoRdbmsConnection;

  private:
    //
    // Prevent the use of the copy constructor by definning it and not implemeting it.
    // DO NOT IMPLEMENT
    FdoRdbmsFeatureTransaction(const FdoRdbmsFeatureTransaction &right);

    FdoRdbmsFeatureTransaction(FdoIConnection *connection);

protected:
    virtual ~FdoRdbmsFeatureTransaction ();

    // Most providers do not roll back DDL statements on transaction rollback,
    // leaving MetaSchema and physical schema inconsistent when an ApplySchema
    // command gets rolled back.
    // This function synchronizes the physical schema with the MetaSchema on
    // such a rollback.
    void SynchSchema();

public:

    virtual FdoIConnection* GetConnection();

    virtual void Commit();

    virtual void Rollback();

    virtual void Dispose();

  private:
      char mTransactionName[32];
      DbiConnection* mConnection;
      int mTransactionStarted;
      FdoRdbmsConnection* mFdoConnection;
};

#endif // FDORDBMSFEATURETRANSACTION_H
