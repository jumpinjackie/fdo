// 
//  Copyright (C) 2004-2006  Autodesk, Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
#include "SDF/ICreateSDFFile.h"
#include "SdfCommand.h"

//see documentation for ICreateSDFFile
class SdfCreateSDFFile : public SdfCommand<FdoICreateSDFFile>
{
    //-------------------------------------------------------
    // Constructor / destructor
    //-------------------------------------------------------

    public:
        // constructs an Insert command using the specified connection
        SDF_API SdfCreateSDFFile(SdfConnection* connection);

    protected:
        // default destructor
        SDF_API virtual ~SdfCreateSDFFile();


    //-------------------------------------------------------
    // ICreateSDFFile implementation
    //-------------------------------------------------------

        /// <summary>Sets the name of SDF file being created.
        /// The name must be a fully qualified absolute path.</summary>
        /// <param name="name">The fully qualified SDF file name.</param>
        /// <returns></returns> 
        SDF_API virtual void SetFileName(FdoString* name);

        /// <summary>Gets the name of SDF file being created.
        /// The name is a fully qualified absolute path.</summary>
        /// <returns>Returns the name of the SDF file.</returns> 
        SDF_API virtual FdoString* GetFileName();

        /// <summary>Sets the name of the spatial context as a string.</summary>
        /// <param name="name">Input the name of the spatial context</param> 
        /// <returns>Returns nothing</returns> 
        SDF_API virtual void SetSpatialContextName(FdoString* name);

        /// <summary>Gets the name of the coordinate system of the spatial context.
        /// If the result is a null or empty string, the coordinate system is
        /// unknown to the spatial context.</summary>
        /// <returns>Returns the coordinate system name of the spatial context.</returns> 
        SDF_API virtual FdoString* GetSpatialContextName();

        /// <summary>Sets the description of the spatial context as a string.</summary>
        /// <param name="description">Input the description of the spatial context</param> 
        /// <returns>Returns nothing</returns> 
        SDF_API virtual void SetSpatialContextDescription(FdoString* description);

        /// <summary>Gets the description of the spatial context.</summary>
        /// <returns>Returns the description of the spatial context.</returns> 
        SDF_API virtual FdoString* GetSpatialContextDescription();

        /// <summary>Sets the coordinate system of the context to create as a string in
        /// OpenGIS SRS WKT format.</summary>
        /// <param name="value">Input the coordinate system</param> 
        /// <returns>Returns nothing</returns> 
        SDF_API virtual void SetCoordinateSystemWKT(FdoString* wkt);

        /// <summary>Gets the name of the coordinate system in OpenGIS SRS WKT format of the spatial context.
        /// If the result is an empty string, the coordinate system WKT description is
        /// not available.</summary>
        /// <returns>Returns the coordinate system description in WKT of of the spatial context.</returns> 
        SDF_API virtual FdoString* GetCoordinateSystemWKT();

        /// <summary>Sets the tolerance value to use for X/Y ordinates. Tolerances are used
        /// in some geometric tests, mostly for equality between coordinates.  This
        /// occurs frequently in spatial queries, especially with "on boundary" or "just
        /// touching" conditions.  It is also an objective amount that can be used
        /// when formulating step distances for parametric curve interpolation.</summary>
        /// <param name="value">Input the tolerance</param> 
        /// <returns>Returns nothing</returns> 
        SDF_API virtual void SetXYTolerance(double tolerance);

        /// <summary>Gets the tolerance value for XY ordinates of the spatial context.
        /// Tolerances are used in some geometric tests,
        /// mostly for equality between coordinates.  This occurs frequently in spatial
        /// queries, especially with "on boundary" or "just touching" conditions.
        /// It is also an objective amount that can be used when formulating step
        /// distances for parametric curve interpolation.</summary>
        /// <returns>Returns the tolerance</returns>
        SDF_API virtual double GetXYTolerance();
        
        /// <summary>Sets the tolerance value to use for Z ordinates. Tolerances are used in
        /// some geometric tests, mostly for equality between coordinates.  This
        /// occurs frequently in spatial queries, especially with "on boundary" or "just
        /// touching" conditions.  It is also an objective amount that can be used
        /// when formulating step distances for parametric curve interpolation.</summary>
        /// <param name="value">Input the tolerance</param> 
        /// <returns>Returns nothing</returns> 
        SDF_API virtual void SetZTolerance(double tolerance);

        /// <summary>Gets the tolerance value for Z ordinates of the spatial context.
        /// Tolerances are used in some geometric tests,
        /// mostly for equality between coordinates.  This occurs a frequently in spatial
        /// queries, especially with "on boundary" or "just touching" conditions.
        /// It is also an objective amount that can be used when formulating step
        /// distances for parametric curve interpolation.</summary>
        /// <returns>Returns the tolerance</returns>
        SDF_API virtual double GetZTolerance();

        ///<summary>
        /// Executes the command. An exception will be thrown if file fails to be 
        /// created.
        ///</summary>
        SDF_API virtual void Execute();

private:
    FdoStringP m_filename;
    FdoStringP m_scname;
    FdoStringP m_scdesc;
    FdoStringP m_wkt;
    double m_xyTolerance;
    double m_zTolerance;
};

