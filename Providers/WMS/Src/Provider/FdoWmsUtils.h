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

#ifndef FDOWMSUTILS_H
#define FDOWMSUTILS_H

class FdoWmsLayer;
class FdoWmsLayerCollection;
class FdoWmsBoundingBox;

// the default message catalog filename
extern char *fdofdowms_cat;  
#define S_FDOFDOWMS_1 1 

// Convenient NLS macros
FdoString* NlsMsgGet(int msg_num, char* default_msg, ...);
FdoString* NlsMsgGet(int msg_num, char* default_msg, char* file, int line, ...);

// case insensitive string comparison
#define STREQUAL(s1, s2)	(FdoCommonOSUtil::wcsicmp(s1, s2) == 0)

// case sensitive string comparison
#define STRCASEEQ(s1, s2)	(wcscmp(s1, s2) == 0)

// Bounding Box helper functions
void _calcLayerBoundingBox (FdoWmsLayer* layer, FdoString* srsName, FdoWmsBoundingBox* bbox);
void _calcLayersBoundingBox (FdoWmsLayerCollection* layers, FdoString* srsName, FdoWmsBoundingBox* bbox);
FdoBoolean _reverseCheck(FdoStringP epsgName); 


#endif // FDOWMSUTILS_H
