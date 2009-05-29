
#include "stdafx.h"
#include "SltExprExtensions.h"
#include "SltConversionUtils.h"
#include "SltGeomUtils.h"
#include "FdoCommonOSUtil.h"
#include <math.h>
#include <algorithm>

#define SQLITE_INFO_STRING(/*IN*/str, /*OUT*/len, /*OUT*/lenInChars, /*IN-OUT*/charsToStudy) {  \
  len = lenInChars = 0;                                                                         \
  const char* z2 = NULL;                                                                        \
  for(z2 = str; *z2 && charsToStudy; charsToStudy--) {                                          \
      lenInChars++;                                                                             \
      SQLITE_SKIP_UTF8(z2);                                                                     \
  }                                                                                             \
  len = (z2 - str);                                                                             \
}                                                                                               \

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
    double rVal2 = 0.0;

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
    case 9: rVal = exp(rVal); break;
    case 10: rVal = log10(rVal); break;
    case 11: rVal = log(rVal2)/log(rVal); break;
    case 12: rVal = pow(rVal, rVal2); break;
    case 13: rVal = atan2(rVal, rVal2); break;
    }

    sqlite3_result_double(context, rVal);
}

//===============================================================================
//  date operations
//===============================================================================
static void currDateFunc(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    assert( argc==0 );
    FdoDateTime dt;
    struct tm local_time;
    FdoCommonOSUtil::getsystime(&local_time);
    dt.year    = (FdoInt16)(local_time.tm_year + 1900);
    dt.month   = (FdoInt8)(local_time.tm_mon + 1);
    dt.day     = (FdoInt8)(local_time.tm_mday);
    dt.hour    = (FdoInt8)(local_time.tm_hour);
    dt.minute  = (FdoInt8)(local_time.tm_min);
    dt.seconds = (float)local_time.tm_sec;
    char* res = (char*)alloca(31); // more than datetime needs
    *res = '\0';
    DateToString(&dt, res, 31);
    sqlite3_result_text(context, res, -1, SQLITE_TRANSIENT);
}

