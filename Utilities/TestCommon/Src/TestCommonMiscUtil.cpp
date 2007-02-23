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
 *         $Id: //providers_reliant/TestCommon/src/TestCommonMiscUtil.cpp#1 $
 *     $Author: dalcoup $
 *   $DateTime: 2006/09/25 15:47:54 $
 *     $Change: 11018 $
 *
 */

#include "stdafx.h"
#include "TestCommonMiscUtil.h"


void TestCommonFail (FdoException* ge)
{
    wchar_t message[2000];
    char* multibyte;
    FdoPtr<FdoException> cause;

    wcscpy(message, (wchar_t*)ge->GetExceptionMessage ());
    cause = ge->GetCause();
    while (cause != NULL)
    {
        wcscat(message, L" [");
        wcscat(message, cause->GetExceptionMessage());
        wcscat(message, L"]");
        cause = cause->GetCause();
    }
    wide_to_multibyte (multibyte, message);
    if (NULL == multibyte)
        multibyte = "Exception message could not be converted.";
    ge->Release ();
    CPPUNIT_FAIL (multibyte);
}


void TestCommonFail (char* error)
{
    char message[4096];
    sprintf (message, "\nFAILURE: %s\n", error);
    CPPUNIT_FAIL (message);
}


FdoPropertyValue* TestCommonMiscUtil::AddNewProperty( FdoPropertyValueCollection* propertyValues, FdoString* name )
{
    FdoPropertyValue*  propertyValue = propertyValues->FindItem( name );

    if ( !propertyValue ) 
    {
        propertyValue =  FdoPropertyValue::Create();
        propertyValue->SetName( name );
        propertyValues->Add( propertyValue );
    }
    
    return propertyValue;
}

FdoPtr<FdoIInsert>  TestCommonMiscUtil::InsertObject( FdoIConnection* connection, FdoPtr<FdoIInsert> insertCmd, FdoStringP schemaName, FdoString* className, ... )
{
    va_list arguments;

    FdoString* propName;

    va_start(arguments, className);

    propName = va_arg(arguments,FdoString*);

    FdoPtr<FdoIInsert> insertCommand = insertCmd;
    
    if ( !insertCommand )
        insertCommand = (FdoIInsert *) connection->CreateCommand(FdoCommandType_Insert);

    FdoPtr<FdoPropertyValueCollection> propertyValues;
    FdoPtr<FdoDataValue> dataValue;
    FdoPtr<FdoPropertyValue> propertyValue;

    insertCommand->SetFeatureClassName(schemaName + L":" + FdoStringP(className));
    propertyValues = insertCommand->GetPropertyValues();

    while ( propName != NULL ) {

        propertyValue = AddNewProperty( propertyValues, propName);
        
        propertyValue->SetValue( ArgsToDataValue(arguments) );

        propName = va_arg(arguments,FdoString*);
    }
        
	FdoPtr<FdoIFeatureReader> rdr = insertCommand->Execute();

    va_end(arguments);

    return insertCommand;
}

void TestCommonMiscUtil::DeleteObjects( FdoIConnection* connection, FdoStringP schemaName, FdoString* className, ... )
{
    va_list arguments;

    FdoPtr<FdoIDelete> deleteCommand = (FdoIDelete *) connection->CreateCommand(FdoCommandType_Delete);
    deleteCommand->SetFeatureClassName(schemaName + L":" + className);

    va_start(arguments, className);

    FdoString* propName = va_arg(arguments,FdoString*);

    if ( propName != NULL ) {

        FdoPtr<FdoIdentifier> pIdent = FdoIdentifier::Create( propName );
        FdoPtr<FdoComparisonCondition> pCompare = FdoComparisonCondition::Create(pIdent, FdoComparisonOperations_EqualTo, ArgsToDataValue(arguments) );
       
        deleteCommand->SetFilter( pCompare );
    }
        
    va_end(arguments);
    
    deleteCommand->Execute();
}

