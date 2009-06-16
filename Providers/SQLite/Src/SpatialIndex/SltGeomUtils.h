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

#ifndef SLGEOMUTILS_H
#define SLGEOMUTILS_H

#include <float.h>
#include <memory.h>

//Higly recommended to use 2D float bounding boxes
//for the spatial index.
#define SI_DIM 2
#define USE_FLOAT_BOUNDS 1

//Enabling SSE makes a huge difference when building
//the spatial index and also significantly speeds up
//spatial index searches, especially when data is not
//spatially coherent and the index is therefore inefficient.
#ifdef _MSC_VER
#define ENABLE_SSE 1
#endif

#if ENABLE_SSE
#ifndef _MSC_VER
#if defined(__ICC)
#include <emmintrin.h>
#else
#include <xmmintrin.h>
#endif
#else
#include <xmmintrin.h>
#include <emmintrin.h>
#endif
#endif

#ifdef _MSC_VER
  #define ALGNW __declspec(align(16))
  #define ALGNL
#else
  #define ALGNW
  #define ALGNL __attribute__((aligned(16)))
#endif


#if USE_FLOAT_BOUNDS
typedef float Real;
#define REAL_MAX FLT_MAX;
#else
typedef double Real;
#define REAL_MAX DBL_MAX;
#endif

struct DBounds
{
    double min[SI_DIM];
    double max[SI_DIM];

    DBounds()
    {
        SetEmpty();
    }

    bool IsEmpty() const
    {
        return min[0] > max[0];
    }

    void SetEmpty()
    {
        for (int i=0; i<SI_DIM; i++)
        {
            min[i] = DBL_MAX;
            max[i] = -DBL_MAX;
        }
    }

    static void Union(DBounds* res, DBounds* b1, DBounds* b2)
    {
        //Union returns an empty box when one of the
        //arguments is empty -- this may be counterintuitive
        //but in this context empty means that it encompasses
        //the whole world (i.e. we will be forcing a table scan)
        if (b1->IsEmpty())
        {
            *res = *b1;
            return;
        }

        if (b2->IsEmpty())
        {
            *res = *b2;
            return;
        }

        for (int i=0; i<SI_DIM; i++)
        {
            res->min[i] = std::min<double>(b1->min[i], b2->min[i]);
            res->max[i] = std::max<double>(b1->max[i], b2->max[i]);
        }
    }

    static void Intersection(DBounds* res, DBounds* b1, DBounds* b2)
    {
        //Intersection returns the other argument box when one of the
        //arguments is empty -- this may be counterintuitive
        //but in this context empty means that it encompasses
        //the whole world (i.e. we will be forcing a table scan)
        //With intersection we want to return the intersection of
        //the whole world with a given box which means we have to return
        //the given box. See also similar note in Union()
        if (b1->IsEmpty())
        {
            *res = *b2;
            return;
        }

        if (b2->IsEmpty())
        {
            *res = *b1;
            return;
        }

        for (int i=0; i<SI_DIM; i++)
        {
            res->min[i] = std::max<double>(b1->min[i], b2->min[i]);
            res->max[i] = std::min<double>(b1->max[i], b2->max[i]);
        }
    }

    bool Contains(const DBounds& b)
    {
        for (int i=0; i<SI_DIM; i++)
            if (min[i] > b.min[i])
                return false;

        for (int i=0; i<SI_DIM; i++)
            if (max[i] < b.max[i])
                return false;
       
        return true; 
    }
};

