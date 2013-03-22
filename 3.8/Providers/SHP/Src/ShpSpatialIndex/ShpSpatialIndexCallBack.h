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

#ifndef SHPSPATIALINDEXCALLBACK_H
#define SHPSPATIALINDEXCALLBACK_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

//-------------------------------------------------------------------------
//
// PURPOSE: Declaration of the ShpSpatialIndexFileCallback class. The IndexFileCall-
//          back declares an interface to an object that is used to report
//          progress during lengthy operations, and to allow those operations
//          to be cancelled without completing. An application is responsible
//          for deriving a class from ShpSpatialIndexFileCallback to implement the
//          required functionality. 
//
//-------------------------------------------------------------------------
class ShpSpatialIndexFileCallback {
public:
    // constructor (explicit destructor not required)

    ShpSpatialIndexFileCallback();

    // methods set/get the cancellation flag

    void SetCanceled();
    BOOL GetCanceled() const;

    // methods to set the progress range, and to report progress

    void SetProgressRange(double minProgress, double maxProgress);
    virtual void SetProgress(double progress);
    double GetProgress() const;

protected:
    BOOL m_canceled;       // has operation been canceled ?
    double m_minProgress;  // minimum value of progress range (default = 0)
    double m_maxProgress;  // maximum value of progress range (default = 100)
    double m_progress;     // current level of progress
};    

#endif // SHPSPATIALINDEXCALLBACK_H

