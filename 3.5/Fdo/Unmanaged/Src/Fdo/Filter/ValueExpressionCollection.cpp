// 
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
//  
#include <Fdo/Filter/ValueExpressionCollection.h>

// Constructs a default empty instance of a ValueCollection.
FDO_API FdoValueExpressionCollection* FdoValueExpressionCollection::Create()
{
    return new FdoValueExpressionCollection();
}

FDO_API FdoValueExpressionCollection* FdoValueExpressionCollection::Create(FdoString** values, FdoInt32 length)
{
    FdoValueExpressionCollection* pCollection = Create();

    FdoParameter*   pParam = NULL;
    FdoInt32     i;
    try
    {
        for (i = 0; i < length; i++)
        {
            if (values[i] != NULL)
            {
                FdoParameter*   pParam = FdoParameter::Create(values[i]);
                pCollection->Add(pParam);
                FDO_SAFE_RELEASE(pParam);      // both the add and create do an addref.
            }
        }
    }
    catch (FdoException* pExcept)
    {
        FDO_SAFE_RELEASE(pCollection);
        FDO_SAFE_RELEASE(pParam);
        throw pExcept;
    }

    return pCollection;
}

