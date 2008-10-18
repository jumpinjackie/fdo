
#include "stdafx.h"
#include "SltExprExtensions.h"
#include <math.h>
#include <algorithm>

//===============================================================================
//  Basic math functions
//===============================================================================

static void mathFunc(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    if (sqlite3_value_type(argv[0]) == SQLITE_NULL)
    {
        sqlite3_result_null(context);
        return;
    }

    long funcId = (long)sqlite3_user_data(context);

    double rVal = sqlite3_value_double(argv[0]);
    double rVal2;

    if (argc == 2)
    {
        if (sqlite3_value_type(argv[1]) == SQLITE_NULL)
        {
            sqlite3_result_null(context);
            return;
        }

        rVal2 = sqlite3_value_double(argv[1]);
    }
    
    switch (funcId)
    {
    case 1: rVal = sin(rVal); break;
    case 2: rVal = cos(rVal); break;
    case 3: rVal = tan(rVal); break;
    case 4: rVal = atan(rVal); break;
    case 5: rVal = acos(rVal); break;
    case 6: rVal = asin(rVal); break;
    case 7: rVal = sqrt(rVal); break;
    case 8: rVal = log(rVal); break;
    case 9: rVal = log10(rVal); break;
    case 10: rVal = exp(rVal); break;
    case 11: rVal = pow(rVal, rVal2); break;
    case 12: rVal = atan2(rVal, rVal2); break;
    }

    sqlite3_result_double(context, rVal);
}

//===============================================================================
//  Numeric operations
//===============================================================================

static void numFunc(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    //if an argument is null, return null
    for (int i=0; i<argc; i++)
    {
        if (sqlite3_value_type(argv[i]) == SQLITE_NULL)
        {
            sqlite3_result_null(context);
            return;
        }
    }

    long funcId = (long)sqlite3_user_data(context);

    if (funcId == 1) //integer modulo
    {
        assert(argc == 2);
        i64 a1 = sqlite3_value_int64(argv[0]);
        i64 a2 = sqlite3_value_int64(argv[1]);
        sqlite3_result_int64(context, a1 % a2);
        return;
    }

    //all the other numeric funcs use
    //a single double input
    double val = sqlite3_value_double(argv[0]);
    bool resIsDouble = false;

    switch (funcId)
    {
    case 2: val = floor(val); break;
    case 3: val = ceil(val); break;
    case 4: 
        {
            if (argc == 2)
            {
                //if there is a second argument, it indicates
                //how many digits after the decimal we want in 
                //the result
                resIsDouble = true;
                int digits = sqlite3_value_int(argv[1]);
                double multiplier = 1;
                for (int j=0; j<digits; j++) multiplier *= 10.0;
                val *= multiplier;
                val = (i64)val;
                val /= multiplier;
            }
            else
                val = (i64)val; 
        }
        break;
    case 5: val = val > 0 ? 1 : (val < 0 ? -1 : 0); //sign function
        break;
    }

    if (resIsDouble)
        sqlite3_result_double(context, val);
    else
        sqlite3_result_int64(context, (i64)val);
}

//===============================================================================
//  String operations
//===============================================================================
static void strFunc(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    assert(argc == 2);

    const char* a1 = (const char*)sqlite3_value_text(argv[0]);
    const char* a2 = (const char*)sqlite3_value_text(argv[1]);
    
    long funcId = (long)sqlite3_user_data(context);

    if (funcId == 1)
    {
        size_t len1 = strlen(a1);
        size_t len2 = strlen(a2);
        char* res = (char*)alloca(len1 + len2 + 1);
        *res = 0;
        if (a1) strcat(res, a1);
        if (a2) strcat(res, a2);
        sqlite3_result_text(context, res, -1, SQLITE_TRANSIENT);
    }
    else
        sqlite3_result_null(context);
}

//===============================================================================
//  Conversion fuctions
//===============================================================================
//NOTE: these are implemented only for compatibility with FDO filters -- SQLite
//expression evaluation uses implicit type conversion, so they are not sctrictly
//necessary
static void convFunc(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    assert(argc == 1);
    
    long optype = (long)sqlite3_user_data(context);

    switch(optype)
    {
    case 1: //todate
        {
            //assumes date is stored in ISO 8601 string format
            const char* s = (const char*)sqlite3_value_text(argv[0]);
            sqlite3_result_text(context, s, -1, SQLITE_TRANSIENT);
        }
        break;
    case 2: //todouble
        {
            double d = sqlite3_value_double(argv[0]);
            sqlite3_result_double(context, d);
        }
        break;
    case 3: //tofloat
        {
            float f = (float)sqlite3_value_double(argv[0]);
            sqlite3_result_double(context, f);
        }
        break;
    case 4: //toint32
        {
            int i = sqlite3_value_int(argv[0]);
            sqlite3_result_int(context, i);
        }
        break;
    case 5: //toint64
        {
            sqlite_int64 i = sqlite3_value_int64(argv[0]);
            sqlite3_result_int64(context, i);
        }
        break;
    case 6: //tostring
        {
            const char* s = (const char*)sqlite3_value_text(argv[0]);
            sqlite3_result_text(context, s, -1, SQLITE_TRANSIENT);
        }
        break;
    }
}

