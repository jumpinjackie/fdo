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
/* This executable converts the contents of a file into an array of strings
   that can be included in a C++ source file.

    It takes the following:
        argv[1] - input file
        argv[2] - output file
        argv[3] - variable

    and creates the output file in C++ header format. The output contains
    a single static variable (type: array of char*) with a string for each
    line from the input file.

    The array is terminated by a NULL element.
*/


#include "stdafx.h"
#ifdef _WIN32
#include <tchar.h>

int _tmain(int argc, _TCHAR* argv[])
#else
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
#endif
{
    FILE* inFile = fopen( argv[1], "r" );
    FILE* outFile = fopen( argv[2], "w+" );


    if ( !inFile || !outFile ) {
        printf( "Error in FileToHdr %s %s\n", argv[1], argv[2] );
        return 1;
    }
    char buffer[5000];
    unsigned int i;

    // Write the "include once" part
    fprintf( outFile, "#ifndef %s_H\n", argv[3] );
    fprintf( outFile, "#define %s_H\n\n", argv[3] );

    // Declare the variable as an array of string pointers 
    fprintf( outFile, "static const char* %s[] = {\n", argv[3] );

    // Make each line from the input file an element in the array
    while( fgets(buffer, 4999, inFile) ) {

        fprintf( outFile, "\"" );
        for ( i = 0; i < strlen(buffer); i++ ) {
            if ( buffer[i] == '\\' || buffer[i] == '"' )
                fprintf( outFile, "\\" );

            if ( buffer[i] != '\n' && buffer[i] != '\010' ) 
                fprintf( outFile, "%c", buffer[i] );
        }

        fprintf( outFile, "\",\n" );
    }

    // NULL terminate the array.
    fprintf( outFile, "NULL\n};\n\n", argv[3] );
    fprintf( outFile, "#endif\n", argv[3] );

    fclose( inFile );
    fclose( outFile );
	return 0;
}


