//
// Copyright (C) 2004-2006  Autodesk, Inc.
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

#ifndef FDOOWSXMLSAXCONTEXT_H
#define FDOOWSXMLSAXCONTEXT_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

class FdoOwsXmlSaxContext : public FdoXmlSaxContext
{
private:
    int m_stateCapability;
    int m_stateRequestMetadata;
    int m_stateServiceMetadata;
    int m_stateExceptionReport;

    FdoPtr<FdoXmlCharDataHandler> m_serviceIdCharDataHandler;
    FdoPtr<FdoXmlCharDataHandler> m_exceptionReportCharDataHandler;

public:
    FDOOWS_API int StateCapability() const { return m_stateCapability; }
    FDOOWS_API int StateRequestMetadata() const { return m_stateRequestMetadata; }
    FDOOWS_API int StateServiceMetadata() const { return m_stateServiceMetadata; }
    FDOOWS_API int StateExceptionReport() const { return m_stateExceptionReport; }
    FDOOWS_API void SetStateCapability(int state) { m_stateCapability = state; }
    FDOOWS_API void SetStateRequestMetadata(int state) { m_stateRequestMetadata = state; }
    FDOOWS_API void SetStateServiceMetadata(int state) { m_stateServiceMetadata = state; }
    FDOOWS_API void SetStateExceptionReport(int state) { m_stateExceptionReport = state; }

    FDOOWS_API FdoXmlCharDataHandler* ServiceIdCharDataHandler() { return FDO_SAFE_ADDREF(m_serviceIdCharDataHandler.p); }
    FDOOWS_API FdoXmlCharDataHandler* ExceptionReportCharDataHandler() { return FDO_SAFE_ADDREF(m_exceptionReportCharDataHandler.p); }

    FDOOWS_API void SetServiceIdCharDataHandler(FdoXmlCharDataHandler* handler) { m_serviceIdCharDataHandler = FDO_SAFE_ADDREF(handler); }
    FDOOWS_API void SetExceptionReportCharDataHandler(FdoXmlCharDataHandler* handler) { m_exceptionReportCharDataHandler = FDO_SAFE_ADDREF(handler) ;}

    FDOOWS_API FdoOwsXmlSaxContext(FdoXmlReader* reader) : FdoXmlSaxContext(reader)
    {
        m_stateCapability = 0;
        m_stateRequestMetadata = 0;
        m_stateServiceMetadata = 0;
        m_stateExceptionReport = 0;
    }
};

typedef FdoPtr<FdoOwsXmlSaxContext> FdoOwsXmlSaxContextP;

#endif


