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

#include <Spatial/SpatialIndex.h>
#include <Spatial/SpatialUtility.h>
#include "SpatialUtilityGeometryExtents.h"
#include "rtree.h"

FdoSpatialIndex* FdoSpatialIndex::Create(FdoSpatialIndexMode mode)
{
    return new FdoSpatialIndex(mode);
}

FdoSpatialIndex::FdoSpatialIndex(FdoSpatialIndexMode mode)
{
    m_si = new fdo::rtree();
    m_count = 0;
    m_mode = mode;
}

FdoSpatialIndex::~FdoSpatialIndex()
{
    delete m_si;
}

void FdoSpatialIndex::Dispose()
{
    delete this;
}

FdoSpatialIndexMode FdoSpatialIndex::GetMode()
{
    return m_mode;
}

// FdoSpatialIndex_ByGeometriesBoundingBox
void FdoSpatialIndex::InsertObject(FdoInt64 objectId, FdoIEnvelope *ext)
{
	if (ext == NULL)
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADPARAMETER)));

	if (m_mode != FdoSpatialIndex_ByGeometriesBoundingBox)
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_188_SI_BAD_PARAMETER_FOR_MODE)));

    fdo::dbox bbox;

    bbox.min[0] = ext->GetMinX();
    bbox.min[1] = ext->GetMinY();
    bbox.max[0] = ext->GetMaxX();
    bbox.max[1] = ext->GetMaxY();

	((fdo::rtree*)m_si)->insert((fdo::fid_t)objectId, bbox);
    m_count++; 
}


void FdoSpatialIndex::InsertObject(FdoInt32 objectId, FdoByteArray* fgfArray)
{
	if (fgfArray == NULL)
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADPARAMETER)));

	if (objectId <= 0 && m_mode != FdoSpatialIndex_BySegmentsSingleFeature)
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADPARAMETER)));

    if (m_mode == FdoSpatialIndex_ByGeometriesBoundingBox)
    {
		fdo::dbox bbox;
		double zmin, zmax;
		FdoSpatialUtilityGeometryExtents::GetExtents(fgfArray, bbox.min[0], bbox.min[1], zmin, bbox.max[0], bbox.max[1], zmax);
		
		((fdo::rtree*)m_si)->insert(objectId, bbox);
    }
	else if (m_mode == FdoSpatialIndex_BySegmentsSingleFeature && m_count > 0)
    {
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_189_SI_TOO_MANY_FEATURES_FOR_MODE)));
    }
    else //FdoSpatialIndex_BySegmentsSingleFeature or FdoSpatialIndex_BySegmentsMultipleFeatures
    {	
		unsigned char* fgf = fgfArray->GetData();
		int* ireader = (int*)fgf;
	    
		// the geometry type
		int geom_type = (FdoGeometryType) (*ireader++);

		if (geom_type == FdoGeometryType_CurveString ||
			geom_type == FdoGeometryType_MultiCurveString ||
			geom_type == FdoGeometryType_CurvePolygon ||
			geom_type == FdoGeometryType_MultiCurvePolygon)
		{
			insertBySegmentsWithCurves(objectId, fgfArray);
		} 
		else
		{
			insertBySegmentsWithoutCurves(objectId, fgfArray);
		}
    }
	m_count++; 		
}

// FdoSpatialIndex_ByGeometriesBoundingBox
void FdoSpatialIndex::EraseObject(FdoInt64 objectId, FdoIEnvelope *ext)
{
    fdo::dbox bbox;

	if (ext)
	{
		bbox.min[0] = ext->GetMinX();
		bbox.min[1] = ext->GetMinY();
		bbox.max[0] = ext->GetMaxX();
		bbox.max[1] = ext->GetMaxY();
	}
	else
	{
		FdoPtr<FdoIEnvelope> env = GetTotalExtent();

		bbox.min[0] = env->GetMinX();
		bbox.min[1] = env->GetMinY();
		bbox.max[0] = env->GetMaxX();
		bbox.max[1] = env->GetMaxY();
	}

	if (m_mode == FdoSpatialIndex_ByGeometriesBoundingBox)
	{
		// Remove the unique entry if any
		if (((fdo::rtree*)m_si)->erase((fdo::fid_t)objectId, bbox))
			m_count--;
	}
	else if (m_mode == FdoSpatialIndex_BySegmentsMultipleFeatures)
	{
		// Remove all the entries related to this object identifier. Run a query.
		FdoSpatialIndexIterator iter(this, bbox.min[0], bbox.min[1], bbox.max[0], bbox.max[1]);
		
		FdoInt64 marker = 0;
		while (marker = iter.GetNextObject())
		{
			FdoInt32 objId;
			FdoInt32 dummy;
			DecodeMarker(marker, objId, dummy);
		
			if (objId == objectId)
			{
				if (((fdo::rtree*)m_si)->erase((fdo::fid_t)marker, bbox))
					m_count--;
			}	
		}
	}
	else // FdoSpatialIndex_BySegmentsSingleFeature
	{
		// Just remove and recreate the rtree.
		delete m_si;
		m_si = new fdo::rtree();
		m_count = 0;
	}
}

