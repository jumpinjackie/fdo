#ifndef OGR_SPATIAL_EXTENTS_DATA_READER_H
#define OGR_SPATIAL_EXTENTS_DATA_READER_H

#include "stdafx.h"
#include <ogrsf_frmts.h>
#include "Fdo.h"
#include "FdoDefaultDataReader.h"

//A specialized FdoIDataReader for SpatialExtent() results
class OgrSpatialExtentsDataReader : public FdoDefaultDataReader
{
public:
    OgrSpatialExtentsDataReader(OGREnvelope* env, FdoStringP name);
    ~OgrSpatialExtentsDataReader();

    OGR_API virtual FdoInt32 GetPropertyCount();
    OGR_API virtual FdoString* GetPropertyName(FdoInt32 index);
    OGR_API virtual FdoInt32 GetPropertyIndex(FdoString* propertyName);
    OGR_API virtual FdoDataType GetDataType(FdoString* propertyName);
    OGR_API virtual FdoPropertyType GetPropertyType(FdoString* propertyName);
    OGR_API virtual bool GetBoolean(FdoString* propertyName);
    OGR_API virtual FdoByte GetByte(FdoString* propertyName);
    OGR_API virtual FdoDateTime GetDateTime(FdoString* propertyName);
    OGR_API virtual double GetDouble(FdoString* propertyName);
    OGR_API virtual FdoInt16 GetInt16(FdoString* propertyName);
    OGR_API virtual FdoInt32 GetInt32(FdoString* propertyName);
    OGR_API virtual FdoInt64 GetInt64(FdoString* propertyName);
    OGR_API virtual float GetSingle(FdoString* propertyName);
    OGR_API virtual FdoString* GetString(FdoString* propertyName);
    OGR_API virtual FdoLOBValue* GetLOB(FdoString* propertyName);
    OGR_API virtual FdoIStreamReader* GetLOBStreamReader(const wchar_t* propertyName );
    OGR_API virtual bool IsNull(FdoString* propertyName);
    OGR_API virtual FdoByteArray* GetGeometry(FdoString* propertyName);
    OGR_API virtual FdoIRaster* GetRaster(FdoString* propertyName);
    OGR_API virtual bool ReadNext();
    OGR_API virtual void Close();

protected:
    virtual void Dispose() { delete this; }

private:
    FdoInt32 m_read;
    FdoStringP m_name;
    FdoPtr<FdoByteArray> m_envelope;
};

#endif //OGR_SPATIAL_EXTENTS_DATA_READER_H