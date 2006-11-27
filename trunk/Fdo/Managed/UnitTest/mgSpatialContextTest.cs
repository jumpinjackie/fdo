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

using System;
using System.Diagnostics;
using OSGeo.Common.Io;
using OSGeo.Spatial;
using OSGeo.FDO.Xml;
using OSGeo.Common.Xml;
using OSGeo.FDO.Commands.SpatialContext;
using OSGeo.Geometry;

namespace Fdo_Test
{
    /// <summary>
    /// Summary description for mgSpatialContextTest.
    /// </summary>
    public class mgSpatialContextTest
    {
        public mgSpatialContextTest()
        {
            Console.WriteLine("start mgSpatialContextTest");
        }

        public void testXml()
        {
            Console.WriteLine("start mgSpatialContextTest.testXml");

            try 
            {  
                IoMemoryStream stream = new IoMemoryStream();
                IoMemoryStream stream2 = new IoMemoryStream();

                XmlSpatialContextWriter writer = new XmlSpatialContextWriter( new XmlWriter(stream) );        

                // Try to write nameless spatial context (should fail)
                bool bFailed = false;
                try 
                {
                    writer.WriteSpatialContext();
                }
                catch ( OSGeo.Common.Exception e ) 
                {
                    Debug.Assert( e.Message.Substring(e.Message.LastIndexOf(") ") + 1) == " Current spatial context has no name; cannot write to XML. ");
                    bFailed = true;
                }
                Debug.Assert(bFailed, "Writing nameless spatial context should have failed." );

                writer.Name = "SC_0" ;
                writer.CoordinateSystem = "Quebec MTM Zone 8 (NAD83)" ;

                // Try to write extentless spatial context (should fail)
                bFailed = false;
                try 
                {
                    writer.WriteSpatialContext();
                }
                catch ( OSGeo.Common.Exception e ) 
                {
                    Debug.Assert( e.Message.Substring(e.Message.LastIndexOf(") ") + 1) == " Current spatial context 'SC_0' has no extents; cannot write to XML. ");
                    bFailed = true;
                }
                Debug.Assert(bFailed, "Writing nameless spatial context should have failed." );

                // now for the happy tests.
                byte[] extent = SerializeExtent( 100000, 1000000, 200000, 1100000 );
                writer.Extent = extent;

                // write spatial context with no optional attributes set.
                writer.WriteSpatialContext();

                // write spatial contexts with optional attributes set.
                writer.Name = "Subway Map";
                writer.Description = "Diagram of subway stops";
                writer.CoordinateSystem = "Non-Earth (Meter)";
                writer.CoordinateSystemWkt = "LOCAL_CS [ \"Non-Earth (Meter)\", LOCAL_DATUM [\"Local Datum\", 0], UNIT [\"Meter\", 1.0], AXIS [\"X\", EAST], AXIS[\"Y\", NORTH]]" ;
                extent = SerializeExtent( 0, 0, 15000, 10000 );
                writer.Extent = extent;
                writer.ExtentType = SpatialContextExtentType.SpatialContextExtentType_Dynamic;
                writer.XYTolerance = 0.1;
                writer.ZTolerance = 0.01;
                writer.WriteSpatialContext();        
                writer.Dispose();
                stream.Reset();
                stream2.Reset();

                // Round trip the spatial contexts that were written to a 2nd XML document.
                // This tests whether we lose anything when deserializing and serializing.
                XmlCopy( stream, stream2 );

                // Format output files for comparison. Adds carriage returns.
                stream.Reset();
                IoFileStream fileStream = new IoFileStream( "TestFiles\\sc1a.xml", "w+");
                mgXmlFormatter formatter1a = new mgXmlFormatter(stream, fileStream);
                formatter1a.Format();

                stream2.Reset();
                fileStream = new IoFileStream( "TestFiles\\sc2a.xml", "w+");
                mgXmlFormatter formatter2a = new mgXmlFormatter(stream2, fileStream);
                formatter2a.Format();

                stream.Length = 0;
                stream2.Length = 0;

                // This next test writes an XML document with a single spatial context.

                writer = new XmlSpatialContextWriter( new XmlWriter(stream), new XmlSpatialContextFlags("www.ttc.ca" ));

                writer.Name = "Subway Map";
                writer.Description ="Diagram of subway stops";
                writer.CoordinateSystem = "Non-Earth (Meter)";
                extent = SerializeExtent( 0, 0, 15000, 10000 );
                writer.Extent = extent;
                writer.ExtentType = SpatialContextExtentType.SpatialContextExtentType_Dynamic;
                writer.XYTolerance = 0.1;
                writer.ZTolerance = 0.01;

                writer.WriteSpatialContext();
                writer = null;

                stream.Reset();
                stream2.Reset();

                // Test round tripping for second document
                XmlCopy( stream, stream2 );

                // Format output files for comparison. Adds carriage returns.
                stream.Reset();
                fileStream = new IoFileStream( "TestFiles\\sc1b.xml", "w+");
                mgXmlFormatter formatter1b = new mgXmlFormatter(stream, fileStream);
                formatter1b.Format();

                stream2.Reset();
                fileStream = new IoFileStream( "TestFiles\\sc2b.xml", "w+");
                mgXmlFormatter formatter2b = new mgXmlFormatter(stream2, fileStream);
                formatter2b.Format();

                // Compare output and expected results.
                mgUnitTestUtil.CheckOutput("TestFiles\\sc1a_master.txt", "TestFiles\\sc1a.xml");
                mgUnitTestUtil.CheckOutput("TestFiles\\sc1b_master.txt", "TestFiles\\sc1b.xml");
                mgUnitTestUtil.CheckOutput("TestFiles\\sc2a_master.txt", "TestFiles\\sc2a.xml");
                mgUnitTestUtil.CheckOutput("TestFiles\\sc2b_master.txt", "TestFiles\\sc2b.xml");
            }
            catch ( OSGeo.Common.Exception e) 
            {
                Console.WriteLine(e.Message);
            }            
            //Console.WriteLine("finish mgSpatialContextTest.testXml");            
        }

