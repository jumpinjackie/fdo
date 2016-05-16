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
 * Revision Control Modification History
 *
 *  $HeadURL: http://svn.hobu.net/frank/GdalFile/src/Overrides/FdoGrfpRasterImageDefinition.cpp $
 *   $Author: fwarmerdam $
 *     $Date: 2006-01-26 13:02:46 -0500 (Thu, 26 Jan 2006) $
 * $Revision: 24 $
 *
 */

#include "Overrides.h"
#include "FdoGrfpXmlGlobals.h"
#include <FdoCommonOSUtil.h>
#include <GdalFile/Override/FdoGrfpRasterImageDefinition.h>
#include <GdalFile/Override/FdoGrfpRasterGeoreferenceLocation.h>

FdoGrfpRasterImageDefinition::FdoGrfpRasterImageDefinition(void) : 
        m_state(0), m_frameNumber(-1), 
        m_haveBounds(false), 
        m_minX(-12300000), m_minY(-12300000), m_maxX(-12300000), m_maxY(-12300000)
{
}

FdoGrfpRasterImageDefinition::~FdoGrfpRasterImageDefinition(void)
{
}

void FdoGrfpRasterImageDefinition::Dispose()
{
    delete this;
}

FdoGrfpRasterImageDefinition* FdoGrfpRasterImageDefinition::Create()
{
    return new FdoGrfpRasterImageDefinition();
}

FdoInt32 FdoGrfpRasterImageDefinition::GetFrameNumber()
{
    return m_frameNumber;
}

void FdoGrfpRasterImageDefinition::SetFrameNumber(FdoInt32 frameNumber)
{
    m_frameNumber = frameNumber;
}

bool FdoGrfpRasterImageDefinition::GetBounds( 
    double &minX, double &minY, double &maxX, double &maxY )

{
    minX = m_minX;
    minY = m_minY;
    maxX = m_maxX;
    maxY = m_maxY;
    
    return m_haveBounds;
}

void FdoGrfpRasterImageDefinition::SetBounds( 
    double minX, double minY, double maxX, double maxY )

{
    m_haveBounds = true;
    m_minX = minX;
    m_minY = minY;
    m_maxX = maxX;
    m_maxY = maxY;
}

// Initialize the object using the objects XML attributes
void FdoGrfpRasterImageDefinition::InitFromXml(FdoXmlSaxContext* pContext, FdoXmlAttributeCollection* attrs)
{
    try
    {
        if (attrs == NULL || pContext == NULL) 
            throw FdoCommandException::Create(NlsMsgGet(GRFP_3_NULL_ARGUMENT, "A required argument was set to NULL"));

        BaseType::InitFromXml(pContext, attrs);

        FdoInt32 frameNumber = 1;
        FdoXmlAttributeP att = attrs->FindItem(FdoGrfpXmlGlobals::FrameNumber);
        if (att != NULL) {
            FdoStringP attrValue = att->GetLocalValue();
            swscanf(attrValue, L"%d", &frameNumber);
            if (frameNumber <= 0) {
                throw FdoCommandException::Create(
                    NlsMsgGet2(GRFP_99_INVALID_XML_ATTRIBUTE, 
                               "Raster Configuration file contains an invalid value '%1$ls' for XML attribute: '%1$ls'.",
                               (FdoString*)attrValue, FdoGrfpXmlGlobals::FrameNumber
                               )
                    );
            }
        }

        SetFrameNumber(frameNumber);
    }
    catch (FdoException* ex)
    {
        pContext->AddError(ex);
        ex->Release();
    }
}

