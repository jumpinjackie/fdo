#ifndef _GDBIQUERYRESULT_
#define _GDBIQUERYRESULT_ 1
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
#ifdef _WIN32
#pragma once
#endif

#include <map>
#include <string>

class GdbiCommands;
#include "GdbiTypes.h"

#define GDBI_COLUMN_SIZE   255

typedef struct _col_type_ {
    int     original_type;
    int     type;
    int     size;
    int     index;
    void    *value;
	int		null_allowed;
    GDBI_NI_TYPE *isNull;
} GdbiColumnInfoType;

class GdbiQueryResult
{
    friend class GdbiConnection;
    friend class GdbiStatement;

private:
    GdbiCommands*     m_pGdbiCommands;
    int               mQueryId;
	int				  mArrayTCount; // total number of fetched rows
	int				  mArrayCCount; // number of fetched rows in a sngle fetch;
	int				  mArrayPos; // position within array of fetched rows
    std::map<std::wstring,GdbiColumnInfoType*> *mColMap;
    wchar_t           *mUnicodeBuffer;   // Used to return wchar string that should be copied
    int               mUnicodeBufferSize;
    char              *mAsciiValBuffer;
    int               mAsciiValBufferSize;
	bool			  mOwnsQueryId;

private:
    GdbiQueryResult( GdbiCommands* command, int qid, bool ownsQid = true  );
	void do_copy(char *ascii_I, char *ascii_O, int len, int *ccode);

   int GetAsciiValue(
        const wchar_t *ColName,
        int length,
        char *ascii,
        bool *isnull,
        int *ccode
        );

   template<typename T> T GetNumber(
        const wchar_t *ColName,
        bool *isnull,
        int *ccode
        );

public:
    ~GdbiQueryResult(void);

    int GetBinaryValue(
            const wchar_t *ColName,
            int length,
            char *address,
            bool *null_ind,
            int *ccode
            );

 

    FdoDouble GetDouble( const char *ColName, bool *isnull, int *ccode );

    FdoInt32 GetInt32( const char *ColName, bool *isnull, int *ccode );

    FdoInt16 GetInt16( const char *ColName, bool *isnull, int *ccode );

	FdoInt8 GetInt8( const char *ColName, bool *isnull, int *ccode );

    FdoInt64 GetInt64( const char *ColName, bool *isnull, int *ccode );

    FdoFloat GetFloat( const char *ColName, bool *isnull, int *ccode );

    FdoString* GetString( const char *ColName, bool *isnull, int *ccode );

    FdoBoolean GetBoolean( const char *ColName, bool *isnull, int *ccode );

    bool GetIsNull( const char *ColName );

	FdoDouble GetDouble( const wchar_t *ColName, bool *isnull, int *ccode );

    FdoInt32 GetInt32( const wchar_t *ColName, bool *isnull, int *ccode );

    FdoInt16 GetInt16( const wchar_t *ColName, bool *isnull, int *ccode );

	FdoInt8 GetInt8( const wchar_t *ColName, bool *isnull, int *ccode );

    FdoInt64 GetInt64( const wchar_t *ColName, bool *isnull, int *ccode );

    FdoFloat GetFloat( const wchar_t *ColName, bool *isnull, int *ccode );

    FdoString* GetString( const wchar_t *ColName, bool *isnull, int *ccode );

    FdoBoolean GetBoolean( const wchar_t *ColName, bool *isnull, int *ccode );

	FdoString* GetString( int index, bool *isnull, int *ccode );

    bool GetIsNull( const wchar_t *ColName );

    int ReadNext();

    void Close();

	void End(); // end selection before reaching end-of-fetch

    int GetColumnCount( );

    int GetColumnDesc( int colIdx, GdbiColumnDesc &desc );
};

#endif

