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
/* This executable adjusts relative paths in the FDO XML schema (XSD)
    files that are included in the FDO SDK.

    In the FDO development environment, the generic XSD files are kept
    in Fdo/Docs/XmlSchema, while the provider specific files are under
    the corresponding Docs/XmlSchema directory for the provider.

    In the SDK, all of these files reside in the same directory.

    The provider specific files import the generic files. These imports
    are specified through relative paths based on the development 
    directory structure. This program removes the relative path parts
    of these import specifications to produce files base on the SDK 
    directory structure.

    The program arguments are a follows:
        argv[1] - input directory. All files in this directory, with
                  .xsd suffix, are processed
        argv[2] - output directory. The modified files are written
                  to this directory. The input files remain untouched.

*/



#include "stdafx.h"
#ifdef _WIN32
#include "windows.h"
#else
#include <dirent.h>
#include <string.h>
#endif

bool doFile( char* inDir, char* outDir, char* fileName );
void doLine( FILE* outFp, char* buffer );

#ifdef _WIN32
int _tmain(int argc, _TCHAR* argv[])
#else
int main(int argc, char* argv[])
#endif

{
    if ( argc < 3 ) {
        printf( "usage XsdInstallPaths <input directory> <output directory>\n" );
        return 1;
    }

    char    fileMask[1000];

    // Process each XSD file in the input directory
    sprintf( fileMask, "%s/*.xsd", argv[1] );

#ifdef _WIN32
    WIN32_FIND_DATA fdata;
    HANDLE fh;
    fh = FindFirstFile( (LPCTSTR) fileMask, &fdata );

    while ( fh != INVALID_HANDLE_VALUE ) {

        if ( strcmp( fdata.cFileName, "." ) &&
             strcmp( fdata.cFileName, ".." )) {

            if ( !doFile( argv[1], argv[2], fdata.cFileName ) )
                return 1;
        }

        if ( !FindNextFile( fh, &fdata ) ) {
            FindClose(fh);
            break;
        }
    }
#else
    DIR* pDir = opendir(argv[1]);

    dirent* dirEntry = readdir(pDir);

    while ( dirEntry ) {

        if ( strcmp( &(dirEntry->d_name[strlen(dirEntry->d_name) - 4]), ".xsd" ) == 0 ) {
            if ( !doFile( argv[1], argv[2], dirEntry->d_name ) )
                return 1;
        }

        dirEntry = readdir(pDir);
    }

    closedir(pDir);
#endif
    return 0;
}

// Processes a single XSD file.
bool doFile( char* inDir, char* outDir, char* fileName )
{
    char inFile[1000];
    char outFile[1000];

    sprintf( inFile, "%s/%s", inDir, fileName );
    sprintf( outFile, "%s/%s", outDir, fileName );

    FILE* inFp = fopen( inFile, "r" );
    if ( inFp == NULL ) {
        printf( "XsdInstallPaths failed to open input file %s\n", inFile );
        return false;
    }

    FILE* outFp = fopen( outFile, "w" );
    if ( outFp == NULL ) {
        printf( "XsdInstallPaths failed to open output file %s\n", outFile );
        return false;
    }

    char buffer[100000];

    while ( fgets( buffer, 99999, inFp ) ) {
        doLine( outFp, buffer );
    }

    fclose(inFp);
    fclose(outFp);

    printf( "XsdInstallPaths: wrote %s\n", outFile );

    return true;
}

// Process a line in the XSD file.
// This is done by a state-driven partial parse. This parser
// is not completely bullet-proof but works for our current XSD files.
void doLine( FILE* outFp, char* buffer )
{
    static int state = 0;
    int i;
    int j = 0;
    int savePoint;
    char outBuffer[100000];

    // Process each character.
    for ( i = 0; i < (int)(strlen(buffer)); i++ ) {
        switch ( state ) {
        case 0: //start state,looking for xs:import tag
            if ( strncmp( &buffer[i], "<xs:import", 10 ) == 0 ) 
                state = 1;

            outBuffer[j++] = buffer[i];
            break;

        case 1: // xs:import tag found, looking for schemaLocation attribute
            if ( strncmp( &buffer[i], "schemaLocation=", 15 ) == 0 ) 
                state = 2;

            outBuffer[j++] = buffer[i];
            break;

        case 2: // schemaLocation found, looking for path value (starting quote)
            if ( buffer[i] == '"' ) 
                state = 3;

            outBuffer[j++] = buffer[i];
            break;

        case 3: // schemaLocation value found, trim the path in this value
            // Only paths with an "up" component need to be trimmed.
            if ( strncmp( &buffer[i], "..", 2 ) == 0 ) {
                state = 4;
                savePoint = j;
            }
            else {
                state = 5;
            }

            outBuffer[j++] = buffer[i];
            break;

        case 4: // trimming path
            // Trim out the directory parts of the path, up to 
            // and including the XmlSchema part. Any subdirectory
            // after XmlSchema (e.g. GML) needs to be kept.
            if ( buffer[i] == '/' ) {
                // Trim out current path component by setting back the output 
                // buffer index.
                j = savePoint;

                // For these types if paths, we're guaranteed to hit "XmlSchema" at some point.
                if ( strncmp( &buffer[i], "/XmlSchema", 10 ) == 0 )
                    state = 7;
            }
            else {
                outBuffer[j++] = buffer[i];
                // Done trimming if closing quote reached
                if ( buffer[i] == '"' ) 
                    state = 6;
            }

            break;

        case 5: // in schemaLocation path value that doesn't need trimming, just look for ending quote.
            if ( buffer[i] == '"' ) 
                state = 6;

            outBuffer[j++] = buffer[i];
            break;

        case 6: // still in xs:import but past schemaLocation value, look for end of tag.
            if ( buffer[i] == '>' ) 
                state = 0;

            outBuffer[j++] = buffer[i];
            break;

        case 7: // in XmlSchema part of schemaLocation path value
            // When / is reach we're past the XmlSchema part, and 
            // need to stop trimming the path.
            if ( buffer[i] == '/' ) {
                // trim out XmlSchema part but keep the rest of the path.
                j = savePoint;
                state = 5;
            }

            break;
        }
    }

    outBuffer[j++] = 0;

    fprintf( outFp, outBuffer );
}