// Handle the start of a sub-element
FdoXmlSaxHandler* FdoGrfpRasterImageDefinition::XmlStartElement(
   FdoXmlSaxContext* context, 
   FdoString* uri, 
   FdoString* name, 
   FdoString* qname, 
   FdoXmlAttributeCollection* atts)
{
    FdoXmlSaxHandler* pRet = NULL;

    try
    {
        if (context == NULL || name == NULL)
            throw FdoCommandException::Create(NlsMsgGet(GRFP_3_NULL_ARGUMENT, "A required argument was set to NULL"));

        pRet = BaseType::XmlStartElement(context, uri, name, qname, atts);
        if (pRet == NULL)
        {
            bool bOK = true;

            if (STREQUAL(name, FdoGrfpXmlGlobals::Georeference))
            {
                m_geoReference = FdoGrfpRasterGeoreferenceLocation::Create();
                if (m_state != 0)
                    bOK = false;
                m_state = 1;
            }
            else if (STREQUAL(name, FdoGrfpXmlGlobals::InsertionPointX))
            {
                m_state = 2;
            }
            else if (STREQUAL(name, FdoGrfpXmlGlobals::InsertionPointY))
            {
                m_state = 3;
            }
            else if (STREQUAL(name, FdoGrfpXmlGlobals::ResolutionX))
            {
                m_state = 4;
            }
            else if (STREQUAL(name, FdoGrfpXmlGlobals::ResolutionY))
            {
                m_state = 5;
            }
            else if (STREQUAL(name, FdoGrfpXmlGlobals::RotationX))
            {
                m_state = 6;
            }
            else if (STREQUAL(name, FdoGrfpXmlGlobals::RotationY))
            {
                m_state = 7;
            }
            else if (STREQUAL(name, FdoGrfpXmlGlobals::Bounds))
            {
                if (m_state != 0)
                    bOK = false;
                m_state = 8;
            }
            else if (STREQUAL(name, FdoGrfpXmlGlobals::MinX))
            {
                m_state = 9;
            }
            else if (STREQUAL(name, FdoGrfpXmlGlobals::MinY))
            {
                m_state = 10;
            }
            else if (STREQUAL(name, FdoGrfpXmlGlobals::MaxX))
            {
                m_state = 11;
            }
            else if (STREQUAL(name, FdoGrfpXmlGlobals::MaxY))
            {
                m_state = 12;
            }
            else
            {
                bOK = false;
            }

            if (!bOK)
                throw FdoCommandException::Create(NlsMsgGet(GRFP_94_INVALID_GEOREFERENCE_DEF, "Invalid georeference definition."));
        }
    }
    catch (FdoException* ex) 
    {
        context->AddError(ex);
        ex->Release();
    }
	
    return pRet; 
}

// Handle the end of a sub-element
FdoBoolean FdoGrfpRasterImageDefinition::XmlEndElement(
   FdoXmlSaxContext* context, 
   FdoString* uri, 
   FdoString* name, 
   FdoString* qname)
{
    try
    {
        if (context == NULL || name == NULL)
            throw FdoCommandException::Create(NlsMsgGet(GRFP_3_NULL_ARGUMENT, "A required argument was set to NULL"));

        if (BaseType::XmlEndElement(context, uri, name, qname))
            return true;

        bool bOK = true;
        switch (m_state)
        {
          case 1:
          {
              if (!STREQUAL(name, FdoGrfpXmlGlobals::Georeference))
                  bOK = false;
              break;
          }
          case 2:
          {
              if (!STREQUAL(name, FdoGrfpXmlGlobals::InsertionPointX))
                  bOK = false;
              break;
          }
          case 3:
          {
              if (!STREQUAL(name, FdoGrfpXmlGlobals::InsertionPointY))
                  bOK = false;
              break;
          }
          case 4:
          {
              if (!STREQUAL(name, FdoGrfpXmlGlobals::ResolutionX) != 0)
                  bOK = false;
              break;
          }
          case 5:
          {
              if (!STREQUAL(name, FdoGrfpXmlGlobals::ResolutionY) != 0)
                  bOK = false;
              break;
          }
          case 6:
          {
              if (!STREQUAL(name, FdoGrfpXmlGlobals::RotationX) != 0)
                  bOK = false;
              break;
          }
          case 7:
          {
              if (!STREQUAL(name, FdoGrfpXmlGlobals::RotationY) != 0)
                  bOK = false;
              break;
          }
          case 8:
          {
              if (!STREQUAL(name, FdoGrfpXmlGlobals::Bounds) != 0)
                  bOK = false;
              break;
          }
          case 9:
          {
              if (!STREQUAL(name, FdoGrfpXmlGlobals::MinX) != 0)
                  bOK = false;
              break;
          }
          case 10:
          {
              if (!STREQUAL(name, FdoGrfpXmlGlobals::MinY) != 0)
                  bOK = false;
              break;
          }
          case 11:
          {
              if (!STREQUAL(name, FdoGrfpXmlGlobals::MaxX) != 0)
                  bOK = false;
              break;
          }
          case 12:
          {
              if (!STREQUAL(name, FdoGrfpXmlGlobals::MaxY) != 0)
                  bOK = false;
              break;
          }
          default:
            bOK = false;
            break;
        }
        if (m_state >= 2 && m_state <=7)
            m_state = 1;
        else if (m_state >= 9 && m_state <=12)
            m_state = 8;
        else
            m_state = 0;

        if (!bOK)
            throw FdoCommandException::Create(NlsMsgGet(GRFP_94_INVALID_GEOREFERENCE_DEF, "Invalid georeference definition."));
    }
    catch (FdoException* ex) 
    {
        context->AddError(ex);
        ex->Release();
        return true;
    }

    return false;
}

