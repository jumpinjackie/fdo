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

#pragma once
//#include <windows.h>
#include <map>
#include <string>

class ProjConverter
{
    private:
        std::map<std::string, std::string> m_translations;
        bool GetLine(std::ifstream& infile, char* dest, std::streamsize buffersize);

    public:
        ProjConverter();
        virtual ~ProjConverter();
        const wchar_t* TranslateProjection(const wchar_t* proj);
        static ProjConverter* ProjectionConverter;

};
