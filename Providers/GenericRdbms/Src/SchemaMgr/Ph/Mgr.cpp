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

#include "stdafx.h"
#include <Sm/Ph/PropertyWriter.h>
#include <Sm/Ph/SchemaWriter.h>
#include "Mgr.h"
#include "Rd/QueryReader.h"
#include "CommandWriter.h"
#include "ClassWriter.h"
#include "NullIndicator.h"
#include "SpatialContextWriter.h"
#include "SpatialContextGroupWriter.h"

#define MAX_STACK 200
#define MAX_STATIC_CURSORS 80

//FdoSmPhGrdMgr::StringMap FdoSmPhGrdMgr::mReservedDbObjectNames;

FdoSmPhGrdMgr::FdoSmPhGrdMgr( GdbiConnection* connection ) :
    mGdbiConnection(connection),
    mRdbiContext(connection->GetRdbiContext())
{
}

FdoSmPhGrdMgr::~FdoSmPhGrdMgr(void)
{
}


FdoPtr<FdoSmPhRdQueryReader> FdoSmPhGrdMgr::CreateQueryReader(
    FdoPtr<FdoSmPhRowCollection> rows,
    FdoStringP where,
    FdoPtr<FdoSmPhRow> binds
)
{
    return new FdoSmPhRdGrdQueryReader(rows, where, FDO_SAFE_ADDREF(this), binds );
}

FdoPtr<FdoSmPhRdQueryReader> FdoSmPhGrdMgr::CreateQueryReader(
    FdoPtr<FdoSmPhRow> fields, 
    FdoStringP sStatement, 
    FdoPtr<FdoSmPhRow> binds
)
{
    return new FdoSmPhRdGrdQueryReader(fields, sStatement, FDO_SAFE_ADDREF(this), binds );
}



FdoPtr<FdoSmPhCommandWriter> FdoSmPhGrdMgr::CreateCommandWriter( FdoPtr<FdoSmPhRow> row )
{
    return new FdoSmPhGrdCommandWriter(row, FDO_SAFE_ADDREF(this) );
}

FdoPtr<FdoSmPhNullIndicator> FdoSmPhGrdMgr::CreateNullIndicator()
{
    return new FdoSmPhGrdNullIndicator(mGdbiConnection);
}

FdoStringP FdoSmPhGrdMgr::ClassifyDbObject( FdoStringP tableName, FdoRdbmsOvSchemaMappingP mapping, FdoBoolean hasKey )
{
    FdoRdbmsOvSchemaAutoGenerationP     autoGen = mapping ? mapping->GetAutoGeneration() : NULL;
    FdoStringP className;
    FdoInt32 idx;

    // Reverse-engineering tables to classes only done when schema
    // has an autogeneration directive
    if ( autoGen ) {
            // First check if the table is in the autogeneration table list.
        FdoStringsP tableList = autoGen->GetGenTableList();

        // The following does a case-insensitive search of the table list.
        if ( tableList->IndexOf(tableName, false) >= 0 ) {
            className = tableName;            
        }
        else {
            // Check if the table is on the table list of any other schema
            FdoSchemaMappingsP otherMappings = GetConfigMappings();
            for ( idx = 0; idx < otherMappings->GetCount(); idx++ ) { 
                FdoRdbmsOvSchemaMappingP otherMapping = (FdoRdbmsOvPhysicalSchemaMapping*) otherMappings->GetItem(idx);
                    
                // Skip the given mapping (it was already checked)
                if ( FdoStringP(otherMapping->GetName()) != mapping->GetName() ) {
                    FdoRdbmsOvSchemaMappingP otherMapping2 = (FdoRdbmsOvPhysicalSchemaMapping*) otherMappings->GetItem(
                        GetProviderName(),
                        otherMapping->GetName()
                    );

                    // Skip mappings for other providers
                    if ( otherMapping2 ) {
                        FdoRdbmsOvSchemaAutoGenerationP otherAutoGen = otherMapping ? otherMapping->GetAutoGeneration() : NULL;
                        if ( otherAutoGen ) {
                            FdoStringsP otherTableList = otherAutoGen->GetGenTableList();

                            // The following does a case-insensitive search of the table list.
                            if ( otherTableList->IndexOf(tableName, false) >= 0 ) {
                                // Table is in list for other schema so return L"".
                                // This prevents table from being classified into given
                                // schema
                                return className;  
                            }
                        }
                    }
                }
            }
        
            // Table is not in any table list so check if it matches the table prefix

            // Not in the list so check if it starts with the table prefix

            FdoStringP tablePrefix = autoGen->GetGenTablePrefix();

            if ( tablePrefix != L"" ) {
                if ( tableName.Mid( 0, tablePrefix.GetLength()).ICompare(tablePrefix) == 0 ) {
                    // Table name starts with the prefix. Remove prefix to generate class name
                    // if directed to do so.
                    if ( autoGen->GetRemoveTablePrefix() )
                        className = tableName.Mid( tablePrefix.GetLength(), 9999 );
                    else
                        className = tableName;
                }
            }
            else {
                if ( tableList->GetCount() == 0 ) {
                    // Special case. If table list is empty and table prefix blank then 
                    // always reverse-engineer this table.
                    className = tableName;
                }
            }
        }
    }

    className = className.Replace(L":",L"_").Replace(L".",L"_");

    if ( className != L"" ) 
        className = FdoStringP(mapping->GetName()) + L":" + className;

    return className;
}

