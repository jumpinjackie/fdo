#include "StdAfx.h"
#include "c_KgKmlFdoClassConfig.h"

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>

XERCES_CPP_NAMESPACE_USE

#define D_CFG_FILENAME L"KingKmlFdo.xml"

#define D_XML_FDO_CLASS_KML "FdoClassKml"
#define D_XML_CLASS_NAME "FdoClassName"
#define D_XML_FDO_PROP_FOR_PLACEMARK_NAME "PlacemarkName"
#define D_XML_FDO_PROP_FOR_PLACEMARK_DESC "PlacemarkDescription"
#define D_XML_FDO_PROP_FOR_GEOMETRY_EXTRUDE "GemoetryExtrude"
#define D_XML_FDO_PROP_FOR_GEOMETRY_ALTITUDE_MODE "GemoetryAltitudeMode"
#define D_XML_FDO_PROP_FOR_GEOMETRY_TESSELLATE "GemoetryTessellate"

class XStr
{
public :
  // -----------------------------------------------------------------------
  //  Constructors and Destructor
  // -----------------------------------------------------------------------
  XStr(const char* const toTranscode)
  {
    // Call the private transcoding method
    fUnicodeForm = XMLString::transcode(toTranscode);
  }

  ~XStr()
  {
    XMLString::release(&fUnicodeForm);
  }


  // -----------------------------------------------------------------------
  //  Getter methods
  // -----------------------------------------------------------------------
  const XMLCh* unicodeForm() const
  {
    return fUnicodeForm;
  }

private :
  // -----------------------------------------------------------------------
  //  Private data members
  //
  //  fUnicodeForm
  //      This is the Unicode XMLCh format of the string.
  // -----------------------------------------------------------------------
  XMLCh*   fUnicodeForm;
};

#define X(str) XStr(str).unicodeForm()
// From MapGuide UnicodeString.h

// Define shortcuts to convert to/from wchar_t strings to XMLCh strings on
// each platform.  Since Windows and Linux have different wchar_t representations,
// these macros are platforms dependant
#ifdef _WIN32
// On Windows, wchar_t and XMLCh are both UTF-16.  No conversion is necessary
#   define W2X(str) (const XMLCh*) str
#   define X2W(str) (const wchar_t*) str
#else
// On Linux, wchar_t is UTF-32.  Do the appropriate conversion
#   define W2X(oldstr) (XMLCh*) (UnicodeString::UTF32toUTF16((const LCh*) oldstr).c_str())
#   define X2W(oldstr) (const wchar_t*) UnicodeString::UTF16toUTF32(oldstr).c_str()
#endif



c_KgKmlFdoClassAttributeConfig::c_KgKmlFdoClassAttributeConfig(void)
{
  m_IsParsed = false; 
}


c_KgKmlFdoClassAttributeConfig::~c_KgKmlFdoClassAttributeConfig(void)
{
}

void c_KgKmlFdoClassAttributeConfig::Dispose()
{
  delete this;
}

void c_KgKmlFdoClassAttributeConfig::ParseExp( )
{
  m_IsParsed = true;
  try
  {
    m_FdoExp = FdoExpression::Parse(m_Exp);
  }
  catch(FdoException* ex)
  {
    m_FdoExp=NULL;
    ex->Release();
  }

}

FDOKGKML_API bool c_KgKmlFdoClassAttributeConfig::GetIsFdoExp() 
{ 
  if( !m_IsParsed ) ParseExp();

  return m_FdoExp.p ? true : false;
}
FDOKGKML_API FdoExpression* c_KgKmlFdoClassAttributeConfig::GetFdoExpr()
{
  if( !m_IsParsed ) ParseExp();

  return FDO_SAFE_ADDREF(m_FdoExp.p);
}



c_KgKmlFdoClassConfig::c_KgKmlFdoClassConfig(void)
{
  m_AttrCollection = new c_KgKmlFdoClassAttributeConfigCollection; 
}


c_KgKmlFdoClassConfig::~c_KgKmlFdoClassConfig(void)
{
}

void c_KgKmlFdoClassConfig::Dispose()
{
  delete this;
}


void c_KgKmlFdoClassConfig::ParseExp( FdoString* Exp,FdoPtr<FdoExpression>& FdoExp,bool& IsParsed )
{
  IsParsed = true;
  try
  {
    FdoExp = FdoExpression::Parse(Exp);
  }
  catch(FdoException* ex)
  {
    FdoExp=NULL;
    ex->Release();
  }
  
}





void c_KgKmlFdoClassConfigCollection::Dispose()
{
  delete this;
}

