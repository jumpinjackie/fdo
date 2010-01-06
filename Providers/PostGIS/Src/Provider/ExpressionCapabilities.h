//
// Copyright (C) 2007 Refractions Research, Inc. 
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
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
//
#ifndef FDOPOSTGIS_EXPRESSIONCAPABILITIES_H_INCLUDED
#define FDOPOSTGIS_EXPRESSIONCAPABILITIES_H_INCLUDED

//
// Forward declarations
//
class FdoFunctionDefinitionCollection;

namespace fdo { namespace postgis {

/// Implementation of interface describing capabilities of expressions.
/// supported by PostGIS provider.
///
class ExpressionCapabilities : public FdoIExpressionCapabilities
{
public:

    /// Default constructor.
    ExpressionCapabilities();

    //
    // FdoIExpressionCapabilities interface
    //

    /// Return array of FdoExpressionType objects supported
    /// by the PostGIS provider.
    FdoExpressionType* GetExpressionTypes(FdoInt32& size);

    /// Return collection of FdoFunctionDefinition objects supported
    /// by the PostGISprovider within expressions. 
    FdoFunctionDefinitionCollection* GetFunctions();

protected:

    /// Destructor.
    virtual ~ExpressionCapabilities();

    //
    // FdoIDisposable interface
    //

    /// Dispose this object.
    void Dispose();

private:
	
};

}} // namespace fdo::postgis

#endif // FDOPOSTGIS_EXPRESSIONCAPABILITIES_H_INCLUDED
