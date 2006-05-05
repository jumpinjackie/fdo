/*! \mainpage Introduction
<div class="mainpage">
<!-- This is the index.html page that the user first sees. -->

<p>The Feature Data Objects (FDO) API provides access to data in a 
data store. A provider is a specific implementation of the FDO API 
that provides access to data in a particular data store. The FDO 
Provider for SHP provides FDO with access to a SHP-based data store.
</p>
<p>The FDO Provider for SHP uses a standalone file format that
supports GIS data. The FDO Provider for SHP (Shape) has the 
following characteristics: 
</p>
<ul>
  <li> Read-only access is provided to pre-existing
    spatial and attribute data from an Environmental Systems
    Research Institute (ESRI) Shape file (SHP).
  <li> The FDO Provider for SHP can run in a multi-platform environment,
    including Windows and Linux.
  <li> A Shape file consists of three separate files: SHP (shape
    geometry), SHX (shape index), and DBF (shape attributes in
    dBASE format).
  <li> The FDO Provider for SHP accesses the information in each of the 
  three separate files, and treats each SHP, and its associated DBF file, 
  as a feature class with a single geometry property, and optionally, 
  with data attribute properties.
  <li> Schema configuration of the data store is provided to the
    FDO Provider for SHP through an XML file containing the
    Geographic Markup Language (GML) definition of the schema
    that maps SHP and DBF data in the data store to feature
    classes and property mappings in the FDO data model.
  <li> Although it does not support concurrency control (locking), the SHP 
  file format is a valid alternative to RDBMS.
</ul>
<p>
For more information, see <i>The Essential FDO</i> (FET_TheEssentialFDO.pdf) 
and the <i>FDO Developer's Guide</i> (FDG_FDODevGuide.pdf).
</p>

<br>
</div>
*/




