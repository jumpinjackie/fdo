//
// Copyright (C) 2004-2007  Autodesk, Inc.
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

///////////////////////////////////////////////////////////////////// 
//
// Ignore these following constructs.  These will not be wrapped by the 
// FDO python wrapper.
//
///////////////////////////////////////////////////////////////////// 

%ignore FdoArray::operator[];
%ignore FdoIDisposableCollection::Dispose(); 
%ignore FdoIException::Dispose(); 
%ignore NLSGetMessage; 
%ignore FdoArrayHelper;
%ignore FdoStringElement::Create;
%ignore FdoStringElement::operator=;
%ignore FdoStringElement::GetString;	
%ignore FdoXmlReader::GetTextReader;
%ignore FdoXmlReader::PopSaxHandler;
%ignore FdoXmlReader::GetSaxHandler;
%ignore FdoXmlReader::PushSaxHandler;
%ignore FdoIoBufferStream::Create;
%ignore FdoXmlWriter::UriToQName;

///////////////////////////////////////////////////////////////////// 
//
// Import the basic FDO and Fdo common types
//
///////////////////////////////////////////////////////////////////// 

%include "Common/Std.h"
%include "Common/IDisposable.h"
%include "Common/Disposable.h"
%include "Common/Collection.h"
%include "Common/FdoTypes.h"
%include "FdoStd.h"
%include "Common/Exception.h"
%include "Common/Array.h"
 
/* Handle FdoStringP */
%ignore FdoStringP::operator FdoString*;
%ignore FdoStringP::operator wchar_t*;
%ignore FdoStringP::operator const char*;
%ignore FdoStringP::operator char*;
%ignore FdoStringP::Utf8FromUnicode;
%ignore FdoStringP::Utf8ToUnicode;
%ignore FdoStringP::Utf8Len(char const*);
%ignore FdoStringP::FdoStringP(char const *);
%ignore FdoStringP::operator =(FdoString *);
%ignore FdoStringP::operator +(FdoStringP const) const; 
%ignore FdoStringP::operator +=(FdoStringP const) ;
%ignore FdoStringP::operator ==(FdoString const *) const;

%ignore FdoDirectPositionImpl;
//%ignore FdoGeometryFactoryAbstract::CreateGeometry;


%include "Common/StringP.h"

// Handle the FdoStringCollection
//
// Note: Do NOT wrap the FdoPtr<FdoStringCollection>.  This causes problems
// with calls to FdoException::Dispose(), which causes linking problems.
%ignore FdoPtr<FdoStringCollection>;
%ignore FdoStringCollection::Create(FdoStringCollection const *);
%template (BaseFdoStringCollection) FdoCollection<FdoStringElement,FdoException >;
%include "Common/StringCollection.h"

/* Handle FdoDictionary */
%include "Common/DictionaryElement.h"
%template (BaseFdoDictionaryElementCollection) FdoCollection<FdoDictionaryElement,FdoException >;
%include "Common/NamedCollection.h"
%template (BaseFdoDictionaryElementNamedCollection) FdoNamedCollection<FdoDictionaryElement,FdoException >;
%include "Common/Dictionary.h"

// Handle FdoVector
//
// Note: Do NOT wrap the FdoPtr<FdoVector>.  This causes problems
// with calls to FdoException::Dispose(), which causes linking problems.
%template (BaseVectorElementCollection) FdoCollection<FdoVectorElement,FdoException >;
%include "Common/Vector.h"

/* Handle the ByteStreamReader */
%include "Common/Io/Stream.h"
%include "Common/IStreamReader.h"
%include "Common/Io/BufferStream.h"
%include "Common/Io/MemoryStream.h"
%include "Common/Io/FileStream.h"
%include "Common/Io/ObjectStreamReader.h"
%include "Common/IStreamReaderTmpl.h"
%template (BaseFdoIoByStreamReaderTmpl) FdoIStreamReaderTmpl<FdoByte >;
%template (BaseFdoIoByStreamReader) FdoIoObjectStreamReader<FdoByte >;
%include "Common/Io/ByteStreamReader.h"

/* Handle XmlAttributeCollection */
%template (BaseFdoXmlCollection) FdoCollection<FdoXmlAttribute,FdoXmlException >;
%template (BaseFdoXmlNamedCollection) FdoNamedCollection<FdoXmlAttribute,FdoXmlException >;
%include "Common/Xml/AttributeCollection.h"

/* Handle the rest of FdoCommon.h */
%include "Common/Context.h"
%include "Common/GeometryType.h"
%include "Common/Dimensionality.h"
%include "Common/Xml/Xml.h"
%include "Common/Xml/XmlException.h"
%include "Common/Xml/Attribute.h"
%include "Common/ReadOnlyNamedCollection.h"
%include "Common/RestrictedNamedCollection.h"
%include "Common/Io/TextReader.h"
%include "Common/Io/TextWriter.h"
%include "Common/Xml/SaxContext.h"
%include "Common/Xml/SaxHandler.h"
%include "Common/Xml/CharDataHandler.h"
%include "Common/Xml/SkipElementHandler.h"
%include "Common/Xml/Reader.h"
%include "Common/Xml/Writer.h"
%include "Common/Xml/CopyHandler.h"
%include "Common/Xsl/Transformer.h"
%include "Common/Gml212/Schema.h"
%include "Common/Gml212/Gml212.h"
%include "Common/Stack.h"