// Cloned from FdoSpatialUtilityGeometryExtents::getExtentsWithoutCurve()
void FdoSpatialIndex::insertBySegmentsWithoutCurves(FdoInt32 objectId, FdoByteArray* fgfArray)
{
    unsigned char* fgf = fgfArray->GetData();
    int* ireader = (int*)fgf;
    
    // the geometry type
    int geom_type = (FdoGeometryType) (*ireader++);

    FdoDimensionality geom_dim;
    bool is_multi = (geom_type == FdoGeometryType_MultiLineString)
        || (geom_type == FdoGeometryType_MultiPolygon
        || (geom_type == FdoGeometryType_MultiPoint));

    //temp variables used inside the loop
    int num_geoms = 1;
    int point_count;
    int contour_count;
    double* dreader;

    double x0, y0, x1, y1;
	int segNo = 1;  // 1-based
    fdo::dbox bbox;

    //in case of multipolygon or multilinestring or multipoint, 
    //read poly or linestring count
    if (is_multi) num_geoms = (*ireader++);

    for (int nPart = 0; nPart < num_geoms; nPart++)
    {
        //skip past geometry type of subgeometry
        //we know it is LineString or Polygon or Point respectively
        if (is_multi) ireader++; 

        //read cordinate typeB
        FdoDimensionality sub_geom_dim = (FdoDimensionality) (*ireader++);

        geom_dim = sub_geom_dim;
 
		bool hasZ = ((geom_dim & FdoDimensionality_Z) != 0);
		bool hasM = ((geom_dim & FdoDimensionality_M) != 0);

        // the number of contours in current polygon/linestring
        contour_count = 1; //for linestrings

        switch (geom_type)
        {
            case FdoGeometryType_Polygon:
            case FdoGeometryType_MultiPolygon:
                contour_count = (*ireader++);
            default: break;
        }

        for (int i = 0; i < contour_count; i++)
        {
            point_count = 1;

            //point geoms do not have a point count, since 
            //each piece is just one point each
			bool isPoint = true;
            if ((geom_type != FdoGeometryType_MultiPoint) 
                && (geom_type != FdoGeometryType_Point))
            {
                point_count = (*ireader++);
				isPoint = false;
            }

            int ords_per_position = 2 /*X,Y*/ + (hasZ?1:0)/*Z*/ + (hasM?1:0)/*M*/;

            //*** ireader not valid from here down
            dreader = (double*) ireader;
           
            for (int k = 0; k < point_count; k++)
            {
                x1 = *dreader++;
                y1 = *dreader++;

				//skip past z and m
                if (hasZ)
                    dreader++;
                if (hasM)
                    dreader++;

				// Swap and read the next point unless it is a point
				if (k == 0)
				{			
					x0 = x1;
					y0 = y1; 

					if (!isPoint) continue;
				}

				// Encode
				FdoInt64 marker = 0;
				if (m_mode == FdoSpatialIndex_BySegmentsMultipleFeatures)
				{
					marker = encodeMarker(objectId, segNo); 
					segNo++;
				}
				else if (m_mode == FdoSpatialIndex_BySegmentsSingleFeature)
				{
					marker = encodeMarker(nPart, i, (isPoint)? k + 1 : k);
				}

				// Insert into Rtree 
				bbox.min[0] = x0 < x1 ? x0 : x1;
				bbox.min[1] = y0 < y1 ? y0 : y1;
				bbox.max[0] = x0 > x1 ? x0 : x1;
				bbox.max[1] = y0 > y1 ? y0 : y1;

				((fdo::rtree*)m_si)->insert((fdo::fid_t)marker, bbox);

				// Swap
				x0 = x1;
				y0 = y1;                
            }

            ireader = (int*)dreader;
            //*** ireader valid again
        }
    }       
}