FdoStringP FdoSmPhGrdMgr::GetOverrideDatabase(FdoRdbmsOvSchemaMappingP mapping)
{
    return L"";
}

FdoStringP FdoSmPhGrdMgr::GetOverrideOwner(FdoRdbmsOvSchemaMappingP mapping)
{
    return L"";
}

FdoStringP FdoSmPhGrdMgr::GetOverrideTableStorage(FdoRdbmsOvSchemaMappingP mapping)
{
    return L"";
}

FdoStringP FdoSmPhGrdMgr::GetOverrideIndexStorage(FdoRdbmsOvSchemaMappingP mapping)
{
    return L"";
}

FdoStringP FdoSmPhGrdMgr::GetOverrideTextStorage(FdoRdbmsOvSchemaMappingP mapping)
{
    return L"";
}

FdoStringP FdoSmPhGrdMgr::GetOverrideTextInRow(FdoRdbmsOvSchemaMappingP mapping)
{
    return L"";
}

FdoStringP FdoSmPhGrdMgr::GetOverrideTableStorageEngine(FdoRdbmsOvSchemaMappingP mapping)
{
    return L"";
}

bool FdoSmPhGrdMgr::IsRdbUnicode()
{
    return GetGdbiConnection()->GetCommands()->SupportsUnicode();
}

bool FdoSmPhGrdMgr::ConfigIncludeNativeSCs()
{
    FdoBoolean includeNative = false;
    if ( mConfigMappings ) {
        FdoInt32 idx;

        for ( idx = 0; idx < mConfigMappings->GetCount(); idx++ ) {
            FdoRdbmsOvSchemaMappingP configMapping = (FdoRdbmsOvPhysicalSchemaMapping*) mConfigMappings->GetItem(idx);
            FdoRdbmsOvSchemaAutoGenerationP autoGen = configMapping ? configMapping->GetAutoGeneration() : NULL;

            if ( autoGen ) {
                // At least one Schema Mapping set has an autogeneration element, so some classes may be reverse
                // engineered, rather than specified directly in the config document. This means that geometric 
                // properties in these classes can reference spatial contexts not in the config doc, and we need
                // to merge sc's from the datastore into those from the config doc when determining all sc's for
                // the current connection.
                includeNative = true;
                break;
            }
        }
    }

    return includeNative;
}



FdoSmPhSchemaWriterP FdoSmPhGrdMgr::NewSchemaWriter()
{
    return new FdoSmPhSchemaWriter( FDO_SAFE_ADDREF(this) );
}

FdoSmPhClassWriterP FdoSmPhGrdMgr::NewClassWriter()
{
    return (FdoSmPhClassWriter*)(new FdoSmPhGrdClassWriter(FDO_SAFE_ADDREF(this)) );
}

FdoSmPhPropertyWriterP FdoSmPhGrdMgr::NewPropertyWriter()
{
    return new FdoSmPhPropertyWriter( FDO_SAFE_ADDREF(this) );
}

FdoSmPhDependencyWriterP FdoSmPhGrdMgr::NewDependencyWriter()
{
    return new FdoSmPhDependencyWriter( FDO_SAFE_ADDREF(this) );
}

