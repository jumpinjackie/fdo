/*! \mainpage Introduction
<div class="mainpage">
<!-- This is the index.html page that the user first sees. -->

<p>The Feature Data Objects (FDO) API provides access to data in a 
data store. A provider is a specific implementation of the FDO API 
that provides access to data in a particular data store. The FDO 
Provider for MySQL provides FDO with access to a MySQL-based data store.
</p>
<p>The FDO Provider for MySQL API provides custom commands that are
specifically designed to work with the FDO API. For example, using 
these commands, you can do the following:

</p>
<ul>
  <li> Gather information about a provider.</li>
  <li> Transmit client services exceptions.</li>
  <li> Get lists of accessible data stores.</li>
  <li> Create connection objects.</li>
  <li> Create and execute spatial queries.</li>
</ul>
<p>There is also support for spatial data types.
</p>

<p>The MySQL architecture supports different storage engines. Choose an 
engine as needed, depending on its characteristics and capabilities, such as the following: 
</p>
<ul>
  <li> MyISAM is a disk-based storage engine. It does not
    support transactions.</li>
  <li> InnoDB is a disk-based storage engine. It has full ACID transaction
    capability.</li>
  <li> Memory (Heap) is a storage engine utilizing only RAM. It
    is very fast.</li>
  <li> BDB is the Berkley DB storage engine. It supports transactions.</li>
  <li> NDB is the MySQL Cluster storage engine.</li>
  <li> MERGE is a variation of MyISAM. A MERGE table is a
    collection of identical MyISAM tables, which means that
    all tables have the same columns, column types, indexes, and so on.</li>
</ul>
<p>
For more information, see <i>The Essential FDO</i> (FET_TheEssentialFDO.pdf) 
and the <i>FDO Developer's Guide</i> (FDG_FDODevGuide.pdf).
</p>

<br>
</div>
*/




