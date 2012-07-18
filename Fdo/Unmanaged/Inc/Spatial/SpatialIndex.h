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
#ifndef FDOSPATIALINDEX_H
#define FDOSPATIALINDEX_H 1

#ifdef _WIN32
#pragma once
#endif //_WIN32

#include <Spatial/SpatialStd.h>
#include <FdoGeometry.h>

/// \ingroup (enums)
/// \brief
/// FdoSpatialIndexMode is an enumeration of the types of the Spatial Index,
/// representing the granularity.
enum FdoSpatialIndexMode
{
    /// Mode #1 - Regular Spatial Index: one entry in the SI for the entire geometry
    FdoSpatialIndex_ByGeometriesBoundingBox = 0,

    /// Mode #2 - The geometries are broken into segments and each segment has an entry in the SI.
    /// The segment index is contiguous over parts and subparts.
    FdoSpatialIndex_BySegmentsMultipleFeatures = 1,

    /// Mode #3 - Just one geometry is allowed. The geometry is broken into segments 
    /// and each segment has an entry in the SI. The part and subpart are encoded to allow 
    /// for sorting and fast processing.
    FdoSpatialIndex_BySegmentsSingleFeature = 2
};

/// \brief
/// Spatial Index class.
class FdoSpatialIndex : public FdoIDisposable
{
    friend class FdoSpatialIndexIterator;

public:
	/// \brief
	/// Creates a Spatial Index instance
	///
    /// \param mode 
    /// Input Type of the spatial index
	///
	/// \return
    /// Returns A spatial index instance
    /// 
    FDO_SPATIAL_API static FdoSpatialIndex* Create(FdoSpatialIndexMode mode = FdoSpatialIndex_ByGeometriesBoundingBox); 

	/// \brief
	/// Retrieves the Spatial index mode set at creation time
	///
	/// \return
    /// Returns the Spatial index mode set at creation
    /// 
    FDO_SPATIAL_API FdoSpatialIndexMode GetMode();

	/// \brief
	/// Inserts a feature into the spatial index given a bounding box
	///
	/// \remarks
    /// Applies only for Mode #1. The objectId is not encoded.
    ///
	/// \param objectId 
    /// Input The object identifier
	/// \param ext 
    /// Input A bounding box
	/// 
	/// \return
    /// Returns True if the insert was succesful and False otherwise.
    /// 
    FDO_SPATIAL_API void InsertObject(FdoInt64 objectId, FdoIEnvelope *ext);

	/// \brief
	/// Inserts a feature into the spatial index given a bounding box
	///
	/// \remarks
	/// Applies for Mode #2 and #3. The objectId is encoded as follows:
	/// Mode #2: 
    ///      32 bits (0-31)  - 1-based objectId # 
    ///      32 bits (32-63) - 1-based segment # (contiguous)
    ///
	/// Mode #3:
	///		 0 bits          - objectId # (will be ignored)
    ///      16 bits (0-15)  - part # (for multi-features) max. 65534
    ///      16 bits (16-31) - subpart # (e.g. polygons with interior rings) max 65534
    ///      32 bits (32-63) - 1-based segment # 
    ///
	/// Note in Mode #3 the insert may fail in case the encoding fails. The caller should
	/// prior check the number of parts and subparts against the 65534 limit. Mode #2 can be 
	/// instead in case of overflow.
	///
	/// \param objectId 
    /// Input The object identifier
	/// \param fgfArray 
    /// Input A geometry in FGF format
	/// 
	/// \return
    /// Returns Nothing
    /// 
    FDO_SPATIAL_API void InsertObject(FdoInt32 objectId, FdoByteArray* fgfArray);

	/// \brief
	/// Erases an entry in the spatial index given the object id and a bounding box
	///
	/// \remarks
    /// In Mode #2 removes all the entries related to the object identifier. 
    /// In Mode #3 the spatial index will be emptied.
	///
	/// \param featId 
    /// Input The object identifier. 
	/// \param ext 
    /// Input The bounding box of the object. It may be NULL in which case 
    /// the spatial index extent will be used.
	/// 
	/// \return
    /// Returns Nothing
    /// 
    FDO_SPATIAL_API void EraseObject(FdoInt64 featId, FdoIEnvelope *ext);

