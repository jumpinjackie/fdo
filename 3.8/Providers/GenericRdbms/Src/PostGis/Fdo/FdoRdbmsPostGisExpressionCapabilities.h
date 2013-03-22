#ifndef FDORDBMSPOSTGISEXPRESSIONCAPABILITIES_H
#define FDORDBMSPOSTGISEXPRESSIONCAPABILITIES_H     1

// 
//  Copyright (C) 2004-2011  Autodesk, Inc.
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

#include <FdoExpressionEngine.h>
#include "FdoRdbmsExpressionCapabilities.h"

class FdoRdbmsPostGisExpressionCapabilities : public FdoRdbmsExpressionCapabilities
{
    //-------------------------------------------------------
    // Constructor / destructor
    //-------------------------------------------------------

    public:
        // default constructor
        FdoRdbmsPostGisExpressionCapabilities();

    protected:
        // default destructor
        virtual ~FdoRdbmsPostGisExpressionCapabilities();

    //-------------------------------------------------------
    // FdoIDisposable implementation
    //-------------------------------------------------------

    protected:
        // dispose this object
        virtual void Dispose();

    //-------------------------------------------------------
    // FdoIExpressionCapabilities implementation
    //-------------------------------------------------------

    public:
        // Returns an array of FunctionDefinitions the feature provider supports
        // within expressions.  The length parameter gives the number of function
        // definitions in the array.
        virtual FdoFunctionDefinitionCollection* GetFunctions();

    private:
        FdoPtr<FdoFunctionDefinitionCollection> m_supportedFunctions;
};

#endif // FdoRdbmsPostGisExpressionCapabilities_H

