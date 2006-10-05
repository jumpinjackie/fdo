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

#include <stdafx.h>
#include <FdoCommonStringUtil.h>

#pragma warning(push)
#pragma warning(disable: 4275)  // Disable warning C4275: non dll-interface class
#pragma warning(disable: 4251)  // Disable warning C4251: needs to have dll-interface to be used by clients of class
#include <boost/bind.hpp>
#include <boost/algorithm/string.hpp>
#pragma warning(pop) // Enable warnings

#include "FdoOwsHttpHandler.h"
#include <FdoCommonOSUtil.h>
#include <OWS/FdoOwsGlobals.h>

FdoOwsHttpHandler::FdoOwsHttpHandler()   :
                            m_url(NULL), m_bGet(false), m_parameters(NULL),
                            m_connectionState(ConnectionState_BeforeConnect),
                            m_disposed(false), m_userName(NULL), m_passwd(NULL),
                            m_bValidDocument(false), m_contentType(FdoOwsMIMEType_unknown),
                            m_currentSize(0), m_currentRead(0), m_bRunning(false)
{
    m_errorBuffer[0] = '\0';
    m_tvConnect = 0;
}

FdoOwsHttpHandler::FdoOwsHttpHandler(const char* url, bool bGet, const char* parameters, 
                         const char* userName, const char* passwd) :
                            m_url(url), m_bGet(bGet), m_parameters(parameters),
                            m_connectionState(ConnectionState_BeforeConnect),
                            m_disposed(false), m_userName(userName), m_passwd(passwd),
                            m_bValidDocument(false), m_contentType(FdoOwsMIMEType_unknown),
                            m_currentSize(0), m_currentRead(0), m_bRunning(false)
{
    m_errorBuffer[0] = '\0';
    m_tvConnect = 0;    
}

FdoOwsHttpHandler::~FdoOwsHttpHandler()
{
    size_t size = m_contents.size();
    for (size_t i = 0; i < size; i++)
        delete[] m_contents[i];
}

void FdoOwsHttpHandler::Dispose()
{
    {
        boost::mutex::scoped_lock lock(m_mutex);
        m_disposed = true;
        // wait till the thread stops
        while (m_bRunning)
            m_condition.wait(lock);
    }

	if (m_thread.get())
		m_thread->join ();
    delete this;
}

FdoOwsHttpHandler* FdoOwsHttpHandler::Create(const char* url, bool bGet, const char* parameters,
    const char* userName, const char* passwd)
{
    return new FdoOwsHttpHandler(url, bGet, parameters, userName, passwd);
}


size_t FdoOwsHttpHandler::HeaderCallback( void *ptr, size_t size, size_t nmemb, void *stream)
{
    return reinterpret_cast<FdoOwsHttpHandler*>(stream)->_headerCallback(ptr, size, nmemb);
}

size_t FdoOwsHttpHandler::WriteCallback( void *ptr, size_t size, size_t nmemb, void *stream)
{
    return reinterpret_cast<FdoOwsHttpHandler*>(stream)->_writeCallback(ptr, size, nmemb);
}

