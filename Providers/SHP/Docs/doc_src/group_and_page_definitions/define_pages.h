/*! \mainpage Introduction
<div class="mainpage">
<!-- This is the index.html page that the user first sees. -->

<p>The Feature Data Objects (FDO) API provides access to data in a 
data store. A provider is a specific implementation of the FDO API 
that provides access to data in a particular data store. The FDO 
Provider for SHP provides FDO with access to a SHP-based data store.
</p>

<p>The standalone ESRI SHP file format supports GIS data using a number of 
separate files: SHP (shape geometry), SHX (shape index), PRJ (projection 
information), CPG (code page files), IDX (spatial index), and DBF (shape 
attributes in dBASE format). You must have either a SHP or DBF file present to 
connect to data or work with schemas. Otherwise, these files are optional. Files 
with empty records are created, if they do not exist initially.
</p>

<p>In AutoCAD Map 3D 2008, the Schema Editor treats each SHP file, and its 
associated DBF file, as a feature class with a single geometry property and, 
optionally, data attribute properties. The SHP file format supports a single 
writer at any time, with multiple readers.
</p>

<p>SHP files can include only one geometry type per file, but you can stored and 
copy to multiple SHP files in a folder. You can use a configuration file to 
support multiple schemas when you establish your connection to a folder 
containing SHP files.
</p>

<p>A SHP schema can support the following:
</p>
<ul>
  <li>A single geometry type per file
  <li>Spatial contexts (determined by coordinate system information in the PRJ file)
  <li>Auto ID generation
  <li>Null value constraints
  <li>These geometry types: point, line string, polygon, multi-point, multi-line 
  string, multi-polygon, linear ring, and line string segment.
</ul>
 
<p>
For more information, see <i>The Essential FDO</i> (FET_TheEssentialFDO.pdf) 
and the <i>FDO Developer's Guide</i> (FDG_FDODevGuide.pdf).
</p>

<br>
</div>
*/
/// \defgroup typedefs