static void dateFunc(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    assert( argc==2 );
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
    switch(funcId)
    {
        case 1:
        {
            const char* dateStr = (const char*)sqlite3_value_text(argv[0]);
            if (sqlite3_value_bytes(argv[0]) == 0)
            {
                sqlite3_result_null(context);
                return;
            }
            FdoInt64 months_to_add = sqlite3_value_int64(argv[1]);
            if (months_to_add == 0)
            {
                sqlite3_result_text(context, dateStr, sqlite3_value_bytes(argv[0]), SQLITE_TRANSIENT);
                return;
            }
            FdoDateTime dt = DateFromString(dateStr);
            FdoInt16 num_of_years  = (FdoInt16) (months_to_add / 12);
            FdoInt8  num_of_months = (FdoInt8)  (months_to_add % 12);
            FdoInt16 curr_year  = (dt.year  == -1) ? 0 : dt.year;
            FdoInt8  curr_month = (dt.month == -1) ? 1 : dt.month;
            
            if (months_to_add > 0)
            {
                if ((curr_month + num_of_months) > 12)
                {
                    num_of_years++;
                    dt.month = (curr_month + num_of_months) - 12;
                }
                else
                    dt.month = (curr_month + num_of_months);
            }
            else
            {
                if ((curr_month + num_of_months) < 0)
                {
                    num_of_years--;
                    dt.month = 12 + (curr_month + num_of_months);
                }
                else
                    dt.month = (curr_month + num_of_months);
            }
            dt.year = curr_year + num_of_years;
            
            char* res = (char*)alloca(31); // more than datetime needs
            *res = '\0';
            DateToString(&dt, res, 31);
            sqlite3_result_text(context, res, -1, SQLITE_TRANSIENT);
        }
            break;
        case 2:
        case 3:
        case 4:
        {
            int operation = -1;
            if (sqlite3_value_type(argv[0]) == SQLITE_TEXT && sqlite3_value_bytes(argv[0]) != 0)
            {
                const char* pOption = (const char*)sqlite3_value_text(argv[0]);
                if (sqlite3StrICmp(pOption, "YEAR") == 0)
                    operation = 0;
                else if (sqlite3StrICmp(pOption, "MONTH") == 0)
                    operation = 1;
                else if (sqlite3StrICmp(pOption, "DAY") == 0)
                    operation = 2;
                else if (sqlite3StrICmp(pOption, "HOUR") == 0)
                    operation = 3;
                else if (sqlite3StrICmp(pOption, "MINUTE") == 0)
                    operation = 4;
                else if (sqlite3StrICmp(pOption, "SECOND") == 0)
                    operation = 5;
            }
            if (operation != -1 && sqlite3_value_bytes(argv[1]) != 0)
            {
                const char* dateStr = (const char*)sqlite3_value_text(argv[1]);
                FdoDateTime dt = DateFromString(dateStr);
                FdoDateTime dtRet;
                double valRet = 0.0;
                switch(operation)
                {
                case 0:
                    valRet = dtRet.year = dt.year;
                    break;
                case 1:
                    valRet = dtRet.month = dt.month;
                    break;
                case 2:
                    valRet = dtRet.day = dt.day;
                    break;
                case 3:
                    valRet = dtRet.hour = dt.hour;
                    break;
                case 4:
                    valRet = dtRet.minute = dt.minute;
                    break;
                case 5:
                    valRet = dtRet.seconds = dt.seconds;
                    break;
                }
                if (funcId == 2)
                {
                    char* res = (char*)alloca(31); // more than datetime needs
                    *res = '\0';
                    DateToString(&dtRet, res, 31);
                    sqlite3_result_text(context, res, -1, SQLITE_TRANSIENT);
                }
                else if (funcId == 3)
                    sqlite3_result_double(context, valRet);
                else // (funcId == 4)
                    sqlite3_result_int(context, (FdoInt16)((FdoInt32)((valRet - floor(valRet)) < 0.5) ? floor(valRet) : ceil(valRet)));
            }
            else            
                sqlite3_result_null(context);
        }
            break;
        case 5:
        {
            const char* dateStr1 = (const char*)sqlite3_value_text(argv[0]);
            const char* dateStr2 = (const char*)sqlite3_value_text(argv[1]);
            if (sqlite3_value_bytes(argv[0]) != 0 && sqlite3_value_bytes(argv[1]) != 0)
            {
                FdoDateTime start = DateFromString(dateStr1);
                FdoDateTime end = DateFromString(dateStr2);
                if ((start.year == -1) || (start.month == -1) || (end.year == -1) || (end.month == -1))
                    sqlite3_result_null(context);
                else
                    sqlite3_result_double(context, (end.year - start.year) * 12 + end.month - start.month);
            }
            else            
                sqlite3_result_null(context);
        }
            break;
        default:
            sqlite3_result_null(context);
            break;
    }
}

