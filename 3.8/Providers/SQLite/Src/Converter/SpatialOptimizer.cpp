// 
//  
//  Copyright (C) 2008 Autodesk Inc.
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
#include "slt.h"
#include "SltGeomUtils.h"
#include "SpatialOptimizer.h"

//Morton number -- a.k.a bit interleaving
//http://graphics.stanford.edu/~seander/bithacks.html#InterleaveTableObvious
unsigned int InterleaveBits(unsigned short xin, unsigned short yin)
{
    static const unsigned int B[] = {0x55555555, 0x33333333, 0x0F0F0F0F, 0x00FF00FF};
    static const unsigned int S[] = {1, 2, 4, 8};

    // Interleave lower 16 bits of x and y, so the bits of x
    // are in the even positions and bits from y in the odd;
    // z gets the resulting 32-bit Morton Number.
    unsigned int x = xin;
    unsigned int y = yin;
    unsigned int z;

    x = (x | (x << S[3])) & B[3];
    x = (x | (x << S[2])) & B[2];
    x = (x | (x << S[1])) & B[1];
    x = (x | (x << S[0])) & B[0];

    y = (y | (y << S[3])) & B[3];
    y = (y | (y << S[2])) & B[2];
    y = (y | (y << S[1])) & B[1];
    y = (y | (y << S[0])) & B[0];

    z = x | (y << 1);
    return z;
}


std::vector<FeatRec>* ComputeMortonOrder(FdoIConnection* con, FdoFeatureClass* fc)
{
    FdoPtr<FdoDataPropertyDefinitionCollection> idpdc = fc->GetIdentityProperties();
    FdoPtr<FdoGeometricPropertyDefinition> gpd = fc->GetGeometryProperty();
    FdoPtr<FdoDataPropertyDefinition> idp = idpdc->GetItem(0);

    std::wstring gname = gpd->GetName();
    std::wstring idname = idp->GetName();

    //first, get the overall extents
    FdoPtr<FdoISelectAggregates> sa = (FdoISelectAggregates*)con->CreateCommand(FdoCommandType_SelectAggregates);

    sa->SetFeatureClassName(fc->GetName());
    
    
    FdoPtr<FdoIdentifierCollection> ids = sa->GetPropertyNames();
    std::wstring filter = L"SpatialExtents(";
    filter += gname;
    filter += L")";
    FdoPtr<FdoExpression> expr = FdoExpression::Parse(filter.c_str());
    FdoPtr<FdoComputedIdentifier> cid = FdoComputedIdentifier::Create(L"MBR", expr);
    ids->Add(cid);
    FdoPtr<FdoIDataReader> rdr = sa->Execute();

    rdr->ReadNext();
    FdoPtr<FdoByteArray> geomBytes = rdr->GetGeometry(L"MBR");
    double text[4];
    GetFgfExtents(geomBytes->GetData(), geomBytes->GetCount(), text);
    rdr->Close();


    //ok done... now, map that to an integer square extent
    //we will use a 32 bit integer for the Morton number (16 bit in X and 16 in Y)

    double width = text[2] - text[0];
    double height = text[3] - text[1];

    double scaleX = (double)(65535) / width;
    double scaleY = (double)(65535) / height;

    
    //now iterate over the features and compute a Morton number for each one
    std::vector<FeatRec>* recs = new std::vector<FeatRec>;


    FdoPtr<FdoISelect> sel = (FdoISelect*)con->CreateCommand(FdoCommandType_Select);
    sel->SetFeatureClassName(fc->GetName());
    FdoPtr<FdoIFeatureReader> frdr = sel->Execute();

    while (frdr->ReadNext())
    {
        //TODO: for now assume int32 id property...
        int id = frdr->GetInt32(idname.c_str());
        int len;
        const unsigned char* geomblob = frdr->GetGeometry(gname.c_str(), &len);

        double ext[4];
        GetFgfExtents(geomblob, len, ext);

        double cx = 0.5*(ext[0] + ext[2]);
        double cy = 0.5*(ext[1] + ext[3]);

        unsigned short ix = (unsigned short)((cx - text[0]) * scaleX);
        unsigned short iy = (unsigned short)((cy - text[1]) * scaleY);
        unsigned int m = InterleaveBits(ix, iy);

        FeatRec fr;
        fr.id = id;
        fr.mortonid = m;
        recs->push_back(fr);        
    }

    frdr->Close();

    std::sort(recs->begin(), recs->end());
    return recs;
}
