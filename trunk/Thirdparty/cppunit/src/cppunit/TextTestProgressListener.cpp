#include <cppunit/TestFailure.h>
#include <cppunit/TextTestProgressListener.h>
#include <iostream>

// CUSTOMIZED CODE - BEGIN:
#include <cppunit/Test.h>
#ifdef _WIN32
#include "mbstring.h" // for multibyte functions
#include <time.h>
#else //_WIN32
#include <wctype.h>  // for wide-char functions
#include <inttypes.h>
#include <unistd.h>
#include <termios.h>
#endif
// CUSTOMIZED CODE - END.

namespace CppUnit
{


TextTestProgressListener::TextTestProgressListener()
{
}


TextTestProgressListener::~TextTestProgressListener()
{
}


void 
TextTestProgressListener::startTest( Test *test )
{
  // CUSTOMIZED CODE - BEGIN:
  struct tm systime;
#ifdef _WIN32
#pragma warning(disable : 4996)
  _getsystime (&systime);
#pragma warning(default : 4996)
#else
  time_t current;
  time (&current);
  localtime_r (&current, &systime);
#endif
  char datetimestr[100];
  strftime(datetimestr, 100, "%c", &systime);

  std::cerr << "\n" << test->getName() << " (" << datetimestr << "):\n";
  // CUSTOMIZED CODE - END.

  std::cerr.flush();
}


void 
TextTestProgressListener::addFailure( const TestFailure &failure )
{
  std::cerr << ( failure.isError() ? "E" : "F" );
  std::cerr.flush();
}


void 
TextTestProgressListener::done()
{
  std::cerr  <<  std::endl;
  std::cerr.flush();
}

} //  namespace CppUnit