static bool TruncateDate(FdoDateTime& dt, int opDate)
{
    bool truncation_done = false;
    FdoInt16   year = -1;
    FdoInt8    month = -1, day = -1, hour = -1, minute = -1;
    switch(opDate)
    {
      case 0: // Year
        if ((dt.year  != -1) && (dt.month != -1) && (dt.day   != -1))
        {
            year = dt.year;
            month = 1;
            day = 1;
            if (dt.IsDateTime())
                hour = minute = 0;
            truncation_done = true;
        }
        break;
      case 1: // Month
        if ((dt.year  != -1) && (dt.month != -1) && (dt.day   != -1))
        {
            year = dt.year;
            month = dt.month;
            day = 1;
            if (dt.IsDateTime())
                hour = minute = 0;
            truncation_done = true;
        }
        break;
      case 2: // Day
        break;
      case 3: // Hour
        if ((dt.hour != -1  ) && (dt.minute != -1))
        {
            hour = dt.hour;
            minute = 0;
            truncation_done = true;
            if ((dt.year  != -1) && (dt.month != -1) && (dt.day   != -1))
            {
                year = dt.year;
                month = dt.month;
                day = dt.day;
            }
        }
        break;
      case 4: // Minute
        if ((dt.hour != -1  ) && (dt.minute != -1))
        {
            hour = dt.hour;
            minute = dt.hour;
            truncation_done = true;
            if ((dt.year  != -1) && (dt.month != -1) && (dt.day   != -1))
            {
                year = dt.year;
                month = dt.month;
                day = dt.day;
            }
        }
        break;
    }
    if (truncation_done)
    {
        dt.year = year;
        dt.month = month;
        dt.day = day;
        dt.hour = hour;
        dt.minute = minute;
        dt.seconds = 0.0f;
    }
    return truncation_done;
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
                int operation = -1;
                if (sqlite3_value_type(argv[1]) == SQLITE_TEXT)
                {
                    const char* pOption = (const char*)sqlite3_value_text(argv[1]);
                    if (sqlite3StrICmp(pOption, "YEAR") == 0)
                        operation = 0;
                    else if (sqlite3StrICmp(pOption, "MONTH") == 0)
                        operation = 1;
                    else if (sqlite3StrICmp(pOption, "DAY") == 0)
                        operation = 2;
                    else if (sqlite3StrICmp(pOption, "HOUR") == 0)
                        operation = 3;
                    else if (sqlite3StrICmp(pOption, "MINUTE") == 0)
                        operation = 4;
                }
                if (operation != -1)
                {
                    const char* dateStr = (const char*)sqlite3_value_text(argv[0]);
                    FdoDateTime dt = DateFromString(dateStr);
                    if(TruncateDate(dt, operation))
                    {
                        char* res = (char*)alloca(31); // more than datetime needs
                        *res = '\0';
                        DateToString(&dt, res, 31);
                        sqlite3_result_text(context, res, -1, SQLITE_TRANSIENT);
                    }
                    else
                        sqlite3_result_null(context);
                    return;
                }
                else
                {
                    //if there is a second argument, it indicates
                    //how many digits after the decimal we want in 
                    //the result
                    resIsDouble = true;
                    int digits = sqlite3_value_int(argv[1]);
                    double multiplier = 1;
                    for (int j=0; j<digits; j++) multiplier *= 10.0;
                    val *= multiplier;
                    val = (double)((i64)val);
                    val /= multiplier;
                }
            }
            else
                val = (double)((i64)val);
        }
        break;
    case 5: val = val > 0 ? 1 : (val < 0 ? -1 : 0); //sign function
        break;
    case 6: 
        {
            resIsDouble = true;
            if (val != 0.0)
            {
                double divisor = (argc != 2) ? 0.0 : sqlite3_value_double(argv[1]);
                val = fmod(val, divisor);
            }
        }
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
    assert(argc >= 2);

    const char* a1 = (const char*)sqlite3_value_text(argv[0]);
    const char* a2 = (const char*)sqlite3_value_text(argv[1]);
    
    bool isNullargv0 = (sqlite3_value_type(argv[0]) == SQLITE_NULL) || (a1 == NULL);
    bool isNullargv1 = (sqlite3_value_type(argv[1]) == SQLITE_NULL) || (a2 == NULL);
    
    long funcId = (long)sqlite3_user_data(context);

    switch(funcId)
    {
        case 1: // concat
        {
            assert(argc == 2);
            if (isNullargv0 && isNullargv1)
                sqlite3_result_null(context);
            else if((isNullargv0 && !isNullargv1) || (!isNullargv0 && isNullargv1))
                sqlite3_result_text(context, (a1 != NULL) ? a1 : a2, -1, SQLITE_TRANSIENT);
            else
            {
                char* res = (char*)alloca(sqlite3_value_bytes(argv[0]) + sqlite3_value_bytes(argv[1]) + 1);
                *res = '\0';
                strcat(res, a1);
                strcat(res, a2);
                sqlite3_result_text(context, res, -1, SQLITE_TRANSIENT);
            }
        }
            break;
        case 2: // instr
        {
            assert(argc == 2);
            if (!isNullargv0 && !isNullargv1)
            {
                i64 pos = 0;
                const char* strPos = strstr(a1, a2);
                if (strPos != NULL)
                    pos = strPos - a1 + 1;
                sqlite3_result_int64(context, pos);
            }
            else
                sqlite3_result_null(context);
        }
            break;
        case 3: // translate
        {
            assert(argc == 3);
            const char* a3 = (const char*)sqlite3_value_text(argv[2]);
            bool isNullargv2 = (sqlite3_value_type(argv[1]) == SQLITE_NULL) || (a3 == NULL);
            size_t src_len = sqlite3_value_bytes(argv[0]);
            size_t from_len = sqlite3_value_bytes(argv[1]);
            size_t to_len = sqlite3_value_bytes(argv[2]);
            if (isNullargv0 || isNullargv1|| isNullargv2 || src_len == 0 || from_len == 0 || to_len == 0)
            {
                sqlite3_result_null(context);
                return;
            }
            // we may replace normal chars with UTF-8 characters
            char* res = (char*)alloca(6*src_len + 1);
            *res = '\0';
            char curr_char[7];
            const char* z2 = NULL;
            size_t main_idx = 0; 
            for(z2 = a1; *z2;)
            {
                size_t sub_idx = 0; 
                curr_char[sub_idx++] = *z2;
                // SQLITE_SKIP_UTF8 with copy character into curr_char
                if( (*(z2++))>=0xc0 )
                {
                    while((*z2 & 0xc0) == 0x80)
                    {
                        curr_char[sub_idx++] = *z2;
                        z2++;
                    }
                }
                curr_char[sub_idx] = '\0';
                const char* posInStr = strstr(a2, curr_char);
                if (posInStr != NULL)
                {
                    size_t pos = posInStr-a2;
                    if (pos <= to_len)
                    {
                        const char* z3 = a3+pos;
                        *(res + main_idx++) = *z3;
                        // SQLITE_SKIP_UTF8 with copy character into res
                        if( (*(z3++))>=0xc0 )
                        {
                            while((*z3 & 0xc0) == 0x80)
                            {
                                *(res + main_idx++) = *z3;
                                z3++;
                            }
                        }
                    }
                }
                else
                {
                    memcpy(res + main_idx, curr_char, sub_idx);
                    main_idx += sub_idx;
                }
            }
            *(res + main_idx) = '\0';
            sqlite3_result_text(context, res, main_idx, SQLITE_TRANSIENT);
        }
            break;
        default:
            sqlite3_result_null(context);
        break;
    }
}

