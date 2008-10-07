/*! \mainpage Introduction
<div class="mainpage">
<!-- This is the index.html page that the user first sees. -->

<p>The Feature Data Objects (FDO) API provides access to data in a 
data store. A provider is a specific implementation of the FDO API 
that provides access to data in a particular data store. The FDO 
Provider for ESRI ArcSDE provides FDO with access to an ArcSDE-based data store 
(that is, with an underlying, supported Oracle or SQL Server 
database).
</p>

<p>You can connect via ArcSDE to an underlying Oracle or SQL Server database. 
You can insert, select, update, and delete feature data in existing ArcSDE 
schemas. However, you cannot create or modify an ArcSDE schema.
</p>

<p>To work with ArcSDE, you must install ArcSDE 9.1 and a supported data source, 
such as Oracle 9<i>i</i>, in the network. The host machine running AutoCAD Map 
3D, for example, must also have the required DLLs (three dynamically linked 
libraries, sde91.dll, sg91.dll, and pe91.dll) and the PATH environment variable 
must reference the local folder containing these DLLs. To accomplish this, you 
can install an ArcGIS 9.1 Desktop application or the ArcSDE SDK. For more 
information about ArcGIS 9.1 Desktop applications and the ArcSDE SDK, refer to 
the ESRI documentation.
</p>

<p>Furthermore, AutoCAD Map 3D uses facilities provided by ArcSDE for long 
transaction versioning and persistent locking. (ArcSDE supports one or the 
other, but not both, on the same class). ProductName uses the standard 
ArcSDE API and ArcSDE-supported storage on both the Oracle and SQL Server 
platforms. AutoCAD Map 3D does not use FDO metadata for ArcSDE schema—it uses 
existing metadata only.
</p>

<p>If ArcSDE encounters curved segments, it converts them to a series of line 
segments that approximate the original arc segment (an approximation of the 
original geometry). 
</p>

<p>
For more information, see <i>The Essential FDO</i> 
and the <i>FDO Developer's Guide</i>.
</p>

<br>
</div>
*/




