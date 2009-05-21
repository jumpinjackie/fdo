/*
* Copyright (C) 2008  SL-King d.o.o
* 
* This library is free software; you can redistribute it and/or
* modify it under the terms of version 2.1 of the GNU Lesser
* General Public License as published by the Free Software Foundation.
* 
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
* 
* You should have received a copy of the GNU Lesser General Public
* License along with this library; if not, write to the Free Software
* Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/


#ifndef _c_KmlSdfFileData_h
#define _c_KmlSdfFileData_h

class c_KmlSdfFileData : public FdoIDisposable
{
public:
  c_KmlSdfFileData(void);
public:
  ~c_KmlSdfFileData(void);
  
  void Dispose ();
  
public:
  std::string m_KmlFileName;  
  FdoStringP m_SdfFileName;  
};

class  c_KmlSdfFileDataCollection : public FdoCollection<c_KmlSdfFileData,FdoException>
{

protected:
  virtual void Dispose()
  {
    delete this;
  } 

public:

  c_KmlSdfFileData* FindKmlSdfFileData(const std::string& KmlFileName)
  {
    FdoPtr<c_KmlSdfFileData> fdoplace;
    int count = GetCount();
    for(int ind=0;ind<count;ind++)
    {
      fdoplace = GetItem(ind)  ;
      if( fdoplace ->m_KmlFileName == KmlFileName )
      {
        return FDO_SAFE_ADDREF(fdoplace.p);
      }
    }       

    return NULL;
  }

};

#endif