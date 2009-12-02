//
// Copyright (C) 2004-2006  Autodesk, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA


#ifndef FDOOWSIHTTPHANDLER_H
#define FDOOWSIHTTPHANDLER_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include <OWS/FdoOwsMIMEType.h>

class FdoOwsIHttpHandler : public FdoIoStream
{
public:
    FDOOWS_API static FdoOwsIHttpHandler* Create(const char* url, 
                                                 bool bGet, 
                                                 const char* parameters,
                                                 const char* userName, 
                                                 const char* passwd);

    FDOOWS_API virtual void Perform() = 0;
    FDOOWS_API virtual FdoOwsMIMEType GetMIMEType() = 0;
};



#endif