FdoSmPhAssociationWriterP FdoSmPhGrdMgr::NewAssociationWriter()
{
    return new FdoSmPhAssociationWriter( FDO_SAFE_ADDREF(this) );
}

FdoSmPhSADWriterP FdoSmPhGrdMgr::NewSADWriter()
{
    return (FdoSmPhSADWriter*)(new FdoSmPhSADWriter(FDO_SAFE_ADDREF(this)) );
}

FdoSmPhSpatialContextWriterP FdoSmPhGrdMgr::NewSpatialContextWriter()
{
    return (FdoSmPhSpatialContextWriter*)(new FdoSmPhGrdSpatialContextWriter(FDO_SAFE_ADDREF(this)) );
}

FdoSmPhSpatialContextGroupWriterP FdoSmPhGrdMgr::NewSpatialContextGroupWriter()
{
    return (FdoSmPhSpatialContextGroupWriter*)(new FdoSmPhGrdSpatialContextGroupWriter(FDO_SAFE_ADDREF(this)) );
}

FdoBoolean FdoSmPhGrdMgr::IsDbObjectNameReserved( FdoStringP objName )
{
    return mReservedDbObjectNames.IsReserved( objName );
}

// This function was copied from adb/exec_file.c
void FdoSmPhGrdMgr::ExecSchemaFile( FdoStringP fileName, FdoStringsP keywords )
{
    GdbiConnection* gdbiConn = GetGdbiConnection();
    char sql_buf[10*1024];      /* buffer to build SQL commands */
    char line_buf[256];         /* buffer to read from file */
    char *ptr;                  /* pointer into buffer */
    char *separator;            /* pointer to separator (';') */
    FILE *init_file;            /* channel for file open/read/close */
    FdoSize count;              /* byte count from read */
    char stack[MAX_STACK];      /* Stack for keeping track of nested #ifs */
                                /* 'i' = include, 'e' = exclude           */
    char *stack_top = stack;    /* Initialize to empty position.          */
    int including_lines = TRUE; /* True when not excluded by #if[n]def    */
    int reading_plsql = FALSE;  /* TRUE when looking for the "end" tag of PL/SQL block. */
    int want_sep_in_sql = FALSE;/* TRUE when separator is wanted in SQL string (for PL/SQL).*/
    int  inside_comment = 0; /* True if inside C-style comment. */
    char quote_char = '\0';     /* Either ' or " when in a quote   */

#define STACK_EMPTY (stack_top == stack)

    *stack_top = 'i';           /* Default to including lines of text */
    inside_comment = FALSE;

    FdoStringP fullFileName = FdoStringP::Format(
        L"%ls/%ls",
        (FdoString*) GetHomeDir(),
        (FdoString*) fileName
    );
#ifdef _WIN32
    init_file = _wfopen( (const wchar_t*) fullFileName, L"rt" );
#else
    init_file = fopen( (const char*) fullFileName, "rt" );
#endif
    if (init_file == NULL) {                        /* errors - so exit */
        throw FdoSchemaException::Create (
            NlsMsgGet1 (
                FDORDBMS_142,
	            "Could not open file: '%1$ls'",
	            (FdoString*)fullFileName 
	        )
	    );
    }

 /* ----------------------------------------------
      Parse the file, searching for separators.
    ----------------------------------------------  */

    ptr = sql_buf;
    while (fgets (line_buf, 256, init_file) != NULL) {
        int ignore_this_line = FALSE;       /* True if current line       */
                                            /* should be ignored.         */
        want_sep_in_sql = FALSE;

        RemoveFileComments(line_buf, &inside_comment, &quote_char);          /* Strip out C-style comments */

        if (line_buf[0] == '#') {
            char keywd[256];                /* Keyword after #if[n]def    */

            ignore_this_line = TRUE;        /* Don't process line         */

            if (sscanf(line_buf, "#ifdef %s", keywd) == 1) {
                stack_top++;
                if ( keywords->IndexOf(keywd) >= 0 ) {
                    *stack_top = 'i';   /* Include following lines */
                }
                else {
                    *stack_top = 'e';   /* Exclude following lines */
                }
            }
            else if (sscanf(line_buf, "#ifndef %s", keywd) == 1) {
                stack_top++;
                if ( keywords->IndexOf(keywd) >= 0 ) {
                    *stack_top = 'e';   /* Exclude following lines */
                }
                else {
                    *stack_top = 'i';   /* Include following lines */
                }
            }
            else if (strncmp(line_buf, "#else", 5) == 0) {
                if (STACK_EMPTY) {
                    throw FdoSchemaException::Create (
                        NlsMsgGet1 (
                            FDORDBMS_143,
	                        "#else without #if[n]def in file: '%1$ls'",
	                        (FdoString*)fullFileName 
	                    )
	                );
                }
                /* Toggle between 'e' and 'i' on top of stack */
                if (*stack_top == 'e')
                    *stack_top = 'i';
                else
                    *stack_top = 'e';
            }
            else if (strncmp(line_buf, "#endif", 6) == 0) {
                if (STACK_EMPTY) {
                    throw FdoSchemaException::Create (
                        NlsMsgGet1 (
                            FDORDBMS_144,
	                        "#endif without #if[n]def in file: '%1$ls'",
	                        (FdoString*)fullFileName 
	                    )
	                );
                }
                stack_top--;
            }
            else {
                /* The line begins with '#' but it's not        */
                /* something we recognize.  Issue a warning.    */

                throw FdoSchemaException::Create (
                    NlsMsgGet2 (
                        FDORDBMS_145,
                        "Unknown directive '%1$ls' in file: '%2$ls'",
                        (FdoString*)FdoStringP(line_buf),
	                    (FdoString*)fullFileName 
	                )
	            );
            }
            *(stack_top+1) = '\0';      /* Null terminate for strpbrk   */

            /* including_lines is true if there are no occurences of    */
            /* 'e' (exclude) on the stack.                              */
            including_lines = ((char*)strpbrk(stack, "e") == NULL);
        } else if (strncmp(line_buf, "create or replace procedure", 27) == 0 || strncmp(line_buf, "begin", 5) == 0) {
            /* This is the beginning of a PL/SQL block.  It may contain many
             * statements, and we want to read until the "end" tag. */
            reading_plsql = TRUE;
        } else if (strncmp(line_buf, "end", 3) == 0) {
            /* This is the end of a PL/SQL block.  This line should end
             * with a separator character, so we can just take all collected
             * SQL text as usual now. */
            reading_plsql = FALSE;
            want_sep_in_sql = TRUE;
        }


        /* Ignore statements with leading '$' or "rem".                 */
        /* Also ignore lines if exclusion by an #if[n]def is in effect. */

        if (( ! including_lines) ||
            (line_buf[0] == '$') ||
            (strncmp (line_buf, "rem", 3) == 0) ||
            (strncmp (line_buf, "REM", 3) == 0)) {
            ignore_this_line = TRUE;
        }

        if (!ignore_this_line) {

            count = strlen (line_buf);

            /* Search for a ';' character */
            separator = (char*)strpbrk(line_buf, ";");

            /* if the separator was found ... */
            if (separator != NULL && !reading_plsql) {
                int sep_extra = (want_sep_in_sql? 1 : 0);
                /* copy line into main buffer */
                strncpy (ptr, line_buf, (separator - line_buf) + sep_extra);
                /* adjust the end pointer */
                ptr += (separator - line_buf) + sep_extra;
                /* NULL terminate */
                *ptr = '\0';

                /* Strip off leading and trailing whitespace */
                //TODO Move ut_strstrip to FdoStringP
//                ut_strstrip(sql_buf, sql_buf);

                if ( sql_buf[0] != '\0' ) {
                    /* Perform the RDBMS calls to execute this command */
                    gdbiConn->ExecuteNonQuery( sql_buf );
                }
                /* reset this pointer for next command */
                ptr = sql_buf;
            } else {
                /* add to the buffer */
                strncpy (ptr, line_buf, count);
                ptr += count;
            }
        }
    }

    fclose (init_file);                         /* close the file   */

    if ( ! (STACK_EMPTY) ) {
        throw FdoSchemaException::Create (
            NlsMsgGet1 (
                FDORDBMS_146,
                "One or more missing #endifs in file: '%1$ls'",
                (FdoString*)fullFileName 
            )
        );
    }
}

