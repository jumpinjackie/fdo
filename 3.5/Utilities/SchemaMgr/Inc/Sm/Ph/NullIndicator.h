#ifndef FDOSMPHNULLINDICATOR_H
#define FDOSMPHNULLINDICATOR_H		1
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

#define _NI_TYPE    void // Keep it in sync with GDBI_NI_TYPE


// Represents a NULL indicator, which indicates whether an accompanying
// value is NULL. Wraps provider-specific null indicators. For RDBMS 
// providers, these would be the null indicators "bound" into SQL statements.
//
// Abstract class. Specific Schema Managers must provide derivation.
class FdoSmPhNullIndicator : public FdoSmDisposable, public FdoSmBase
{
public:

    /// Creates a new null indicator
    FdoSmPhNullIndicator() {}

    virtual ~FdoSmPhNullIndicator(void) {};

    /// Get the provider-specific null indicator
    virtual _NI_TYPE* GetDbIndicator() = 0;

    // Set this indicator to NULL.
    virtual void SetNull() = 0;

    // Set this indicator to not NULL.
    virtual void SetNNull() = 0;

    // Get current indicator value.
    virtual bool IsNull() = 0;

protected:

private:
};

typedef FdoPtr<FdoSmPhNullIndicator> FdoSmPhNullIndicatorP;

#endif