size_t FdoOwsHttpHandler::_headerCallback( void *ptr, size_t size, size_t nmemb)
{
    assert(!m_disposed);
    size_t len = size * nmemb;
    if (!m_bValidDocument)
    {
        // parse the HTTP return code and find out 
        // whether current document is valid
        if (len > FdoOwsGlobals::MIN_LEN_HTTP_RESPONSE_FIRST_LINE && 
            strncmp((const char*)ptr, FdoOwsGlobals::HTTP_SLASH, sizeof(FdoOwsGlobals::HTTP_SLASH) - 1) == 0)
        {
            char retCodeStr[FdoOwsGlobals::LEN_HTTP_RET_CODE];
            strncpy(retCodeStr, 
                (const char*)ptr + FdoOwsGlobals::MIN_LEN_HTTP_RESPONSE_FIRST_LINE - FdoOwsGlobals::LEN_HTTP_RET_CODE, 
                FdoOwsGlobals::LEN_HTTP_RET_CODE);
            int retCode = atoi(retCodeStr);
            if (retCode < FdoOwsGlobals::MIN_HTTP_ERROR_CODE)
                m_bValidDocument = true;
        }
    }
    else
    {
        // try to find out the content type
        size_t nContentType = FdoOwsGlobals::Content_Type_Len;
        if (len > nContentType && 
            FdoCommonOSUtil::strnicmp((const char*)ptr, FdoOwsGlobals::Content_Type, nContentType) == 0)
        {
            // trim the blank(s) and ':';
            size_t pt = nContentType;
            while (pt < len &&
                (((const char*)ptr)[pt] == FdoOwsGlobals::Space ||
                ((const char*)ptr)[pt] == FdoOwsGlobals::Colon))
                pt++;
            if (pt < len)
            {
                const char* typeString = (const char*)ptr + pt;
                size_t nType = len - pt;
                if (nType >= FdoOwsGlobals::text_xml_len &&
                    FdoCommonOSUtil::strnicmp(typeString, FdoOwsGlobals::text_xml, FdoOwsGlobals::text_xml_len) == 0)
                    m_contentType = FdoOwsMIMEType_text_xml;
				else if (nType >= FdoOwsGlobals::image_png_len &&
					FdoCommonOSUtil::strnicmp(typeString, FdoOwsGlobals::image_png, FdoOwsGlobals::image_png_len) == 0)
					m_contentType = FdoOwsMIMEType_image_png;
				else if (nType >= FdoOwsGlobals::image_jpeg_len &&
					FdoCommonOSUtil::strnicmp(typeString, FdoOwsGlobals::image_jpeg, FdoOwsGlobals::image_jpeg_len) == 0)
					m_contentType = FdoOwsMIMEType_image_jpeg;
				else if (nType >= FdoOwsGlobals::image_tiff_len &&
					FdoCommonOSUtil::strnicmp(typeString, FdoOwsGlobals::image_tiff, FdoOwsGlobals::image_tiff_len) == 0)
					m_contentType = FdoOwsMIMEType_image_tiff;
            }

        }


    }
    return len;

}

size_t FdoOwsHttpHandler::_writeCallback( void *ptr, size_t size, size_t nmemb)
{
    // this function may be called by multi threads
    boost::mutex::scoped_lock lock(m_mutex);
    if (m_disposed) // this object is already disposed by the main thread
        return 0;
    size_t len = size * nmemb;
    if (!m_bValidDocument)
        return len;
    if (len > 0)
    {
        if (m_contents.size() == 0)
        {
            // we are first here, change the state
            m_connectionState = ConnectionState_Connected;
            m_condition.notify_all();
        }
        char* content = new char[len];
        memcpy(content, ptr, len);
        m_contents.push_back(content);
        m_contentSizes.push_back(len);
        m_currentSize += len;
        m_condition.notify_all(); // someone else may be waiting for the content
    }


    return len;
}

void FdoOwsHttpHandler::Perform()
{
    // run this object
    m_thread = std::auto_ptr<boost::thread>(
        new boost::thread(boost::bind(&FdoOwsHttpHandler::Proc, this)));
    boost::mutex::scoped_lock lock(m_mutex);
    // wait for Receiving_Content
    m_condition.wait(lock);
    // check whether current state is expected
    if (m_connectionState == ConnectionState_BeforeConnect)
    {
        wchar_t* wError;
        multibyte_to_wide(wError, m_errorBuffer);
        FdoException* e = FdoException::Create(FdoException::NLSGetMessage(FDO_133_UNEXPECTEDERROR_PERFORMING, "Unexpected error encountered while accessing the server."));
        e->SetCause (FdoException::Create(wError));
        throw e;
    }
}

