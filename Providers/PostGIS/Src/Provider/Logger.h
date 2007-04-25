/////////////////////////////////////////////////////////////////////////////
// Author        : Nicholas Tsipanov (nicholas@spbteam.com)
// Modifications : Mateusz Loskot (mateusz@loskot.net)
// Created       : March 2001
// Modified      : February 2007
// Original ver. : 3.71
// Description   : Logging and debug tracing to file 
//
//
/////////////////////////////////////////////////////////////////////////////
// LICENSE
// 
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name is included.
//
// This code can be compiled, modified and distributed freely, providing
// that this copyright information remains intact in the distribution.
//
// This code may be compiled in original or modified form in any private 
// or commercial application.
//
// THIS CODE IS DISTRIBUTED "AS IS".  NO WARRANTY OF ANY
// KIND IS EXPRESSED OR IMPLIED.  YOU USE AT YOUR OWN RISK.
// THE AUTHOR WILL NOT BE LIABLE FOR DATA LOSS, DAMAGES, LOSS
// OF PROFITS OR ANY OTHER KIND OF LOSS WHILE USING OR MISUSING
// THIS SOFTWARE.
//
#ifndef FDOLOGFILE_H_INCLUDED
#define FDOLOGFILE_H_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4311)
#pragma warning(disable:4312)

#include <tchar.h>
#include <wtypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <objbase.h> // StringFromGUID2, 

//  Comment the line below if you don't want logging
#define FDOLOG_DEBUG 1

//  If FDOLOG_CREATE_NEW is defined then every time your program starts it will create 
//  a new log file named <Executable>_log00.txt, <Executable>_log01.txt, etc. Otherwise it will 
//  overwrite the old <Executable>_log.txt file.
//#define FDOLOG_CREATE_NEW 1

//Every time the program starts the program will delete the previous file if it uses the 
// single log (sometime it is not needed, for example you want to have all logs of all program runs)
#define FDOLOG_DELETE_OLD 1

//  By default the logfile is created in the root folder, but you 
// may want to create a log in the directory where the executable is located
#define FDOLOG_CREATE_FILE_IN_THE_SAME_DIRECTORY 1

//  Undef this if you don't want compile time info (file name and line number 
//  where the logging has been called) included in the log
//#define FDOLOG_PRINT_COMPILE_INFO 1

//override standard TRACE macro behaviour
#define FDOLOG_USE_FOR_TRACE

//You can specify your log file name if you want to. In most cases 
// it can be done automatically : Logfile determines where the executable is located
// and creates an <executable>_log.txt file, but if you want a custom file, then 
//unocomment the #define below and somewhere in you .cpp files (stdafx.cpp most appropriate)
// add the line  LPCTSTR cszLogFileName = _T("C:\\LogFile");
//#define FDOLOG_FILENAME extern LPCTSTR cszLogFileName;

#define FDOLOG_MAXCOUNTERS 30

// This definition controls if logger should write quad part or milliseconds.
//#define FDOLOG_USE_PERFORMANCE_COUNTER

// This definition controls multithreading support
#define FDOLOG_MULTITHREADING

