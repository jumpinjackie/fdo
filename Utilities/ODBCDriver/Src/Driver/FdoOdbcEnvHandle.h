#pragma once

class FdoOdbcEnvHandle: public FdoDisposable
{
private:
    FdoOdbcEnvHandle(void);
    FdoOdbcEnvHandle(HINSTANCE handle);
    virtual ~FdoOdbcEnvHandle(void);

public:
    static FdoOdbcEnvHandle* Create(void);
    static FdoOdbcEnvHandle* Create(HINSTANCE handle);

public:
    HINSTANCE GetHandle();
    void  SetHandle(HINSTANCE handle);

private:
    HINSTANCE m_Handle;
};