void FdoSmPhGrdMgr::ExecuteSQL( FdoStringP sqlStmt, bool isDDL )
{
    GdbiConnection*     gdbiConn = GetGdbiConnection();

    gdbiConn->ExecuteNonQuery( (FdoString*) sqlStmt, isDDL );
}

// This function was copied from adb/exec_file.c
void FdoSmPhGrdMgr::RemoveFileComments( char* line, int* inside_comment, char* quote_char)
{
    char        *src;                   /* Temporary pointer (source)      */
    char        *dst;                   /* Temporary pointer (destination) */
    int         zero_written = FALSE;   /* True when '\0' written.         */

    /* src scans along the line, skipping over comments.  When it's */
    /* not inside a comment, *src is copied over *dst.              */

    /* first search for an "interesting" character, ie. either a    */
    /* '*' or a '/' or a quote.                                     */

    src = (char*)strpbrk(line, "/*'\"");
    if (src == NULL) {              /* No comments starting or ending */
        if (*inside_comment) {
            line[0] = '\n';         /* Preserve the line, but         */
            line[1] = '\0';         /* zero it out, since             */
                                    /* it's inside a comment.         */
        }
        return;
    }

    src = line;
    dst = line;
    while (!zero_written) {                 /* while not at end of string */
        if ( ! *inside_comment ) {
            /* Only look for quotes if not inside a comment */
            if ((*quote_char == '\0') &&
                ((*src == '\'') || (*src == '"'))) {
                *quote_char = *src;                          /* Opening quote */
            }
            else if ((*quote_char != '\0') && (*src == *quote_char)) {
                *quote_char = '\0';                          /* Closing quote */
            }
        }

        if (*quote_char == '\0') {
            /* Only look for comments if not inside a quote */
            if ((*src == '/') && (*(src+1) == '*')) {       /* Comment start */
                *inside_comment = TRUE;
                src++;
                src++;  /* Get past the '/' and the '*' */
            }
            else if ((*src == '*') && (*(src+1) == '/')) {  /* Comment end */
                *inside_comment = FALSE;
                src++;
                src++;  /* Get past the '*' and the '/' */
            }
        }

        if ((*quote_char == '\0') && ( *src == ''))
            *src = ' '; /* Replace  with a space if not inside quote */

        if ( (! *inside_comment) || (*src == '\0') ) {
            *dst = *src;
            if (*dst == '\0') {
                zero_written = TRUE;
                if (*inside_comment) {
                    *dst = '\n';        /* Retain the newline. */
                    *(dst+1) = '\0';
                }
            }
            dst++;
            src++;
        }
        else {
            /* Inside a comment, so leave dst where it is and   */
            /* advance src.                                     */
            src++;
        }
    }
}