// Cloned from FdoSpatialUtilityGeometryExtents::getExtentsWithoutCurve()
void FdoSpatialIndex::insertBySegmentsWithCurves(FdoInt32 objectId, FdoByteArray* fgfArray)
{
    unsigned char* fgf = fgfArray->GetData();
    int* ireader = (int*)fgf;
    
    // the geometry type
    int geom_type = (FdoGeometryType) (*ireader++);

    FdoDimensionality geom_dim;
    bool is_multi = (geom_type == FdoGeometryType_MultiCurveString)
        || (geom_type == FdoGeometryType_MultiCurvePolygon);
 
    //temp variables used inside the loop
    int num_geoms = 1;
    int point_count;
    int contour_count;
    double* dreader;

    double x, y, x1, y1;
	int segNo = 1;  // 1-based
    fdo::dbox bbox;

    //in case of multicurvepolygon or multicurvelinestring  
    //read poly or linestring count
    if (is_multi) num_geoms = (*ireader++);

    for (int nPart = 0; nPart < num_geoms; nPart++)
    {
        //skip past geometry type of subgeometry
        //we know it is LineString or Polygon or Point respectively
        if (is_multi) ireader++; 

        //read cordinate typeB
        FdoDimensionality sub_geom_dim = (FdoDimensionality) (*ireader++);

        geom_dim = sub_geom_dim;
 
		bool hasZ = ((geom_dim & FdoDimensionality_Z) != 0);
		bool hasM = ((geom_dim & FdoDimensionality_M) != 0);

        // the number of contours in current polygon/linestring
        contour_count = 1; //for linestrings, no rings, just one

        switch (geom_type)
        {
            case FdoGeometryType_CurvePolygon:
            case FdoGeometryType_MultiCurvePolygon:
                contour_count = (*ireader++);
            default: break;
        }

        for (int i = 0; i < contour_count; i++)
        {
            point_count = 1;

            int ords_per_position = 2 /*X,Y*/ + (hasZ?1:0)/*Z*/ + (hasM?1:0)/*M*/;

            //*** ireader not valid from here down
            dreader = (double*) ireader;

            x = *dreader++;
            y = *dreader++;
			if (hasZ)dreader++;
			if (hasM)dreader++;

            // ireader valid again
            ireader = (int*)dreader;
            int seg_count = *ireader++; //# curve segments

            for (int j = 0; j < seg_count; j++)
            {
                int seg_type = *ireader++;

                switch (seg_type)
                {
                case FdoGeometryComponentType_CircularArcSegment:
                    {
                        dreader = (double*)ireader;
 
                        //circular arc : read midpont and endpoint
                        //first point was either the start point or
                        //the last point if the previous segment

                        double midx = *dreader++;
                        double midy = *dreader++;
 						if (hasZ)dreader++;
						if (hasM)dreader++;

                        double endx = *dreader++;
                        double endy = *dreader++; 
  						if (hasZ)dreader++;
						if (hasM)dreader++;

						// Use the public API to compute the extents
						FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();

						FdoPtr<FdoIDirectPosition> startPos = gf->CreatePositionXY(x, y);
						FdoPtr<FdoIDirectPosition> midPos = gf->CreatePositionXY(midx, midy);
						FdoPtr<FdoIDirectPosition> endPos = gf->CreatePositionXY(endx, endy);

						FdoPtr<FdoICircularArcSegment> arcSeg = gf->CreateCircularArcSegment(startPos, midPos, endPos);

						FdoPtr<FdoCurveSegmentCollection> curveSegs = FdoCurveSegmentCollection::Create();
						curveSegs->Add(arcSeg);

						FdoPtr<FdoICurveString> curveString = gf->CreateCurveString(curveSegs);
						FdoPtr<FdoByteArray>  arc = gf->GetFgf(curveString);

                        double minX, minY, minZ, maxX, maxY, maxZ;
						FdoSpatialUtilityGeometryExtents::GetExtents(arc, minX, minY, minZ, maxX, maxY, maxZ);
    
						bbox.min[0] = minX;
						bbox.min[1] = minY;
						bbox.max[0] = maxX;
						bbox.max[1] = maxY;

						// Encode
						FdoInt64 marker = 0;
						if (m_mode == FdoSpatialIndex_BySegmentsMultipleFeatures)
						{
							marker = encodeMarker(objectId, segNo); 
							segNo++;
						}
						else if (m_mode == FdoSpatialIndex_BySegmentsSingleFeature)
						{
							marker = encodeMarker(nPart, i, segNo); 
						}
						segNo++;

						// Insert into Rtree 
						((fdo::rtree*)m_si)->insert((fdo::fid_t)marker, bbox);

						// Swap 
                        x = endx;
                        y = endy;

                        ireader = (int*) dreader;
                        break;
                    }
                case FdoGeometryComponentType_LineStringSegment:
                    {
                        int point_count = *ireader++;

                        dreader = (double*)ireader;

						for (int k = 0; k < point_count; k++)
						{
							x1 = *dreader++;
							y1 = *dreader++;

							//skip past z and m
							if (hasZ)dreader++;
							if (hasM)dreader++;

							bbox.min[0] = x < x1 ? x : x1;
							bbox.min[1] = y < y1 ? y : y1;
							bbox.max[0] = x > x1 ? x : x1;
							bbox.max[1] = y > y1 ? y : y1;

							// Encode
							FdoInt64 marker = 0;
							if (m_mode == FdoSpatialIndex_BySegmentsMultipleFeatures)
							{
								marker = encodeMarker(objectId, segNo); 								
							}
							else if (m_mode == FdoSpatialIndex_BySegmentsSingleFeature)
							{
								marker = encodeMarker(nPart, i, segNo);
							}
							segNo++;

							// Insert into Rtree 
							((fdo::rtree*)m_si)->insert((fdo::fid_t)marker, bbox);

							// Swap
							x = x1;
							y = y1;                
						}

                        ireader = (int*) dreader;
                        break;
                    }
                } // switch
            } //seg count
        } //ring count of polygons
    }// geom count of multi
}