/*USAGE::
#include this file into any header that will be included in all files.
For VC projects using precompiled headers the ideal place would be stdafx.h
You can either insert this file into the project or not, in the latter case 
the log classes won't litter your project workspace. This won't affect
usage of logs.

The primary usage of this code is to have a simple way to output debugging
information into the log file, which could be easiliy turned off or on during 
the compile time.
Example :

FDOLOG_WRITE(_T("saving %d transactions"), nTransactions); 

Another STLogFile feature, marker: 

void MyFunction()
{
	FDOLOG_MARKER(_T("MyFunction"));
	if (somethingbad())
		return FALSE;
	if (somethinggood())
		return TRUE;

	return FALSE; 
}

If you want to output some binary data (buffer contents) you should use a FDOLOG_WRITE_DATA macro:

  {
  ...
  char buffer[125];
  // .. fill the buffer with data 

  FDOLOG_WRITE_DATA(buffer, 125);
  }

This code will write a >>[MyFunction] line on entering the function and <<[MyFunction] 
on exit wherever the program leaves your function.

To find out the problem of the wrong function execution simply add FDOLOG_LASTERROR:

  HANDLE hFile = CreateFile(....);
  if (INVALID_HANDLE == hFile)
  {
	FDOLOG_WRITE("Cannot open file");
	FDOLOG_LASTERROR;
  }
This will print the last textual description of error returned by GetLastError() funciton.

And .. profiling. Before describing log file profiling features it would be appropriate 
to say that all measured timing includes the time for file operation, which slightly 
increases the execution time, therefore this method could only help to *estimate* and 
compare execution times to find bottlenecks of your code.


  ....
  FDOLOG_PROFILE_FUNCTION(Calculate());
  ..
This code will estimate time needed for function execution. Simple. When the 
logging is turned off this will be transformed to straight call to Calculate().


  {
	FDOLOG_PROFILE_BLOCK;
	.. code here ...

  }
  This sample of code will print to log file the time when the execution entered the
  profiled block, when the execution leaves it and the time interval between the two points.
  When your program finishes, this macro will print profiling statistics, like how many 
  times this code has been executed, how much time did it take in total, in average, 
  maximium and minimum timings.


  Sometime you want to have intermediate timings and STLogFile has a solution for this case:
  {
	FDOLOG_PROFILE_BLOCK;
	Phase0();
	FDOLOG_PROFILE_CHECKPOINT
	Phase1();
	FDOLOG_PROFILE_CHECKPOINT
	Phase2();
  }
  This will print timings between the check points and the time from beginning of the block 
  every time execution reaches any check point.

There are some options, which can be used for fine tuning like where to create log file,
how to create it and others. 




*/

//---------------  Don't touch anything below this line. 
//---------------  Of all changes and improvements please notify authors via email. 

