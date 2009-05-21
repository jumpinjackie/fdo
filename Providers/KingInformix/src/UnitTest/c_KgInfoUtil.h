#ifndef c_KgInfoUtil_h
#define c_KgInfoUtil_h

class c_KgInfoUtil
{
public:
  c_KgInfoUtil(void);
public:
  ~c_KgInfoUtil(void);
  
  static FdoIConnection* OpenUnitTestConnection();
  static FdoIConnection* OpenPerfomanceTestConnection();
};

#endif