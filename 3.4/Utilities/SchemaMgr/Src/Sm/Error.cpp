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

#include "stdafx.h"
#include <Sm/Error.h>

FdoSmError::FdoSmError()
{
}

FdoSmError::FdoSmError(FdoSmErrorType errorType, FdoSchemaExceptionP error) :
	FdoSmSchemaElement( L"", L"" ),
	mType(errorType),
    mError(error)
{
}

FdoSmError::~FdoSmError(void)
{
}

FdoSmErrorType FdoSmError::GetType(void) const
{
	return mType;
}

FdoSchemaExceptionP FdoSmError::CreateException( FdoSchemaException* pPrevException ) const
{
    FdoSchemaExceptionP pException;

    // Most errors just become FdoSchemaException exceptions. However, the FDO API
    // specifies that any errors dealing with data property value autogeneration
    // must become FdoAutogenerationException exceptions.
    switch ( GetType() ) {
    case FdoSmErrorType_AutoGen:
        pException = FdoAutogenerationException::Create( mError->GetExceptionMessage(), pPrevException );
        break;

    case FdoSmErrorType_SpatialMismatch:
        // Todo: create an FdoSpatialContextMismatchException instead.
        // However, first need to relax this function and callers to
        // pass around FdoException instead of FdoSchemaException.
        pException = FdoSchemaException::Create( mError->GetExceptionMessage(), pPrevException );
        break;

    default:
        pException = FdoSchemaException::Create( mError->GetExceptionMessage(), pPrevException );
        break;
    }

    return pException;
}

void FdoSmError::XMLSerialize( FILE* xmlFp, int ref  ) const
{
	fprintf( xmlFp, "<error >\n");
    fprintf( xmlFp, "%ls", mError ? mError->GetExceptionMessage() : L"" );
	fprintf( xmlFp, "</error>\n" );
}

FdoString* FdoSmError::NLSGetMessage(FdoInt32 msgNum, char* defMsg, char* file, int line, ...)
{
    va_list arguments;

    va_start(arguments, line);
#ifdef _WIN32
    FdoString*  result = FdoException::NLSGetMessage(msgNum, defMsg, file, line, "SmMessage.dll", arguments);
#else
    FdoString*  result = FdoException::NLSGetMessage(msgNum, defMsg, file, line, "SmMessage.cat", arguments);
#endif
    va_end(arguments);
    return result;
}

FdoString* FdoSmError::NLSGetMessage(FdoInt32 msgNum, char* defMsg,  ...)
{
    va_list arguments;

    va_start(arguments, defMsg);
#ifdef _WIN32
    FdoString*  result = FdoException::NLSGetMessage(msgNum, defMsg, "SmMessage.dll", arguments);
#else
    FdoString*  result = FdoException::NLSGetMessage(msgNum, defMsg, "SmMessage.cat", arguments);
#endif
    va_end(arguments);
    return result;
}