#ifdef FDOLOG_DEBUG

	#define FDOLOG_MAXINDENT 100

	#ifdef FDOLOG_PRINT_COMPILE_INFO
		static CHAR *CURRENT_FILE;
		static DWORD CURRENT_LINE;
	#endif

	#ifdef FDOLOG_FILENAME
		FDOLOG_FILENAME;
	#endif

	static int ___g_nCounterIndex___ = 0;

	class CSTLogFile  
	{
	public:

		static CSTLogFile *GetLogFile() 
		{
			static CSTLogFile LogFile;
			if (!LogFile.m_bIsStarted) 
				LogFile.Start();
			return &LogFile;
		}

		inline void Write0(LPCSTR szEntry)
		{
			::EnterCriticalSection(&m_crit);
			BOOL bOk = TRUE;
			char szTimeString[400];
			bOk = GetTimeString(szTimeString, sizeof(szTimeString));
			char buffer[1024];
			char szIndent[FDOLOG_MAXINDENT];
			memset(szIndent, ' ', sizeof(szIndent))	;

			int nIndent = reinterpret_cast<int>(TlsGetValue(m_dwTLSIndex));
			*(szIndent + nIndent) = 0;
#ifdef FDOLOG_MULTITHREADING
#define	FDOLOG_MULTITHREADING_STUB1 "(0x%08X)"
#define	FDOLOG_MULTITHREADING_STUB2 GetCurrentThreadId(),
#else
#define	FDOLOG_MULTITHREADING_STUB1 
#define	FDOLOG_MULTITHREADING_STUB2 

#endif
#ifdef FDOLOG_PRINT_COMPILE_INFO
			int nBytes = _snprintf(buffer, sizeof(buffer), "%s "FDOLOG_MULTITHREADING_STUB1" : %s%s\t(from %s,%d)\r\n", szTimeString, FDOLOG_MULTITHREADING_STUB2 szIndent, szEntry, CURRENT_FILE, CURRENT_LINE);
#else
            // XXX: mloskot - thread info customizations
			//int nBytes = _snprintf(buffer, sizeof(buffer), "%s,"FDOLOG_MULTITHREADING_STUB1" : %s%s\r\n", szTimeString, FDOLOG_MULTITHREADING_STUB2 szIndent, szEntry);

            int nBytes = _snprintf(buffer, sizeof(buffer), "%s "FDOLOG_MULTITHREADING_STUB1" : %s%s\r\n", szTimeString, FDOLOG_MULTITHREADING_STUB2 szIndent, szEntry);
#endif
			bOk = (nBytes > 0);
			if (!bOk) goto exit_function;
			DWORD dwWrittenBytes;
			::SetFilePointer(m_hFile, 0, 0, FILE_END);
			::WriteFile(m_hFile, buffer, nBytes, &dwWrittenBytes, NULL);
			bOk = (dwWrittenBytes != (DWORD)nBytes);
			if (!bOk) goto exit_function;
			::FlushFileBuffers(m_hFile);
		exit_function:
			::LeaveCriticalSection(&m_crit);
		}

		void WriteData(LPCVOID pBytes, DWORD dwSize) 
		{
			Write("***Binary data (%d bytes)",  dwSize);
			DWORD dwWrittenBytes;
			::SetFilePointer(m_hFile, 0, 0, FILE_END);
			::WriteFile(m_hFile, pBytes, dwSize, &dwWrittenBytes, NULL);
			::WriteFile(m_hFile, "\r\n", 2, &dwWrittenBytes, NULL);
			Write("***End binary data (written %d bytes)", dwWrittenBytes);
		}

		inline void Write(LPCSTR szEntry, ...)
		{
			BOOL bOk = TRUE;
			if (m_bIsStarted) 
			{
				char buffer[1024];
				va_list args;
				va_start(args, szEntry);
				bOk = (-1 != _vsnprintf(buffer, sizeof(buffer), szEntry, args));
				va_end(args);
				if (!bOk) return;
				Write0(buffer);
			}
		}

		inline void Write(LPCWSTR szEntry, ...) 
		{
			BOOL bOk = TRUE;
			if (m_bIsStarted) 
			{
				WCHAR buffer[1024];
				va_list args;
				va_start(args, szEntry);
				int nSize = _vsnwprintf(buffer, sizeof(buffer) / sizeof(WCHAR), szEntry, args);
				bOk = (-1 != nSize);
				va_end(args);
				if (!bOk) return;
				char *pAnsiString = new char[nSize+1];
				wcstombs(pAnsiString, buffer, nSize+1);
				Write0(pAnsiString);
				delete pAnsiString;
			}
		}

		void WriteGUID(REFGUID rguid)
		{
			OLECHAR szBuffer[40];
			StringFromGUID2(rguid, szBuffer, 40);
			Write(_T("GUID: %s"), szBuffer);

		}

		void WriteCLSID(REFCLSID rclsid)
		{
			LPOLESTR lpszBuffer;
			StringFromCLSID(rclsid, &lpszBuffer);
			Write(_T("CLSID: %s"), lpszBuffer);
			CoTaskMemFree(lpszBuffer);
		}

		void WriteIID(REFIID  rclsid)
		{
			LPOLESTR lpszBuffer;
			StringFromIID(rclsid, &lpszBuffer);
			Write(_T("CLSID: %s"), lpszBuffer);
			CoTaskMemFree(lpszBuffer);
		}

		void WriteLastError() 
		{ 
			DWORD dwError = GetLastError(); 
			LPVOID lpMsgBuf;                 
			if (!FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL, dwError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL ))
			{
				Write(_T("GetLastError returned : %d (no further information)"), dwError);
			} else {
				TCHAR *ret;
				while (ret = _tcsrchr((LPTSTR)lpMsgBuf, _T('\n'))) *ret = _T(' ');
				while (ret = _tcsrchr((LPTSTR)lpMsgBuf, _T('\r'))) *ret = _T(' ');
				CSTLogFile::GetLogFile()->Write(_T("GetLastError returned : 0x%08x: %s)"), dwError, lpMsgBuf);
				LocalFree( lpMsgBuf ); 
			}
		}

		void Start() 
		{
			TlsSetValue(m_dwTLSIndex, 0);

#ifdef FDOLOG_FILENAME
			Start(cszLogFileName);
#else
			TCHAR wszFileName[MAX_PATH];
			GetLogFileName(wszFileName);
			Start(wszFileName);
#endif
		}

		void Start(LPCTSTR szFilePath) 
		{
			if (m_bIsStarted) return;
			::EnterCriticalSection(&m_crit);
#ifdef FDOLOG_DELETE_OLD
			::DeleteFile(szFilePath);
#endif
			m_hFile = ::CreateFile(szFilePath, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, 
							OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
			HRESULT hr = GetLastError();
			if (m_hFile != INVALID_HANDLE_VALUE)
			{
				::SetFilePointer(m_hFile, 0, 0, FILE_END);
				m_bIsStarted = TRUE;
				TCHAR szExecutable[MAX_PATH];
				GetModuleFileName(NULL, szExecutable, MAX_PATH);
				DWORD dwProcID = GetCurrentProcessId();
				SYSTEMTIME st;
				GetLocalTime(&st);
				Write(TEXT("==========================================================\n")
				      TEXT("\tLog started on %02u.%02u.%04u, at %02u:%02u:%02u:%03u\n")
                      TEXT("\tModule: %s (ProcID: 0x%08x)\n")
                      TEXT("\tCompile time: %s %s"),
                      st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds,
                      szExecutable, dwProcID,
                      TEXT(__DATE__) , TEXT(__TIME__));
                Write(TEXT("=========================================================="));
			}
			::LeaveCriticalSection(&m_crit);
		}

		void Stop() 
		{
			::EnterCriticalSection(&m_crit);
			DeleteStaticCounters();
			if (m_bIsStarted) {
				Write(TEXT("Log finished"));
				::CloseHandle(m_hFile);
				m_bIsStarted = FALSE;
			}
			::LeaveCriticalSection(&m_crit);
		}

	protected:

		CSTLogFile() 
			: m_bIsStarted(FALSE), nLastCounter(0)
		{
			::InitializeCriticalSection(&m_crit);
			m_dwTLSIndex = TlsAlloc ();
#ifdef FDOLOG_USE_PERFORMANCE_COUNTER
			::QueryPerformanceFrequency(&m_nPerfFreq);
			::QueryPerformanceCounter(&m_nStartTime);
#endif 
		}

	public:

		virtual ~CSTLogFile() 
		{
			if (m_bIsStarted)
				Stop();
			::DeleteCriticalSection(&m_crit);
			TlsFree(m_dwTLSIndex);
		}

	private:

		HANDLE m_hFile;
		CRITICAL_SECTION m_crit;
		BOOL m_bIsStarted;


		DWORD m_dwTLSIndex;

	#ifdef FDOLOG_USE_PERFORMANCE_COUNTER
		LARGE_INTEGER m_nPerfFreq;
		LARGE_INTEGER m_nStartTime;
	#endif

		BOOL GetTimeString(LPSTR szString, int nStringSize) 
		{
			SYSTEMTIME st;
			GetLocalTime(&st);
#ifdef FDOLOG_USE_PERFORMANCE_COUNTER
			LARGE_INTEGER counter;
			QueryPerformanceCounter(&counter);
			return (0 < _snprintf(szString, nStringSize, "%02u:%02u:%02u(%I64d)", st.wHour, st.wMinute, st.wSecond, counter.QuadPart));
#else
			return (0 < _snprintf(szString, nStringSize, "%02u:%02u:%02u:%03u", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds));
#endif
		}

	public:

		inline DWORD GetIndent()
		{
			return reinterpret_cast<DWORD>(TlsGetValue(m_dwTLSIndex));
		}

		inline void SetIndent(DWORD dwIndent)
		{
			TlsSetValue(m_dwTLSIndex, reinterpret_cast<LPVOID>(dwIndent));
		}

		inline void IncrIndent()
		{
			DWORD dwIndent = reinterpret_cast<DWORD>(TlsGetValue(m_dwTLSIndex));
			dwIndent++;
			TlsSetValue(m_dwTLSIndex, reinterpret_cast<LPVOID>(dwIndent));
		}

		inline void DecrIndent()
		{
			DWORD dwIndent = reinterpret_cast<DWORD>(TlsGetValue(m_dwTLSIndex));
			dwIndent--;
			TlsSetValue(m_dwTLSIndex, reinterpret_cast<LPVOID>(dwIndent));
		}

	private:

		void GetLogFileName(LPTSTR szFileName)
        {
			TCHAR wszExecutableFilePath[MAX_PATH];
			::GetModuleFileName(NULL, wszExecutableFilePath, MAX_PATH);
#ifdef FDOLOG_CREATE_FILE_IN_THE_SAME_DIRECTORY
			TCHAR *wszExecutableFileName = wszExecutableFilePath;
#else
			TCHAR *wszExecutableFileName = ::_tcsrchr(wszExecutableFilePath, _T('\\'));
#endif
			TCHAR *wszLastDot = ::_tcsrchr(wszExecutableFileName, _T('.'));

#ifdef FDOLOG_CREATE_NEW
			BOOL bFound = FALSE;
			int nFreeNumber = 0;
			TCHAR wszTemp[MAX_PATH];

			while (!bFound) {

				_stprintf(wszTemp, _T("_fdopostgis_%02d.log"), nFreeNumber);
				::_tcscpy(wszLastDot, wszTemp);
				if (0xFFFFFFFF == ::GetFileAttributes(wszExecutableFileName))
				{
					bFound = TRUE;
				} else {
					nFreeNumber++;
				}
			}
#else 
			::_tcscpy(wszLastDot, _T("_fdopostgis.log"));
#endif 
			::_tcscpy(szFileName, wszExecutableFileName);
		}

	public:
		
		class Counter
        {
		public:

			Counter(char *szFile, int nLine) : m_nCounter(0), m_szFile(szFile), m_nLine(nLine)
			{
				m_nCounterIndex = ___g_nCounterIndex___++;
				::InitializeCriticalSection(&m_crit);
				CSTLogFile::GetLogFile()->Write("***Counter %d initialized at %s,%d", m_nCounterIndex, szFile, nLine);
				m_TotalTime.QuadPart = 0;
			}

			~Counter()	
			{
				CSTLogFile::GetLogFile()->Write("Counter %d statistics\r\n\tCounts: %d,\r\n\tMinTime: %I64d ms,\r\n\tMaxTime: %I64d ms,\r\n\tTotalTime: %I64d ms,\r\n\tAverageTime: %I64d ms", 
					m_nCounterIndex, m_nCounter, m_MinTime.QuadPart, m_MaxTime.QuadPart, m_TotalTime.QuadPart, m_TotalTime.QuadPart / m_nCounter); 
				::DeleteCriticalSection(&m_crit);
			}

			inline void StartSection() 
			{	
				::EnterCriticalSection(&m_crit);
				m_nCounter++;
				CSTLogFile::GetLogFile()->Write("Counter %d started", m_nCounterIndex); 
				::QueryPerformanceCounter(&m_StartTime);
				m_LastCheckPoint.QuadPart = m_StartTime.QuadPart;
				::LeaveCriticalSection(&m_crit);
			}

			inline void StopSection() 
			{
				::EnterCriticalSection(&m_crit);
				LARGE_INTEGER liCurrentTime;
				::QueryPerformanceCounter(&liCurrentTime);
                CSTLogFile::GetLogFile()->Write("Counter %d stopped\r\n\tFrom start: %I64d ms\r\n\tFrom last checkpoint: %I64d ms", m_nCounterIndex,  liCurrentTime.QuadPart - m_StartTime.QuadPart, liCurrentTime.QuadPart - m_LastCheckPoint.QuadPart); 
				liCurrentTime.QuadPart -= m_StartTime.QuadPart;
				if (m_nCounter == 1 || m_MinTime.QuadPart > liCurrentTime.QuadPart) m_MinTime.QuadPart = liCurrentTime.QuadPart;
				if (m_nCounter == 1 || m_MaxTime.QuadPart < liCurrentTime.QuadPart) m_MaxTime.QuadPart = liCurrentTime.QuadPart;
				m_TotalTime.QuadPart += liCurrentTime.QuadPart;
				::LeaveCriticalSection(&m_crit);
			}

			inline void CheckPoint(int nLine) 
			{
				::EnterCriticalSection(&m_crit);
				LARGE_INTEGER liCheckPoint; ::QueryPerformanceCounter(&liCheckPoint);
                CSTLogFile::GetLogFile()->Write("Counter %d check point at line %d\r\n\tFrom start: %I64d ms\r\n\tFrom last checkpoint: %I64d ms", m_nCounterIndex,  nLine, liCheckPoint.QuadPart - m_StartTime.QuadPart, liCheckPoint.QuadPart - m_LastCheckPoint.QuadPart); 
				m_LastCheckPoint.QuadPart = liCheckPoint.QuadPart;
				::LeaveCriticalSection(&m_crit);
			}

			inline LARGE_INTEGER GetTimeFromStart() 
			{
				LARGE_INTEGER liCurrentTime;
				::QueryPerformanceCounter(&liCurrentTime);
				liCurrentTime.QuadPart -= m_StartTime.QuadPart;
				return liCurrentTime;
			}

		private:

			LARGE_INTEGER m_StartTime;
			LARGE_INTEGER m_MaxTime;
			LARGE_INTEGER m_MinTime;
			LARGE_INTEGER m_TotalTime;
			LARGE_INTEGER m_LastCheckPoint;
			int m_nCounter;
			CRITICAL_SECTION m_crit;
			friend class CounterAux;
			char *m_szFile;
			int m_nLine;
			int m_nCounterIndex ;
		
        }; // class Counter

		struct CounterAux
		{
			CounterAux(Counter *pCounter) : m_pCounter(pCounter) { if (pCounter) pCounter->StartSection();}
			~CounterAux() { if (m_pCounter) m_pCounter->StopSection();}
			Counter *m_pCounter;
		};

		Counter *m_counters[FDOLOG_MAXCOUNTERS];
		int nLastCounter;

		static Counter *GetStaticCounter(char *szFile, int nLine) 
		{
			CSTLogFile* pLogFile = GetLogFile();
			if (pLogFile->nLastCounter == FDOLOG_MAXCOUNTERS)
			{
				pLogFile->Write("*****Max counters (%d) reached\r\n\tCounter at %s, %d will not be created", FDOLOG_MAXCOUNTERS, szFile, nLine);
				return 0;
			}

			Counter *p = new Counter(szFile, nLine);
			pLogFile->m_counters[pLogFile->nLastCounter++] = p;
			return p;
		}

		void DeleteStaticCounters()
		{
			while (nLastCounter)
			{
				delete m_counters[--nLastCounter];
			}
		}

		struct Marker
		{
            Marker(LPCSTR szEntry)
                : m_szEntry(szEntry), m_szEntryW(NULL), m_bWide(FALSE)
			{
				CSTLogFile::GetLogFile()->Write(">>[%s]", m_szEntry);	
				if (CSTLogFile::GetLogFile()->GetIndent() < FDOLOG_MAXINDENT)
					CSTLogFile::GetLogFile()->IncrIndent();
			}

			Marker(LPCWSTR szEntry)
                : m_szEntry(NULL), m_szEntryW(NULL), m_bWide(TRUE)
			{
				CSTLogFile::GetLogFile()->Write(L">>[%s]", m_szEntryW);
				if (CSTLogFile::GetLogFile()->GetIndent() < FDOLOG_MAXINDENT)
					CSTLogFile::GetLogFile()->IncrIndent();
			}

			~Marker()
			{
				if (CSTLogFile::GetLogFile()->GetIndent() > 0)
					CSTLogFile::GetLogFile()->DecrIndent();

				if (m_bWide)
					CSTLogFile::GetLogFile()->Write(_T("<<[%s]"), m_szEntryW);
				else
					CSTLogFile::GetLogFile()->Write("<<[%s]", m_szEntry);
			}

		private:

			LPCSTR m_szEntry;
            LPCWSTR m_szEntryW;
			BOOL m_bWide;
		
        }; // struct Marker

	}; // class CSTLogFile

