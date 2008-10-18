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

#ifndef FDORDBMSODBCDEXPRESSIONCAPABILITIES_H_
#define FDORDBMSODBCDEXPRESSIONCAPABILITIES_H_     1

#ifdef _WIN32
#pragma once
#endif


class FdoRdbmsOdbcExpressionCapabilities :    public FdoIExpressionCapabilities
{
public:
    FdoRdbmsOdbcExpressionCapabilities(void);

    // Returns an array of the ExpressionTypes the feature provider supports.
    virtual FdoExpressionType* GetExpressionTypes(int& length);

    // Returns an array of FuntionDefinitions the feature provider supports within expressions.
    virtual FdoFunctionDefinitionCollection* GetFunctions();

protected:
    ~FdoRdbmsOdbcExpressionCapabilities(void);
    virtual void Dispose();

private:

    // Stores the list of supported functions.
    FdoPtr<FdoFunctionDefinitionCollection> m_supportedFunctions;
};

#endif // FDORDBMSODBCDEXPRESSIONCAPABILITIES_H_
