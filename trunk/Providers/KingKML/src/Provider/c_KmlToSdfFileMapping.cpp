#include "StdAfx.h"
#include "c_KmlToSdfFileMapping.h"

c_KmlToSdfFileMapping::c_KmlToSdfFileMapping(void)
{
  m_KmlSdfFileDataCollection = new c_KmlSdfFileDataCollection();
}

c_KmlToSdfFileMapping::~c_KmlToSdfFileMapping(void)
{
}

SdfConnection* c_KmlToSdfFileMapping::OpenSdfConnection( const string& KmlFileName )
{
  FdoPtr<c_KmlSdfFileData> filedata;
  
  SdfConnection *sdfconn=NULL;
  
  filedata = m_KmlSdfFileDataCollection->FindKmlSdfFileData(KmlFileName);
  
  if( !filedata.p )
  {
    filedata = new c_KmlSdfFileData();
    
  // generate new temporary file name for sdf which will be used to store kml file
    
  
  // create new sdf file.
    sdfconn = SdfConnection::Create();
    FdoPtr<FdoICreateSDFFile> sdfcom = (FdoICreateSDFFile*)sdfconn->CreateCommand(SdfCommandType_CreateSDFFile);
    sdfcom->SetFileName(filedata->m_SdfFileName);
  }
  
  if(filedata.p)
  {
    // Create SDF for it
    sdfconn = SdfConnection::Create();  
    FdoPtr<FdoIConnectionInfo> conninfo = sdfconn->GetConnectionInfo();
    FdoPtr<FdoIConnectionPropertyDictionary> connprop = conninfo->GetConnectionProperties();

    
    connprop->SetProperty(PROP_NAME_FILE,filedata->m_SdfFileName);
    connprop->SetProperty(PROP_NAME_RDONLY,L"false");

    sdfconn->Open();
  }
  
  return sdfconn;
}//end of c_KmlToSdfFileMapping::OpenSdfConnection