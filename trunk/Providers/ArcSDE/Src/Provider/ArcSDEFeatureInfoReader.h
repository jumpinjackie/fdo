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

#ifndef ARCSDEFEATUREINFOREADER_H
#define ARCSDEFEATUREINFOREADER_H
#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "FdoDefaultFeatureReader.h"

class ArcSDEFeatureInfoReader: public FdoDefaultFeatureReader
{
    friend class ArcSDEInsertCommand;
//    friend class ArcSDEUpdateCommand;
private:
    FdoPtr <FdoPropertyValueCollection> mFeatInfoCollection;
    FdoPtr <FdoClassDefinition> mClassDefinition;
    bool mClosed;
    FdoInt32 m_iRowIndex;
    
    ArcSDEFeatureInfoReader (FdoPropertyValueCollection *info, FdoClassDefinition *definition);

    // Prevent the use of the Copy Constructor by defining it and not implementing it.
    // DO NOT IMPLEMENT
    ArcSDEFeatureInfoReader (const ArcSDEFeatureInfoReader &right);

    // Prevent the use of the Assignment Operation by defining it and not implementing it.
    // DO NOT IMPLEMENT
    ArcSDEFeatureInfoReader& operator= (const ArcSDEFeatureInfoReader &right);

protected:
    virtual ~ArcSDEFeatureInfoReader ();
    virtual void Dispose ();
    virtual void Validate();
    template <typename T> T* GetValue(FdoString* name, FdoString* typeName);

public:
          
    virtual FdoClassDefinition* GetClassDefinition ();
    virtual int GetDepth ();
    virtual bool     GetBoolean (const wchar_t *propertyName);
    virtual FdoByte  GetByte (const wchar_t *propertyName);
    virtual double   GetDouble (const wchar_t* propertyName);
    virtual FdoInt16 GetInt16 (const wchar_t *propertyName);
    virtual FdoInt32 GetInt32 (const wchar_t *propertyName);
    virtual FdoInt64 GetInt64 (const wchar_t *propertyName);
    virtual float    GetSingle (const wchar_t *propertyName);
    virtual const wchar_t* GetString (const wchar_t *propertyName);
    virtual FdoLOBValue* GetLOB(FdoString* propertyName);
    virtual FdoIStreamReader* GetLOBStreamReader(const wchar_t* propertyName );
    virtual FdoDateTime GetDateTime (const wchar_t *propertyName);
    virtual bool     IsNull (const wchar_t *propertyName);
    virtual FdoIFeatureReader* GetFeatureObject (const wchar_t* propertyName);
    virtual FdoByteArray* GetGeometry (const wchar_t* propertyName);
    virtual const FdoByte * GetGeometry(FdoString* propertyName, FdoInt32 * count);
    virtual FdoIRaster* GetRaster(FdoString* propertyName);
    virtual bool     ReadNext ();
    virtual void     Close ();
};

#endif // ARCSDEFEATUREINFOREADER_H

