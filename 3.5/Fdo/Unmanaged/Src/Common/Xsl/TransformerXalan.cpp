// 
//  
//  Copyright (C) 2004-2006  Autodesk, Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//  
#include <cstring>
#include <FdoCommon.h>
#include <Common/Xsl/TransformerXalan.h>
#include <Common/Xml/UtilXrcs.h>
#include <stdio.h>
#include <malloc.h>
#include <sys/stat.h>
#include <xalanc/XalanDOM/XalanNode.hpp>
#include <xercesc/util/TransENameMap.hpp>
#include "XML885915Transcoder.h"

class FdoXmlUtilXrcsSingleton
{
public:
    FdoXmlUtilXrcsSingleton()
    {
        XERCES_CPP_NAMESPACE::XMLPlatformUtils::Initialize();

        // Fix Ticket #600 - the capability document with ISO-8859-15 can't be parsed by Xerecs library
        if (XERCES_CPP_NAMESPACE::XMLPlatformUtils::fgTransService)
        {
            XERCES_CPP_NAMESPACE::XMLPlatformUtils::fgTransService->addEncoding((const XMLCh*)fgISO885915EncodingString, new XERCES_CPP_NAMESPACE::ENameMapFor<XML885915Transcoder>(fgISO885915EncodingString));
            XERCES_CPP_NAMESPACE::XMLPlatformUtils::fgTransService->addEncoding((const XMLCh*)fgISO885915EncodingString2, new XERCES_CPP_NAMESPACE::ENameMapFor<XML885915Transcoder>(fgISO885915EncodingString2));
            XERCES_CPP_NAMESPACE::XMLPlatformUtils::fgTransService->addEncoding((const XMLCh*)fgISO885915EncodingString3, new XERCES_CPP_NAMESPACE::ENameMapFor<XML885915Transcoder>(fgISO885915EncodingString3));
        }
    }

    ~FdoXmlUtilXrcsSingleton()
    {
        XERCES_CPP_NAMESPACE::XMLPlatformUtils::Terminate();
    }

private:
    static const XMLCh fgISO885915EncodingString[];  //"ISO8859-15"
    static const XMLCh fgISO885915EncodingString2[]; //"ISO-8859-15"
    static const XMLCh fgISO885915EncodingString3[]; //"ISO_8859-15"
};

// "ISO8859-15"
const XMLCh FdoXmlUtilXrcsSingleton::fgISO885915EncodingString[] = 
{
    0x49, 0x53, 0x4F, /*I,S,O,*/
    0x38, 0x38, 0x35, 0x39, /*8,8,5,9*/
    0x2D, /*-*/
    0x31, 0x35, 0x00 /*1,5*/
};

// "ISO-8859-15"
const XMLCh FdoXmlUtilXrcsSingleton::fgISO885915EncodingString2[] = 
{
    0x49, 0x53, 0x4F, /*I,S,O,*/
    0x2D, /*-*/
    0x38, 0x38, 0x35, 0x39, /*8,8,5,9*/
    0x2D, /*-*/
    0x31, 0x35, 0x00 /*1,5*/
};

// "ISO_8859-15"
const XMLCh FdoXmlUtilXrcsSingleton::fgISO885915EncodingString3[] = 
{
    0x49, 0x53, 0x4F, /*I,S,O,*/
    0x5F, /*_*/
    0x38, 0x38, 0x35, 0x39, /*8,8,5,9*/
    0x2D, /*-*/
    0x31, 0x35, 0x00 /*1,5*/
};

class FdoXslSingleton
{
public:
    FdoXslSingleton()
    {
        XALAN_USING_XERCES(XMLPlatformUtils)
        XALAN_USING_XALAN(XalanTransformer)
        XALAN_USING_XALAN(XalanDOMString)

        // Initialize Xalan.
        XalanTransformer::initialize();
    }

    ~FdoXslSingleton()
    {
        XALAN_USING_XERCES(XMLPlatformUtils)
        XALAN_USING_XALAN(XalanTransformer)
        XALAN_USING_XALAN(XalanDOMString)

        // Terminate Xalan.
        XalanTransformer::terminate();
    }
};

static FdoXmlUtilXrcsSingleton Singleton1;

static FdoXslSingleton Singleton2;