// Copy the character strings in the XML stream 
void FdoGrfpRasterImageDefinition::XmlCharacters(FdoXmlSaxContext* context, FdoString* chars)
{
    try
    {
        if (context == NULL || chars == NULL)
            throw FdoCommandException::Create(NlsMsgGet(GRFP_3_NULL_ARGUMENT, "A required argument was set to NULL"));

        BaseType::XmlCharacters(context, chars);

        if( (m_state >= 2 && m_state <= 7) 
            || (m_state >= 9 && m_state <= 12) )
        {
            double temp = 0.0;
            if (EOF == swscanf(chars, L"%lf", &temp)) {
                throw FdoCommandException::Create(NlsMsgGet(GRFP_94_INVALID_GEOREFERENCE_DEF, "Invalid georeference definition."));
            }
            else 
            {
                switch (m_state)
                {
                  case 2:
                    m_geoReference->SetXInsertionPoint(temp);
                    break;
                  case 3:
                    m_geoReference->SetYInsertionPoint(temp);
                    break;
                  case 4:
                    m_geoReference->SetXResolution(temp);
                    break;
                  case 5:
                    m_geoReference->SetYResolution(temp);
                    break;
                  case 6:
                    m_geoReference->SetXRotation(temp);
                    break;
                  case 7:
                    m_geoReference->SetYRotation(temp);
                    break;

                  case 9:
                    m_minX = temp;
                    m_haveBounds = true;
                    break;
                  case 10:
                    m_minY = temp;
                    m_haveBounds = true;
                    break;
                  case 11:
                    m_maxX = temp;
                    m_haveBounds = true;
                    break;
                  case 12:
                    m_maxY = temp;
                    m_haveBounds = true;
                    break;

                  default:
                    break;
                }
            }
        }
    }
    catch (FdoException* ex) 
    {
        context->AddError(ex);
        ex->Release();
    }
}