FdoSmPhGrdMgr::StringMap::StringMap()
{
    // List of words reserved by most RDBMS's
    // TODO: some of these might be Oracle-specific and need to move to
    // FdoSmPhOraMgr.

    Insert( L"access" ); 
    Insert( L"add" ); 
    Insert( L"all" ); 
    Insert( L"alter" ); 
    Insert( L"and" );        
    Insert( L"any" ); 
    Insert( L"as" ); 
    Insert( L"asc" ); 
    Insert( L"audit" );

    Insert( L"between" ); 
    Insert( L"btree" );
    Insert( L"by" );

    Insert( L"cascade" ); 
    Insert( L"char" ); 
    Insert( L"check" );  
    Insert( L"childstate" ); 
    Insert( L"classid" ); 
    Insert( L"cluster" );
    Insert( L"column" ); 
    Insert( L"comment" ); 
    Insert( L"commit" ); 
    Insert( L"compress" ); 
    Insert( L"connect" ); 
    Insert( L"constraint" ); 
    Insert( L"create" ); 
    Insert( L"current" );

    Insert( L"date" ); 
    Insert( L"decimal" ); 
    Insert( L"default" ); 
    Insert( L"delete" ); 
    Insert( L"delstatus" ); 
    Insert( L"desc" );
    Insert( L"distinct" ); 
    Insert( L"drop" );

    Insert( L"else" ); 
    Insert( L"exclusive" ); 
    Insert( L"exists" );

    Insert( L"file" ); 
    Insert( L"float" ); 
    Insert( L"for" ); 
    Insert( L"foreign" ); 
    Insert( L"format" ); 
    Insert( L"from" );

    Insert( L"grant" ); 
    Insert( L"group" );

    Insert( L"having" );

    Insert( L"identified" ); 
    Insert( L"if" ); 
    Insert( L"immediate" ); 
    Insert( L"in" ); 
    Insert( L"increment" ); 
    Insert( L"index" );
    Insert( L"initial" ); 
    Insert( L"insert" ); 
    Insert( L"integer" ); 
    Insert( L"integer1" );
    Insert( L"intersect" ); 
    Insert( L"into" ); 
    Insert( L"is" ); 
    Insert( L"isam" ); 

    Insert( L"join" ); 

    Insert( L"key" );

    Insert( L"level" ); 
    Insert( L"like" ); 
    Insert( L"lock" ); 
    Insert( L"long" ); 
    Insert( L"ltlock" );

    Insert( L"minus" ); 
    Insert( L"mlslabel" ); 
    Insert( L"mode" ); 
    Insert( L"modify" );

    Insert( L"new" ); 
    Insert( L"next" ); 
    Insert( L"nextver" ); 
    Insert( L"noaudit" ); 
    Insert( L"nocompress" ); 
    Insert( L"not" ); 
    Insert( L"nowait" ); 
    Insert( L"null" ); 
    Insert( L"number" );

    Insert( L"of" ); 
    Insert( L"offline" ); 
    Insert( L"on" ); 
    Insert( L"online" ); 
    Insert( L"option" );
    Insert( L"or" ); 
    Insert( L"order" );
    Insert( L"outer" );

    Insert( L"parentstate" ); 
    Insert( L"primary" ); 
    Insert( L"prior" ); 
    Insert( L"privileges" ); 
    Insert( L"public" );

    Insert( L"raw" ); 
    Insert( L"rename" ); 
    Insert( L"resource" ); 
    Insert( L"revisionnumber" ); 
    Insert( L"revoke" );
    Insert( L"row" ); 
    Insert( L"rowid" ); 
    Insert( L"rowlockid" ); 
    Insert( L"rowlocktype" ); 
    Insert( L"rownum" ); 
    Insert( L"rows" );

    Insert( L"select" ); 
    Insert( L"set" ); 
    Insert( L"share" ); 
    Insert( L"size" ); 
    Insert( L"smallint" );
    Insert( L"start" ); 
    Insert( L"successful" ); 
    Insert( L"sum" );
    Insert( L"synonym" ); 

    Insert( L"table" ); 
    Insert( L"than" ); 
    Insert( L"then" ); 
    Insert( L"to" ); 
    Insert( L"trigger" );
    Insert( L"triggers" );

    Insert( L"uid" ); 
    Insert( L"union" ); 
    Insert( L"unique" ); 
    Insert( L"update" ); 
    Insert( L"user" );
    Insert( L"using" );

    Insert( L"validate" ); 
    Insert( L"value" ); 
    Insert( L"values" ); 
    Insert( L"varchar" ); 
    Insert( L"varchar2" ); 
    Insert( L"view" );

    Insert( L"whenever" ); 
    Insert( L"where" ); 
    Insert( L"with" ); 
}