FdoXslTransformerXalan::FdoXslTransformerXalan( 
    FdoXmlReader* inDoc, 
    FdoXmlReader* stylesheet, 
    FdoXmlWriter* outDoc, 
    FdoIoTextWriter* log
) :
    FdoXslTransformer( inDoc, stylesheet, outDoc, log ),
    mPrologueState(0)
{
}

FdoXslTransformerXalan* FdoXslTransformerXalan::Create( 
    FdoXmlReader* inDoc, 
    FdoXmlReader* stylesheet, 
    FdoXmlWriter* outDoc,
    FdoIoTextWriter* log
)
{
    Initialize();
    return( new FdoXslTransformerXalan( inDoc, stylesheet, outDoc, log ) );
}

void FdoXslTransformerXalan::Transform()
{
    // Reading from a stream at EOF is a common client error.
    // Therefore trap this particular problem.
    FdoIoStreamP stream = FdoXmlReaderP(GetInDoc())->GetStream();

    FdoInt64 streamLen = stream->GetLength();

    if ( (streamLen >= 0) && (stream->GetIndex() >= streamLen) ) 
        throw FdoXmlException::Create(
            FdoException::NLSGetMessage(
                FDO_NLSID(FDO_55_XSLINPUTEOF)
            )
        );

    stream = FdoXmlReaderP(GetStylesheet())->GetStream();

    streamLen = stream->GetLength();

    if ( (streamLen >= 0) && (stream->GetIndex() >= streamLen) ) 
        throw FdoXmlException::Create(
            FdoException::NLSGetMessage(
                FDO_NLSID(FDO_56_XSLSTYLESHEETEOF)
            )
        );

    // Create the Xalan transformer
    XALAN_CPP_NAMESPACE::XalanTransformer transformer;
    // Attach the in document to it
	FdoXmlReaderP readerP = GetInDoc();
    InputSource inSource( readerP );
    // Attach the stylesheet to it.
	readerP = GetStylesheet();
	InputSource sheetSource( readerP );
    sheetSource.setSystemId((const XMLCh*) L"sheet");

    try {
        FdoDictionaryP params = GetParameters();
        FdoInt32 i;

        // Give Xalan all the parameters
        for ( i = 0; i < params->GetCount(); i++ ) {
            FdoDictionaryElementP param = params->GetItem(i);

            if ( param->GetValue() )
                transformer.setStylesheetParam( 
                    XALAN_CPP_NAMESPACE::XalanDOMString( (const char*) FdoStringP(param->GetName()) ), 
                    XALAN_CPP_NAMESPACE::XalanDOMString( (const char*) FdoStringP(param->GetValue()) )
                );
        }

        // Listen to messages, warnings, and errors:
        transformer.setProblemListener(this);

        // Do the transformation.
        if ( transformer.transform( inSource, sheetSource, (void*) this, (XalanOutputHandlerType) outputHandler, (XalanFlushHandlerType) flushHandler ) ) {
			const char* lastError = transformer.getLastError();
			size_t i = strlen ( lastError ) + 1;
			wchar_t* wstrLastError = (wchar_t*)alloca (i * sizeof (wchar_t));

//#ifdef _WIN32
//			i = MultiByteToWideChar ( CP_THREAD_ACP, 0,lastError,(int)i, wstrLastError,(int)i);
			
//#else
			i = mbstowcs (wstrLastError, lastError, i);
//#endif
			if( i == 0 )
				wstrLastError[0] = 0;
			throw FdoXmlException::Create(
                FdoException::NLSGetMessage(
                    FDO_NLSID(FDO_42_GENERICCHAR),
                    wstrLastError
                )
            );

        }
    }
    catch ( const XALAN_CPP_NAMESPACE::XSLException& ex ) {
        throw FdoXmlException::Create(
            FdoException::NLSGetMessage(
                FDO_NLSID(FDO_43_XSLERROR),
                ex.getLineNumber(),
                ex.getColumnNumber(),
                (FdoString*)XalanDomStringToUnicode(ex.getMessage())
            )
        );
    }
}

void FdoXslTransformerXalan::Initialize()
{
}

