/*
 * 
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


#ifndef SHPSPATIALCONTEXT_H
#define SHPSPATIALCONTEXT_H

#ifdef _WIN32
#pragma once
#endif

class ShpSpatialContext : public FdoDisposable
{
private:
    FdoStringP mName;
    FdoStringP mDescription;
    FdoStringP mCoordSysName;
    FdoStringP mWkt;
    FdoSpatialContextExtentType mExtentType;
    FdoPtr<FdoByteArray> mExtent;
	bool				 mIsExtentUpdated;
	bool				 mIsFromConfigFile;
    double mXYTolerance;
    double mZTolerance;

public:
    ShpSpatialContext (void);
    virtual ~ShpSpatialContext ();

    virtual FdoString* GetName ();
    virtual void SetName (FdoString* name);
    virtual FdoString* GetDescription ();
    virtual void SetDescription (FdoString* desc);
    virtual FdoString* GetCoordinateSystem ();
    virtual void SetCoordSysName (FdoString* csName);
    virtual FdoString* GetCoordinateSystemWkt ();
    virtual void SetCoordinateSystemWkt (FdoString* wkt);
    virtual FdoSpatialContextExtentType GetExtentType ();
    virtual void SetExtentType (FdoSpatialContextExtentType type);
    virtual FdoByteArray* GetExtent ();
    virtual void SetExtent (FdoByteArray* extent);
    virtual double GetXYTolerance ();
    virtual void SetXYTolerance (double tol);
    virtual double GetZTolerance ();
    virtual void SetZTolerance (double tol);

    // To please FdoNamedCollection:
    virtual FdoBoolean CanSetName() { return true; }
	virtual bool	   GetIsExtentUpdated();
	void			   SetIsExtentUpdated( bool value );	   

	void			   SetIsFromConfigFile( bool value );
	bool			   GetIsFromConfigFile();
};

typedef FdoPtr<ShpSpatialContext> ShpSpatialContextP;

class ShpSpatialContextCollection : public FdoNamedCollection<ShpSpatialContext, FdoException>
{
public:
    ShpSpatialContextCollection(void) {}

protected:
    virtual ~ShpSpatialContextCollection(void) {}

    /// <summary>Dispose this object instance.</summary>
    /// <returns>Returns nothing</returns>
    virtual void Dispose() 
    { 
        delete this; 
    }
};

typedef FdoPtr<ShpSpatialContextCollection> ShpSpatialContextCollectionP;

#endif // SHPSPATIALCONTEXT_H