	/// \brief
	/// Returns the extent of the spatial index
	///
	/// \return
    /// Returns The extent of the spatial index
    /// 
    FDO_SPATIAL_API FdoIEnvelope* GetTotalExtent();

	/// \brief
	/// Decodes a marker returned by the Spatial index iterator.
	/// Applies only for Mode #2, BySegmentsMultipleFeatures.
	///
	/// \param marker
    /// Input A marker returned by the Spatial index iterator 
	/// \param objectId 
    /// Output The object identifier 
	/// \param iSegment 
    /// Output The index of the segment in the original geometry, 1-based
	///
	/// \return
    /// Returns Nothing
    /// 
    FDO_SPATIAL_API void DecodeMarker(FdoInt64 marker, FdoInt32 &objectId, FdoInt32 &iSegment);

	/// \brief
	/// Decodes a marker returned by the Spatial index iterator.
	/// Applies only for Mode #3, BySegmentsSingleFeature.
	///
	/// \param marker
    /// Input A marker returned by the Spatial index iterator 
	/// \param iPart 
	/// Output The index of the part in a multi-geometry, e.g. a polygon # in a multi-polygon 
	/// \param iSubPart
	/// Output The index of the subpart in a geometry, e.g. a ring # in a polygon 
	/// \param iVertex 
    /// Output The index of the segment in the sub-part, 1-based
	///
	/// \return
    /// Returns Nothing
    /// 
    FDO_SPATIAL_API void     DecodeMarker(FdoInt64 marker, FdoInt32 &iPart, FdoInt32 &iSubPart, FdoInt32 &iSegment);

/// \cond DOXYGEN-IGNORE
    virtual void Dispose();
/// \endcond
    
protected:
    FdoSpatialIndex(FdoSpatialIndexMode mode = FdoSpatialIndex_ByGeometriesBoundingBox); 

    virtual ~FdoSpatialIndex();

private:
    FdoInt64 encodeMarker(FdoInt32 featId, FdoInt32 nSegment);
    FdoInt64 encodeMarker(int nPart, int nSubPart, int nSegment);
    void     insertBySegmentsWithCurves(FdoInt32 featId, FdoByteArray* fgfArray);
	void     insertBySegmentsWithoutCurves(FdoInt32 featId, FdoByteArray* fgfArray);

private:
    FdoSpatialIndexMode     m_mode;
    FdoInt32                m_count;
    void *					m_si;   // implementation specific        
};

/// \brief
/// Spatial Index Iterator class
class FdoSpatialIndexIterator
{
public:

	/// \brief
	/// Creates a Spatial Index Iterator instance
	///
    /// \param si 
    /// Input A Spatial Index instance
	/// \param minx
	/// Input The minimim X of the bounding box representing the search area
	/// \param miny
	/// Input The minimim Y of the bounding box representing the search area
	/// \param maxx
	/// Input The maximum X of the bounding box representing the search area
	/// \param maxy
	/// Input The maximum Y of the bounding box representing the search area
	///
	/// \return
    /// Returns A Spatial Index Iterator instance
    /// 
    FDO_SPATIAL_API FdoSpatialIndexIterator(FdoSpatialIndex* si, double minx, double miny, double maxx, double maxy);

	/// \brief
	/// Iterator over the features selected in the the search area
	///
	/// \return
    /// Returns an encoded marker. Zero (0) return value stands for the end of fetch
    /// 
    FDO_SPATIAL_API FdoInt64 GetNextObject();

	FDO_SPATIAL_API ~FdoSpatialIndexIterator();

private:
    FdoPtr<FdoSpatialIndex> m_si;
    void *					m_siter; // implementation specific
};


#endif // FdoSpatialIndex_H
