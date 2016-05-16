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

#ifndef FDOWFSNAMEDIOSTREAM_H
#define FDOWFSNAMEDIOSTREAM_H
#ifdef _WIN32
#pragma once
#endif // _WIN32

class FdoWfsNamedIoStream : public FdoIDisposable
{
private:
    FdoPtr<FdoIoStream> m_stream;
    FdoStringP m_name;

protected:
	FdoWfsNamedIoStream() {}
    FdoWfsNamedIoStream(FdoString* name, FdoIoStream* stream):
         m_name(name), m_stream(stream)
    {
        FDO_SAFE_ADDREF(stream);
    }
    virtual ~FdoWfsNamedIoStream() {}
    virtual void Dispose() { delete this; }

public:
    static FdoWfsNamedIoStream* Create(FdoString* name, FdoIoStream* stream)
    {
        return new FdoWfsNamedIoStream(name, stream);
    }
    FdoString* GetName() { return m_name; }
    FdoBoolean CanSetName() { return false; }
    FdoIoStream* GetStream() { return FDO_SAFE_ADDREF(m_stream.p); }

};

class FdoWfsNamedIoStreamCollection : public FdoNamedCollection<FdoWfsNamedIoStream, FdoException>
{
protected:
    FdoWfsNamedIoStreamCollection() {}
    virtual ~FdoWfsNamedIoStreamCollection() {}
    virtual void Dispose() { delete this; }

public:
    static FdoWfsNamedIoStreamCollection* Create()
    {
        return new FdoWfsNamedIoStreamCollection();
    }
};



#endif

