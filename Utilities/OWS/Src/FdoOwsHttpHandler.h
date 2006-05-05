/*
 * Copyright (C) 2004-2006  Autodesk, Inc.
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
 *
 */

#ifndef FDOOWSHTTPHANDLER_H
#define FDOOWSHTTPHANDLER_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#pragma warning(push)
#pragma warning(disable: 4275)  // Disable warning C4275: non dll-interface class
#pragma warning(disable: 4251)  // Disable warning C4251: needs to have dll-interface to be used by clients of class
#include <boost/thread/thread.hpp>
#include <boost/thread/condition.hpp>
#pragma warning(pop) // Enable warning C4275: non dll-interface class

#include <curl/curl.h>
#include <exception>
#include <vector>
#include <OWS/FdoOwsIHttpHandler.h>

#define CURL_TRUE   1
#define CURL_FALSE  0

class FdoOwsHttpHandler : public FdoOwsIHttpHandler
{
private:
    enum ConnectionState
    {
        ConnectionState_BeforeConnect,
        ConnectionState_Connected,
        ConnectionState_Terminated,
    };

    ConnectionState m_connectionState;
    bool m_disposed;
    bool m_bRunning;

    std::string m_url;
    bool m_bGet;
    std::string m_parameters;
    std::string m_userName;
    std::string m_passwd;

    // thread related variables
    std::auto_ptr<boost::thread> m_thread;
    boost::mutex m_mutex;
    boost::condition m_condition;

    //error related
    char m_errorBuffer[CURL_ERROR_SIZE];

    bool m_bValidDocument;
    FdoOwsMIMEType m_contentType;

    // http content related
    std::vector<char*> m_contents;
    std::vector<size_t> m_contentSizes;
    size_t m_currentSize;

    size_t m_currentRead;

    //support timeout
    unsigned int m_tvConnect;

protected:
    FdoOwsHttpHandler();
    FdoOwsHttpHandler(const char* url, bool bGet, const char* parameters,
        const char* userName, const char* passwd);
    virtual ~FdoOwsHttpHandler();
    virtual void Dispose();

public:
    static FdoOwsHttpHandler* Create(const char* url, bool bGet, const char* parameters,
        const char* userName, const char* passwd);

    void Perform();

    FdoOwsMIMEType GetMIMEType()
    {
        return m_contentType;
    }
    void Proc();

    // FdoIoStream implementation
    virtual FdoSize Read( FdoByte* buffer, FdoSize count );
    virtual void Write( FdoByte* buffer, FdoSize count );
    virtual void Write( FdoIoStream* stream, FdoSize count = 0 );
    virtual void SetLength( FdoInt64 length );
    virtual FdoInt64 GetLength();
    virtual FdoInt64 GetIndex();
    virtual void Skip( FdoInt64 offset );
    virtual void Reset();

    virtual FdoBoolean CanRead();
    virtual FdoBoolean CanWrite();
    virtual FdoBoolean HasContext();

    // support timeout
    void SetConnectionTimeout (unsigned int seconds);
    unsigned int GetConnectionTimeout (void);

    // static callbacks
    static size_t HeaderCallback( void *ptr, size_t size, size_t nmemb, void *stream); 
    static size_t WriteCallback( void *ptr, size_t size, size_t nmemb, void *stream);

private:
    // instance callbacks, called directly by static callbacks
    size_t _headerCallback( void *ptr, size_t size, size_t nmemb); 
    size_t _writeCallback( void *ptr, size_t size, size_t nmemb);

};



#endif
