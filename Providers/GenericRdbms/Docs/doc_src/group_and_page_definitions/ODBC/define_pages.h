/*! \mainpage Introduction
<div class="mainpage">
<!-- This is the index.html page that the user first sees. -->

<p>The Feature Data Objects (FDO) API provides access to data in a 
data store. A provider is a specific implementation of the FDO API 
that provides access to data in a particular data store. The FDO 
Provider for ODBC provides FDO with access to an ODBC-based data store.
</p>
<p>The FDO Provider for ODBC can access simple x, y, z
feature objects that can run in a multi-platform
environment, including Windows, Linux, and UNIX.
</p>

<p>The FDO Provider for ODBC has the following characteristics:
</p>
<ul>
  <li> The FDO Provider for ODBC supports the definition of one or more 
    feature classes in terms of any relational database table that 
    contains an X, Y, and optionally, Z columns.</li>
  <li> Metadata--that maps the table name, and X, Y, and optionally, Z
    columns to a feature class--is maintained outside the
    database in a configuration file. This information, in
    conjunction with the table structure in the database,
    provides the definition of the feature class.</li>
  <li> The x, y, and z locations of objects are stored in
    separate properties in the primary object definition of a
    feature, but are accessible through a single class
    property ‘Geometry’.</li>
<li> Read-only access is provided to pre-existing data defined and
    populated through 3rd party applications (that is, FDO
    Provider for ODBC will not be responsible for defining the
    physical schema of the data store nor for populating the object
    data).</li>
<li> The schema configuration of the data store is
    provided to the FDO Provider for ODBC through an optional XML
    file containing the Geographic Markup Language (GML)
    definition of the schema that maps ‘tables’ and ‘columns’ in
    the data store to feature classes and property mappings in
    the FDO data model.</li>
</ul>
\note
Microsoft Excel (must have at least one named range; do not use DATABASE 
or other reserved words as a range name) 

<p>For more information about setting up an ODBC data store in AutoCAD Map 3D
2008 and configuring Excel for database ranges, see the 'Managing Data' topic
in the <i>AutoCAD Map 3D User's Guide</i> (acmap.chm).
</p>

<p>For ODBC, only a point geometry (X, Y, and optionally, Z columns define a 
point) is supported. The default column names are X, Y, and Z, but when you pick 
the columns to represent these, you override the defaults. In AutoCAD Map 3D 
2008, this information is stored in the map file itself, so if you go through 
the same process in another map, you will need to override the settings again 
in the same way.
</p>

<p>There is no spatial indexing. Object locations are stored in separate 
properties in the object definition.
<p>

<p>The existing schema is used, and you cannot add, change, or delete it, nor 
can you add FDO metadata to the data store.
</p>

<p>When you set up a Data Source Name for your ODBC data store (by adding a DSN 
in Windows), you will need to specify certain configuration options, depending 
on the type of ODBC data store you use. The connection information (Login ID 
and password) you enter here is used only for defining the DSN, and not at 
connection time with the ODBC provider. 
</p>

\note
For MySQL, make sure you have installed the MySQL driver from MyODBC 3.51. 
You can find it at http://dev.mysql.com/downloads/connector/odbc/3.51.html.

<p>
For more information, see <i>The Essential FDO</i> (FET_TheEssentialFDO.pdf) 
and the <i>FDO Developer's Guide</i> (FDG_FDODevGuide.pdf).
</p>

<br>
</div>
*/




