#ifndef FDORDBMSPHRDGRDFIELDARRAY_H
#define FDORDBMSPHRDGRDFIELDARRAY_H     1
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

#ifdef _WIN32
#pragma once
#endif
#include <Sm/Ph/Field.h>
#include <Inc/Rdbi/context.h>
#include "../../../Gdbi/GdbiTypes.h"
#include "../../../Gdbi/GdbiQueryResult.h"
#include "../../../Gdbi/GdbiConnection.h"

// This class provides a buffer for a field. The buffer receives field
// values when a row is read from an RDBMS query reader.
class FdoSmPhRdGrdFieldArray : public FdoSmPhSchemaElement
{
public:

    // Creates a new Field Array Definition.
    //
    // Parameters:
    //      field: the field that this field array represents.
    //      results: underlying results reader that will fill this object's
    //          buffer.
    //      posn: fields 1-based position within the query select list.
    FdoSmPhRdGrdFieldArray(
        FdoSmPhFieldP field,
        GdbiQueryResult* results,
        int posn
    );

    ~FdoSmPhRdGrdFieldArray(void);

    // Get the current field value in string format
    // Returns L"" if value is null.
    virtual FdoStringP GetString();

    // Get the current field value in integer format
    // Returns 0 if value is null.
    virtual int GetInteger();

    // Get the current field value in long integer format
    // Returns 0 if value is null.
    virtual long GetLong();

    // Get the current field value in double precision format
    // Returns 0.0 if value is null.
    virtual double GetDouble();

    // Get the current field value in boolean format
    // Returns false if value is null.
    virtual bool GetBoolean();

	// Update results
	virtual void UpdateResults( GdbiQueryResult* results );

protected:
    // unused constructor needed only to build on Linux
    FdoSmPhRdGrdFieldArray() {}

private:

    FdoSmPhFieldP mpField;
    long mValueSize;
    long mPosn;

    FdoByte* mpBuffer;
    bool		 mNull;
    GdbiQueryResult* mResults;
    GdbiColumnDesc mColDesc;
};

typedef FdoPtr<FdoSmPhRdGrdFieldArray> FdoSmPhRdGrdFieldArrayP;

#endif
