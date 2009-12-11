//  
//  NOTE : This code was contributed to the apache project by Dominik Stadler for Xerces defect 27287.  
//  Please see the bug for details: http://issues.apache.org/jira/browse/XERCESC-1167.
//..

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/util/TranscodingException.hpp>
#include "XML885915Transcoder.h"
#include <xercesc/util/XMLString.hpp>
#include <string.h>

// ---------------------------------------------------------------------------
//  XML885915Transcoder: Constructors and Destructor
// ---------------------------------------------------------------------------
XML885915Transcoder::XML885915Transcoder( const   XMLCh* const    encodingName
                                        , const unsigned int    blockSize
                                        , XERCES_CPP_NAMESPACE::MemoryManager* const  manager) :

    XMLTranscoder(encodingName, blockSize, manager)
{
}


XML885915Transcoder::~XML885915Transcoder()
{
}
struct __ascii_iso_map
{
    XMLByte ascii;
    XMLCh   iso;
};
static const __ascii_iso_map ISO88591Replacements [] = {
    {0xA4,0x20AC}, //EURO SIGN
    {0xA6,0x0160}, //CAPITAL S CARON
    {0xA8,0x0161}, //SMALL S CARON
    {0xB4,0x017D}, //CAPITAL Z CARON
    {0xB8,0x017E}, //SMALL Z CARON 
    {0xBC,0x0152}, //CAPITAL LIGATURE OE
    {0xBD,0x0153}, //SMALL LIGATURE OE 
    {0xBE,0x0178} //CAPITAL Y DIAERESIS 
};

static const int ISO88591ReplacementsCount = sizeof(ISO88591ReplacementsCount) / sizeof(__ascii_iso_map);

// ---------------------------------------------------------------------------
//  XML885915Transcoder: Implementation of the transcoder API
// ---------------------------------------------------------------------------
unsigned int
XML885915Transcoder::transcodeFrom(  const   XMLByte* const       srcData
                                    , const unsigned int         srcCount
                                    ,       XMLCh* const         toFill
                                    , const unsigned int         maxChars
                                    ,       unsigned int&        bytesEaten
                                    ,       unsigned char* const charSizes)
{
    // If debugging, make sure that the block size is legal
    #if defined(XERCES_DEBUG)
    checkBlockSize(maxChars);
    #endif

    //
    //  Calculate the max chars we can do here. Its the lesser of the
    //  max output chars and the number of bytes in the source.
    //
    const unsigned int countToDo = srcCount < maxChars ? srcCount : maxChars;

    //
    //  Loop through the bytes to do and convert over each byte. Its just
    //  a cast to the wide char type.
    //
    const XMLByte*  srcPtr = srcData;
    XMLCh*          destPtr = toFill;
    const XMLByte*  srcEnd = srcPtr + countToDo;
    while (srcPtr < srcEnd)
    {
        XMLCh rep = 0;
        for (int i=0; i < ISO88591ReplacementsCount;++i)
        {
            if (*srcPtr == ISO88591Replacements[i].ascii)
            {
                rep = ISO88591Replacements[i].iso;
                break;
            }

        }
        if (rep == 0) // no special character found, use the souce as it is.
            rep = XMLCh(*srcPtr);
        ++srcPtr;
        *destPtr++ = rep;
    }
    // Set the bytes eaten, and set the char size array to the fixed size
    bytesEaten = countToDo;
    memset(charSizes, 1, countToDo);

    // Return the chars we transcoded
    return countToDo;
}


unsigned int
XML885915Transcoder::transcodeTo(const   XMLCh* const    srcData
                                , const unsigned int    srcCount
                                ,       XMLByte* const  toFill
                                , const unsigned int    maxBytes
                                ,       unsigned int&   charsEaten
                                , const UnRepOpts       options)
{
    // If debugging, make sure that the block size is legal
    #if defined(XERCES_DEBUG)
    checkBlockSize(maxBytes);
    #endif

    //
    //  Calculate the max chars we can do here. Its the lesser of the
    //  max output bytes and the number of chars in the source.
    //
    const unsigned int countToDo = srcCount < maxBytes ? srcCount : maxBytes;

    //
    //  Loop through the bytes to do and convert over each byte. Its just
    //  a downcast of the wide char, checking for unrepresentable chars.
    //
    const XMLCh*    srcPtr  = srcData;
    const XMLCh*    srcEnd  = srcPtr + countToDo;
    XMLByte*        destPtr = toFill;
    while (srcPtr < srcEnd)
    {
        // If its legal, take it and jump back to top
        if (*srcPtr < 256)
        {
            XMLByte rep = 0;
            for (int i=0; i < ISO88591ReplacementsCount;++i)
            {
                if (*srcPtr == ISO88591Replacements[i].iso)
                {
                    rep = ISO88591Replacements[i].ascii;
                    break;
                }
                
            }
            if (rep == NULL) // no special character found, use the souce as it is.
                rep = XMLByte(*srcPtr);
            ++srcPtr;
            *destPtr++ = rep;
            continue;
        }

        //
        //  Its not representable so use a replacement char. According to
        //  the options, either throw or use the replacement.
        //
        if (options == UnRep_Throw)
        {
            XMLCh tmpBuf[16];
            XERCES_CPP_NAMESPACE_QUALIFIER XMLString::binToText((unsigned int)*srcPtr, tmpBuf, 16, 16);
            ThrowXML2
            (
                XERCES_CPP_NAMESPACE_QUALIFIER TranscodingException
                , XERCES_CPP_NAMESPACE_QUALIFIER XMLExcepts::Trans_Unrepresentable
                , tmpBuf
                , getEncodingName()
            );
        }
        *destPtr++ = 0x1A;
        srcPtr++;
    }

    // Set the chars eaten
    charsEaten = countToDo;

    // Return the bytes we transcoded
    return countToDo;
}


bool XML885915Transcoder::canTranscodeTo(const unsigned int toCheck) const
{
    return (toCheck < 256);
}
