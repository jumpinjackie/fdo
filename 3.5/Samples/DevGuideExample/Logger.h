// Logger.h

#ifndef _LOGGER_H_
#define _LOGGER_H_
//
// Copyright (C) 2008  Autodesk, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//

// ostream + fstream = ofstream
// there is no separate ofstream header file
// originally include just <ostream> but changed this to iostream
// once I found that I couldn't use cout with just <ostream>
// ostream + sstream = ostringstream
#include <iostream>
#include <ostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <wchar.h>

// <ctime> is time.h within the std namespace
#include <ctime>

// tests
// to test constructor, set permissions of <filename> to read-only

using namespace std;

class Logger
{
   public:
      Logger(string filename);
      
      ~Logger();

      void write(string entry);
      void write(string label, string entry);
	  void write(const wchar_t* entry);
	  void str(string s);
	  void endline();
	  void timestr();

   private:
      ofstream ofile;
      struct tm * timeinfo;

      size_t stringLength;
      
      static const size_t maxLenDateString = 127;
      char dateString[128];
      const char * dateStringFormat;
      
      static const size_t maxLenTimeString = 127;
      char timeString[128];
      const char * timeStringFormat;

      void getDateTime();
};
   
#endif

