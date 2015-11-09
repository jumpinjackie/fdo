#ifndef _FUNCTIONEXTENSIONCOLLECTION_H_
#define _FUNCTIONEXTENSIONCOLLECTION_H_
// 

//
// Copyright (C) 2004-2007  Autodesk, Inc.
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
//
#ifdef _WIN32
#pragma once
#endif

#include <FdoStd.h>
#include <FdoExpressionEngineIFunction.h>
#include "ExpressionEngine.h"


/// \brief
/// The FdoExpressionEngineFunctionCollection class represents a collection of FdoExpressionEngineIFunction objects.
class FdoExpressionEngineFunctionCollection : public FdoCollection<FdoExpressionEngineIFunction, FdoException>
{
protected:
    EXPRESSIONENGINE_API  virtual void Dispose();

public:
    /// \brief
    /// Constructs a default empty instance of an FdoExpressionEngineFunctionCollection.
    /// 
    /// \return
    /// Returns FdoIFunctionExtensionCollection
    /// 
    EXPRESSIONENGINE_API static FdoExpressionEngineFunctionCollection* Create();

    /// \brief
    /// Constructs an instance of an FdoExpressionEngineFunctionCollection from an array of 
    /// FdoFunctionDefinition objects.
    /// 
    /// \param arguments 
    /// Input an array of FdoExpressionEngineIFunction objects
    /// \param length 
    /// Input the number of arguments
    /// 
    /// \return
    /// Returns FdoExpressionEngineFunctionCollection
    /// 
    EXPRESSIONENGINE_API static FdoExpressionEngineFunctionCollection* Create(FdoExpressionEngineIFunction** arguments, FdoInt32 length);
};
#endif
