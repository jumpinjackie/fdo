#include "OgrSpatialExtentsDataReader.h"
#include "OgrFdoUtil.h"

OgrSpatialExtentsDataReader::OgrSpatialExtentsDataReader(OGREnvelope* env, FdoStringP name)
    : m_read(0), m_name(name)
{
    //generate FGF polygon and return as refcounted byte array
    double coords[10];
    coords[0] = env->MinX;
    coords[1] = env->MinY;
    coords[2] = env->MaxX;
    coords[3] = env->MinY;
    coords[4] = env->MaxX;
    coords[5] = env->MaxY;
    coords[6] = env->MinX;
    coords[7] = env->MaxY;
    coords[8] = env->MinX;
    coords[9] = env->MinY;

    FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
    FdoPtr<FdoILinearRing> lr = gf->CreateLinearRing(FdoDimensionality_XY, 10, coords);
    FdoPtr<FdoIPolygon> fgfgeom = gf->CreatePolygon(lr, NULL);

    m_envelope = gf->GetFgf(fgfgeom);
}

OgrSpatialExtentsDataReader::~OgrSpatialExtentsDataReader()
{

}

FdoInt32 OgrSpatialExtentsDataReader::GetPropertyCount()
{
    return 1;
}

FdoString* OgrSpatialExtentsDataReader::GetPropertyName(FdoInt32 index)
{
    if (0 == index)
        return m_name;

    throw FdoCommandException::Create(L"Invalid property index");
}

FdoInt32 OgrSpatialExtentsDataReader::GetPropertyIndex(FdoString* propertyName)
{
    if (m_name == propertyName)
    {
        return 0;
    }
    throw FdoCommandException::Create(L"Invalid property name");
}

FdoDataType OgrSpatialExtentsDataReader::GetDataType(FdoString* propertyName)
{
    throw FdoCommandException::Create(L"Not implemented");
}

FdoPropertyType OgrSpatialExtentsDataReader::GetPropertyType(FdoString* propertyName)
{
    if (m_name == propertyName)
    {
        return FdoPropertyType_GeometricProperty;
    }
    throw FdoCommandException::Create(L"Invalid property name");
}

bool OgrSpatialExtentsDataReader::GetBoolean(FdoString* propertyName)
{
    throw FdoCommandException::Create(L"Not implemented");
}

FdoByte OgrSpatialExtentsDataReader::GetByte(FdoString* propertyName)
{
    throw FdoCommandException::Create(L"Not implemented");
}

FdoDateTime OgrSpatialExtentsDataReader::GetDateTime(FdoString* propertyName)
{
    throw FdoCommandException::Create(L"Not implemented");
}

double OgrSpatialExtentsDataReader::GetDouble(FdoString* propertyName)
{
    throw FdoCommandException::Create(L"Not implemented");
}

FdoInt16 OgrSpatialExtentsDataReader::GetInt16(FdoString* propertyName)
{
    throw FdoCommandException::Create(L"Not implemented");
}

FdoInt32 OgrSpatialExtentsDataReader::GetInt32(FdoString* propertyName)
{
    throw FdoCommandException::Create(L"Not implemented");
}

FdoInt64 OgrSpatialExtentsDataReader::GetInt64(FdoString* propertyName)
{
    throw FdoCommandException::Create(L"Not implemented");
}

float OgrSpatialExtentsDataReader::GetSingle(FdoString* propertyName)
{
    throw FdoCommandException::Create(L"Not implemented");
}

FdoString* OgrSpatialExtentsDataReader::GetString(FdoString* propertyName)
{
    throw FdoCommandException::Create(L"Not implemented");
}

FdoLOBValue* OgrSpatialExtentsDataReader::GetLOB(FdoString* propertyName)
{
    throw FdoCommandException::Create(L"Not implemented");
}

FdoIStreamReader* OgrSpatialExtentsDataReader::GetLOBStreamReader(const wchar_t* propertyName )
{
    throw FdoCommandException::Create(L"Not implemented");
}

bool OgrSpatialExtentsDataReader::IsNull(FdoString* propertyName)
{
    if (m_name == propertyName)
    {
        if (m_read == 0)
            throw FdoCommandException::Create(L"Un-initialized reader");
        else if (m_read == 1)
            return false;
        else
            throw FdoCommandException::Create(L"End of reader");
    }
    throw FdoCommandException::Create(L"Not implemented");
}

FdoByteArray* OgrSpatialExtentsDataReader::GetGeometry(FdoString* propertyName)
{
    return FDO_SAFE_ADDREF(m_envelope.p);
}

FdoIRaster* OgrSpatialExtentsDataReader::GetRaster(FdoString* propertyName)
{
    throw FdoCommandException::Create(L"Not implemented");
}

bool OgrSpatialExtentsDataReader::ReadNext()
{
    if (m_read <= 0) {
        m_read++;
        return true;
    }
    m_read++; //Signify end of reader (>1)
    return false;
}

void OgrSpatialExtentsDataReader::Close()
{

}