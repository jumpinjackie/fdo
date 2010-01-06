// Logger.cpp
//
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
//

#include "stdafx.h"
#include "Logger.h"

Logger::Logger(string filename)
{
   dateStringFormat = "%A, %B %d, %Y";
   timeStringFormat = "%H:%M:%S";
   ofile.open(filename.c_str(), ios_base::app);
   // failbit set if open operation fails
   if (ofile.fail()) {
//    when ofile.exceptions and ofile.clear are called  (see last 2 comments)  
//    the Microsoft Runtime catches the exeption and displays
//    the following message
//    "The application has requested the Runtime to terminate it in
//    an unusual way. Please contact the application's support team
//    for more information.
//    ofile.exceptions(ios_base::failbit);
//    ofile.clear(ios_base::failbit);
//    so take a different approach
      ostringstream oss(ostringstream::out);
      oss << "Logger constructor failed to open " << filename << endl;
      throw ios_base::failure(oss.str());
   }
   getDateTime();
   stringLength = strftime(dateString, maxLenDateString, dateStringFormat, timeinfo);
   ofile << dateString << endl;
   stringLength = strftime(timeString, maxLenTimeString, timeStringFormat, timeinfo);
   ofile << timeString << ' ' << "Logger instance created" << endl;
}

Logger::~Logger() {
   ofile.close();
}

void
Logger::write(string entry) {
   getDateTime();
   stringLength = strftime(timeString, maxLenTimeString, timeStringFormat, timeinfo);
   ofile << timeString << ' ' << entry << endl;
}
void
Logger::write(string label, string entry) {
   getDateTime();
   stringLength = strftime(timeString, maxLenTimeString, timeStringFormat, timeinfo);
   ofile << timeString << ' ' << label << ' ' << entry << endl;
}

void
Logger::str(string s) {
	ofile << ' ' << s;
}

void
Logger::endline() {
	ofile << endl;;
}

void
Logger::timestr() {
   getDateTime();
   stringLength = strftime(timeString, maxLenTimeString, timeStringFormat, timeinfo);
   ofile << timeString;
}

void
Logger::getDateTime() {
   time_t secondsSinceJan1_1970 = time(NULL);
   timeinfo = localtime ( &secondsSinceJan1_1970 );
}

