// 
//  
//  Copyright (C) 2007 Autodesk Inc.
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

#include "stdafx.h"
#include "ProjConverter.h"
#include <iostream>
#include <fstream>

using namespace std;

const wchar_t* ProjConverter::TranslateProjection(const wchar_t* proj)
{
    //Quick return if no translations are installed
    if (m_translations.size() == 0)
        return proj;

    W2A(proj);
    string s(mbproj);

    std::map<std::string, std::string>::iterator res = m_translations.find(s);
    if (res != m_translations.end())
    {
#if DEBUG
        printf("Converted projection from %s to %s\"", mbproj, res->second.c_str());
#endif
        const char* y = res->second.c_str();
        A2W(y);
        proj = wy;
    }

    return proj;
}

bool ProjConverter::GetLine(ifstream& infile, char* dest, std::streamsize buffersize)
{
    while(infile.good())
    {
        infile.getline(dest, buffersize);

        int p = 0;
        char c = dest[p];

        while ((c == ' ' || c == '\t') && p < buffersize)
            c = dest[p++];

        //Filter blanks and comments
        if (dest[p] == '#' || dest[p] == '\n' || dest[p] == '\r')
            continue;

        return true;
    }
    return false;
}

ProjConverter::ProjConverter()
{
    ifstream infile;
    try
    {
        char fromline[8096];
        char toline[8096];

        infile.open ("projections.txt", ifstream::in);
        do
        {
            if (GetLine(infile, fromline, 8096) && GetLine(infile, toline, 8096))
                m_translations.insert(make_pair(fromline, toline));
            else
                break;
                
        } while(true);
    }
    catch (...)
    {
    }
#if DEBUG
    printf("OGR Provider loaded %d projections\n",  m_translations.size());
#endif
}

ProjConverter::~ProjConverter()
{
}

ProjConverter* ProjConverter::ProjectionConverter = NULL;