ALGNW struct Bounds
{
    Real min[SI_DIM];
    Real max[SI_DIM];

    Bounds(bool empty)
    {
        for (int i=0; i<SI_DIM; i++)
        {
            min[i] = REAL_MAX;
            max[i] = -REAL_MAX;
        }
    }

    Bounds()
    {
    }
    
    bool IsEmpty() const
    {
        return min[0] > max[0];
    }

    static inline void Add(Bounds* __restrict dst, const Bounds* __restrict src)
    {
#if ENABLE_SSE && (SI_DIM == 2) && USE_FLOAT_BOUNDS
        __m128 S = _mm_load_ps((float*)src);    //S = smaxy smaxx sminy sminx
        __m128 D = _mm_load_ps((float*)dst);    //D = dmaxy dmaxx dminy dminx
        __m128 tmp = D;
        D = _mm_min_ps(D, S);                   //D = min values
        S = _mm_max_ps(S, tmp);                 //S = max values
        D = _mm_shuffle_ps(D, S, 0xE4);         //pack the results (min from D, max from S);
        _mm_store_ps((float*)dst, D);           //store the result
#else
        for (int i=0; i<SI_DIM; i++)
        {
            if (dst->min[i] > src->min[i])
                dst->min[i] = src->min[i];
            if (dst->max[i] < src->max[i])
                dst->max[i] = src->max[i];
        }
#endif
    }

    static inline int Disjoint(const Bounds* __restrict r, const Bounds* __restrict s)
    {
#if ENABLE_SSE && (SI_DIM == 2) && USE_FLOAT_BOUNDS
        __m128 R = _mm_load_ps((float*)r);      //R = rmaxy rmaxx rminy rminx
        __m128 S = _mm_load_ps((float*)s);      //S = smaxy smaxx sminy sminx
        __m128 tmpR = R;
        tmpR = _mm_shuffle_ps(tmpR, S, 0x44);   //tmpR = sminy sminx rminy rminx
        S = _mm_shuffle_ps(S, R, 0xEE);         //S = rmaxy rmaxx smaxy smaxx
        S = _mm_cmplt_ps(S, tmpR);              //S < tmpR?
        return _mm_movemask_ps(S);
#else
        for (int i=0; i<SI_DIM; i++)
        {
            if (r->min[i] > s->max[i])
                return 1;
            if (r->max[i] < s->min[i])
                return 1;
        }

        return 0;
#endif
    }


    static inline int NotContain(const Bounds* __restrict r, const Bounds* __restrict s)
    {
        //assumes overlap has been checked prevously -- otherwise could give wrong result

#if ENABLE_SSE && (SI_DIM == 2) && USE_FLOAT_BOUNDS
        __m128 R = _mm_load_ps((float*)r);      // R = rmaxy rmaxx rminy rminx
        __m128 S = _mm_load_ps((float*)s);      // S = smaxy smaxx sminy sminx
        __m128 tmpR = R;
        tmpR = _mm_shuffle_ps(tmpR, S, 0xE4);   //tmpR = smaxy smaxx rminy rminx
        S = _mm_shuffle_ps(S, R, 0xE4);         //S = rmaxy rmaxx sminy sminx
        S = _mm_cmplt_ps(S, tmpR);              //S < tmpR?
        return _mm_movemask_ps(S);
#else
        for (int i=0; i<SI_DIM; i++)
        {
            if (r->min[i] > s->min[i])
                return 1;
            if (r->max[i] < s->max[i])
                return 1;
        }

        return 0;
#endif
    }
} ALGNL;


static __forceinline void FillMem(Bounds* __restrict dst, const Bounds* __restrict src, int count)
{
#if ENABLE_SSE && (SI_DIM == 2) && USE_FLOAT_BOUNDS
    __m128 S = _mm_loadu_ps((float*)src);
    
    //SSE memset using Duff's device... and non-cache polluting store instructions.
    int n = (count + 7) / 8;
    switch (count % 8) 
    {
    case 0: do { _mm_stream_ps((float*)dst, S);dst++;
    case 7:      _mm_stream_ps((float*)dst, S);dst++;
    case 6:      _mm_stream_ps((float*)dst, S);dst++;
    case 5:      _mm_stream_ps((float*)dst, S);dst++;
    case 4:      _mm_stream_ps((float*)dst, S);dst++;
    case 3:      _mm_stream_ps((float*)dst, S);dst++;
    case 2:      _mm_stream_ps((float*)dst, S);dst++;
    case 1:      _mm_stream_ps((float*)dst, S);dst++;
               } while (--n > 0);
    }
#else
    Bounds* dptr = dst;
    for (int i=0; i<count; i++)
    {
        *dptr++ = *src;
    }
#endif
}

//finds the bounding box of a given array of points
//and stores minx, miny, maxx and maxy in ext
//Does not clobber input ext, just adds to it.
static __forceinline void AddToExtent(int npts, int dim, double* __restrict pts, double* __restrict ext)
{
#if ENABLE_SSE
    //unfortunately here we have to use unaligned access
    //since things inside the FGF stream are not aligned
    __m128d MIN = _mm_loadu_pd((double*)ext);
    __m128d MAX = _mm_loadu_pd((double*)(ext+2));
    double * ptr = pts;
    int n = npts;

    __m128d PT;
    do
    {
        PT = _mm_loadu_pd((double*)ptr);
        MIN = _mm_min_pd(MIN, PT);
        MAX = _mm_max_pd(MAX, PT);
        ptr += dim;
    } while (--n > 0);

    _mm_storeu_pd((double*)ext, MIN);
    _mm_storeu_pd((double*)(ext+2), MAX);
#else
    int end = npts * dim;
    for (int i=0; i<end; i+=dim)
    {
        if (pts[i] < ext[0])
            ext[0] = pts[i];
        if (pts[i] > ext[2])
            ext[2] = pts[i];
        if (pts[i+1] < ext[1])
            ext[1] = pts[i+1];
        if (pts[i+1] > ext[3])
            ext[3] = pts[i+1];
    }
#endif
}