unsigned long FdoXslTransformerXalan::HandleOutput(const void* data, unsigned long length )
{
    FdoByte* lData                          = (FdoByte*) data;
    FdoSize lLength                         = length;
    FdoSize idx                             = 0;
    
    // Skip the prologue, FdoXmlWriter writes its own. 

    if ( mPrologueState < 3 ) {
        // End of prologue not yet encountered. Look for end ( "?>" sequence ).

        for ( idx = 0; (idx < length) && (mPrologueState < 3); idx++ ) {
            switch ( mPrologueState ) {
            case 0:
                if ( (*lData) == '?' ) 
                    // Found '?', move onto next state ( look for '>' ).
                    mPrologueState++;
                break;

            case 1:
                if ( (*lData) == '>' ) 
                    // Found '>', move onto next state ( prologue found ).
                    mPrologueState++;
                else
                    mPrologueState = 0;
                break;

            case 2:
                if ( (*lData) == '<' )
                    // Found '<' (start of first element), move onto next state ( ready to write ).
                    mPrologueState++;

                break;
            }

            // Increment current char if not ready to write yet.
            if ( mPrologueState < 3 ) {
                lData++;
                lLength--;
            }
        }

    }

    if ( lLength > 0 ) {
        FdoXmlWriterP pWriter = GetOutDoc();
        pWriter->WriteBytes(lData,lLength);
    }
    
    return(length);
}

CallbackSizeType FdoXslTransformerXalan::outputHandler(const void* data, CallbackSizeType length, const void *handle)
{
    return ( ((FdoXslTransformerXalan*) handle)->HandleOutput(data, (unsigned long)length ) );
}

void FdoXslTransformerXalan::flushHandler(const void* /* handle */)
{
    //Nothing to flush.
}

FdoXslTransformerXalan::InputSource::InputSource( FdoXmlReader* reader )
{
    reader->AddRef();
    mReader = reader;
}

XALAN_CPP_NAMESPACE::BinInputStreamType* 
    FdoXslTransformerXalan::InputSource::makeStream () const 
{
    return new InputStream(mReader);
}

FdoXslTransformerXalan::InputSource::InputStream::InputStream( FdoXmlReader* reader ) 
{
    reader->AddRef();
    mReader = reader;
}

unsigned int FdoXslTransformerXalan::InputSource::InputStream::curPos()  const
{
    return( (unsigned int) FdoIoStreamP(mReader->GetStream())->GetIndex() );
}

unsigned int FdoXslTransformerXalan::InputSource::InputStream::readBytes( 
    XMLByte *const  toFill,  
    const unsigned int  maxToRead 
)
{
    return (unsigned int)(( FdoIoStreamP(mReader->GetStream())->Read( toFill, maxToRead ) ));
}


void FdoXslTransformerXalan::setPrintWriter(XALAN_CPP_NAMESPACE::PrintWriter*		pw)
{
    // do nothing
}