FdoPtr<FdoDataValue> TestCommonMiscUtil::ArgsToDataValue( va_list& arguments )
{
    FdoPtr<FdoDataValue> dataValue;
    FdoBoolean boolArg;
    FdoByte byteArg;
    FdoInt32 int32Arg;
    FdoFloat floatArg;
    FdoDouble doubleArg;
    FdoString* strArg;

    FdoDataType dataType = (FdoDataType) va_arg(arguments,int);

    switch ( dataType ) {
    case FdoDataType_Boolean:
        boolArg = (FdoBoolean) va_arg(arguments,int);
        dataValue = FdoDataValue::Create(boolArg);
        break;
    case FdoDataType_Byte:
        byteArg = (FdoByte) va_arg(arguments,int);
        dataValue = FdoDataValue::Create(byteArg);
        break;
    case FdoDataType_Int32:
        int32Arg = va_arg(arguments,FdoInt32);
        dataValue = FdoDataValue::Create(int32Arg);
        break;
    case FdoDataType_Single:
        floatArg = (FdoFloat) va_arg(arguments,double);
        dataValue = FdoDataValue::Create(floatArg);
        break;
    case FdoDataType_Double:
    case FdoDataType_Decimal:
        doubleArg = (FdoDouble) va_arg(arguments,double);
        dataValue = FdoDataValue::Create(doubleArg, dataType);
        break;
    case FdoDataType_String:
        strArg = va_arg(arguments,FdoString*);
        dataValue = FdoDataValue::Create(strArg);
        break;
    default:
        throw FdoException::Create( L"TestCommonMiscUtil::InsertObject dataType not yet implemented; please implement" );
        break;
    }

    return dataValue;
}

#ifdef _WIN32
void TestCommonMiscUtil::SetupLeakReport()
{
    DeleteFile( L"leak_rpt.txt" );
    _CrtSetReportHook( TestCommonMiscUtil::LeakReportHook );
    _CrtSetBreakAlloc( 3453788 );
}

int TestCommonMiscUtil::LeakReportHook( int reportType, char *message, int *returnValue )
{
    static int allocNum = 0;
    const char* className = NULL;
    static const char* ukCl = "Unknown: name retrieve failed";

    FILE* outFp = fopen( "leak_rpt.txt", "at" );

    if ( message[0] == '{' ) {
        sscanf( message, "{%d}", &allocNum );
    }
    else if ( strncmp(message, "normal block at", 15) == 0 ) {
        long addr;
        bool isClass = false;
        sscanf( message, "normal block at %x", &addr );
        FdoIDisposable* disp = NULL;
        FdoSchemaElement* elem = NULL;
        try {
            const type_info& info = typeid( *((FdoIDisposable*)(addr)) );
            if ( (((long)(&info)) & 0x00ffffff) != 0x00cccccc ) {
                // BUG: this sometimes generates a segv. Need a better way to validate
                // the info returned by typeid().
                className = typeid( *((FdoIDisposable*)(addr)) ).name();
                isClass = true;
            }
            else {
                className = ukCl;
            }
        }
        catch (...) {
            className = NULL;
        }

        if ( className  ) {
            try {
                // Assume bad class name of > 199 characters
                char checkstr[200];
                strncpy( checkstr, className, 199 );
                checkstr[199] = 0;
                if ( strlen(checkstr) >= 199 )
                    className = NULL;
            }
            catch (...) {
                className = NULL;
            }
        }

        if ( className ) {
            fprintf( outFp, "{%d} %s", allocNum, className );

            if ( strncmp( &className[6], "Fdo", 3 ) == 0 ) {
                try {
                    disp = dynamic_cast<FdoIDisposable*>( (FdoIDisposable*) addr );
                    if ( disp ) {
                        fprintf( outFp, "; refcount %d", disp->GetRefCount() );
                        elem = dynamic_cast<FdoSchemaElement*>(disp);

                        if ( elem ) 
                            fprintf( outFp, "; Name '%ls'", (FdoString*) elem->GetName() );
                    }
                }
                catch ( ... ) 
                {
                }
            }

            fprintf( outFp, "\n" );
            fflush( outFp );
        }
        else {
            fprintf( outFp, "{%d} %s", allocNum, message );
        }
    }

    fclose(outFp);

    return(0);
}
#endif



// Get time on system clock (in seconds) -- used for computing elapsed time.
double TestCommonMiscUtil::GetTime_S(void)
{
#ifdef _WIN32   

   double our_time;
 
   our_time = GetTickCount() / 1000.0;
   return our_time;
 
#else

	struct timeval			this_time;
	static struct timezone	time_zone = { 0, 0 };
	double					sec, micro, time;


	if (gettimeofday(&this_time, &time_zone) == -1)
		return (double) 0.0;

	micro = (double) ((double) this_time.tv_usec / (double) 1000000.0);
	sec = (double) this_time.tv_sec;
	time = micro + sec;
	return time;
	
#endif 
}


bool TestCommonMiscUtil::FuzzyEqual (const double d1, const double d2)
{
    if ((d1==0.0) || (d2==0.0))
        return 1e-5 > fabs(d1 - d2);
    else
        return 1e-5 > fabs(1.0 - (d2 / d1));
}

