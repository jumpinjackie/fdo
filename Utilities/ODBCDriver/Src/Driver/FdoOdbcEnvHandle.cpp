#include "StdHdr.h"
#include "FdoOdbcEnvHandle.h"

FdoOdbcEnvHandle::FdoOdbcEnvHandle(void)
    :m_Handle(NULL)
{
}

FdoOdbcEnvHandle::FdoOdbcEnvHandle(HINSTANCE handle)
    :m_Handle(handle)
{
}

FdoOdbcEnvHandle::~FdoOdbcEnvHandle(void)
{
}

FdoOdbcEnvHandle* FdoOdbcEnvHandle::Create(void)
{
    return new FdoOdbcEnvHandle();
}

FdoOdbcEnvHandle* FdoOdbcEnvHandle::Create(HINSTANCE handle)
{
    return new FdoOdbcEnvHandle(handle);
}

HINSTANCE FdoOdbcEnvHandle::GetHandle()
{
    return m_Handle;
}

void FdoOdbcEnvHandle::SetHandle(HINSTANCE handle)
{
    m_Handle = handle;
}