//===============================================================================
//  Geometric operations
//===============================================================================

static void spatialOpFunc(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    assert(argc == 2);

    //args must be both blobs or text -- else fail.
    int type[] = { sqlite3_value_type(argv[0]), sqlite3_value_type(argv[1]) };

    if (  ((type[0] != SQLITE_BLOB) && (type[0] != SQLITE_TEXT))
        ||((type[1] != SQLITE_BLOB) && (type[1] != SQLITE_TEXT)) )
    {
        sqlite3_result_int(context, 0);
        return;
    }

    FdoPtr<FdoIGeometry> fg[2];
    FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();

    try
    {
        for (int i=0; i<2; i++)
        {
            //We need to figure out what type of geometry format we are dealing with.
            //I am not sure whether to be disgusted or delighted, but there is a way
            //to do this without drilling a giant hole in the code back to the 
            //FDO connection/schema. We can (with high confidence) determine the encoding by
            //(1) checking if the length of the stream is even or odd -- FGF is always even
            //while WKB is odd in the case of simple geometries, and (2) if the length
            //is even, by looking if the first 4 bytes in the stream, when treated as an 
            //integer are below 256 (due to the byte order byte resulting in a shift by 8 bits
            //of the geometry type in the case of WKB. This is only valid on little endian
            //machines (but works for both Intel and Motorla WKB streams).
            //The rest of this provider is not endian-safe anyway. If it is 
            //necessary that this work on big-endian, the solution would be to try to parse
            //using the FDO geometry factory, catch the resulting exception and try again
            //with the alternate encoding type.
            if (type[i] == SQLITE_BLOB)
            {
                const unsigned char* g1 = (const unsigned char*)sqlite3_value_blob(argv[i]);
                int len1 = sqlite3_value_bytes(argv[i]);

                bool isFGF = true;

                //even length check
                if (len1 % 2)
                    isFGF = false;
                else
                {
                    int type = *(int*)g1;

                    //check if first 4 bytes are out of range
                    //of FDO geometry types, due to there being
                    //a WKB byte order marker.
                    if (type > 255)
                        isFGF = false;
                }

                if (isFGF)
                {
                    fg[i] = gf->CreateGeometryFromFgf(g1, len1);
                }
                else
                {
                    FdoPtr<FdoByteArray> baba = FdoByteArray::Create(g1, len1);
                    fg[i] = gf->CreateGeometryFromWkb(baba);
                }
            }
            else
            {
                //WKT case
                const char* wkt = (const char*)sqlite3_value_text(argv[i]);
                int len = strlen(wkt) + 1;
                wchar_t* wwkt = (wchar_t*)alloca(sizeof(wchar_t) * len);
                mbstowcs(wwkt, wkt, len);
                fg[i] = gf->CreateGeometry(wwkt);        
            }
        }
    }
    catch (FdoException * e)
    {
        //geometry failed to parse -- return false for this spatial op
        e->Release();
        sqlite3_result_int(context, 0);
        return;
    }

    //retrieve the spatial op
    FdoSpatialOperations spatialOp = (FdoSpatialOperations)(long)sqlite3_user_data(context);

    //call the spatial utility to eval the spatial op
    bool res = FdoSpatialUtility::Evaluate(fg[0], spatialOp, fg[1]);

    sqlite3_result_int(context, res ? 1 : 0);
}

static void GeomFromText(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    assert(argc == 1);

    //always false if an arg is null
    if (sqlite3_value_type(argv[0]) == SQLITE_NULL)
    {
        sqlite3_result_null(context);
        return;
    }

    const char* wkt = (const char*)sqlite3_value_text(argv[0]);
    int len = strlen(wkt) + 1;
    wchar_t* wwkt = (wchar_t*)alloca(sizeof(wchar_t) * len);
    mbstowcs(wwkt, wkt, len);

    FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
    FdoPtr<FdoIGeometry> geom = gf->CreateGeometry(wwkt);
    FdoPtr<FdoByteArray> fgf = gf->GetFgf(geom);

    sqlite3_result_blob(context, fgf->GetData(), fgf->GetCount(), SQLITE_TRANSIENT);
}

//===============================================================================
//  Statistics
//===============================================================================

//Context for variance computation
struct VarCtx 
{
    i64 n; //number of items processed
    double mean; //incremental mean
    double S; //incremental variance
};

