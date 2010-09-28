#ifndef FdoRdbmsFeatureInfoReader_H
#define FdoRdbmsFeatureInfoReader_H     1
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
#include "Sm/Lp/ClassDefinition.h"
#include "FdoDefaultFeatureReader.h"

class FdoRdbmsFeatureInfoReader: public FdoDefaultFeatureReader
{
  friend class FdoRdbmsInsertCommand;
  friend class FdoRdbmsUpdateCommand;

  private:
      FdoRdbmsFeatureInfoReader( FdoPropertyValueCollection    *mFeatInfo, const FdoSmLpClassDefinition *classDefinition );

      FdoRdbmsFeatureInfoReader(const FdoRdbmsFeatureInfoReader &right);

      FdoRdbmsFeatureInfoReader & operator=(const FdoRdbmsFeatureInfoReader &right);

      void ThrowPropertyNotFountExp( const wchar_t* propertyName );

protected:
    virtual ~FdoRdbmsFeatureInfoReader();
    virtual void Dispose();

public:

        virtual FdoClassDefinition* GetClassDefinition();
        virtual int GetDepth();
        virtual bool     GetBoolean( const wchar_t *propertyName );
        virtual FdoByte  GetByte( const wchar_t *propertyName );
        virtual double   GetDouble(const wchar_t* propertyName);
        virtual short    GetInt16( const wchar_t *propertyName );
        virtual int      GetInt32( const wchar_t *propertyName );
        virtual FdoInt64 GetInt64( const wchar_t *propertyName );
        virtual float    GetSingle( const wchar_t *propertyName );
        virtual const wchar_t* GetString( const wchar_t *propertyName );
        virtual FdoLOBValue* GetLOB(const wchar_t* propertyName );
        virtual FdoIStreamReader* GetLOBStreamReader(const wchar_t* propertyName );
        virtual bool     IsNull( const wchar_t *propertyName );
        virtual FdoIFeatureReader* GetFeatureObject(const wchar_t* propertyName);
        virtual FdoByteArray* GetGeometry(const wchar_t* propertyName);
        virtual const FdoByte * GetGeometry(const wchar_t* propertyName, FdoInt32 * count);
        virtual FdoIRaster* GetRaster(const wchar_t* propertyName);
        virtual bool     ReadNext( );
        virtual void     Close();

        virtual FdoDateTime GetDateTime(const wchar_t *propertyName );


  private:
      FdoPropertyValueCollection    *mFeatInfoCollection;
      bool                          mCalled;
      const FdoSmLpClassDefinition     *mClassDefinition;
};

#endif // FdoRdbmsFeatureInfoReader_H

