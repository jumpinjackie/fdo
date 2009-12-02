#ifndef _ShpCreateSpatialContextCommand_H_
#define _ShpCreateSpatialContextCommand_H_
//
//  Copyright (C) 2001-2005 Autodesk, Inc. All Rights Reserved.
//
//  By using this code, you are agreeing to the terms and conditions of
//  the License Agreement included in the documentation for this code.
//
//  AUTODESK MAKES NO WARRANTIES, EXPRESS OR IMPLIED, AS TO THE
//  CORRECTNESS OF THIS CODE OR ANY DERIVATIVE WORKS WHICH INCORPORATE
//  IT. AUTODESK PROVIDES THE CODE ON AN "AS-IS" BASIS AND EXPLICITLY
//  DISCLAIMS ANY LIABILITY, INCLUDING CONSEQUENTIAL AND INCIDENTAL
//  DAMAGES FOR ERRORS, OMISSIONS, AND OTHER PROBLEMS IN THE CODE.
//
//  Use, duplication, or disclosure by the U.S. Government is subject
//  to restrictions set forth in FAR 52.227-19 (Commercial Computer
//  Software Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
//  (Rights in Technical Data and Computer Software), as applicable.

#ifdef _WIN32
#pragma once
#endif


/// <summary>The ICreateSpatialContext interface defines the CreateSpatialContext
/// command, which creates a new spatial context. 
/// Because of the SHP limitations only the WKT setting is relevant. The spatial context name 
/// is the same as the coordinate sytesm and they are extracted from WKT. All the other settings will be ignored. 
/// The Command execution will fail if the context already exists.</summary>

class ShpCreateSpatialContextCommand : public FdoCommonCommand<FdoICreateSpatialContext, ShpConnection>
{
public:

    // 0-argument constructor to please FdoPtr::operator->() :
    ShpCreateSpatialContextCommand ();

    //    Constructs an instance of a ShpCreateSpatialContextCommand using the
    //    specified arguments.
    ShpCreateSpatialContextCommand (FdoIConnection *connection);

    /// <summary>Gets the name of the context to create as a String.</summary>
    /// <returns>Returns the name of the spatial context</returns>
    virtual FdoString* GetName();

    /// <summary>Sets the name of the context to create as a String.</summary>
    /// <param name="value">Input the name of the spatial context</param>
    /// <returns>Returns nothing</returns>
    virtual void SetName(FdoString* value);

    /// <summary>Gets the description of the context to create as a String.</summary>
    /// <returns>Returns the description of the spatial context</returns>
    virtual FdoString* GetDescription();

    /// <summary>Sets the description of the context to create as a String.</summary>
	/// <param name="value">Input the description of the spatial context (ignored)</param>
    /// <returns>Returns nothing</returns>
    virtual void SetDescription(FdoString* value);

    /// <summary>Gets the coordinate system of the context to create as a String
    /// in OpenGIS SRS WKT format.</summary>
    /// <returns>Returns the coordinate system</returns>
    virtual FdoString* GetCoordinateSystem();

    /// <summary>Sets the coordinate system of the context to create as a String in
    /// OpenGIS SRS WKT format.</summary>
    /// <param name="value">Input the coordinate system (ignored)</param>
    /// <returns>Returns nothing</returns>
    virtual void SetCoordinateSystem(FdoString* value);

    /// <summary>Gets the coordinate system definition in OGC SRS WKT format for the 
    /// spatial context currently being created. If the result is an empty string, 
    /// the coordinate system WKT description has not been set.</summary>
    /// <returns>Returns the coordinate system description in WKT of of the spatial context.</returns> 
    virtual FdoString* GetCoordinateSystemWkt();

    /// <summary>Sets the coordinate system definition in OGC SRS WKT 
    /// format for the spatial context currently being created.</summary>
    /// <param name="value">Input the coordinate system WKT</param> 
    /// <returns>Returns nothing</returns> 
    virtual void SetCoordinateSystemWkt(FdoString* value);

    /// <summary>Gets the desired extent type of the context to create, either static or dynamic.</summary>
    /// <returns>Returns the extent type </returns>
    virtual FdoSpatialContextExtentType GetExtentType();

    /// <summary>Sets the desired extent type of the context to create, either static or dynamic.</summary>
	/// <param name="value">Input the extent type (ignored, it is Dynamic)</param>
    /// <returns>Returns nothing</returns>
    virtual void SetExtentType(FdoSpatialContextExtentType value);

    /// <summary>Gets the extent of the context to create as a byte array in FGF format.</summary>
    /// <returns>Returns the extent of the spatial context</returns>
    virtual FdoByteArray* GetExtent();

    /// <summary>Sets the extent of the context to create as a byte array in FGF format (ignored).</summary>
    /// <param name="value">Input the extent of the spatial context</param>
    /// <returns>Returns nothing</returns>
    virtual void SetExtent(FdoByteArray* value);

    /// <summary>Gets the tolerance value to use for X/Y ordinates (ignored).</summary>
    /// <returns>Returns the tolerance</returns>
    virtual const double GetXYTolerance();

    /// <summary>Sets the tolerance value to use for X/Y ordinates (ignored).</summary>
    /// <param name="value">Input the tolerance</param>
    /// <returns>Returns nothing</returns>
    virtual void SetXYTolerance(const double value);

    /// <summary>Gets the tolerance value to use for Z ordinates.</summary>
    /// <returns>Returns the tolerance</returns>
    virtual const double GetZTolerance();

    /// <summary>Sets the tolerance value to use for Z ordinates (ignored).</summary>
    /// <param name="value">Input the tolerance</param>
    /// <returns>Returns nothing</returns>
    virtual void SetZTolerance(const double value);

    /// <summary>Gets a Boolean flag that indicates how the CreateSpatialContext command
    /// should behave if the spatial context already exists. If update existing
    /// is true and a spatial context with the specified name already exists
    /// then it will be updated. If update existing is false and a spatial
    /// context with the specified name already exists then command execution
    /// will fail.</summary>
    /// <returns>Returns true if command should update existing contexts</returns>
    virtual const bool GetUpdateExisting();

    /// <summary>Sets a Boolean flag that indicates how the CreateSpatialContext command
    /// should behave if the spatial context already exists. If update existing
    /// is true and a spatial context with the specified name already exists
    /// then it will be updated. If update existing is false and a spatial
    /// context with the specified name already exists then command execution
    /// will fail.</summary>
    /// <param name="value">Input true if existing contexts should be updated</param>
    /// <returns>Returns nothing</returns>
    virtual void SetUpdateExisting(const bool value);

    /// <summary>Executes the CreateSpatialContext command. An exception is thrown if
    /// the spatial context already exists, or if the coordinate system or
    /// extent type is not supported by the provider.</summary>
    /// <returns>Returns nothing</returns>
    virtual void Execute();

protected:
    virtual ~ShpCreateSpatialContextCommand();
 
private:
    ShpConnection*				mConnection;

    FdoStringP                  mSCName;
    FdoStringP                  mDescription;
    FdoStringP                  mCoordSysName;
    FdoStringP                  mCoordSysWkt;

    FdoSpatialContextExtentType mExtentType;
    FdoPtr<FdoByteArray>        mExtent;

    double                      mXYTolerance;
    double                      mZTolerance;

    bool                        mUpdateExisting;
};
#endif