#endif

#ifdef FDOLOG_DEBUG 
	extern CSTLogFile logfile;

	#ifdef FDOLOG_PRINT_COMPILE_INFO
	#define FDOLOG_EXPAND_INFO(file, line) \
		CURRENT_FILE = file;	CURRENT_LINE=line;
	#else
		inline void _______DoNothing0() {};
		#define FDOLOG_EXPAND_INFO(file, line) \
			1 ? void(0) : _______DoNothing0();
	#endif

	#define FDOLOG_WRITE \
		FDOLOG_EXPAND_INFO(__FILE__, __LINE__)\
		CSTLogFile::GetLogFile()->Write

	#define FDOLOG_WRITE_DATA	\
		FDOLOG_EXPAND_INFO(__FILE__, __LINE__)\
		CSTLogFile::GetLogFile()->WriteData

	#define	FDOLOG_WRITE_GUID \
		FDOLOG_EXPAND_INFO(__FILE__, __LINE__) \
		CSTLogFile::GetLogFile()->WriteGUID

	#define	FDOLOG_WRITE_CLSID\
		FDOLOG_EXPAND_INFO(__FILE__, __LINE__) \
		CSTLogFile::GetLogFile()->WriteCLSID

	#define	FDOLOG_WRITE_IID\
		FDOLOG_EXPAND_INFO(__FILE__, __LINE__) \
		CSTLogFile::GetLogFile()->WriteIID


	#define FDOLOG_PROFILE_BLOCK \
		FDOLOG_EXPAND_INFO(__FILE__, __LINE__)\
		static CSTLogFile::Counter *___pCounter___  = CSTLogFile::GetStaticCounter( __FILE__, __LINE__); \
		CSTLogFile::CounterAux __counter_aux__(___pCounter___);  

	#define FDOLOG_PROFILE_FUNCTION(function_name)	\
	FDOLOG_EXPAND_INFO(__FILE__, __LINE__)\
	{   CSTLogFile *pLog = CSTLogFile::GetLogFile(); \
		pLog->Write(_T("Estimating call to function %s"), _T(#function_name));  \
		static CSTLogFile::Counter *___pCounter___  = CSTLogFile::GetStaticCounter( __FILE__, __LINE__); \
		CSTLogFile::CounterAux __counter_aux__(___pCounter___);  \
		((void)(function_name)); \
		pLog->Write(_T("Function %s finished in %I64d ms"), _T(#function_name), ___pCounter___->GetTimeFromStart());  \
	}

	#define  FDOLOG_PROFILE_CHECKPOINT \
		if (___pCounter___) ___pCounter___->CheckPoint(__LINE__);

	#define FDOLOG_MARKER \
		FDOLOG_EXPAND_INFO(__FILE__, __LINE__)\
		CSTLogFile::Marker ____marker____

	#define FDOLOG_LASTERROR \
		FDOLOG_EXPAND_INFO(__FILE__, __LINE__)\
		CSTLogFile::GetLogFile()->WriteLastError();


	#ifdef FDOLOG_USE_FOR_TRACE
	#ifdef TRACE 
	#undef TRACE
	#undef TRACE0
	#undef TRACE1
	#undef TRACE2
	#undef TRACE3
	#endif
	#define TRACE FDOLOG_WRITE
	#define TRACE0 FDOLOG_WRITE
	#define TRACE1 FDOLOG_WRITE
	#define TRACE2 FDOLOG_WRITE
	#define TRACE3 FDOLOG_WRITE
	#endif

#else 
	inline void _______DoNothing( ...) {};
	#define FDOLOG_WRITE 1? void(0) : _______DoNothing
	#define FDOLOG_WRITE_DATA 1? void(0) : _______DoNothing
	#define FDOLOG_PROFILE_FUNCTION(function_name) ((void)(function_name));
	#define FDOLOG_PROFILE_CHECKPOINT ;
	#define FDOLOG_PROFILE_BLOCK ;
	#define FDOLOG_MARKER 1? void(0) : _______DoNothing
	#define FDOLOG_LASTERROR ;
	#define FDOLOG_WRITE_GUID 1? void(0) : _______DoNothing
	#define FDOLOG_WRITE_CLSID 1? void(0) : _______DoNothing
	#define FDOLOG_WRITE_IID 1? void(0) : _______DoNothing
#endif

//For backward compatibility
#define WRITE_LOG FDOLOG_WRITE

//Some like it quick
#define FDOLOG FDOLOG_WRITE

#pragma warning(default:4311)
#pragma warning(default:4312)

#endif // FDOLOGFILE_H_INCLUDED
