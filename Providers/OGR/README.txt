
=========================================================

          FDO OGR Provider Installation Notes

=========================================================


---------------------------------------------------------
1. Compiling

Windows:

Use the provided Visual Studio 2005 project file.
Make sure to set the correct locations for the OGR and FDO headers
and libraries in the project's Properties pages. GDAL/OGR must be
compiled and available before you compile the provider.

If your OGR is compiled with components that require extra libraries
to be linked in, you will need to add these to the list under
"Additional Libraries". An example is OCI.lib for Oracle support.


Linux:

Assuming you have GDAL 1.3.2 and FDO 3.x.x propertly installed, 
use the following command line call:

g++ -O2 -shared -Wall -fPIC -I/usr/local/fdo-3.x.x/include OgrProvider.cpp -L/usr/local/fdo-3.x.x/lib/ -lFDO -lgdal -o libOGRProvider.so


Make sure to replace /usr/local/fdo-3.x.x by the actual FDO location on your system.
This assumes that GDAL is installed in a global location like /usr/include and /usr/lib.

The output of this command will be the OGR provider library.



----------------------------------------------------------
2. Installing the provider


For both Windows and Linux, do the following:

    * Copy the provider library (OGRProvider.dll or libOGRProvider.so) to
      the location of FDO libraries. On Linux this is /usr/local/fdo-3.x.x/lib.
      On Windows, it would be in the \FDO subdirectory of where your MapGuide Server
      component is installed, e.g.
        C:\Program Files\MapGuideOpenSource\Server\Bin\FDO


    * Add an entry for the provider to the providers.xml file.

        <FeatureProvider>
            <Name>OSGeo.OGR.0.1</Name> 
            <DisplayName>FDO Provider for OGR</DisplayName> 
            <Description>Access to OGR data sources</Description> 
            <IsManaged>False</IsManaged> 
            <Version>0.1.0.0</Version> 
            <FeatureDataObjectsVersion>3.1.0.0</FeatureDataObjectsVersion> 
            <LibraryPath>OGRProvider.dll</LibraryPath> 
       </FeatureProvider>

      Make sure you match the FDO version to whatever the other entries in the
      provider.xml are using (in this example it is 3.1.0.0, but the provider
      will work with 3.0.0.0 also).


----------------------------------------------------------
3. Using the provider in MapGuide

Connecting to an OGR data source in MapGuide Studio is done by creating
a new Data Connection to the OGR Provider and setting the two OGR connection 
properties.

The property named DataSource is an OGR connection string. For example
it could point to an SHP file or an Oracle table. See OGR documentation
at http://www.remotesensing.org/gdal/ogr/ogr_formats.html for more
information.

The ReadOnly property indicates if the connection should be opened
for reading only. Set that to TRUE as the provider is currently read only.


----------------------------------------------------------
4. Release Notes

So far the OGR provider has been tested with:
    * SHP files
    * MapInfo files (.TAB)
    * Oracle Spatial
    * ESRI personal geodatabase (.mdb)
    * PostGIS (on PostgreSQL 8.1.4)

Linux platforms tested:
    * Gentoo x64, gcc 3.4


==========================================================

June 20, 2006 Traian Stanev