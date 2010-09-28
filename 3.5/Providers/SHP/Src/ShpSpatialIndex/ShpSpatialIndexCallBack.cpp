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

#include "stdafx.h"
#include "ShpSpatialIndexCallBack.h"
#include <math.h>


//-------------------------------------------------------------------------
//
// METHOD: Constructor.
//
// PURPOSE: Create and initial an instance of the ShpSpatialIndexFileCallback class.
//
// PARAMETERS:
//
//     None.
//
//-------------------------------------------------------------------------

ShpSpatialIndexFileCallback::ShpSpatialIndexFileCallback() :
    m_canceled(FALSE),
    m_minProgress(0.0),
    m_maxProgress(100.0),
    m_progress(0.0)
{
} // end: Constructor


//-------------------------------------------------------------------------
//
// METHOD: SetCanceled().
//
// PURPOSE: Set the cancelation flag to TRUE.
//
// PARAMETERS:
//
//     None.
//
// RETURNS: None.
//
//-------------------------------------------------------------------------

void ShpSpatialIndexFileCallback::SetCanceled()
{
    m_canceled = TRUE;

} // end: SetCanceled()


//-------------------------------------------------------------------------
//
// METHOD: GetCanceled().
//
// PURPOSE: Get the cancelation flag.
//
// PARAMETERS:
//
//     None.
//
// RETURNS: Returns TRUE if the cancelation flag is set, and FALSE other-
//          wise.
//
//-------------------------------------------------------------------------

BOOL ShpSpatialIndexFileCallback::GetCanceled() const
{
    return m_canceled;

} // end: GetCanceled()


//-------------------------------------------------------------------------
//
// METHOD: SetProgressRange().
//
// PURPOSE: Set the minimum and maximum values of the progress range. By
//          default the range is 0.0 to 100.0.
//
// PARAMETERS:
//
//     Input:
//
//         minProgress - passes the minimum value of the progress range.
//         maxProgress - passes the maximum value of the progress range.
//
//     Output:
//
//         None.
//
// RETURNS: None.
//
//-------------------------------------------------------------------------

void ShpSpatialIndexFileCallback::SetProgressRange(double minProgress, double maxProgress)
{
    m_minProgress = minProgress;
    m_maxProgress = maxProgress;

} // end: SetProgressRange()


//-------------------------------------------------------------------------
//
// METHOD: SetProgress().
//
// PURPOSE: Set the current level progress. The base class implementation 
//          simply records the progress value. Derived classes should also
//          record the level of progress in the member 'progress'.
//
// PARAMETERS:
//
//     Input:
//
//         progress - passes the level of progress.
//
//     Output:
//
//         None.
//
// RETURNS: None.
//
//-------------------------------------------------------------------------

void ShpSpatialIndexFileCallback::SetProgress(double progress)
{
    m_progress = progress;

} // end: SetProgress()


//-------------------------------------------------------------------------
//
// METHOD: GetProgress().
//
// PURPOSE: Get the current level progress.
//
// PARAMETERS:
//
//     None.
//
// RETURNS: Returns the current amount of progress.
//
//-------------------------------------------------------------------------

double ShpSpatialIndexFileCallback::GetProgress() const
{
    return m_progress;

} // end: GetProgress()

