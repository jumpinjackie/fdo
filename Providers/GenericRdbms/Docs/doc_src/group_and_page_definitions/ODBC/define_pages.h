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
  <li> The FDO Provider for ODBC supports the definition of one or more feature classes in terms of
    any relational database table that contains an X, Y, and
    optionally, Z columns.</li>
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

<p>
For more information, see <i>The Essential FDO</i> (FET_TheEssentialFDO.pdf) 
and the <i>FDO Developer's Guide</i> (FDG_FDODevGuide.pdf).
</p>

<br>
</div>
*/




