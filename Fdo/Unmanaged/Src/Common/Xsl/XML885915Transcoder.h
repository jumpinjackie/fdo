#ifndef XML885915TRANSCODER_H
#define XML885915TRANSCODER_H

#include <xercesc/util/XercesDefs.hpp>
#include <xercesc/util/TransService.hpp>

//  
//  NOTE : This code was contributed to the apache project by Dominik Stadler for Xerces defect 27287.  
//  Please see the bug for details: http://issues.apache.org/jira/browse/XERCESC-1167.
//..
//  This class provides an implementation of the XMLTranscoder interface
//  for a simple 8859-15 transcoder. The parser does some encodings
//  intrinsically without depending upon external transcoding services.
//  To make everything more orthagonal, we implement these internal
//  transcoders using the same transcoder abstraction as the pluggable
//  transcoding services do.
//
class XML885915Transcoder : public XERCES_CPP_NAMESPACE_QUALIFIER XMLTranscoder
{
public :
    // -----------------------------------------------------------------------
    //  Public constructors and destructor
    // -----------------------------------------------------------------------
    XML885915Transcoder
    (
        const   XMLCh* const    encodingName
        , const unsigned int    blockSize
        , XERCES_CPP_NAMESPACE::MemoryManager* const manager = XERCES_CPP_NAMESPACE::XMLPlatformUtils::fgMemoryManager
    );

    virtual ~XML885915Transcoder();


    // -----------------------------------------------------------------------
    //  Implementation of the XMLTranscoder interface
    // -----------------------------------------------------------------------
    virtual unsigned int transcodeFrom
    (
        const   XMLByte* const          srcData
        , const unsigned int            srcCount
        ,       XMLCh* const            toFill
        , const unsigned int            maxChars
        ,       unsigned int&           bytesEaten
        ,       unsigned char* const    charSizes
    );

    virtual unsigned int transcodeTo
    (
        const   XMLCh* const    srcData
        , const unsigned int    srcCount
        ,       XMLByte* const  toFill
        , const unsigned int    maxBytes
        ,       unsigned int&   charsEaten
        , const UnRepOpts       options
    );

    virtual bool canTranscodeTo
    (
        const   unsigned int    toCheck
    )   const;


private :
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    XML885915Transcoder(const XML885915Transcoder&);
    XML885915Transcoder& operator=(const XML885915Transcoder&);
};

#endif
