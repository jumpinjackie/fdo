// 
//  
//  Copyright (C) 2006 Autodesk Inc.
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

class OGRLayer;


//Utility functions for conversion between FDO and OGR data
class OgrFdoUtil
{
public:


    static FdoClassDefinition* ConvertClass(OGRLayer* layer, FdoIdentifierCollection* requestedProps = NULL);
    static void ConvertFeature(FdoPropertyValueCollection* src, OGRFeature* dst, OGRLayer* layer);
    static void ApplyFilter(OGRLayer* layer, FdoFilter* filter);
    static int Fgf2Wkb(const unsigned char* fgf, unsigned char* wkb);
    static int Wkb2Fgf(const unsigned char* wkb, unsigned char* fgf);

};