        public void testXmlError()
        {
            Console.WriteLine("start mgSpatialContextTest.testXmlError");

            try 
            { 
                // Read bad XML document under strict error checking
                XmlErrorLevel( "TestFiles\\sc_err1a_master.txt", "TestFiles\\sc_err1a.txt", XmlFlags.ErrorLevel.ErrorLevel_High);
                // Retry with lax error checking, should get fewer errors
                XmlErrorLevel( "TestFiles\\sc_err1b_master.txt", "TestFiles\\sc_err1b.txt", XmlFlags.ErrorLevel.ErrorLevel_Normal);
            }
            catch ( OSGeo.Common.Exception e ) 
            {
                Console.WriteLine(e.Message);
            }

            //Console.WriteLine("finish mgSpatialContextTest.testXmlError");            
        }
        
        public void testXmlNesting()
        {
            Console.WriteLine("start mgSpatialContextTest.testXmlNesting");

            try 
            {
                // Create an XML document with Spatial Contexts nested 2 levels down from
                // root element.
        
                IoMemoryStream stream = new IoMemoryStream();
                XmlWriter writer = new XmlWriter( stream );
        
                // Create 2 Level1 elements under the root element.
        
                writer.WriteStartElement( "Level1" );
                writer.WriteStartElement( "Level2" );
                writer.WriteEndElement();
                writer.WriteStartElement( "Level2" );
                writer.WriteEndElement();
                writer.WriteEndElement();
        
                // The Spatial Contexts are under the second Level1 element.
        
                writer.WriteStartElement( "Level1" );
        
                // Mix them up with some other elements that should be skipped.
                writer.WriteStartElement( "Level2" );
                writer.WriteEndElement();
        
                XmlSpatialContextWriter SCWriter = new XmlSpatialContextWriter(writer);
                SCWriter.Name = "Embedded1";
                byte[] extent = SerializeExtent( 0, 0, 15000, 10000 );
                SCWriter.Extent = extent;
                SCWriter.WriteSpatialContext();
                SCWriter = null;
        
                writer.WriteStartElement( "Level2" );
                writer.WriteEndElement();
        
                SCWriter = new XmlSpatialContextWriter( writer );
                SCWriter.Name = "Embedded2";
                extent = SerializeExtent( 0, 0, 15000, 10000 );
                SCWriter.Extent = extent;
                SCWriter.WriteSpatialContext();
                SCWriter = null;
        
                writer.WriteStartElement( "Level2" );
                writer.WriteEndElement();
                writer = null;
        
                stream.Reset();
        
                IoFileStream fileStream = new IoFileStream( "TestFiles\\sc_nested.xml", "w+");
                mgXmlFormatter formatter1 = new mgXmlFormatter(stream, fileStream);
                formatter1.Format();
                stream.Reset();
            }
            catch ( OSGeo.Common.Exception e ) 
            {
                Console.WriteLine(e.Message);
            }

            //Console.WriteLine("finish mgSpatialContextTest.testXmlNesting");
            
        }
        
        private byte[] SerializeExtent( double minX, double minY, double maxX, double maxY )
        {
            // Create a byte array 
            FgfGeometryFactory gf = new FgfGeometryFactory();
            IEnvelope env = gf.CreateEnvelopeXY(minX, minY, maxX, maxY );  
            IGeometry geom = gf.CreateGeometry(env);

            return (gf.GetFgf(geom));
        }
        
        private void XmlCopy( IoStream inStream, IoStream outStream )
        {
            XmlSpatialContextReader reader = new XmlSpatialContextReader(new XmlReader(inStream), new XmlSpatialContextFlags(null, XmlFlags.ErrorLevel.ErrorLevel_High));
        
            XmlSpatialContextWriter writer = new XmlSpatialContextWriter(new XmlWriter(outStream), new XmlSpatialContextFlags("www.nexturl.com", XmlSpatialContextFlags.ErrorLevel.ErrorLevel_High));
              
            while ( reader.ReadNext() ) 
            {
                writer.Name = reader.GetName();
                writer.Description =reader.GetDescription();
                writer.CoordinateSystem = reader.GetCoordinateSystem();
                writer.CoordinateSystemWkt = reader.GetCoordinateSystemWkt();
                writer.ExtentType = reader.GetExtentType();
                byte[] extent = reader.GetExtent();
                writer.Extent = extent;
                writer.XYTolerance = reader.GetXYTolerance();
                writer.ZTolerance = reader.GetZTolerance();
        
                writer.WriteSpatialContext();
            }
        }
        
        private void XmlErrorLevel( string masterFile, string errFile, XmlFlags.ErrorLevel errorLevel )
        {
            string wErrFile = errFile;
        
            IoFileStream errStream = new IoFileStream( wErrFile, "w+t" );
        
            XmlSpatialContextReader reader = new XmlSpatialContextReader(new XmlReader("TestFiles\\sc_error.xml"), new XmlSpatialContextFlags(null, errorLevel));
        
            int i;
            bool more;
        
            for ( more = true, i = 0; more && (i <100); i++) 
            {
                try 
                {
                    more = reader.ReadNext();
                }
                catch ( OSGeo.Common.Exception e ) 
                {
                    //Console.WriteLine(e.Message);
                }
            }
        
            //Debug.Assert(mgUnitTestUtil.CheckOutput(masterFile, errFile), "file not the same");        
        }       
    }
}