void FdoOwsHttpHandler::Proc()
{
    {
        boost::mutex::scoped_lock lock(m_mutex);
        m_bRunning = true;
        m_connectionState = ConnectionState_BeforeConnect;
    }
    CURL* curlHandle = curl_easy_init();

    // Explanation of error handling here:
    // It seems that we just totally ignored all libcurl error, but actually not.
    // Please note that FdoOwsHttpHandler::Proc is running in another thread, 
    // so how could we throw an exception from a working thread to the main thread? 
    // Actually whenever there is a libcurl error code returned, libcurl would put 
    // the error message in a buffer and what we did is just set the state of the 
    // http connection to proper value so that the main thread could detect that 
    // something is wrong. After that, it would generate the exception from the 
    // error buffer and throw it out  in the main thread so that the user can catch it.

    CURLcode rv = CURL_LAST;
    for(;;) {
        if (curlHandle == NULL) break;
        
        // set error options
        rv = curl_easy_setopt(curlHandle, CURLOPT_ERRORBUFFER, m_errorBuffer);
        if (rv != CURLE_OK) break;
        rv = curl_easy_setopt(curlHandle, CURLOPT_FAILONERROR, CURL_TRUE);
        if (rv != CURLE_OK) break;

        // set behavior options
#ifdef _DEBUG // for debug
        rv = curl_easy_setopt(curlHandle, CURLOPT_VERBOSE, CURL_TRUE);
        if (rv != CURLE_OK) break;
#endif
        rv = curl_easy_setopt(curlHandle, CURLOPT_NOSIGNAL, CURL_TRUE);
        if (rv != CURLE_OK) break;

        // set network options
        std::string url = m_url;
        if (m_bGet && m_parameters.size() != 0)
        {
            // search "?" in the url, if not found append it
			if (url.find('?') == std::string::npos)
				url += '?';
			else
			{
				if (url[url.size() - 1] != '&')
					url += '&';
			}

			std::string encoded;
			std::vector<std::string> pairs;
			boost::split (pairs, m_parameters, boost::is_any_of("&"));
			for ( std::vector<std::string>::iterator it = pairs.begin(); 
				it != pairs.end(); 
				++it )
			{				
				size_t pos = it->find ("=");
				char* temp = curl_escape (it->substr(pos + 1).c_str(), 0);			
				if (temp == NULL) break;
				encoded += it->substr (0, pos + 1) + temp;
				encoded += "&";
				curl_free (temp);				
			}
			encoded = encoded.substr(0, encoded.rfind("&"));
			url += encoded;
        }
		
        rv = curl_easy_setopt(curlHandle, CURLOPT_URL, url.c_str());
#ifdef _WIN32
		// this is extremely useful for debug in Mako, you could see in the VS output window 
		// the actual HTTP requests that are being sent out in the backend
		_RPT1(0, "\n%s\n", url.c_str()); 
#endif
        if (rv != CURLE_OK) break;

        // set authentication options
        std::string userPwd;
        if (m_userName.size() != 0)
        {
            userPwd = m_userName;
            userPwd += ':';
            userPwd += m_passwd;
            rv = curl_easy_setopt(curlHandle, CURLOPT_USERPWD, userPwd.c_str());
            if (rv != CURLE_OK) break;
            rv = curl_easy_setopt(curlHandle, CURLOPT_HTTPAUTH, CURLAUTH_ANY);
            if (rv != CURLE_OK) break;
        }

        // set http options
        rv = curl_easy_setopt(curlHandle, CURLOPT_FOLLOWLOCATION, CURL_TRUE);
        if (rv != CURLE_OK) break;
        rv = curl_easy_setopt(curlHandle, CURLOPT_UNRESTRICTED_AUTH, CURL_TRUE);
        if (rv != CURLE_OK) break;
        rv = curl_easy_setopt(curlHandle, CURLOPT_MAXREDIRS, 4);
        if (rv != CURLE_OK) break;
        curl_slist* headers = NULL;
        if (m_bGet)
        {
            rv = curl_easy_setopt(curlHandle, CURLOPT_HTTPGET, CURL_TRUE);
            if (rv != CURLE_OK) break;
        }
        else
        {
            rv = curl_easy_setopt(curlHandle, CURLOPT_POST, CURL_TRUE);
            if (rv != CURLE_OK) break;
            // change the content type to text/xml
            headers = curl_slist_append(headers, "Content-Type: text/xml");
            rv = curl_easy_setopt(curlHandle, CURLOPT_HTTPHEADER, headers);
            if (rv != CURLE_OK) break;
            rv = curl_easy_setopt(curlHandle, CURLOPT_HTTPHEADER, m_parameters.c_str());
            if (rv != CURLE_OK) break;
            rv = curl_easy_setopt(curlHandle, CURLOPT_POSTFIELDSIZE, m_parameters.size());
            if (rv != CURLE_OK) break;
        }

        // Sets the timeout value. libcurl supports two kinds of timeout.
        // One is for "connection" timeout, the other is for "transfer" timeout.        
        // Option "CURLOPT_CONNECTIONTIMEOUT" is limitted to connection phase. 
        // In this phase name lookups may take serveral minutes.        
        if (m_tvConnect > 0)
        {
            rv = curl_easy_setopt(curlHandle, CURLOPT_CONNECTTIMEOUT, m_tvConnect);
            if (rv != CURLE_OK) break;
        }
        else 
        {
            // disable the timeout
            rv = curl_easy_setopt(curlHandle, CURLOPT_CONNECTTIMEOUT, 0);
            if (rv != CURLE_OK) break;
        }

        // set callback options
        rv = curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, &WriteCallback);
        if (rv != CURLE_OK) break;
        // use this as the pass-in parameter to write callback
        rv = curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, this);
        if (rv != CURLE_OK) break;
        rv = curl_easy_setopt(curlHandle, CURLOPT_HEADERFUNCTION, &HeaderCallback);
        if (rv != CURLE_OK) break;
        rv = curl_easy_setopt(curlHandle, CURLOPT_WRITEHEADER, this);

