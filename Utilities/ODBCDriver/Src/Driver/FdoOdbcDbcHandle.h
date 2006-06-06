#pragma once

class FdoOdbcDbcHandle : public FdoDisposable
{
private:
    FdoOdbcDbcHandle(void);
    FdoOdbcDbcHandle(FdoIConnection* handle);
    virtual ~FdoOdbcDbcHandle(void);

public:
    static FdoOdbcDbcHandle* Create(void);
    static FdoOdbcDbcHandle* Create(FdoIConnection* handle);

public:
    FdoIConnection* GetHandle();
    void SetHandle(FdoIConnection* handle);

private:
    FdoIConnection* m_Handle;
};
