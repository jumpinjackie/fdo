function getWordOccurences(word) {
var result = new Array();
if ("/a".match(word)) {
result = result.concat([[31,1],[32,1]]);
}
if ("/app/oracle/orahome".match(word)) {
result = result.concat([[54,2]]);
}
if ("/bin".match(word)) {
result = result.concat([[49,1]]);
}
if ("/build".match(word)) {
result = result.concat([[59,2],[61,26]]);
}
if ("/checkoutsvn".match(word)) {
result = result.concat([[47,4]]);
}
if ("/etc".match(word)) {
result = result.concat([[54,4]]);
}
if ("/etc/ld".match(word)) {
result = result.concat([[54,2]]);
}
if ("/etc/odbc".match(word)) {
result = result.concat([[54,1],[68,1]]);
}
if ("/home/opensource".match(word)) {
result = result.concat([[42,2],[43,2],[44,2],[45,2],[47,4],[61,2]]);
}
if ("/home/opensource/fdo/unittest".match(word)) {
result = result.concat([[64,1]]);
}
if ("/home/opensource/providers/arcsde/src/unittest".match(word)) {
result = result.concat([[65,1]]);
}
if ("/home/opensource/providers/gdal/src/unittest".match(word)) {
result = result.concat([[66,1]]);
}
if ("/home/opensource/providers/genericrdbms/src/unittest".match(word)) {
result = result.concat([[67,1],[68,1]]);
}
if ("/home/opensource/providers/sdf/src/unittest".match(word)) {
result = result.concat([[69,1]]);
}
if ("/home/opensource/providers/shp/src/unittest".match(word)) {
result = result.concat([[70,1]]);
}
if ("/home/opensource/providers/wms/src/unittest".match(word)) {
result = result.concat([[71,1]]);
}
if ("/home/opensource\\providers\\arcsde".match(word)) {
result = result.concat([[44,2],[45,2]]);
}
if ("/home/opensource\\providers\\gdal".match(word)) {
result = result.concat([[44,2],[45,2]]);
}
if ("/home/opensource\\providers\\genericrdbms".match(word)) {
result = result.concat([[44,2],[45,2]]);
}
if ("/home/opensource\\providers\\sdf".match(word)) {
result = result.concat([[44,2],[45,2]]);
}
if ("/home/opensource\\providers\\shp".match(word)) {
result = result.concat([[44,2],[45,2]]);
}
if ("/home/opensource\\providers\\wfs".match(word)) {
result = result.concat([[44,2],[45,2]]);
}
if ("/home/opensource\\providers\\wms".match(word)) {
result = result.concat([[44,2],[45,2]]);
}
if ("/install".match(word)) {
result = result.concat([[54,1]]);
}
if ("/isql".match(word)) {
result = result.concat([[54,1]]);
}
if ("/odbc".match(word)) {
result = result.concat([[54,1]]);
}
if ("/tcp".match(word)) {
result = result.concat([[29,8],[65,8]]);
}
if ("/unittest".match(word)) {
result = result.concat([[64,1],[65,4],[66,1],[69,1],[70,1],[71,1]]);
}
if ("/unittestodbc".match(word)) {
result = result.concat([[68,1]]);
}
if ("/urs/local".match(word)) {
result = result.concat([[54,1]]);
}
if ("/usr/bin".match(word)) {
result = result.concat([[40,1],[49,5],[50,1]]);
}
if ("/usr/include/mysql".match(word)) {
result = result.concat([[55,1]]);
}
if ("/usr/lib/mysql".match(word)) {
result = result.concat([[55,1]]);
}
if ("/usr/local".match(word)) {
result = result.concat([[54,6]]);
}
if ("/usr/local/bin".match(word)) {
result = result.concat([[50,1]]);
}
if ("/usr/local/easysoft/unixodbc".match(word)) {
result = result.concat([[55,1]]);
}
if ("/usr/local/easysoft/unixodbc/bin".match(word)) {
result = result.concat([[54,1]]);
}
if ("/usr/local/fdo".match(word)) {
result = result.concat([[55,1],[59,2],[61,40]]);
}
if ("0".match(word)) {
result = result.concat([[12,1],[15,1],[16,6],[38,1],[40,2],[52,3],[53,6],[54,15],[59,4],[61,28]]);
}
if ("0/docs/html/fdo".match(word)) {
result = result.concat([[61,2]]);
}
if ("0/docs/xmlschema".match(word)) {
result = result.concat([[61,2]]);
}
if ("0/include".match(word)) {
result = result.concat([[61,16]]);
}
if ("0/lib".match(word)) {
result = result.concat([[55,1],[59,2],[61,18]]);
}
if ("0/nls".match(word)) {
result = result.concat([[61,2]]);
}
if ("0\\bin".match(word)) {
result = result.concat([[16,1]]);
}
if ("1".match(word)) {
result = result.concat([[2,2],[8,7],[12,4],[13,3],[15,8],[22,2],[39,2],[40,10],[45,7],[49,4],[50,1],[52,7],[54,2],[59,1]]);
}
if ("1/providers/arcsde".match(word)) {
result = result.concat([[8,2],[45,2]]);
}
if ("1/providers/gdal".match(word)) {
result = result.concat([[8,2],[45,2]]);
}
if ("1/providers/genericrdbms".match(word)) {
result = result.concat([[8,2],[45,2]]);
}
if ("1/providers/sdf".match(word)) {
result = result.concat([[8,2],[45,2]]);
}
if ("1/providers/shp".match(word)) {
result = result.concat([[8,2],[45,2]]);
}
if ("1/providers/wfs".match(word)) {
result = result.concat([[8,2],[45,2]]);
}
if ("1/providers/wms".match(word)) {
result = result.concat([[8,2],[45,2]]);
}
if ("10g".match(word)) {
result = result.concat([[54,1]]);
}
if ("10gr1".match(word)) {
result = result.concat([[54,3]]);
}
if ("113mb".match(word)) {
result = result.concat([[2,2]]);
}
if ("115mb".match(word)) {
result = result.concat([[39,2]]);
}
if ("12".match(word)) {
result = result.concat([[13,1],[32,1],[39,2]]);
}
if ("13".match(word)) {
result = result.concat([[39,2]]);
}
if ("17".match(word)) {
result = result.concat([[59,2]]);
}
if ("2".match(word)) {
result = result.concat([[7,26],[8,21],[13,3],[15,1],[39,4],[40,1],[44,26],[45,21],[50,1],[54,1],[55,1],[59,2],[61,68]]);
}
if ("2000".match(word)) {
result = result.concat([[28,6],[32,1]]);
}
if ("2005".match(word)) {
result = result.concat([[12,2]]);
}
if ("2007".match(word)) {
result = result.concat([[13,1],[50,1]]);
}
if ("21".match(word)) {
result = result.concat([[52,1]]);
}
if ("22".match(word)) {
result = result.concat([[52,2],[53,2]]);
}
if ("24".match(word)) {
result = result.concat([[40,1]]);
}
if ("25".match(word)) {
result = result.concat([[59,1]]);
}
if ("3".match(word)) {
result = result.concat([[7,5],[8,5],[22,2],[32,1],[38,1],[40,3],[44,5],[45,5],[54,8],[55,1],[59,2],[61,68]]);
}
if ("3/bin/".match(word)) {
result = result.concat([[40,2]]);
}
if ("32".match(word)) {
result = result.concat([[54,1]]);
}
if ("4".match(word)) {
result = result.concat([[12,5],[13,2],[49,1],[50,1]]);
}
if ("46".match(word)) {
result = result.concat([[40,1]]);
}
if ("5".match(word)) {
result = result.concat([[2,2],[13,2],[16,5],[22,2],[49,2],[50,1],[53,4]]);
}
if ("50727".match(word)) {
result = result.concat([[12,1]]);
}
if ("51".match(word)) {
result = result.concat([[32,2]]);
}
if ("6".match(word)) {
result = result.concat([[39,2]]);
}
if ("7".match(word)) {
result = result.concat([[2,2],[40,1]]);
}
if ("84gb".match(word)) {
result = result.concat([[2,4]]);
}
if ("875".match(word)) {
result = result.concat([[12,2],[49,3]]);
}
if ("8\\common7\\ide".match(word)) {
result = result.concat([[12,1]]);
}
if ("8\\vc\\vcpackages".match(word)) {
result = result.concat([[12,1]]);
}
if ("8gb".match(word)) {
result = result.concat([[2,2]]);
}
if ("9".match(word)) {
result = result.concat([[15,7],[52,7],[59,1]]);
}
if (":".match(word)) {
result = result.concat([[59,2]]);
}
if ("\\".match(word)) {
result = result.concat([[29,4],[30,1],[34,1],[35,1]]);
}
if ("\\bin\\win32\\debug\\unittest".match(word)) {
result = result.concat([[29,4],[30,1],[34,1],[35,1]]);
}
if ("\\databases".match(word)) {
result = result.concat([[28,1]]);
}
if ("\\etc".match(word)) {
result = result.concat([[28,1]]);
}
if ("\\fdo\\docs".match(word)) {
result = result.concat([[24,10]]);
}
if ("\\fdo\\docs\\html\\arcsde".match(word)) {
result = result.concat([[24,1]]);
}
if ("\\fdo\\docs\\html\\fdo".match(word)) {
result = result.concat([[24,2]]);
}
if ("\\fdo\\docs\\html\\mysql".match(word)) {
result = result.concat([[24,1]]);
}
if ("\\fdo\\docs\\html\\odbc".match(word)) {
result = result.concat([[24,1]]);
}
if ("\\fdo\\docs\\html\\sdf".match(word)) {
result = result.concat([[24,1]]);
}
if ("\\fdo\\docs\\html\\shp".match(word)) {
result = result.concat([[24,1]]);
}
if ("\\fdo\\docs\\html\\wfs".match(word)) {
result = result.concat([[24,1]]);
}
if ("\\fdo\\docs\\html\\wms".match(word)) {
result = result.concat([[24,1]]);
}
if ("abasicopentests".match(word)) {
result = result.concat([[29,1]]);
}
if ("absolute".match(word)) {
result = result.concat([[5,1],[42,1]]);
}
if ("accept".match(word)) {
result = result.concat([[54,4]]);
}
if ("acquiring".match(word)) {
result = result.concat([[54,2]]);
}
if ("acquisition".match(word)) {
result = result.concat([[54,1]]);
}
if ("action".match(word)) {
result = result.concat([[22,3],[24,4],[54,1],[59,3],[61,1]]);
}
if ("action:".match(word)) {
result = result.concat([[22,1],[24,1],[59,1],[61,1]]);
}
if ("activestate".match(word)) {
result = result.concat([[12,1],[28,4]]);
}
if ("actual".match(word)) {
result = result.concat([[32,2]]);
}
if ("actually".match(word)) {
result = result.concat([[28,2]]);
}
if ("add".match(word)) {
result = result.concat([[13,1],[55,1],[61,2],[65,2]]);
}
if ("added".match(word)) {
result = result.concat([[17,1],[28,1]]);
}
if ("additional".match(word)) {
result = result.concat([[28,2]]);
}
if ("address".match(word)) {
result = result.concat([[31,1],[54,1],[67,1]]);
}
if ("adds".match(word)) {
result = result.concat([[55,2]]);
}
if ("admin".match(word)) {
result = result.concat([[32,1]]);
}
if ("again".match(word)) {
result = result.concat([[22,2],[28,4],[34,1],[59,2]]);
}
if ("against".match(word)) {
result = result.concat([[28,5]]);
}
if ("agree".match(word)) {
result = result.concat([[17,1]]);
}
if ("akefile".match(word)) {
result = result.concat([[59,1],[61,1]]);
}
if ("allows".match(word)) {
result = result.concat([[16,2]]);
}
if ("already".match(word)) {
result = result.concat([[40,1],[54,1]]);
}
if ("alternate".match(word)) {
result = result.concat([[13,1]]);
}
if ("alternatively".match(word)) {
result = result.concat([[28,2]]);
}
if ("answer".match(word)) {
result = result.concat([[54,3]]);
}
if ("apache".match(word)) {
result = result.concat([[22,4]]);
}
if ("api".match(word)) {
result = result.concat([[5,2],[9,2],[13,2],[17,1],[24,26],[42,2],[46,2],[50,1],[61,7]]);
}
if ("appears".match(word)) {
result = result.concat([[28,1]]);
}
if ("applications".match(word)) {
result = result.concat([[1,2],[38,2]]);
}
if ("appropriate".match(word)) {
result = result.concat([[5,2],[42,2]]);
}
if ("april".match(word)) {
result = result.concat([[13,1],[50,1]]);
}
if ("ar".match(word)) {
result = result.concat([[40,2]]);
}
if ("arcgis".match(word)) {
result = result.concat([[28,1]]);
}
if ("arcsde".match(word)) {
result = result.concat([[2,2],[10,2],[12,1],[14,2],[15,11],[24,8],[26,2],[28,24],[29,14],[39,2],[47,2],[49,1],[51,2],[52,9],[55,5],[61,8],[63,2],[65,9]]);
}
if ("are:".match(word)) {
result = result.concat([[10,2],[47,2]]);
}
if ("argument".match(word)) {
result = result.concat([[5,1],[24,2],[42,1]]);
}
if ("arguments".match(word)) {
result = result.concat([[10,3],[29,6],[47,3]]);
}
if ("ask".match(word)) {
result = result.concat([[28,4]]);
}
if ("asp".match(word)) {
result = result.concat([[13,1]]);
}
if ("aspx".match(word)) {
result = result.concat([[12,1]]);
}
if ("assigned".match(word)) {
result = result.concat([[32,2],[68,1]]);
}
if ("assigns".match(word)) {
result = result.concat([[55,2]]);
}
if ("assumption".match(word)) {
result = result.concat([[22,2],[59,2]]);
}
if ("assword".match(word)) {
result = result.concat([[10,1],[47,1]]);
}
if ("authentication".match(word)) {
result = result.concat([[28,1],[32,1]]);
}
if ("authorization".match(word)) {
result = result.concat([[28,1],[54,1]]);
}
if ("authorize".match(word)) {
result = result.concat([[28,1]]);
}
if ("autoconf".match(word)) {
result = result.concat([[49,1]]);
}
if ("automake".match(word)) {
result = result.concat([[49,1]]);
}
if ("automatically".match(word)) {
result = result.concat([[49,1],[54,1]]);
}
if ("available".match(word)) {
result = result.concat([[3,2],[14,2],[28,2],[36,1],[40,2],[51,2]]);
}
if ("aware".match(word)) {
result = result.concat([[18,2]]);
}
if ("back".match(word)) {
result = result.concat([[28,1]]);
}
if ("based".match(word)) {
result = result.concat([[3,2],[14,2],[40,2]]);
}
if ("basicarcsdetests".match(word)) {
result = result.concat([[29,1]]);
}
if ("basicconnectiontests".match(word)) {
result = result.concat([[29,1]]);
}
if ("basicdeletetests".match(word)) {
result = result.concat([[29,1]]);
}
if ("basicinserttests".match(word)) {
result = result.concat([[29,1]]);
}
if ("basicschematests".match(word)) {
result = result.concat([[29,1]]);
}
if ("basictransactiontests".match(word)) {
result = result.concat([[29,1]]);
}
if ("bat".match(word)) {
result = result.concat([[5,1],[10,9],[17,6],[18,4],[20,5],[22,10],[23,7],[24,8],[57,2]]);
}
if ("become".match(word)) {
result = result.concat([[4,2],[41,2]]);
}
if ("begin".match(word)) {
result = result.concat([[54,2]]);
}
if ("behavior".match(word)) {
result = result.concat([[54,2]]);
}
if ("below".match(word)) {
result = result.concat([[55,2]]);
}
if ("bin".match(word)) {
result = result.concat([[13,1]]);
}
if ("bin\\edgemt".match(word)) {
result = result.concat([[15,1]]);
}
if ("bin\\gsrvrdb291".match(word)) {
result = result.concat([[15,1]]);
}
if ("bin\\gsrvrinf91".match(word)) {
result = result.concat([[15,1]]);
}
if ("bin\\gsrvrora8i91".match(word)) {
result = result.concat([[15,1]]);
}
if ("bin\\gsrvrora9i91".match(word)) {
result = result.concat([[15,1]]);
}
if ("bin\\gsrvrsg191".match(word)) {
result = result.concat([[15,1]]);
}
if ("bin\\icudt221".match(word)) {
result = result.concat([[15,1]]);
}
if ("bin\\icuuc22".match(word)) {
result = result.concat([[15,1]]);
}
if ("bin\\libtiff".match(word)) {
result = result.concat([[15,1]]);
}
if ("bin\\loceng".match(word)) {
result = result.concat([[15,1]]);
}
if ("bin\\locssa".match(word)) {
result = result.concat([[15,1]]);
}
if ("bin\\mtchloc".match(word)) {
result = result.concat([[15,1]]);
}
if ("bin\\mtchmt".match(word)) {
result = result.concat([[15,1]]);
}
if ("bin\\pe91".match(word)) {
result = result.concat([[15,1]]);
}
if ("bin\\sde91".match(word)) {
result = result.concat([[15,1]]);
}
if ("bin\\sdedb2srvr91".match(word)) {
result = result.concat([[15,1]]);
}
if ("bin\\sdeinfsrvr91".match(word)) {
result = result.concat([[15,1]]);
}
if ("bin\\sdeora8isrvr91".match(word)) {
result = result.concat([[15,1]]);
}
if ("bin\\sdeora9isrvr91".match(word)) {
result = result.concat([[15,1]]);
}
if ("bin\\sdesqlsrvr91".match(word)) {
result = result.concat([[15,1]]);
}
if ("bin\\sg91".match(word)) {
result = result.concat([[15,1]]);
}
if ("bin\\xerces".match(word)) {
result = result.concat([[15,1]]);
}
if ("binaries".match(word)) {
result = result.concat([[1,4],[12,3],[22,3],[24,2],[38,4],[49,2],[59,2],[61,10]]);
}
if ("binary".match(word)) {
result = result.concat([[12,5],[40,1],[49,5],[55,1],[65,2]]);
}
if ("bison".match(word)) {
result = result.concat([[12,3],[23,2],[49,2],[60,2]]);
}
if ("bit".match(word)) {
result = result.concat([[54,1]]);
}
if ("boost".match(word)) {
result = result.concat([[22,12]]);
}
if ("box".match(word)) {
result = result.concat([[28,2],[54,1]]);
}
if ("boxes".match(word)) {
result = result.concat([[28,2]]);
}
if ("branch".match(word)) {
result = result.concat([[10,2],[47,2]]);
}
if ("branch:".match(word)) {
result = result.concat([[10,1]]);
}
if ("branched".match(word)) {
result = result.concat([[7,5],[8,5],[44,5],[45,5]]);
}
if ("branches".match(word)) {
result = result.concat([[10,1],[47,1]]);
}
if ("branchidentifier".match(word)) {
result = result.concat([[10,1],[47,1]]);
}
if ("briefer".match(word)) {
result = result.concat([[1,2],[38,2]]);
}
if ("browse".match(word)) {
result = result.concat([[3,2],[40,2],[54,3]]);
}
if ("browser".match(word)) {
result = result.concat([[3,2],[40,2]]);
}
if ("browsing".match(word)) {
result = result.concat([[10,1],[47,1]]);
}
if ("build".match(word)) {
result = result.concat([[1,12],[2,6],[5,4],[9,2],[10,4],[11,5],[12,7],[13,5],[15,2],[16,2],[17,6],[20,10],[21,9],[22,16],[23,9],[24,19],[26,2],[38,12],[39,6],[42,4],[46,2],[47,4],[48,5],[49,7],[50,5],[52,4],[54,3],[55,7],[57,10],[58,11],[59,7],[60,9],[61,37],[63,2],[65,2]]);
}
if ("builddocs".match(word)) {
result = result.concat([[24,1],[61,1]]);
}
if ("builddocs:".match(word)) {
result = result.concat([[22,1],[24,1],[61,1]]);
}
if ("buildfolderpath".match(word)) {
result = result.concat([[24,4]]);
}
if ("building".match(word)) {
result = result.concat([[0,5],[12,2],[13,2],[14,2],[19,5],[24,10],[37,5],[49,2],[50,2],[51,2],[56,5]]);
}
if ("buildinstall".match(word)) {
result = result.concat([[22,3],[24,4],[59,1],[61,19]]);
}
if ("builds".match(word)) {
result = result.concat([[12,2],[20,2],[53,2],[57,2]]);
}
if ("buildtype".match(word)) {
result = result.concat([[22,1],[24,1],[61,1]]);
}
if ("buildtype:".match(word)) {
result = result.concat([[22,1],[24,1],[61,1]]);
}
if ("built".match(word)) {
result = result.concat([[1,1],[22,20],[24,40],[38,1],[59,2],[61,1]]);
}
if ("bundled".match(word)) {
result = result.concat([[54,4]]);
}
if ("button".match(word)) {
result = result.concat([[28,2]]);
}
if ("buy".match(word)) {
result = result.concat([[15,2],[52,2]]);
}
if ("c".match(word)) {
result = result.concat([[12,2],[13,4],[15,1],[22,4],[24,4],[49,1],[50,4],[52,1],[59,2],[61,6]]);
}
if ("c:\\arcgis\\arcsde\\sqlexe".match(word)) {
result = result.concat([[28,1]]);
}
if ("c:\\arcgis\\arcsde\\sqlexe\\tools\\arcsdesetupsql".match(word)) {
result = result.concat([[28,1]]);
}
if ("c:\\esri\\arcsdeclient91\\windows\\".match(word)) {
result = result.concat([[15,2]]);
}
if ("c:\\opensource".match(word)) {
result = result.concat([[5,2],[6,2],[7,2],[8,2]]);
}
if ("c:\\opensource\\fdo\\unmanaged\\bin\\win32\\debug".match(word)) {
result = result.concat([[27,1]]);
}
if ("c:\\opensource\\providers\\arcsde".match(word)) {
result = result.concat([[7,2],[8,2]]);
}
if ("c:\\opensource\\providers\\arcsde\\src\\unittest".match(word)) {
result = result.concat([[29,2]]);
}
if ("c:\\opensource\\providers\\arcsde\\testdata".match(word)) {
result = result.concat([[28,4]]);
}
if ("c:\\opensource\\providers\\gdal".match(word)) {
result = result.concat([[7,2],[8,2]]);
}
if ("c:\\opensource\\providers\\gdal\\src\\unittest".match(word)) {
result = result.concat([[30,1]]);
}
if ("c:\\opensource\\providers\\gdal\\testdata".match(word)) {
result = result.concat([[30,1]]);
}
if ("c:\\opensource\\providers\\genericrdbms".match(word)) {
result = result.concat([[7,2],[8,2]]);
}
if ("c:\\opensource\\providers\\genericrdbms\\src\\unittest".match(word)) {
result = result.concat([[31,1],[32,1],[33,2]]);
}
if ("c:\\opensource\\providers\\sdf".match(word)) {
result = result.concat([[7,2],[8,2]]);
}
if ("c:\\opensource\\providers\\sdf\\src\\unittest".match(word)) {
result = result.concat([[34,1]]);
}
if ("c:\\opensource\\providers\\sdf\\testdata".match(word)) {
result = result.concat([[34,1]]);
}
if ("c:\\opensource\\providers\\shp".match(word)) {
result = result.concat([[7,2],[8,2]]);
}
if ("c:\\opensource\\providers\\shp\\src\\unittest".match(word)) {
result = result.concat([[35,1]]);
}
if ("c:\\opensource\\providers\\shp\\testdata".match(word)) {
result = result.concat([[35,1]]);
}
if ("c:\\opensource\\providers\\shp\\testdata\\clean".match(word)) {
result = result.concat([[35,1]]);
}
if ("c:\\opensource\\providers\\wfs".match(word)) {
result = result.concat([[7,2],[8,2]]);
}
if ("c:\\opensource\\providers\\wms".match(word)) {
result = result.concat([[7,2],[8,2]]);
}
if ("c:\\opensource\\providers\\wms\\bin\\win32\\debug".match(word)) {
result = result.concat([[36,1]]);
}
if ("c:\\perl\\bin".match(word)) {
result = result.concat([[12,1]]);
}
if ("c:\\program".match(word)) {
result = result.concat([[12,4],[13,3],[16,3]]);
}
if ("c:\\windows\\microsoft".match(word)) {
result = result.concat([[12,1]]);
}
if ("c:\\windows\\system32".match(word)) {
result = result.concat([[12,1]]);
}
if ("c:\\winnt\\system32\\drivers\\etc".match(word)) {
result = result.concat([[28,1]]);
}
if ("called".match(word)) {
result = result.concat([[5,1],[28,6],[42,1],[54,5]]);
}
if ("case".match(word)) {
result = result.concat([[2,4],[28,1],[39,4]]);
}
if ("cat".match(word)) {
result = result.concat([[61,2]]);
}
if ("cause".match(word)) {
result = result.concat([[28,1]]);
}
if ("causes".match(word)) {
result = result.concat([[34,1],[54,3],[61,20]]);
}
if ("cd".match(word)) {
result = result.concat([[12,2],[27,1],[29,2],[30,1],[31,1],[32,1],[33,2],[34,1],[35,1],[36,1],[54,2],[61,2],[64,1],[65,1],[66,1],[67,1],[68,1],[69,1],[70,1],[71,1]]);
}
if ("change".match(word)) {
result = result.concat([[12,2],[17,3],[23,2],[28,2],[55,2],[60,2]]);
}
if ("changed".match(word)) {
result = result.concat([[22,2]]);
}
if ("changes".match(word)) {
result = result.concat([[28,2]]);
}
if ("changing".match(word)) {
result = result.concat([[22,2],[59,2]]);
}
if ("chapter".match(word)) {
result = result.concat([[1,2],[38,2]]);
}
if ("characters".match(word)) {
result = result.concat([[28,1]]);
}
if ("check".match(word)) {
result = result.concat([[16,1],[54,6]]);
}
if ("checked".match(word)) {
result = result.concat([[28,1]]);
}
if ("checkout".match(word)) {
result = result.concat([[6,2],[7,16],[8,16],[10,13],[28,2],[43,2],[44,16],[45,16],[47,12]]);
}
if ("checkoutsvn".match(word)) {
result = result.concat([[5,1],[10,11],[22,2],[42,1],[47,7],[59,2]]);
}
if ("chm".match(word)) {
result = result.concat([[13,2],[24,18]]);
}
if ("class".match(word)) {
result = result.concat([[13,1],[50,1]]);
}
if ("clean".match(word)) {
result = result.concat([[22,1],[24,1],[31,1],[59,1],[61,6],[67,1],[68,2]]);
}
if ("click".match(word)) {
result = result.concat([[3,4],[4,2],[13,5],[28,15],[40,4],[41,2],[54,3]]);
}
if ("clicking".match(word)) {
result = result.concat([[10,2],[47,2]]);
}
if ("client".match(word)) {
result = result.concat([[2,2],[3,6],[5,2],[14,2],[15,15],[16,5],[39,2],[40,6],[42,2],[51,2],[52,15],[53,5],[54,7],[55,2],[65,2]]);
}
if ("clients".match(word)) {
result = result.concat([[1,1],[2,2],[3,2],[14,5],[38,1],[39,2],[40,2],[51,5],[55,2]]);
}
if ("cmd".match(word)) {
result = result.concat([[5,2],[10,2],[17,2],[22,2],[24,2],[27,2],[28,2],[29,2],[30,2],[31,2],[32,2],[33,4],[34,2],[35,3],[36,2],[69,2],[70,2],[71,2]]);
}
if ("code".match(word)) {
result = result.concat([[1,2],[3,4],[4,2],[6,5],[7,5],[8,5],[38,2],[40,4],[41,2],[43,5],[44,5],[45,5],[49,1]]);
}
if ("coded".match(word)) {
result = result.concat([[28,2]]);
}
if ("com".match(word)) {
result = result.concat([[13,1],[54,1]]);
}
if ("com/activeperl".match(word)) {
result = result.concat([[12,1],[28,2]]);
}
if ("com/downloads/connector/odbc/3".match(word)) {
result = result.concat([[32,1]]);
}
if ("com/downloads/mysql/5".match(word)) {
result = result.concat([[16,2],[53,2]]);
}
if ("com/library".match(word)) {
result = result.concat([[13,1]]);
}
if ("com/library/en".match(word)) {
result = result.concat([[13,1]]);
}
if ("com/netframework/downloads/updates/default".match(word)) {
result = result.concat([[12,1]]);
}
if ("com/software/arcgis/arcsde/how".match(word)) {
result = result.concat([[15,2],[52,2]]);
}
if ("combined".match(word)) {
result = result.concat([[61,1]]);
}
if ("comes".match(word)) {
result = result.concat([[12,1],[54,2]]);
}
if ("command".match(word)) {
result = result.concat([[3,2],[12,2],[22,2],[28,4],[29,4],[40,2],[61,6]]);
}
if ("command:".match(word)) {
result = result.concat([[6,2],[43,2]]);
}
if ("commands".match(word)) {
result = result.concat([[5,2],[42,2],[61,9],[65,2]]);
}
if ("commands:".match(word)) {
result = result.concat([[7,2],[8,2],[44,2],[45,2]]);
}
if ("commencing".match(word)) {
result = result.concat([[55,2]]);
}
if ("common".match(word)) {
result = result.concat([[61,2]]);
}
if ("company".match(word)) {
result = result.concat([[54,1]]);
}
if ("compiler".match(word)) {
result = result.concat([[17,1]]);
}
if ("completed".match(word)) {
result = result.concat([[28,2]]);
}
if ("completes".match(word)) {
result = result.concat([[54,1]]);
}
if ("component".match(word)) {
result = result.concat([[22,2],[59,2],[61,4]]);
}
if ("components".match(word)) {
result = result.concat([[5,2],[10,2],[12,1],[16,4],[21,4],[22,2],[26,2],[42,2],[47,2],[49,1],[58,4],[63,2]]);
}
if ("compressed".match(word)) {
result = result.concat([[13,1]]);
}
if ("conf".match(word)) {
result = result.concat([[54,2]]);
}
if ("configmakefiles".match(word)) {
result = result.concat([[59,1],[61,1]]);
}
if ("configmakefiles:".match(word)) {
result = result.concat([[59,1],[61,1]]);
}
if ("configuration".match(word)) {
result = result.concat([[12,1],[17,1],[28,3]]);
}
if ("configure".match(word)) {
result = result.concat([[28,9],[32,5],[49,1],[59,3],[61,11],[65,2]]);
}
if ("configuring".match(word)) {
result = result.concat([[28,2],[59,2],[61,1]]);
}
if ("connect".match(word)) {
result = result.concat([[32,1],[54,2]]);
}
if ("connection".match(word)) {
result = result.concat([[28,1],[29,3]]);
}
if ("connections".match(word)) {
result = result.concat([[36,1]]);
}
if ("connector".match(word)) {
result = result.concat([[32,1]]);
}
if ("console".match(word)) {
result = result.concat([[28,1]]);
}
if ("contain".match(word)) {
result = result.concat([[18,2]]);
}
if ("contained".match(word)) {
result = result.concat([[9,2],[22,2],[24,2],[28,2],[46,2],[54,1],[61,2]]);
}
if ("containing".match(word)) {
result = result.concat([[13,1],[53,2],[54,1]]);
}
if ("contains".match(word)) {
result = result.concat([[16,2],[20,2],[28,2],[54,2],[57,2]]);
}
if ("contents".match(word)) {
result = result.concat([[31,1],[54,2],[67,1],[68,1]]);
}
if ("context".match(word)) {
result = result.concat([[12,1],[49,1]]);
}
if ("control".match(word)) {
result = result.concat([[3,2],[28,1],[40,2]]);
}
if ("controls".match(word)) {
result = result.concat([[49,1]]);
}
if ("converts".match(word)) {
result = result.concat([[12,1],[49,1]]);
}
if ("coordsys".match(word)) {
result = result.concat([[15,1],[52,1]]);
}
if ("copied".match(word)) {
result = result.concat([[22,8],[24,63]]);
}
if ("copies".match(word)) {
result = result.concat([[24,2]]);
}
if ("copy".match(word)) {
result = result.concat([[16,2],[28,2],[31,1],[32,1]]);
}
if ("core".match(word)) {
result = result.concat([[20,1],[21,2],[24,7],[26,2],[57,1],[63,2]]);
}
if ("corner".match(word)) {
result = result.concat([[4,2],[41,2]]);
}
if ("correct".match(word)) {
result = result.concat([[16,1],[17,1],[28,1]]);
}
if ("cp".match(word)) {
result = result.concat([[67,1],[68,1]]);
}
if ("cpp".match(word)) {
result = result.concat([[23,2],[60,2]]);
}
if ("create".match(word)) {
result = result.concat([[16,1],[28,15],[32,2],[54,1]]);
}
if ("created".match(word)) {
result = result.concat([[28,1],[29,2],[32,2],[34,1],[54,1],[61,12],[68,2]]);
}
if ("creates".match(word)) {
result = result.concat([[34,1]]);
}
if ("creators”".match(word)) {
result = result.concat([[32,1]]);
}
if ("cscript".match(word)) {
result = result.concat([[12,1]]);
}
if ("ction".match(word)) {
result = result.concat([[22,1],[24,1],[59,1],[61,1]]);
}
if ("curl".match(word)) {
result = result.concat([[22,4]]);
}
if ("current".match(word)) {
result = result.concat([[10,2],[47,2],[61,2]]);
}
if ("custom".match(word)) {
result = result.concat([[16,2],[28,2]]);
}
if ("d".match(word)) {
result = result.concat([[13,1],[22,1],[24,4],[50,1],[61,6]]);
}
if ("daemon".match(word)) {
result = result.concat([[54,1]]);
}
if ("data".match(word)) {
result = result.concat([[1,1],[28,15],[30,2],[34,2],[35,2],[38,1],[54,6],[68,1]]);
}
if ("database".match(word)) {
result = result.concat([[1,1],[28,22],[29,3],[32,1],[38,1],[54,7],[68,1]]);
}
if ("databases".match(word)) {
result = result.concat([[28,2],[29,1],[54,1]]);
}
if ("dataset".match(word)) {
result = result.concat([[28,3],[29,5],[65,4]]);
}
if ("datasetname".match(word)) {
result = result.concat([[28,2],[29,4],[65,4]]);
}
if ("date".match(word)) {
result = result.concat([[13,1],[50,1]]);
}
if ("dav".match(word)) {
result = result.concat([[40,2]]);
}
if ("dba".match(word)) {
result = result.concat([[32,1]]);
}
if ("dbg/unittestmysql".match(word)) {
result = result.concat([[67,1]]);
}
if ("dbg\\unittestmysql".match(word)) {
result = result.concat([[31,1]]);
}
if ("dbg\\unittestodbc".match(word)) {
result = result.concat([[33,7]]);
}
if ("dbinit".match(word)) {
result = result.concat([[28,1]]);
}
if ("debug".match(word)) {
result = result.concat([[1,1],[2,4],[16,3],[22,5],[24,7],[26,2],[28,1],[38,1],[39,4],[61,11],[63,2]]);
}
if ("decide".match(word)) {
result = result.concat([[18,2]]);
}
if ("default".match(word)) {
result = result.concat([[10,1],[16,2],[22,6],[24,6],[47,1],[54,4],[55,11],[59,4],[61,5],[68,1]]);
}
if ("defaults".match(word)) {
result = result.concat([[28,2]]);
}
if ("define".match(word)) {
result = result.concat([[12,2],[23,2],[28,1],[60,2]]);
}
if ("defined".match(word)) {
result = result.concat([[18,2]]);
}
if ("defs".match(word)) {
result = result.concat([[28,1]]);
}
if ("delete".match(word)) {
result = result.concat([[34,1],[61,1]]);
}
if ("dependencies".match(word)) {
result = result.concat([[39,2]]);
}
if ("dependenct".match(word)) {
result = result.concat([[51,2]]);
}
if ("dependent".match(word)) {
result = result.concat([[14,2]]);
}
if ("depending".match(word)) {
result = result.concat([[29,2]]);
}
if ("depends".match(word)) {
result = result.concat([[49,1]]);
}
if ("described".match(word)) {
result = result.concat([[28,2],[54,2],[65,2]]);
}
if ("describes:".match(word)) {
result = result.concat([[1,2],[38,2]]);
}
if ("description".match(word)) {
result = result.concat([[1,2],[12,2],[13,1],[38,2],[49,2],[50,1]]);
}
if ("desiredcomponent".match(word)) {
result = result.concat([[10,2],[47,2]]);
}
if ("destination".match(word)) {
result = result.concat([[10,1],[22,1],[24,1],[47,1]]);
}
if ("detects".match(word)) {
result = result.concat([[55,2]]);
}
if ("developer".match(word)) {
result = result.concat([[16,4]]);
}
if ("development".match(word)) {
result = result.concat([[13,1]]);
}
if ("devenv".match(word)) {
result = result.concat([[12,1]]);
}
if ("diagrams".match(word)) {
result = result.concat([[13,1],[50,1]]);
}
if ("dialog".match(word)) {
result = result.concat([[28,4]]);
}
if ("different".match(word)) {
result = result.concat([[22,1]]);
}
if ("differs".match(word)) {
result = result.concat([[54,2]]);
}
if ("dimitri/doxygen/download".match(word)) {
result = result.concat([[13,1],[50,1]]);
}
if ("dir".match(word)) {
result = result.concat([[28,1]]);
}
if ("directories".match(word)) {
result = result.concat([[55,4],[61,6]]);
}
if ("directory".match(word)) {
result = result.concat([[5,1],[10,2],[17,2],[38,2],[42,1],[47,4],[52,2],[54,5],[55,8],[59,2],[61,13]]);
}
if ("disable".match(word)) {
result = result.concat([[28,1]]);
}
if ("disk".match(word)) {
result = result.concat([[2,11],[39,11]]);
}
if ("displayed".match(word)) {
result = result.concat([[3,2],[10,2],[28,1],[40,2],[47,2]]);
}
if ("displaying".match(word)) {
result = result.concat([[28,1]]);
}
if ("distribution".match(word)) {
result = result.concat([[59,2],[61,2]]);
}
if ("dll".match(word)) {
result = result.concat([[15,22],[16,2],[22,12],[24,51]]);
}
if ("dlls".match(word)) {
result = result.concat([[20,2],[22,12],[24,4],[57,2]]);
}
if ("dnshostnameoripaddr".match(word)) {
result = result.concat([[29,4],[65,4]]);
}
if ("docs".match(word)) {
result = result.concat([[61,1]]);
}
if ("document".match(word)) {
result = result.concat([[3,2],[15,2],[40,4],[49,1],[53,2],[54,6]]);
}
if ("documentation".match(word)) {
result = result.concat([[2,2],[5,2],[13,9],[17,1],[42,2],[50,8],[61,23]]);
}
if ("documented".match(word)) {
result = result.concat([[1,1],[38,1]]);
}
if ("doing".match(word)) {
result = result.concat([[4,2],[28,2],[41,2]]);
}
if ("done".match(word)) {
result = result.concat([[28,2],[53,2]]);
}
if ("dot".match(word)) {
result = result.concat([[50,1]]);
}
if ("double".match(word)) {
result = result.concat([[13,4]]);
}
if ("download".match(word)) {
result = result.concat([[4,2],[5,2],[12,1],[13,1],[16,2],[32,1],[41,2],[42,2],[49,1],[50,1],[53,2],[54,6]]);
}
if ("downloaded".match(word)) {
result = result.concat([[5,2],[32,1],[42,2]]);
}
if ("downloading".match(word)) {
result = result.concat([[41,2]]);
}
if ("downloads".match(word)) {
result = result.concat([[13,2]]);
}
if ("doxygen".match(word)) {
result = result.concat([[13,5],[17,2],[50,3]]);
}
if ("draw".match(word)) {
result = result.concat([[13,1],[50,1]]);
}
if ("drawing".match(word)) {
result = result.concat([[13,1],[50,1]]);
}
if ("driver".match(word)) {
result = result.concat([[32,2],[39,2],[54,30],[55,2],[68,1]]);
}
if ("dsn".match(word)) {
result = result.concat([[54,4]]);
}
if ("dsnmysql".match(word)) {
result = result.concat([[68,2]]);
}
if ("dsnoracle".match(word)) {
result = result.concat([[68,2]]);
}
if ("dsnsqlserver".match(word)) {
result = result.concat([[68,2]]);
}
if ("during".match(word)) {
result = result.concat([[12,2],[13,2],[22,4],[26,2],[49,3],[50,2],[53,2],[59,2],[61,2],[63,2],[68,1]]);
}
if ("earth".match(word)) {
result = result.concat([[40,2]]);
}
if ("easysoft".match(word)) {
result = result.concat([[54,14],[68,1]]);
}
if ("edit".match(word)) {
result = result.concat([[28,4],[31,1],[32,3],[67,1],[68,1]]);
}
if ("editor".match(word)) {
result = result.concat([[12,1],[49,1]]);
}
if ("effect".match(word)) {
result = result.concat([[22,2],[24,2]]);
}
if ("either".match(word)) {
result = result.concat([[10,1],[28,1],[41,2],[47,1]]);
}
if ("elp".match(word)) {
result = result.concat([[10,1],[22,1],[24,1],[47,1],[59,1],[61,1]]);
}
if ("email".match(word)) {
result = result.concat([[54,1]]);
}
if ("embedded".match(word)) {
result = result.concat([[13,1],[50,1]]);
}
if ("enableoraclesetup".match(word)) {
result = result.concat([[68,2]]);
}
if ("enables".match(word)) {
result = result.concat([[5,2],[42,2]]);
}
if ("encounter".match(word)) {
result = result.concat([[28,2]]);
}
if ("engine".match(word)) {
result = result.concat([[12,1]]);
}
if ("enter".match(word)) {
result = result.concat([[10,2],[22,2],[24,2],[47,2],[54,7],[59,2],[61,4]]);
}
if ("entered".match(word)) {
result = result.concat([[54,1]]);
}
if ("enterprise".match(word)) {
result = result.concat([[28,3],[29,1],[32,1]]);
}
if ("entitled".match(word)) {
result = result.concat([[52,2],[54,1]]);
}
if ("entry".match(word)) {
result = result.concat([[68,1]]);
}
if ("environment".match(word)) {
result = result.concat([[16,1],[17,8],[18,2],[28,5],[52,2],[54,1],[55,8],[65,2]]);
}
if ("equivalent".match(word)) {
result = result.concat([[28,1]]);
}
if ("error".match(word)) {
result = result.concat([[28,1]]);
}
if ("es".match(word)) {
result = result.concat([[38,1]]);
}
if ("esri".match(word)) {
result = result.concat([[15,4],[52,4]]);
}
if ("etc/services".match(word)) {
result = result.concat([[52,1]]);
}
if ("examples".match(word)) {
result = result.concat([[29,2]]);
}
if ("exception".match(word)) {
result = result.concat([[29,4]]);
}
if ("exclusive".match(word)) {
result = result.concat([[2,2],[39,2]]);
}
if ("exe".match(word)) {
result = result.concat([[5,2],[10,2],[12,2],[13,3],[17,3],[22,2],[24,2],[27,2],[28,7],[29,8],[30,3],[31,3],[33,11],[34,6],[35,4],[36,3],[50,1],[67,1],[69,2],[70,2],[71,2]]);
}
if ("executable".match(word)) {
result = result.concat([[17,1],[50,1]]);
}
if ("executables".match(word)) {
result = result.concat([[1,1],[26,2],[38,1],[49,1],[61,1],[63,2]]);
}
if ("execute".match(word)) {
result = result.concat([[5,2],[6,2],[7,2],[8,2],[28,2],[42,2],[43,2],[44,2],[45,2]]);
}
if ("executed".match(word)) {
result = result.concat([[29,4]]);
}
if ("executing".match(word)) {
result = result.concat([[28,2],[35,2]]);
}
if ("execution".match(word)) {
result = result.concat([[29,4]]);
}
if ("exercise".match(word)) {
result = result.concat([[32,1],[54,1]]);
}
if ("exist".match(word)) {
result = result.concat([[55,2]]);
}
if ("existence".match(word)) {
result = result.concat([[55,2]]);
}
if ("exit".match(word)) {
result = result.concat([[54,1]]);
}
if ("expects".match(word)) {
result = result.concat([[54,1]]);
}
if ("experience".match(word)) {
result = result.concat([[35,1]]);
}
if ("explained".match(word)) {
result = result.concat([[65,2]]);
}
if ("explains".match(word)) {
result = result.concat([[54,4]]);
}
if ("explorer".match(word)) {
result = result.concat([[28,1]]);
}
if ("export".match(word)) {
result = result.concat([[54,1],[55,1]]);
}
if ("expression".match(word)) {
result = result.concat([[12,2],[20,1],[21,2],[23,2],[57,1],[58,2],[60,2]]);
}
if ("f".match(word)) {
result = result.concat([[68,1]]);
}
if ("fail".match(word)) {
result = result.concat([[36,1]]);
}
if ("failures".match(word)) {
result = result.concat([[35,1]]);
}
if ("false".match(word)) {
result = result.concat([[68,4]]);
}
if ("fax".match(word)) {
result = result.concat([[54,1]]);
}
if ("fay".match(word)) {
result = result.concat([[40,2]]);
}
if ("fdg".match(word)) {
result = result.concat([[24,1]]);
}
if ("fdo".match(word)) {
result = result.concat([[0,5],[1,4],[3,4],[4,2],[5,4],[6,5],[7,5],[8,5],[9,2],[10,3],[12,6],[13,2],[14,2],[17,1],[19,5],[20,2],[21,4],[22,3],[23,4],[24,28],[26,2],[27,5],[37,5],[38,4],[40,4],[41,2],[42,4],[43,5],[44,5],[45,5],[46,2],[47,3],[49,4],[50,2],[55,1],[56,5],[57,2],[58,4],[60,4],[61,13],[63,2],[64,5]]);
}
if ("fdo\\bin".match(word)) {
result = result.concat([[22,8],[24,22]]);
}
if ("fdo\\bin\\com".match(word)) {
result = result.concat([[24,2]]);
}
if ("fdo\\inc".match(word)) {
result = result.concat([[24,8]]);
}
if ("fdo\\inc\\sdf".match(word)) {
result = result.concat([[24,2]]);
}
if ("fdo\\inc\\shp".match(word)) {
result = result.concat([[24,2]]);
}
if ("fdo\\lib".match(word)) {
result = result.concat([[24,12]]);
}
if ("fdoarcsde".match(word)) {
result = result.concat([[10,3],[28,2],[47,3]]);
}
if ("fdocommon".match(word)) {
result = result.concat([[61,2]]);
}
if ("fdocore".match(word)) {
result = result.concat([[5,1],[10,7],[42,1],[47,6],[61,2]]);
}
if ("fdodevguide".match(word)) {
result = result.concat([[24,1]]);
}
if ("fdogdal".match(word)) {
result = result.concat([[10,2]]);
}
if ("fdogeometry".match(word)) {
result = result.concat([[61,2]]);
}
if ("fdomessage".match(word)) {
result = result.concat([[61,4]]);
}
if ("fdomysql".match(word)) {
result = result.concat([[17,2],[55,3]]);
}
if ("fdoodbc".match(word)) {
result = result.concat([[54,3],[55,4]]);
}
if ("fdordbms".match(word)) {
result = result.concat([[10,3],[47,3]]);
}
if ("fdosdf".match(word)) {
result = result.concat([[10,3],[47,3]]);
}
if ("fdoshp".match(word)) {
result = result.concat([[10,3],[47,3]]);
}
if ("fdothirdparty".match(word)) {
result = result.concat([[17,1],[55,1]]);
}
if ("fdothirdparty/esri/arcsdeclient91/linux".match(word)) {
result = result.concat([[55,1]]);
}
if ("fdothirdparty/mysql/rhlinux".match(word)) {
result = result.concat([[55,1]]);
}
if ("fdoutilities".match(word)) {
result = result.concat([[17,1],[55,1]]);
}
if ("fdowfs".match(word)) {
result = result.concat([[10,3],[47,3]]);
}
if ("fdowms".match(word)) {
result = result.concat([[10,3],[47,3]]);
}
if ("feature".match(word)) {
result = result.concat([[1,1],[38,1]]);
}
if ("fet".match(word)) {
result = result.concat([[24,1]]);
}
if ("fifth".match(word)) {
result = result.concat([[28,2]]);
}
if ("file".match(word)) {
result = result.concat([[1,2],[9,2],[10,1],[12,1],[18,4],[22,2],[24,18],[28,2],[29,1],[31,1],[32,1],[38,2],[41,2],[46,2],[47,1],[54,10],[67,1],[68,3]]);
}
if ("files".match(word)) {
result = result.concat([[1,1],[5,4],[10,1],[12,2],[13,2],[15,2],[16,1],[17,1],[18,7],[20,2],[21,2],[23,10],[24,87],[28,2],[34,2],[38,1],[42,4],[47,1],[49,4],[52,2],[54,6],[55,1],[57,2],[58,2],[59,2],[60,10],[61,12]]);
}
if ("files:".match(word)) {
result = result.concat([[20,2],[57,2]]);
}
if ("files\\att\\graphviz\\bin".match(word)) {
result = result.concat([[13,1]]);
}
if ("files\\doxygen\\bin".match(word)) {
result = result.concat([[13,1]]);
}
if ("files\\gnuwin32\\bin".match(word)) {
result = result.concat([[12,2]]);
}
if ("files\\html".match(word)) {
result = result.concat([[13,1]]);
}
if ("files\\microsoft".match(word)) {
result = result.concat([[12,2]]);
}
if ("files\\mysql\\mysql".match(word)) {
result = result.concat([[16,3]]);
}
if ("filesystem".match(word)) {
result = result.concat([[28,1]]);
}
if ("fill".match(word)) {
result = result.concat([[28,2]]);
}
if ("filter".match(word)) {
result = result.concat([[12,3],[20,1],[21,2],[23,2],[49,1],[57,1],[58,2],[60,2]]);
}
if ("final".match(word)) {
result = result.concat([[2,2],[28,1],[39,2]]);
}
if ("finally".match(word)) {
result = result.concat([[58,2]]);
}
if ("find".match(word)) {
result = result.concat([[28,2],[52,2],[54,1],[55,3],[65,2]]);
}
if ("first".match(word)) {
result = result.concat([[4,2],[21,2],[28,2],[41,2],[58,2]]);
}
if ("five".match(word)) {
result = result.concat([[28,1]]);
}
if ("folder".match(word)) {
result = result.concat([[1,2],[5,6],[9,2],[10,8],[15,2],[16,2],[17,4],[20,2],[22,9],[24,50],[28,6],[42,6],[46,2],[47,6],[57,2]]);
}
if ("folders".match(word)) {
result = result.concat([[13,1],[16,1]]);
}
if ("follow".match(word)) {
result = result.concat([[3,2],[4,2],[40,2],[41,2]]);
}
if ("follow:".match(word)) {
result = result.concat([[29,2]]);
}
if ("followed".match(word)) {
result = result.concat([[54,2]]);
}
if ("following".match(word)) {
result = result.concat([[6,2],[7,2],[8,2],[12,2],[13,2],[17,2],[22,2],[24,2],[27,2],[29,4],[30,2],[31,2],[33,4],[34,2],[35,2],[36,2],[40,2],[43,2],[44,2],[45,2],[49,2],[50,2],[55,1],[59,2],[61,10],[64,2],[65,2],[66,2],[67,2],[68,2],[69,2],[70,2],[71,2]]);
}
if ("following:".match(word)) {
result = result.concat([[28,4],[32,2],[54,2]]);
}
if ("follows".match(word)) {
result = result.concat([[15,2],[52,2]]);
}
if ("follows:".match(word)) {
result = result.concat([[10,2],[47,2]]);
}
if ("form".match(word)) {
result = result.concat([[28,2],[29,2]]);
}
if ("formattedcomments".match(word)) {
result = result.concat([[13,1],[50,1]]);
}
if ("found".match(word)) {
result = result.concat([[54,1]]);
}
if ("foundation".match(word)) {
result = result.concat([[3,2],[4,2],[40,2],[41,2]]);
}
if ("four".match(word)) {
result = result.concat([[61,2]]);
}
if ("fourth".match(word)) {
result = result.concat([[28,2]]);
}
if ("framework".match(word)) {
result = result.concat([[17,1]]);
}
if ("free".match(word)) {
result = result.concat([[12,1],[49,1]]);
}
if ("full".match(word)) {
result = result.concat([[17,1],[54,1]]);
}
if ("fully".match(word)) {
result = result.concat([[65,2]]);
}
if ("gb".match(word)) {
result = result.concat([[39,4]]);
}
if ("gdal".match(word)) {
result = result.concat([[10,3],[22,5],[24,4],[26,2],[30,6],[47,2],[61,6],[63,2],[66,5]]);
}
if ("gdal1".match(word)) {
result = result.concat([[22,2]]);
}
if ("gdal13".match(word)) {
result = result.concat([[22,3]]);
}
if ("gdalfile".match(word)) {
result = result.concat([[61,2]]);
}
if ("general".match(word)) {
result = result.concat([[12,1],[28,1],[49,1]]);
}
if ("generate".match(word)) {
result = result.concat([[13,2],[17,1],[21,2],[58,2]]);
}
if ("generated".match(word)) {
result = result.concat([[23,2],[26,2],[60,2],[61,18],[63,2]]);
}
if ("generates".match(word)) {
result = result.concat([[13,1],[49,1],[50,1]]);
}
if ("generation".match(word)) {
result = result.concat([[49,1]]);
}
if ("generator".match(word)) {
result = result.concat([[12,1],[49,1]]);
}
if ("generic".match(word)) {
result = result.concat([[24,4]]);
}
if ("genericrdbms".match(word)) {
result = result.concat([[5,1],[42,1]]);
}
if ("geometry".match(word)) {
result = result.concat([[61,2]]);
}
if ("geospatial".match(word)) {
result = result.concat([[3,2],[4,2],[40,2],[41,2]]);
}
if ("giomgr".match(word)) {
result = result.concat([[28,1]]);
}
if ("give".match(word)) {
result = result.concat([[24,1]]);
}
if ("glibc".match(word)) {
result = result.concat([[54,6]]);
}
if ("gnu".match(word)) {
result = result.concat([[49,1]]);
}
if ("go".match(word)) {
result = result.concat([[4,2],[13,1],[32,1],[41,2]]);
}
if ("grammar".match(word)) {
result = result.concat([[12,3],[20,1],[21,2],[23,2],[49,3],[57,1],[58,2],[60,2]]);
}
if ("grant".match(word)) {
result = result.concat([[32,2]]);
}
if ("graph".match(word)) {
result = result.concat([[13,1],[50,1]]);
}
if ("graphviz".match(word)) {
result = result.concat([[13,4],[17,1],[50,3]]);
}
if ("greater".match(word)) {
result = result.concat([[49,1]]);
}
if ("grfp".match(word)) {
result = result.concat([[24,2]]);
}
if ("group\\".match(word)) {
result = result.concat([[28,1]]);
}
if ("gzipped".match(word)) {
result = result.concat([[41,2]]);
}
if ("h".match(word)) {
result = result.concat([[10,4],[15,9],[22,4],[23,2],[24,16],[47,4],[52,9],[59,4],[60,2],[61,12]]);
}
if ("hand".match(word)) {
result = result.concat([[3,2],[4,2],[40,2],[41,2]]);
}
if ("happens".match(word)) {
result = result.concat([[49,1]]);
}
if ("hard".match(word)) {
result = result.concat([[28,2]]);
}
if ("hat".match(word)) {
result = result.concat([[38,1],[49,1]]);
}
if ("header".match(word)) {
result = result.concat([[61,4]]);
}
if ("headers".match(word)) {
result = result.concat([[52,2]]);
}
if ("help".match(word)) {
result = result.concat([[3,2],[10,2],[13,7],[17,1],[22,2],[24,2],[40,2],[47,2],[61,2]]);
}
if ("help:".match(word)) {
result = result.concat([[10,1],[22,1],[24,1],[47,1],[59,1],[61,1]]);
}
if ("hhc".match(word)) {
result = result.concat([[17,1]]);
}
if ("hierarchy".match(word)) {
result = result.concat([[55,2]]);
}
if ("home".match(word)) {
result = result.concat([[54,1]]);
}
if ("home/network/admin/tnsnames".match(word)) {
result = result.concat([[54,1],[68,1]]);
}
if ("host".match(word)) {
result = result.concat([[12,1]]);
}
if ("hosting".match(word)) {
result = result.concat([[28,4],[29,4],[31,1],[67,1]]);
}
if ("hostname".match(word)) {
result = result.concat([[31,2],[54,1],[67,2]]);
}
if ("html".match(word)) {
result = result.concat([[12,1],[13,6],[15,2],[16,2],[17,1],[24,18],[32,1],[49,1],[50,1],[52,2],[53,2]]);
}
if ("htmlhelp".match(word)) {
result = result.concat([[13,1]]);
}
if ("http://dev".match(word)) {
result = result.concat([[16,2],[32,1],[53,2]]);
}
if ("http://fdo".match(word)) {
result = result.concat([[10,1],[47,1]]);
}
if ("http://gnuwin32".match(word)) {
result = result.concat([[12,1]]);
}
if ("http://msdn".match(word)) {
result = result.concat([[12,1],[13,2]]);
}
if ("http://prdownloads".match(word)) {
result = result.concat([[12,1]]);
}
if ("http://rpmfind".match(word)) {
result = result.concat([[49,1]]);
}
if ("http://the".match(word)) {
result = result.concat([[40,2]]);
}
if ("http://www".match(word)) {
result = result.concat([[12,1],[13,2],[15,2],[28,2],[50,2],[52,2],[54,5]]);
}
if ("httpd".match(word)) {
result = result.concat([[40,2]]);
}
if ("https://svn".match(word)) {
result = result.concat([[6,2],[7,16],[8,16],[43,2],[44,16],[45,16]]);
}
if ("https://www".match(word)) {
result = result.concat([[3,2],[4,2],[40,2],[41,2]]);
}
if ("i386".match(word)) {
result = result.concat([[40,3],[49,1]]);
}
if ("id".match(word)) {
result = result.concat([[10,1],[47,1]]);
}
if ("identical".match(word)) {
result = result.concat([[29,2]]);
}
if ("identified".match(word)) {
result = result.concat([[28,2]]);
}
if ("identifier".match(word)) {
result = result.concat([[10,1],[47,1]]);
}
if ("identifies".match(word)) {
result = result.concat([[5,4],[42,4],[54,2]]);
}
if ("identify".match(word)) {
result = result.concat([[24,1]]);
}
if ("idl".match(word)) {
result = result.concat([[13,1],[50,1]]);
}
if ("important".match(word)) {
result = result.concat([[16,1]]);
}
if ("include".match(word)) {
result = result.concat([[1,1],[16,1],[38,1],[55,3],[61,2]]);
}
if ("include/pe".match(word)) {
result = result.concat([[52,2]]);
}
if ("include/pedef".match(word)) {
result = result.concat([[52,1]]);
}
if ("include/pef".match(word)) {
result = result.concat([[52,1]]);
}
if ("include/sdeerno".match(word)) {
result = result.concat([[52,1]]);
}
if ("include/sderaster".match(word)) {
result = result.concat([[52,1]]);
}
if ("include/sdetype".match(word)) {
result = result.concat([[52,1]]);
}
if ("include/sg".match(word)) {
result = result.concat([[52,1]]);
}
if ("include/sgerr".match(word)) {
result = result.concat([[52,1]]);
}
if ("include\\pe".match(word)) {
result = result.concat([[15,2]]);
}
if ("include\\pedef".match(word)) {
result = result.concat([[15,1]]);
}
if ("include\\pef".match(word)) {
result = result.concat([[15,1]]);
}
if ("include\\sdeerno".match(word)) {
result = result.concat([[15,1]]);
}
if ("include\\sderaster".match(word)) {
result = result.concat([[15,1]]);
}
if ("include\\sdetype".match(word)) {
result = result.concat([[15,1]]);
}
if ("include\\sg".match(word)) {
result = result.concat([[15,1]]);
}
if ("include\\sgerr".match(word)) {
result = result.concat([[15,1]]);
}
if ("includes".match(word)) {
result = result.concat([[5,1],[42,1],[54,2],[55,1]]);
}
if ("including".match(word)) {
result = result.concat([[2,2],[28,1],[39,2]]);
}
if ("indexes".match(word)) {
result = result.concat([[54,1]]);
}
if ("individually".match(word)) {
result = result.concat([[33,4]]);
}
if ("information".match(word)) {
result = result.concat([[9,7],[28,6],[46,7],[52,2],[54,4]]);
}
if ("ini".match(word)) {
result = result.concat([[54,13],[68,1]]);
}
if ("initfiletest".match(word)) {
result = result.concat([[31,1],[33,4],[67,1],[68,1]]);
}
if ("initiate".match(word)) {
result = result.concat([[29,2],[54,1]]);
}
if ("initiates".match(word)) {
result = result.concat([[54,1]]);
}
if ("inline".match(word)) {
result = result.concat([[54,1]]);
}
if ("input".match(word)) {
result = result.concat([[12,1],[49,2]]);
}
if ("inside".match(word)) {
result = result.concat([[55,2]]);
}
if ("install".match(word)) {
result = result.concat([[1,3],[2,4],[3,7],[11,5],[12,4],[14,5],[15,2],[16,2],[22,9],[24,28],[28,12],[32,1],[38,3],[39,4],[40,8],[48,5],[51,5],[52,2],[54,16],[59,3],[61,11],[68,1]]);
}
if ("installation".match(word)) {
result = result.concat([[16,3],[28,5],[29,1],[54,11]]);
}
if ("installed".match(word)) {
result = result.concat([[16,1],[24,1],[40,2],[49,1],[54,4],[55,1],[59,2]]);
}
if ("installer".match(word)) {
result = result.concat([[16,2],[32,2],[54,6]]);
}
if ("installers".match(word)) {
result = result.concat([[13,1]]);
}
if ("installing".match(word)) {
result = result.concat([[28,2],[54,1]]);
}
if ("installoracle".match(word)) {
result = result.concat([[28,2]]);
}
if ("installs".match(word)) {
result = result.concat([[20,2],[57,2]]);
}
if ("instance".match(word)) {
result = result.concat([[28,4],[54,2]]);
}
if ("instancesqlserver".match(word)) {
result = result.concat([[28,2]]);
}
if ("instant".match(word)) {
result = result.concat([[54,3]]);
}
if ("instructions".match(word)) {
result = result.concat([[3,4],[4,2],[15,2],[40,4],[41,2],[52,2]]);
}
if ("intact".match(word)) {
result = result.concat([[61,2]]);
}
if ("intend".match(word)) {
result = result.concat([[1,2],[12,2],[38,2]]);
}
if ("intermediate".match(word)) {
result = result.concat([[61,4]]);
}
if ("introduction".match(word)) {
result = result.concat([[1,5],[26,5],[38,5],[63,5]]);
}
if ("invocations".match(word)) {
result = result.concat([[29,2]]);
}
if ("invoke".match(word)) {
result = result.concat([[28,2]]);
}
if ("involve".match(word)) {
result = result.concat([[36,1]]);
}
if ("ip".match(word)) {
result = result.concat([[31,1],[67,1]]);
}
if ("it:".match(word)) {
result = result.concat([[55,1]]);
}
if ("ith".match(word)) {
result = result.concat([[10,1],[22,1],[24,1],[47,1],[61,1]]);
}
if ("itself".match(word)) {
result = result.concat([[1,2],[38,2]]);
}
if ("java".match(word)) {
result = result.concat([[13,1],[50,1]]);
}
if ("just".match(word)) {
result = result.concat([[28,1],[54,1]]);
}
if ("key".match(word)) {
result = result.concat([[29,4]]);
}
if ("known".match(word)) {
result = result.concat([[28,1]]);
}
if ("la".match(word)) {
result = result.concat([[61,26]]);
}
if ("labeled".match(word)) {
result = result.concat([[4,2],[28,14],[41,2]]);
}
if ("lalr".match(word)) {
result = result.concat([[12,1],[49,1]]);
}
if ("language".match(word)) {
result = result.concat([[12,3],[20,1],[21,2],[23,2],[49,1],[57,1],[58,2],[60,2]]);
}
if ("last".match(word)) {
result = result.concat([[61,2]]);
}
if ("later".match(word)) {
result = result.concat([[54,2]]);
}
if ("latest".match(word)) {
result = result.concat([[16,2],[53,2]]);
}
if ("latestsrc".match(word)) {
result = result.concat([[13,1],[50,1]]);
}
if ("lato".match(word)) {
result = result.concat([[61,2]]);
}
if ("latter".match(word)) {
result = result.concat([[28,1],[54,2]]);
}
if ("launch".match(word)) {
result = result.concat([[18,2]]);
}
if ("ld".match(word)) {
result = result.concat([[55,3],[65,2]]);
}
if ("leave".match(word)) {
result = result.concat([[28,2]]);
}
if ("leaving".match(word)) {
result = result.concat([[54,2],[61,2]]);
}
if ("left".match(word)) {
result = result.concat([[3,2],[40,2]]);
}
if ("li/pub/subversion/summersoft".match(word)) {
result = result.concat([[40,2]]);
}
if ("lib".match(word)) {
result = result.concat([[15,4],[16,3],[22,12],[24,36],[54,1],[55,2]]);
}
if ("lib/libedgemt".match(word)) {
result = result.concat([[52,1]]);
}
if ("lib/libgsrvrdb291".match(word)) {
result = result.concat([[52,1]]);
}
if ("lib/libgsrvrora9i91".match(word)) {
result = result.concat([[52,1]]);
}
if ("lib/libicudata".match(word)) {
result = result.concat([[52,1]]);
}
if ("lib/libicuuc".match(word)) {
result = result.concat([[52,1]]);
}
if ("lib/libloceng".match(word)) {
result = result.concat([[52,1]]);
}
if ("lib/liblocssa".match(word)) {
result = result.concat([[52,1]]);
}
if ("lib/libmtchloc".match(word)) {
result = result.concat([[52,1]]);
}
if ("lib/libmtchmt".match(word)) {
result = result.concat([[52,1]]);
}
if ("lib/libpe91".match(word)) {
result = result.concat([[52,2]]);
}
if ("lib/libsde91".match(word)) {
result = result.concat([[52,2]]);
}
if ("lib/libsdedb2srvr91".match(word)) {
result = result.concat([[52,1]]);
}
if ("lib/libsdeora9isrvr91".match(word)) {
result = result.concat([[52,1]]);
}
if ("lib/libsg91".match(word)) {
result = result.concat([[52,2]]);
}
if ("lib/libxerces".match(word)) {
result = result.concat([[52,1]]);
}
if ("lib\\icuuc".match(word)) {
result = result.concat([[15,1]]);
}
if ("lib\\pe91".match(word)) {
result = result.concat([[15,1]]);
}
if ("lib\\sde91".match(word)) {
result = result.concat([[15,1]]);
}
if ("lib\\sg91".match(word)) {
result = result.concat([[15,1]]);
}
if ("libarcsdeprovider".match(word)) {
result = result.concat([[61,4]]);
}
if ("libcurl".match(word)) {
result = result.concat([[22,4]]);
}
if ("libfdo".match(word)) {
result = result.concat([[61,4]]);
}
if ("libfdomysql".match(word)) {
result = result.concat([[61,4]]);
}
if ("libfdoodbc".match(word)) {
result = result.concat([[61,4]]);
}
if ("libfdoows".match(word)) {
result = result.concat([[61,4]]);
}
if ("libgdal".match(word)) {
result = result.concat([[59,1]]);
}
if ("libgrfpoverrides".match(word)) {
result = result.concat([[61,4]]);
}
if ("libgrfpprovider".match(word)) {
result = result.concat([[61,4]]);
}
if ("libmysql".match(word)) {
result = result.concat([[16,1]]);
}
if ("libraries".match(word)) {
result = result.concat([[14,2],[51,2],[52,2],[54,1],[55,1],[65,2]]);
}
if ("library".match(word)) {
result = result.concat([[55,3],[65,2]]);
}
if ("libs".match(word)) {
result = result.concat([[55,1]]);
}
if ("libschemamgr".match(word)) {
result = result.concat([[61,4]]);
}
if ("libsdfprovider".match(word)) {
result = result.concat([[61,4]]);
}
if ("libshpoverrides".match(word)) {
result = result.concat([[61,4]]);
}
if ("libshpprovider".match(word)) {
result = result.concat([[61,4]]);
}
if ("libwfsprovider".match(word)) {
result = result.concat([[61,4]]);
}
if ("libwmsoverrides".match(word)) {
result = result.concat([[61,4]]);
}
if ("libwmsprovider".match(word)) {
result = result.concat([[61,4]]);
}
if ("libxalan".match(word)) {
result = result.concat([[59,1]]);
}
if ("libxalanmsg".match(word)) {
result = result.concat([[59,1]]);
}
if ("libxerces".match(word)) {
result = result.concat([[59,1]]);
}
if ("license".match(word)) {
result = result.concat([[9,2],[46,2],[54,11]]);
}
if ("licensed".match(word)) {
result = result.concat([[54,4]]);
}
if ("licensing".match(word)) {
result = result.concat([[9,7],[46,7],[54,1]]);
}
if ("limit".match(word)) {
result = result.concat([[22,2]]);
}
if ("line".match(word)) {
result = result.concat([[3,2],[12,2],[22,2],[28,3],[29,4],[40,2],[55,1]]);
}
if ("link".match(word)) {
result = result.concat([[3,4],[4,4],[10,2],[40,4],[41,4],[47,2]]);
}
if ("linux".match(word)) {
result = result.concat([[37,5],[38,2],[49,1],[50,1],[53,2],[54,7],[57,1],[61,32]]);
}
if ("linuxtest".match(word)) {
result = result.concat([[34,2]]);
}
if ("list".match(word)) {
result = result.concat([[3,2],[15,2],[28,1],[29,4],[40,2],[52,2],[54,1]]);
}
if ("list:".match(word)) {
result = result.concat([[22,2],[24,2],[61,2]]);
}
if ("listen".match(word)) {
result = result.concat([[29,3]]);
}
if ("listens".match(word)) {
result = result.concat([[28,1]]);
}
if ("local".match(word)) {
result = result.concat([[5,2],[42,2]]);
}
if ("locate".match(word)) {
result = result.concat([[30,1],[34,1],[35,1],[54,1]]);
}
if ("located".match(word)) {
result = result.concat([[4,2],[28,1],[30,1],[34,1],[35,1],[41,2]]);
}
if ("location".match(word)) {
result = result.concat([[5,1],[10,3],[15,4],[16,2],[42,1],[47,3],[52,2],[54,3],[55,3]]);
}
if ("locations".match(word)) {
result = result.concat([[55,2]]);
}
if ("locktests".match(word)) {
result = result.concat([[29,1]]);
}
if ("logged".match(word)) {
result = result.concat([[4,2],[41,2]]);
}
if ("logging".match(word)) {
result = result.concat([[4,2],[41,2]]);
}
if ("login".match(word)) {
result = result.concat([[4,2],[32,1],[41,2],[54,1]]);
}
if ("longer".match(word)) {
result = result.concat([[28,1]]);
}
if ("longtransactiontests".match(word)) {
result = result.concat([[29,1]]);
}
if ("m".match(word)) {
result = result.concat([[59,4],[61,5]]);
}
if ("m4".match(word)) {
result = result.concat([[49,1]]);
}
if ("machine".match(word)) {
result = result.concat([[1,1],[16,1],[28,4],[29,6],[31,1],[38,1],[40,1],[49,1],[54,1],[67,1]]);
}
if ("makefile".match(word)) {
result = result.concat([[49,2]]);
}
if ("makefiles".match(word)) {
result = result.concat([[59,4],[61,1]]);
}
if ("managed".match(word)) {
result = result.concat([[24,6]]);
}
if ("manager".match(word)) {
result = result.concat([[28,3],[29,1],[32,1],[54,11],[55,1]]);
}
if ("manager’s".match(word)) {
result = result.concat([[55,1]]);
}
if ("manual".match(word)) {
result = result.concat([[28,2]]);
}
if ("mapping".match(word)) {
result = result.concat([[54,2]]);
}
if ("may".match(word)) {
result = result.concat([[4,2],[17,1],[28,1],[41,2]]);
}
if ("mb".match(word)) {
result = result.concat([[39,4]]);
}
if ("menu".match(word)) {
result = result.concat([[18,2],[28,2]]);
}
if ("message".match(word)) {
result = result.concat([[28,1],[54,1]]);
}
if ("microsoft".match(word)) {
result = result.concat([[12,3],[13,3],[18,5],[28,1]]);
}
if ("missing".match(word)) {
result = result.concat([[16,1]]);
}
if ("mod".match(word)) {
result = result.concat([[40,2]]);
}
if ("modified".match(word)) {
result = result.concat([[12,1],[21,2],[34,1],[49,1]]);
}
if ("modify".match(word)) {
result = result.concat([[1,2],[18,2],[38,2],[55,2]]);
}
if ("modifying".match(word)) {
result = result.concat([[18,5]]);
}
if ("msbuild".match(word)) {
result = result.concat([[12,1]]);
}
if ("msi".match(word)) {
result = result.concat([[32,2]]);
}
if ("multi".match(word)) {
result = result.concat([[29,6],[65,4]]);
}
if ("multiport".match(word)) {
result = result.concat([[29,3],[65,2]]);
}
if ("mysql".match(word)) {
result = result.concat([[2,2],[5,1],[14,2],[16,12],[24,8],[26,2],[31,7],[32,3],[39,2],[42,1],[51,2],[53,9],[55,3],[61,2],[63,2],[67,7],[68,2]]);
}
if ("mysqlinit".match(word)) {
result = result.concat([[31,1],[67,1]]);
}
if ("mysqlinitex".match(word)) {
result = result.concat([[31,3],[67,3]]);
}
if ("mysqlserver".match(word)) {
result = result.concat([[31,1],[32,1],[67,1],[68,2]]);
}
if ("mysqlserver”".match(word)) {
result = result.concat([[31,1],[67,1]]);
}
if ("name".match(word)) {
result = result.concat([[28,10],[31,1],[32,4],[54,10],[67,1],[68,3]]);
}
if ("named".match(word)) {
result = result.concat([[5,2],[32,1],[42,2]]);
}
if ("names".match(word)) {
result = result.concat([[28,3],[29,6],[54,1]]);
}
if ("need".match(word)) {
result = result.concat([[22,2],[54,2],[59,2]]);
}
if ("neon".match(word)) {
result = result.concat([[40,1]]);
}
if ("net".match(word)) {
result = result.concat([[12,1],[17,1],[54,1]]);
}
if ("net/gnuwin32/".match(word)) {
result = result.concat([[12,1]]);
}
if ("net/linux/rpm/fedora/1/i386/bison".match(word)) {
result = result.concat([[49,1]]);
}
if ("net/packages".match(word)) {
result = result.concat([[12,1]]);
}
if ("net\\framework\\v2".match(word)) {
result = result.concat([[12,1]]);
}
if ("netservicename".match(word)) {
result = result.concat([[68,2]]);
}
if ("new".match(word)) {
result = result.concat([[3,2],[28,1],[32,2],[40,2]]);
}
if ("next".match(word)) {
result = result.concat([[28,8]]);
}
if ("nl/".match(word)) {
result = result.concat([[13,1],[50,1]]);
}
if ("nlsdir".match(word)) {
result = result.concat([[17,1]]);
}
if ("no".match(word)) {
result = result.concat([[22,2],[24,2],[28,2],[29,2],[54,3],[61,18]]);
}
if ("noconfigure".match(word)) {
result = result.concat([[59,3],[61,4]]);
}
if ("non".match(word)) {
result = result.concat([[49,1]]);
}
if ("noted".match(word)) {
result = result.concat([[55,2]]);
}
if ("notice".match(word)) {
result = result.concat([[16,1],[22,2],[59,2]]);
}
if ("notified".match(word)) {
result = result.concat([[28,1]]);
}
if ("number".match(word)) {
result = result.concat([[28,2],[54,3]]);
}
if ("o".match(word)) {
result = result.concat([[10,2],[22,10],[24,50],[47,6]]);
}
if ("object".match(word)) {
result = result.concat([[1,1],[38,1],[54,2]]);
}
if ("objective".match(word)) {
result = result.concat([[13,1],[50,1]]);
}
if ("objects".match(word)) {
result = result.concat([[55,1]]);
}
if ("obtain".match(word)) {
result = result.concat([[3,4],[29,1],[40,4],[49,1],[54,1]]);
}
if ("occur".match(word)) {
result = result.concat([[29,2]]);
}
if ("occurs".match(word)) {
result = result.concat([[2,2],[39,2]]);
}
if ("ocs".match(word)) {
result = result.concat([[22,1],[24,1],[61,1]]);
}
if ("odbc".match(word)) {
result = result.concat([[5,1],[24,8],[26,2],[32,7],[33,7],[39,2],[42,1],[51,2],[54,25],[55,1],[61,2],[63,2],[68,8]]);
}
if ("odbcaccesstests".match(word)) {
result = result.concat([[33,1]]);
}
if ("odbcexceltests".match(word)) {
result = result.concat([[33,1]]);
}
if ("odbcinit".match(word)) {
result = result.concat([[32,1],[68,1]]);
}
if ("odbcinitex".match(word)) {
result = result.concat([[32,4],[33,4],[68,3]]);
}
if ("odbcinst".match(word)) {
result = result.concat([[54,6]]);
}
if ("odbcmysqltests".match(word)) {
result = result.concat([[33,1]]);
}
if ("odbcodbctests".match(word)) {
result = result.concat([[33,1]]);
}
if ("odbcoracletests".match(word)) {
result = result.concat([[68,1]]);
}
if ("odbcsqlservertests".match(word)) {
result = result.concat([[33,1]]);
}
if ("odbctexttests".match(word)) {
result = result.concat([[33,1]]);
}
if ("ok".match(word)) {
result = result.concat([[28,2]]);
}
if ("once".match(word)) {
result = result.concat([[4,2],[22,2],[33,2],[41,2],[59,2]]);
}
if ("ones".match(word)) {
result = result.concat([[28,1]]);
}
if ("onfig".match(word)) {
result = result.concat([[22,1],[24,1],[61,1]]);
}
if ("open".match(word)) {
result = result.concat([[3,2],[4,2],[18,2],[32,1],[40,2],[41,2]]);
}
if ("opensourcebuild".match(word)) {
result = result.concat([[1,2],[38,2]]);
}
if ("openssl".match(word)) {
result = result.concat([[12,1],[22,8],[49,1]]);
}
if ("operating".match(word)) {
result = result.concat([[12,1]]);
}
if ("operation".match(word)) {
result = result.concat([[16,1],[18,2],[28,3]]);
}
if ("operations".match(word)) {
result = result.concat([[12,1],[49,1]]);
}
if ("opt".match(word)) {
result = result.concat([[16,2]]);
}
if ("option".match(word)) {
result = result.concat([[10,4],[22,28],[24,98],[28,1],[47,4],[54,4],[59,2],[61,41]]);
}
if ("optional".match(word)) {
result = result.concat([[29,2],[54,2],[55,1]]);
}
if ("optionally".match(word)) {
result = result.concat([[21,2],[23,5],[58,2],[60,5]]);
}
if ("optionand".match(word)) {
result = result.concat([[24,2]]);
}
if ("options".match(word)) {
result = result.concat([[12,1],[22,2],[54,2],[61,1]]);
}
if ("ora".match(word)) {
result = result.concat([[54,1],[68,1]]);
}
if ("oracle".match(word)) {
result = result.concat([[28,5],[29,8],[32,2],[54,28],[65,2],[68,6]]);
}
if ("oracle’s".match(word)) {
result = result.concat([[28,2]]);
}
if ("oraserver".match(word)) {
result = result.concat([[32,1],[68,2]]);
}
if ("oraserver”".match(word)) {
result = result.concat([[68,1]]);
}
if ("order".match(word)) {
result = result.concat([[21,5],[58,5]]);
}
if ("org".match(word)) {
result = result.concat([[3,2],[4,2],[10,1],[28,2],[40,2],[41,2],[47,1],[54,4]]);
}
if ("org/download".match(word)) {
result = result.concat([[13,1],[50,1]]);
}
if ("org/fdo/trunk".match(word)) {
result = result.concat([[6,2],[43,2]]);
}
if ("org/fdoarcsde/branches/3".match(word)) {
result = result.concat([[7,2],[8,2],[44,2],[45,2]]);
}
if ("org/fdocore/branches/3".match(word)) {
result = result.concat([[7,2],[8,2],[44,2],[45,2]]);
}
if ("org/fdogdal/branches/3".match(word)) {
result = result.concat([[7,2],[8,2],[44,2],[45,2]]);
}
if ("org/fdordbms/branches/3".match(word)) {
result = result.concat([[7,2],[8,2],[44,2],[45,2]]);
}
if ("org/fdosdf/branches/3".match(word)) {
result = result.concat([[7,2],[8,2],[44,2],[45,2]]);
}
if ("org/fdoshp/branches/3".match(word)) {
result = result.concat([[7,2],[8,2],[44,2],[45,2]]);
}
if ("org/fdowfs/branches/3".match(word)) {
result = result.concat([[7,2],[8,2],[44,2],[45,2]]);
}
if ("org/fdowms/branches/3".match(word)) {
result = result.concat([[7,2],[8,2],[44,2],[45,2]]);
}
if ("osgeo".match(word)) {
result = result.concat([[3,2],[4,2],[6,2],[7,16],[8,16],[10,1],[28,2],[40,2],[41,2],[43,2],[44,16],[45,16],[47,1]]);
}
if ("osql".match(word)) {
result = result.concat([[28,2]]);
}
if ("ource".match(word)) {
result = result.concat([[10,1],[47,1]]);
}
if ("outfolder".match(word)) {
result = result.concat([[10,1],[22,1],[24,1],[47,1]]);
}
if ("outfolder:".match(word)) {
result = result.concat([[10,1],[22,1],[24,1],[47,1]]);
}
if ("outpath".match(word)) {
result = result.concat([[24,1]]);
}
if ("output".match(word)) {
result = result.concat([[61,2]]);
}
if ("outputfolder".match(word)) {
result = result.concat([[24,15]]);
}
if ("outputfolder\\fdo\\docs\\html\\fdo".match(word)) {
result = result.concat([[24,2]]);
}
if ("outputs".match(word)) {
result = result.concat([[12,1],[49,1]]);
}
if ("outside".match(word)) {
result = result.concat([[15,2],[52,2]]);
}
if ("ov".match(word)) {
result = result.concat([[61,4]]);
}
if ("ows".match(word)) {
result = result.concat([[24,8]]);
}
if ("p".match(word)) {
result = result.concat([[10,6],[47,6]]);
}
if ("package".match(word)) {
result = result.concat([[53,2],[54,4]]);
}
if ("packages".match(word)) {
result = result.concat([[49,1]]);
}
if ("page".match(word)) {
result = result.concat([[3,2],[13,1],[40,2]]);
}
if ("pane".match(word)) {
result = result.concat([[28,1]]);
}
if ("panel".match(word)) {
result = result.concat([[28,1]]);
}
if ("parameters".match(word)) {
result = result.concat([[65,2]]);
}
if ("parcel".match(word)) {
result = result.concat([[34,1]]);
}
if ("parcl".match(word)) {
result = result.concat([[34,1]]);
}
if ("parent".match(word)) {
result = result.concat([[55,1]]);
}
if ("parse".match(word)) {
result = result.concat([[12,1],[20,1],[23,7],[49,1],[57,1],[60,7]]);
}
if ("parser".match(word)) {
result = result.concat([[12,1],[49,1]]);
}
if ("part".match(word)) {
result = result.concat([[49,6],[50,2]]);
}
if ("party".match(word)) {
result = result.concat([[12,1],[20,1],[22,4],[49,1],[57,1],[59,4]]);
}
if ("password".match(word)) {
result = result.concat([[10,1],[28,2],[31,1],[32,2],[47,1],[54,1],[67,1],[68,2]]);
}
if ("password:".match(word)) {
result = result.concat([[10,1],[47,1]]);
}
if ("passwordmysql".match(word)) {
result = result.concat([[32,2]]);
}
if ("passwordoracle".match(word)) {
result = result.concat([[32,2],[68,2]]);
}
if ("passwordsqlserver".match(word)) {
result = result.concat([[32,2],[68,2]]);
}
if ("path".match(word)) {
result = result.concat([[5,3],[12,1],[13,2],[16,2],[17,4],[18,2],[28,4],[42,3],[49,1],[50,1],[54,2],[55,3],[65,2]]);
}
if ("paths".match(word)) {
result = result.concat([[17,1],[30,1],[34,1],[35,1],[54,2]]);
}
if ("pathtooracleclient".match(word)) {
result = result.concat([[54,2]]);
}
if ("pdf".match(word)) {
result = result.concat([[24,2]]);
}
if ("performs".match(word)) {
result = result.concat([[12,1],[49,1]]);
}
if ("perl".match(word)) {
result = result.concat([[12,1],[28,12],[29,1],[49,1]]);
}
if ("permanently".match(word)) {
result = result.concat([[28,1]]);
}
if ("pertaining".match(word)) {
result = result.concat([[28,1]]);
}
if ("phone".match(word)) {
result = result.concat([[54,1]]);
}
if ("php".match(word)) {
result = result.concat([[13,2],[16,3],[50,2]]);
}
if ("platform".match(word)) {
result = result.concat([[13,1],[54,1]]);
}
if ("pop".match(word)) {
result = result.concat([[28,1]]);
}
if ("port".match(word)) {
result = result.concat([[28,2],[29,17],[54,1],[65,12]]);
}
if ("possible".match(word)) {
result = result.concat([[10,3],[47,3]]);
}
if ("post".match(word)) {
result = result.concat([[28,5],[29,1]]);
}
if ("precedes".match(word)) {
result = result.concat([[16,1]]);
}
if ("present".match(word)) {
result = result.concat([[16,1],[40,1]]);
}
if ("press".match(word)) {
result = result.concat([[54,4]]);
}
if ("previous".match(word)) {
result = result.concat([[28,2],[54,3]]);
}
if ("prj".match(word)) {
result = result.concat([[15,1],[52,1]]);
}
if ("problem".match(word)) {
result = result.concat([[28,3]]);
}
if ("problems".match(word)) {
result = result.concat([[28,2]]);
}
if ("procedure".match(word)) {
result = result.concat([[65,2]]);
}
if ("process".match(word)) {
result = result.concat([[1,3],[12,2],[13,2],[38,3],[49,2],[50,2]]);
}
if ("produced".match(word)) {
result = result.concat([[61,2]]);
}
if ("produces".match(word)) {
result = result.concat([[49,1]]);
}
if ("production".match(word)) {
result = result.concat([[16,2],[53,2]]);
}
if ("products".match(word)) {
result = result.concat([[54,1]]);
}
if ("program".match(word)) {
result = result.concat([[12,1],[49,3]]);
}
if ("programs".match(word)) {
result = result.concat([[28,1]]);
}
if ("project".match(word)) {
result = result.concat([[18,9]]);
}
if ("projects".match(word)) {
result = result.concat([[12,1]]);
}
if ("prompt".match(word)) {
result = result.concat([[54,1]]);
}
if ("prompted".match(word)) {
result = result.concat([[28,2]]);
}
if ("properties".match(word)) {
result = result.concat([[12,1],[28,1]]);
}
if ("provide".match(word)) {
result = result.concat([[24,1],[29,2]]);
}
if ("provided".match(word)) {
result = result.concat([[28,1],[29,2]]);
}
if ("provider".match(word)) {
result = result.concat([[2,2],[16,3],[20,1],[21,2],[22,6],[24,26],[39,2],[57,1],[68,2]]);
}
if ("provider\\wfs".match(word)) {
result = result.concat([[24,2]]);
}
if ("provider\\wms".match(word)) {
result = result.concat([[24,2]]);
}
if ("providers".match(word)) {
result = result.concat([[5,3],[10,2],[14,2],[20,1],[22,1],[24,4],[26,2],[42,3],[47,2],[51,2],[57,1],[58,2],[61,5],[63,2]]);
}
if ("providers\\arcsde".match(word)) {
result = result.concat([[24,2]]);
}
if ("providers\\arcsde\\docs".match(word)) {
result = result.concat([[24,1]]);
}
if ("providers\\arcsde\\docs\\html\\arcsde".match(word)) {
result = result.concat([[24,1]]);
}
if ("providers\\gdal".match(word)) {
result = result.concat([[24,2]]);
}
if ("providers\\genericrdbms".match(word)) {
result = result.concat([[24,4]]);
}
if ("providers\\mysql\\docs".match(word)) {
result = result.concat([[24,1]]);
}
if ("providers\\mysql\\docs\\html\\mysql".match(word)) {
result = result.concat([[24,1]]);
}
if ("providers\\odbc\\docs".match(word)) {
result = result.concat([[24,1]]);
}
if ("providers\\odbc\\docs\\html\\odbc".match(word)) {
result = result.concat([[24,1]]);
}
if ("providers\\sdf".match(word)) {
result = result.concat([[24,2]]);
}
if ("providers\\sdf\\docs".match(word)) {
result = result.concat([[24,1]]);
}
if ("providers\\sdf\\docs\\html\\sdf".match(word)) {
result = result.concat([[24,1]]);
}
if ("providers\\shp".match(word)) {
result = result.concat([[24,2]]);
}
if ("providers\\shp\\docs".match(word)) {
result = result.concat([[24,1]]);
}
if ("providers\\shp\\docs\\html\\shp".match(word)) {
result = result.concat([[24,1]]);
}
if ("providers\\wfs\\docs".match(word)) {
result = result.concat([[24,1]]);
}
if ("providers\\wfs\\docs\\html\\wfs".match(word)) {
result = result.concat([[24,1]]);
}
if ("providers\\wms\\docs".match(word)) {
result = result.concat([[24,1]]);
}
if ("providers\\wms\\docs\\html\\wms".match(word)) {
result = result.concat([[24,1]]);
}
if ("provides".match(word)) {
result = result.concat([[12,1]]);
}
if ("purchase".match(word)) {
result = result.concat([[15,4],[52,4]]);
}
if ("purpose".match(word)) {
result = result.concat([[12,1],[15,2],[40,2],[49,1]]);
}
if ("puts".match(word)) {
result = result.concat([[40,1]]);
}
if ("pwd/fdo".match(word)) {
result = result.concat([[55,1]]);
}
if ("pwd/thirdparty".match(word)) {
result = result.concat([[55,1]]);
}
if ("pwd/utilities".match(word)) {
result = result.concat([[55,1]]);
}
if ("python".match(word)) {
result = result.concat([[13,1],[50,1]]);
}
if ("question".match(word)) {
result = result.concat([[28,2],[54,3]]);
}
if ("radio".match(word)) {
result = result.concat([[28,2]]);
}
if ("ran".match(word)) {
result = result.concat([[22,2],[24,2],[59,2]]);
}
if ("rdbms".match(word)) {
result = result.concat([[10,2],[14,7],[24,4],[28,2],[29,6],[47,2],[51,5],[61,6],[65,4]]);
}
if ("re".match(word)) {
result = result.concat([[28,2]]);
}
if ("read".match(word)) {
result = result.concat([[4,1],[41,1],[52,2],[54,4]]);
}
if ("readme".match(word)) {
result = result.concat([[1,2],[9,2],[38,2],[46,2]]);
}
if ("reason".match(word)) {
result = result.concat([[28,1]]);
}
if ("receives".match(word)) {
result = result.concat([[5,2],[42,2]]);
}
if ("recommendations".match(word)) {
result = result.concat([[54,2]]);
}
if ("recommends".match(word)) {
result = result.concat([[54,4]]);
}
if ("recommittest".match(word)) {
result = result.concat([[29,1]]);
}
if ("recreates".match(word)) {
result = result.concat([[20,1],[57,1]]);
}
if ("red".match(word)) {
result = result.concat([[38,1],[49,1]]);
}
if ("reference".match(word)) {
result = result.concat([[24,11]]);
}
if ("references".match(word)) {
result = result.concat([[18,2]]);
}
if ("refers".match(word)) {
result = result.concat([[29,1]]);
}
if ("regenerate".match(word)) {
result = result.concat([[23,2],[60,2]]);
}
if ("register".match(word)) {
result = result.concat([[4,2],[41,2]]);
}
if ("registered".match(word)) {
result = result.concat([[4,2],[41,2]]);
}
if ("registering".match(word)) {
result = result.concat([[4,2],[41,2]]);
}
if ("related".match(word)) {
result = result.concat([[36,1]]);
}
if ("relative".match(word)) {
result = result.concat([[5,1],[30,1],[34,1],[35,1],[42,1]]);
}
if ("release".match(word)) {
result = result.concat([[2,2],[13,1],[16,2],[22,3],[24,5],[39,2],[50,1],[53,2],[61,5]]);
}
if ("remain".match(word)) {
result = result.concat([[32,1]]);
}
if ("remove".match(word)) {
result = result.concat([[61,4]]);
}
if ("replace".match(word)) {
result = result.concat([[31,2],[32,3],[67,2],[68,4]]);
}
if ("repositories".match(word)) {
result = result.concat([[3,4],[4,2],[5,8],[40,4],[41,2],[42,8]]);
}
if ("repository".match(word)) {
result = result.concat([[3,5],[5,5],[6,7],[7,7],[8,7],[28,5],[40,5],[42,5],[43,7],[44,7],[45,7]]);
}
if ("request".match(word)) {
result = result.concat([[16,2],[55,2]]);
}
if ("requested".match(word)) {
result = result.concat([[29,1]]);
}
if ("requesting".match(word)) {
result = result.concat([[54,1]]);
}
if ("requests".match(word)) {
result = result.concat([[28,1],[29,3]]);
}
if ("require".match(word)) {
result = result.concat([[2,2],[39,2]]);
}
if ("required".match(word)) {
result = result.concat([[2,2],[16,2],[17,2],[39,2],[40,1]]);
}
if ("requirement".match(word)) {
result = result.concat([[2,4],[39,4]]);
}
if ("requires".match(word)) {
result = result.concat([[28,4],[40,1],[49,1]]);
}
if ("respective".match(word)) {
result = result.concat([[13,1]]);
}
if ("result".match(word)) {
result = result.concat([[54,2]]);
}
if ("results".match(word)) {
result = result.concat([[28,1]]);
}
if ("retry".match(word)) {
result = result.concat([[35,1]]);
}
if ("return".match(word)) {
result = result.concat([[28,1],[54,1]]);
}
if ("revert".match(word)) {
result = result.concat([[34,1]]);
}
if ("review".match(word)) {
result = result.concat([[9,5],[17,1],[28,2],[46,5]]);
}
if ("rewarded".match(word)) {
result = result.concat([[28,1]]);
}
if ("rhel3".match(word)) {
result = result.concat([[40,2],[49,6],[50,2],[54,1]]);
}
if ("right".match(word)) {
result = result.concat([[4,2],[28,1],[41,2]]);
}
if ("role".match(word)) {
result = result.concat([[32,2],[68,2]]);
}
if ("roles:".match(word)) {
result = result.concat([[32,1]]);
}
if ("root".match(word)) {
result = result.concat([[31,1],[32,2],[54,5],[67,1],[68,2]]);
}
if ("root\\microsoft".match(word)) {
result = result.concat([[28,1]]);
}
if ("rootpassword".match(word)) {
result = result.concat([[31,1],[67,1]]);
}
if ("rpm".match(word)) {
result = result.concat([[40,4],[49,1],[53,2]]);
}
if ("rpms".match(word)) {
result = result.concat([[40,2],[55,1]]);
}
if ("run".match(word)) {
result = result.concat([[12,1],[17,2],[18,2],[22,9],[23,7],[24,5],[27,5],[28,13],[29,5],[30,5],[31,5],[32,1],[33,13],[34,5],[35,5],[36,5],[49,1],[54,3],[59,9],[60,7],[61,5],[64,5],[65,7],[66,5],[67,5],[68,5],[69,5],[70,5],[71,5]]);
}
if ("running".match(word)) {
result = result.concat([[10,4],[28,5],[32,2],[34,2],[47,4],[55,1]]);
}
if ("runs".match(word)) {
result = result.concat([[29,2],[54,1]]);
}
if ("s".match(word)) {
result = result.concat([[10,9],[47,9],[49,1]]);
}
if ("saying".match(word)) {
result = result.concat([[54,1]]);
}
if ("schema".match(word)) {
result = result.concat([[61,2]]);
}
if ("screen".match(word)) {
result = result.concat([[28,2]]);
}
if ("screens".match(word)) {
result = result.concat([[28,1]]);
}
if ("script".match(word)) {
result = result.concat([[5,1],[10,4],[12,2],[17,7],[18,2],[22,9],[23,7],[24,7],[28,20],[29,1],[42,1],[47,4],[52,2],[54,12],[55,11],[59,9],[60,7],[61,5]]);
}
if ("scripting".match(word)) {
result = result.concat([[12,1],[49,1]]);
}
if ("scripts".match(word)) {
result = result.concat([[20,5],[49,1],[57,5]]);
}
if ("scroll".match(word)) {
result = result.concat([[54,1]]);
}
if ("sde".match(word)) {
result = result.concat([[28,3],[52,1]]);
}
if ("sdehome".match(word)) {
result = result.concat([[17,2],[28,4],[52,4],[55,3]]);
}
if ("sdehome/lib".match(word)) {
result = result.concat([[55,1],[65,2]]);
}
if ("sdepwd".match(word)) {
result = result.concat([[29,4],[65,4]]);
}
if ("sdeuserpassword".match(word)) {
result = result.concat([[29,4],[65,4]]);
}
if ("sdf".match(word)) {
result = result.concat([[10,2],[22,7],[24,8],[26,2],[34,6],[47,2],[61,8],[63,2],[69,5]]);
}
if ("sdk".match(word)) {
result = result.concat([[13,1],[15,7],[52,7]]);
}
if ("sdx".match(word)) {
result = result.concat([[34,2]]);
}
if ("second".match(word)) {
result = result.concat([[28,2]]);
}
if ("section".match(word)) {
result = result.concat([[28,2],[52,2],[54,1]]);
}
if ("sed".match(word)) {
result = result.concat([[12,3],[23,2],[49,1],[60,2]]);
}
if ("select".match(word)) {
result = result.concat([[28,2],[54,3]]);
}
if ("selectaggregatestests".match(word)) {
result = result.concat([[29,1]]);
}
if ("selected".match(word)) {
result = result.concat([[28,1]]);
}
if ("selecttests".match(word)) {
result = result.concat([[29,1]]);
}
if ("ser".match(word)) {
result = result.concat([[10,1],[47,1]]);
}
if ("server".match(word)) {
result = result.concat([[16,3],[28,42],[29,20],[31,1],[32,3],[65,6],[67,1]]);
}
if ("servers\\sql".match(word)) {
result = result.concat([[28,1]]);
}
if ("server’s".match(word)) {
result = result.concat([[28,2]]);
}
if ("service".match(word)) {
result = result.concat([[28,8],[31,1],[32,2],[54,2],[67,1]]);
}
if ("servicemysql".match(word)) {
result = result.concat([[32,1],[68,2]]);
}
if ("serviceoracle".match(word)) {
result = result.concat([[32,1],[68,2]]);
}
if ("services".match(word)) {
result = result.concat([[28,3],[29,1]]);
}
if ("servicesqlserver".match(word)) {
result = result.concat([[32,1],[68,2]]);
}
if ("setenvironment".match(word)) {
result = result.concat([[17,4],[18,4],[22,2],[24,2],[54,1],[55,4],[59,2]]);
}
if ("sets".match(word)) {
result = result.concat([[17,2],[68,2]]);
}
if ("setting".match(word)) {
result = result.concat([[12,1],[28,2]]);
}
if ("settings".match(word)) {
result = result.concat([[28,2]]);
}
if ("settings”".match(word)) {
result = result.concat([[28,2]]);
}
if ("setup".match(word)) {
result = result.concat([[13,1],[28,2]]);
}
if ("setuptestdata".match(word)) {
result = result.concat([[28,8],[29,1]]);
}
if ("several".match(word)) {
result = result.concat([[23,2],[60,2]]);
}
if ("sh".match(word)) {
result = result.concat([[42,1],[47,9],[54,1],[55,4],[57,3],[59,12],[60,7],[61,32]]);
}
if ("share".match(word)) {
result = result.concat([[5,1],[42,1]]);
}
if ("shared".match(word)) {
result = result.concat([[54,2],[55,1]]);
}
if ("shell".match(word)) {
result = result.concat([[18,2],[49,1],[55,1]]);
}
if ("shp".match(word)) {
result = result.concat([[10,2],[24,8],[26,2],[35,6],[47,2],[61,8],[63,2],[70,5]]);
}
if ("sideof".match(word)) {
result = result.concat([[3,2],[40,2]]);
}
if ("simultaneously".match(word)) {
result = result.concat([[28,1]]);
}
if ("single".match(word)) {
result = result.concat([[29,6],[65,4]]);
}
if ("singleport".match(word)) {
result = result.concat([[29,3],[65,2]]);
}
if ("site".match(word)) {
result = result.concat([[4,4],[41,4]]);
}
if ("skip".match(word)) {
result = result.concat([[22,1],[24,1],[59,2],[61,1]]);
}
if ("skipped".match(word)) {
result = result.concat([[61,1]]);
}
if ("sln".match(word)) {
result = result.concat([[18,2]]);
}
if ("software".match(word)) {
result = result.concat([[12,2],[13,2],[22,2],[49,3],[50,2],[54,2],[59,4]]);
}
if ("solution".match(word)) {
result = result.concat([[12,2],[18,9]]);
}
if ("solutions".match(word)) {
result = result.concat([[12,1]]);
}
if ("solved".match(word)) {
result = result.concat([[28,2]]);
}
if ("something".match(word)) {
result = result.concat([[28,3]]);
}
if ("source".match(word)) {
result = result.concat([[1,3],[3,6],[4,13],[5,2],[6,2],[7,2],[8,2],[10,30],[20,1],[21,2],[23,4],[38,1],[40,6],[41,13],[42,2],[43,2],[44,2],[45,2],[47,29],[49,3],[54,6],[55,2],[57,1],[58,2],[60,4],[68,1]]);
}
if ("source:".match(word)) {
result = result.concat([[47,1]]);
}
if ("sourceforge".match(word)) {
result = result.concat([[12,2]]);
}
if ("space".match(word)) {
result = result.concat([[2,11],[39,11]]);
}
if ("spatial".match(word)) {
result = result.concat([[28,1]]);
}
if ("spatialcontexttests".match(word)) {
result = result.concat([[29,1]]);
}
if ("specially".match(word)) {
result = result.concat([[13,1],[50,1]]);
}
if ("specified".match(word)) {
result = result.concat([[10,2],[12,1],[17,1],[22,14],[24,76],[29,3],[47,2]]);
}
if ("specify".match(word)) {
result = result.concat([[22,12],[24,22],[28,4],[54,1]]);
}
if ("sql".match(word)) {
result = result.concat([[24,2],[28,19],[29,6],[32,3],[54,1]]);
}
if ("sqlplus".match(word)) {
result = result.concat([[28,2]]);
}
if ("sqlserver".match(word)) {
result = result.concat([[28,1],[29,3],[32,1],[65,2],[68,2]]);
}
if ("sqltests".match(word)) {
result = result.concat([[29,1]]);
}
if ("stack".match(word)) {
result = result.concat([[13,1],[50,1]]);
}
if ("standard".match(word)) {
result = result.concat([[13,1]]);
}
if ("start".match(word)) {
result = result.concat([[28,1]]);
}
if ("started".match(word)) {
result = result.concat([[28,2]]);
}
if ("status".match(word)) {
result = result.concat([[28,2]]);
}
if ("step".match(word)) {
result = result.concat([[28,11],[54,3],[61,1]]);
}
if ("steps".match(word)) {
result = result.concat([[28,2]]);
}
if ("stored".match(word)) {
result = result.concat([[3,2],[40,2]]);
}
if ("streams".match(word)) {
result = result.concat([[12,1],[49,1]]);
}
if ("structure".match(word)) {
result = result.concat([[54,1]]);
}
if ("studio".match(word)) {
result = result.concat([[12,5],[17,1],[18,4]]);
}
if ("su".match(word)) {
result = result.concat([[54,1]]);
}
if ("subfolder".match(word)) {
result = result.concat([[22,16],[24,12]]);
}
if ("subfolders".match(word)) {
result = result.concat([[16,1],[22,8],[24,19]]);
}
if ("subversion".match(word)) {
result = result.concat([[2,2],[3,6],[4,2],[5,8],[10,12],[28,2],[39,2],[40,7],[41,2],[42,8],[47,11]]);
}
if ("subversions".match(word)) {
result = result.concat([[10,2],[47,2]]);
}
if ("success".match(word)) {
result = result.concat([[28,1]]);
}
if ("successfully".match(word)) {
result = result.concat([[28,2]]);
}
if ("suffix".match(word)) {
result = result.concat([[28,2]]);
}
if ("suite".match(word)) {
result = result.concat([[29,8]]);
}
if ("suites".match(word)) {
result = result.concat([[29,2]]);
}
if ("supply".match(word)) {
result = result.concat([[54,1]]);
}
if ("supported".match(word)) {
result = result.concat([[5,2],[42,2]]);
}
if ("sure".match(word)) {
result = result.concat([[16,1]]);
}
if ("svn".match(word)) {
result = result.concat([[3,2],[5,4],[6,2],[7,16],[8,16],[40,5],[42,4],[43,2],[44,16],[45,16]]);
}
if ("symbolic".match(word)) {
result = result.concat([[31,1],[67,1]]);
}
if ("syntax".match(word)) {
result = result.concat([[10,4],[47,4]]);
}
if ("sys".match(word)) {
result = result.concat([[32,1]]);
}
if ("sysadmin".match(word)) {
result = result.concat([[28,2]]);
}
if ("sysdba".match(word)) {
result = result.concat([[32,1]]);
}
if ("system".match(word)) {
result = result.concat([[1,1],[12,1],[13,2],[38,1]]);
}
if ("tab".match(word)) {
result = result.concat([[28,1]]);
}
if ("tag".match(word)) {
result = result.concat([[10,3],[47,3]]);
}
if ("tagidentifier".match(word)) {
result = result.concat([[10,1],[47,1]]);
}
if ("takes".match(word)) {
result = result.concat([[12,1],[49,1]]);
}
if ("tar".match(word)) {
result = result.concat([[41,2],[54,5]]);
}
if ("target".match(word)) {
result = result.concat([[32,1],[54,1]]);
}
if ("tells".match(word)) {
result = result.concat([[54,1]]);
}
if ("temporarily".match(word)) {
result = result.concat([[28,1]]);
}
if ("terminal".match(word)) {
result = result.concat([[42,2],[47,2],[54,2],[59,2],[61,2],[64,2],[66,2],[67,2],[68,2]]);
}
if ("test".match(word)) {
result = result.concat([[1,1],[26,2],[28,23],[29,10],[30,2],[34,2],[35,2],[38,1],[40,1],[61,1],[63,2]]);
}
if ("tested".match(word)) {
result = result.concat([[12,1]]);
}
if ("testmultidb".match(word)) {
result = result.concat([[28,1]]);
}
if ("tests".match(word)) {
result = result.concat([[12,1],[25,5],[27,5],[28,12],[29,9],[30,6],[31,5],[32,8],[33,11],[34,6],[35,7],[36,7],[49,1],[54,2],[55,2],[62,5],[64,5],[65,7],[66,5],[67,5],[68,7],[69,5],[70,5],[71,5]]);
}
if ("text".match(word)) {
result = result.concat([[10,2],[12,4],[22,2],[24,2],[28,4],[47,2],[49,4],[61,2]]);
}
if ("theessentialfdoi".match(word)) {
result = result.concat([[24,1]]);
}
if ("third".match(word)) {
result = result.concat([[12,1],[20,1],[22,4],[28,2],[49,1],[57,1],[59,4]]);
}
if ("thirdparty".match(word)) {
result = result.concat([[10,3],[20,1],[21,2],[22,18],[24,1],[47,3],[57,1],[58,2],[59,8]]);
}
if ("thirdparty\\apache".match(word)) {
result = result.concat([[22,2]]);
}
if ("thirdparty\\sqllite3".match(word)) {
result = result.concat([[22,2]]);
}
if ("threadingtests".match(word)) {
result = result.concat([[29,5],[65,2]]);
}
if ("three".match(word)) {
result = result.concat([[20,2],[57,2],[61,2]]);
}
if ("thus".match(word)) {
result = result.concat([[54,2]]);
}
if ("time".match(word)) {
result = result.concat([[54,2]]);
}
if ("together".match(word)) {
result = result.concat([[61,18]]);
}
if ("told".match(word)) {
result = result.concat([[28,1]]);
}
if ("tool".match(word)) {
result = result.concat([[3,5],[12,2],[13,1],[28,8],[29,2],[40,5],[49,2],[50,1]]);
}
if ("toolkit".match(word)) {
result = result.concat([[13,1],[50,1]]);
}
if ("tools".match(word)) {
result = result.concat([[1,1],[2,2],[11,5],[12,5],[13,6],[38,1],[39,2],[48,5],[49,5],[50,5]]);
}
if ("top".match(word)) {
result = result.concat([[1,2],[38,2]]);
}
if ("topic".match(word)) {
result = result.concat([[65,4]]);
}
if ("total".match(word)) {
result = result.concat([[2,2],[39,2]]);
}
if ("true".match(word)) {
result = result.concat([[31,1],[67,1]]);
}
if ("trunk".match(word)) {
result = result.concat([[6,5],[10,2],[43,5],[47,2]]);
}
if ("try".match(word)) {
result = result.concat([[28,4],[35,1]]);
}
if ("two".match(word)) {
result = result.concat([[28,2],[32,1],[68,2]]);
}
if ("txt".match(word)) {
result = result.concat([[1,2],[9,2],[31,4],[32,5],[33,4],[38,2],[46,2],[54,2],[67,4],[68,4]]);
}
if ("type".match(word)) {
result = result.concat([[28,5],[54,2]]);
}
if ("typically".match(word)) {
result = result.concat([[55,1]]);
}
if ("u".match(word)) {
result = result.concat([[10,6],[47,6]]);
}
if ("uncheck".match(word)) {
result = result.concat([[28,1]]);
}
if ("undo".match(word)) {
result = result.concat([[28,2]]);
}
if ("uninstall".match(word)) {
result = result.concat([[28,4],[59,1],[61,11]]);
}
if ("unique".match(word)) {
result = result.concat([[28,1]]);
}
if ("unit".match(word)) {
result = result.concat([[1,1],[12,1],[25,5],[26,2],[27,5],[28,6],[29,5],[30,6],[31,5],[32,5],[33,7],[34,6],[35,7],[36,5],[38,1],[49,1],[54,2],[61,1],[62,5],[63,2],[64,5],[65,7],[66,5],[67,5],[68,5],[69,5],[70,5],[71,5]]);
}
if ("unittest".match(word)) {
result = result.concat([[27,1],[29,2],[34,3],[36,1],[55,1],[65,2]]);
}
if ("unix".match(word)) {
result = result.concat([[54,2],[68,1]]);
}
if ("unixodbc".match(word)) {
result = result.concat([[54,9],[55,1]]);
}
if ("unlicensed".match(word)) {
result = result.concat([[54,1]]);
}
if ("unmanaged".match(word)) {
result = result.concat([[24,1]]);
}
if ("unsuccessfuly".match(word)) {
result = result.concat([[28,1]]);
}
if ("untar".match(word)) {
result = result.concat([[54,2]]);
}
if ("untarring".match(word)) {
result = result.concat([[54,2]]);
}
if ("up".match(word)) {
result = result.concat([[17,1],[28,1]]);
}
if ("update".match(word)) {
result = result.concat([[10,11],[47,6]]);
}
if ("updated".match(word)) {
result = result.concat([[22,2],[59,2]]);
}
if ("updates".match(word)) {
result = result.concat([[5,1],[42,1],[50,2]]);
}
if ("updating".match(word)) {
result = result.concat([[47,5]]);
}
if ("upper".match(word)) {
result = result.concat([[4,2],[41,2]]);
}
if ("url".match(word)) {
result = result.concat([[5,2],[42,2]]);
}
if ("us/htmlhelp/hwmicrosofthtmlhelpdownloads".match(word)) {
result = result.concat([[13,1]]);
}
if ("us/pub/subversion/latest/rhel".match(word)) {
result = result.concat([[40,2]]);
}
if ("used".match(word)) {
result = result.concat([[5,1],[12,3],[13,5],[16,1],[17,1],[20,1],[22,5],[24,1],[28,3],[29,3],[42,1],[49,4],[50,4],[53,2],[54,2],[57,1],[59,2]]);
}
if ("user".match(word)) {
result = result.concat([[4,2],[10,2],[28,6],[32,8],[41,2],[47,2],[54,4],[68,2]]);
}
if ("user:".match(word)) {
result = result.concat([[10,1],[47,1]]);
}
if ("userid".match(word)) {
result = result.concat([[10,1],[47,1]]);
}
if ("username".match(word)) {
result = result.concat([[31,1],[67,1]]);
}
if ("usernamemysql".match(word)) {
result = result.concat([[32,2],[68,2]]);
}
if ("usernameoracle".match(word)) {
result = result.concat([[32,2],[68,2]]);
}
if ("usernamesqlserver".match(word)) {
result = result.concat([[32,2],[68,2]]);
}
if ("userpassword".match(word)) {
result = result.concat([[10,1],[47,1],[68,1]]);
}
if ("users".match(word)) {
result = result.concat([[28,6]]);
}
if ("uses".match(word)) {
result = result.concat([[23,2],[52,2],[54,1],[60,2]]);
}
if ("using".match(word)) {
result = result.concat([[10,2],[12,2],[18,2],[22,8],[24,48],[28,5],[29,2],[32,1],[47,2],[54,2],[61,14]]);
}
if ("utilities".match(word)) {
result = result.concat([[10,3],[23,2],[47,3],[60,2],[61,10]]);
}
if ("utilities\\ows".match(word)) {
result = result.concat([[24,4]]);
}
if ("utility".match(word)) {
result = result.concat([[12,2],[58,2]]);
}
if ("utpath".match(word)) {
result = result.concat([[10,1],[22,1],[24,1],[47,1]]);
}
if ("v".match(word)) {
result = result.concat([[54,1]]);
}
if ("valid".match(word)) {
result = result.concat([[32,1],[54,1]]);
}
if ("value".match(word)) {
result = result.concat([[24,2],[28,2],[29,12],[32,1],[54,2],[68,1]]);
}
if ("values".match(word)) {
result = result.concat([[17,2],[28,6],[29,2],[32,4],[54,1],[55,4]]);
}
if ("variable".match(word)) {
result = result.concat([[13,1],[16,1],[17,1],[28,6],[52,4],[54,2],[65,2]]);
}
if ("variables".match(word)) {
result = result.concat([[17,7],[18,2],[52,2],[54,1],[55,10]]);
}
if ("vcbuild".match(word)) {
result = result.concat([[12,1]]);
}
if ("vcproj".match(word)) {
result = result.concat([[18,2]]);
}
if ("vendor".match(word)) {
result = result.concat([[15,2],[52,2]]);
}
if ("verify".match(word)) {
result = result.concat([[17,1],[28,2],[54,2]]);
}
if ("version".match(word)) {
result = result.concat([[2,2],[3,2],[12,1],[26,2],[39,2],[40,4],[49,4],[53,2],[54,15],[63,2]]);
}
if ("versions".match(word)) {
result = result.concat([[2,2],[12,1],[39,2]]);
}
if ("view".match(word)) {
result = result.concat([[28,2]]);
}
if ("visual".match(word)) {
result = result.concat([[12,6],[17,1],[18,4]]);
}
if ("w".match(word)) {
result = result.concat([[10,4],[22,10],[24,20],[47,4],[61,43]]);
}
if ("want".match(word)) {
result = result.concat([[28,4]]);
}
if ("website".match(word)) {
result = result.concat([[3,2],[4,2],[36,1],[40,2],[41,2]]);
}
if ("websites".match(word)) {
result = result.concat([[36,1]]);
}
if ("wfs".match(word)) {
result = result.concat([[10,2],[22,5],[24,10],[47,2],[61,6]]);
}
if ("whether".match(word)) {
result = result.concat([[1,2],[28,2],[38,2]]);
}
if ("whose".match(word)) {
result = result.concat([[24,1]]);
}
if ("win32".match(word)) {
result = result.concat([[13,1],[32,1]]);
}
if ("window".match(word)) {
result = result.concat([[3,2],[5,2],[10,2],[17,2],[22,2],[24,2],[28,9],[35,1],[40,2],[42,2],[47,2],[54,2],[59,2],[61,2]]);
}
if ("window:".match(word)) {
result = result.concat([[27,2],[29,2],[30,2],[31,2],[33,4],[34,2],[35,2],[36,2],[64,2],[66,2],[67,2],[68,2],[69,2],[70,2],[71,2]]);
}
if ("windows".match(word)) {
result = result.concat([[0,5],[1,3],[12,1],[13,2],[16,2],[32,1],[65,2]]);
}
if ("winnt".match(word)) {
result = result.concat([[28,1]]);
}
if ("wish".match(word)) {
result = result.concat([[17,1],[28,4]]);
}
if ("within".match(word)) {
result = result.concat([[28,4]]);
}
if ("withmodule".match(word)) {
result = result.concat([[10,1],[22,1],[24,1],[47,1],[61,1]]);
}
if ("withmodule:".match(word)) {
result = result.concat([[10,1],[22,1],[24,1],[47,1],[61,1]]);
}
if ("wizard".match(word)) {
result = result.concat([[28,2]]);
}
if ("wm".match(word)) {
result = result.concat([[32,1]]);
}
if ("wms".match(word)) {
result = result.concat([[10,2],[22,2],[24,10],[26,2],[36,5],[47,2],[61,8],[63,2],[71,5]]);
}
if ("work".match(word)) {
result = result.concat([[12,1],[54,1]]);
}
if ("works".match(word)) {
result = result.concat([[54,2]]);
}
if ("workshop".match(word)) {
result = result.concat([[13,2]]);
}
if ("worse".match(word)) {
result = result.concat([[2,2],[39,2]]);
}
if ("write".match(word)) {
result = result.concat([[22,2],[24,4]]);
}
if ("writing".match(word)) {
result = result.concat([[49,1],[53,2]]);
}
if ("written".match(word)) {
result = result.concat([[22,20],[24,44],[54,4],[61,28]]);
}
if ("x".match(word)) {
result = result.concat([[7,2],[44,2]]);
}
if ("x/providers/arcsde".match(word)) {
result = result.concat([[7,2],[44,2]]);
}
if ("x/providers/gdal".match(word)) {
result = result.concat([[7,2],[44,2]]);
}
if ("x/providers/genericrdbms".match(word)) {
result = result.concat([[7,2],[44,2]]);
}
if ("x/providers/sdf".match(word)) {
result = result.concat([[7,2],[44,2]]);
}
if ("x/providers/shp".match(word)) {
result = result.concat([[7,2],[44,2]]);
}
if ("x/providers/wfs".match(word)) {
result = result.concat([[7,2],[44,2]]);
}
if ("x/providers/wms".match(word)) {
result = result.concat([[7,2],[44,2]]);
}
if ("x86".match(word)) {
result = result.concat([[16,2],[53,2],[54,7]]);
}
if ("xalanroot".match(word)) {
result = result.concat([[17,1]]);
}
if ("xercescroot".match(word)) {
result = result.concat([[17,1]]);
}
if ("xml".match(word)) {
result = result.concat([[24,3],[61,2]]);
}
if ("xp".match(word)) {
result = result.concat([[1,1],[12,1]]);
}
if ("xxxx".match(word)) {
result = result.concat([[31,1],[32,5],[67,1],[68,16]]);
}
if ("xxxx”".match(word)) {
result = result.concat([[31,1],[67,1],[68,3]]);
}
if ("y".match(word)) {
result = result.concat([[12,2],[23,6],[60,6]]);
}
if ("yacc".match(word)) {
result = result.concat([[20,1],[57,1]]);
}
if ("yourpassword".match(word)) {
result = result.concat([[10,4],[47,4]]);
}
if ("yourusername".match(word)) {
result = result.concat([[10,4],[47,4]]);
}
if ("  ".match(word)) {
result = result.concat([[6,2],[7,16],[8,16],[43,2],[44,16],[45,16]]);
}
if ("–a".match(word)) {
result = result.concat([[61,4]]);
}
if ("–c".match(word)) {
result = result.concat([[61,2]]);
}
if ("‘connected’".match(word)) {
result = result.concat([[54,1]]);
}
if ("‘oracle".match(word)) {
result = result.concat([[54,1]]);
}
if ("‘oracle’".match(word)) {
result = result.concat([[54,1]]);
}
if ("‘testmultidb’".match(word)) {
result = result.concat([[28,4]]);
}
if ("‘testsingledb’".match(word)) {
result = result.concat([[28,4]]);
}
if ("‘u’".match(word)) {
result = result.concat([[28,2]]);
}
if ("’".match(word)) {
result = result.concat([[54,1]]);
}
if ("“checkout”".match(word)) {
result = result.concat([[4,3],[41,3]]);
}
if ("“database".match(word)) {
result = result.concat([[32,1]]);
}
if ("“dsnoracle".match(word)) {
result = result.concat([[68,1]]);
}
if ("“environment".match(word)) {
result = result.concat([[52,2],[54,1]]);
}
if ("“fdo".match(word)) {
result = result.concat([[28,2]]);
}
if ("“hardcoded".match(word)) {
result = result.concat([[28,2]]);
}
if ("“password".match(word)) {
result = result.concat([[31,2],[67,2],[68,2]]);
}
if ("“provider".match(word)) {
result = result.concat([[31,1],[67,1]]);
}
if ("“service".match(word)) {
result = result.concat([[31,2],[67,2],[68,1]]);
}
if ("“servicemysql".match(word)) {
result = result.concat([[32,1]]);
}
if ("“serviceoracle".match(word)) {
result = result.concat([[32,1],[68,1]]);
}
if ("“servicesqlserver".match(word)) {
result = result.concat([[32,1]]);
}
if ("“usernameoracle".match(word)) {
result = result.concat([[68,1]]);
}
if ("“xxxx”".match(word)) {
result = result.concat([[68,2]]);
}
if ("”".match(word)) {
result = result.concat([[28,2],[31,3],[32,3],[52,2],[54,1],[67,3],[68,2]]);
}
if ("”y”".match(word)) {
result = result.concat([[28,4]]);
}
return result; }