//finds the bounding box of a given array of points
//and stores minx, miny, maxx and maxy in ext
//This version of the function assumes that the contents
//of the input ext can be clobbered.
static __forceinline void AddToEmptyExtent(int npts, int dim, double* __restrict pts, double* __restrict ext)
{
#if ENABLE_SSE
    //unfortunately here we have to use unaligned access
    //since things inside the FGF stream are not aligned
    __m128d PT = _mm_loadu_pd((double*)pts);
    __m128d MIN = PT;
    __m128d MAX = PT;

    pts += dim;

    while (--npts > 0)
    {
        PT = _mm_loadu_pd((double*)pts);
        MIN = _mm_min_pd(MIN, PT);
        MAX = _mm_max_pd(MAX, PT);
        pts += dim;
    } 

    _mm_storeu_pd((double*)ext, MIN);
    _mm_storeu_pd((double*)(ext+2), MAX);
#else
    memcpy(ext, pts, sizeof(double)*2);
    memcpy(ext+2, pts, sizeof(double)*2);
   
    int end = npts * dim;
    for (int i=dim; i<end; i+=dim)
    {
        if (pts[i] < ext[0])
            ext[0] = pts[i];
        if (pts[i] > ext[2])
            ext[2] = pts[i];
        if (pts[i+1] < ext[1])
            ext[1] = pts[i+1];
        if (pts[i+1] > ext[3])
            ext[3] = pts[i+1];
    }
#endif
}


//Translates given bounds by given offset, converts to float
//and stores in destination Bounds structure
static __forceinline void TranslateBounds(const DBounds* __restrict src, const double* __restrict offset, Bounds* __restrict dst)
{
#if ENABLE_SSE && (SI_DIM == 2) && USE_FLOAT_BOUNDS
    __m128d OFF = _mm_loadu_pd(offset);
    __m128d MIN = _mm_loadu_pd((double*)src);
    __m128d MAX = _mm_loadu_pd((double*)src+2);
    MIN = _mm_sub_pd(MIN, OFF);
    MAX = _mm_sub_pd(MAX, OFF);
    __m128 MINf = _mm_cvtpd_ps(MIN);
    __m128 MAXf = _mm_cvtpd_ps(MAX);
   MINf = _mm_shuffle_ps(MINf, MAXf, 0x44);
   _mm_store_ps((float*)dst, MINf);
#else
    for (int i=0; i<SI_DIM; i++)
    {
        dst->min[i] = (Real)(src->min[i] - offset[i]);
        dst->max[i] = (Real)(src->max[i] - offset[i]);
    }
#endif
}

static __forceinline void AssignBounds(double* __restrict ext, const double* __restrict pt)
{
#if ENABLE_SSE
    __m128d PT = _mm_loadu_pd(pt);
    _mm_storeu_pd((double*)ext, PT);
    _mm_storeu_pd((double*)(ext+2), PT);
#else
    memcpy(ext, pt, sizeof(double)*2);
    memcpy(ext+2, pt, sizeof(double)*2);
#endif
}

//Structures that are binary compatible with certain 
//types of FGF streams, allowing casting of streams
//to those structures.
#pragma pack(push, 1) 
struct FgfPoint
{
    int geom_type;
    int dim;
    double coords[3];
};
struct FgfPolyline
{
    FdoGeometryType type;
    FdoDimensionality dim;
    int np;
    double p[2];
};
struct FgfPolygon
{
    FgfPolygon() : type(FdoGeometryType_Polygon), dim(FdoDimensionality_XY), nrings(1), np(5) {}

    FdoGeometryType type;
    FdoDimensionality dim;
    int nrings;
    int np;
    double p[10];
};
#pragma pack(pop)

//FGF utility routines

void GetFgfExtents(const unsigned char* fgf, int len, double ext[4]);

int Fgf2Wkb(const unsigned char* fgf, unsigned char* wkb);

int Wkb2Fgf(const unsigned char* wkb, unsigned char* fgf);

double ComputeGeometryLength(const unsigned char* fgf, bool computeGeodetic = false);
double ComputeGeometryArea(const unsigned char* fgf, bool computeGeodetic = false);

#endif