c_KgKmlFdoClassConfig* c_KgKmlFdoClassConfigCollection::FindKmlFdoClassConfig( FdoString* ClassName )
{
    FdoPtr<c_KgKmlFdoClassConfig> fdoclass;
  int count = GetCount();
  for(int ind=0;ind<count;ind++)
  {
    fdoclass = GetItem(ind)  ;
    if( fdoclass->m_ClassName.ICompare(ClassName) == 0 )
    {
      return FDO_SAFE_ADDREF(fdoclass.p);
    }
  }       

  return NULL;
}

extern wchar_t g_HomeDir[];
static wchar_t g_CfgFile[MAX_PATH*2];

DOMElement* AppendChildAndText(DOMDocument* Doc,DOMElement* Element,const XMLCh* ChildElementName,const XMLCh* TextTorWrite)
{


  DOMElement*    childelem = Doc->createElement(ChildElementName);
  Element->appendChild(childelem);

  if( TextTorWrite )
  {
    DOMText*    provider_text = Doc->createTextNode(TextTorWrite);
    childelem->appendChild(provider_text);
  }

  return childelem;
}

const XMLCh* GetTextFromElement(DOMElement* Element)
{

  DOMNode* child = Element->getFirstChild();
  while(child)
  {
    if(child->getNodeType() == DOMNode::TEXT_NODE)
    {
      return child->getNodeValue();
    }
    child = child->getNextSibling();
  }

  return NULL;
}

const XMLCh* GetTextFromChildElement(DOMElement* Element,const XMLCh* ChildElementName)
{

  DOMNodeList* list = Element->getElementsByTagName(ChildElementName);

  if( list->getLength() <= 0 ) return NULL;


  return GetTextFromElement((DOMElement*)list->item(0));
}

void GetClassDef(DOMElement* Element,c_KgKmlFdoClassConfig* ClassDef)
{
  const XMLCh* valsource = GetTextFromChildElement(Element,X(D_XML_CLASS_NAME));
  const XMLCh* valplacename = GetTextFromChildElement(Element,X(D_XML_FDO_PROP_FOR_PLACEMARK_NAME));
  const XMLCh* valplacedesc = GetTextFromChildElement(Element,X(D_XML_FDO_PROP_FOR_PLACEMARK_DESC));
  
  const XMLCh* valgeom_altmode = GetTextFromChildElement(Element,X(D_XML_FDO_PROP_FOR_GEOMETRY_ALTITUDE_MODE));
  const XMLCh* valgeom_tess = GetTextFromChildElement(Element,X(D_XML_FDO_PROP_FOR_GEOMETRY_TESSELLATE));
  


  ClassDef->SetClassName( X2W(valsource) );
  ClassDef->SetPlacemarkNameExp( X2W(valplacename) );
  ClassDef->SetPlacemarkDescExp( X2W(valplacedesc) );
  
  ClassDef->SetGeomAltModeExp( X2W(valgeom_altmode) );
  ClassDef->SetGeomTessellateExp( X2W(valgeom_tess) );
  
  /*
  DOMNodeList *nodelistdest = Element->getElementsByTagName(X(D_XML_PROPERTY_COPY_DEF));
  if( nodelistdest && (nodelistdest->getLength() > 0) )
  {
    FdoPtr<c_F2FPropertyCopyDefCollection> prop_copydef_coll = ClassCopyDef->GetPropertyCopyDefCollection();
    for(XMLSize_t ind =0;ind<nodelistdest->getLength();ind++)
    {
      DOMElement *prop_copydef_elem =   (DOMElement*)nodelistdest->item(ind);

      FdoPtr<c_F2FPropertyCopyDef> prop_copydef = new c_F2FPropertyCopyDef();
      GetPropertyCopyDef(prop_copydef_elem,prop_copydef);      
      prop_copydef_coll->Add( prop_copydef );
    }
  }
*/
}
void c_KgKmlFdoClassConfigCollection::ReadConfigXML()
{
  wcscpy (g_CfgFile, g_HomeDir);
  wcscat (g_CfgFile, D_CFG_FILENAME);
  
  XercesDOMParser *parser = new XercesDOMParser;

  FdoStringP fstr = g_CfgFile;
  parser->parse(fstr);
  DOMDocument* root = parser->getDocument();

  if( !root ) return ;
  DOMElement   *root_elem = root->getDocumentElement();
  if( !root_elem ) return ;
  
  {
    DOMNodeList *nodelistdest = root_elem->getElementsByTagName(X(D_XML_FDO_CLASS_KML));
    if( nodelistdest && (nodelistdest->getLength() > 0) )
    {
      for(XMLSize_t ind =0;ind<nodelistdest->getLength();ind++)
      {
        DOMElement *class_def_elem =   (DOMElement*)nodelistdest->item(ind);

        FdoPtr<c_KgKmlFdoClassConfig> class_def = new c_KgKmlFdoClassConfig();
        GetClassDef(class_def_elem,class_def);      
        Add( class_def );
      }
    }
  }
  
}