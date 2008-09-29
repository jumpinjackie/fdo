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
/* This executable converts NLS message catalogue (MC file) to 
   an MSF file.

    It takes the following:
        argv[1] - input file (usually a .mc file )
        argv[2] - output file (usually a .msf file)
        argv[3] - Symbolic name prefix. Only symbolic names
                  starting with this prefix are copied.
                  if '*' then all symbolic names are copied.
                  if '*' then argv[4] must be specified.
        argv[4] - catalogue name. Substituted into line 3 of
                  the MSF file: "$set S%s_1".
                  Optional argument. Defaults to argv[3].
        argv[5] - if set then add the "$set 9 ErrorHandler line"
                  needed by dbgql.msf, etc.
                  if NULL then don't add this line.

    It assumes that the MC file follows the standard message
    catalogue format; if not then the contents of the .msf file
    produced will not be right.
*/


#include "stdafx.h"

bool doPass( char* inFile, FILE* outFile, char* prefix, char* catalogue, char* setError, int passNum );

#ifdef _WIN32
#include <tchar.h>

int _tmain(int argc, _TCHAR* argv[])
#else
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
#endif
{
    if ( argc < 4 ) {
        fprintf(stderr, "Parameter 3 (Symbolic name prefix) not specified\n" );
        return 1;
    }

    FILE* outFile = fopen( argv[2], "w+" );

    if ( !outFile ) {
        fprintf(stderr, "Failed to open output file\n" );
        return 1;
    }


    if ( (strcmp( argv[3], "*" ) == 0) && !argv[4] ) {
        fprintf(stderr, "Parameter 4 must be specified when parameter 3 is '*'\n" );
        return 1;
    }

    if ( !doPass( argv[1], outFile, argv[3], argc > 4 ? argv[4] : NULL, argc > 5 ? argv[5] : NULL, 1 ) ) 
        return 0;

    if ( !doPass( argv[1], outFile, argv[3], argc > 4 ? argv[4] : NULL, argc > 5 ? argv[5] : NULL, 2 ) )
        return 0;

    fclose( outFile );
	return 0;
}

bool doPass( char* inFileName, FILE* outFile, char* prefix, char* catalogue, char* setError, int passNum )
{
    FILE* inFile = fopen( inFileName, "r" );

    if ( !inFile ) {
        fprintf(stderr, "Failed to open input file\n" );
        return false;
    }

    char buffer[5000];
    char buffer2[5000];
    char symNamePrefix[5000];
    int i;
    int state = 0;
    bool errHdgWritten = false;

    if ( strcmp( prefix, "*" ) == 0 ) 
        // Copy any message
        strcpy( symNamePrefix, "SymbolicName=");
    else
        // Copy only messages with the given prefix.
        sprintf( symNamePrefix, "SymbolicName=%s", prefix );

    if ( passNum == 1 ) {
        // Write common first 3 msf file lines.
        fprintf( outFile, "$quote \"\n\n" );
        fprintf( outFile, "$set S_%s_1\n", catalogue ? catalogue : prefix );
    }

    // Read the input file
    while( fgets(buffer, 4999, inFile) ) {

        if ( buffer[strlen(buffer)-1] == '\n' )
            buffer[strlen(buffer)-1] = 0;

        switch (state) {
        case 0:
            // Check if symbolic name declared
            if ( strncmp(buffer, symNamePrefix, strlen(symNamePrefix)) == 0 ) {
                // Chop out trailing blanks
                for ( i = (((int)strlen(buffer2)) - 1); 
                    (i >= 0) && (buffer2[i] == ' ' || buffer[2] == '\n');
                    i--
                )
                    buffer2[i] = 0;

                // Extract the message id part
                strcpy( buffer2, &buffer[13] );

                // Extract the message number part, if any
                char* pMsgNum = strchr( buffer2, '_' );
                int msgNum = 0;
 
                if ( pMsgNum ) {
                    pMsgNum++;
                    msgNum = atoi(pMsgNum);
                }

                // Handle all messages with number in pass 1
                // Handle all messages in pass 1 if not adding the error line
                if ( passNum == 1 && (setError == NULL || msgNum > 0) ) {
                    // Write the message id (no \n since message
                    // text is on same line).
                    fprintf( outFile, buffer2 );
                    state = 1;
                }
                else if ( passNum == 2 && setError && msgNum == 0 ) {
                    // Handle all "9 error" messages in pass 2.
                    if ( !errHdgWritten ) {
                        fprintf( outFile, "\n$set 9 Error Handler Messages\n" );
                        errHdgWritten = true;
                    }

                    // Write the message id (no \n since message
                    // text is on same line).
                    fprintf( outFile, buffer2 );
                    state = 1;
                }
            }

            break;

        case 1:
            // Sanity check, make sure Language line follows
            // symbolic name line.
            if ( strncmp( buffer, "Language=", 9 ) == 0 ) {
                state = 2;
            }
            else {
                fprintf( outFile, "ERROR!!! ERROR!!!\n" );
                state = 0;
            }

            break;

        case 2:
            // 2 lines after symbolic name is the message text.
            if ( strcmp(buffer,".") == 0 ) 
                fprintf( outFile, " \"\"\n" );
            else
                fprintf( outFile, " \"%s\"\n", buffer );
            state = 0;
            break;
        }
    }

    fclose( inFile );
	return true;
}


