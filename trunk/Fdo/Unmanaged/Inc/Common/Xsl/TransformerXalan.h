#ifndef FDO_XSL_TRANSFORMERXALAN_H
#define FDO_XSL_TRANSFORMERXALAN_H
// 

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
//


#include <FdoCommon.h>
#include <xercesc/sax/Locator.hpp>
#include <xercesc/util/BinInputStream.hpp>
#define XALAN_USE_NATIVE_WCHAR_T 1
#include <xalanc/XSLT/ProblemListener.hpp>
#include <xalanc/XalanDOM/XalanDOMString.hpp>
#include <xalanc/XalanDOM/XalanNode.hpp>
#include <xalanc/XalanTransformer/XalanTransformer.hpp>

/// \cond DOXYGEN-IGNORE
// Subclass of FdoXslTransformer that uses Xalan to perform the transformation.
// This class is not directly exposed to the API, only through the 
// FdoXslTransformer interface.
class FdoXslTransformerXalan : public FdoXslTransformer, public XALAN_CPP_NAMESPACE::ProblemListener
{
public:

    static FdoXslTransformerXalan* Create( 
        FdoXmlReader* inDoc = NULL, 
        FdoXmlReader* stylesheet = NULL, 
        FdoXmlWriter* outDoc = NULL,
        FdoIoTextWriter* log = NULL
    );

    virtual void Transform();

    static void Initialize();

    /// Implementation of XALAN_CPP_NAMESPACE::ProblemListener interface:
    virtual void setPrintWriter(XALAN_CPP_NAMESPACE::PrintWriter* pw);

    /**
      * Function that is called when a problem event occurs.
     * 
     * @param   where          either in XMLPARSER, XSLPROCESSOR, or QUERYENGINE
     * @param   classification either MESSAGE, ERROR or WARNING
     * @param   sourceNode     source tree node where the problem occurred (may be 0)
     * @param   styleNode      style tree node where the problem occurred (may be 0)
     * @param   msg            string message explaining the problem.
     * @param   uri            the URI of the document where the problem occurred.  May be 0.
     * @param   lineNo         line number where the problem occurred.
     * @param   charOffset     character offset where the problem.
     */
    virtual void
    problem(
            eSource                                         where,
            eClassification                                 classification,
            const XALAN_CPP_NAMESPACE::XalanNode*           sourceNode,
            const XALAN_CPP_NAMESPACE::ElemTemplateElement* styleNode,
            const XALAN_CPP_NAMESPACE::XalanDOMString&      msg,
            const XALAN_CPP_NAMESPACE::XalanDOMChar*        uri,
            XALAN_CPP_NAMESPACE::XalanFileLoc               lineNo,
            XALAN_CPP_NAMESPACE::XalanFileLoc               charOffset);

    virtual void
    problem(
            eSource                                         source,
            eClassification                                 classification,
            const XALAN_CPP_NAMESPACE::XalanDOMString&      msg,
            const XERCES_CPP_NAMESPACE::Locator*            locator,
            const XALAN_CPP_NAMESPACE::XalanNode*           sourceNode);

    virtual void
    problem(
            eSource                                        source,
            eClassification                                classification,
            const XALAN_CPP_NAMESPACE::XalanDOMString&     msg,
            const XALAN_CPP_NAMESPACE::XalanNode*          sourceNode);

protected:
    /// Reader adapter. Allows Xalan to read from a FdoXmlReader.
    class InputSource : public XALAN_CPP_NAMESPACE::XSLTInputSource
    {
    public:

        InputSource( FdoXmlReader* reader );

        ~InputSource() {}

        virtual XALAN_CPP_NAMESPACE::BinInputStreamType* makeStream () const;

    private:
        class InputStream : public XALAN_CPP_NAMESPACE::BinInputStreamType
        {
        public:
            InputStream( FdoXmlReader* reader ); 

            ~InputStream() {}

            /// Tells Xalan the current position of the reader
            virtual XMLFilePos curPos()  const;

            /// Gets data from the reader to Xalan.
            virtual XMLSize_t readBytes( 
                XMLByte *const  toFill,  
                const XMLSize_t  maxToRead 
            );

            /**
             * Return the "out-of-band" content type for the data supplied by this
             * input stream in the form of the media-type production (mime type
             * with optional parameters such as encoding) as defined by the HTTP 1.1
             * specification. If no such content type is provided for the data, 0 is
             * returned. This function is expected to return the correct value at
             * any time after the construction of the stream.
             *
             * An example of the stream that may return non-0 from this function is
             * an HTTP stream with the value returned taken from the "Content-Type"
             * HTTP header. Note also that if the encoding of the data is known
             * to the application by some other means then the setEncoding function
             * in the InputSource object should be used instead. The getContentType
             * function should only be used to return information that is intrinsic
             * to the stream.
             *
             * @return The content type, or 0 if one is not available.
             */
            virtual const XMLCh* getContentType() const {return 0;}

        private:
            FdoXmlReaderP mReader;
        };

        FdoXmlReaderP mReader;
    };

    FdoXslTransformerXalan( 
        FdoXmlReader* inDoc = NULL, 
        FdoXmlReader* stylesheet = NULL, 
        FdoXmlWriter* outDoc = NULL,
        FdoIoTextWriter* log = NULL
    );

    virtual ~FdoXslTransformerXalan(void) {}

    /// Output handler callback implementation. Writes the data to the 
    /// out document
    unsigned long HandleOutput(const void* data, unsigned long length );

    /// Output handler callback implementation. Forwards the data to this 
    /// transformer, which is passed in through the handle parameter.
    static CallbackSizeType outputHandler(const void* data, CallbackSizeType length, const void *handle);

    /// Handles output flush requests from Xalan.
    static void flushHandler(const void *handle);

    /// Convert a XalanDOMString to a FdoStringP string:
    FdoStringP XalanDomStringToUnicode(const XALAN_CPP_NAMESPACE::XalanDOMString &xalanDomString);

    /// Convert a XalanNode to a FdoStringP string:
    FdoStringP XalanNodeToUnicode(const XALAN_CPP_NAMESPACE::XalanNode* inNode);

private:
    FdoInt32 mPrologueState;
};
/// \endcond

#endif
