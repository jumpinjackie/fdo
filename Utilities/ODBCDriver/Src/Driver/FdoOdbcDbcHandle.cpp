#include "StdHdr.h"
#include "FdoOdbcDbcHandle.h"

FdoOdbcDbcHandle::FdoOdbcDbcHandle(void)
    :m_Handle(NULL)
{
    FdoPtr<IConnectionManager> manager = FdoFeatureAccessManager::GetConnectionManager ();
    m_Handle = manager->CreateConnection (L"OSGeo.SDF.3.2");
}

FdoOdbcDbcHandle::FdoOdbcDbcHandle(FdoIConnection* handle)
    :m_Handle(handle)
{
}

FdoOdbcDbcHandle::~FdoOdbcDbcHandle(void)
{
}

FdoOdbcDbcHandle* FdoOdbcDbcHandle::Create(void)
{
    return new FdoOdbcDbcHandle();
}

FdoOdbcDbcHandle* FdoOdbcDbcHandle::Create(FdoIConnection* handle)
{
    return new FdoOdbcDbcHandle(handle);
}

FdoIConnection* FdoOdbcDbcHandle::GetHandle()
{
    return m_Handle;
}

void FdoOdbcDbcHandle::SetHandle(FdoIConnection* handle)
{
    m_Handle = handle;
}