void FdoSmPhGrdMgr::StringMap::Insert( FdoString* resString)
{
    insert( std::pair<FdoStringP,void*>(FdoStringP(resString,true), NULL) );            
}

bool FdoSmPhGrdMgr::StringMap::IsReserved( FdoStringP checkString)
{
    std::map <FdoStringP,void*> :: const_iterator iter =
        find( checkString.Lower() );

    return ( iter != end() );
}

void FdoSmPhGrdMgr::SetStaticReader( FdoStringP name, FdoPtr<FdoSmPhReader> reader )
{
	if (!mStaticReaders)	
		mStaticReaders = new FdoSmPhStaticReaderCollection();
	else	{
		if ( mStaticReaders->GetCount() < MAX_STATIC_CURSORS )	{
			FdoSmPhStaticReaderP staticReader = new FdoSmPhStaticReader( name, reader );
			mStaticReaders->Add( staticReader );
		}
	}
}

FdoPtr<FdoSmPhReader> FdoSmPhGrdMgr::GetStaticReader ( FdoStringP readerName )
{
	if ( !mStaticReaders )
		return (FdoSmPhReader *) NULL;
	FdoSmPhStaticReaderP staticReader = mStaticReaders->FindItem( (const wchar_t*)readerName );
	if (staticReader)
		return staticReader->GetReader();
	else
		return (FdoSmPhReader *) NULL;
}

void FdoSmPhGrdMgr::RemoveStaticReaders()
{
	if ( mStaticReaders )	
		mStaticReaders->Clear();
}
