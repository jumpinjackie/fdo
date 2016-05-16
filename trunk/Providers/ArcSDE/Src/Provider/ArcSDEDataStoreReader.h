/*
 * Copyright (C) 2004-2005  Autodesk, Inc.
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

#ifndef ARCSDEDATASTOREREADER_H
#define ARCSDEDATASTOREREADER_H


class ArcSDEDataStoreReader :
    public FdoIDataStoreReader
{
public:
    // 0-argument constructor to please FdoPtr::operator->() :
    ArcSDEDataStoreReader() { ArcSDEDataStoreReader(NULL); }

    ArcSDEDataStoreReader(ArcSDEConnection *connection);
    virtual ~ArcSDEDataStoreReader(void);

    // methods inherited from FdoIDataStoreReader:
public:
	virtual FdoString* GetName();
	virtual FdoString* GetDescription();
	virtual bool GetIsFdoEnabled();
	virtual FdoIDataStorePropertyDictionary* GetDataStoreProperties();
	virtual bool ReadNext();
	virtual void Close();

protected:
    virtual void Dispose();
    void Validate(void);

    FdoPtr<ArcSDEConnection> mConnection;
    LONG mDataStoreIndex;
    FdoPtr<FdoIConnectionPropertyDictionary> mConnProps;
};

#endif // ARCSDEDATASTOREREADER_H

