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
 * Revision Control Modification History
 *
 *         $Id$
 *     $Author$
 *   $DateTime$
 *     $Change$
 *
 */


#ifndef SHAPECPG_H
#define SHAPECPG_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include <FdoCommonFile.h>

class ShapeCPG : public FdoCommonFile
{

public:
    ShapeCPG (const wchar_t* name, int& status);
    ShapeCPG (const wchar_t* name, char* locale);

    virtual ~ShapeCPG (void);

    FdoStringP GetCodePage();
    void       SetCodePage( FdoString* codePage );

private:
    FdoStringP  mCodePageESRI;
};

#endif // ShapeCPG_H