#ifndef FALSE
#define FALSE 0
#endif
		// don't verify the peer (server)
		curl_easy_setopt(curlHandle, CURLOPT_SSL_VERIFYPEER, FALSE);

        // oops, a lot of options set
        // now we can perform
        rv = curl_easy_perform(curlHandle);
        if (rv != CURLE_OK) break;

        // we are done, set the state to Terminated
        boost::mutex::scoped_lock lock(m_mutex);
        m_connectionState = ConnectionState_Terminated;
        break;
    } // end of for (;;)

    if (rv != CURLE_OK) {
        // error occurs, set the state to BeforeConnect
        boost::mutex::scoped_lock lock(m_mutex);
        m_connectionState = ConnectionState_BeforeConnect;

    }

    if (curlHandle != NULL)
        curl_easy_cleanup(curlHandle);

    
    {
        boost::mutex::scoped_lock lock(m_mutex);
        // we still need to nofity other processes in case it is waiting for us
        m_condition.notify_all();
        m_bRunning = false;
    }

}


FdoSize FdoOwsHttpHandler::Read( FdoByte* buffer, FdoSize toRead )
{
    boost::mutex::scoped_lock lock(m_mutex);
    while (m_currentRead >= m_currentSize) // no data available, we must wait if still connected
    {
		if (m_connectionState == ConnectionState_BeforeConnect) // something bad happened
			throw FdoException::Create(FdoException::NLSGetMessage(FDO_132_CONNECTION_BROKEN, "Network connection is broken, or the server stopped responding."));
        if (m_connectionState == ConnectionState_Terminated)
			return 0;
        m_condition.wait(lock);
    }
    size_t block = 0;
    size_t offset = m_currentRead;
    while (offset >= m_contentSizes[block])
        offset -= m_contentSizes[block++];
    size_t numRead = 0;
    while (toRead > 0 && block < m_contents.size())
    {
        size_t numReadBlock = m_contentSizes[block] - offset > toRead ? toRead :
                                m_contentSizes[block] - offset;
        memcpy(buffer + numRead, m_contents[block] + offset, numReadBlock);
        numRead += numReadBlock;
        toRead -= numReadBlock;
        offset += numReadBlock;
        if (offset == m_contentSizes[block])
        {
            offset = 0;
            block++;
        }
    }
    m_currentRead += numRead;
    return numRead;

}

void FdoOwsHttpHandler::Write( FdoByte* buffer, FdoSize count )
{
    throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_3_NOTIMPLEMENTED), "Method is not implemented."));
}

void FdoOwsHttpHandler::Write( FdoIoStream* stream, FdoSize count)
{
    throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_3_NOTIMPLEMENTED), "Method is not implemented."));
}

void FdoOwsHttpHandler::SetLength( FdoInt64 length )
{
    throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_3_NOTIMPLEMENTED), "Method is not implemented."));
}

FdoInt64 FdoOwsHttpHandler::GetLength()
{
    return (FdoInt64)0xffffffff;
}

FdoInt64 FdoOwsHttpHandler::GetIndex()
{
    boost::mutex::scoped_lock lock(m_mutex);
    return m_currentRead;
}

void FdoOwsHttpHandler::Skip( FdoInt64 offset )
{
    boost::mutex::scoped_lock lock(m_mutex);
    if (offset < 0 && -offset > m_currentRead)
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_INDEXOUTOFBOUNDS)));
    m_currentRead += (size_t)offset;
}

void FdoOwsHttpHandler::Reset()
{
    boost::mutex::scoped_lock lock(m_mutex);
    m_currentRead = 0;
}

FdoBoolean FdoOwsHttpHandler::CanRead()
{
    return true;
}

FdoBoolean FdoOwsHttpHandler::CanWrite()
{
    return false;
}

FdoBoolean FdoOwsHttpHandler::HasContext()
{
    return true;
}

void FdoOwsHttpHandler::SetConnectionTimeout (unsigned int seconds)
{
    m_tvConnect = seconds;
}

unsigned int FdoOwsHttpHandler::GetConnectionTimeout (void)
{
    return m_tvConnect;
}
