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
<p>The FDO Provider for ArcSDE has the following characteristics: 

</p>
<ul>
<li>You can connect to the FDO Provider for ArcSDE in one step if
    you already know the name of the data store that you want to
    use. Otherwise, you must connect in two steps.</li>
<li>With the FDO Provider for ArcSDE, you can either lock rows in a table or 
    version a table. You cannot do both at the
    same time. To do either one, you must first alter the table’s
    registration.</li>
<li>The FDO Provider for ArcSDE does not support the creation or
    destruction of feature schema (that is,it does not support the
    FdoIApplySchema and FdoIDestroySchema commands.) However, it
    does support the FdoIDescribeSchema command. The FDO Provider 
    for ArcSDE is intended to operate on already existing
    feature schemas, and it supports inserting,
    selecting, updating, and deleting data in existing schemas.</li>
</ul>
<p>
For more information, see <i>The Essential FDO</i> (FET_TheEssentialFDO.pdf) 
and the <i>FDO Developer's Guide</i> (FDG_FDODevGuide.pdf).
</p>

<br>
</div>
*/