static void padFunc(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    assert( argc==3 || argc==2 );

    long funcId = (long)sqlite3_user_data(context);
    const char* a1 = (const char*)sqlite3_value_text(argv[0]);
    bool isNullargv0 = (sqlite3_value_type(argv[0]) == SQLITE_NULL) || (a1 == NULL);
    bool isNullargv1 = (sqlite3_value_type(argv[1]) == SQLITE_NULL);
    bool isNullargv2 = true;
    const char* a2 = NULL;
    i64 cnt = 0;
    
    if(argc==3)
    {
        a2 = (const char*)sqlite3_value_text(argv[2]);
        isNullargv2 = (sqlite3_value_type(argv[2]) == SQLITE_NULL) || (a2 == NULL);
    }
    
    if (isNullargv0 || isNullargv1 || (cnt = sqlite3_value_int64(argv[1])) <= 0)
    {
        sqlite3_result_null(context);
        return;
    }

    i64 limit = sqlite3_context_db_handle(context)->aLimit[SQLITE_LIMIT_LENGTH];
    if (cnt >= limit)
        cnt = limit;

    if (isNullargv2)
        a2 = " ";
    isNullargv2 = false;

    switch(funcId)
    {
        case 1: // lpad
        case 2: // rpad
        {
            i64 charsToGet = cnt;
            i64 len1 = 0;
            i64 charsInChars1 = 0;
            SQLITE_INFO_STRING(a1, len1, charsInChars1, charsToGet);
            if (charsToGet == 0) // we have less characters to get
                sqlite3_result_text(context, a1, (size_t)len1, SQLITE_TRANSIENT);
            else
            {
                i64 charsToGet2 = charsToGet;
                i64 len2 = 0;
                i64 charsInChars2 = 0;
                SQLITE_INFO_STRING(a2, len2, charsInChars2, charsToGet2);
                // how many times a2 must be copied
                size_t cntStrToInsert = (size_t)(charsToGet/charsInChars2);
                i64 cntRemainToInsert = charsToGet%charsInChars2;

                i64 lenR2 = 0;
                i64 charsInCharsR2 = 0;
                // how many chars can we copy from a2 in case we cannot fit the entire string
                if (cntRemainToInsert != 0)
                {
                    SQLITE_INFO_STRING(a2, lenR2, charsInCharsR2, cntRemainToInsert);
                }
                i64 pos = 0;
                char* res = (char*)alloca((size_t)(len1 + cntStrToInsert*len2 + lenR2 + 1));
                if (funcId == 2)
                {
                    // add original string
                    memcpy(res+pos, a1, (size_t)len1);
                    pos += len1;
                }
                // add all full pad strings
                for (size_t i = 0; i < cntStrToInsert; i++)
                {
                    memcpy(res+pos, a2, (size_t)len2);
                    pos += len2;
                }
                // add all partial pad string
                if (lenR2 != 0)
                {
                    memcpy(res+pos, a2, (size_t)lenR2);
                    pos += lenR2;
                }
                if (funcId == 1)
                {
                    // add original string
                    memcpy(res+pos, a1, (size_t)len1);
                    pos += len1;
                }
                *(res+pos) = '\0';
                sqlite3_result_text(context, res, (size_t)pos, SQLITE_TRANSIENT);
            }
        }
            break;
        default:
            sqlite3_result_null(context);
        break;
    }
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

static void nullvalueFunc(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    assert(argc == 2);
    sqlite3_value* val = (sqlite3_value_type(argv[0]) == SQLITE_NULL || 
        sqlite3_value_bytes(argv[0]) == 0) ? argv[1] : argv[0];

    switch(sqlite3_value_type(val))
    {
        case SQLITE_INTEGER:
            sqlite3_result_int64(context, sqlite3_value_int64(val));
            break;
        case SQLITE_FLOAT:
            sqlite3_result_double(context, sqlite3_value_double(val));
            break;
        case SQLITE_BLOB:
            sqlite3_result_blob(context, sqlite3_value_blob(val), sqlite3_value_bytes(val), SQLITE_TRANSIENT);
            break;
        case SQLITE_NULL:
            sqlite3_result_null(context);
            break;
        case SQLITE_TEXT:
            sqlite3_result_text(context, (const char*)sqlite3_value_text(val), -1, SQLITE_TRANSIENT);
            break;
        default:
            sqlite3_result_null(context);
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

//Implementation of the X(), Y(), Z() and M() functions for point geometries
static void xyzmFunc(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    assert(argc == 1);

    double ret = 0;

    //extract operation type 1 = X, 2 = Y, 3 = Z, 4 = M
    long optype = (long)sqlite3_user_data(context);

    FgfPoint pt;
    FdoPtr<FdoIGeometry> fg;
    FdoPtr<FdoFgfGeometryFactory> gf;
    bool valid = false;
    memset(&pt, 0x00, sizeof(pt));

    //data type of argument -- must be BLOB (FGF) or text.
    int type = sqlite3_value_type(argv[0]);

    //extract the point
    if (type == SQLITE_BLOB)
    {
        const unsigned char* g1 = (const unsigned char*)sqlite3_value_blob(argv[0]);
        int len1 = sqlite3_value_bytes(argv[0]);

        if (len1 % 2) //this check is enough to detect WKB in case of simple geometries, but may fail on multi ones
                      //also see the long explanation in spatialOpFunc.
        {
            //case of WKB, not FGF
            gf = FdoFgfGeometryFactory::GetInstance();
            FdoPtr<FdoByteArray> ba = FdoByteArray::Create(g1, len1);
            fg = gf->CreateGeometryFromWkb(ba);
        }
        else
        {
            //case of FGF, we can directly use the byte array as FgfPoint
            pt = *(FgfPoint*)g1;

            if (pt.geom_type == FdoGeometryType_Point)
                valid = true;
        }
    }
    else if (type == SQLITE_TEXT)
    {
        const char* wkt = (const char*)sqlite3_value_text(argv[0]);
        int len = strlen(wkt) + 1;
        wchar_t* wwkt = (wchar_t*)alloca(sizeof(wchar_t) * len);
        mbstowcs(wwkt, wkt, len);

        gf = FdoFgfGeometryFactory::GetInstance();
        fg = gf->CreateGeometry(wwkt);        
    }

    if (fg.p && fg->GetDerivedType() == FdoGeometryType_Point)
    {
        valid = true;
        FdoPtr<FdoByteArray> ba = gf->GetFgf(fg);
        pt = *(FgfPoint*)ba->GetData();
    }

    //if we have a valid geometry, extract the value we need to return
    if (valid)
    {
        switch (optype)
        {
        case 1: ret = pt.coords[0]; break;
        case 2: ret = pt.coords[1]; break;
        case 3: if (pt.dim & FdoDimensionality_Z) 
                    ret = pt.coords[2]; break;
        case 4: if (pt.dim & FdoDimensionality_M)
                    if (pt.dim & FdoDimensionality_Z)
                        ret = pt.coords[3];
                    else 
                        ret = pt.coords[2];
                break;
        }
    }

    sqlite3_result_double(context, ret);
}

//Implementation of the Lenght(), Area() functions for geometries
static void geomFunc(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    assert(argc == 1);

    //extract operation type 1 = len, 2 = area
    long optype = (long)sqlite3_user_data(context);

    FdoPtr<FdoFgfGeometryFactory> gf;
    FdoPtr<FdoIGeometry> fg;

    const unsigned char* geom = NULL;
    long lenGeom = 0;

    //data type of argument -- must be BLOB (FGF) or text.
    int type = sqlite3_value_type(argv[0]);

    if (type == SQLITE_BLOB)
    {
        const unsigned char* g1 = (const unsigned char*)sqlite3_value_blob(argv[0]);
        int len = sqlite3_value_bytes(argv[0]);

        if (g1 != NULL && len != 0)
        {
            if (len % 2) //this check is enough to detect WKB in case of simple geometries, but may fail on multi ones
                          //also see the long explanation in spatialOpFunc.
            {
                //case of WKB, not FGF
                gf = FdoFgfGeometryFactory::GetInstance();
                FdoPtr<FdoByteArray> ba = FdoByteArray::Create(g1, len);
                fg = gf->CreateGeometryFromWkb(ba);
                ba = gf->GetFgf(fg);
                geom = (const unsigned char*)ba->GetData();
                lenGeom = ba->GetCount();
            }
            else
            {
                //case of FGF, we can directly use the byte array
                geom = g1;
                lenGeom = len;
            }
        }
    }
    else if (type == SQLITE_TEXT)
    {
        const char* wkt = (const char*)sqlite3_value_text(argv[0]);
        int len = sqlite3_value_bytes(argv[0]);
        if (wkt != NULL && len != 0)
        {
            wchar_t* wwkt = (wchar_t*)alloca(sizeof(wchar_t) * (len+1));
            mbstowcs(wwkt, wkt, len+1);

            gf = FdoFgfGeometryFactory::GetInstance();
            fg = gf->CreateGeometry(wwkt);        
            FdoPtr<FdoByteArray> ba = gf->GetFgf(fg);
            geom = (const unsigned char*)ba->GetData();
            lenGeom = ba->GetCount();
        }
    }

    if (geom == NULL) // enforce a null return
        optype = 0;

    // TODO: we need to find a way to 'detect' if geometry is geodetic
    // somehow from class definition-geometric property we need to look at 
    // spatial context WKT if contains/starts with 'GEOGCS'
    // the problem is not to detect if is geodetic, it is to 
    // propagate it till here from select command
    bool computeGeodetic = false; // for now keep it to false
    switch(optype)
    {
    case 1:
        sqlite3_result_double(context, ComputeGeometryLength(geom, computeGeodetic));
        break;
    case 2:
        sqlite3_result_double(context, ComputeGeometryArea(geom, computeGeodetic));
        break;
    default:
        sqlite3_result_null(context);
        break;
    }
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

        double dRes = 0.0;

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
        { "exp",                1,  9, SQLITE_UTF8,    0, mathFunc },
        { "log10",              1, 10, SQLITE_UTF8,    0, mathFunc },
        { "log",                2, 11, SQLITE_UTF8,    0, mathFunc },
        { "power",              2, 12, SQLITE_UTF8,    0, mathFunc },
        { "atan2",              2, 13, SQLITE_UTF8,    0, mathFunc },

        { "mod",                2, 1,  SQLITE_UTF8,    0, numFunc },
        { "floor",              1, 2,  SQLITE_UTF8,    0, numFunc },
        { "ceil",               1, 3,  SQLITE_UTF8,    0, numFunc },
        { "trunc",              1, 4,  SQLITE_UTF8,    0, numFunc },
        { "trunc",              2, 4,  SQLITE_UTF8,    0, numFunc },
        { "sign",               1, 5,  SQLITE_UTF8,    0, numFunc },
        { "remainder",          2, 6,  SQLITE_UTF8,    0, numFunc },

        { "concat",             2, 1,  SQLITE_UTF8,    0, strFunc },
        { "instr",              2, 2,  SQLITE_UTF8,    0, strFunc },
        { "translate",          3, 3,  SQLITE_UTF8,    0, strFunc },

        { "lpad",               2, 1,  SQLITE_UTF8,    0, padFunc },
        { "lpad",               3, 1,  SQLITE_UTF8,    0, padFunc },
        { "rpad",               2, 2,  SQLITE_UTF8,    0, padFunc },
        { "rpad",               3, 2,  SQLITE_UTF8,    0, padFunc },

        { "currentdate",        0, 1,  SQLITE_UTF8,    0, currDateFunc },
        { "addmonths",          2, 1,  SQLITE_UTF8,    0, dateFunc },
        { "extract",            2, 2,  SQLITE_UTF8,    0, dateFunc },
        { "extracttodouble",    2, 3,  SQLITE_UTF8,    0, dateFunc },
        { "extracttoint",       2, 4,  SQLITE_UTF8,    0, dateFunc },
        { "monthsbetween",      2, 5,  SQLITE_UTF8,    0, dateFunc },
        

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

        { "X",                  1, 1, SQLITE_UTF8,     0, xyzmFunc },
        { "Y",                  1, 2, SQLITE_UTF8,     0, xyzmFunc },
        { "Z",                  1, 3, SQLITE_UTF8,     0, xyzmFunc },
        { "M",                  1, 4, SQLITE_UTF8,     0, xyzmFunc },

        { "Length2D",           1, 1, SQLITE_UTF8,     0, geomFunc },
        { "Area2D",             1, 2, SQLITE_UTF8,     0, geomFunc },

        { "GeomFromText",       1, 0, SQLITE_UTF8, 0, GeomFromText },

        { "todate",             1, 1,  SQLITE_UTF8,    0, convFunc },
        { "todouble",           1, 2,  SQLITE_UTF8,    0, convFunc },
        { "tofloat",            1, 3,  SQLITE_UTF8,    0, convFunc },
        { "toint32",            1, 4,  SQLITE_UTF8,    0, convFunc },
        { "toint64",            1, 5,  SQLITE_UTF8,    0, convFunc },
        { "tostring",           1, 6,  SQLITE_UTF8,    0, convFunc },
        { "nullvalue",          2, 1,  SQLITE_UTF8,    0, nullvalueFunc }

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
        { "stddev",      1, 0, 0, varStep,      stdevFinalize  },
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