static void varStep(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    VarCtx *p;
    int type;
    assert( argc==1 );
    p = (VarCtx*)sqlite3_aggregate_context(context, sizeof(*p));
    type = sqlite3_value_numeric_type(argv[0]);
    if( p && type!=SQLITE_NULL )
    {
        //Numerically stable, incremental variance algorithm
        //Knuth TACP Vol. 2
        p->n++;
        double val = sqlite3_value_double(argv[0]);
        double delta = val - p->mean;
        p->mean += delta / (double)p->n;
        p->S += delta * (val - p->mean);
    }
}
static void varFinalize(sqlite3_context *context)
{
    VarCtx *p;
    p = (VarCtx*)sqlite3_aggregate_context(context, 0);
    if( p )
    {
        sqlite3_result_double(context, (p->n == 1) ? 0 : p->S/((double)p->n - 1));
    }
}
static void stdevFinalize(sqlite3_context *context)
{
    VarCtx *p;
    p = (VarCtx*)sqlite3_aggregate_context(context, 0);
    if( p )
    {
        sqlite3_result_double(context, (p->n == 1) ? 0 : sqrt(p->S/((double)p->n - 1)));
    }
}


struct MedCtx
{
    std::vector<double> * vals;
    int resInt;
};

static void medStep(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    MedCtx *p;
    int type;
    assert( argc==1 );
    p = (MedCtx*)sqlite3_aggregate_context(context, sizeof(*p));
    type = sqlite3_value_numeric_type(argv[0]);
    if( p && type!=SQLITE_NULL )
    {
        if (!p->vals)
        {
            p->vals = new std::vector<double>();
            p->resInt = 1;
        }

        if (type!=SQLITE_INTEGER)
            p->resInt = 0;

        p->vals->push_back(sqlite3_value_double(argv[0]));
    }
}


static void medFinalize(sqlite3_context *context)
{
    MedCtx *p;
    p = (MedCtx*)sqlite3_aggregate_context(context, 0);
    if( p )
    {
        size_t nvals = p->vals->size();

        double dRes;

        if (nvals == 0)
        {
            sqlite3_result_null(context);
        }
        else if (nvals == 1)
        {
            dRes = p->vals->at(0);
        }
        else
        {
            double dRes;
            
            //nth_element is using Hoare's algorithm for expected O(n)
            //running time. It's the best we can do for median theoretically.
            std::nth_element(p->vals->begin(), p->vals->begin() + nvals/2, p->vals->end());

            if (nvals % 2 != 0)
            {
                //odd number of elements -- median is located
                //at element n/2
                dRes = p->vals->at(nvals/2);
            }
            else
            {
                //even number of elements -- median is average between
                //array elements n/2 and n/2-1 -- we need to do the
                //nth_element once more.
                std::nth_element(p->vals->begin(), p->vals->begin() + nvals/2-1, p->vals->end());

                dRes = (p->vals->at(nvals/2) + p->vals->at(nvals/2-1)) / 2.0;

                //if the median is no longer an integer
                //make sure the result is returned as double
                if (p->resInt && dRes != (int)dRes)
                    p->resInt = 0;
            }
        }

        if (p->resInt)
            sqlite3_result_int(context, (int)dRes);
        else
            sqlite3_result_double(context, dRes);

        delete p->vals;
    }
}


//===============================================================================
//  Register extensions functions with SQLite
//===============================================================================

