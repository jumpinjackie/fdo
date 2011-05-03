#ifndef FDORDBMSPROPBINDHELPER_H
#define FDORDBMSPROPBINDHELPER_H 1
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
#include <Sm/SchemaManager.h>
#include <../../Gdbi/GdbiCommands.h>
// Temporary datatype until FdoDataType_Geometry is available
#ifndef FdoRdbmsDataType_Geometry
#define FdoRdbmsDataType_Geometry    FdoDataType_CLOB + 2
#endif

class FdoRdbmsPvdBindDef;

//
// Defines an abstract class for the Property Value Collection handlers
class FdoRdbmsPropBindHelper
{
public:
    FdoRdbmsPropBindHelper(FdoRdbmsConnection *connection);    

    /// <summary>Given a PVC operation, execute the handler.</summary>
    /// <returns>Returns the number of affected objects</returns> 
    virtual void BindParameters(GdbiStatement* statement, std::vector<std::pair<FdoLiteralValue*, FdoInt64>>* params);

    /// <summary>Clear all parameters making them ready for a new bind (no free memory will be done)</summary>
    void Clear();

private:
    FdoRdbmsPvdBindDef* mBindParams;
    FdoRdbmsConnection* mFdoConnection;
};

#endif // FDORDBMSPROPBINDHELPER_H