// Write the Image Definition to the OUT stream
void FdoGrfpRasterImageDefinition::_writeXml( FdoXmlWriter* xmlWriter, const FdoXmlFlags* flags )
{
    if (xmlWriter == NULL || flags == NULL) 
        throw FdoCommandException::Create(NlsMsgGet(GRFP_3_NULL_ARGUMENT, "A required argument was set to NULL"));

    xmlWriter->WriteStartElement(FdoGrfpXmlGlobals::Image);
    BaseType::_writeXml(xmlWriter, flags);

    // write 'frameNumber' Attribute
    FdoInt32 frameNumber = GetFrameNumber();
    if (frameNumber > 0) {
        char buff[20];
        FdoCommonOSUtil::itoa(frameNumber, buff); 
        xmlWriter->WriteAttribute(FdoGrfpXmlGlobals::FrameNumber, FdoStringP(buff));
    }

    // <Georeference>
    xmlWriter->WriteStartElement(FdoGrfpXmlGlobals::Georeference);
	
    // <InsertionPointX>
    xmlWriter->WriteStartElement(FdoGrfpXmlGlobals::InsertionPointX);
    xmlWriter->WriteCharacters(FdoStringP::Format(L"%f", m_geoReference->GetXInsertionPoint()));
    xmlWriter->WriteEndElement(); 

    // <InsertionPointY>
    xmlWriter->WriteStartElement(FdoGrfpXmlGlobals::InsertionPointY);
    xmlWriter->WriteCharacters(FdoStringP::Format(L"%f", m_geoReference->GetYInsertionPoint()));
    xmlWriter->WriteEndElement(); 

    // <ResolutionX>
    xmlWriter->WriteStartElement(FdoGrfpXmlGlobals::ResolutionX);
    xmlWriter->WriteCharacters(FdoStringP::Format(L"%f", m_geoReference->GetXResolution()));
    xmlWriter->WriteEndElement(); 

    // <ResolutionY>
    xmlWriter->WriteStartElement(FdoGrfpXmlGlobals::ResolutionY);
    xmlWriter->WriteCharacters(FdoStringP::Format(L"%f", m_geoReference->GetYResolution()));
    xmlWriter->WriteEndElement();

    // <RotationX>
    xmlWriter->WriteStartElement(FdoGrfpXmlGlobals::RotationX);
    xmlWriter->WriteCharacters(FdoStringP::Format(L"%f", m_geoReference->GetXRotation()));
    xmlWriter->WriteEndElement(); 
	
    // <RotationY>
    xmlWriter->WriteStartElement(FdoGrfpXmlGlobals::RotationY);
    xmlWriter->WriteCharacters(FdoStringP::Format(L"%f", m_geoReference->GetYRotation()));
    xmlWriter->WriteEndElement();

    xmlWriter->WriteEndElement(); //end of Georeference definition

    if( m_haveBounds )
    {
        xmlWriter->WriteStartElement(FdoGrfpXmlGlobals::Bounds);

        // <MinX>
        xmlWriter->WriteStartElement(FdoGrfpXmlGlobals::MinX);
        xmlWriter->WriteCharacters(FdoStringP::Format(L"%f", m_minX));
        xmlWriter->WriteEndElement(); 

        // <MinY>
        xmlWriter->WriteStartElement(FdoGrfpXmlGlobals::MinY);
        xmlWriter->WriteCharacters(FdoStringP::Format(L"%f", m_minY));
        xmlWriter->WriteEndElement(); 
	
        // <MaxX>
        xmlWriter->WriteStartElement(FdoGrfpXmlGlobals::MaxX);
        xmlWriter->WriteCharacters(FdoStringP::Format(L"%f", m_maxX));
        xmlWriter->WriteEndElement(); 
	
        // <MaxY>
        xmlWriter->WriteStartElement(FdoGrfpXmlGlobals::MaxY);
        xmlWriter->WriteCharacters(FdoStringP::Format(L"%f", m_maxY));
        xmlWriter->WriteEndElement(); 

        xmlWriter->WriteEndElement(); 
    }

    xmlWriter->WriteEndElement(); //end of Image definition
}

FdoGrfpRasterGeoreferenceLocation * FdoGrfpRasterImageDefinition::GetGeoreferencedLocation()
{
    return FDO_SAFE_ADDREF(m_geoReference.p);
}

void FdoGrfpRasterImageDefinition::SetGeoreferencedLocation (FdoGrfpRasterGeoreferenceLocation * location)
{
    m_geoReference = FDO_SAFE_ADDREF(location);
}