void FdoXslTransformerXalan::problem(
			eProblemSource				                    where,
			eClassification				                    classification,
			const XALAN_CPP_NAMESPACE::XalanNode*			sourceNode,
			const XALAN_CPP_NAMESPACE::ElemTemplateElement*	styleNode,
			const XALAN_CPP_NAMESPACE::XalanDOMString&		msg,
			const XALAN_CPP_NAMESPACE::XalanDOMChar*		uri,
			int							                    lineNo,
			int							                    charOffset)
{
    FdoIoTextWriterP pLog = GetLog();
    FdoIoFileStreamP pFileStream;
    
    if (pLog == NULL)
    {
        // When no log specified, send the message to stdout or stderr for
        // convenience.
        FILE* defaultFp = ((classification == eERROR) || (classification == eWARNING)) ?
                        stderr :
                        stdout;

        // First check if stdout or stderr is available. 
        // This code was cloned from FdoIoFileStream and should
        // eventually be centralized in a FdoIoFileStream static function.
   
#ifdef _WIN32
        int defaultFd = _fileno(defaultFp);
#else
        int defaultFd = fileno(defaultFp);
#endif

        struct stat fileStat;

        if ( fstat(defaultFd, &fileStat) == 0 ) 
        {
            // Out file ok, try to wrap a writer around it.
            try 
            {
                pFileStream = FdoIoFileStream::Create( defaultFp );

                if ( pFileStream->CanWrite() )
                    pLog = FdoIoTextWriter::Create( pFileStream ); 

            }
            catch (...)
            {
                // On exception, silently give up. 
                // Don't want to bring down FDO if the out file 
                // cannot be wrapped. 
                pLog = NULL;
            }
        }
    }

    if ( pLog ) 
    {
        // Output message type:
        switch (classification)
        {
            case eERROR:
                pLog->Write(FdoException::NLSGetMessage(FDO_114_XSL_LOG_ERROR, "ERROR"));
                break;
            case eWARNING:
                pLog->Write(FdoException::NLSGetMessage(FDO_115_XSL_LOG_WARNING, "WARNING"));
                break;
            default:
            case eMESSAGE:
                pLog->Write(FdoException::NLSGetMessage(FDO_116_XSL_LOG_MESSAGE, "MESSAGE"));
                break;
        }

        // Output message originator:
        switch (where)
        {
            case eXMLPARSER:
                pLog->Write(FdoStringP::Format(L" (%ls): ", FdoException::NLSGetMessage(FDO_117_XSL_LOG_ORIGINATOR_XMLPARSER, "XML Parser")));
                break;
            case eXSLPROCESSOR:
                pLog->Write(FdoStringP::Format(L" (%ls): ", FdoException::NLSGetMessage(FDO_118_XSL_LOG_ORIGINATOR_XSLPROCESSOR, "XSL Processor")));
                break;
            case eXPATH:
                pLog->Write(FdoStringP::Format(L" (%ls): ", FdoException::NLSGetMessage(FDO_119_XSL_LOG_ORIGINATOR_XPATH, "XPath")));
                break;
            default:
                pLog->Write(FdoStringP::Format(L" (%ls): ", FdoException::NLSGetMessage(FDO_120_XSL_LOG_ORIGINATOR_UNKNOWN, "Unknown originator")));
                break;
        }

        // Output source node (if any):
        if (sourceNode != NULL)
            pLog->Write(FdoStringP::Format(L"%ls, ", FdoException::NLSGetMessage(FDO_121_XSL_ATSOURCENODE, "At source node '%1$ls'", (FdoString*)XalanNodeToUnicode(sourceNode))));

        // Output actual message:
        pLog->Write(XalanDomStringToUnicode(msg));

        // Output stylesheet/etc location where message was issued from:
        pLog->WriteLine(FdoStringP::Format(L", %ls", FdoException::NLSGetMessage(FDO_122_XSL_ATURI, "at URI '%1$ls' (line %2$d, column %3$d)", uri ? uri : L"", lineNo, charOffset)));
    }
}


FdoStringP FdoXslTransformerXalan::XalanDomStringToUnicode(const XALAN_CPP_NAMESPACE::XalanDOMString &xalanDomString)
{
#if XALAN_VERSION_MAJOR == 1 && XALAN_VERSION_MINOR >= 10
    XALAN_CPP_NAMESPACE::CharVectorType charData;
    xalanDomString.transcode( charData );
    std::string buffer;
    XALAN_CPP_NAMESPACE::CharVectorType::iterator it;
    for( it = charData.begin(); it != charData.end(); it++ )
         buffer += *it;

    return FdoStringP(buffer.c_str());
#else
    XALAN_CPP_NAMESPACE::CharVectorType vector = xalanDomString.transcode();
    char buffer[5001];
    wchar_t wbuffer[5001];

    // Todo: the following will work but should look for a better way to
    // transcode the exception message.
    FdoSize i = 0;
    for ( i = 0; (i < 5000) && (i < vector.size()); i++ )
        buffer[i] = vector[i];
    buffer[i] = '\0';
    i = mbstowcs (wbuffer, buffer, 5000);

    return FdoStringP(wbuffer);
#endif
}

FdoStringP FdoXslTransformerXalan::XalanNodeToUnicode(const XALAN_CPP_NAMESPACE::XalanNode* inNode)
{
    FdoStringP serializedNode;
    const XALAN_CPP_NAMESPACE::XalanNode* node = inNode;
    while (node != NULL)
    {
        serializedNode = FdoStringP::Format(
            L"%ls%ls%ls",  //WITH INDEX: L"%ls[%d]%ls%ls",
            (FdoString*)XalanDomStringToUnicode(node->getNodeName()),
            //node->getIndex(),
            serializedNode.GetLength() > 0 ? L"/" : L"",
            (FdoString*)serializedNode);
        node = node->getParentNode();
    }
    return serializedNode;
}