void RegisterExtensions (sqlite3* db)
{
    static const struct {
        const char *zName;
        signed char nArg;
        u8 argType;           /* ff: db   1: 0, 2: 1, 3: 2,...  N:  N-1. */
        u8 eTextRep;          /* 1: UTF-16.  0: UTF-8 */
        u8 needCollSeq;
        void (*xFunc)(sqlite3_context*,int,sqlite3_value **);
    } aFuncs[] = {
        { "sin",                1,  1, SQLITE_UTF8,    0, mathFunc },
        { "cos",                1,  2, SQLITE_UTF8,    0, mathFunc },
        { "tan",                1,  3, SQLITE_UTF8,    0, mathFunc },
        { "atan",               1,  4, SQLITE_UTF8,    0, mathFunc },
        { "acos",               1,  5, SQLITE_UTF8,    0, mathFunc },
        { "asin",               1,  6, SQLITE_UTF8,    0, mathFunc },
        { "sqrt",               1,  7, SQLITE_UTF8,    0, mathFunc },
        { "ln",                 1,  8, SQLITE_UTF8,    0, mathFunc },
        { "log",                1,  9, SQLITE_UTF8,    0, mathFunc },
        { "exp",                1, 10, SQLITE_UTF8,    0, mathFunc },
        { "pow",                2, 11, SQLITE_UTF8,    0, mathFunc },
        { "atan2",              2, 12, SQLITE_UTF8,    0, mathFunc },

        { "mod",                2, 1,  SQLITE_UTF8,    0, numFunc },
        { "floor",              1, 2,  SQLITE_UTF8,    0, numFunc },
        { "ceil",               1, 3,  SQLITE_UTF8,    0, numFunc },
        { "trunc",              1, 4,  SQLITE_UTF8,    0, numFunc },
        { "trunc",              2, 4,  SQLITE_UTF8,    0, numFunc },
        { "sign",               2, 5,  SQLITE_UTF8,    0, numFunc },

        { "concat",             2, 1,  SQLITE_UTF8,    0, strFunc },

        { g_spatial_op_map[FdoSpatialOperations_Contains],  2, FdoSpatialOperations_Contains,   SQLITE_UTF8,    0, spatialOpFunc },
        { g_spatial_op_map[FdoSpatialOperations_Crosses],   2, FdoSpatialOperations_Crosses,    SQLITE_UTF8,    0, spatialOpFunc },
        { g_spatial_op_map[FdoSpatialOperations_Disjoint],  2, FdoSpatialOperations_Disjoint,   SQLITE_UTF8,    0, spatialOpFunc },
        { g_spatial_op_map[FdoSpatialOperations_Equals],    2, FdoSpatialOperations_Equals,     SQLITE_UTF8,    0, spatialOpFunc },
        { g_spatial_op_map[FdoSpatialOperations_Intersects],2, FdoSpatialOperations_Intersects, SQLITE_UTF8,    0, spatialOpFunc },
        { g_spatial_op_map[FdoSpatialOperations_Overlaps],  2, FdoSpatialOperations_Overlaps,   SQLITE_UTF8,    0, spatialOpFunc },
        { g_spatial_op_map[FdoSpatialOperations_Touches],   2, FdoSpatialOperations_Touches,    SQLITE_UTF8,    0, spatialOpFunc },
        { g_spatial_op_map[FdoSpatialOperations_Within],    2, FdoSpatialOperations_Within,     SQLITE_UTF8,    0, spatialOpFunc },
        { g_spatial_op_map[FdoSpatialOperations_CoveredBy], 2, FdoSpatialOperations_CoveredBy,  SQLITE_UTF8,    0, spatialOpFunc },
        { g_spatial_op_map[FdoSpatialOperations_Inside],    2, FdoSpatialOperations_Inside,     SQLITE_UTF8,    0, spatialOpFunc },
        { g_spatial_op_map[FdoSpatialOperations_EnvelopeIntersects],  2, FdoSpatialOperations_EnvelopeIntersects, SQLITE_UTF8, 0, spatialOpFunc },

        { "GeomFromText",       1, 0, SQLITE_UTF8, 0, GeomFromText },

        { "todate",             1, 1,  SQLITE_UTF8,    0, convFunc },
        { "todouble",           1, 2,  SQLITE_UTF8,    0, convFunc },
        { "tofloat",            1, 3,  SQLITE_UTF8,    0, convFunc },
        { "toint32",            1, 4,  SQLITE_UTF8,    0, convFunc },
        { "toint64",            1, 5,  SQLITE_UTF8,    0, convFunc },
        { "tostring",           1, 6,  SQLITE_UTF8,    0, convFunc }

    };
   
    static const struct {
        const char *zName;
        signed char nArg;
        u8 argType;
        u8 needCollSeq;
        void (*xStep)(sqlite3_context*,int,sqlite3_value**);
        void (*xFinalize)(sqlite3_context*);
    } aAggs[] = 
    {
        { "variance",    1, 0, 0, varStep,      varFinalize    },
        { "stdev",       1, 0, 0, varStep,      stdevFinalize  },
        { "median",      1, 0, 0, medStep,      medFinalize    },
    };
    
    int i;

    for(i=0; i<sizeof(aFuncs)/sizeof(aFuncs[0]); i++)
    {
        void *pArg;
        u8 argType = aFuncs[i].argType;
        if( argType==0xff )
        {
            pArg = db;
        }else
        {
            pArg = (void*)(int)argType;
        }

        sqlite3_create_function(db, aFuncs[i].zName, aFuncs[i].nArg,
            aFuncs[i].eTextRep, pArg, aFuncs[i].xFunc, 0, 0);
    }

    for(i=0; i<sizeof(aAggs)/sizeof(aAggs[0]); i++)
    {
        void *pArg = (void*)(int)aAggs[i].argType;
        sqlite3CreateFunc(db, aAggs[i].zName, aAggs[i].nArg, SQLITE_UTF8, 
            pArg, 0, aAggs[i].xStep, aAggs[i].xFinalize);
    }
}