FdoIEnvelope* FdoSpatialIndex::GetTotalExtent()
{
    fdo::dbox bbox;

    ((fdo::rtree*)m_si)->get_total_extent(bbox);

    FdoPtr<FdoFgfGeometryFactory>  gf = FdoFgfGeometryFactory::GetInstance();
    FdoPtr<FdoIEnvelope> env = gf->CreateEnvelopeXY(bbox.minx, bbox.miny, bbox.maxx, bbox.maxy);  

    return FDO_SAFE_ADDREF(env.p);
}

FdoInt64 FdoSpatialIndex::encodeMarker(FdoInt32 objectId, FdoInt32 segNo)
{
    // Encoding
    // 32 bits (0-31)  - objectId # 
    // 32 bits (32-63) - vertex # (contiguous )

    FdoInt64   marker = objectId;
    marker = (marker << 32);
    marker += (segNo + 1); // 1-based

    return marker;
}

FdoInt64 FdoSpatialIndex::encodeMarker(int nPart, int nSubPart, int nSegment /* 1- based */)
{
    // Encoding:
    // 16 bits (0-15)  - part # (for multipolygons) max. 65534
    // 16 bits (16-31) - ring # (polygons with interior rings) max 65534
    // 32 bits (32-63) - vertex # 

	if (nPart > SHRT_MAX)
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_191_SI_NUMBER_PARTS_ERROR)));
	else if (nSubPart > SHRT_MAX)
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_192_SI_NUMBER_SUBPARTS_ERROR)));

    FdoInt64   marker = (nPart << 16);
    marker += nSubPart;
    marker = (marker << 32);
    marker += nSegment; 

    return marker;
}

void FdoSpatialIndex::DecodeMarker(FdoInt64 marker, FdoInt32 &iPart, FdoInt32 &iSubPart, FdoInt32 &iSegment)
{
    if (m_mode == FdoSpatialIndex_BySegmentsSingleFeature)
    {
        iSegment = (int) (marker);
        //iVertex--; // back to 0-based, see Encode()
        iSubPart = (int) ((marker & 0x00FF0000) >> 32);
        iPart = (int) (marker >> 48);
    }
    else
    {
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_190_SI_BAD_DECODE_METHOD_FOR_MODE)));
    }
}

void FdoSpatialIndex::DecodeMarker(FdoInt64 marker, FdoInt32 &objectId, FdoInt32 &iSegment)
{
    bool bFound = false;
    objectId = -1;
 
    if (m_mode == FdoSpatialIndex_ByGeometriesBoundingBox || 
		m_mode == FdoSpatialIndex_BySegmentsSingleFeature)
    {
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_190_SI_BAD_DECODE_METHOD_FOR_MODE)));
    }
    else if (m_mode == FdoSpatialIndex_BySegmentsMultipleFeatures)
    {
        objectId = (FdoInt32)(marker >> 32);
        iSegment = (FdoInt32)marker;
    }
}

//////////////////////////////////////////////////////
// FdoSpatialIndexIterator  class
//////////////////////////////////////////////////////
FdoSpatialIndexIterator::FdoSpatialIndexIterator(FdoSpatialIndex* si, double minx, double miny, double maxx, double maxy)
{
    m_si = FDO_SAFE_ADDREF(si);

    double      tol = 0.0000001; // small fudge factor
    fdo::dbox   bbox;

    bbox.min[0] = minx - tol;
    bbox.min[1] = miny - tol;
    bbox.max[0] = maxx + tol;
    bbox.max[1] = maxy + tol;

	m_siter = (fdo::rtree_iterator*)_aligned_malloc(sizeof(fdo::rtree_iterator), 16);
	((fdo::rtree_iterator*)m_siter)->initialize((fdo::rtree *)si->m_si, bbox);
}

FdoSpatialIndexIterator::~FdoSpatialIndexIterator()
{
	((fdo::rtree_iterator*)m_siter)->clear();
	_aligned_free(m_siter);
}

FdoInt64 FdoSpatialIndexIterator::GetNextObject()
{
    return((fdo::rtree_iterator*)m_siter)->next();
}

