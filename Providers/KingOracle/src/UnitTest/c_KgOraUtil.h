#ifndef _c_KgOraUtil_h
#define _c_KgOraUtil_h

class c_KgOraUtil
{
public:
  c_KgOraUtil(void);
public:
  ~c_KgOraUtil(void);
  
  static FdoIConnection* OpenUnitTestConnection_10_2();
  static FdoIConnection* OpenUnitTestConnection_9_2();
  static FdoIConnection* OpenPerfomanceTestConnection();
  
  static void RecreateTable(FdoIConnection* Connection,const wchar_t*TableName);
  static FdoIConnection* OpentConnection(wchar_t* ConnString);
  static FdoIConnection* OpenNanaimoConnection_10_2();
  static FdoIConnection* OpenUnitTestConn_10_2_SDE();
};

#endif