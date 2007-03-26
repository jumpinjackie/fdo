function getWordOccurences(word) {
var result = new Array();
if ("/".match(word)) {
result = result.concat([[50,6],[51,4],[52,10],[72,2],[81,2]]);
}
if ("//".match(word)) {
result = result.concat([[16,6],[18,2],[27,16],[29,14],[31,10],[33,6],[35,6],[37,6],[50,2],[51,2],[52,66],[55,70],[56,26],[61,26],[71,14],[83,4],[87,102],[91,4],[109,4]]);
}
if ("//buildings”/".match(word)) {
result = result.concat([[51,4]]);
}
if ("//samplefeatureclass".match(word)) {
result = result.concat([[52,2]]);
}
if ("//  ".match(word)) {
result = result.concat([[61,8]]);
}
if ("/bin/fdordbms".match(word)) {
result = result.concat([[22,1]]);
}
if ("/column".match(word)) {
result = result.concat([[51,6],[52,6]]);
}
if ("/constraint".match(word)) {
result = result.concat([[51,4],[52,2]]);
}
if ("/docs/xmlschema/fdodocument".match(word)) {
result = result.concat([[50,2]]);
}
if ("/name".match(word)) {
result = result.concat([[51,10],[52,8]]);
}
if ("/table".match(word)) {
result = result.concat([[51,2],[52,2]]);
}
if ("/type".match(word)) {
result = result.concat([[51,8],[52,6]]);
}
if ("/usr/local/fdo".match(word)) {
result = result.concat([[19,4]]);
}
if ("/xs:annotation".match(word)) {
result = result.concat([[50,8],[51,26],[52,8]]);
}
if ("/xs:complexcontent".match(word)) {
result = result.concat([[50,2],[51,6],[52,2]]);
}
if ("/xs:complextype".match(word)) {
result = result.concat([[50,2],[51,6],[52,2]]);
}
if ("/xs:documentation".match(word)) {
result = result.concat([[50,10],[51,32],[52,10]]);
}
if ("/xs:element".match(word)) {
result = result.concat([[50,10],[51,18],[52,6]]);
}
if ("/xs:extension".match(word)) {
result = result.concat([[50,2],[51,6],[52,2]]);
}
if ("/xs:key".match(word)) {
result = result.concat([[50,2],[51,6],[52,2]]);
}
if ("/xs:restriction".match(word)) {
result = result.concat([[50,4],[51,4],[52,2]]);
}
if ("/xs:schema".match(word)) {
result = result.concat([[50,2],[51,8],[52,2]]);
}
if ("/xs:sequence".match(word)) {
result = result.concat([[50,2],[51,6],[52,2]]);
}
if ("/xs:simpletype".match(word)) {
result = result.concat([[50,2],[51,4],[52,2]]);
}
if ("0".match(word)) {
result = result.concat([[10,1],[13,2],[16,2],[20,8],[22,7],[23,12],[33,10],[35,6],[37,8],[50,4],[51,6],[52,4],[55,42],[56,2],[57,2],[61,12],[78,2],[79,2],[83,50],[87,26],[90,1],[92,2],[96,2],[109,2],[110,44],[123,1]]);
}
if ("0/nls".match(word)) {
result = result.concat([[19,2]]);
}
if ("0/nls/".match(word)) {
result = result.concat([[19,2]]);
}
if ("000".match(word)) {
result = result.concat([[13,2],[92,2]]);
}
if ("000x10".match(word)) {
result = result.concat([[13,2],[92,2]]);
}
if ("02/24/05".match(word)) {
result = result.concat([[105,2],[106,2],[107,2]]);
}
if ("046r1".match(word)) {
result = result.concat([[51,7],[52,2],[110,4]]);
}
if ("050".match(word)) {
result = result.concat([[86,2]]);
}
if ("0x01".match(word)) {
result = result.concat([[91,1]]);
}
if ("0x02".match(word)) {
result = result.concat([[91,1]]);
}
if ("0x04".match(word)) {
result = result.concat([[91,1]]);
}
if ("0x08".match(word)) {
result = result.concat([[91,1]]);
}
if ("0”".match(word)) {
result = result.concat([[51,2]]);
}
if ("0”/".match(word)) {
result = result.concat([[51,6]]);
}
if ("1".match(word)) {
result = result.concat([[13,2],[19,2],[22,1],[50,6],[51,4],[52,2],[61,2],[79,2],[83,14],[87,10],[90,1],[93,4],[99,6],[106,2],[107,2],[114,2],[117,2],[120,2],[123,1]]);
}
if ("10".match(word)) {
result = result.concat([[13,2],[52,2],[55,4],[80,2],[83,79],[87,2],[90,1],[92,2],[109,4],[110,26]]);
}
if ("100".match(word)) {
result = result.concat([[110,22]]);
}
if ("101".match(word)) {
result = result.concat([[55,2],[56,4],[57,2],[61,2]]);
}
if ("11".match(word)) {
result = result.concat([[83,37],[87,2],[90,1],[123,1]]);
}
if ("11:00:02".match(word)) {
result = result.concat([[80,2]]);
}
if ("12".match(word)) {
result = result.concat([[80,2],[83,31],[87,2],[90,1],[111,1],[114,1],[117,1],[120,1],[123,1]]);
}
if ("123".match(word)) {
result = result.concat([[111,1]]);
}
if ("12345678901234567".match(word)) {
result = result.concat([[79,2]]);
}
if ("128".match(word)) {
result = result.concat([[39,2]]);
}
if ("13".match(word)) {
result = result.concat([[55,4],[87,2],[90,1]]);
}
if ("13:08:02".match(word)) {
result = result.concat([[105,2],[106,2],[107,2]]);
}
if ("14".match(word)) {
result = result.concat([[51,2]]);
}
if ("15".match(word)) {
result = result.concat([[83,8]]);
}
if ("160".match(word)) {
result = result.concat([[111,1]]);
}
if ("18".match(word)) {
result = result.concat([[55,10]]);
}
if ("18111".match(word)) {
result = result.concat([[105,2],[106,2],[107,2]]);
}
if ("1971".match(word)) {
result = result.concat([[80,2]]);
}
if ("2".match(word)) {
result = result.concat([[13,2],[14,2],[19,4],[23,1],[61,2],[66,2],[83,6],[87,14],[90,1],[93,4],[96,2],[109,4],[111,1],[114,1],[117,1],[120,1]]);
}
if ("20".match(word)) {
result = result.concat([[83,66],[105,2]]);
}
if ("200".match(word)) {
result = result.concat([[14,2],[66,2],[114,2]]);
}
if ("2003".match(word)) {
result = result.concat([[80,2]]);
}
if ("2147483647".match(word)) {
result = result.concat([[114,1]]);
}
if ("221".match(word)) {
result = result.concat([[19,2]]);
}
if ("23".match(word)) {
result = result.concat([[55,2],[80,2]]);
}
if ("24".match(word)) {
result = result.concat([[80,2]]);
}
if ("255".match(word)) {
result = result.concat([[114,2],[120,1],[123,6]]);
}
if ("28".match(word)) {
result = result.concat([[117,2]]);
}
if ("2d".match(word)) {
result = result.concat([[86,2],[93,2]]);
}
if ("2e13".match(word)) {
result = result.concat([[79,2]]);
}
if ("2”".match(word)) {
result = result.concat([[23,2]]);
}
if ("3".match(word)) {
result = result.concat([[10,1],[13,2],[19,4],[22,4],[23,3],[61,2],[79,2],[83,6],[87,10],[90,1],[93,4]]);
}
if ("30".match(word)) {
result = result.concat([[83,14],[114,1]]);
}
if ("32".match(word)) {
result = result.concat([[79,1],[111,1]]);
}
if ("333".match(word)) {
result = result.concat([[19,2]]);
}
if ("38".match(word)) {
result = result.concat([[109,4]]);
}
if ("3d".match(word)) {
result = result.concat([[13,2],[86,2],[93,4]]);
}
if ("3rd".match(word)) {
result = result.concat([[116,1]]);
}
if ("4".match(word)) {
result = result.concat([[14,2],[67,4],[68,2],[79,2],[83,2],[87,6],[90,1],[109,4],[111,2],[114,2],[117,2],[120,2],[123,1]]);
}
if ("40".match(word)) {
result = result.concat([[83,24]]);
}
if ("4294967296".match(word)) {
result = result.concat([[111,2]]);
}
if ("48".match(word)) {
result = result.concat([[55,2]]);
}
if ("4d".match(word)) {
result = result.concat([[86,1]]);
}
if ("5".match(word)) {
result = result.concat([[83,2],[87,2],[90,1]]);
}
if ("50".match(word)) {
result = result.concat([[110,22]]);
}
if ("5151/tcp                ".match(word)) {
result = result.concat([[108,2]]);
}
if ("52".match(word)) {
result = result.concat([[55,4]]);
}
if ("56".match(word)) {
result = result.concat([[117,1]]);
}
if ("59".match(word)) {
result = result.concat([[55,6]]);
}
if ("6".match(word)) {
result = result.concat([[55,2],[87,2],[90,1],[96,2],[109,6]]);
}
if ("64".match(word)) {
result = result.concat([[39,4],[41,2],[51,2],[52,6],[110,24],[111,1],[114,1]]);
}
if ("65".match(word)) {
result = result.concat([[111,1],[114,1]]);
}
if ("66".match(word)) {
result = result.concat([[55,2]]);
}
if ("67".match(word)) {
result = result.concat([[55,2]]);
}
if ("7".match(word)) {
result = result.concat([[87,2],[90,1],[123,1]]);
}
if ("8".match(word)) {
result = result.concat([[52,2],[83,2],[109,6],[111,1],[114,1],[117,1],[120,1],[126,1]]);
}
if ("8byte".match(word)) {
result = result.concat([[87,2]]);
}
if ("8bytes".match(word)) {
result = result.concat([[114,1],[117,1],[120,1]]);
}
if ("9".match(word)) {
result = result.concat([[55,2],[78,2],[79,2],[96,10],[99,10],[105,2]]);
}
if ("90".match(word)) {
result = result.concat([[83,12]]);
}
if ("95".match(word)) {
result = result.concat([[114,1]]);
}
if ("98".match(word)) {
result = result.concat([[51,7],[52,2],[110,4]]);
}
if ("98046r1".match(word)) {
result = result.concat([[110,1]]);
}
if ("980946r1".match(word)) {
result = result.concat([[110,2]]);
}
if ("99".match(word)) {
result = result.concat([[86,2]]);
}
if (":".match(word)) {
result = result.concat([[16,4],[20,4],[23,2],[43,2],[50,8],[51,8],[55,2],[56,2],[72,2],[76,2],[82,2],[83,2],[87,8],[105,10],[106,10],[107,12],[111,1],[114,1],[117,1],[120,1],[123,1]]);
}
if ("::".match(word)) {
result = result.concat([[71,34],[72,22],[83,42]]);
}
if ("\\fdo\\bin".match(word)) {
result = result.concat([[98,2]]);
}
if ("a18111".match(word)) {
result = result.concat([[107,2]]);
}
if ("aaa".match(word)) {
result = result.concat([[77,2]]);
}
if ("abc".match(word)) {
result = result.concat([[75,2]]);
}
if ("abide".match(word)) {
result = result.concat([[101,2]]);
}
if ("ability".match(word)) {
result = result.concat([[3,1]]);
}
if ("able".match(word)) {
result = result.concat([[42,2]]);
}
if ("above".match(word)) {
result = result.concat([[20,2],[23,4]]);
}
if ("absence".match(word)) {
result = result.concat([[99,2]]);
}
if ("abstract".match(word)) {
result = result.concat([[16,2],[41,6],[43,1],[51,2],[85,2],[89,9],[91,1]]);
}
if ("abstraction".match(word)) {
result = result.concat([[7,4],[13,2]]);
}
if ("accepted".match(word)) {
result = result.concat([[47,1],[48,2],[49,2]]);
}
if ("access".match(word)) {
result = result.concat([[6,2],[9,2],[10,9],[22,3],[23,1],[96,8],[108,4],[113,6],[116,9],[119,4],[122,7],[125,8],[128,6]]);
}
if ("accesses".match(word)) {
result = result.concat([[122,1]]);
}
if ("accessible".match(word)) {
result = result.concat([[10,1],[89,2],[113,1],[116,1]]);
}
if ("accessing".match(word)) {
result = result.concat([[7,2],[89,1]]);
}
if ("accomplish".match(word)) {
result = result.concat([[50,2]]);
}
if ("according".match(word)) {
result = result.concat([[87,2]]);
}
if ("account".match(word)) {
result = result.concat([[23,2]]);
}
if ("achieve".match(word)) {
result = result.concat([[6,2],[43,2]]);
}
if ("acid".match(word)) {
result = result.concat([[113,1]]);
}
if ("acquirelock".match(word)) {
result = result.concat([[111,1]]);
}
if ("across".match(word)) {
result = result.concat([[41,2],[46,2]]);
}
if ("act".match(word)) {
result = result.concat([[45,2]]);
}
if ("actions".match(word)) {
result = result.concat([[46,2],[89,1]]);
}
if ("activate".match(word)) {
result = result.concat([[14,2],[64,2]]);
}
if ("activated".match(word)) {
result = result.concat([[64,2]]);
}
if ("activatelongtransaction".match(word)) {
result = result.concat([[64,2],[111,1]]);
}
if ("activates".match(word)) {
result = result.concat([[64,4]]);
}
if ("activatespatialcontext".match(word)) {
result = result.concat([[111,1]]);
}
if ("active".match(word)) {
result = result.concat([[64,14],[92,3]]);
}
if ("acts".match(word)) {
result = result.concat([[46,4],[108,2],[128,1]]);
}
if ("actual".match(word)) {
result = result.concat([[13,2]]);
}
if ("adapt".match(word)) {
result = result.concat([[6,2]]);
}
if ("add".match(word)) {
result = result.concat([[14,2],[23,2],[43,5],[47,2],[51,17],[52,26],[55,26],[56,6],[72,2],[81,4],[93,2],[110,30]]);
}
if ("added".match(word)) {
result = result.concat([[16,2],[18,2],[41,2],[48,4],[49,2],[51,5],[55,8],[86,1]]);
}
if ("adding".match(word)) {
result = result.concat([[48,1],[55,6],[110,2]]);
}
if ("addition".match(word)) {
result = result.concat([[9,3],[16,2],[57,2]]);
}
if ("additional".match(word)) {
result = result.concat([[3,5],[23,2]]);
}
if ("additionally".match(word)) {
result = result.concat([[48,2]]);
}
if ("addref".match(word)) {
result = result.concat([[16,21],[20,2]]);
}
if ("address".match(word)) {
result = result.concat([[13,6],[51,4],[110,2]]);
}
if ("adds".match(word)) {
result = result.concat([[55,2]]);
}
if ("administration".match(word)) {
result = result.concat([[14,2],[63,2]]);
}
if ("administrator".match(word)) {
result = result.concat([[23,2],[110,2]]);
}
if ("advanced".match(word)) {
result = result.concat([[102,3]]);
}
if ("affect".match(word)) {
result = result.concat([[3,1],[14,2]]);
}
if ("affected".match(word)) {
result = result.concat([[64,1],[68,4]]);
}
if ("affects".match(word)) {
result = result.concat([[46,2]]);
}
if ("again".match(word)) {
result = result.concat([[22,2],[108,2]]);
}
if ("against".match(word)) {
result = result.concat([[9,1],[14,6],[41,2],[60,6],[67,4],[68,4]]);
}
if ("aggregation".match(word)) {
result = result.concat([[13,2]]);
}
if ("aliases".match(word)) {
result = result.concat([[110,2]]);
}
if ("alignment".match(word)) {
result = result.concat([[86,3]]);
}
if ("allocate".match(word)) {
result = result.concat([[16,2]]);
}
if ("allocated".match(word)) {
result = result.concat([[16,2]]);
}
if ("allocation      :".match(word)) {
result = result.concat([[106,2],[107,2]]);
}
if ("allow".match(word)) {
result = result.concat([[13,2],[22,2],[23,2],[41,2],[75,2],[76,4],[78,2],[86,2],[91,2]]);
}
if ("allowed".match(word)) {
result = result.concat([[87,4],[91,2]]);
}
if ("allowing".match(word)) {
result = result.concat([[6,4],[9,1],[14,2]]);
}
if ("allows".match(word)) {
result = result.concat([[43,2],[49,2],[64,6],[87,2]]);
}
if ("almost".match(word)) {
result = result.concat([[89,2]]);
}
if ("alone".match(word)) {
result = result.concat([[45,4],[46,11],[64,1]]);
}
if ("along".match(word)) {
result = result.concat([[92,2]]);
}
if ("alphanumeric".match(word)) {
result = result.concat([[75,2],[76,2]]);
}
if ("already".match(word)) {
result = result.concat([[43,1],[51,4],[108,2],[110,2]]);
}
if ("alter".match(word)) {
result = result.concat([[104,1],[106,2],[107,2],[110,28]]);
}
if ("alteration".match(word)) {
result = result.concat([[104,2]]);
}
if ("alternate".match(word)) {
result = result.concat([[43,2]]);
}
if ("alternates".match(word)) {
result = result.concat([[14,2],[63,2]]);
}
if ("alternative".match(word)) {
result = result.concat([[10,2],[119,1],[122,1]]);
}
if ("although".match(word)) {
result = result.concat([[13,2],[43,2],[46,2],[119,1],[122,1]]);
}
if ("always".match(word)) {
result = result.concat([[13,2],[37,2],[44,2],[64,2]]);
}
if ("analogue".match(word)) {
result = result.concat([[88,2]]);
}
if ("analogues".match(word)) {
result = result.concat([[91,2]]);
}
if ("analysis".match(word)) {
result = result.concat([[7,2],[9,1],[14,4],[102,1]]);
}
if ("analyze".match(word)) {
result = result.concat([[4,2]]);
}
if ("analyzer".match(word)) {
result = result.concat([[83,2]]);
}
if ("ancestor".match(word)) {
result = result.concat([[14,2],[63,2]]);
}
if ("ancestors".match(word)) {
result = result.concat([[64,2]]);
}
if ("and/or".match(word)) {
result = result.concat([[50,2],[51,2],[64,2]]);
}
if ("angular".match(word)) {
result = result.concat([[92,2]]);
}
if ("anno".match(word)) {
result = result.concat([[100,1]]);
}
if ("annotation".match(word)) {
result = result.concat([[100,1]]);
}
if ("anything".match(word)) {
result = result.concat([[19,2],[98,2]]);
}
if ("api".match(word)) {
result = result.concat([[0,3],[1,2],[2,4],[3,2],[4,4],[5,5],[6,2],[7,12],[9,18],[10,5],[11,2],[12,5],[13,2],[15,2],[18,2],[22,4],[23,4],[24,4],[25,7],[27,2],[29,2],[31,2],[41,6],[53,2],[58,2],[59,2],[64,2],[84,7],[85,6],[88,2],[89,9],[91,4],[92,2],[95,2],[96,6],[100,2],[102,14],[103,4],[111,14],[112,2],[113,12],[114,14],[115,2],[116,8],[117,14],[118,2],[119,8],[120,14],[121,2],[122,8],[123,14],[124,2],[125,6],[126,14],[127,2],[128,8],[129,14]]);
}
if ("api:".match(word)) {
result = result.concat([[41,2],[102,2]]);
}
if ("apis".match(word)) {
result = result.concat([[4,2],[6,2],[9,2],[85,2]]);
}
if ("api”".match(word)) {
result = result.concat([[84,2]]);
}
if ("appear".match(word)) {
result = result.concat([[13,2],[89,1]]);
}
if ("appendix".match(word)) {
result = result.concat([[10,2],[22,2],[42,2],[95,2],[112,2],[115,2],[118,2],[121,2],[124,2],[127,2]]);
}
if ("appendixes:".match(word)) {
result = result.concat([[2,2]]);
}
if ("application".match(word)) {
result = result.concat([[0,2],[1,2],[2,2],[4,2],[6,5],[7,5],[9,1],[11,2],[14,14],[18,2],[22,1],[23,8],[46,2],[62,2],[66,6],[67,4],[68,2],[91,2],[96,2],[99,2]]);
}
if ("applications".match(word)) {
result = result.concat([[1,2],[2,1],[6,2],[9,2],[11,5],[42,2],[85,2],[99,2],[116,1]]);
}
if ("applied".match(word)) {
result = result.concat([[14,2],[50,4],[55,2],[60,2]]);
}
if ("applies".match(word)) {
result = result.concat([[14,4],[66,2]]);
}
if ("apply".match(word)) {
result = result.concat([[2,7],[14,2],[63,2]]);
}
if ("applyschema".match(word)) {
result = result.concat([[52,6],[114,1],[120,1],[123,1]]);
}
if ("approach".match(word)) {
result = result.concat([[68,2]]);
}
if ("appropriate".match(word)) {
result = result.concat([[10,2],[14,2],[22,2],[42,2],[47,3],[55,1]]);
}
if ("approximate".match(word)) {
result = result.concat([[103,2]]);
}
if ("approximation".match(word)) {
result = result.concat([[103,2]]);
}
if ("arc".match(word)) {
result = result.concat([[13,5],[83,16],[87,2],[103,4]]);
}
if ("arceditor®".match(word)) {
result = result.concat([[99,2]]);
}
if ("arcgis".match(word)) {
result = result.concat([[96,4],[99,6],[102,7]]);
}
if ("architecture".match(word)) {
result = result.concat([[3,5],[9,8],[10,1],[113,2]]);
}
if ("arcinfo®".match(word)) {
result = result.concat([[99,2]]);
}
if ("arcobjects".match(word)) {
result = result.concat([[102,7],[103,2]]);
}
if ("arcsde".match(word)) {
result = result.concat([[3,1],[9,3],[10,2],[22,2],[23,2],[62,1],[95,7],[96,11],[97,5],[98,2],[99,20],[100,10],[101,9],[102,2],[103,6],[104,7],[108,21],[109,2],[110,41],[111,5]]);
}
if ("arcsde®".match(word)) {
result = result.concat([[2,1],[96,2]]);
}
if ("arcview®".match(word)) {
result = result.concat([[99,2]]);
}
if ("are:".match(word)) {
result = result.concat([[11,2],[54,2],[89,2],[91,2]]);
}
if ("area".match(word)) {
result = result.concat([[91,1],[110,2]]);
}
if ("areas:".match(word)) {
result = result.concat([[3,2]]);
}
if ("arg".match(word)) {
result = result.concat([[50,10],[51,17]]);
}
if ("argument".match(word)) {
result = result.concat([[16,8],[19,4],[23,3],[51,2],[55,1],[93,2],[110,4]]);
}
if ("argumentdefinition".match(word)) {
result = result.concat([[33,8]]);
}
if ("argumentdescription".match(word)) {
result = result.concat([[33,2]]);
}
if ("argumentname".match(word)) {
result = result.concat([[33,2]]);
}
if ("arguments".match(word)) {
result = result.concat([[23,2],[33,6],[61,1],[82,2],[110,3]]);
}
if ("argumenttype".match(word)) {
result = result.concat([[33,2]]);
}
if ("array".match(word)) {
result = result.concat([[16,2],[23,5],[55,10],[61,2],[87,2]]);
}
if ("arrays".match(word)) {
result = result.concat([[55,2],[87,2],[89,2]]);
}
if ("ascending".match(word)) {
result = result.concat([[45,2]]);
}
if ("aspect".match(word)) {
result = result.concat([[13,2],[92,2]]);
}
if ("asphalt".match(word)) {
result = result.concat([[14,2],[67,2]]);
}
if ("assembled".match(word)) {
result = result.concat([[9,2]]);
}
if ("assign".match(word)) {
result = result.concat([[20,2],[23,2]]);
}
if ("assigned".match(word)) {
result = result.concat([[61,2]]);
}
if ("assigning".match(word)) {
result = result.concat([[20,7]]);
}
if ("assigns".match(word)) {
result = result.concat([[16,2]]);
}
if ("associated".match(word)) {
result = result.concat([[10,1],[13,4],[22,2],[43,1],[44,2],[45,2],[46,6],[54,2],[64,2],[122,1]]);
}
if ("association".match(word)) {
result = result.concat([[13,13],[43,1],[45,2],[114,1],[120,1]]);
}
if ("associationproperty".match(word)) {
result = result.concat([[111,1],[120,1],[123,1]]);
}
if ("assume".match(word)) {
result = result.concat([[23,2]]);
}
if ("assumed".match(word)) {
result = result.concat([[83,3]]);
}
if ("assumes".match(word)) {
result = result.concat([[25,2],[110,2]]);
}
if ("assuming".match(word)) {
result = result.concat([[23,2],[108,2]]);
}
if ("assumption".match(word)) {
result = result.concat([[23,1]]);
}
if ("atomic".match(word)) {
result = result.concat([[14,7]]);
}
if ("attach".match(word)) {
result = result.concat([[56,4],[57,2]]);
}
if ("attempt".match(word)) {
result = result.concat([[23,1]]);
}
if ("attribute".match(word)) {
result = result.concat([[10,1],[13,2],[46,2],[50,6],[51,5],[122,2]]);
}
if ("attributes".match(word)) {
result = result.concat([[7,2],[10,1],[13,2],[49,2],[50,4],[51,2],[92,2],[122,1]]);
}
if ("attributes:".match(word)) {
result = result.concat([[92,2]]);
}
if ("audience".match(word)) {
result = result.concat([[1,5]]);
}
if ("authority".match(word)) {
result = result.concat([[14,2]]);
}
if ("auto".match(word)) {
result = result.concat([[45,1],[111,2],[114,2],[117,2],[120,2],[123,2]]);
}
if ("autodesk".match(word)) {
result = result.concat([[3,2],[6,2],[9,2],[10,1],[22,2],[25,1],[62,1],[83,4],[85,2],[119,2]]);
}
if ("autodesk’s".match(word)) {
result = result.concat([[10,1]]);
}
if ("autogeneratedtypes".match(word)) {
result = result.concat([[29,4]]);
}
if ("automate".match(word)) {
result = result.concat([[23,2]]);
}
if ("automatically".match(word)) {
result = result.concat([[14,2],[16,2],[77,2],[103,2]]);
}
if ("availability".match(word)) {
result = result.concat([[114,2],[117,2]]);
}
if ("available".match(word)) {
result = result.concat([[7,2],[9,2],[60,2],[64,2],[74,2],[99,2],[108,2],[125,1]]);
}
if ("available:".match(word)) {
result = result.concat([[64,4],[81,6]]);
}
if ("avg".match(word)) {
result = result.concat([[111,1],[114,1],[117,1],[120,1],[123,1]]);
}
if ("avoiding".match(word)) {
result = result.concat([[89,1]]);
}
if ("back".match(word)) {
result = result.concat([[14,2],[50,4],[64,2],[103,2]]);
}
if ("back”".match(word)) {
result = result.concat([[49,2]]);
}
if ("base".match(word)) {
result = result.concat([[10,1],[13,2],[41,15],[44,10],[46,2],[50,6],[51,12],[52,4],[91,1]]);
}
if ("baseclass".match(word)) {
result = result.concat([[50,6],[51,6]]);
}
if ("based".match(word)) {
result = result.concat([[2,1],[6,2],[9,1],[10,10],[13,4],[22,3],[41,6],[64,2],[66,2],[67,2],[68,2],[87,2],[89,4],[96,2],[100,2],[113,4],[116,2],[119,2],[122,2],[125,3],[128,5]]);
}
if ("basic".match(word)) {
result = result.concat([[12,2],[43,2],[60,2],[87,7],[111,2],[114,2],[117,2],[120,2],[123,2],[126,2],[128,1]]);
}
if ("basis".match(word)) {
result = result.concat([[13,2],[128,1]]);
}
if ("bbb".match(word)) {
result = result.concat([[77,2]]);
}
if ("becomes".match(word)) {
result = result.concat([[23,1],[110,2]]);
}
if ("becoming".match(word)) {
result = result.concat([[108,4]]);
}
if ("begin".match(word)) {
result = result.concat([[14,2]]);
}
if ("behavior".match(word)) {
result = result.concat([[20,4]]);
}
if ("behaviors".match(word)) {
result = result.concat([[20,5]]);
}
if ("belong".match(word)) {
result = result.concat([[41,2],[50,2]]);
}
if ("belonging".match(word)) {
result = result.concat([[22,2],[25,2],[52,2],[55,5],[110,2]]);
}
if ("below".match(word)) {
result = result.concat([[64,2],[110,1]]);
}
if ("best".match(word)) {
result = result.concat([[2,1]]);
}
if ("beyond".match(word)) {
result = result.concat([[71,2],[73,2],[111,1]]);
}
if ("bezier".match(word)) {
result = result.concat([[102,1]]);
}
if ("biggest".match(word)) {
result = result.concat([[68,2]]);
}
if ("bin".match(word)) {
result = result.concat([[19,2]]);
}
if ("binaries".match(word)) {
result = result.concat([[99,4]]);
}
if ("binary".match(word)) {
result = result.concat([[13,2],[55,1],[80,1],[81,2],[86,2],[87,5],[88,2],[89,2]]);
}
if ("binaryexpression".match(word)) {
result = result.concat([[72,4]]);
}
if ("binarylogicaloperations".match(word)) {
result = result.concat([[71,4]]);
}
if ("binarylogicaloperator".match(word)) {
result = result.concat([[71,4]]);
}
if ("binaryoperations".match(word)) {
result = result.concat([[81,5]]);
}
if ("bit".match(word)) {
result = result.concat([[41,2],[79,1],[87,2],[91,1]]);
}
if ("bit/pixel/unsigned".match(word)) {
result = result.concat([[129,5]]);
}
if ("bitonal/1".match(word)) {
result = result.concat([[129,1]]);
}
if ("bits".match(word)) {
result = result.concat([[13,2]]);
}
if ("blanks".match(word)) {
result = result.concat([[76,2]]);
}
if ("blob".match(word)) {
result = result.concat([[50,2],[55,1],[72,2],[80,1],[109,4],[111,3],[120,1],[123,1],[129,6]]);
}
if ("block".match(word)) {
result = result.concat([[87,2]]);
}
if ("blue".match(word)) {
result = result.concat([[55,12]]);
}
if ("bluelake".match(word)) {
result = result.concat([[55,6]]);
}
if ("bluelakeexteriorringordinates".match(word)) {
result = result.concat([[55,2]]);
}
if ("bnf".match(word)) {
result = result.concat([[71,4],[88,2]]);
}
if ("bool".match(word)) {
result = result.concat([[23,14],[27,12],[29,18],[31,14],[35,4],[39,8]]);
}
if ("boolean".match(word)) {
result = result.concat([[13,2],[50,2],[55,1],[109,2],[111,1],[114,3],[117,2],[120,3],[123,2],[126,2]]);
}
if ("boolean:".match(word)) {
result = result.concat([[61,2]]);
}
if ("bottom".match(word)) {
result = result.concat([[72,2]]);
}
if ("bound".match(word)) {
result = result.concat([[13,2]]);
}
if ("boundary".match(word)) {
result = result.concat([[55,4],[110,4]]);
}
if ("bounds”".match(word)) {
result = result.concat([[110,1]]);
}
if ("box".match(word)) {
result = result.concat([[23,4],[110,1]]);
}
if ("breaks".match(word)) {
result = result.concat([[11,2]]);
}
if ("bridges".match(word)) {
result = result.concat([[110,6]]);
}
if ("brief".match(word)) {
result = result.concat([[22,1]]);
}
if ("bsupportscreatedatastore".match(word)) {
result = result.concat([[23,4]]);
}
if ("buffer".match(word)) {
result = result.concat([[110,1]]);
}
if ("build".match(word)) {
result = result.concat([[11,1],[14,2],[67,2]]);
}
if ("builders".match(word)) {
result = result.concat([[66,2]]);
}
if ("building".match(word)) {
result = result.concat([[51,5],[87,2],[110,2]]);
}
if ("buildings".match(word)) {
result = result.concat([[51,10],[110,8]]);
}
if ("buildings”".match(word)) {
result = result.concat([[51,1]]);
}
if ("building”".match(word)) {
result = result.concat([[51,2]]);
}
if ("buildmajor".match(word)) {
result = result.concat([[22,2]]);
}
if ("buildminor".match(word)) {
result = result.concat([[22,2]]);
}
if ("built".match(word)) {
result = result.concat([[14,4],[63,4],[87,2]]);
}
if ("byfdoptr".match(word)) {
result = result.concat([[16,2]]);
}
if ("byte".match(word)) {
result = result.concat([[13,2],[50,2],[55,11],[61,2],[86,5],[87,6],[89,2],[109,2],[111,1],[114,8],[117,7],[120,10],[123,7],[126,2]]);
}
if ("bytearray".match(word)) {
result = result.concat([[61,4]]);
}
if ("bytes".match(word)) {
result = result.concat([[111,5],[114,5],[117,5],[120,5],[123,2],[126,8],[129,1]]);
}
if ("c".match(word)) {
result = result.concat([[87,2],[106,4],[110,48]]);
}
if ("c:\\arcgis\\arcsde\\bin".match(word)) {
result = result.concat([[99,2]]);
}
if ("c:\\program".match(word)) {
result = result.concat([[99,4]]);
}
if ("c:\\windows\\system32\\command".match(word)) {
result = result.concat([[110,1]]);
}
if ("c:\\windows\\system32\\drivers\\etc".match(word)) {
result = result.concat([[108,2]]);
}
if ("cad".match(word)) {
result = result.concat([[6,2]]);
}
if ("call".match(word)) {
result = result.concat([[16,4],[19,2],[20,7],[23,8],[47,1],[49,2],[55,1],[83,2],[108,12]]);
}
if ("call:".match(word)) {
result = result.concat([[91,1]]);
}
if ("called".match(word)) {
result = result.concat([[13,10],[16,2],[19,2],[41,2],[52,10],[84,2],[110,7]]);
}
if ("calling".match(word)) {
result = result.concat([[23,4],[48,3],[49,2],[50,4],[61,2],[111,14],[114,14],[117,14],[120,14],[123,14],[126,14],[129,14]]);
}
if ("calls".match(word)) {
result = result.concat([[16,6],[20,7],[23,4],[27,7],[29,7],[31,7],[33,7],[35,7],[37,7],[39,7]]);
}
if ("cancelled".match(word)) {
result = result.concat([[14,2]]);
}
if ("cancels".match(word)) {
result = result.concat([[14,2]]);
}
if ("cannot".match(word)) {
result = result.concat([[14,1],[19,4],[43,1],[44,6],[46,2],[48,2],[73,2],[104,1],[110,2],[111,1],[114,1],[117,1],[120,1],[123,1],[126,1]]);
}
if ("capabilities".match(word)) {
result = result.concat([[2,2],[3,1],[6,2],[7,2],[9,5],[10,1],[23,2],[24,11],[25,17],[26,5],[28,5],[30,5],[32,5],[34,5],[36,5],[38,5],[66,4],[90,1],[111,80],[113,2],[114,80],[117,80],[120,80],[123,80],[126,80],[129,78]]);
}
if ("capability".match(word)) {
result = result.concat([[9,1],[24,2],[25,4],[92,2],[113,1],[129,2]]);
}
if ("capture".match(word)) {
result = result.concat([[61,2]]);
}
if ("cardinality".match(word)) {
result = result.concat([[13,2]]);
}
if ("care”".match(word)) {
result = result.concat([[90,1]]);
}
if ("cascading".match(word)) {
result = result.concat([[13,2]]);
}
if ("case".match(word)) {
result = result.concat([[18,2],[22,3],[23,1],[44,2],[46,2],[51,2],[55,2],[73,2],[96,2]]);
}
if ("cases".match(word)) {
result = result.concat([[13,2],[18,2],[43,2],[92,2]]);
}
if ("cast".match(word)) {
result = result.concat([[23,2],[61,4],[93,4]]);
}
if ("cat".match(word)) {
result = result.concat([[19,2]]);
}
if ("catalog".match(word)) {
result = result.concat([[19,2]]);
}
if ("catalogs".match(word)) {
result = result.concat([[19,4]]);
}
if ("catch".match(word)) {
result = result.concat([[18,12],[23,1]]);
}
if ("categories".match(word)) {
result = result.concat([[9,1],[24,2],[25,4]]);
}
if ("categories:".match(word)) {
result = result.concat([[12,2],[25,2],[111,2],[114,2],[117,2],[120,2],[123,2],[126,2],[129,2]]);
}
if ("cause".match(word)) {
result = result.concat([[14,2],[99,2]]);
}
if ("ceil".match(word)) {
result = result.concat([[114,1],[117,1],[120,1],[123,1]]);
}
if ("centerline".match(word)) {
result = result.concat([[110,4]]);
}
if ("centerlines".match(word)) {
result = result.concat([[110,2]]);
}
if ("certain".match(word)) {
result = result.concat([[23,2],[66,2]]);
}
if ("ch/db".match(word)) {
result = result.concat([[71,2]]);
}
if ("chain".match(word)) {
result = result.concat([[20,7]]);
}
if ("change".match(word)) {
result = result.concat([[14,4],[22,4],[25,1],[68,2],[86,1],[92,2]]);
}
if ("changed".match(word)) {
result = result.concat([[48,3],[49,2],[56,2]]);
}
if ("changes".match(word)) {
result = result.concat([[3,15],[14,21],[43,1],[47,1],[48,6],[49,4],[63,4]]);
}
if ("changes:".match(word)) {
result = result.concat([[51,2]]);
}
if ("changing".match(word)) {
result = result.concat([[14,2],[63,2]]);
}
if ("chapter".match(word)) {
result = result.concat([[4,2],[10,4],[12,2],[15,2],[21,2],[24,2],[25,2],[40,2],[53,2],[55,2],[59,2],[61,2],[62,2],[65,2],[84,2],[108,2],[110,2]]);
}
if ("chapters".match(word)) {
result = result.concat([[2,2],[9,2],[11,2]]);
}
if ("character".match(word)) {
result = result.concat([[13,2],[55,1],[75,4],[77,6],[82,5]]);
}
if ("characteristics".match(word)) {
result = result.concat([[10,1],[113,2]]);
}
if ("characteristics:".match(word)) {
result = result.concat([[116,2],[119,2],[122,2],[128,2]]);
}
if ("characters".match(word)) {
result = result.concat([[69,1],[75,4],[76,2],[77,2],[78,2],[81,2],[82,2],[111,1],[114,1],[117,1],[120,1],[123,1],[126,1]]);
}
if ("checking".match(word)) {
result = result.concat([[76,2]]);
}
if ("checks".match(word)) {
result = result.concat([[23,2]]);
}
if ("child".match(word)) {
result = result.concat([[41,2],[44,6]]);
}
if ("chm".match(word)) {
result = result.concat([[113,2],[116,2],[119,2],[122,2],[128,2]]);
}
if ("choices".match(word)) {
result = result.concat([[23,2]]);
}
if ("choose".match(word)) {
result = result.concat([[23,4],[108,2],[113,2]]);
}
if ("chosen".match(word)) {
result = result.concat([[110,2]]);
}
if ("circular".match(word)) {
result = result.concat([[13,4]]);
}
if ("circulararc".match(word)) {
result = result.concat([[87,2]]);
}
if ("circulararccurvestringelement".match(word)) {
result = result.concat([[87,2]]);
}
if ("circulararcsegment".match(word)) {
result = result.concat([[13,1],[83,4],[120,2],[126,2]]);
}
if ("city".match(word)) {
result = result.concat([[13,2]]);
}
if ("clarity".match(word)) {
result = result.concat([[9,2]]);
}
if ("class".match(word)) {
result = result.concat([[3,2],[7,6],[10,1],[13,62],[16,6],[17,6],[18,8],[19,2],[20,4],[22,4],[27,1],[29,1],[31,1],[41,22],[42,2],[43,7],[44,20],[45,8],[46,93],[48,2],[50,4],[51,13],[52,30],[55,19],[56,16],[57,8],[58,5],[60,10],[61,2],[89,8],[110,6],[111,18],[114,18],[116,3],[117,18],[120,18],[122,1],[123,18],[126,18],[129,18]]);
}
if ("classdef".match(word)) {
result = result.concat([[61,4]]);
}
if ("classdefinition".match(word)) {
result = result.concat([[50,4]]);
}
if ("classes".match(word)) {
result = result.concat([[3,1],[9,11],[10,1],[13,8],[14,2],[16,8],[17,2],[18,4],[23,1],[41,23],[43,3],[46,4],[49,2],[50,2],[52,4],[58,2],[60,4],[67,2],[68,2],[69,2],[89,9],[110,13],[116,2],[122,1],[128,1]]);
}
if ("classification".match(word)) {
result = result.concat([[6,2],[10,2]]);
}
if ("classname".match(word)) {
result = result.concat([[41,4],[50,10],[51,11],[55,3],[56,2]]);
}
if ("classtypes".match(word)) {
result = result.concat([[29,6]]);
}
if ("clause".match(word)) {
result = result.concat([[14,2],[60,2],[67,2]]);
}
if ("clauses".match(word)) {
result = result.concat([[67,2],[68,4]]);
}
if ("clean".match(word)) {
result = result.concat([[18,2]]);
}
if ("cleans".match(word)) {
result = result.concat([[18,2]]);
}
if ("clear".match(word)) {
result = result.concat([[52,6]]);
}
if ("click".match(word)) {
result = result.concat([[23,1]]);
}
if ("client".match(word)) {
result = result.concat([[6,5],[7,5],[9,4],[90,1],[91,2],[113,1],[125,4]]);
}
if ("client/server".match(word)) {
result = result.concat([[10,2],[125,2]]);
}
if ("clients".match(word)) {
result = result.concat([[6,2],[49,2]]);
}
if ("clip".match(word)) {
result = result.concat([[129,1]]);
}
if ("clob".match(word)) {
result = result.concat([[50,2],[55,1],[72,2],[80,1],[109,2],[111,1],[120,1],[123,1]]);
}
if ("closed".match(word)) {
result = result.concat([[13,2]]);
}
if ("cluster".match(word)) {
result = result.concat([[113,1]]);
}
if ("cmd".match(word)) {
result = result.concat([[110,1]]);
}
if ("code".match(word)) {
result = result.concat([[1,2],[9,1],[16,2],[18,2],[19,4],[20,2],[22,2],[23,4],[24,2],[25,4],[27,5],[29,5],[31,5],[33,5],[35,5],[37,5],[39,5],[49,2],[50,12],[52,8],[55,2],[61,6],[93,4],[94,2]]);
}
if ("code:".match(word)) {
result = result.concat([[52,2]]);
}
if ("coding".match(word)) {
result = result.concat([[11,2]]);
}
if ("collection".match(word)) {
result = result.concat([[9,5],[13,8],[17,4],[22,2],[23,2],[41,2],[43,2],[45,3],[46,8],[47,4],[48,3],[50,2],[52,14],[55,17],[56,4],[83,6],[113,1]]);
}
if ("collections".match(word)) {
result = result.concat([[17,5],[41,2],[45,2],[46,2],[47,1],[48,3]]);
}
if ("colon".match(word)) {
result = result.concat([[76,4],[82,2]]);
}
if ("column".match(word)) {
result = result.concat([[41,2],[51,10],[52,12],[55,2],[100,1],[104,8],[105,2],[106,9],[107,2],[109,2],[110,8],[113,1]]);
}
if ("columns".match(word)) {
result = result.concat([[10,1],[42,2],[67,2],[68,2],[113,1],[116,2]]);
}
if ("column          :".match(word)) {
result = result.concat([[105,2],[106,2],[107,2]]);
}
if ("com".match(word)) {
result = result.concat([[16,2],[110,1]]);
}
if ("combination".match(word)) {
result = result.concat([[13,2]]);
}
if ("combined".match(word)) {
result = result.concat([[55,2]]);
}
if ("combines".match(word)) {
result = result.concat([[14,2],[67,2]]);
}
if ("comes".match(word)) {
result = result.concat([[6,2]]);
}
if ("comma".match(word)) {
result = result.concat([[82,2]]);
}
if ("command".match(word)) {
result = result.concat([[9,3],[14,31],[18,6],[23,8],[25,1],[30,5],[31,2],[49,2],[52,10],[55,19],[56,20],[57,8],[60,2],[61,12],[64,22],[66,10],[67,2],[68,2],[92,2],[105,4],[106,4],[107,4],[110,28],[111,7],[114,8],[117,8],[120,7],[123,6],[126,5],[129,5]]);
}
if ("commandcapabilities".match(word)) {
result = result.concat([[23,4],[31,20]]);
}
if ("commands".match(word)) {
result = result.concat([[3,1],[9,6],[11,1],[14,17],[18,2],[23,4],[31,2],[60,2],[64,4],[66,6],[67,6],[68,2],[92,2],[110,18],[111,4],[113,4],[114,5],[117,5],[120,5],[123,5],[126,4],[129,4]]);
}
if ("commandtypes".match(word)) {
result = result.concat([[31,6]]);
}
if ("commas".match(word)) {
result = result.concat([[110,2]]);
}
if ("commit".match(word)) {
result = result.concat([[3,1],[64,7]]);
}
if ("commitlongtransaction".match(word)) {
result = result.concat([[64,2],[111,1]]);
}
if ("committed".match(word)) {
result = result.concat([[14,1],[64,2]]);
}
if ("common".match(word)) {
result = result.concat([[3,5],[7,2],[13,4],[81,2],[92,6]]);
}
if ("communication".match(word)) {
result = result.concat([[10,1],[125,2]]);
}
if ("company/foundation/originator".match(word)) {
result = result.concat([[22,5]]);
}
if ("compare".match(word)) {
result = result.concat([[73,2]]);
}
if ("compared".match(word)) {
result = result.concat([[46,2]]);
}
if ("comparisionoperations".match(word)) {
result = result.concat([[71,2]]);
}
if ("comparison".match(word)) {
result = result.concat([[61,2],[81,7],[111,1],[114,1],[117,1],[120,1],[123,1]]);
}
if ("comparisoncondition".match(word)) {
result = result.concat([[71,4]]);
}
if ("comparisonoperations".match(word)) {
result = result.concat([[71,2]]);
}
if ("comparisons".match(word)) {
result = result.concat([[60,4]]);
}
if ("compatibility".match(word)) {
result = result.concat([[81,2]]);
}
if ("complete".match(word)) {
result = result.concat([[9,2],[23,1]]);
}
if ("completed".match(word)) {
result = result.concat([[14,1],[23,2]]);
}
if ("completely".match(word)) {
result = result.concat([[89,2]]);
}
if ("complex".match(word)) {
result = result.concat([[13,8],[55,2]]);
}
if ("component".match(word)) {
result = result.concat([[10,2],[13,2],[111,2],[114,2],[120,2],[123,2],[126,2],[129,2]]);
}
if ("components".match(word)) {
result = result.concat([[86,2],[89,4],[98,5]]);
}
if ("components:".match(word)) {
result = result.concat([[85,2]]);
}
if ("composite".match(word)) {
result = result.concat([[111,1],[114,2],[117,1],[120,1],[126,1]]);
}
if ("comprise".match(word)) {
result = result.concat([[12,3],[55,1],[90,2],[91,2]]);
}
if ("comprises".match(word)) {
result = result.concat([[13,2]]);
}
if ("computable".match(word)) {
result = result.concat([[87,2]]);
}
if ("computer".match(word)) {
result = result.concat([[10,1],[128,4]]);
}
if ("concat".match(word)) {
result = result.concat([[114,1],[117,1],[120,1],[123,1]]);
}
if ("concatenation".match(word)) {
result = result.concat([[81,2]]);
}
if ("concept".match(word)) {
result = result.concat([[13,2],[41,4],[42,2]]);
}
if ("concepts".match(word)) {
result = result.concat([[2,1],[10,2],[12,9],[13,7],[14,7]]);
}
if ("conceptual".match(word)) {
result = result.concat([[9,4]]);
}
if ("concrete".match(word)) {
result = result.concat([[85,1],[89,9]]);
}
if ("concurrency".match(word)) {
result = result.concat([[119,1],[122,1]]);
}
if ("condition".match(word)) {
result = result.concat([[60,4]]);
}
if ("conditional".match(word)) {
result = result.concat([[14,4],[63,4]]);
}
if ("conditionally".match(word)) {
result = result.concat([[25,2]]);
}
if ("conditions".match(word)) {
result = result.concat([[60,8],[111,1],[114,1],[117,1],[120,1],[123,1]]);
}
if ("conditions:".match(word)) {
result = result.concat([[82,2]]);
}
if ("conditiontypes".match(word)) {
result = result.concat([[35,6]]);
}
if ("config".match(word)) {
result = result.concat([[105,2],[106,2],[107,2]]);
}
if ("configuration".match(word)) {
result = result.concat([[9,2],[22,2],[23,10],[99,2],[116,2],[117,1],[122,1],[123,1],[129,1]]);
}
if ("configure".match(word)) {
result = result.concat([[66,2]]);
}
if ("configured".match(word)) {
result = result.concat([[23,2]]);
}
if ("configuring".match(word)) {
result = result.concat([[8,2],[22,2]]);
}
if ("confirm".match(word)) {
result = result.concat([[110,2]]);
}
if ("conflictoption".match(word)) {
result = result.concat([[93,2]]);
}
if ("conflicts".match(word)) {
result = result.concat([[3,1]]);
}
if ("conform".match(word)) {
result = result.concat([[52,2]]);
}
if ("conforms".match(word)) {
result = result.concat([[22,1],[128,1]]);
}
if ("conjunction".match(word)) {
result = result.concat([[116,1]]);
}
if ("conndict".match(word)) {
result = result.concat([[23,10]]);
}
if ("connect".match(word)) {
result = result.concat([[22,6],[23,4],[25,2],[108,8],[110,4]]);
}
if ("connected".match(word)) {
result = result.concat([[13,7],[25,2],[108,2],[110,2]]);
}
if ("connecting".match(word)) {
result = result.concat([[8,2],[22,2],[23,3]]);
}
if ("connection".match(word)) {
result = result.concat([[2,1],[9,6],[10,1],[11,1],[14,2],[21,9],[22,17],[23,49],[24,2],[25,5],[26,5],[27,2],[29,2],[31,2],[33,2],[35,2],[37,2],[39,2],[56,2],[57,2],[76,2],[108,11],[111,7],[113,1],[114,7],[117,7],[120,6],[123,7],[126,7],[129,7]]);
}
if ("connection:".match(word)) {
result = result.concat([[23,2]]);
}
if ("connectioncapabilities".match(word)) {
result = result.concat([[27,22]]);
}
if ("connections".match(word)) {
result = result.concat([[9,2],[18,2]]);
}
if ("connections/capabilities".match(word)) {
result = result.concat([[9,1]]);
}
if ("connectmgr".match(word)) {
result = result.concat([[23,4]]);
}
if ("connects".match(word)) {
result = result.concat([[101,2]]);
}
if ("considered".match(word)) {
result = result.concat([[69,2]]);
}
if ("consistent".match(word)) {
result = result.concat([[6,2],[14,2]]);
}
if ("consists".match(word)) {
result = result.concat([[2,2],[9,2],[10,1],[13,2],[46,2],[55,4],[85,2],[122,1]]);
}
if ("consortium".match(word)) {
result = result.concat([[50,2],[51,2],[52,2],[128,2]]);
}
if ("constant".match(word)) {
result = result.concat([[61,2]]);
}
if ("constants".match(word)) {
result = result.concat([[77,2]]);
}
if ("constgraint".match(word)) {
result = result.concat([[52,2]]);
}
if ("constitute".match(word)) {
result = result.concat([[13,2]]);
}
if ("constraint".match(word)) {
result = result.concat([[51,4],[52,4]]);
}
if ("constraints".match(word)) {
result = result.concat([[3,1],[14,1],[69,1],[70,5],[111,3],[114,6],[117,1],[120,4],[123,1],[125,1],[128,1]]);
}
if ("construct".match(word)) {
result = result.concat([[14,4],[23,1],[66,2],[67,2]]);
}
if ("constructed".match(word)) {
result = result.concat([[2,1],[90,1]]);
}
if ("construction".match(word)) {
result = result.concat([[7,4]]);
}
if ("constructs".match(word)) {
result = result.concat([[12,5],[13,2]]);
}
if ("consult".match(word)) {
result = result.concat([[110,2],[111,14],[114,14],[117,14],[120,14],[123,14],[126,14],[129,14]]);
}
if ("consumer".match(word)) {
result = result.concat([[6,2]]);
}
if ("contain".match(word)) {
result = result.concat([[7,2],[13,6],[14,2],[31,2],[41,2],[51,2],[63,2],[108,6],[110,1],[128,1]]);
}
if ("contained".match(word)) {
result = result.concat([[46,9],[48,2],[51,4],[88,2]]);
}
if ("container".match(word)) {
result = result.concat([[13,2],[22,4],[23,2],[46,11],[92,2]]);
}
if ("containers".match(word)) {
result = result.concat([[46,2]]);
}
if ("containing".match(word)) {
result = result.concat([[13,2],[48,2],[50,2],[51,4],[52,2],[61,4],[99,2],[103,2],[116,1],[122,1]]);
}
if ("contains".match(word)) {
result = result.concat([[9,5],[13,4],[19,2],[22,4],[41,2],[49,2],[51,4],[52,2],[55,4],[56,2],[61,1],[68,2],[71,2],[73,2],[108,2],[110,4],[111,1],[116,1],[120,1]]);
}
if ("content".match(word)) {
result = result.concat([[25,1],[51,2],[89,2],[111,1],[114,1],[117,1],[120,1],[123,1],[126,1],[129,1]]);
}
if ("contents".match(word)) {
result = result.concat([[19,2],[52,2]]);
}
if ("context".match(word)) {
result = result.concat([[8,2],[13,17],[14,2],[92,21]]);
}
if ("contexts".match(word)) {
result = result.concat([[92,5],[111,1],[114,1],[123,1],[126,1]]);
}
if ("context’s".match(word)) {
result = result.concat([[92,2]]);
}
if ("contraints".match(word)) {
result = result.concat([[3,1]]);
}
if ("control".match(word)) {
result = result.concat([[14,2],[41,2],[92,3],[119,1],[122,1]]);
}
if ("converse".match(word)) {
result = result.concat([[49,2]]);
}
if ("conversion".match(word)) {
result = result.concat([[50,2]]);
}
if ("conversions".match(word)) {
result = result.concat([[88,2]]);
}
if ("convert".match(word)) {
result = result.concat([[55,3],[61,2]]);
}
if ("converted".match(word)) {
result = result.concat([[55,2],[79,1],[83,2]]);
}
if ("coordinate".match(word)) {
result = result.concat([[13,2],[87,5],[92,10],[111,1]]);
}
if ("coordinatedimensionality".match(word)) {
result = result.concat([[87,36]]);
}
if ("coordinates".match(word)) {
result = result.concat([[87,2]]);
}
if ("coords".match(word)) {
result = result.concat([[87,10]]);
}
if ("core".match(word)) {
result = result.concat([[7,2]]);
}
if ("correspondence".match(word)) {
result = result.concat([[13,2]]);
}
if ("correspondends".match(word)) {
result = result.concat([[55,2]]);
}
if ("corresponding".match(word)) {
result = result.concat([[55,2]]);
}
if ("corresponds".match(word)) {
result = result.concat([[55,4]]);
}
if ("costly".match(word)) {
result = result.concat([[89,1]]);
}
if ("count".match(word)) {
result = result.concat([[16,6],[23,10],[111,1],[114,1],[117,1],[120,1],[123,1]]);
}
if ("counted".match(word)) {
result = result.concat([[16,2]]);
}
if ("country".match(word)) {
result = result.concat([[22,1]]);
}
if ("covered".match(word)) {
result = result.concat([[86,1]]);
}
if ("coveredby".match(word)) {
result = result.concat([[71,2],[73,2],[111,1]]);
}
if ("create".match(word)) {
result = result.concat([[2,2],[8,2],[10,1],[13,2],[14,2],[16,2],[23,11],[40,2],[43,7],[51,2],[52,16],[55,31],[56,8],[57,4],[59,2],[60,6],[61,6],[67,2],[92,2],[93,2],[103,2],[105,2],[110,34],[113,2],[125,1]]);
}
if ("create/delete".match(word)) {
result = result.concat([[8,2]]);
}
if ("createcommand".match(word)) {
result = result.concat([[23,2],[52,6],[55,2],[56,2],[57,2],[61,2]]);
}
if ("createconnection".match(word)) {
result = result.concat([[23,3]]);
}
if ("created".match(word)) {
result = result.concat([[46,10],[50,2],[52,4],[55,8],[61,2],[86,2],[92,2],[93,2],[110,2]]);
}
if ("createdatastore".match(word)) {
result = result.concat([[23,4],[114,1],[120,1]]);
}
if ("createdatastorecmd".match(word)) {
result = result.concat([[23,6]]);
}
if ("createdsdict".match(word)) {
result = result.concat([[23,4]]);
}
if ("creategeometry".match(word)) {
result = result.concat([[88,1],[93,4]]);
}
if ("creategeometryfromagf".match(word)) {
result = result.concat([[61,2]]);
}
if ("createlinearring".match(word)) {
result = result.concat([[55,4]]);
}
if ("createlinestring".match(word)) {
result = result.concat([[20,2]]);
}
if ("createlongtransaction".match(word)) {
result = result.concat([[64,4],[111,1]]);
}
if ("createpolygon".match(word)) {
result = result.concat([[55,2]]);
}
if ("creates".match(word)) {
result = result.concat([[52,2],[61,2],[64,2],[92,1],[93,2],[110,2]]);
}
if ("createsdffile".match(word)) {
result = result.concat([[120,1],[123,1]]);
}
if ("createspatialcontext".match(word)) {
result = result.concat([[111,1],[114,1],[120,1],[123,1]]);
}
if ("createspatialindex".match(word)) {
result = result.concat([[114,1]]);
}
if ("creating".match(word)) {
result = result.concat([[6,2],[10,2],[11,1],[43,6],[45,1],[46,2],[51,5],[52,12],[55,4],[60,5],[61,4],[93,7],[110,5]]);
}
if ("creation".match(word)) {
result = result.concat([[7,2],[9,1],[13,4],[43,2],[51,2],[104,1],[105,5],[110,6]]);
}
if ("criteria".match(word)) {
result = result.concat([[114,1],[117,1]]);
}
if ("cross".match(word)) {
result = result.concat([[41,7]]);
}
if ("crosses".match(word)) {
result = result.concat([[71,2],[73,2],[111,1]]);
}
if ("ct".match(word)) {
result = result.concat([[87,2]]);
}
if ("curre".match(word)) {
result = result.concat([[18,8]]);
}
if ("current".match(word)) {
result = result.concat([[64,4]]);
}
if ("currently".match(word)) {
result = result.concat([[13,2],[48,1],[64,4],[80,1]]);
}
if ("curve".match(word)) {
result = result.concat([[13,9],[91,2]]);
}
if ("curved".match(word)) {
result = result.concat([[103,7]]);
}
if ("curveelementtype".match(word)) {
result = result.concat([[87,8]]);
}
if ("curveelmenttype".match(word)) {
result = result.concat([[87,2]]);
}
if ("curvepolygon".match(word)) {
result = result.concat([[13,1],[83,12],[87,6],[90,2],[120,1],[126,1]]);
}
if ("curvepolygon:".match(word)) {
result = result.concat([[61,2]]);
}
if ("curvepolygoncollection".match(word)) {
result = result.concat([[83,6]]);
}
if ("curvepolygons".match(word)) {
result = result.concat([[13,1]]);
}
if ("curves".match(word)) {
result = result.concat([[13,2],[102,1]]);
}
if ("curvesegment".match(word)) {
result = result.concat([[83,6]]);
}
if ("curvesegmentcollection".match(word)) {
result = result.concat([[83,6]]);
}
if ("curvestring".match(word)) {
result = result.concat([[13,1],[83,18],[87,6],[90,2],[120,1],[126,1]]);
}
if ("curvestring:".match(word)) {
result = result.concat([[61,2]]);
}
if ("curvestringcollection".match(word)) {
result = result.concat([[83,8]]);
}
if ("curvestringelement".match(word)) {
result = result.concat([[87,6]]);
}
if ("curvestrings".match(word)) {
result = result.concat([[13,1],[87,2]]);
}
if ("custom".match(word)) {
result = result.concat([[10,1],[11,3],[113,2]]);
}
if ("customer".match(word)) {
result = result.concat([[50,4],[51,6]]);
}
if ("customer/".match(word)) {
result = result.concat([[52,2]]);
}
if ("customer/ogc980461fs”".match(word)) {
result = result.concat([[51,12]]);
}
if ("customer/samplefeatureschema".match(word)) {
result = result.concat([[52,2]]);
}
if ("customer”".match(word)) {
result = result.concat([[51,2]]);
}
if ("customization".match(word)) {
result = result.concat([[0,2]]);
}
if ("customize".match(word)) {
result = result.concat([[42,2]]);
}
if ("d".match(word)) {
result = result.concat([[105,2],[110,24]]);
}
if ("d18111".match(word)) {
result = result.concat([[107,2]]);
}
if ("data".match(word)) {
result = result.concat([[0,2],[2,3],[3,1],[6,14],[7,16],[8,4],[9,11],[10,34],[11,1],[12,3],[13,61],[14,43],[22,35],[23,26],[41,8],[42,4],[45,2],[46,26],[50,4],[51,8],[52,14],[53,7],[54,7],[55,36],[56,2],[58,2],[59,2],[60,4],[61,4],[63,6],[64,4],[65,2],[66,4],[69,1],[74,7],[85,2],[86,3],[87,4],[89,1],[92,2],[96,16],[99,2],[100,1],[108,38],[109,11],[110,9],[111,8],[113,13],[114,11],[116,18],[117,11],[119,15],[120,11],[122,20],[123,6],[125,18],[126,10],[128,19],[129,4]]);
}
if ("database".match(word)) {
result = result.concat([[7,2],[10,4],[13,4],[14,2],[22,5],[46,2],[55,2],[63,2],[64,2],[92,6],[96,4],[108,2],[110,2],[116,4],[119,2]]);
}
if ("databases".match(word)) {
result = result.concat([[22,2]]);
}
if ("dataproperty".match(word)) {
result = result.concat([[50,8],[51,2]]);
}
if ("dataproperty:".match(word)) {
result = result.concat([[61,2]]);
}
if ("datapropertydef".match(word)) {
result = result.concat([[61,4]]);
}
if ("datastore".match(word)) {
result = result.concat([[23,12],[52,2],[61,2],[111,1],[114,2],[117,1],[120,1],[123,1],[126,1],[129,1]]);
}
if ("datatype".match(word)) {
result = result.concat([[50,12],[51,3],[61,4],[109,1]]);
}
if ("datatypes".match(word)) {
result = result.concat([[29,6]]);
}
if ("datavalue".match(word)) {
result = result.concat([[72,4]]);
}
if ("date".match(word)) {
result = result.concat([[73,2],[80,6],[109,4]]);
}
if ("date/time".match(word)) {
result = result.concat([[13,2]]);
}
if ("datetime".match(word)) {
result = result.concat([[50,2],[55,1],[72,2],[80,5],[109,2],[111,3],[114,5],[117,4],[120,3],[123,2],[126,2]]);
}
if ("date      :".match(word)) {
result = result.concat([[105,2],[106,2],[107,2]]);
}
if ("dbase".match(word)) {
result = result.concat([[122,1]]);
}
if ("dbf".match(word)) {
result = result.concat([[10,1],[122,3]]);
}
if ("dbmslimit".match(word)) {
result = result.concat([[109,2]]);
}
if ("dd".match(word)) {
result = result.concat([[80,4]]);
}
if ("deactivated".match(word)) {
result = result.concat([[64,2]]);
}
if ("deactivatelongtransaction".match(word)) {
result = result.concat([[64,2],[111,1]]);
}
if ("deactivates".match(word)) {
result = result.concat([[64,2]]);
}
if ("decimal".match(word)) {
result = result.concat([[13,2],[50,6],[55,1],[78,2],[79,2],[83,2],[109,2],[111,1],[114,13],[117,12],[120,11],[123,11],[126,4]]);
}
if ("declarations".match(word)) {
result = result.concat([[27,7],[29,7],[31,7],[33,7],[35,7],[37,7],[39,7]]);
}
if ("declare".match(word)) {
result = result.concat([[61,2],[66,2]]);
}
if ("declared".match(word)) {
result = result.concat([[19,2],[25,2]]);
}
if ("def".match(word)) {
result = result.concat([[75,2]]);
}
if ("default".match(word)) {
result = result.concat([[13,14],[19,12],[42,1],[49,2],[50,2],[51,4],[83,2],[88,2],[89,2],[92,7],[93,2],[99,2],[114,1],[117,1]]);
}
if ("defaults".match(word)) {
result = result.concat([[13,2],[105,2],[106,2],[107,2]]);
}
if ("defaultval".match(word)) {
result = result.concat([[23,2]]);
}
if ("defaultvalue".match(word)) {
result = result.concat([[50,4],[51,6]]);
}
if ("deferred".match(word)) {
result = result.concat([[54,1]]);
}
if ("define".match(word)) {
result = result.concat([[4,2],[9,3],[10,1],[13,2],[14,4],[16,6],[23,2],[41,2],[45,2],[55,4],[82,2],[87,2],[110,5]]);
}
if ("defined".match(word)) {
result = result.concat([[13,31],[14,4],[16,2],[41,8],[43,1],[46,11],[50,2],[55,6],[64,2],[76,2],[81,2],[86,1],[110,4],[116,1],[128,1]]);
}
if ("defines".match(word)) {
result = result.concat([[12,2],[13,6],[16,2],[41,2],[44,2],[46,10],[51,5],[62,2],[64,12],[86,1],[87,4],[91,1],[110,3],[128,2]]);
}
if ("defining".match(word)) {
result = result.concat([[46,2],[50,4],[55,2],[82,2],[86,2],[116,1]]);
}
if ("definition".match(word)) {
result = result.concat([[10,1],[13,16],[16,4],[41,2],[43,1],[44,2],[46,2],[51,28],[52,28],[55,4],[56,2],[83,2],[87,5],[109,1],[110,2],[116,4],[122,1],[128,1]]);
}
if ("definitions".match(word)) {
result = result.concat([[12,4],[13,6],[22,2],[51,7],[104,1],[110,2]]);
}
if ("degrees".match(word)) {
result = result.concat([[92,2]]);
}
if ("delete".match(word)) {
result = result.concat([[13,2],[14,6],[16,2],[48,1],[57,6],[66,4],[105,2],[106,2],[107,2],[111,1],[114,1],[117,1],[120,1],[123,1],[125,1]]);
}
if ("deleted".match(word)) {
result = result.concat([[48,4],[49,4],[57,2],[64,2]]);
}
if ("deletes".match(word)) {
result = result.concat([[92,1]]);
}
if ("deleting".match(word)) {
result = result.concat([[10,3],[54,1],[57,10],[110,2]]);
}
if ("deletion".match(word)) {
result = result.concat([[47,1],[57,2]]);
}
if ("delimited".match(word)) {
result = result.concat([[110,2]]);
}
if ("demonstrates".match(word)) {
result = result.concat([[52,6]]);
}
if ("denoted".match(word)) {
result = result.concat([[13,2]]);
}
if ("depend".match(word)) {
result = result.concat([[92,2]]);
}
if ("dependencies".match(word)) {
result = result.concat([[99,5]]);
}
if ("dependency".match(word)) {
result = result.concat([[14,2],[63,2]]);
}
if ("dependent".match(word)) {
result = result.concat([[41,2],[99,2],[105,3],[106,2],[107,4]]);
}
if ("depending".match(word)) {
result = result.concat([[14,2],[25,2],[61,1],[63,2],[89,2],[113,2]]);
}
if ("deregister".match(word)) {
result = result.concat([[22,4]]);
}
if ("derivation".match(word)) {
result = result.concat([[13,2]]);
}
if ("derive".match(word)) {
result = result.concat([[18,2],[41,2],[89,2]]);
}
if ("derived".match(word)) {
result = result.concat([[18,2],[23,1],[61,4]]);
}
if ("descendents".match(word)) {
result = result.concat([[63,4],[64,6]]);
}
if ("descending".match(word)) {
result = result.concat([[45,2]]);
}
if ("describe".match(word)) {
result = result.concat([[13,2],[20,2],[25,2],[41,2],[105,4],[106,4],[107,4]]);
}
if ("described".match(word)) {
result = result.concat([[13,4],[55,1],[71,2],[74,2],[87,2],[110,2]]);
}
if ("describes".match(word)) {
result = result.concat([[2,5],[13,6],[40,2],[59,2],[84,2],[110,4]]);
}
if ("describeschema".match(word)) {
result = result.concat([[52,6],[111,1],[114,1],[117,1],[120,1],[123,1],[126,1],[129,1]]);
}
if ("describeschemamapping".match(word)) {
result = result.concat([[114,1],[117,1],[123,1],[129,1]]);
}
if ("describing".match(word)) {
result = result.concat([[10,1],[43,1],[46,5],[52,7]]);
}
if ("description".match(word)) {
result = result.concat([[13,2],[22,3],[41,1],[46,4],[50,10],[51,17],[52,20],[64,2],[110,4],[111,15],[114,15],[117,15],[120,15],[123,15],[126,15],[129,15]]);
}
if ("description”".match(word)) {
result = result.concat([[43,1]]);
}
if ("deserialization".match(word)) {
result = result.concat([[7,2]]);
}
if ("designed".match(word)) {
result = result.concat([[13,2],[113,2]]);
}
if ("desired".match(word)) {
result = result.concat([[50,2]]);
}
if ("desktop".match(word)) {
result = result.concat([[96,2],[99,6]]);
}
if ("destroy".match(word)) {
result = result.concat([[16,2],[52,2]]);
}
if ("destroydatastore".match(word)) {
result = result.concat([[114,1],[120,1]]);
}
if ("destroyed".match(word)) {
result = result.concat([[16,4]]);
}
if ("destroying".match(word)) {
result = result.concat([[52,7]]);
}
if ("destroyschema".match(word)) {
result = result.concat([[52,4],[114,1],[123,1]]);
}
if ("destroyspatialcontext".match(word)) {
result = result.concat([[111,1],[114,1]]);
}
if ("destroyspatialindex".match(word)) {
result = result.concat([[114,1]]);
}
if ("destruction".match(word)) {
result = result.concat([[110,2]]);
}
if ("destructors".match(word)) {
result = result.concat([[16,2]]);
}
if ("detached".match(word)) {
result = result.concat([[48,3]]);
}
if ("detail".match(word)) {
result = result.concat([[9,2],[11,2]]);
}
if ("detailed".match(word)) {
result = result.concat([[0,1],[1,2],[8,2],[10,2],[11,2]]);
}
if ("details".match(word)) {
result = result.concat([[8,2],[9,2]]);
}
if ("detects".match(word)) {
result = result.concat([[14,2]]);
}
if ("determine".match(word)) {
result = result.concat([[2,1],[3,1],[9,1],[23,3],[24,2],[25,2],[46,2],[61,2],[76,2]]);
}
if ("determined".match(word)) {
result = result.concat([[14,2]]);
}
if ("develop".match(word)) {
result = result.concat([[2,1],[11,2]]);
}
if ("developer".match(word)) {
result = result.concat([[23,4],[110,2]]);
}
if ("developers".match(word)) {
result = result.concat([[1,2],[67,2],[68,2]]);
}
if ("developer’s".match(word)) {
result = result.concat([[0,2]]);
}
if ("developing".match(word)) {
result = result.concat([[11,5]]);
}
if ("development".match(word)) {
result = result.concat([[0,2],[2,8],[4,2],[11,2],[15,5],[95,2],[112,2],[115,2],[118,2],[121,2],[124,2],[127,2]]);
}
if ("device".match(word)) {
result = result.concat([[13,2]]);
}
if ("diagram".match(word)) {
result = result.concat([[9,2],[46,1]]);
}
if ("diagrams".match(word)) {
result = result.concat([[13,2],[92,2]]);
}
if ("dialog".match(word)) {
result = result.concat([[23,4],[110,1]]);
}
if ("dict".match(word)) {
result = result.concat([[23,20]]);
}
if ("dictionary".match(word)) {
result = result.concat([[23,15]]);
}
if ("dictionary’s".match(word)) {
result = result.concat([[23,3]]);
}
if ("differ".match(word)) {
result = result.concat([[22,2],[86,2]]);
}
if ("difference".match(word)) {
result = result.concat([[68,2],[110,2]]);
}
if ("differences".match(word)) {
result = result.concat([[43,2]]);
}
if ("different".match(word)) {
result = result.concat([[9,2],[13,6],[41,4],[42,4],[64,4],[85,2],[86,2],[87,4],[113,2],[119,1]]);
}
if ("differently".match(word)) {
result = result.concat([[13,2]]);
}
if ("differing".match(word)) {
result = result.concat([[13,2]]);
}
if ("digital".match(word)) {
result = result.concat([[128,2]]);
}
if ("digits".match(word)) {
result = result.concat([[114,1],[117,1],[120,1],[123,1],[126,1]]);
}
if ("dimension".match(word)) {
result = result.concat([[13,10],[86,1]]);
}
if ("dimensional".match(word)) {
result = result.concat([[13,2]]);
}
if ("dimensionalities".match(word)) {
result = result.concat([[13,2],[37,8]]);
}
if ("dimensionality".match(word)) {
result = result.concat([[13,15],[61,8],[83,25],[88,2],[91,3],[93,9],[111,2],[114,2],[117,2],[120,2],[123,2],[126,2],[129,2]]);
}
if ("dimensionality:".match(word)) {
result = result.concat([[13,2]]);
}
if ("dimensions".match(word)) {
result = result.concat([[13,10]]);
}
if ("dimensions:".match(word)) {
result = result.concat([[13,2]]);
}
if ("direct".match(word)) {
result = result.concat([[46,4]]);
}
if ("directly".match(word)) {
result = result.concat([[13,2],[46,4],[48,2],[49,2]]);
}
if ("directory".match(word)) {
result = result.concat([[19,2]]);
}
if ("disable".match(word)) {
result = result.concat([[104,1],[107,5]]);
}
if ("disassociates".match(word)) {
result = result.concat([[47,1]]);
}
if ("discovers".match(word)) {
result = result.concat([[23,2]]);
}
if ("discussed".match(word)) {
result = result.concat([[10,4],[56,2],[87,2]]);
}
if ("discusses".match(word)) {
result = result.concat([[2,12],[65,2],[95,2],[101,2],[112,2],[115,2],[118,2],[121,2],[124,2],[127,2]]);
}
if ("discussion".match(word)) {
result = result.concat([[13,2],[54,1]]);
}
if ("disjoint".match(word)) {
result = result.concat([[13,6],[71,2],[73,2],[111,1],[120,1]]);
}
if ("disk".match(word)) {
result = result.concat([[14,1],[113,2]]);
}
if ("display".match(word)) {
result = result.concat([[22,5],[23,7],[128,2]]);
}
if ("displayname".match(word)) {
result = result.concat([[22,1],[23,4]]);
}
if ("dispose".match(word)) {
result = result.concat([[16,2]]);
}
if ("distance".match(word)) {
result = result.concat([[71,4],[73,2],[92,4],[111,2],[114,1]]);
}
if ("distancecondition".match(word)) {
result = result.concat([[71,4]]);
}
if ("distanceoperations".match(word)) {
result = result.concat([[35,6],[71,4]]);
}
if ("distances".match(word)) {
result = result.concat([[92,2]]);
}
if ("distinct".match(word)) {
result = result.concat([[111,1],[114,1],[117,1],[120,1],[123,1]]);
}
if ("distinction".match(word)) {
result = result.concat([[55,2]]);
}
if ("distinguish".match(word)) {
result = result.concat([[44,2]]);
}
if ("divide".match(word)) {
result = result.concat([[72,2],[81,4]]);
}
if ("divided".match(word)) {
result = result.concat([[110,6]]);
}
if ("dll".match(word)) {
result = result.concat([[19,2],[22,1],[99,6]]);
}
if ("dlls".match(word)) {
result = result.concat([[19,6],[98,2],[99,6]]);
}
if ("document".match(word)) {
result = result.concat([[10,2],[22,2],[42,2],[51,4],[52,4],[54,1],[86,2],[110,6]]);
}
if ("documentation".match(word)) {
result = result.concat([[10,1],[99,2],[111,14],[114,14],[117,14],[120,14],[123,14],[125,1],[126,14],[129,14]]);
}
if ("done".match(word)) {
result = result.concat([[16,2],[18,2],[23,2],[43,2],[60,2],[88,2]]);
}
if ("dot".match(word)) {
result = result.concat([[82,2]]);
}
if ("double".match(word)) {
result = result.concat([[13,2],[50,2],[55,5],[71,2],[72,2],[75,6],[77,2],[79,5],[83,24],[87,18],[109,4],[111,11],[114,12],[117,11],[120,14],[123,11],[126,2],[129,8]]);
}
if ("down".match(word)) {
result = result.concat([[11,2]]);
}
if ("drawing".match(word)) {
result = result.concat([[13,2]]);
}
if ("drawings".match(word)) {
result = result.concat([[13,4],[92,4]]);
}
if ("due".match(word)) {
result = result.concat([[3,2]]);
}
if ("duplicate".match(word)) {
result = result.concat([[46,2]]);
}
if ("durable".match(word)) {
result = result.concat([[14,1]]);
}
if ("duration".match(word)) {
result = result.concat([[14,2]]);
}
if ("during".match(word)) {
result = result.concat([[22,4],[23,10],[83,1]]);
}
if ("dynamic".match(word)) {
result = result.concat([[9,1],[13,2],[23,2],[61,2],[86,1],[93,4],[120,1]]);
}
if ("dynamically".match(word)) {
result = result.concat([[10,1],[98,2],[99,2],[128,2]]);
}
if ("dynamics".match(word)) {
result = result.concat([[12,4]]);
}
if ("e".match(word)) {
result = result.concat([[52,2],[79,2],[110,55]]);
}
if ("ease".match(word)) {
result = result.concat([[67,2],[68,2]]);
}
if ("editing".match(word)) {
result = result.concat([[51,5]]);
}
if ("editor".match(word)) {
result = result.concat([[91,2]]);
}
if ("efficient".match(word)) {
result = result.concat([[86,2]]);
}
if ("effort".match(word)) {
result = result.concat([[6,2]]);
}
if ("either".match(word)) {
result = result.concat([[13,8],[14,9],[23,8],[41,2],[46,2],[47,2],[51,2],[66,2],[68,2],[89,2],[91,1],[99,4],[104,2]]);
}
if ("electrical".match(word)) {
result = result.concat([[13,2]]);
}
if ("element".match(word)) {
result = result.concat([[13,6],[41,4],[46,1],[47,4],[48,21],[50,14],[51,12],[111,6],[114,6],[117,6],[120,6],[123,6],[126,6],[129,5]]);
}
if ("elements".match(word)) {
result = result.concat([[13,9],[47,4],[48,7],[49,4],[50,4],[51,2],[87,4],[128,2]]);
}
if ("eliminated".match(word)) {
result = result.concat([[18,2]]);
}
if ("ellipses".match(word)) {
result = result.concat([[102,1]]);
}
if ("employes".match(word)) {
result = result.concat([[89,1]]);
}
if ("enable".match(word)) {
result = result.concat([[9,1],[19,2],[50,2],[104,2],[106,8],[107,7]]);
}
if ("enabled".match(word)) {
result = result.concat([[3,2],[104,2]]);
}
if ("enable”".match(word)) {
result = result.concat([[107,1]]);
}
if ("enabling".match(word)) {
result = result.concat([[6,2]]);
}
if ("encapsulated".match(word)) {
result = result.concat([[85,1]]);
}
if ("enclosed".match(word)) {
result = result.concat([[75,2],[77,2]]);
}
if ("encoded".match(word)) {
result = result.concat([[10,3],[125,6]]);
}
if ("encoding".match(word)) {
result = result.concat([[52,2],[86,1],[87,4]]);
}
if ("encounters".match(word)) {
result = result.concat([[103,2]]);
}
if ("encroached".match(word)) {
result = result.concat([[68,4]]);
}
if ("endian".match(word)) {
result = result.concat([[86,1],[87,4]]);
}
if ("ends".match(word)) {
result = result.concat([[19,2]]);
}
if ("endure".match(word)) {
result = result.concat([[14,2]]);
}
if ("engine".match(word)) {
result = result.concat([[96,2],[113,6]]);
}
if ("engineer".match(word)) {
result = result.concat([[7,5]]);
}
if ("engines".match(word)) {
result = result.concat([[10,1],[113,2]]);
}
if ("enhancements".match(word)) {
result = result.concat([[3,3]]);
}
if ("ensure".match(word)) {
result = result.concat([[46,2],[99,2],[110,1]]);
}
if ("enter".match(word)) {
result = result.concat([[76,2],[83,2],[110,1]]);
}
if ("entering".match(word)) {
result = result.concat([[71,2]]);
}
if ("entire".match(word)) {
result = result.concat([[46,2],[110,1]]);
}
if ("entirely".match(word)) {
result = result.concat([[41,2]]);
}
if ("entity".match(word)) {
result = result.concat([[110,2]]);
}
if ("entry".match(word)) {
result = result.concat([[16,2],[108,6]]);
}
if ("entry:".match(word)) {
result = result.concat([[108,2]]);
}
if ("enum".match(word)) {
result = result.concat([[27,3],[29,2],[31,2],[46,2],[87,6],[90,2]]);
}
if ("enumcount".match(word)) {
result = result.concat([[23,6]]);
}
if ("enumerable".match(word)) {
result = result.concat([[23,3]]);
}
if ("enumerated".match(word)) {
result = result.concat([[27,6],[29,4],[33,2],[60,2]]);
}
if ("enumeratepropertyvalues".match(word)) {
result = result.concat([[23,6]]);
}
if ("enumeration".match(word)) {
result = result.concat([[13,6],[61,2],[91,5]]);
}
if ("enumerations".match(word)) {
result = result.concat([[64,2]]);
}
if ("enumname".match(word)) {
result = result.concat([[23,2]]);
}
if ("enumnames".match(word)) {
result = result.concat([[23,4]]);
}
if ("envelopeintersects".match(word)) {
result = result.concat([[111,1],[114,1],[117,1],[120,1],[123,1]]);
}
if ("environment".match(word)) {
result = result.concat([[10,1],[11,1],[18,2],[19,4],[99,4],[116,2],[122,1],[125,2],[128,2]]);
}
if ("environmental".match(word)) {
result = result.concat([[122,1]]);
}
if ("envrionment".match(word)) {
result = result.concat([[19,2]]);
}
if ("eq".match(word)) {
result = result.concat([[71,2],[81,4]]);
}
if ("equals".match(word)) {
result = result.concat([[16,1],[61,2],[71,2],[73,2],[111,1]]);
}
if ("equalto".match(word)) {
result = result.concat([[61,2],[71,2],[81,2]]);
}
if ("equivalent".match(word)) {
result = result.concat([[22,1],[50,2],[83,2]]);
}
if ("error".match(word)) {
result = result.concat([[14,2],[83,1]]);
}
if ("error:".match(word)) {
result = result.concat([[18,2]]);
}
if ("errorlevel".match(word)) {
result = result.concat([[93,2]]);
}
if ("errors".match(word)) {
result = result.concat([[110,1]]);
}
if ("esri".match(word)) {
result = result.concat([[10,2],[99,2],[122,1]]);
}
if ("esri®".match(word)) {
result = result.concat([[2,1],[96,2]]);
}
if ("essential".match(word)) {
result = result.concat([[0,1],[8,2],[9,2],[12,2],[20,2],[22,6],[23,1],[42,2],[113,2],[116,2],[119,2],[122,2],[125,2],[128,2]]);
}
if ("ession".match(word)) {
result = result.concat([[92,1]]);
}
if ("establish".match(word)) {
result = result.concat([[2,1],[9,1],[21,2]]);
}
if ("establishes".match(word)) {
result = result.concat([[87,2]]);
}
if ("establishing".match(word)) {
result = result.concat([[11,1],[21,5],[23,7],[108,2]]);
}
if ("evaluated".match(word)) {
result = result.concat([[66,2]]);
}
if ("evaluates".match(word)) {
result = result.concat([[66,2]]);
}
if ("even".match(word)) {
result = result.concat([[46,1],[86,1]]);
}
if ("event".match(word)) {
result = result.concat([[19,2]]);
}
if ("every".match(word)) {
result = result.concat([[86,1]]);
}
if ("ex".match(word)) {
result = result.concat([[18,8]]);
}
if ("exactly".match(word)) {
result = result.concat([[50,2]]);
}
if ("examination".match(word)) {
result = result.concat([[23,2]]);
}
if ("examining".match(word)) {
result = result.concat([[6,2]]);
}
if ("example".match(word)) {
result = result.concat([[6,2],[7,4],[9,1],[13,44],[14,16],[16,4],[18,4],[19,6],[20,2],[21,2],[22,7],[23,7],[25,2],[40,2],[41,8],[42,2],[43,2],[46,20],[48,2],[49,2],[55,4],[56,4],[61,7],[66,4],[67,4],[68,2],[75,2],[76,2],[77,2],[83,1],[87,4],[91,1],[92,4],[94,2],[99,4],[110,2],[113,2]]);
}
if ("example:".match(word)) {
result = result.concat([[18,2],[20,2],[52,20],[55,5],[56,5],[57,5],[80,2]]);
}
if ("examples".match(word)) {
result = result.concat([[1,2],[9,3],[15,2],[19,2],[52,5],[83,2]]);
}
if ("examples:".match(word)) {
result = result.concat([[58,2],[79,2]]);
}
if ("excel".match(word)) {
result = result.concat([[116,1]]);
}
if ("except".match(word)) {
result = result.concat([[48,1]]);
}
if ("exception".match(word)) {
result = result.concat([[10,1],[18,17],[19,17],[23,1],[64,2],[67,2],[68,2],[89,4],[99,2],[100,1]]);
}
if ("exception:".match(word)) {
result = result.concat([[18,2]]);
}
if ("exceptions".match(word)) {
result = result.concat([[10,1],[18,4],[113,1]]);
}
if ("exchange".match(word)) {
result = result.concat([[86,2],[96,2]]);
}
if ("exchanged".match(word)) {
result = result.concat([[89,2]]);
}
if ("exclude".match(word)) {
result = result.concat([[66,2]]);
}
if ("exclusion".match(word)) {
result = result.concat([[14,2]]);
}
if ("exclusive".match(word)) {
result = result.concat([[111,1],[114,1],[120,1]]);
}
if ("execute".match(word)) {
result = result.concat([[14,4],[23,4],[25,2],[43,1],[52,10],[55,3],[56,6],[57,4],[61,4],[64,6],[110,2],[113,1]]);
}
if ("executed".match(word)) {
result = result.concat([[48,2],[55,2],[60,2],[64,2],[110,1]]);
}
if ("executes".match(word)) {
result = result.concat([[9,1],[14,2],[61,1],[66,2]]);
}
if ("executing".match(word)) {
result = result.concat([[52,2],[89,1],[110,2]]);
}
if ("exercise".match(word)) {
result = result.concat([[110,2]]);
}
if ("exercised".match(word)) {
result = result.concat([[25,2]]);
}
if ("exist".match(word)) {
result = result.concat([[46,2]]);
}
if ("existing".match(word)) {
result = result.concat([[10,2],[14,2],[46,2],[68,4],[110,4],[116,1],[122,1]]);
}
if ("exists".match(word)) {
result = result.concat([[23,1],[44,2],[50,2]]);
}
if ("expect".match(word)) {
result = result.concat([[90,1]]);
}
if ("explained".match(word)) {
result = result.concat([[9,2]]);
}
if ("explains".match(word)) {
result = result.concat([[0,2],[1,2],[4,2],[15,2],[21,2],[24,2],[40,2],[62,2],[84,2]]);
}
if ("explanation".match(word)) {
result = result.concat([[111,14],[114,14],[117,14],[120,14],[123,14],[126,14],[129,14]]);
}
if ("explicitly".match(word)) {
result = result.concat([[93,2]]);
}
if ("explored".match(word)) {
result = result.concat([[11,2]]);
}
if ("exponent".match(word)) {
result = result.concat([[79,2]]);
}
if ("export".match(word)) {
result = result.concat([[19,2]]);
}
if ("expose".match(word)) {
result = result.concat([[9,1]]);
}
if ("exposed".match(word)) {
result = result.concat([[6,2],[128,2]]);
}
if ("expressed".match(word)) {
result = result.concat([[51,2],[52,2]]);
}
if ("expression".match(word)) {
result = result.concat([[7,2],[9,2],[14,11],[25,1],[32,5],[33,4],[65,5],[66,2],[67,8],[68,5],[69,4],[70,5],[71,8],[72,34],[73,5],[111,5],[114,5],[117,5],[120,5],[123,5],[126,5],[129,5]]);
}
if ("expressioncapabilities".match(word)) {
result = result.concat([[33,8]]);
}
if ("expressioncollection".match(word)) {
result = result.concat([[72,6]]);
}
if ("expressioncollections".match(word)) {
result = result.concat([[82,2]]);
}
if ("expressions".match(word)) {
result = result.concat([[2,1],[9,2],[14,4],[65,4],[67,15],[68,4],[70,2],[71,2],[111,3],[114,4],[117,4],[120,4],[123,4],[126,4],[129,3]]);
}
if ("expressiontypes".match(word)) {
result = result.concat([[33,4]]);
}
if ("extend".match(word)) {
result = result.concat([[14,2]]);
}
if ("extended".match(word)) {
result = result.concat([[86,2]]);
}
if ("extendedselect".match(word)) {
result = result.concat([[120,1]]);
}
if ("extends".match(word)) {
result = result.concat([[76,2]]);
}
if ("extension".match(word)) {
result = result.concat([[51,4],[83,2]]);
}
if ("extensions".match(word)) {
result = result.concat([[83,2]]);
}
if ("extensive".match(word)) {
result = result.concat([[7,2]]);
}
if ("extent".match(word)) {
result = result.concat([[111,1],[114,1],[117,1],[120,1],[123,1],[126,1],[129,1]]);
}
if ("extents".match(word)) {
result = result.concat([[13,2],[110,3]]);
}
if ("exterior".match(word)) {
result = result.concat([[61,2]]);
}
if ("exteriorring".match(word)) {
result = result.concat([[61,8]]);
}
if ("exteriorringbluelake".match(word)) {
result = result.concat([[55,6]]);
}
if ("external".match(word)) {
result = result.concat([[55,4],[99,5]]);
}
if ("extra".match(word)) {
result = result.concat([[83,1]]);
}
if ("extracts".match(word)) {
result = result.concat([[61,1]]);
}
if ("facilitate".match(word)) {
result = result.concat([[67,2],[68,2]]);
}
if ("fact".match(word)) {
result = result.concat([[89,1]]);
}
if ("factory".match(word)) {
result = result.concat([[55,5],[85,3],[88,2],[89,2],[93,2]]);
}
if ("fails".match(word)) {
result = result.concat([[14,1]]);
}
if ("false".match(word)) {
result = result.concat([[23,14],[50,20],[51,26],[52,28],[72,2],[73,2]]);
}
if ("fast".match(word)) {
result = result.concat([[113,1]]);
}
if ("fdo".match(word)) {
result = result.concat([[0,7],[1,4],[2,16],[3,29],[4,4],[5,5],[6,8],[7,6],[8,4],[9,53],[10,12],[11,2],[12,10],[13,16],[14,20],[15,2],[16,34],[17,2],[18,12],[19,2],[20,13],[21,2],[22,20],[23,8],[24,2],[25,6],[27,2],[29,2],[31,2],[40,2],[41,22],[42,8],[43,2],[46,5],[48,1],[49,4],[50,25],[51,16],[53,2],[55,2],[58,2],[59,2],[62,2],[64,4],[65,2],[66,8],[67,8],[68,4],[76,4],[77,2],[85,2],[86,2],[90,2],[91,4],[92,8],[95,9],[96,15],[97,5],[98,6],[99,6],[100,7],[101,2],[108,7],[109,3],[110,8],[111,21],[112,9],[113,21],[114,21],[115,9],[116,25],[117,21],[118,9],[119,17],[120,21],[121,9],[122,25],[123,21],[124,9],[125,19],[126,21],[127,9],[128,28],[129,21]]);
}
if ("fdo:byte".match(word)) {
result = result.concat([[50,2]]);
}
if ("fdo:datastore".match(word)) {
result = result.concat([[51,4]]);
}
if ("fdo:geometrictypes".match(word)) {
result = result.concat([[50,2]]);
}
if ("fdo:geometryname".match(word)) {
result = result.concat([[50,2]]);
}
if ("fdo:geometryreadonly".match(word)) {
result = result.concat([[50,2],[51,2]]);
}
if ("fdo:haselevation".match(word)) {
result = result.concat([[50,2],[51,2]]);
}
if ("fdo:hasmeasure".match(word)) {
result = result.concat([[50,2],[51,2]]);
}
if ("fdo:int16".match(word)) {
result = result.concat([[50,2]]);
}
if ("fdo:int32".match(word)) {
result = result.concat([[50,2],[52,2],[56,2]]);
}
if ("fdo:int64".match(word)) {
result = result.concat([[50,2]]);
}
if ("fdo:readonly".match(word)) {
result = result.concat([[51,2]]);
}
if ("fdo:srsname".match(word)) {
result = result.concat([[50,2]]);
}
if ("fdoagfgeometryfactory".match(word)) {
result = result.concat([[55,1]]);
}
if ("fdoargumentdefinition".match(word)) {
result = result.concat([[33,2]]);
}
if ("fdoassociationpropertydefinition".match(word)) {
result = result.concat([[13,2],[43,1]]);
}
if ("fdobytearray".match(word)) {
result = result.concat([[55,2],[58,1],[61,2],[89,2]]);
}
if ("fdoclass".match(word)) {
result = result.concat([[16,2],[43,4],[45,11],[46,3],[48,2]]);
}
if ("fdoclass::create".match(word)) {
result = result.concat([[43,1]]);
}
if ("fdoclasscollection".match(word)) {
result = result.concat([[52,4]]);
}
if ("fdoclasscollection::add".match(word)) {
result = result.concat([[43,1]]);
}
if ("fdoclassdefinition".match(word)) {
result = result.concat([[13,2],[20,2],[61,2]]);
}
if ("fdoclassdefinition::getbaseproperties".match(word)) {
result = result.concat([[46,1]]);
}
if ("fdoclassdefinition::getidentityproperties".match(word)) {
result = result.concat([[43,2],[48,1]]);
}
if ("fdoclassdefinition::getproperties".match(word)) {
result = result.concat([[43,1],[46,1]]);
}
if ("fdoclassp".match(word)) {
result = result.concat([[16,2]]);
}
if ("fdoclasstype".match(word)) {
result = result.concat([[29,7],[46,6]]);
}
if ("fdocommandexception".match(word)) {
result = result.concat([[18,6]]);
}
if ("fdocommandtype".match(word)) {
result = result.concat([[23,4],[31,3],[52,6],[55,2],[56,2],[57,2],[61,2],[111,23],[114,16],[117,8],[120,11],[123,11],[126,4],[129,5]]);
}
if ("fdocomparisoncondition::create".match(word)) {
result = result.concat([[61,4]]);
}
if ("fdoconditiontype".match(word)) {
result = result.concat([[35,2]]);
}
if ("fdoconnection".match(word)) {
result = result.concat([[23,12]]);
}
if ("fdoconnectionexception".match(word)) {
result = result.concat([[18,2]]);
}
if ("fdoconnectionexception:".match(word)) {
result = result.concat([[18,2]]);
}
if ("fdoconnectionmanager".match(word)) {
result = result.concat([[23,4]]);
}
if ("fdoconnectionstate".match(word)) {
result = result.concat([[23,9]]);
}
if ("fdodataproperty".match(word)) {
result = result.concat([[43,1]]);
}
if ("fdodatapropertydefinition".match(word)) {
result = result.concat([[52,8],[61,4]]);
}
if ("fdodatapropertydefinition::create".match(word)) {
result = result.concat([[43,1],[50,2],[52,4]]);
}
if ("fdodatapropertydefinitioncollection".match(word)) {
result = result.concat([[52,2]]);
}
if ("fdodatatype".match(word)) {
result = result.concat([[29,11],[33,2],[45,1],[52,4],[61,8],[109,26]]);
}
if ("fdodatavalue".match(word)) {
result = result.concat([[55,13],[56,4],[58,1],[61,4]]);
}
if ("fdodatavalue::create".match(word)) {
result = result.concat([[55,4],[56,2],[61,2]]);
}
if ("fdodimensinality".match(word)) {
result = result.concat([[37,2]]);
}
if ("fdodimensionality".match(word)) {
result = result.concat([[13,2],[37,4],[61,10],[91,1]]);
}
if ("fdodirectpositionimpl".match(word)) {
result = result.concat([[89,1]]);
}
if ("fdodistancecondition".match(word)) {
result = result.concat([[60,2]]);
}
if ("fdodistanceoperations".match(word)) {
result = result.concat([[35,2]]);
}
if ("fdoenvelopeimpl".match(word)) {
result = result.concat([[89,1]]);
}
if ("fdoexception".match(word)) {
result = result.concat([[18,8]]);
}
if ("fdoexception::nlsgetmessage".match(word)) {
result = result.concat([[19,2]]);
}
if ("fdoexpression::parse".match(word)) {
result = result.concat([[72,2]]);
}
if ("fdoexpressiontype".match(word)) {
result = result.concat([[33,2]]);
}
if ("fdofeatureaccessmanager::getconnectionmanager".match(word)) {
result = result.concat([[23,2]]);
}
if ("fdofeatureaccessmanager::getproviderregistry".match(word)) {
result = result.concat([[23,2]]);
}
if ("fdofeatureclass".match(word)) {
result = result.concat([[16,6],[18,4],[43,4],[44,9],[45,2],[46,4],[52,4],[61,7]]);
}
if ("fdofeatureclass::create".match(word)) {
result = result.concat([[43,1],[50,2],[51,1],[52,2]]);
}
if ("fdofeatureclasses".match(word)) {
result = result.concat([[46,1]]);
}
if ("fdofeatureschema".match(word)) {
result = result.concat([[9,1],[41,4],[46,2],[48,2],[49,2],[50,4],[52,6]]);
}
if ("fdofeatureschema::acceptchanges".match(word)) {
result = result.concat([[49,2]]);
}
if ("fdofeatureschema::create".match(word)) {
result = result.concat([[43,1],[50,2],[51,2],[52,2]]);
}
if ("fdofeatureschema::getclasses".match(word)) {
result = result.concat([[43,1],[46,1]]);
}
if ("fdofeatureschema::rejectchanges".match(word)) {
result = result.concat([[49,2]]);
}
if ("fdofeatureschemacollection".match(word)) {
result = result.concat([[9,1],[46,4],[50,6],[52,2]]);
}
if ("fdofeatureschemas".match(word)) {
result = result.concat([[46,2]]);
}
if ("fdofgfgeometryfactory".match(word)) {
result = result.concat([[55,2],[58,1],[89,3],[93,2]]);
}
if ("fdofgfgeometryfactory::getinstance".match(word)) {
result = result.concat([[55,2],[93,2]]);
}
if ("fdofilter".match(word)) {
result = result.concat([[61,2]]);
}
if ("fdofilter::parse".match(word)) {
result = result.concat([[71,2]]);
}
if ("fdofunctiondefinition".match(word)) {
result = result.concat([[33,2]]);
}
if ("fdofunctiondefinitioncollection".match(word)) {
result = result.concat([[33,2]]);
}
if ("fdogeometricpropertydefinition".match(word)) {
result = result.concat([[52,4]]);
}
if ("fdogeometricpropertydefinition::create".match(word)) {
result = result.concat([[43,1],[50,4],[51,2],[52,2]]);
}
if ("fdogeometrictype".match(word)) {
result = result.concat([[13,2],[52,2]]);
}
if ("fdogeometrictypes".match(word)) {
result = result.concat([[50,4],[51,6]]);
}
if ("fdogeometry".match(word)) {
result = result.concat([[84,2]]);
}
if ("fdogeometrycomponenttype".match(word)) {
result = result.concat([[37,2]]);
}
if ("fdogeometryfactoryabstract::".match(word)) {
result = result.concat([[88,1]]);
}
if ("fdogeometryfactorytabstract".match(word)) {
result = result.concat([[89,2]]);
}
if ("fdogeometryproperty".match(word)) {
result = result.concat([[43,1]]);
}
if ("fdogeometrytype".match(word)) {
result = result.concat([[13,2],[37,2],[61,28],[90,15],[91,1]]);
}
if ("fdogeometryvalue".match(word)) {
result = result.concat([[55,8],[58,1]]);
}
if ("fdogeometryvalue::create".match(word)) {
result = result.concat([[55,2]]);
}
if ("fdoiactivatelongtransaction".match(word)) {
result = result.concat([[64,8]]);
}
if ("fdoiapplyschema".match(word)) {
result = result.concat([[47,1],[52,4],[110,2]]);
}
if ("fdoiapplyschema::execute".match(word)) {
result = result.concat([[49,2]]);
}
if ("fdoiapplyschemacommand::execute".match(word)) {
result = result.concat([[43,1]]);
}
if ("fdoiapplyschemacommand::setfeatureschema".match(word)) {
result = result.concat([[43,1]]);
}
if ("fdoicommandcapabilities".match(word)) {
result = result.concat([[23,2],[31,3],[111,4],[114,4],[117,4],[120,4],[123,4],[126,4],[129,4]]);
}
if ("fdoicommitlongtransaction".match(word)) {
result = result.concat([[64,8]]);
}
if ("fdoiconnection".match(word)) {
result = result.concat([[9,1],[23,2],[25,4],[111,14],[114,14],[117,14],[120,14],[123,14],[126,14],[129,14]]);
}
if ("fdoiconnection::getgeometrycapabilities".match(word)) {
result = result.concat([[13,2]]);
}
if ("fdoiconnectioncapabilities".match(word)) {
result = result.concat([[27,3],[111,4],[114,4],[117,4],[120,4],[123,4],[126,4],[129,4]]);
}
if ("fdoiconnectioninfo".match(word)) {
result = result.concat([[23,2]]);
}
if ("fdoiconnectionpropertydictionary".match(word)) {
result = result.concat([[23,4]]);
}
if ("fdoicreatedatastore".match(word)) {
result = result.concat([[23,6]]);
}
if ("fdoicreatelongtransaction".match(word)) {
result = result.concat([[64,8]]);
}
if ("fdoicurvepolygon".match(word)) {
result = result.concat([[90,1]]);
}
if ("fdoicurvestring".match(word)) {
result = result.concat([[90,1]]);
}
if ("fdoidatapropertydictionary".match(word)) {
result = result.concat([[23,2]]);
}
if ("fdoidatastorepropertydictionary".match(word)) {
result = result.concat([[23,4]]);
}
if ("fdoideactivatelongtransaction".match(word)) {
result = result.concat([[64,8]]);
}
if ("fdoidelete".match(word)) {
result = result.concat([[57,6],[58,1]]);
}
if ("fdoidentifier".match(word)) {
result = result.concat([[61,4]]);
}
if ("fdoidescribeschema".match(word)) {
result = result.concat([[46,2],[48,1],[49,2],[52,4],[110,2]]);
}
if ("fdoidescribeschema::execute".match(word)) {
result = result.concat([[46,2]]);
}
if ("fdoidestroyschema".match(word)) {
result = result.concat([[52,4],[110,2]]);
}
if ("fdoidirectposition".match(word)) {
result = result.concat([[61,2],[89,1]]);
}
if ("fdoidisposable".match(word)) {
result = result.concat([[16,6]]);
}
if ("fdoienvelope".match(word)) {
result = result.concat([[89,1]]);
}
if ("fdoiexpressioncapabilities".match(word)) {
result = result.concat([[33,2],[111,4],[114,4],[117,4],[120,4],[123,4],[126,4],[129,4]]);
}
if ("fdoifeaturecommand".match(word)) {
result = result.concat([[60,4]]);
}
if ("fdoifeaturereader".match(word)) {
result = result.concat([[13,2],[55,4],[60,4],[61,3]]);
}
if ("fdoifiltercapabilities".match(word)) {
result = result.concat([[35,2],[111,4],[114,4],[117,4],[120,4],[123,4],[126,4],[129,4]]);
}
if ("fdoigeometry".match(word)) {
result = result.concat([[61,2],[89,2]]);
}
if ("fdoigeometry::".match(word)) {
result = result.concat([[88,1]]);
}
if ("fdoigeometrycapabilities".match(word)) {
result = result.concat([[37,2],[111,4],[114,4],[117,4],[120,4],[123,4],[126,4],[129,4]]);
}
if ("fdoigetlongtransaction".match(word)) {
result = result.concat([[64,1]]);
}
if ("fdoigetlongtransactions".match(word)) {
result = result.concat([[64,7]]);
}
if ("fdoiinsert".match(word)) {
result = result.concat([[55,5],[58,1]]);
}
if ("fdoilinearring".match(word)) {
result = result.concat([[55,4],[58,1],[61,4]]);
}
if ("fdoilinestring".match(word)) {
result = result.concat([[90,1]]);
}
if ("fdoimulticurvepolygon".match(word)) {
result = result.concat([[90,1]]);
}
if ("fdoimulticurvestring".match(word)) {
result = result.concat([[90,1]]);
}
if ("fdoimultigeometry".match(word)) {
result = result.concat([[90,1]]);
}
if ("fdoimultilinestring".match(word)) {
result = result.concat([[90,1]]);
}
if ("fdoimultipoint".match(word)) {
result = result.concat([[90,1]]);
}
if ("fdoimultipolygon".match(word)) {
result = result.concat([[90,1]]);
}
if ("fdoin32".match(word)) {
result = result.concat([[23,2]]);
}
if ("fdoint32".match(word)) {
result = result.concat([[23,6],[27,4],[31,2],[33,6],[35,6],[37,6],[55,6],[56,2],[57,2],[61,8],[87,2]]);
}
if ("fdoipoint".match(word)) {
result = result.concat([[90,1],[93,2]]);
}
if ("fdoipolygon".match(word)) {
result = result.concat([[55,2],[58,1],[61,4],[90,1]]);
}
if ("fdoipropertydictionary".match(word)) {
result = result.concat([[23,1]]);
}
if ("fdoirastercapabilities".match(word)) {
result = result.concat([[39,2],[111,4],[114,4],[117,4],[120,4],[123,4],[126,4],[129,4]]);
}
if ("fdoirollbacklongtransaction".match(word)) {
result = result.concat([[64,8]]);
}
if ("fdoischemacapabilities".match(word)) {
result = result.concat([[29,3],[111,4],[114,4],[117,4],[120,4],[123,4],[126,4],[129,4]]);
}
if ("fdoiselect".match(word)) {
result = result.concat([[60,4],[61,4]]);
}
if ("fdoiupdate".match(word)) {
result = result.concat([[56,6],[58,1]]);
}
if ("fdolinearringcollection".match(word)) {
result = result.concat([[55,2],[58,1]]);
}
if ("fdolinearringcollection::create".match(word)) {
result = result.concat([[55,2]]);
}
if ("fdolinestring".match(word)) {
result = result.concat([[20,4]]);
}
if ("fdolocktype".match(word)) {
result = result.concat([[27,5]]);
}
if ("fdoobjectproperty".match(word)) {
result = result.concat([[43,1]]);
}
if ("fdoobjectpropertydefinition".match(word)) {
result = result.concat([[46,4]]);
}
if ("fdoobjectpropertydefinition::create".match(word)) {
result = result.concat([[43,1]]);
}
if ("fdophysicalclassmapping".match(word)) {
result = result.concat([[41,2]]);
}
if ("fdophysicalelementmapping".match(word)) {
result = result.concat([[41,2]]);
}
if ("fdophysicalpropertymapping".match(word)) {
result = result.concat([[41,2]]);
}
if ("fdophysicalschemamapping".match(word)) {
result = result.concat([[41,2]]);
}
if ("fdopoint".match(word)) {
result = result.concat([[93,2]]);
}
if ("fdopropertydefinition".match(word)) {
result = result.concat([[20,4],[61,2]]);
}
if ("fdopropertydefinitioncollection".match(word)) {
result = result.concat([[20,6],[46,2],[52,2],[61,2]]);
}
if ("fdopropertytype".match(word)) {
result = result.concat([[61,6]]);
}
if ("fdopropertyvalue".match(word)) {
result = result.concat([[55,7],[58,1]]);
}
if ("fdopropertyvaluecollection".match(word)) {
result = result.concat([[55,3],[58,1]]);
}
if ("fdoprovider".match(word)) {
result = result.concat([[23,2]]);
}
if ("fdoprovidercollection".match(word)) {
result = result.concat([[23,2]]);
}
if ("fdoproviderregistry".match(word)) {
result = result.concat([[22,2],[23,4]]);
}
if ("fdoptr".match(word)) {
result = result.concat([[16,27],[17,2],[18,6],[20,23],[23,18],[52,26],[55,28],[56,4],[57,2],[61,16]]);
}
if ("fdorasterdatamodel".match(word)) {
result = result.concat([[39,2]]);
}
if ("fdorasterdatamodel::create".match(word)) {
result = result.concat([[39,2]]);
}
if ("fdorasterdatamodeltype".match(word)) {
result = result.concat([[39,2]]);
}
if ("fdorasterdataorganization".match(word)) {
result = result.concat([[39,2]]);
}
if ("fdordbms".match(word)) {
result = result.concat([[19,2]]);
}
if ("fdordbmscommandtype".match(word)) {
result = result.concat([[31,3],[114,3],[126,1]]);
}
if ("fdoreadonlyargumentdefinitioncollection".match(word)) {
result = result.concat([[33,2]]);
}
if ("fdos".match(word)) {
result = result.concat([[7,2]]);
}
if ("fdoschemacollection::remove".match(word)) {
result = result.concat([[47,1]]);
}
if ("fdoschemacollection::removeat".match(word)) {
result = result.concat([[47,1]]);
}
if ("fdoschemaelement".match(word)) {
result = result.concat([[41,2]]);
}
if ("fdoschemaelement::delete".match(word)) {
result = result.concat([[47,1]]);
}
if ("fdoschemaelement::getelementstate".match(word)) {
result = result.concat([[48,2]]);
}
if ("fdoschemaelementnametype".match(word)) {
result = result.concat([[111,5],[114,5],[117,5],[120,5],[123,5],[126,5],[129,5]]);
}
if ("fdoschemaexception".match(word)) {
result = result.concat([[18,4],[19,2]]);
}
if ("fdoschemaexception::create".match(word)) {
result = result.concat([[19,4]]);
}
if ("fdosm".match(word)) {
result = result.concat([[19,2]]);
}
if ("fdosmerror::nlsgetmessage".match(word)) {
result = result.concat([[19,2]]);
}
if ("fdospatialcondition".match(word)) {
result = result.concat([[60,2]]);
}
if ("fdospatialcontextextenttype".match(word)) {
result = result.concat([[27,5]]);
}
if ("fdospatialoperations".match(word)) {
result = result.concat([[35,2]]);
}
if ("fdostring".match(word)) {
result = result.concat([[23,4],[33,8],[61,2],[88,1]]);
}
if ("fdostringp".match(word)) {
result = result.concat([[23,16]]);
}
if ("fdothreadcapability".match(word)) {
result = result.concat([[27,5]]);
}
if ("fdoversion".match(word)) {
result = result.concat([[22,1]]);
}
if ("fdoxmldeserializable".match(word)) {
result = result.concat([[50,2]]);
}
if ("fdoxmlfeatureflags".match(word)) {
result = result.concat([[93,4]]);
}
if ("fdoxmlflags".match(word)) {
result = result.concat([[93,2]]);
}
if ("fdoxmlserializable".match(word)) {
result = result.concat([[9,1],[50,2]]);
}
if ("feature".match(word)) {
result = result.concat([[0,2],[1,2],[2,1],[3,1],[4,2],[6,4],[7,8],[8,2],[9,4],[10,14],[13,40],[14,4],[22,10],[40,2],[41,44],[42,2],[43,3],[44,2],[46,19],[50,18],[51,26],[52,45],[55,17],[56,12],[57,8],[60,12],[61,3],[64,4],[66,4],[67,2],[68,2],[85,3],[86,2],[96,2],[110,25],[111,1],[113,2],[114,1],[116,9],[117,1],[119,2],[120,1],[122,4],[123,1],[125,12],[126,1],[128,3],[129,1]]);
}
if ("featureclass".match(word)) {
result = result.concat([[50,10]]);
}
if ("features".match(word)) {
result = result.concat([[0,2],[9,4],[13,6],[14,10],[40,2],[41,10],[44,6],[51,8],[59,2],[60,6],[66,4],[67,2],[86,2],[125,3],[128,1]]);
}
if ("features/attributes".match(word)) {
result = result.concat([[10,1],[22,1],[119,2]]);
}
if ("featureschema".match(word)) {
result = result.concat([[50,4],[51,2]]);
}
if ("featureschemaname".match(word)) {
result = result.concat([[50,6],[51,8]]);
}
if ("feature”".match(word)) {
result = result.concat([[50,2],[51,6]]);
}
if ("feet".match(word)) {
result = result.concat([[92,2]]);
}
if ("fet".match(word)) {
result = result.concat([[0,1],[113,2],[116,2],[119,2],[122,2],[125,2],[128,2]]);
}
if ("fgf".match(word)) {
result = result.concat([[83,23],[85,3],[86,10],[87,7],[88,15],[89,6]]);
}
if ("fid".match(word)) {
result = result.concat([[51,2],[52,2],[105,2],[106,5],[107,2],[110,40]]);
}
if ("field".match(word)) {
result = result.concat([[23,2],[86,1],[87,2]]);
}
if ("file".match(word)) {
result = result.concat([[3,7],[9,1],[10,3],[19,6],[22,6],[50,14],[51,13],[52,14],[108,4],[116,2],[119,10],[122,7],[128,2]]);
}
if ("file:".match(word)) {
result = result.concat([[52,4]]);
}
if ("filename".match(word)) {
result = result.concat([[51,2]]);
}
if ("files".match(word)) {
result = result.concat([[10,1],[19,6],[119,2],[122,1]]);
}
if ("files:".match(word)) {
result = result.concat([[122,1]]);
}
if ("files\\arcgis".match(word)) {
result = result.concat([[99,2]]);
}
if ("files\\arcgis\\arcsde\\bin".match(word)) {
result = result.concat([[99,2]]);
}
if ("filter".match(word)) {
result = result.concat([[2,1],[7,2],[13,2],[14,21],[25,1],[34,5],[56,8],[57,6],[60,2],[61,13],[65,7],[66,18],[67,8],[68,13],[69,4],[70,5],[71,19],[73,5],[76,2],[77,2],[111,5],[114,5],[117,5],[120,5],[123,5],[126,5],[129,7]]);
}
if ("filtercapabilities".match(word)) {
result = result.concat([[35,14]]);
}
if ("filters".match(word)) {
result = result.concat([[9,4],[14,2],[56,2],[60,2],[65,4],[66,11],[68,2],[70,2],[111,1],[114,1],[117,1],[120,1],[123,1],[126,1],[129,1]]);
}
if ("final".match(word)) {
result = result.concat([[51,5]]);
}
if ("finally".match(word)) {
result = result.concat([[55,2],[56,2],[57,2],[61,1],[68,2],[110,4]]);
}
if ("find".match(word)) {
result = result.concat([[3,2],[23,2],[52,2]]);
}
if ("finding".match(word)) {
result = result.concat([[19,2]]);
}
if ("finditem".match(word)) {
result = result.concat([[52,4]]);
}
if ("finishing".match(word)) {
result = result.concat([[16,2]]);
}
if ("first".match(word)) {
result = result.concat([[13,2],[23,5],[25,2],[51,2],[55,2],[56,2],[57,2],[87,2],[93,2],[108,8],[110,4]]);
}
if ("five".match(word)) {
result = result.concat([[13,2]]);
}
if ("flag".match(word)) {
result = result.concat([[22,1],[48,4],[49,2],[86,3]]);
}
if ("flags".match(word)) {
result = result.concat([[47,1],[48,2]]);
}
if ("flexible".match(word)) {
result = result.concat([[6,2]]);
}
if ("float".match(word)) {
result = result.concat([[109,4]]);
}
if ("floating".match(word)) {
result = result.concat([[55,1],[79,3],[83,2]]);
}
if ("floor".match(word)) {
result = result.concat([[114,1],[117,1],[120,1],[123,1]]);
}
if ("flush".match(word)) {
result = result.concat([[120,1],[123,1]]);
}
if ("folder".match(word)) {
result = result.concat([[9,2],[19,2],[99,2]]);
}
if ("follow".match(word)) {
result = result.concat([[2,1],[11,2],[15,4],[76,2]]);
}
if ("followed".match(word)) {
result = result.concat([[76,2]]);
}
if ("following".match(word)) {
result = result.concat([[2,2],[3,4],[9,4],[13,3],[14,2],[16,2],[18,6],[19,6],[22,2],[23,3],[25,4],[41,2],[43,2],[45,2],[46,4],[47,2],[50,2],[51,2],[52,10],[55,6],[56,2],[58,2],[61,2],[64,2],[71,4],[72,2],[73,2],[81,4],[82,2],[83,2],[85,2],[86,2],[87,8],[88,2],[90,2],[92,2],[93,2],[99,2],[102,2],[104,2],[110,4],[111,16],[114,16],[116,2],[117,14],[119,2],[120,16],[122,2],[123,16],[126,14],[128,2],[129,16]]);
}
if ("following:".match(word)) {
result = result.concat([[13,2],[23,2],[27,4],[29,4],[31,4],[33,4],[35,4],[37,4],[39,4],[46,2],[55,2],[61,2],[100,2],[113,4],[125,2]]);
}
if ("follows".match(word)) {
result = result.concat([[106,2]]);
}
if ("follows:".match(word)) {
result = result.concat([[10,2],[51,4],[52,2],[91,2],[105,2],[107,2],[110,6]]);
}
if ("footprint".match(word)) {
result = result.concat([[51,2]]);
}
if ("forests".match(word)) {
result = result.concat([[110,6]]);
}
if ("form".match(word)) {
result = result.concat([[22,3],[50,2],[55,4]]);
}
if ("format".match(word)) {
result = result.concat([[3,9],[10,2],[22,1],[50,7],[51,2],[52,2],[55,2],[56,2],[85,1],[86,8],[87,4],[88,4],[89,2],[119,7],[122,4],[128,2]]);
}
if ("format:".match(word)) {
result = result.concat([[22,2]]);
}
if ("formats".match(word)) {
result = result.concat([[2,1],[10,1],[42,2],[84,2],[86,2],[128,2]]);
}
if ("formatted".match(word)) {
result = result.concat([[87,2],[128,1]]);
}
if ("found".match(word)) {
result = result.concat([[13,2],[19,6],[86,2],[99,2],[102,2]]);
}
if ("foundation".match(word)) {
result = result.concat([[3,1],[10,1]]);
}
if ("four".match(word)) {
result = result.concat([[13,6],[67,2],[68,2]]);
}
if ("fragment".match(word)) {
result = result.concat([[25,2],[50,2],[51,2]]);
}
if ("framework".match(word)) {
result = result.concat([[86,1]]);
}
if ("freed".match(word)) {
result = result.concat([[16,2]]);
}
if ("freeing".match(word)) {
result = result.concat([[16,2]]);
}
if ("friendly".match(word)) {
result = result.concat([[22,1]]);
}
if ("full".match(word)) {
result = result.concat([[22,5],[41,2],[64,4],[113,1]]);
}
if ("fully".match(word)) {
result = result.concat([[41,4],[55,2],[56,2],[85,1]]);
}
if ("function".match(word)) {
result = result.concat([[2,1],[16,4],[18,2],[20,7],[46,4],[72,4],[73,2],[82,2],[83,2],[114,2],[117,2],[120,2],[123,2],[129,2]]);
}
if ("functionality".match(word)) {
result = result.concat([[6,2],[9,2],[10,1],[100,1],[102,2],[125,1]]);
}
if ("functiondefinition".match(word)) {
result = result.concat([[33,8]]);
}
if ("functiondescription".match(word)) {
result = result.concat([[33,2]]);
}
if ("functionname".match(word)) {
result = result.concat([[33,2]]);
}
if ("functions".match(word)) {
result = result.concat([[16,2],[33,8],[46,6],[111,3],[114,3],[117,3],[120,3],[123,3],[126,2],[129,3]]);
}
if ("further".match(word)) {
result = result.concat([[14,2]]);
}
if ("furthermore".match(word)) {
result = result.concat([[68,2]]);
}
if ("future".match(word)) {
result = result.concat([[54,1]]);
}
if ("gain".match(word)) {
result = result.concat([[14,2]]);
}
if ("gather".match(word)) {
result = result.concat([[10,1],[113,1]]);
}
if ("ge".match(word)) {
result = result.concat([[71,2],[81,4]]);
}
if ("general".match(word)) {
result = result.concat([[7,2],[13,2],[14,2],[55,5],[67,2],[68,2],[92,2]]);
}
if ("generally".match(word)) {
result = result.concat([[10,1],[47,1],[49,2],[128,2]]);
}
if ("generate".match(word)) {
result = result.concat([[50,2],[110,2]]);
}
if ("generated".match(word)) {
result = result.concat([[14,3],[22,2],[23,2],[45,1],[50,8],[111,1],[114,1],[117,1],[120,1],[123,1]]);
}
if ("generating".match(word)) {
result = result.concat([[83,1]]);
}
if ("generation".match(word)) {
result = result.concat([[111,1],[114,2],[117,1],[120,1],[123,1]]);
}
if ("generic".match(word)) {
result = result.concat([[110,2]]);
}
if ("genuine".match(word)) {
result = result.concat([[89,2]]);
}
if ("geodatabase".match(word)) {
result = result.concat([[22,1],[119,2]]);
}
if ("geodetic".match(word)) {
result = result.concat([[92,4]]);
}
if ("geographic".match(word)) {
result = result.concat([[10,1],[13,2],[116,1],[122,1],[125,4],[128,4]]);
}
if ("geography".match(word)) {
result = result.concat([[50,2],[125,4]]);
}
if ("geometric".match(word)) {
result = result.concat([[13,29],[52,10],[55,2],[61,2],[91,11],[110,2],[119,2]]);
}
if ("geometriccondition".match(word)) {
result = result.concat([[71,4]]);
}
if ("geometricproperty".match(word)) {
result = result.concat([[50,4],[51,2],[91,2],[111,1],[120,3],[123,3],[126,2],[129,1]]);
}
if ("geometricproperty:".match(word)) {
result = result.concat([[61,2]]);
}
if ("geometrictype".match(word)) {
result = result.concat([[91,6]]);
}
if ("geometries".match(word)) {
result = result.concat([[10,1],[13,7],[87,2],[89,2],[91,2],[92,3],[93,5],[102,1],[110,4],[125,2]]);
}
if ("geometry".match(word)) {
result = result.concat([[2,2],[7,2],[9,2],[10,2],[13,33],[25,1],[36,5],[44,2],[51,3],[52,2],[55,59],[60,2],[61,12],[69,3],[83,7],[84,7],[85,9],[86,8],[87,29],[88,6],[89,14],[90,10],[91,19],[93,4],[94,9],[103,6],[110,21],[111,10],[114,10],[117,8],[120,10],[122,2],[123,10],[126,10],[129,10]]);
}
if ("geometry:".match(word)) {
result = result.concat([[87,2]]);
}
if ("geometrybytearray".match(word)) {
result = result.concat([[55,4]]);
}
if ("geometrycapabilities".match(word)) {
result = result.concat([[37,10]]);
}
if ("geometrycollection".match(word)) {
result = result.concat([[83,8]]);
}
if ("geometrycomponenttypes".match(word)) {
result = result.concat([[37,6]]);
}
if ("geometryfactory".match(word)) {
result = result.concat([[93,6]]);
}
if ("geometryproperty".match(word)) {
result = result.concat([[50,6]]);
}
if ("geometrypropertyname".match(word)) {
result = result.concat([[50,2],[51,3]]);
}
if ("geometrytype".match(word)) {
result = result.concat([[61,4],[87,8]]);
}
if ("geometrytypes".match(word)) {
result = result.concat([[37,6]]);
}
if ("geometryvalue".match(word)) {
result = result.concat([[72,4]]);
}
if ("geometry”".match(word)) {
result = result.concat([[51,4]]);
}
if ("geomfromtext".match(word)) {
result = result.concat([[72,2],[73,2],[83,2]]);
}
if ("geomtype".match(word)) {
result = result.concat([[87,24]]);
}
if ("geomvalue".match(word)) {
result = result.concat([[120,1]]);
}
if ("georeferenced".match(word)) {
result = result.concat([[13,4],[92,4]]);
}
if ("geospatial".match(word)) {
result = result.concat([[3,1],[4,2],[7,2],[9,1],[10,3],[40,2],[41,6],[125,2],[128,2]]);
}
if ("getagf".match(word)) {
result = result.concat([[55,2]]);
}
if ("getarguments".match(word)) {
result = result.concat([[33,2]]);
}
if ("getbaseclass".match(word)) {
result = result.concat([[16,6],[18,6]]);
}
if ("getcause".match(word)) {
result = result.concat([[18,2]]);
}
if ("getclassdefinition".match(word)) {
result = result.concat([[61,2]]);
}
if ("getclasses".match(word)) {
result = result.concat([[52,4]]);
}
if ("getclasstypes".match(word)) {
result = result.concat([[29,2]]);
}
if ("getcommandcapabilities".match(word)) {
result = result.concat([[23,2],[31,2],[111,2],[114,2],[117,2],[120,2],[123,2],[126,2],[129,2]]);
}
if ("getcommands".match(word)) {
result = result.concat([[23,2],[31,2]]);
}
if ("getconnection".match(word)) {
result = result.concat([[23,1]]);
}
if ("getconnectioncapabilities".match(word)) {
result = result.concat([[27,2],[111,2],[114,2],[117,2],[120,2],[123,2],[126,2],[129,2]]);
}
if ("getconnectioninfo".match(word)) {
result = result.concat([[23,3]]);
}
if ("getconnectionproperties".match(word)) {
result = result.concat([[23,2]]);
}
if ("getconnectionstring".match(word)) {
result = result.concat([[23,1]]);
}
if ("getcount".match(word)) {
result = result.concat([[23,2],[33,4],[61,4]]);
}
if ("getdatastoreproperties".match(word)) {
result = result.concat([[23,2]]);
}
if ("getdatatype".match(word)) {
result = result.concat([[33,2],[61,2]]);
}
if ("getdatatypes".match(word)) {
result = result.concat([[29,2]]);
}
if ("getderivedtype".match(word)) {
result = result.concat([[61,2]]);
}
if ("getdescription".match(word)) {
result = result.concat([[33,4]]);
}
if ("getdimensionalities".match(word)) {
result = result.concat([[37,2]]);
}
if ("getdimensionality".match(word)) {
result = result.concat([[61,2]]);
}
if ("getdisplayname".match(word)) {
result = result.concat([[23,2]]);
}
if ("getexceptionmessage".match(word)) {
result = result.concat([[18,2]]);
}
if ("getexpressioncapabilities".match(word)) {
result = result.concat([[33,2],[111,2],[114,2],[117,2],[120,2],[123,2],[126,2],[129,2]]);
}
if ("getexpressiontypes".match(word)) {
result = result.concat([[33,2]]);
}
if ("getexteriorring".match(word)) {
result = result.concat([[61,2]]);
}
if ("getfiltercapabilities".match(word)) {
result = result.concat([[35,2],[111,2],[114,2],[117,2],[120,2],[123,2],[126,2],[129,2]]);
}
if ("getfunctions".match(word)) {
result = result.concat([[33,2]]);
}
if ("getgeometrictypes".match(word)) {
result = result.concat([[91,1]]);
}
if ("getgeometry".match(word)) {
result = result.concat([[61,2]]);
}
if ("getgeometrycapabilities".match(word)) {
result = result.concat([[37,2],[111,2],[114,2],[117,2],[120,2],[123,2],[126,2],[129,2]]);
}
if ("getidentityproperties".match(word)) {
result = result.concat([[52,2]]);
}
if ("getinstance".match(word)) {
result = result.concat([[55,1]]);
}
if ("getinteriorring".match(word)) {
result = result.concat([[61,2]]);
}
if ("getinteriorringcount".match(word)) {
result = result.concat([[61,2]]);
}
if ("getitem".match(word)) {
result = result.concat([[20,8],[23,2],[33,4],[61,4]]);
}
if ("getlocalizedname".match(word)) {
result = result.concat([[23,3]]);
}
if ("getlockedobjects".match(word)) {
result = result.concat([[111,1]]);
}
if ("getlockinfo".match(word)) {
result = result.concat([[111,1]]);
}
if ("getlockowners".match(word)) {
result = result.concat([[111,1]]);
}
if ("getlocktypes".match(word)) {
result = result.concat([[27,4]]);
}
if ("getlongtransactions".match(word)) {
result = result.concat([[64,2],[111,1]]);
}
if ("getm".match(word)) {
result = result.concat([[61,2]]);
}
if ("getname".match(word)) {
result = result.concat([[19,2],[20,8],[23,4],[33,4],[61,2]]);
}
if ("getobject".match(word)) {
result = result.concat([[13,2]]);
}
if ("getproperties".match(word)) {
result = result.concat([[20,8],[52,2],[61,2]]);
}
if ("getproperty".match(word)) {
result = result.concat([[23,4]]);
}
if ("getpropertydefault".match(word)) {
result = result.concat([[23,4]]);
}
if ("getpropertynames".match(word)) {
result = result.concat([[23,3]]);
}
if ("getpropertytype".match(word)) {
result = result.concat([[61,2]]);
}
if ("getpropertyvalues".match(word)) {
result = result.concat([[55,3],[56,2]]);
}
if ("getproviders".match(word)) {
result = result.concat([[23,2]]);
}
if ("getrastercapabilities".match(word)) {
result = result.concat([[39,2],[111,2],[114,2],[117,2],[120,2],[123,2],[126,2],[129,2]]);
}
if ("gets".match(word)) {
result = result.concat([[41,2]]);
}
if ("getschemacapabilities".match(word)) {
result = result.concat([[29,2],[111,2],[114,2],[117,2],[120,2],[123,2],[126,2],[129,2]]);
}
if ("getspatialcontexts".match(word)) {
result = result.concat([[111,1],[114,1],[117,1],[120,1],[123,1],[126,1],[129,1]]);
}
if ("getspatialcontexttypes".match(word)) {
result = result.concat([[27,2]]);
}
if ("getspatialindexes".match(word)) {
result = result.concat([[114,1],[126,1]]);
}
if ("gettext".match(word)) {
result = result.concat([[19,2],[88,1]]);
}
if ("getthreadcapability".match(word)) {
result = result.concat([[27,4]]);
}
if ("getting".match(word)) {
result = result.concat([[0,1],[8,5],[23,1]]);
}
if ("getx".match(word)) {
result = result.concat([[61,6]]);
}
if ("gety".match(word)) {
result = result.concat([[61,6]]);
}
if ("getz".match(word)) {
result = result.concat([[61,4]]);
}
if ("gf".match(word)) {
result = result.concat([[20,2]]);
}
if ("gif".match(word)) {
result = result.concat([[10,1],[128,3]]);
}
if ("gis".match(word)) {
result = result.concat([[6,2],[10,2],[44,2],[50,2],[51,2],[52,2],[119,2],[122,2]]);
}
if ("give".match(word)) {
result = result.concat([[23,4]]);
}
if ("given".match(word)) {
result = result.concat([[9,1],[20,2],[37,2],[64,2]]);
}
if ("gives".match(word)) {
result = result.concat([[91,2]]);
}
if ("global".match(word)) {
result = result.concat([[46,2],[90,2]]);
}
if ("gml".match(word)) {
result = result.concat([[3,2],[10,2],[50,22],[51,30],[116,1],[122,1],[125,4]]);
}
if ("gml:".match(word)) {
result = result.concat([[52,2]]);
}
if ("gml:abstractfeaturetype".match(word)) {
result = result.concat([[52,2]]);
}
if ("good".match(word)) {
result = result.concat([[12,2]]);
}
if ("goose".match(word)) {
result = result.concat([[55,4]]);
}
if ("gooseislandlinearringordinates".match(word)) {
result = result.concat([[55,2]]);
}
if ("grammar".match(word)) {
result = result.concat([[69,2],[71,5],[72,5],[83,2]]);
}
if ("grammar:".match(word)) {
result = result.concat([[71,2],[72,2]]);
}
if ("grant".match(word)) {
result = result.concat([[8,2],[110,2]]);
}
if ("grants".match(word)) {
result = result.concat([[110,2]]);
}
if ("graph".match(word)) {
result = result.concat([[14,2],[63,2]]);
}
if ("graphic".match(word)) {
result = result.concat([[10,2]]);
}
if ("graphical".match(word)) {
result = result.concat([[128,2]]);
}
if ("graphics".match(word)) {
result = result.concat([[10,2],[128,4]]);
}
if ("gray/8".match(word)) {
result = result.concat([[129,1]]);
}
if ("greaterthan".match(word)) {
result = result.concat([[71,2],[81,2]]);
}
if ("greaterthanorequalto".match(word)) {
result = result.concat([[71,2],[81,2]]);
}
if ("greatly".match(word)) {
result = result.concat([[13,2]]);
}
if ("green".match(word)) {
result = result.concat([[56,2]]);
}
if ("ground”".match(word)) {
result = result.concat([[92,2]]);
}
if ("group".match(word)) {
result = result.concat([[14,2],[63,2]]);
}
if ("grouped".match(word)) {
result = result.concat([[9,2],[12,2],[25,2],[111,2],[114,2],[117,2],[120,2],[123,2],[126,2],[129,2]]);
}
if ("grouping".match(word)) {
result = result.concat([[114,1],[117,1]]);
}
if ("gt".match(word)) {
result = result.concat([[71,2],[81,4]]);
}
if ("guide".match(word)) {
result = result.concat([[0,7],[1,2],[2,7],[9,2],[50,2],[110,2]]);
}
if ("h".match(word)) {
result = result.concat([[19,2],[22,2],[25,2]]);
}
if ("hand".match(word)) {
result = result.concat([[50,2]]);
}
if ("handle".match(word)) {
result = result.concat([[13,2]]);
}
if ("handled".match(word)) {
result = result.concat([[83,2],[89,2]]);
}
if ("handler".match(word)) {
result = result.concat([[23,2]]);
}
if ("handling".match(word)) {
result = result.concat([[18,5]]);
}
if ("happen".match(word)) {
result = result.concat([[14,6]]);
}
if ("happend".match(word)) {
result = result.concat([[19,2]]);
}
if ("haselevation".match(word)) {
result = result.concat([[13,2]]);
}
if ("hasmeasure".match(word)) {
result = result.concat([[13,2]]);
}
if ("hassdemaintainedrowid".match(word)) {
result = result.concat([[105,4],[106,4],[107,4]]);
}
if ("having".match(word)) {
result = result.concat([[44,2]]);
}
if ("heap".match(word)) {
result = result.concat([[89,1],[113,1]]);
}
if ("height".match(word)) {
result = result.concat([[129,1]]);
}
if ("help".match(word)) {
result = result.concat([[8,2],[9,4],[16,4],[20,2],[27,2],[29,2],[31,2],[58,2],[64,2],[110,2],[113,2],[116,2],[119,2],[122,2],[128,2]]);
}
if ("helper".match(word)) {
result = result.concat([[89,2]]);
}
if ("hereafter".match(word)) {
result = result.concat([[84,2]]);
}
if ("herein".match(word)) {
result = result.concat([[25,1]]);
}
if ("heterogeneous".match(word)) {
result = result.concat([[41,2]]);
}
if ("heterogenous".match(word)) {
result = result.concat([[13,1]]);
}
if ("hh:mm:ss".match(word)) {
result = result.concat([[80,4]]);
}
if ("hierarchy".match(word)) {
result = result.concat([[41,4]]);
}
if ("high".match(word)) {
result = result.concat([[9,2],[10,1],[119,2]]);
}
if ("highest".match(word)) {
result = result.concat([[72,2],[81,2]]);
}
if ("hits".match(word)) {
result = result.concat([[16,2]]);
}
if ("hold".match(word)) {
result = result.concat([[22,2]]);
}
if ("host".match(word)) {
result = result.concat([[99,2],[110,6]]);
}
if ("hosting".match(word)) {
result = result.concat([[108,2]]);
}
if ("however".match(word)) {
result = result.concat([[18,2],[22,2],[42,2],[44,2],[46,2],[50,2],[67,2],[68,2],[92,2],[108,2],[110,4]]);
}
if ("html".match(word)) {
result = result.concat([[71,2]]);
}
if ("http://cui".match(word)) {
result = result.concat([[71,2]]);
}
if ("http://fdo".match(word)) {
result = result.concat([[50,2],[52,10]]);
}
if ("http://www".match(word)) {
result = result.concat([[52,4]]);
}
if ("hydrant".match(word)) {
result = result.concat([[13,2]]);
}
if ("i".match(word)) {
result = result.concat([[23,24],[33,8],[52,2],[61,24],[96,2],[99,2],[110,4]]);
}
if ("iapplyschema".match(word)) {
result = result.concat([[48,2]]);
}
if ("id".match(word)) {
result = result.concat([[51,2],[104,7],[105,4],[106,13],[107,6],[111,2],[114,3],[117,2],[120,2],[123,1],[126,1]]);
}
if ("identical".match(word)) {
result = result.concat([[113,1]]);
}
if ("identified".match(word)) {
result = result.concat([[41,2],[51,2],[64,2],[110,2]]);
}
if ("identifier".match(word)) {
result = result.concat([[43,2],[44,10],[45,2],[46,11],[71,8],[72,8],[73,2],[75,9],[92,2]]);
}
if ("identifiers".match(word)) {
result = result.concat([[16,2],[43,4],[44,2],[45,3],[75,2],[82,2]]);
}
if ("identifies".match(word)) {
result = result.concat([[52,2],[56,2],[104,2],[110,8]]);
}
if ("identify".match(word)) {
result = result.concat([[2,1],[9,1],[14,8],[46,2],[52,2],[56,4],[57,2],[61,1],[65,2],[66,8],[92,2],[104,1],[110,2]]);
}
if ("identifying".match(word)) {
result = result.concat([[56,2],[57,2]]);
}
if ("identity".match(word)) {
result = result.concat([[13,4],[44,2],[46,4],[50,4],[51,4],[52,10],[55,6],[56,8],[57,2],[61,4],[92,2],[106,5],[110,4],[111,1],[114,1],[117,1],[120,1],[123,1],[126,1]]);
}
if ("identityproperty".match(word)) {
result = result.concat([[46,20],[50,2]]);
}
if ("identityproperty1name".match(word)) {
result = result.concat([[50,2],[51,2]]);
}
if ("identitypropertyname".match(word)) {
result = result.concat([[51,1]]);
}
if ("ids".match(word)) {
result = result.concat([[8,2]]);
}
if ("id        ".match(word)) {
result = result.concat([[105,2],[106,2],[107,2]]);
}
if ("id        :".match(word)) {
result = result.concat([[105,2],[106,2],[107,2]]);
}
if ("id”".match(word)) {
result = result.concat([[51,1]]);
}
if ("ieee".match(word)) {
result = result.concat([[87,2]]);
}
if ("ifdoapplyschemacommand".match(word)) {
result = result.concat([[43,1]]);
}
if ("ignored".match(word)) {
result = result.concat([[83,2]]);
}
if ("illustrate".match(word)) {
result = result.concat([[104,2]]);
}
if ("illustrates".match(word)) {
result = result.concat([[16,2],[51,2]]);
}
if ("image".match(word)) {
result = result.concat([[13,4],[39,2],[128,2]]);
}
if ("images".match(word)) {
result = result.concat([[128,2]]);
}
if ("immediately".match(word)) {
result = result.concat([[47,1]]);
}
if ("impacts".match(word)) {
result = result.concat([[110,2]]);
}
if ("implement".match(word)) {
result = result.concat([[2,1],[9,1],[22,2],[62,2]]);
}
if ("implementation".match(word)) {
result = result.concat([[7,2],[10,2],[16,2],[41,2],[89,2],[96,4],[113,2],[116,2],[119,2],[122,2],[125,4],[128,2]]);
}
if ("implementing".match(word)) {
result = result.concat([[89,2]]);
}
if ("implements".match(word)) {
result = result.concat([[51,2]]);
}
if ("inc/fdo/clientservices/providerregistry".match(word)) {
result = result.concat([[22,2]]);
}
if ("include".match(word)) {
result = result.concat([[25,2],[51,4],[60,2],[75,2],[77,2],[79,2],[100,2]]);
}
if ("include:".match(word)) {
result = result.concat([[83,2]]);
}
if ("included".match(word)) {
result = result.concat([[9,2],[10,2],[13,2],[66,2],[82,2],[86,1]]);
}
if ("includes".match(word)) {
result = result.concat([[7,2],[9,2],[13,6],[51,2],[64,2],[86,1],[125,2]]);
}
if ("including".match(word)) {
result = result.concat([[7,2],[9,2],[13,2],[22,3],[46,2],[66,2],[67,2],[85,2],[116,2],[122,1],[128,2]]);
}
if ("inclusive".match(word)) {
result = result.concat([[114,1],[120,1]]);
}
if ("incompletely".match(word)) {
result = result.concat([[90,1]]);
}
if ("incondition".match(word)) {
result = result.concat([[71,4]]);
}
if ("incorporate".match(word)) {
result = result.concat([[55,1]]);
}
if ("incorporated".match(word)) {
result = result.concat([[55,2]]);
}
if ("increased".match(word)) {
result = result.concat([[16,2]]);
}
if ("increments".match(word)) {
result = result.concat([[16,2]]);
}
if ("independent".match(word)) {
result = result.concat([[9,2],[41,2]]);
}
if ("index".match(word)) {
result = result.concat([[10,1],[16,1],[42,4],[55,2],[108,2],[122,1]]);
}
if ("indexed".match(word)) {
result = result.concat([[19,2]]);
}
if ("indexes".match(word)) {
result = result.concat([[113,1]]);
}
if ("indexing".match(word)) {
result = result.concat([[10,1],[22,1],[119,3]]);
}
if ("indicate".match(word)) {
result = result.concat([[86,1]]);
}
if ("indicated".match(word)) {
result = result.concat([[110,6]]);
}
if ("indicates".match(word)) {
result = result.concat([[7,4],[90,1],[110,2]]);
}
if ("indicating".match(word)) {
result = result.concat([[22,1],[110,2]]);
}
if ("indirectly".match(word)) {
result = result.concat([[13,2],[108,2]]);
}
if ("individual".match(word)) {
result = result.concat([[13,1]]);
}
if ("info".match(word)) {
result = result.concat([[23,6]]);
}
if ("information".match(word)) {
result = result.concat([[0,1],[1,2],[2,1],[3,1],[4,2],[6,4],[7,2],[8,2],[9,6],[10,5],[13,6],[14,4],[20,2],[22,8],[23,6],[27,2],[29,2],[31,2],[41,2],[42,2],[46,8],[50,2],[53,2],[58,2],[63,2],[64,6],[66,2],[70,2],[71,2],[83,2],[94,2],[96,2],[99,2],[108,4],[110,2],[113,3],[116,3],[119,2],[122,3],[125,2],[128,7]]);
}
if ("informational".match(word)) {
result = result.concat([[25,1]]);
}
if ("inherit".match(word)) {
result = result.concat([[16,2],[46,1]]);
}
if ("inheritance".match(word)) {
result = result.concat([[41,2],[46,1],[114,1],[117,1],[120,1],[129,1]]);
}
if ("inherited".match(word)) {
result = result.concat([[46,1]]);
}
if ("inheriting".match(word)) {
result = result.concat([[16,2]]);
}
if ("inherits".match(word)) {
result = result.concat([[16,2],[44,4]]);
}
if ("initial".match(word)) {
result = result.concat([[22,1],[108,2]]);
}
if ("initialization".match(word)) {
result = result.concat([[22,2]]);
}
if ("initially".match(word)) {
result = result.concat([[48,1]]);
}
if ("initiates".match(word)) {
result = result.concat([[23,2]]);
}
if ("inline".match(word)) {
result = result.concat([[87,2]]);
}
if ("innodb".match(word)) {
result = result.concat([[113,1]]);
}
if ("input".match(word)) {
result = result.concat([[23,2],[64,4],[67,2],[80,1],[83,2]]);
}
if ("insensitive".match(word)) {
result = result.concat([[73,2]]);
}
if ("insert".match(word)) {
result = result.concat([[46,2],[55,24],[56,4],[94,2],[105,2],[106,2],[107,2],[111,1],[114,1],[117,1],[120,1],[123,1]]);
}
if ("inserted".match(word)) {
result = result.concat([[46,4]]);
}
if ("inserting".match(word)) {
result = result.concat([[10,1],[41,4],[47,2],[54,1],[55,12],[56,2],[57,2],[94,5],[110,3]]);
}
if ("insertion".match(word)) {
result = result.concat([[55,2]]);
}
if ("inserts".match(word)) {
result = result.concat([[46,4]]);
}
if ("inside".match(word)) {
result = result.concat([[71,2],[73,2],[75,2],[77,2],[111,1],[117,1],[120,1],[123,1]]);
}
if ("inspection".match(word)) {
result = result.concat([[13,2]]);
}
if ("install".match(word)) {
result = result.concat([[8,2],[22,1],[50,2],[98,2]]);
}
if ("installation".match(word)) {
result = result.concat([[22,2],[23,4]]);
}
if ("installed".match(word)) {
result = result.concat([[23,4],[98,7],[99,6]]);
}
if ("installing".match(word)) {
result = result.concat([[0,1],[22,2],[99,2]]);
}
if ("instance".match(word)) {
result = result.concat([[13,10],[23,1],[41,2],[55,2],[61,5],[66,4],[108,6],[110,2]]);
}
if ("instances".match(word)) {
result = result.concat([[13,10],[55,4],[125,4]]);
}
if ("instantiable".match(word)) {
result = result.concat([[90,1]]);
}
if ("instantiate".match(word)) {
result = result.concat([[17,2],[22,2]]);
}
if ("instantiated".match(word)) {
result = result.concat([[13,2]]);
}
if ("instantiation".match(word)) {
result = result.concat([[7,2]]);
}
if ("instead".match(word)) {
result = result.concat([[19,2],[20,2],[48,2],[92,2]]);
}
if ("instead:".match(word)) {
result = result.concat([[19,2]]);
}
if ("institute".match(word)) {
result = result.concat([[122,1]]);
}
if ("instructions".match(word)) {
result = result.concat([[13,2]]);
}
if ("insufficient".match(word)) {
result = result.concat([[86,1]]);
}
if ("int".match(word)) {
result = result.concat([[23,10],[33,4],[61,6],[87,64]]);
}
if ("int16".match(word)) {
result = result.concat([[13,2],[50,2],[55,1],[109,2],[111,7],[114,7],[117,8],[120,7],[123,5],[126,2]]);
}
if ("int32".match(word)) {
result = result.concat([[13,2],[50,2],[52,4],[55,1],[109,2],[110,1],[111,8],[114,7],[117,8],[120,8],[123,8],[126,2],[129,2]]);
}
if ("int32:".match(word)) {
result = result.concat([[61,2]]);
}
if ("int53".match(word)) {
result = result.concat([[117,1]]);
}
if ("int64".match(word)) {
result = result.concat([[13,2],[45,1],[50,2],[55,1],[109,2],[111,2],[114,9],[117,8],[120,8],[123,6],[126,2]]);
}
if ("integer".match(word)) {
result = result.concat([[23,4],[41,2],[51,4],[52,2],[55,10],[60,2],[61,2],[71,2],[72,2],[78,5],[79,1],[83,2],[87,2],[91,1],[105,2],[109,4],[110,30],[129,5]]);
}
if ("integers".match(word)) {
result = result.concat([[78,2]]);
}
if ("integrated".match(word)) {
result = result.concat([[9,2],[13,2]]);
}
if ("integrity".match(word)) {
result = result.concat([[14,1]]);
}
if ("intelligent".match(word)) {
result = result.concat([[91,2]]);
}
if ("intended".match(word)) {
result = result.concat([[1,2],[6,2],[67,2],[68,2],[110,2]]);
}
if ("interaction".match(word)) {
result = result.concat([[128,1]]);
}
if ("interdependent".match(word)) {
result = result.concat([[12,2]]);
}
if ("interface".match(word)) {
result = result.concat([[0,2],[3,11],[7,4],[9,2],[14,2],[22,2],[23,2],[25,1],[50,4],[64,12],[66,2],[76,2],[89,2]]);
}
if ("interfaces".match(word)) {
result = result.concat([[14,2],[62,2],[64,9],[85,1]]);
}
if ("interior".match(word)) {
result = result.concat([[13,2],[55,4],[61,2]]);
}
if ("interiorring".match(word)) {
result = result.concat([[61,2]]);
}
if ("interiorringsbluelake".match(word)) {
result = result.concat([[55,6]]);
}
if ("internal".match(word)) {
result = result.concat([[3,2],[23,5],[55,4]]);
}
if ("internalname".match(word)) {
result = result.concat([[23,4]]);
}
if ("international".match(word)) {
result = result.concat([[128,2]]);
}
if ("interoperability".match(word)) {
result = result.concat([[10,1],[119,2]]);
}
if ("interrogate".match(word)) {
result = result.concat([[41,2]]);
}
if ("intersects".match(word)) {
result = result.concat([[71,2],[73,2],[111,1],[114,1],[117,1],[120,1],[123,1]]);
}
if ("introduced".match(word)) {
result = result.concat([[3,2]]);
}
if ("introduces".match(word)) {
result = result.concat([[0,2],[1,2],[4,2]]);
}
if ("introduction".match(word)) {
result = result.concat([[4,5],[85,5]]);
}
if ("invariable".match(word)) {
result = result.concat([[22,2]]);
}
if ("invoke".match(word)) {
result = result.concat([[14,2]]);
}
if ("invoked".match(word)) {
result = result.concat([[49,2],[110,6]]);
}
if ("involved".match(word)) {
result = result.concat([[43,2],[64,2]]);
}
if ("involves".match(word)) {
result = result.concat([[55,1],[56,2],[57,2]]);
}
if ("is:".match(word)) {
result = result.concat([[105,2],[106,2],[107,2]]);
}
if ("isdatastorename".match(word)) {
result = result.concat([[23,2]]);
}
if ("isenumerable".match(word)) {
result = result.concat([[23,4]]);
}
if ("isfilename".match(word)) {
result = result.concat([[23,2]]);
}
if ("isfilepath".match(word)) {
result = result.concat([[23,2]]);
}
if ("island".match(word)) {
result = result.concat([[55,10]]);
}
if ("ismanaged".match(word)) {
result = result.concat([[22,1]]);
}
if ("isnullable".match(word)) {
result = result.concat([[50,4],[51,4]]);
}
if ("iso".match(word)) {
result = result.concat([[9,1],[10,2],[41,2]]);
}
if ("isolated".match(word)) {
result = result.concat([[14,1]]);
}
if ("ispropertydatastorename".match(word)) {
result = result.concat([[23,2]]);
}
if ("ispropertyenumerable".match(word)) {
result = result.concat([[23,3]]);
}
if ("ispropertyfilename".match(word)) {
result = result.concat([[23,2]]);
}
if ("ispropertyfilepath".match(word)) {
result = result.concat([[23,2]]);
}
if ("ispropertyprotected".match(word)) {
result = result.concat([[23,3]]);
}
if ("ispropertyrequired".match(word)) {
result = result.concat([[23,3]]);
}
if ("isprotected".match(word)) {
result = result.concat([[23,2]]);
}
if ("isrequired".match(word)) {
result = result.concat([[23,5]]);
}
if ("issues".match(word)) {
result = result.concat([[2,8],[11,2],[40,2],[46,5],[69,7],[95,2],[112,2],[115,2],[118,2],[121,2],[124,2],[127,2]]);
}
if ("item".match(word)) {
result = result.concat([[46,2]]);
}
if ("iterate".match(word)) {
result = result.concat([[60,2]]);
}
if ("itself".match(word)) {
result = result.concat([[13,4],[87,2],[89,2],[128,2]]);
}
if ("it’s".match(word)) {
result = result.concat([[61,4]]);
}
if ("j".match(word)) {
result = result.concat([[23,8],[33,8]]);
}
if ("jpeg".match(word)) {
result = result.concat([[10,1],[128,3]]);
}
if ("just".match(word)) {
result = result.concat([[16,2]]);
}
if ("key".match(word)) {
result = result.concat([[2,1],[51,2],[52,2]]);
}
if ("keyboard".match(word)) {
result = result.concat([[77,2]]);
}
if ("keyword".match(word)) {
result = result.concat([[75,2]]);
}
if ("keywords".match(word)) {
result = result.concat([[69,1],[73,7]]);
}
if ("keyword        :".match(word)) {
result = result.concat([[105,2],[106,2],[107,2]]);
}
if ("key”".match(word)) {
result = result.concat([[50,2],[51,2]]);
}
if ("kinds".match(word)) {
result = result.concat([[13,2]]);
}
if ("know".match(word)) {
result = result.concat([[108,4]]);
}
if ("knowledge".match(word)) {
result = result.concat([[91,2]]);
}
if ("known".match(word)) {
result = result.concat([[86,2]]);
}
if ("l".match(word)) {
result = result.concat([[52,30],[55,10],[56,8],[57,4],[61,4],[106,2],[107,2],[110,32]]);
}
if ("lake".match(word)) {
result = result.concat([[55,16],[56,2]]);
}
if ("lakes".match(word)) {
result = result.concat([[52,4],[110,6]]);
}
if ("land".match(word)) {
result = result.concat([[13,4]]);
}
if ("landuse".match(word)) {
result = result.concat([[13,2]]);
}
if ("lanes".match(word)) {
result = result.concat([[14,2],[66,2],[67,4],[68,4],[110,4]]);
}
if ("language".match(word)) {
result = result.concat([[18,2],[19,2],[22,1],[50,2],[67,2],[68,2],[69,7],[73,2],[76,2],[77,2],[116,1],[122,1],[125,6]]);
}
if ("languages".match(word)) {
result = result.concat([[65,5]]);
}
if ("languages:".match(word)) {
result = result.concat([[81,2]]);
}
if ("large".match(word)) {
result = result.concat([[3,2],[10,1],[13,4],[55,2],[119,2]]);
}
if ("larger".match(word)) {
result = result.concat([[14,2],[67,2]]);
}
if ("last".match(word)) {
result = result.concat([[49,2]]);
}
if ("latter".match(word)) {
result = result.concat([[51,2],[55,2]]);
}
if ("launch".match(word)) {
result = result.concat([[110,1]]);
}
if ("layer".match(word)) {
result = result.concat([[7,4]]);
}
if ("layout".match(word)) {
result = result.concat([[13,2],[86,2],[87,6]]);
}
if ("le".match(word)) {
result = result.concat([[71,2],[81,4]]);
}
if ("leaf".match(word)) {
result = result.concat([[63,2],[64,4]]);
}
if ("leaks".match(word)) {
result = result.concat([[16,2],[20,2]]);
}
if ("learn".match(word)) {
result = result.concat([[111,14],[114,14],[117,14],[120,14],[123,14],[126,14],[129,14]]);
}
if ("least".match(word)) {
result = result.concat([[45,2],[68,2],[87,4],[116,1]]);
}
if ("leaves".match(word)) {
result = result.concat([[14,1]]);
}
if ("left".match(word)) {
result = result.concat([[64,1],[72,6],[77,2],[82,2]]);
}
if ("length".match(word)) {
result = result.concat([[50,2],[51,3],[87,4],[109,4],[111,7],[114,10],[117,10],[120,10],[123,5],[126,10],[129,2]]);
}
if ("lessthan".match(word)) {
result = result.concat([[71,2],[81,2]]);
}
if ("lessthanorequalto".match(word)) {
result = result.concat([[71,2],[81,2]]);
}
if ("let’s".match(word)) {
result = result.concat([[23,4]]);
}
if ("level".match(word)) {
result = result.concat([[3,1],[9,2],[18,4],[41,2],[46,2],[52,2],[66,2]]);
}
if ("lexical".match(word)) {
result = result.concat([[83,2]]);
}
if ("libraries".match(word)) {
result = result.concat([[98,2],[99,2]]);
}
if ("library".match(word)) {
result = result.concat([[22,2]]);
}
if ("librarypath".match(word)) {
result = result.concat([[22,1]]);
}
if ("like:".match(word)) {
result = result.concat([[68,8]]);
}
if ("limit".match(word)) {
result = result.concat([[60,2]]);
}
if ("limitation".match(word)) {
result = result.concat([[111,5],[114,5],[117,5],[120,5],[123,5],[126,5],[129,5]]);
}
if ("limitations".match(word)) {
result = result.concat([[100,5],[101,9]]);
}
if ("limited".match(word)) {
result = result.concat([[128,1]]);
}
if ("line".match(word)) {
result = result.concat([[13,9],[23,1],[51,2],[91,1],[103,2],[110,1]]);
}
if ("linear".match(word)) {
result = result.concat([[13,3],[55,8],[92,2],[102,1]]);
}
if ("linearcurvestringelement".match(word)) {
result = result.concat([[87,2]]);
}
if ("linearring".match(word)) {
result = result.concat([[87,4],[111,2],[114,2],[120,2],[123,2],[126,2],[129,2]]);
}
if ("linearringgooseisland".match(word)) {
result = result.concat([[55,6]]);
}
if ("lines".match(word)) {
result = result.concat([[23,2]]);
}
if ("linestring".match(word)) {
result = result.concat([[13,1],[83,12],[87,8],[90,2],[111,1],[114,1],[120,1],[123,1],[126,1]]);
}
if ("linestring:".match(word)) {
result = result.concat([[61,2]]);
}
if ("linestringcollection".match(word)) {
result = result.concat([[83,8]]);
}
if ("linestrings".match(word)) {
result = result.concat([[13,1],[87,4]]);
}
if ("linestringsegment".match(word)) {
result = result.concat([[13,1],[83,26],[111,2],[120,2],[123,2],[126,2]]);
}
if ("link".match(word)) {
result = result.concat([[46,2]]);
}
if ("linked".match(word)) {
result = result.concat([[98,2],[99,2]]);
}
if ("linux".match(word)) {
result = result.concat([[19,8],[116,2],[122,1],[128,2]]);
}
if ("list".match(word)) {
result = result.concat([[10,1],[13,2],[16,1],[22,2],[23,13],[27,4],[29,6],[31,2],[33,2],[50,6],[51,8],[55,4],[61,2],[64,2],[90,1],[108,8],[110,2],[120,1]]);
}
if ("listdatastores".match(word)) {
result = result.concat([[111,1],[114,1]]);
}
if ("listening".match(word)) {
result = result.concat([[108,2]]);
}
if ("lists".match(word)) {
result = result.concat([[13,2],[23,2],[113,1]]);
}
if ("literal".match(word)) {
result = result.concat([[76,2],[77,2],[80,2]]);
}
if ("literalvalue".match(word)) {
result = result.concat([[72,4]]);
}
if ("little".match(word)) {
result = result.concat([[86,1],[87,4]]);
}
if ("local".match(word)) {
result = result.concat([[16,2],[23,2],[46,8],[99,2]]);
}
if ("localhost".match(word)) {
result = result.concat([[23,2]]);
}
if ("localized".match(word)) {
result = result.concat([[19,4],[23,2]]);
}
if ("localname".match(word)) {
result = result.concat([[23,2]]);
}
if ("locate".match(word)) {
result = result.concat([[19,4]]);
}
if ("located".match(word)) {
result = result.concat([[22,2],[98,2],[99,4],[108,2],[110,2]]);
}
if ("location".match(word)) {
result = result.concat([[13,2],[14,2],[22,1],[50,2],[66,2],[87,2],[98,2],[99,2]]);
}
if ("locations".match(word)) {
result = result.concat([[10,1],[13,2],[116,1]]);
}
if ("lock".match(word)) {
result = result.concat([[3,1],[9,1],[14,2],[104,1],[105,1],[106,1],[125,1]]);
}
if ("locking".match(word)) {
result = result.concat([[3,2],[7,2],[10,1],[14,7],[22,1],[41,1],[54,2],[63,2],[64,4],[104,9],[106,5],[107,5],[111,2],[119,3],[122,1]]);
}
if ("locks".match(word)) {
result = result.concat([[13,2],[14,12],[104,2]]);
}
if ("locks—transaction".match(word)) {
result = result.concat([[14,2]]);
}
if ("locktypes".match(word)) {
result = result.concat([[27,6]]);
}
if ("lock              ".match(word)) {
result = result.concat([[105,2],[106,2],[107,2]]);
}
if ("lock”".match(word)) {
result = result.concat([[107,1]]);
}
if ("log".match(word)) {
result = result.concat([[110,2]]);
}
if ("logical".match(word)) {
result = result.concat([[9,1],[13,2],[41,14],[42,2]]);
}
if ("logicaloperator".match(word)) {
result = result.concat([[71,4]]);
}
if ("login".match(word)) {
result = result.concat([[110,1]]);
}
if ("long".match(word)) {
result = result.concat([[2,1],[3,3],[14,26],[62,10],[63,25],[64,61],[109,2],[111,1]]);
}
if ("look".match(word)) {
result = result.concat([[67,2],[68,8],[108,2]]);
}
if ("loop".match(word)) {
result = result.concat([[18,2],[23,1],[27,4],[29,4],[31,2],[33,2],[35,6],[37,4],[46,2],[61,4]]);
}
if ("loops".match(word)) {
result = result.concat([[23,1],[61,1]]);
}
if ("lost".match(word)) {
result = result.concat([[14,1]]);
}
if ("lower".match(word)) {
result = result.concat([[114,1],[117,1],[120,1],[123,1]]);
}
if ("lowest:".match(word)) {
result = result.concat([[81,2]]);
}
if ("ls".match(word)) {
result = result.concat([[19,2]]);
}
if ("lt".match(word)) {
result = result.concat([[2,2],[14,2],[62,4],[63,2],[71,2],[81,4]]);
}
if ("l”datastore”".match(word)) {
result = result.concat([[23,2]]);
}
if ("l”fdo".match(word)) {
result = result.concat([[23,2]]);
}
if ("l”geomfromtext".match(word)) {
result = result.concat([[93,4]]);
}
if ("l”localhost”".match(word)) {
result = result.concat([[23,2]]);
}
if ("l”osgeo".match(word)) {
result = result.concat([[23,2]]);
}
if ("l”password”".match(word)) {
result = result.concat([[23,2]]);
}
if ("l”root”".match(word)) {
result = result.concat([[23,2]]);
}
if ("l”service”".match(word)) {
result = result.concat([[23,2]]);
}
if ("l”test”".match(word)) {
result = result.concat([[23,2]]);
}
if ("l”username".match(word)) {
result = result.concat([[23,1]]);
}
if ("l”username”".match(word)) {
result = result.concat([[23,2]]);
}
if ("m".match(word)) {
result = result.concat([[13,6],[16,1],[37,2],[61,6],[87,8],[109,2]]);
}
if ("machine".match(word)) {
result = result.concat([[23,2],[99,2],[108,4],[110,2]]);
}
if ("macro".match(word)) {
result = result.concat([[16,4]]);
}
if ("macros".match(word)) {
result = result.concat([[16,2]]);
}
if ("made".match(word)) {
result = result.concat([[13,2],[14,1],[22,4],[23,2],[48,2]]);
}
if ("main".match(word)) {
result = result.concat([[46,2],[55,2],[89,2],[125,1]]);
}
if ("maintain".match(word)) {
result = result.concat([[2,1],[48,2],[53,2]]);
}
if ("maintained".match(word)) {
result = result.concat([[48,2],[104,4],[106,3],[107,2],[116,1]]);
}
if ("maintenance".match(word)) {
result = result.concat([[11,1],[53,5],[54,7],[58,2],[61,2]]);
}
if ("major".match(word)) {
result = result.concat([[11,4]]);
}
if ("making".match(word)) {
result = result.concat([[23,1]]);
}
if ("man".match(word)) {
result = result.concat([[13,2]]);
}
if ("manage".match(word)) {
result = result.concat([[9,1],[12,1],[13,2],[14,2],[16,2]]);
}
if ("managed".match(word)) {
result = result.concat([[22,1]]);
}
if ("management".match(word)) {
result = result.concat([[2,1],[7,2],[8,2],[9,2],[11,1],[16,5],[40,7],[52,5],[96,2],[110,2]]);
}
if ("management:".match(word)) {
result = result.concat([[43,2]]);
}
if ("manager".match(word)) {
result = result.concat([[20,2],[23,1]]);
}
if ("manager’s".match(word)) {
result = result.concat([[23,1]]);
}
if ("manages".match(word)) {
result = result.concat([[96,2]]);
}
if ("managing".match(word)) {
result = result.concat([[6,2],[20,7]]);
}
if ("manipulate".match(word)) {
result = result.concat([[4,2],[12,1],[13,2],[14,2]]);
}
if ("manipulation".match(word)) {
result = result.concat([[7,2],[64,4],[125,2],[128,2]]);
}
if ("manufacturers".match(word)) {
result = result.concat([[46,2]]);
}
if ("map".match(word)) {
result = result.concat([[10,1],[13,4],[23,1],[50,2],[91,1],[110,6],[128,9]]);
}
if ("mapped".match(word)) {
result = result.concat([[41,2],[52,8]]);
}
if ("mapping".match(word)) {
result = result.concat([[3,7],[13,9],[22,4],[41,2],[42,2],[50,4],[91,5]]);
}
if ("mappings".match(word)) {
result = result.concat([[3,2],[13,6],[41,7],[42,5],[109,7],[116,1],[122,1]]);
}
if ("maps".match(word)) {
result = result.concat([[10,1],[13,4],[41,2],[116,2],[122,1],[128,4]]);
}
if ("mark".match(word)) {
result = result.concat([[23,1]]);
}
if ("marked".match(word)) {
result = result.concat([[48,8]]);
}
if ("marks".match(word)) {
result = result.concat([[48,1]]);
}
if ("markup".match(word)) {
result = result.concat([[50,2],[116,1],[122,1],[125,6]]);
}
if ("mask".match(word)) {
result = result.concat([[91,1]]);
}
if ("match".match(word)) {
result = result.concat([[6,2],[23,1]]);
}
if ("matching".match(word)) {
result = result.concat([[60,2]]);
}
if ("max".match(word)) {
result = result.concat([[111,1],[114,1],[117,1],[120,1],[123,1]]);
}
if ("maximum".match(word)) {
result = result.concat([[111,7],[114,12],[117,12],[120,12],[123,7],[126,12],[129,2]]);
}
if ("maxx".match(word)) {
result = result.concat([[129,2]]);
}
if ("maxy".match(word)) {
result = result.concat([[129,2]]);
}
if ("may".match(word)) {
result = result.concat([[6,2],[13,22],[14,14],[18,2],[19,2],[41,9],[43,2],[45,2],[51,2],[66,4],[70,2],[81,2],[86,1],[92,2],[99,2],[108,2]]);
}
if ("meaning".match(word)) {
result = result.concat([[111,14],[114,14],[117,14],[120,14],[123,14],[126,14],[129,14]]);
}
if ("meanings".match(word)) {
result = result.concat([[13,2]]);
}
if ("means".match(word)) {
result = result.concat([[41,2],[51,2],[56,2],[103,2],[113,1]]);
}
if ("measure".match(word)) {
result = result.concat([[13,2],[86,1]]);
}
if ("measurement".match(word)) {
result = result.concat([[13,2]]);
}
if ("mechanism".match(word)) {
result = result.concat([[9,1],[41,2],[49,7]]);
}
if ("mediates".match(word)) {
result = result.concat([[108,2]]);
}
if ("member".match(word)) {
result = result.concat([[47,1],[55,2],[60,2]]);
}
if ("memory".match(word)) {
result = result.concat([[16,15],[18,4],[20,2],[50,2],[86,4],[87,8],[89,3],[113,1]]);
}
if ("mentioned".match(word)) {
result = result.concat([[23,2]]);
}
if ("menu".match(word)) {
result = result.concat([[22,2],[23,5]]);
}
if ("merge".match(word)) {
result = result.concat([[113,2]]);
}
if ("message".match(word)) {
result = result.concat([[18,4],[19,32],[99,2]]);
}
if ("messages".match(word)) {
result = result.concat([[18,6],[19,7]]);
}
if ("metadata".match(word)) {
result = result.concat([[6,2],[13,4],[41,2],[46,2],[50,12],[51,4],[66,4],[92,1],[100,1],[116,1]]);
}
if ("metafile".match(word)) {
result = result.concat([[10,1],[128,2]]);
}
if ("meter".match(word)) {
result = result.concat([[92,2]]);
}
if ("meters".match(word)) {
result = result.concat([[92,2]]);
}
if ("method".match(word)) {
result = result.concat([[7,2],[13,4],[16,11],[22,2],[23,20],[27,7],[29,7],[31,7],[33,7],[35,7],[37,7],[39,7],[43,9],[46,3],[47,1],[48,1],[49,6],[50,8],[51,2],[52,2],[55,7],[56,4],[57,4],[60,4],[71,2],[72,2],[111,14],[114,14],[117,14],[120,14],[123,14],[126,14],[129,14]]);
}
if ("methods".match(word)) {
result = result.concat([[7,2],[16,4],[19,2],[23,7],[25,4],[43,3],[47,1],[60,2],[64,2],[89,3],[91,1],[111,14],[114,14],[117,14],[120,14],[123,14],[126,14],[129,14]]);
}
if ("methods:".match(word)) {
result = result.concat([[47,2],[88,2]]);
}
if ("metres".match(word)) {
result = result.concat([[14,2],[66,2]]);
}
if ("microsoft".match(word)) {
result = result.concat([[77,2],[99,2],[116,1]]);
}
if ("min".match(word)) {
result = result.concat([[114,1],[117,1],[120,1],[123,1]]);
}
if ("minimal".match(word)) {
result = result.concat([[6,2]]);
}
if ("minimum".match(word)) {
result = result.concat([[105,2],[106,2],[107,2],[108,2],[119,1]]);
}
if ("minoccurs".match(word)) {
result = result.concat([[51,4]]);
}
if ("minus".match(word)) {
result = result.concat([[78,2]]);
}
if ("minutes".match(word)) {
result = result.concat([[92,2]]);
}
if ("minx".match(word)) {
result = result.concat([[129,2]]);
}
if ("miny".match(word)) {
result = result.concat([[129,2]]);
}
if ("mm".match(word)) {
result = result.concat([[80,4]]);
}
if ("model".match(word)) {
result = result.concat([[6,4],[7,2],[10,2],[13,6],[41,2],[47,4],[48,5],[49,6],[116,1],[122,1]]);
}
if ("model:".match(word)) {
result = result.concat([[48,2]]);
}
if ("modeled".match(word)) {
result = result.concat([[13,2]]);
}
if ("models".match(word)) {
result = result.concat([[9,1],[13,2],[41,2],[47,5],[129,2]]);
}
if ("modification".match(word)) {
result = result.concat([[7,2],[114,1],[120,1],[123,1]]);
}
if ("modified".match(word)) {
result = result.concat([[48,6]]);
}
if ("modify".match(word)) {
result = result.concat([[14,2],[63,2]]);
}
if ("modifying".match(word)) {
result = result.concat([[43,1],[47,5]]);
}
if ("msg".match(word)) {
result = result.concat([[18,4],[19,2]]);
}
if ("multi".match(word)) {
result = result.concat([[107,2],[111,1],[114,1],[116,2],[117,1],[122,1],[123,1],[128,2]]);
}
if ("multicurvepolygon".match(word)) {
result = result.concat([[13,1],[83,8],[87,4],[90,2],[120,1],[126,1]]);
}
if ("multicurvepolygon:".match(word)) {
result = result.concat([[61,2]]);
}
if ("multicurvestring".match(word)) {
result = result.concat([[13,1],[83,8],[87,4],[90,2],[120,1],[126,1]]);
}
if ("multicurvestring:".match(word)) {
result = result.concat([[61,2]]);
}
if ("multigeoemtry".match(word)) {
result = result.concat([[126,1]]);
}
if ("multigeometry".match(word)) {
result = result.concat([[13,1],[87,4],[90,2],[120,1]]);
}
if ("multigeometry:".match(word)) {
result = result.concat([[61,2]]);
}
if ("multilinestring".match(word)) {
result = result.concat([[13,1],[83,6],[87,4],[90,2],[110,4],[111,1],[114,1],[120,1],[123,1],[126,1]]);
}
if ("multilinestring:".match(word)) {
result = result.concat([[61,2]]);
}
if ("multipart".match(word)) {
result = result.concat([[110,4]]);
}
if ("multiple".match(word)) {
result = result.concat([[7,2],[10,1],[13,5],[18,2],[22,1],[41,2],[43,2],[45,1],[46,2],[55,1],[108,4],[111,2],[114,2],[117,1],[119,2],[123,2],[125,2],[126,2]]);
}
if ("multiply".match(word)) {
result = result.concat([[72,2],[81,4]]);
}
if ("multipoint".match(word)) {
result = result.concat([[13,1],[83,6],[87,4],[90,2],[111,1],[114,1],[120,1],[123,1],[126,1]]);
}
if ("multipoint:".match(word)) {
result = result.concat([[61,2]]);
}
if ("multipolygon".match(word)) {
result = result.concat([[13,1],[83,6],[87,4],[90,2],[110,4],[111,1],[114,1],[120,1],[126,1]]);
}
if ("multipolygon:".match(word)) {
result = result.concat([[61,2]]);
}
if ("multiversion".match(word)) {
result = result.concat([[107,2]]);
}
if ("myclass".match(word)) {
result = result.concat([[16,4],[18,4]]);
}
if ("myisam".match(word)) {
result = result.concat([[113,3]]);
}
if ("mysql".match(word)) {
result = result.concat([[2,1],[3,1],[7,4],[9,2],[10,3],[13,8],[22,5],[23,14],[112,7],[113,18],[114,5]]);
}
if ("n".match(word)) {
result = result.concat([[19,2],[41,2],[50,2],[109,4]]);
}
if ("name".match(word)) {
result = result.concat([[9,1],[13,14],[19,2],[22,13],[23,38],[41,5],[46,2],[50,18],[51,55],[52,34],[55,8],[56,12],[61,4],[64,10],[76,2],[105,2],[108,10],[110,34],[111,11],[114,10],[116,2],[117,10],[120,10],[123,10],[126,10],[129,10]]);
}
if ("name/column".match(word)) {
result = result.concat([[110,2]]);
}
if ("name:".match(word)) {
result = result.concat([[111,1],[114,1],[117,1],[120,1],[123,1],[126,1]]);
}
if ("named".match(word)) {
result = result.concat([[13,2],[64,2],[110,6],[116,1]]);
}
if ("namely".match(word)) {
result = result.concat([[110,2]]);
}
if ("names".match(word)) {
result = result.concat([[19,2],[22,2],[23,11],[41,2],[46,4],[49,2],[52,2],[64,2],[107,2],[108,2]]);
}
if ("names:".match(word)) {
result = result.concat([[73,2]]);
}
if ("namespace".match(word)) {
result = result.concat([[50,2]]);
}
if ("name            ".match(word)) {
result = result.concat([[105,2],[106,2],[107,2]]);
}
if ("national".match(word)) {
result = result.concat([[19,2]]);
}
if ("native".match(word)) {
result = result.concat([[9,2],[100,1]]);
}
if ("natural".match(word)) {
result = result.concat([[13,2]]);
}
if ("navigate".match(word)) {
result = result.concat([[41,2]]);
}
if ("navigation".match(word)) {
result = result.concat([[41,2]]);
}
if ("ndb".match(word)) {
result = result.concat([[113,1]]);
}
if ("ne".match(word)) {
result = result.concat([[71,2],[81,4]]);
}
if ("nearest".match(word)) {
result = result.concat([[91,2]]);
}
if ("neatline".match(word)) {
result = result.concat([[110,2]]);
}
if ("neatlines".match(word)) {
result = result.concat([[110,6]]);
}
if ("necessarily".match(word)) {
result = result.concat([[42,2]]);
}
if ("necessary".match(word)) {
result = result.concat([[110,2]]);
}
if ("need".match(word)) {
result = result.concat([[11,2],[12,2],[16,4],[18,4],[22,4],[23,1],[46,3],[75,2],[77,2],[80,1],[86,1]]);
}
if ("needed".match(word)) {
result = result.concat([[13,4],[23,2],[56,2],[61,2],[113,2]]);
}
if ("needs".match(word)) {
result = result.concat([[16,2]]);
}
if ("negate".match(word)) {
result = result.concat([[72,2],[81,4]]);
}
if ("nest".match(word)) {
result = result.concat([[18,2]]);
}
if ("net".match(word)) {
result = result.concat([[22,1],[86,1]]);
}
if ("net/gml".match(word)) {
result = result.concat([[52,2]]);
}
if ("net/gml”".match(word)) {
result = result.concat([[50,2],[51,8]]);
}
if ("netmeeting".match(word)) {
result = result.concat([[110,1]]);
}
if ("network".match(word)) {
result = result.concat([[99,2]]);
}
if ("networks".match(word)) {
result = result.concat([[102,1]]);
}
if ("new".match(word)) {
result = result.concat([[3,9],[9,1],[14,4],[48,2],[56,4],[64,2],[110,2],[125,1]]);
}
if ("newer".match(word)) {
result = result.concat([[102,2]]);
}
if ("newly".match(word)) {
result = result.concat([[52,2]]);
}
if ("nls".match(word)) {
result = result.concat([[19,2]]);
}
if ("nlsid".match(word)) {
result = result.concat([[19,2]]);
}
if ("nlsmsgget1".match(word)) {
result = result.concat([[19,2]]);
}
if ("nlspath".match(word)) {
result = result.concat([[19,8]]);
}
if ("no".match(word)) {
result = result.concat([[3,2],[10,1],[13,2],[16,2],[23,2],[25,1],[44,2],[45,2],[46,3],[51,3],[64,4],[90,1],[104,2],[105,1],[111,2],[114,2],[117,2],[120,2],[123,2],[125,1],[126,4],[129,2]]);
}
if ("non".match(word)) {
result = result.concat([[3,7],[6,2],[10,2],[13,10],[14,2],[20,7],[41,2],[45,2],[46,13],[50,2],[60,8],[83,2],[92,4],[119,1],[125,1],[128,1]]);
}
if ("none".match(word)) {
result = result.concat([[14,1],[45,1],[61,2],[87,2],[90,2],[93,2],[105,3],[106,2]]);
}
if ("none:".match(word)) {
result = result.concat([[61,2]]);
}
if ("normal".match(word)) {
result = result.concat([[49,2],[93,2]]);
}
if ("notation".match(word)) {
result = result.concat([[67,2],[68,2],[71,4],[72,2],[87,9]]);
}
if ("note".match(word)) {
result = result.concat([[10,2]]);
}
if ("notequalto".match(word)) {
result = result.concat([[71,2],[81,2]]);
}
if ("nothing".match(word)) {
result = result.concat([[13,2],[92,2]]);
}
if ("np".match(word)) {
result = result.concat([[87,2]]);
}
if ("nr".match(word)) {
result = result.concat([[87,2]]);
}
if ("null".match(word)) {
result = result.concat([[16,16],[18,2],[23,4],[45,1],[51,2],[52,2],[61,12],[71,2],[72,2],[73,2],[111,2],[114,2],[117,2],[120,2],[123,2],[126,1]]);
}
if ("nullcondition".match(word)) {
result = result.concat([[71,4]]);
}
if ("num".match(word)) {
result = result.concat([[110,4]]);
}
if ("numargumentdefinitions".match(word)) {
result = result.concat([[33,4]]);
}
if ("number".match(word)) {
result = result.concat([[3,3],[11,2],[13,4],[19,8],[22,2],[41,2],[55,2],[69,2],[87,6],[92,2],[109,10]]);
}
if ("numberlanes".match(word)) {
result = result.concat([[13,4]]);
}
if ("numbers".match(word)) {
result = result.concat([[79,2],[82,2]]);
}
if ("numbluelakeshorelineordinates".match(word)) {
result = result.concat([[55,4]]);
}
if ("numcommands".match(word)) {
result = result.concat([[23,6]]);
}
if ("numconditiontypes".match(word)) {
result = result.concat([[35,4]]);
}
if ("numcurvestrings".match(word)) {
result = result.concat([[87,4]]);
}
if ("numdeleted".match(word)) {
result = result.concat([[57,4]]);
}
if ("numdistanceoperations".match(word)) {
result = result.concat([[35,2]]);
}
if ("numelements".match(word)) {
result = result.concat([[87,10]]);
}
if ("numexpressiontypes".match(word)) {
result = result.concat([[33,2]]);
}
if ("numfunctiondefinitions".match(word)) {
result = result.concat([[33,6]]);
}
if ("numgeom".match(word)) {
result = result.concat([[87,4]]);
}
if ("numgeometrycomponenttypes".match(word)) {
result = result.concat([[37,2]]);
}
if ("numgeometrycomponnenttypes".match(word)) {
result = result.concat([[37,2]]);
}
if ("numgeometrytypes".match(word)) {
result = result.concat([[37,4]]);
}
if ("numgooseislandshorelineordinates".match(word)) {
result = result.concat([[55,4]]);
}
if ("numinteriorrings".match(word)) {
result = result.concat([[61,4]]);
}
if ("numlinestrings".match(word)) {
result = result.concat([[87,4]]);
}
if ("numlocktypes".match(word)) {
result = result.concat([[27,4]]);
}
if ("numpoints".match(word)) {
result = result.concat([[87,4]]);
}
if ("numpolygons".match(word)) {
result = result.concat([[87,6]]);
}
if ("numpositions".match(word)) {
result = result.concat([[61,4]]);
}
if ("numproperties".match(word)) {
result = result.concat([[61,4]]);
}
if ("numpts".match(word)) {
result = result.concat([[87,8]]);
}
if ("numrings".match(word)) {
result = result.concat([[87,8]]);
}
if ("numspatialcontexts".match(word)) {
result = result.concat([[27,4]]);
}
if ("numspatialoperations".match(word)) {
result = result.concat([[35,6]]);
}
if ("numupdated".match(word)) {
result = result.concat([[56,4]]);
}
if ("o".match(word)) {
result = result.concat([[105,4],[106,4],[107,4],[109,2],[110,80]]);
}
if ("object".match(word)) {
result = result.concat([[7,4],[9,1],[10,2],[13,39],[14,8],[16,19],[22,2],[23,18],[25,2],[27,2],[29,2],[31,2],[33,2],[35,2],[37,2],[39,2],[41,18],[43,3],[46,12],[48,4],[50,4],[52,16],[55,47],[56,16],[57,2],[61,12],[63,2],[89,4],[90,1],[107,2],[111,42],[114,43],[116,2],[117,42],[120,42],[123,42],[126,43],[129,42]]);
}
if ("object:".match(word)) {
result = result.concat([[25,2],[87,2]]);
}
if ("objectclass".match(word)) {
result = result.concat([[46,2]]);
}
if ("objectproperties".match(word)) {
result = result.concat([[45,4]]);
}
if ("objectproperty".match(word)) {
result = result.concat([[45,11],[46,18],[111,1],[123,1]]);
}
if ("objects".match(word)) {
result = result.concat([[0,2],[2,1],[8,2],[9,4],[10,2],[13,8],[14,14],[16,2],[17,2],[22,5],[41,6],[43,2],[46,2],[55,10],[57,4],[63,4],[65,2],[66,8],[85,2],[89,2],[96,2],[105,1],[110,2],[113,3],[116,5],[119,2],[122,2],[125,2],[128,2]]);
}
if ("objects:".match(word)) {
result = result.concat([[46,2]]);
}
if ("objects      :".match(word)) {
result = result.concat([[105,2],[106,2],[107,2]]);
}
if ("objects”".match(word)) {
result = result.concat([[107,1]]);
}
if ("object’s".match(word)) {
result = result.concat([[16,2],[23,4],[56,4],[57,4]]);
}
if ("obtain".match(word)) {
result = result.concat([[23,4],[46,8],[55,1],[110,2]]);
}
if ("obtains".match(word)) {
result = result.concat([[46,3]]);
}
if ("occasionally".match(word)) {
result = result.concat([[128,2]]);
}
if ("odbc".match(word)) {
result = result.concat([[2,1],[3,1],[10,2],[115,7],[116,20],[117,5]]);
}
if ("often".match(word)) {
result = result.concat([[44,2]]);
}
if ("ogc".match(word)) {
result = result.concat([[10,2],[50,2],[51,12],[52,2],[87,2],[88,2],[125,2],[128,3]]);
}
if ("ogc980461fs".match(word)) {
result = result.concat([[110,72]]);
}
if ("ok".match(word)) {
result = result.concat([[23,1]]);
}
if ("old".match(word)) {
result = result.concat([[56,2]]);
}
if ("ole/com".match(word)) {
result = result.concat([[86,2]]);
}
if ("omit".match(word)) {
result = result.concat([[110,4]]);
}
if ("once".match(word)) {
result = result.concat([[23,1],[41,2]]);
}
if ("onto".match(word)) {
result = result.concat([[13,2]]);
}
if ("opaque".match(word)) {
result = result.concat([[125,2]]);
}
if ("open".match(word)) {
result = result.concat([[3,1],[10,1],[23,15],[50,2],[51,2],[52,2],[108,12],[128,2]]);
}
if ("opengis".match(word)) {
result = result.concat([[9,1],[10,2],[41,2],[50,2],[51,8],[52,2],[86,6],[110,6],[125,2]]);
}
if ("operate".match(word)) {
result = result.concat([[64,4],[110,4]]);
}
if ("operating".match(word)) {
result = result.concat([[110,2]]);
}
if ("operation".match(word)) {
result = result.concat([[14,2],[49,2],[55,4],[56,6],[57,2],[61,2],[64,6],[81,2],[99,2]]);
}
if ("operational".match(word)) {
result = result.concat([[2,1],[12,1],[14,5]]);
}
if ("operations".match(word)) {
result = result.concat([[7,2],[10,1],[12,1],[14,2],[41,2],[43,2],[54,7],[55,1],[64,8],[81,9],[89,1],[111,2],[114,1],[117,1],[120,1],[123,1],[125,2],[128,2]]);
}
if ("operator".match(word)) {
result = result.concat([[72,1],[81,5]]);
}
if ("operators".match(word)) {
result = result.concat([[69,1],[72,2],[81,7]]);
}
if ("opposed".match(word)) {
result = result.concat([[3,1]]);
}
if ("optimistic".match(word)) {
result = result.concat([[41,1]]);
}
if ("option".match(word)) {
result = result.concat([[64,2],[110,35]]);
}
if ("optional".match(word)) {
result = result.concat([[23,1],[43,2],[51,2],[67,2],[68,2],[78,2],[79,2],[83,2],[88,2],[108,2],[110,2],[116,1]]);
}
if ("optionally".match(word)) {
result = result.concat([[10,1],[116,2],[122,1]]);
}
if ("oracle".match(word)) {
result = result.concat([[10,1],[22,2],[25,2],[62,1],[63,2],[96,2],[99,2],[108,2],[109,3],[110,12]]);
}
if ("oracle”".match(word)) {
result = result.concat([[22,1]]);
}
if ("order".match(word)) {
result = result.concat([[13,2],[18,2],[22,4],[43,2],[45,2],[46,6],[55,2],[66,2],[67,2],[86,2],[99,2]]);
}
if ("ordered".match(word)) {
result = result.concat([[45,2],[46,3]]);
}
if ("orderedcollection".match(word)) {
result = result.concat([[45,3]]);
}
if ("ordering".match(word)) {
result = result.concat([[114,1],[117,1]]);
}
if ("ordinate".match(word)) {
result = result.concat([[13,12],[91,1]]);
}
if ("ordinates".match(word)) {
result = result.concat([[55,4],[61,2],[83,1],[92,4],[93,2]]);
}
if ("org".match(word)) {
result = result.concat([[3,1],[10,1],[86,2]]);
}
if ("org/2001/xmlschema".match(word)) {
result = result.concat([[52,2]]);
}
if ("org/2001/xmlschema”".match(word)) {
result = result.concat([[50,2],[51,8]]);
}
if ("org/isd/schema”".match(word)) {
result = result.concat([[50,2]]);
}
if ("org/schema".match(word)) {
result = result.concat([[50,2],[52,6]]);
}
if ("org/schema”".match(word)) {
result = result.concat([[51,9]]);
}
if ("org/schema”/".match(word)) {
result = result.concat([[51,4]]);
}
if ("organization".match(word)) {
result = result.concat([[86,2]]);
}
if ("organized".match(word)) {
result = result.concat([[2,5]]);
}
if ("oriented".match(word)) {
result = result.concat([[7,2],[41,2],[89,2]]);
}
if ("original".match(word)) {
result = result.concat([[50,2],[86,2],[103,4]]);
}
if ("originates".match(word)) {
result = result.concat([[88,2]]);
}
if ("originating".match(word)) {
result = result.concat([[128,2]]);
}
if ("osgeo".match(word)) {
result = result.concat([[3,11],[7,2],[9,6],[10,8],[13,4],[22,4],[23,2],[50,4],[51,13],[52,6],[62,1],[95,7],[112,7],[113,2],[115,7],[116,2],[118,7],[119,2],[121,7],[122,2],[124,7],[127,7],[128,2]]);
}
if ("others".match(word)) {
result = result.concat([[6,2],[7,2],[66,2]]);
}
if ("otherwise".match(word)) {
result = result.concat([[23,3],[108,4]]);
}
if ("outer".match(word)) {
result = result.concat([[13,2],[55,4]]);
}
if ("output".match(word)) {
result = result.concat([[105,2],[106,2],[107,2]]);
}
if ("outside".match(word)) {
result = result.concat([[116,1]]);
}
if ("overhead".match(word)) {
result = result.concat([[119,1]]);
}
if ("overlaps".match(word)) {
result = result.concat([[71,2],[73,2],[111,1]]);
}
if ("overridden".match(word)) {
result = result.concat([[13,2]]);
}
if ("override".match(word)) {
result = result.concat([[13,11]]);
}
if ("overrides".match(word)) {
result = result.concat([[3,7],[13,2],[42,13],[114,1],[117,1],[123,1],[129,1]]);
}
if ("overview".match(word)) {
result = result.concat([[2,1],[9,2],[10,2]]);
}
if ("owned".match(word)) {
result = result.concat([[13,2],[56,2]]);
}
if ("owner".match(word)) {
result = result.concat([[68,4]]);
}
if ("owner            :".match(word)) {
result = result.concat([[105,2],[106,2],[107,2]]);
}
if ("owning".match(word)) {
result = result.concat([[13,2],[48,3]]);
}
if ("p".match(word)) {
result = result.concat([[20,4],[105,2],[106,2],[107,2],[110,72]]);
}
if ("p2".match(word)) {
result = result.concat([[20,2]]);
}
if ("package".match(word)) {
result = result.concat([[9,4],[18,4],[41,7],[60,4],[85,1]]);
}
if ("packages".match(word)) {
result = result.concat([[9,11]]);
}
if ("packages:".match(word)) {
result = result.concat([[9,2],[69,2]]);
}
if ("packaging".match(word)) {
result = result.concat([[9,2]]);
}
if ("page".match(word)) {
result = result.concat([[51,2],[52,2]]);
}
if ("pair".match(word)) {
result = result.concat([[55,2]]);
}
if ("pairs".match(word)) {
result = result.concat([[110,2]]);
}
if ("palette/8".match(word)) {
result = result.concat([[129,1]]);
}
if ("parameter".match(word)) {
result = result.concat([[23,4],[72,6],[76,5],[108,4],[114,2]]);
}
if ("parameterization".match(word)) {
result = result.concat([[111,1]]);
}
if ("parameters".match(word)) {
result = result.concat([[13,8],[14,2],[22,4],[66,2],[76,4],[80,1],[82,2],[92,4]]);
}
if ("parametrically".match(word)) {
result = result.concat([[13,2]]);
}
if ("parcel".match(word)) {
result = result.concat([[13,4]]);
}
if ("parcels".match(word)) {
result = result.concat([[68,4]]);
}
if ("parent".match(word)) {
result = result.concat([[41,4]]);
}
if ("parenthesis".match(word)) {
result = result.concat([[82,4]]);
}
if ("parenting".match(word)) {
result = result.concat([[41,9]]);
}
if ("parsed".match(word)) {
result = result.concat([[80,2]]);
}
if ("parsing".match(word)) {
result = result.concat([[83,1]]);
}
if ("part".match(word)) {
result = result.concat([[7,2],[92,2],[96,2],[110,2]]);
}
if ("partial".match(word)) {
result = result.concat([[64,4]]);
}
if ("particular".match(word)) {
result = result.concat([[2,1],[9,1],[10,2],[13,10],[14,6],[22,4],[24,2],[25,2],[66,2],[86,1],[91,2],[96,4],[110,2],[113,2],[116,2],[119,2],[122,2],[125,2],[128,2]]);
}
if ("parts".match(word)) {
result = result.concat([[110,2]]);
}
if ("party".match(word)) {
result = result.concat([[116,1]]);
}
if ("pass".match(word)) {
result = result.concat([[16,1],[55,1]]);
}
if ("passing".match(word)) {
result = result.concat([[41,2],[55,1],[61,2]]);
}
if ("password".match(word)) {
result = result.concat([[23,11],[108,4],[110,4]]);
}
if ("path".match(word)) {
result = result.concat([[22,1],[99,2]]);
}
if ("pattern".match(word)) {
result = result.concat([[50,8],[51,2],[60,2]]);
}
if ("pattern:".match(word)) {
result = result.concat([[51,2]]);
}
if ("pavementtype".match(word)) {
result = result.concat([[13,2],[14,2],[67,2]]);
}
if ("pbase".match(word)) {
result = result.concat([[16,8],[18,10]]);
}
if ("pclassdef".match(word)) {
result = result.concat([[20,8]]);
}
if ("pclassdef:".match(word)) {
result = result.concat([[20,2]]);
}
if ("pdf".match(word)) {
result = result.concat([[0,1],[86,2],[113,2],[116,2],[119,2],[122,2],[125,2],[128,2]]);
}
if ("pe91".match(word)) {
result = result.concat([[99,2]]);
}
if ("peer".match(word)) {
result = result.concat([[13,4]]);
}
if ("pending".match(word)) {
result = result.concat([[23,6]]);
}
if ("per".match(word)) {
result = result.concat([[13,4],[46,2],[50,2],[51,2],[111,1],[114,1],[117,1],[120,1],[123,1],[126,1],[129,1]]);
}
if ("perform".match(word)) {
result = result.concat([[2,1],[9,1],[11,2],[14,4],[59,2],[60,2]]);
}
if ("performance".match(word)) {
result = result.concat([[10,1],[119,2]]);
}
if ("performed".match(word)) {
result = result.concat([[41,2],[64,2]]);
}
if ("performing".match(word)) {
result = result.concat([[59,5],[92,1]]);
}
if ("perimeter".match(word)) {
result = result.concat([[51,4]]);
}
if ("perimeter”".match(word)) {
result = result.concat([[51,1]]);
}
if ("permanent".match(word)) {
result = result.concat([[14,2],[63,2]]);
}
if ("permanently".match(word)) {
result = result.concat([[14,2],[63,2]]);
}
if ("permissible".match(word)) {
result = result.concat([[13,2]]);
}
if ("permissions".match(word)) {
result = result.concat([[8,2]]);
}
if ("permit".match(word)) {
result = result.concat([[23,1],[104,2]]);
}
if ("permits".match(word)) {
result = result.concat([[13,2],[104,2]]);
}
if ("persistent".match(word)) {
result = result.concat([[3,1],[14,5]]);
}
if ("personal".match(word)) {
result = result.concat([[22,1],[119,2]]);
}
if ("perspective".match(word)) {
result = result.concat([[6,5],[7,5]]);
}
if ("pertains".match(word)) {
result = result.concat([[91,1]]);
}
if ("pexception".match(word)) {
result = result.concat([[19,2]]);
}
if ("pfeatschema".match(word)) {
result = result.concat([[19,2]]);
}
if ("physical".match(word)) {
result = result.concat([[3,14],[13,4],[41,13],[42,4],[46,2],[116,1]]);
}
if ("pictorial".match(word)) {
result = result.concat([[128,2]]);
}
if ("pictorially".match(word)) {
result = result.concat([[128,1]]);
}
if ("piece".match(word)) {
result = result.concat([[86,2]]);
}
if ("pieces".match(word)) {
result = result.concat([[86,2]]);
}
if ("pipe".match(word)) {
result = result.concat([[68,2]]);
}
if ("pipenetworks".match(word)) {
result = result.concat([[68,2]]);
}
if ("pipes".match(word)) {
result = result.concat([[68,2]]);
}
if ("pixel".match(word)) {
result = result.concat([[13,2]]);
}
if ("pixels".match(word)) {
result = result.concat([[13,4]]);
}
if ("places".match(word)) {
result = result.concat([[110,6]]);
}
if ("placing".match(word)) {
result = result.concat([[48,1]]);
}
if ("platform".match(word)) {
result = result.concat([[116,2],[122,1],[128,2]]);
}
if ("platforms".match(word)) {
result = result.concat([[119,1]]);
}
if ("plus".match(word)) {
result = result.concat([[13,2],[78,2]]);
}
if ("pnewexception".match(word)) {
result = result.concat([[19,2]]);
}
if ("png".match(word)) {
result = result.concat([[10,1],[128,3]]);
}
if ("point".match(word)) {
result = result.concat([[13,9],[51,2],[52,2],[55,6],[56,2],[61,4],[79,5],[83,34],[87,10],[90,2],[91,4],[93,4],[110,6],[111,1],[114,1],[117,2],[120,1],[123,1],[126,1]]);
}
if ("point:".match(word)) {
result = result.concat([[61,2]]);
}
if ("pointcollection".match(word)) {
result = result.concat([[83,10]]);
}
if ("pointed".match(word)) {
result = result.concat([[16,4]]);
}
if ("pointentity".match(word)) {
result = result.concat([[83,4]]);
}
if ("pointer".match(word)) {
result = result.concat([[16,6],[20,16],[52,10],[55,3],[56,2]]);
}
if ("pointers".match(word)) {
result = result.concat([[20,2]]);
}
if ("pointone".match(word)) {
result = result.concat([[93,6]]);
}
if ("points".match(word)) {
result = result.concat([[13,3],[16,2],[61,2],[86,1],[87,4]]);
}
if ("pointtwo".match(word)) {
result = result.concat([[93,6]]);
}
if ("pole".match(word)) {
result = result.concat([[13,4]]);
}
if ("polygon".match(word)) {
result = result.concat([[13,1],[51,7],[52,2],[55,14],[61,8],[83,10],[87,12],[90,2],[110,3],[111,1],[114,1],[120,1],[123,1],[126,1],[129,1]]);
}
if ("polygon:".match(word)) {
result = result.concat([[61,2]]);
}
if ("polygoncollection".match(word)) {
result = result.concat([[83,6]]);
}
if ("polygons".match(word)) {
result = result.concat([[13,1],[86,1],[87,6],[91,1]]);
}
if ("polylines".match(word)) {
result = result.concat([[86,1]]);
}
if ("ponds".match(word)) {
result = result.concat([[110,6]]);
}
if ("pooling".match(word)) {
result = result.concat([[89,1]]);
}
if ("pools".match(word)) {
result = result.concat([[89,1]]);
}
if ("populate".match(word)) {
result = result.concat([[23,2]]);
}
if ("populated".match(word)) {
result = result.concat([[46,4],[116,1]]);
}
if ("populating".match(word)) {
result = result.concat([[116,1]]);
}
if ("port".match(word)) {
result = result.concat([[108,2]]);
}
if ("portrayal".match(word)) {
result = result.concat([[128,2]]);
}
if ("port”".match(word)) {
result = result.concat([[108,2]]);
}
if ("position".match(word)) {
result = result.concat([[51,4],[61,20],[110,4]]);
}
if ("positional".match(word)) {
result = result.concat([[92,4]]);
}
if ("positions".match(word)) {
result = result.concat([[13,4]]);
}
if ("positionsize".match(word)) {
result = result.concat([[87,16]]);
}
if ("possibilities".match(word)) {
result = result.concat([[76,2]]);
}
if ("possible".match(word)) {
result = result.concat([[22,2],[23,1],[46,2],[109,2]]);
}
if ("possibly".match(word)) {
result = result.concat([[31,2]]);
}
if ("ppropdef".match(word)) {
result = result.concat([[20,4]]);
}
if ("pprops".match(word)) {
result = result.concat([[20,8]]);
}
if ("practices".match(word)) {
result = result.concat([[2,1],[15,9]]);
}
if ("pre".match(word)) {
result = result.concat([[22,1],[41,2],[116,1],[122,1],[128,1]]);
}
if ("precedence".match(word)) {
result = result.concat([[72,5],[81,7]]);
}
if ("preceding".match(word)) {
result = result.concat([[52,2],[57,2],[58,2]]);
}
if ("precision".match(word)) {
result = result.concat([[50,2],[79,1],[83,2],[114,1],[117,1],[120,1],[123,1],[126,1]]);
}
if ("preconditions".match(word)) {
result = result.concat([[55,5]]);
}
if ("predefine".match(word)) {
result = result.concat([[41,2]]);
}
if ("predefined".match(word)) {
result = result.concat([[41,2]]);
}
if ("preliminary".match(word)) {
result = result.concat([[23,2]]);
}
if ("prepared".match(word)) {
result = result.concat([[23,1]]);
}
if ("presence".match(word)) {
result = result.concat([[99,2]]);
}
if ("present".match(word)) {
result = result.concat([[23,5],[91,2],[99,4]]);
}
if ("presentation".match(word)) {
result = result.concat([[23,2]]);
}
if ("presented".match(word)) {
result = result.concat([[10,2]]);
}
if ("presents".match(word)) {
result = result.concat([[2,1]]);
}
if ("preserve".match(word)) {
result = result.concat([[13,2]]);
}
if ("preserves".match(word)) {
result = result.concat([[92,2]]);
}
if ("prevent".match(word)) {
result = result.concat([[16,2]]);
}
if ("prevents".match(word)) {
result = result.concat([[46,2]]);
}
if ("previous".match(word)) {
result = result.concat([[23,2],[55,2]]);
}
if ("previously".match(word)) {
result = result.concat([[3,1],[13,2]]);
}
if ("primary".match(word)) {
result = result.concat([[43,2],[51,2],[52,2],[54,2],[116,1]]);
}
if ("privileges".match(word)) {
result = result.concat([[110,2]]);
}
if ("privileges        :".match(word)) {
result = result.concat([[105,2],[106,2],[107,2]]);
}
if ("process".match(word)) {
result = result.concat([[13,4],[18,8],[22,2],[23,5],[41,2],[64,2],[93,2]]);
}
if ("processed".match(word)) {
result = result.concat([[49,2]]);
}
if ("processing".match(word)) {
result = result.concat([[2,1],[14,2],[62,8],[63,5]]);
}
if ("processor".match(word)) {
result = result.concat([[14,1]]);
}
if ("produced".match(word)) {
result = result.concat([[50,2]]);
}
if ("produces".match(word)) {
result = result.concat([[10,1],[128,2]]);
}
if ("products".match(word)) {
result = result.concat([[6,2],[10,1]]);
}
if ("program".match(word)) {
result = result.concat([[14,1],[23,2]]);
}
if ("programming".match(word)) {
result = result.concat([[0,2],[81,2]]);
}
if ("project".match(word)) {
result = result.concat([[110,4]]);
}
if ("prompted".match(word)) {
result = result.concat([[23,2],[110,2]]);
}
if ("propdef".match(word)) {
result = result.concat([[20,4]]);
}
if ("properly".match(word)) {
result = result.concat([[12,2]]);
}
if ("properties".match(word)) {
result = result.concat([[10,1],[13,25],[14,2],[23,14],[41,17],[44,2],[45,2],[46,8],[50,6],[51,4],[52,10],[55,4],[56,8],[60,2],[61,14],[67,2],[68,2],[91,2],[108,2],[110,4],[111,1],[114,4],[116,1],[117,2],[120,3],[122,1],[123,2],[126,3]]);
}
if ("properties:".match(word)) {
result = result.concat([[13,2],[14,2],[41,2],[55,2],[110,2]]);
}
if ("property".match(word)) {
result = result.concat([[3,1],[7,2],[10,1],[13,82],[14,2],[22,1],[23,24],[41,6],[43,6],[45,5],[46,19],[48,1],[51,20],[52,34],[55,65],[56,27],[57,5],[60,4],[61,13],[91,4],[94,2],[108,4],[110,6],[111,1],[114,1],[116,2],[117,1],[120,1],[122,2],[123,2],[126,2],[128,1],[129,1]]);
}
if ("propertydef".match(word)) {
result = result.concat([[61,8]]);
}
if ("propertyname".match(word)) {
result = result.concat([[50,8],[51,11],[61,4]]);
}
if ("propertytype".match(word)) {
result = result.concat([[61,4]]);
}
if ("proposed".match(word)) {
result = result.concat([[68,4]]);
}
if ("protected".match(word)) {
result = result.concat([[16,2],[23,1],[108,2]]);
}
if ("protocol".match(word)) {
result = result.concat([[108,2]]);
}
if ("provide".match(word)) {
result = result.concat([[6,2],[9,3],[13,2],[19,4],[23,4],[41,2],[42,2],[66,2],[108,2]]);
}
if ("provided".match(word)) {
result = result.concat([[16,4],[19,2],[25,2],[61,1],[89,2],[104,2],[110,1],[116,2],[122,2]]);
}
if ("provider".match(word)) {
result = result.concat([[1,2],[2,10],[3,13],[4,2],[7,6],[9,19],[10,17],[11,1],[13,12],[14,4],[21,2],[22,38],[23,18],[24,4],[25,12],[31,2],[41,14],[42,8],[49,2],[62,2],[64,1],[65,2],[66,4],[67,2],[69,1],[70,5],[76,2],[90,1],[95,7],[96,11],[97,5],[98,2],[99,6],[100,7],[101,2],[108,7],[110,8],[111,7],[112,7],[113,16],[114,7],[115,7],[116,20],[117,7],[118,7],[119,15],[120,7],[121,7],[122,20],[123,7],[124,7],[125,12],[126,7],[127,7],[128,19],[129,7]]);
}
if ("provider:".match(word)) {
result = result.concat([[22,2]]);
}
if ("providers".match(word)) {
result = result.concat([[2,1],[3,11],[7,2],[8,2],[9,10],[10,6],[13,2],[14,2],[22,8],[23,9],[41,11],[42,7],[44,2],[62,1],[66,2],[70,2]]);
}
if ("providers—oracle".match(word)) {
result = result.concat([[9,1]]);
}
if ("provides".match(word)) {
result = result.concat([[1,2],[2,2],[7,2],[8,2],[9,2],[10,3],[15,2],[21,2],[24,2],[41,6],[50,2],[53,2],[89,2],[96,8],[113,8],[116,7],[119,4],[122,6],[125,8],[128,7]]);
}
if ("providing".match(word)) {
result = result.concat([[16,2],[45,2],[92,2]]);
}
if ("psz".match(word)) {
result = result.concat([[20,6]]);
}
if ("ptr".match(word)) {
result = result.concat([[16,10]]);
}
if ("ptr”".match(word)) {
result = result.concat([[16,8]]);
}
if ("public".match(word)) {
result = result.concat([[10,1],[125,1]]);
}
if ("pure".match(word)) {
result = result.concat([[87,2]]);
}
if ("purpose".match(word)) {
result = result.concat([[1,5],[7,2],[23,2]]);
}
if ("put".match(word)) {
result = result.concat([[13,2],[92,2]]);
}
if ("qualified".match(word)) {
result = result.concat([[52,2],[55,2],[56,2]]);
}
if ("queried".match(word)) {
result = result.concat([[46,10]]);
}
if ("queries".match(word)) {
result = result.concat([[2,1],[6,2],[11,1],[41,2],[59,9],[60,8],[92,1],[113,1]]);
}
if ("query".match(word)) {
result = result.concat([[10,1],[13,2],[25,2],[41,2],[60,7],[61,15],[66,2],[83,2],[110,4],[125,1]]);
}
if ("querying".match(word)) {
result = result.concat([[7,2],[128,1]]);
}
if ("querypropertyname".match(word)) {
result = result.concat([[61,8]]);
}
if ("querypropertyvalue".match(word)) {
result = result.concat([[61,8]]);
}
if ("queryresults".match(word)) {
result = result.concat([[61,10]]);
}
if ("quite".match(word)) {
result = result.concat([[55,2]]);
}
if ("quote".match(word)) {
result = result.concat([[75,2],[77,4]]);
}
if ("quoted".match(word)) {
result = result.concat([[110,2]]);
}
if ("quotes".match(word)) {
result = result.concat([[75,2],[77,4]]);
}
if ("ram".match(word)) {
result = result.concat([[113,1]]);
}
if ("range".match(word)) {
result = result.concat([[79,1],[114,2],[116,2],[120,2]]);
}
if ("raster".match(word)) {
result = result.concat([[13,13],[25,1],[38,5],[100,1],[111,8],[114,8],[117,8],[120,8],[123,8],[126,8],[128,1],[129,13]]);
}
if ("rastercapabilities".match(word)) {
result = result.concat([[39,12]]);
}
if ("rasterproperty".match(word)) {
result = result.concat([[111,1],[123,1]]);
}
if ("rather".match(word)) {
result = result.concat([[13,2],[83,1]]);
}
if ("raw".match(word)) {
result = result.concat([[109,2]]);
}
if ("rdbms".match(word)) {
result = result.concat([[3,5],[13,4],[41,2],[42,2],[46,2],[119,1],[122,1]]);
}
if ("re".match(word)) {
result = result.concat([[18,2]]);
}
if ("reaction".match(word)) {
result = result.concat([[48,2]]);
}
if ("reacts".match(word)) {
result = result.concat([[46,2]]);
}
if ("read".match(word)) {
result = result.concat([[10,3],[41,6],[45,1],[50,6],[52,7],[103,2],[116,1],[119,1],[122,1]]);
}
if ("read/write".match(word)) {
result = result.concat([[10,4]]);
}
if ("reader".match(word)) {
result = result.concat([[60,6],[61,1]]);
}
if ("readers".match(word)) {
result = result.concat([[64,2]]);
}
if ("readily".match(word)) {
result = result.concat([[6,2]]);
}
if ("readnext".match(word)) {
result = result.concat([[60,2],[61,2]]);
}
if ("readxml".match(word)) {
result = result.concat([[50,4],[51,2],[52,2]]);
}
if ("ready".match(word)) {
result = result.concat([[55,2]]);
}
if ("real".match(word)) {
result = result.concat([[13,6],[82,2]]);
}
if ("reason".match(word)) {
result = result.concat([[16,1],[22,2]]);
}
if ("receive".match(word)) {
result = result.concat([[110,1]]);
}
if ("records".match(word)) {
result = result.concat([[13,2]]);
}
if ("rectangular".match(word)) {
result = result.concat([[13,2],[92,2]]);
}
if ("rectilinear".match(word)) {
result = result.concat([[92,2]]);
}
if ("refer".match(word)) {
result = result.concat([[22,2],[99,2]]);
}
if ("reference".match(word)) {
result = result.concat([[9,4],[16,8],[18,2],[20,2],[27,7],[29,2],[31,2],[51,2],[58,2],[64,2],[111,14],[113,2],[114,14],[116,2],[117,14],[119,2],[120,14],[122,2],[123,14],[126,14],[128,2],[129,14]]);
}
if ("referenced".match(word)) {
result = result.concat([[10,1],[50,8],[128,2]]);
}
if ("references".match(word)) {
result = result.concat([[29,5],[31,5],[41,7],[99,2],[110,2]]);
}
if ("reference”".match(word)) {
result = result.concat([[20,4]]);
}
if ("referencing".match(word)) {
result = result.concat([[102,1]]);
}
if ("referred".match(word)) {
result = result.concat([[83,2]]);
}
if ("reflected".match(word)) {
result = result.concat([[9,2]]);
}
if ("reg".match(word)) {
result = result.concat([[105,2],[106,4],[107,4],[110,26]]);
}
if ("regarding".match(word)) {
result = result.concat([[13,2],[14,1]]);
}
if ("regardless".match(word)) {
result = result.concat([[128,1]]);
}
if ("register".match(word)) {
result = result.concat([[22,2]]);
}
if ("registered".match(word)) {
result = result.concat([[22,3]]);
}
if ("registration".match(word)) {
result = result.concat([[7,2],[104,3],[105,6],[106,6],[107,6],[110,2]]);
}
if ("registry".match(word)) {
result = result.concat([[22,12],[23,7]]);
}
if ("relate".match(word)) {
result = result.concat([[13,2],[14,2],[41,2],[73,2],[89,2]]);
}
if ("related".match(word)) {
result = result.concat([[2,1],[3,1],[13,8],[20,2],[25,2],[40,2],[58,5],[92,2],[95,2],[112,2],[115,2],[118,2],[121,2],[124,2],[127,2]]);
}
if ("relates".match(word)) {
result = result.concat([[91,2]]);
}
if ("relational".match(word)) {
result = result.concat([[6,2],[7,2],[10,1],[22,2],[55,2],[96,2],[116,1]]);
}
if ("relationship".match(word)) {
result = result.concat([[13,4],[45,4],[46,1]]);
}
if ("relationships".match(word)) {
result = result.concat([[10,2],[41,2],[67,2],[68,2],[102,1]]);
}
if ("relative".match(word)) {
result = result.concat([[102,5]]);
}
if ("relatively".match(word)) {
result = result.concat([[55,2]]);
}
if ("release".match(word)) {
result = result.concat([[10,3],[16,31],[18,6],[20,4],[54,1],[62,1],[64,2]]);
}
if ("released".match(word)) {
result = result.concat([[16,2],[20,2],[48,2]]);
}
if ("releaselock".match(word)) {
result = result.concat([[111,1]]);
}
if ("remain".match(word)) {
result = result.concat([[14,2]]);
}
if ("remains".match(word)) {
result = result.concat([[47,1]]);
}
if ("remotely".match(word)) {
result = result.concat([[110,1]]);
}
if ("remove".match(word)) {
result = result.concat([[47,1],[48,1]]);
}
if ("removed".match(word)) {
result = result.concat([[14,2],[47,2],[64,2]]);
}
if ("removes".match(word)) {
result = result.concat([[14,2],[49,2]]);
}
if ("removing".match(word)) {
result = result.concat([[48,2]]);
}
if ("rendered".match(word)) {
result = result.concat([[10,1],[128,2]]);
}
if ("rendering".match(word)) {
result = result.concat([[91,2]]);
}
if ("replace".match(word)) {
result = result.concat([[56,2],[77,2]]);
}
if ("replaced".match(word)) {
result = result.concat([[51,4]]);
}
if ("repository".match(word)) {
result = result.concat([[13,2]]);
}
if ("represent".match(word)) {
result = result.concat([[9,1],[13,16],[40,2],[41,2],[43,1],[86,1],[88,2],[90,1],[91,1]]);
}
if ("represented".match(word)) {
result = result.concat([[13,6]]);
}
if ("representing".match(word)) {
result = result.concat([[16,2],[83,2],[87,2]]);
}
if ("represents".match(word)) {
result = result.concat([[13,2],[14,2],[55,2],[63,2]]);
}
if ("reproduced".match(word)) {
result = result.concat([[51,2],[52,2]]);
}
if ("request".match(word)) {
result = result.concat([[23,2]]);
}
if ("requesting".match(word)) {
result = result.concat([[23,1]]);
}
if ("requests".match(word)) {
result = result.concat([[108,2]]);
}
if ("require".match(word)) {
result = result.concat([[3,2],[70,2]]);
}
if ("required".match(word)) {
result = result.concat([[3,2],[6,2],[13,2],[23,20],[41,1],[43,2],[45,2],[46,6],[51,2],[55,1],[99,10],[108,4]]);
}
if ("requirement".match(word)) {
result = result.concat([[16,2]]);
}
if ("requirements".match(word)) {
result = result.concat([[17,2],[97,5]]);
}
if ("requires".match(word)) {
result = result.concat([[44,2],[46,2]]);
}
if ("reregister".match(word)) {
result = result.concat([[22,2]]);
}
if ("resample".match(word)) {
result = result.concat([[129,1]]);
}
if ("research".match(word)) {
result = result.concat([[122,1]]);
}
if ("research/enseignement/analyseinfo/aboutbnf".match(word)) {
result = result.concat([[71,2]]);
}
if ("resemblance".match(word)) {
result = result.concat([[50,2]]);
}
if ("reserves".match(word)) {
result = result.concat([[25,1]]);
}
if ("resets".match(word)) {
result = result.concat([[64,1]]);
}
if ("reside".match(word)) {
result = result.concat([[52,2]]);
}
if ("resident".match(word)) {
result = result.concat([[108,2]]);
}
if ("resides".match(word)) {
result = result.concat([[13,4],[110,2]]);
}
if ("resolve".match(word)) {
result = result.concat([[61,2]]);
}
if ("resource".match(word)) {
result = result.concat([[19,8],[110,2]]);
}
if ("respect".match(word)) {
result = result.concat([[13,2]]);
}
if ("respective".match(word)) {
result = result.concat([[46,2]]);
}
if ("respectively".match(word)) {
result = result.concat([[41,2]]);
}
if ("response".match(word)) {
result = result.concat([[125,2]]);
}
if ("responsible".match(word)) {
result = result.concat([[41,2],[116,1]]);
}
if ("restrict".match(word)) {
result = result.concat([[13,2]]);
}
if ("restriction".match(word)) {
result = result.concat([[110,2]]);
}
if ("result".match(word)) {
result = result.concat([[20,2],[23,1],[43,2],[51,5],[61,2],[86,1]]);
}
if ("resulted".match(word)) {
result = result.concat([[23,1]]);
}
if ("resulting".match(word)) {
result = result.concat([[50,2]]);
}
if ("results".match(word)) {
result = result.concat([[41,2],[60,2],[61,5],[108,2]]);
}
if ("rethrow".match(word)) {
result = result.concat([[18,2]]);
}
if ("retrieval".match(word)) {
result = result.concat([[10,2],[23,1]]);
}
if ("retrieve".match(word)) {
result = result.concat([[14,4],[16,2],[22,2],[23,3],[25,2],[46,2],[59,2],[60,6],[64,4],[66,2],[125,2]]);
}
if ("retrieved".match(word)) {
result = result.concat([[25,2],[48,3],[49,2],[50,2]]);
}
if ("retrieves".match(word)) {
result = result.concat([[10,1],[61,2],[64,2]]);
}
if ("retrieving".match(word)) {
result = result.concat([[9,1],[23,1],[24,2]]);
}
if ("return".match(word)) {
result = result.concat([[3,1],[7,2],[16,1],[20,7],[23,3],[43,1],[46,2],[87,6],[91,1]]);
}
if ("returned".match(word)) {
result = result.concat([[18,2],[20,2],[22,2],[23,7],[46,2],[49,2],[55,2],[60,2],[64,2],[108,2]]);
}
if ("returns".match(word)) {
result = result.concat([[16,2],[23,3],[27,6],[29,6],[31,2],[33,2],[52,2],[55,2],[61,3],[64,2]]);
}
if ("reused".match(word)) {
result = result.concat([[55,1]]);
}
if ("reusing".match(word)) {
result = result.concat([[56,2]]);
}
if ("reveal".match(word)) {
result = result.concat([[23,2]]);
}
if ("revision".match(word)) {
result = result.concat([[41,2]]);
}
if ("revisionnumber".match(word)) {
result = result.concat([[41,2]]);
}
if ("revisions".match(word)) {
result = result.concat([[14,2],[63,2]]);
}
if ("rgb".match(word)) {
result = result.concat([[39,2]]);
}
if ("rgb/24".match(word)) {
result = result.concat([[129,1]]);
}
if ("rgba/32".match(word)) {
result = result.concat([[129,1]]);
}
if ("rgbrasterdatamodel".match(word)) {
result = result.concat([[39,4]]);
}
if ("rich".match(word)) {
result = result.concat([[6,2],[10,2]]);
}
if ("right".match(word)) {
result = result.concat([[25,1],[77,2],[82,2]]);
}
if ("ring".match(word)) {
result = result.concat([[13,2],[55,8],[61,4],[87,8],[120,2],[126,2]]);
}
if ("rings".match(word)) {
result = result.concat([[13,4],[55,6],[87,4]]);
}
if ("road".match(word)) {
result = result.concat([[13,16],[14,2],[46,16],[66,2],[110,6]]);
}
if ("roads".match(word)) {
result = result.concat([[67,2],[68,2]]);
}
if ("robust".match(word)) {
result = result.concat([[66,2]]);
}
if ("role".match(word)) {
result = result.concat([[1,2],[4,2]]);
}
if ("roll".match(word)) {
result = result.concat([[14,2]]);
}
if ("rollback".match(word)) {
result = result.concat([[3,1],[49,5],[64,5]]);
}
if ("rollbacklongtransaction".match(word)) {
result = result.concat([[64,2],[111,1]]);
}
if ("rolled".match(word)) {
result = result.concat([[64,2]]);
}
if ("root".match(word)) {
result = result.concat([[14,9],[23,4],[41,2],[51,4],[63,4],[64,5]]);
}
if ("roughly".match(word)) {
result = result.concat([[50,2],[55,6]]);
}
if ("round".match(word)) {
result = result.concat([[50,6]]);
}
if ("routes".match(word)) {
result = result.concat([[110,6]]);
}
if ("row".match(word)) {
result = result.concat([[55,2],[104,9],[105,9],[106,23],[107,15]]);
}
if ("rows".match(word)) {
result = result.concat([[104,1]]);
}
if ("row”".match(word)) {
result = result.concat([[56,2]]);
}
if ("rules".match(word)) {
result = result.concat([[13,2],[14,1],[70,2],[71,2]]);
}
if ("run".match(word)) {
result = result.concat([[99,2],[110,1],[116,2],[122,1],[128,2]]);
}
if ("running".match(word)) {
result = result.concat([[99,2],[110,5]]);
}
if ("runtime".match(word)) {
result = result.concat([[19,4],[99,2]]);
}
if ("s".match(word)) {
result = result.concat([[9,2],[22,1],[46,2],[52,8],[92,1],[119,2]]);
}
if ("s/he".match(word)) {
result = result.concat([[23,2]]);
}
if ("s\\n".match(word)) {
result = result.concat([[18,2]]);
}
if ("safe".match(word)) {
result = result.concat([[16,26],[18,4],[20,2]]);
}
if ("safely".match(word)) {
result = result.concat([[22,2]]);
}
if ("sample".match(word)) {
result = result.concat([[22,2],[50,10],[51,4],[52,28],[55,2],[61,5],[110,2]]);
}
if ("sampleapplyschema".match(word)) {
result = result.concat([[52,12]]);
}
if ("sampleclasscollection".match(word)) {
result = result.concat([[52,10]]);
}
if ("sampledelete".match(word)) {
result = result.concat([[57,10]]);
}
if ("sampledescribeschema".match(word)) {
result = result.concat([[52,6]]);
}
if ("sampledestroyschema".match(word)) {
result = result.concat([[52,8]]);
}
if ("samplefeatureclass".match(word)) {
result = result.concat([[52,24],[56,2],[61,8],[110,14]]);
}
if ("samplefeatureclassidentityproperties".match(word)) {
result = result.concat([[52,6]]);
}
if ("samplefeatureclasskey".match(word)) {
result = result.concat([[52,2]]);
}
if ("samplefeatureclassproperties".match(word)) {
result = result.concat([[52,10]]);
}
if ("samplefeatureclasstype".match(word)) {
result = result.concat([[52,2]]);
}
if ("samplefeaturereader".match(word)) {
result = result.concat([[55,4]]);
}
if ("samplefeatureschema".match(word)) {
result = result.concat([[52,35],[110,15]]);
}
if ("samplefeatureschema:samplefeatureclass".match(word)) {
result = result.concat([[55,2],[56,2],[57,2]]);
}
if ("samplefeatureschema:samplefeatureclasstype".match(word)) {
result = result.concat([[52,2]]);
}
if ("samplefeatureschemacollection".match(word)) {
result = result.concat([[52,14]]);
}
if ("samplegeometricproperty".match(word)) {
result = result.concat([[52,20],[110,3]]);
}
if ("samplegeometryfactory".match(word)) {
result = result.concat([[55,8]]);
}
if ("samplegeometryproperty".match(word)) {
result = result.concat([[55,2]]);
}
if ("samplegeometrypropertyvalue".match(word)) {
result = result.concat([[55,6]]);
}
if ("samplegeometryvalue".match(word)) {
result = result.concat([[55,6]]);
}
if ("sampleidentitydataproperty".match(word)) {
result = result.concat([[52,22],[55,2],[56,4],[57,2],[61,2],[110,5]]);
}
if ("sampleidentitydatavalue".match(word)) {
result = result.concat([[55,4]]);
}
if ("sampleidentitypropertyvalue".match(word)) {
result = result.concat([[55,6]]);
}
if ("sampleinsert".match(word)) {
result = result.concat([[55,10]]);
}
if ("samplenamedataproperty".match(word)) {
result = result.concat([[52,20],[55,2],[56,2],[110,3]]);
}
if ("samplenamedatavalue".match(word)) {
result = result.concat([[55,4],[56,6]]);
}
if ("samplenamepropertyvalue".match(word)) {
result = result.concat([[55,6],[56,6]]);
}
if ("samplepropertyvalues".match(word)) {
result = result.concat([[55,4],[56,6]]);
}
if ("samples".match(word)) {
result = result.concat([[25,1]]);
}
if ("sampleselect".match(word)) {
result = result.concat([[61,10]]);
}
if ("sampleupdate".match(word)) {
result = result.concat([[56,12]]);
}
if ("save".match(word)) {
result = result.concat([[22,2]]);
}
if ("say".match(word)) {
result = result.concat([[23,2]]);
}
if ("sc".match(word)) {
result = result.concat([[52,2]]);
}
if ("scalable".match(word)) {
result = result.concat([[10,1],[128,2]]);
}
if ("scale".match(word)) {
result = result.concat([[50,2],[114,1],[117,1],[120,1],[123,1],[126,1]]);
}
if ("schema".match(word)) {
result = result.concat([[2,1],[3,11],[6,2],[7,6],[9,7],[10,4],[11,1],[13,64],[18,4],[22,4],[24,2],[25,1],[28,5],[40,9],[41,65],[42,16],[43,19],[46,21],[47,2],[48,6],[50,32],[51,34],[52,59],[55,4],[110,31],[111,13],[114,15],[116,3],[117,14],[120,14],[122,2],[123,15],[126,13],[128,4],[129,13]]);
}
if ("schema:".match(word)) {
result = result.concat([[52,2]]);
}
if ("schemaattributedictionary".match(word)) {
result = result.concat([[48,2]]);
}
if ("schemacapabilities".match(word)) {
result = result.concat([[29,28]]);
}
if ("schemalocation".match(word)) {
result = result.concat([[50,2]]);
}
if ("schemaname".match(word)) {
result = result.concat([[41,4],[55,2],[56,2],[110,2]]);
}
if ("schemas".match(word)) {
result = result.concat([[2,1],[7,2],[9,1],[10,4],[13,4],[40,2],[41,2],[42,2],[43,5],[46,2],[50,6],[110,4],[111,1],[114,1],[117,1],[123,1],[126,1]]);
}
if ("schematic".match(word)) {
result = result.concat([[13,2],[92,2]]);
}
if ("scope".match(word)) {
result = result.concat([[14,2],[16,2],[60,2],[66,2],[114,1]]);
}
if ("screen".match(word)) {
result = result.concat([[128,2]]);
}
if ("sde".match(word)) {
result = result.concat([[106,5],[107,2],[110,3]]);
}
if ("sde91".match(word)) {
result = result.concat([[99,2]]);
}
if ("sdelayer".match(word)) {
result = result.concat([[110,33]]);
}
if ("sdetable".match(word)) {
result = result.concat([[105,4],[106,4],[107,4],[109,1],[110,56]]);
}
if ("sde    ".match(word)) {
result = result.concat([[108,2]]);
}
if ("sde”".match(word)) {
result = result.concat([[108,2]]);
}
if ("sdf".match(word)) {
result = result.concat([[2,1],[3,1],[9,3],[10,6],[22,3],[118,7],[119,19],[120,5]]);
}
if ("sdfcommandtype".match(word)) {
result = result.concat([[120,2],[123,1]]);
}
if ("sdf”".match(word)) {
result = result.concat([[22,1]]);
}
if ("sdk".match(word)) {
result = result.concat([[0,1],[9,2],[22,3],[50,2],[98,4],[99,6]]);
}
if ("seamlessly".match(word)) {
result = result.concat([[6,2]]);
}
if ("search".match(word)) {
result = result.concat([[46,2],[60,4]]);
}
if ("searchcondition".match(word)) {
result = result.concat([[71,4]]);
}
if ("searches".match(word)) {
result = result.concat([[46,2]]);
}
if ("second".match(word)) {
result = result.concat([[23,4],[51,1]]);
}
if ("secondly".match(word)) {
result = result.concat([[110,2]]);
}
if ("seconds".match(word)) {
result = result.concat([[92,2]]);
}
if ("section".match(word)) {
result = result.concat([[3,2],[13,2],[14,2],[20,2],[23,2],[25,2],[51,2],[57,2],[74,2],[87,2],[101,2],[109,2],[110,2]]);
}
if ("sections".match(word)) {
result = result.concat([[9,2],[13,1],[25,2],[71,2],[104,2]]);
}
if ("seen".match(word)) {
result = result.concat([[23,2]]);
}
if ("segment".match(word)) {
result = result.concat([[13,2],[103,4]]);
}
if ("segmentation".match(word)) {
result = result.concat([[86,1]]);
}
if ("segments".match(word)) {
result = result.concat([[13,14],[103,9],[110,6]]);
}
if ("select".match(word)) {
result = result.concat([[9,1],[14,6],[23,1],[61,14],[66,4],[67,2],[68,8],[105,2],[106,2],[107,2],[111,2],[114,4],[117,4],[120,3],[123,3],[126,2],[129,2]]);
}
if ("selectaggregate".match(word)) {
result = result.concat([[111,1],[114,1],[117,1],[120,1],[123,1],[129,1]]);
}
if ("selectaggregates".match(word)) {
result = result.concat([[111,2],[114,5],[117,5],[120,3],[123,3],[126,2],[129,1]]);
}
if ("selected".match(word)) {
result = result.concat([[23,2]]);
}
if ("selecting".match(word)) {
result = result.concat([[10,1],[91,2],[110,2]]);
}
if ("selects".match(word)) {
result = result.concat([[23,1],[61,2]]);
}
if ("semantics".match(word)) {
result = result.concat([[14,2],[22,7]]);
}
if ("send".match(word)) {
result = result.concat([[44,2],[108,2]]);
}
if ("separate".match(word)) {
result = result.concat([[9,2],[10,2],[13,2],[46,2],[92,2],[116,1],[122,2]]);
}
if ("sequence".match(word)) {
result = result.concat([[75,2],[87,6]]);
}
if ("serialization".match(word)) {
result = result.concat([[3,1],[7,2]]);
}
if ("series".match(word)) {
result = result.concat([[13,3],[14,2],[103,2]]);
}
if ("server".match(word)) {
result = result.concat([[10,1],[22,1],[102,7],[108,14],[110,15],[125,2]]);
}
if ("serves".match(word)) {
result = result.concat([[128,1]]);
}
if ("service".match(word)) {
result = result.concat([[3,1],[10,1],[23,6],[110,6],[125,4],[128,3]]);
}
if ("services".match(word)) {
result = result.concat([[7,2],[9,4],[10,1],[108,4],[113,1]]);
}
if ("session".match(word)) {
result = result.concat([[14,2],[92,2]]);
}
if ("setbitsperpixel".match(word)) {
result = result.concat([[39,2]]);
}
if ("setconnectionstring".match(word)) {
result = result.concat([[23,1]]);
}
if ("setdatatype".match(word)) {
result = result.concat([[52,4]]);
}
if ("setfeatureclassname".match(word)) {
result = result.concat([[55,3],[56,4],[57,4],[60,4],[61,2]]);
}
if ("setfeatureschema".match(word)) {
result = result.concat([[52,4]]);
}
if ("setfilter".match(word)) {
result = result.concat([[56,4],[57,4],[60,2],[61,2]]);
}
if ("setgeometrictypes".match(word)) {
result = result.concat([[91,2]]);
}
if ("setgeometry".match(word)) {
result = result.concat([[55,2]]);
}
if ("setgeometryproperty".match(word)) {
result = result.concat([[52,2]]);
}
if ("setgeometrytypes".match(word)) {
result = result.concat([[52,2]]);
}
if ("sethaselevation".match(word)) {
result = result.concat([[52,2]]);
}
if ("sethasmeasure".match(word)) {
result = result.concat([[52,2]]);
}
if ("setisabstract".match(word)) {
result = result.concat([[52,2]]);
}
if ("setisautogenerated".match(word)) {
result = result.concat([[52,4]]);
}
if ("setlength".match(word)) {
result = result.concat([[52,2]]);
}
if ("setname".match(word)) {
result = result.concat([[56,2]]);
}
if ("setnullable".match(word)) {
result = result.concat([[52,4]]);
}
if ("setproperty".match(word)) {
result = result.concat([[23,11]]);
}
if ("setreadonly".match(word)) {
result = result.concat([[52,6]]);
}
if ("sets".match(word)) {
result = result.concat([[10,1],[16,2],[23,2],[48,2],[49,2],[119,2]]);
}
if ("setschemaname".match(word)) {
result = result.concat([[52,2]]);
}
if ("settilesizex".match(word)) {
result = result.concat([[39,2]]);
}
if ("settilesizey".match(word)) {
result = result.concat([[39,2]]);
}
if ("setting".match(word)) {
result = result.concat([[19,2],[92,1]]);
}
if ("settings".match(word)) {
result = result.concat([[3,1],[13,2]]);
}
if ("setvalue".match(word)) {
result = result.concat([[56,2]]);
}
if ("several".match(word)) {
result = result.concat([[11,2],[15,2],[18,2],[20,2],[85,2]]);
}
if ("severe".match(word)) {
result = result.concat([[14,2]]);
}
if ("sfsql".match(word)) {
result = result.concat([[51,2],[52,2]]);
}
if ("sg91".match(word)) {
result = result.concat([[99,2]]);
}
if ("shape".match(word)) {
result = result.concat([[2,1],[10,1],[13,6],[91,3],[110,2],[122,7]]);
}
if ("shapes".match(word)) {
result = result.concat([[13,6]]);
}
if ("share".match(word)) {
result = result.concat([[6,2]]);
}
if ("shore".match(word)) {
result = result.concat([[52,2],[110,2]]);
}
if ("shoreline".match(word)) {
result = result.concat([[55,12]]);
}
if ("shores".match(word)) {
result = result.concat([[110,2]]);
}
if ("short".match(word)) {
result = result.concat([[110,1]]);
}
if ("show".match(word)) {
result = result.concat([[19,2]]);
}
if ("shown".match(word)) {
result = result.concat([[9,2],[23,2],[50,2],[72,2],[81,2]]);
}
if ("shows".match(word)) {
result = result.concat([[9,2],[18,2],[46,2],[50,6],[55,2],[94,2],[109,2]]);
}
if ("shp".match(word)) {
result = result.concat([[2,1],[3,1],[10,3],[121,7],[122,25],[123,5]]);
}
if ("shx".match(word)) {
result = result.concat([[122,1]]);
}
if ("side".match(word)) {
result = result.concat([[46,4]]);
}
if ("sidewalk".match(word)) {
result = result.concat([[13,2],[46,16]]);
}
if ("sidewalks".match(word)) {
result = result.concat([[46,2]]);
}
if ("sign".match(word)) {
result = result.concat([[78,2]]);
}
if ("signed".match(word)) {
result = result.concat([[79,2]]);
}
if ("signifying".match(word)) {
result = result.concat([[61,2]]);
}
if ("similar".match(word)) {
result = result.concat([[9,4],[60,2],[110,2]]);
}
if ("similarly".match(word)) {
result = result.concat([[14,2],[68,2],[99,2]]);
}
if ("simple".match(word)) {
result = result.concat([[13,2],[46,2],[51,8],[55,2],[86,2],[87,2],[89,2],[111,1],[114,1],[116,2],[117,1],[120,1],[123,1],[129,1]]);
}
if ("simply".match(word)) {
result = result.concat([[41,2]]);
}
if ("single".match(word)) {
result = result.concat([[10,1],[13,6],[14,4],[27,2],[45,2],[50,2],[55,3],[77,8],[109,2],[111,7],[114,10],[116,1],[117,10],[120,9],[122,1],[123,7],[125,2],[126,2]]);
}
if ("sites".match(word)) {
result = result.concat([[10,1]]);
}
if ("situation".match(word)) {
result = result.concat([[14,2],[63,2]]);
}
if ("size".match(word)) {
result = result.concat([[13,4],[87,38],[111,5],[114,5],[117,5],[120,5],[123,5],[126,5],[129,5]]);
}
if ("skeleton".match(word)) {
result = result.concat([[51,6]]);
}
if ("slight".match(word)) {
result = result.concat([[43,2]]);
}
if ("small".match(word)) {
result = result.concat([[89,2]]);
}
if ("smart".match(word)) {
result = result.concat([[16,2],[20,7]]);
}
if ("smith".match(word)) {
result = result.concat([[68,2]]);
}
if ("software".match(word)) {
result = result.concat([[10,2],[25,1],[89,2],[97,5]]);
}
if ("sole".match(word)) {
result = result.concat([[55,2]]);
}
if ("solid".match(word)) {
result = result.concat([[13,2],[91,2]]);
}
if ("solution".match(word)) {
result = result.concat([[22,2]]);
}
if ("something".match(word)) {
result = result.concat([[108,2]]);
}
if ("somewhat".match(word)) {
result = result.concat([[9,1],[41,2]]);
}
if ("sort".match(word)) {
result = result.concat([[44,2]]);
}
if ("source".match(word)) {
result = result.concat([[3,1],[6,8],[7,8],[9,2],[10,1],[22,9],[23,8],[50,2],[51,6],[52,4],[99,2],[108,12],[128,1]]);
}
if ("sources".match(word)) {
result = result.concat([[6,2],[7,2],[10,4],[22,5]]);
}
if ("space".match(word)) {
result = result.concat([[75,2]]);
}
if ("spatial".match(word)) {
result = result.concat([[6,6],[8,2],[10,8],[13,35],[14,2],[22,2],[41,6],[44,2],[45,2],[60,12],[73,2],[92,33],[96,2],[111,4],[113,1],[114,4],[117,3],[119,5],[120,3],[122,1],[123,4],[125,2],[126,2],[128,2],[129,1]]);
}
if ("spatialcondition".match(word)) {
result = result.concat([[71,4]]);
}
if ("spatialcontextextenttypes".match(word)) {
result = result.concat([[27,6]]);
}
if ("spatialcontextname".match(word)) {
result = result.concat([[50,4],[51,6]]);
}
if ("spatialcontexts".match(word)) {
result = result.concat([[123,1]]);
}
if ("spatialextents".match(word)) {
result = result.concat([[120,1],[126,1],[129,1]]);
}
if ("spatially".match(word)) {
result = result.concat([[10,1],[13,2],[92,2],[128,2]]);
}
if ("spatialoperations".match(word)) {
result = result.concat([[35,6],[71,4]]);
}
if ("special".match(word)) {
result = result.concat([[19,2],[69,1],[70,2],[75,2],[77,2],[81,2],[82,7]]);
}
if ("specialization".match(word)) {
result = result.concat([[13,2]]);
}
if ("specific".match(word)) {
result = result.concat([[7,2],[9,5],[10,2],[11,1],[14,4],[18,4],[22,2],[31,2],[41,2],[42,4],[59,2],[63,2],[66,2],[67,2],[69,1],[70,5],[85,2],[89,2],[90,1],[96,4],[110,2],[113,2],[116,2],[119,2],[122,2],[125,2],[128,3]]);
}
if ("specifically".match(word)) {
result = result.concat([[3,1],[61,2],[99,2],[113,2]]);
}
if ("specification".match(word)) {
result = result.concat([[7,2],[13,2],[22,1],[51,8],[86,5],[87,11],[88,2],[125,2]]);
}
if ("specifications".match(word)) {
result = result.concat([[93,7]]);
}
if ("specified".match(word)) {
result = result.concat([[41,2],[51,2],[57,2],[60,4],[66,2],[67,2],[68,2],[83,2],[92,2],[125,2]]);
}
if ("specifies".match(word)) {
result = result.concat([[13,2],[14,4],[50,2],[60,2],[66,2]]);
}
if ("specify".match(word)) {
result = result.concat([[2,1],[14,2],[40,2],[65,2],[66,2],[67,2],[87,2],[92,1],[93,7]]);
}
if ("specifying".match(word)) {
result = result.concat([[9,1],[13,2],[41,2],[46,2],[111,2]]);
}
if ("spheroid".match(word)) {
result = result.concat([[13,2]]);
}
if ("sql".match(word)) {
result = result.concat([[10,1],[51,2],[60,2],[67,4],[68,6],[76,2],[80,2],[81,4],[83,2],[111,1],[114,1],[117,1]]);
}
if ("sqlcommand".match(word)) {
result = result.concat([[67,2],[68,2]]);
}
if ("sqlcomnmand".match(word)) {
result = result.concat([[111,1],[114,1]]);
}
if ("sss".match(word)) {
result = result.concat([[80,4]]);
}
if ("stand".match(word)) {
result = result.concat([[45,4],[46,11]]);
}
if ("standalone".match(word)) {
result = result.concat([[119,2],[122,2]]);
}
if ("standard".match(word)) {
result = result.concat([[7,2],[14,2],[51,2],[80,2],[128,2]]);
}
if ("standardized".match(word)) {
result = result.concat([[50,2]]);
}
if ("standards".match(word)) {
result = result.concat([[10,2]]);
}
if ("stands".match(word)) {
result = result.concat([[13,2],[19,2],[87,8]]);
}
if ("start".match(word)) {
result = result.concat([[51,4]]);
}
if ("started".match(word)) {
result = result.concat([[0,1],[8,7]]);
}
if ("startpoint".match(word)) {
result = result.concat([[87,4]]);
}
if ("state".match(word)) {
result = result.concat([[14,1],[23,6],[48,10],[49,6],[68,8],[92,2]]);
}
if ("statements".match(word)) {
result = result.concat([[18,2]]);
}
if ("states".match(word)) {
result = result.concat([[48,5]]);
}
if ("static".match(word)) {
result = result.concat([[50,10],[51,12],[55,4],[111,1],[114,1],[117,1],[123,1],[126,1],[129,1]]);
}
if ("status".match(word)) {
result = result.concat([[49,2]]);
}
if ("stddev".match(word)) {
result = result.concat([[111,1]]);
}
if ("step".match(word)) {
result = result.concat([[23,10],[108,4]]);
}
if ("steps".match(word)) {
result = result.concat([[22,2],[23,4],[43,4],[108,2]]);
}
if ("steps:".match(word)) {
result = result.concat([[23,1],[55,4],[104,2]]);
}
if ("storage".match(word)) {
result = result.concat([[10,3],[41,4],[89,2],[113,6]]);
}
if ("store".match(word)) {
result = result.concat([[2,1],[6,2],[7,2],[8,2],[9,5],[10,9],[13,17],[14,30],[17,2],[22,9],[23,17],[41,4],[42,2],[46,4],[50,4],[55,6],[59,2],[60,2],[65,2],[66,4],[86,2],[96,8],[108,18],[110,6],[113,6],[116,9],[119,5],[122,8],[125,8],[128,6]]);
}
if ("stored".match(word)) {
result = result.concat([[10,1],[22,2],[45,2],[46,2],[86,2],[116,1],[125,2]]);
}
if ("stores".match(word)) {
result = result.concat([[9,2],[10,5],[13,8],[22,7],[42,2],[46,4],[108,6],[113,1]]);
}
if ("store”".match(word)) {
result = result.concat([[108,2]]);
}
if ("storing".match(word)) {
result = result.concat([[86,1]]);
}
if ("storing/retrieving".match(word)) {
result = result.concat([[92,1]]);
}
if ("str1val".match(word)) {
result = result.concat([[114,1],[117,1],[120,1],[123,1]]);
}
if ("str2val".match(word)) {
result = result.concat([[114,1],[117,1],[120,1],[123,1]]);
}
if ("straightforward".match(word)) {
result = result.concat([[55,1]]);
}
if ("stream".match(word)) {
result = result.concat([[87,4]]);
}
if ("streaming".match(word)) {
result = result.concat([[87,2]]);
}
if ("streams".match(word)) {
result = result.concat([[110,6]]);
}
if ("streetname".match(word)) {
result = result.concat([[13,2]]);
}
if ("streetnumber".match(word)) {
result = result.concat([[13,2]]);
}
if ("streets".match(word)) {
result = result.concat([[13,2]]);
}
if ("streettype".match(word)) {
result = result.concat([[13,2]]);
}
if ("string".match(word)) {
result = result.concat([[13,4],[19,6],[22,2],[23,3],[41,4],[50,6],[51,2],[52,4],[55,11],[60,2],[61,2],[72,6],[76,2],[77,7],[81,2],[83,3],[93,7],[105,2],[109,4],[110,29],[111,3],[114,12],[117,11],[120,10],[123,9],[126,2],[129,1]]);
}
if ("string:".match(word)) {
result = result.concat([[19,2],[61,2]]);
}
if ("strings".match(word)) {
result = result.concat([[19,2],[77,2],[80,1],[83,2],[110,1]]);
}
if ("strings:".match(word)) {
result = result.concat([[80,2]]);
}
if ("strongly".match(word)) {
result = result.concat([[14,2]]);
}
if ("struct".match(word)) {
result = result.concat([[87,36]]);
}
if ("structure".match(word)) {
result = result.concat([[9,2],[13,2],[41,2],[116,1]]);
}
if ("styles".match(word)) {
result = result.concat([[91,2]]);
}
if ("sub".match(word)) {
result = result.concat([[48,1],[60,2]]);
}
if ("submitted".match(word)) {
result = result.concat([[64,4]]);
}
if ("subsequent".match(word)) {
result = result.concat([[9,4]]);
}
if ("subset".match(word)) {
result = result.concat([[2,1],[9,2],[14,2],[41,2],[50,2],[65,2],[66,2],[100,4]]);
}
if ("subsituted".match(word)) {
result = result.concat([[19,2]]);
}
if ("substitute".match(word)) {
result = result.concat([[51,22]]);
}
if ("substitutiongroup".match(word)) {
result = result.concat([[52,2]]);
}
if ("substitutions".match(word)) {
result = result.concat([[51,2]]);
}
if ("substructure".match(word)) {
result = result.concat([[9,2]]);
}
if ("subtract".match(word)) {
result = result.concat([[72,2],[81,4]]);
}
if ("succeed".match(word)) {
result = result.concat([[64,1]]);
}
if ("succeed:".match(word)) {
result = result.concat([[110,2]]);
}
if ("succeeds".match(word)) {
result = result.concat([[64,1]]);
}
if ("successful".match(word)) {
result = result.concat([[14,2],[108,2]]);
}
if ("successfully".match(word)) {
result = result.concat([[14,1],[93,2]]);
}
if ("sufficient".match(word)) {
result = result.concat([[50,2]]);
}
if ("suitable".match(word)) {
result = result.concat([[128,2]]);
}
if ("sum".match(word)) {
result = result.concat([[111,1],[114,1],[117,1],[120,1],[123,1]]);
}
if ("summarized".match(word)) {
result = result.concat([[64,2]]);
}
if ("summarizes".match(word)) {
result = result.concat([[3,2]]);
}
if ("superset".match(word)) {
result = result.concat([[50,2],[83,2],[88,2]]);
}
if ("supplements".match(word)) {
result = result.concat([[108,2]]);
}
if ("supplying".match(word)) {
result = result.concat([[56,2]]);
}
if ("support".match(word)) {
result = result.concat([[3,6],[6,4],[9,4],[10,3],[13,2],[19,2],[25,1],[41,7],[42,5],[50,2],[62,1],[66,2],[67,2],[68,2],[80,1],[90,1],[92,2],[102,2],[110,6],[113,1],[119,1],[122,1]]);
}
if ("supported".match(word)) {
result = result.concat([[3,2],[13,4],[64,7],[76,2],[78,2],[80,1],[86,1],[99,2],[109,24],[111,10],[114,10],[117,8],[120,10],[123,10],[126,10],[128,1],[129,10]]);
}
if ("supported:".match(word)) {
result = result.concat([[3,2],[13,2],[64,2],[111,12],[114,12],[117,12],[120,12],[123,12],[126,12],[129,14]]);
}
if ("supportparameters".match(word)) {
result = result.concat([[76,2]]);
}
if ("supports".match(word)) {
result = result.concat([[3,2],[6,4],[7,8],[10,9],[13,4],[22,1],[41,6],[50,4],[60,2],[71,2],[72,2],[77,2],[85,2],[92,2],[108,2],[110,4],[113,2],[116,1],[119,4],[122,2]]);
}
if ("supportsassociationproperties".match(word)) {
result = result.concat([[29,4]]);
}
if ("supportsautoidgeneration".match(word)) {
result = result.concat([[29,4]]);
}
if ("supportsconfiguration".match(word)) {
result = result.concat([[27,4]]);
}
if ("supportsdatamodel".match(word)) {
result = result.concat([[39,2]]);
}
if ("supportsdatastorescopeuniqueidgeneration".match(word)) {
result = result.concat([[29,4]]);
}
if ("supportsgeodesicdistance".match(word)) {
result = result.concat([[35,4]]);
}
if ("supportsinheritance".match(word)) {
result = result.concat([[29,6]]);
}
if ("supportslocking".match(word)) {
result = result.concat([[27,6]]);
}
if ("supportslongtransactions".match(word)) {
result = result.concat([[27,4]]);
}
if ("supportsmultipleschemas".match(word)) {
result = result.concat([[29,4]]);
}
if ("supportsnetworkmodel".match(word)) {
result = result.concat([[29,6]]);
}
if ("supportsnonliteralgeometricoperations".match(word)) {
result = result.concat([[35,4]]);
}
if ("supportsobjectproperties".match(word)) {
result = result.concat([[29,4]]);
}
if ("supportsparameters".match(word)) {
result = result.concat([[31,6]]);
}
if ("supportsraster".match(word)) {
result = result.concat([[39,8]]);
}
if ("supportsschemamodification".match(word)) {
result = result.concat([[29,4]]);
}
if ("supportsschemaoverrides".match(word)) {
result = result.concat([[29,4]]);
}
if ("supportsselectdistinct".match(word)) {
result = result.concat([[31,4]]);
}
if ("supportsselectexpressions".match(word)) {
result = result.concat([[31,4]]);
}
if ("supportsselectfunctions".match(word)) {
result = result.concat([[31,4]]);
}
if ("supportsselectgrouping".match(word)) {
result = result.concat([[31,4]]);
}
if ("supportsselectordering".match(word)) {
result = result.concat([[31,4]]);
}
if ("supportssql".match(word)) {
result = result.concat([[27,6]]);
}
if ("supportsstitching".match(word)) {
result = result.concat([[39,4]]);
}
if ("supportssubsampling".match(word)) {
result = result.concat([[39,4]]);
}
if ("supportstimeout".match(word)) {
result = result.concat([[27,6],[31,6]]);
}
if ("supportstransactions".match(word)) {
result = result.concat([[27,4]]);
}
if ("supposed".match(word)) {
result = result.concat([[110,2]]);
}
if ("surface".match(word)) {
result = result.concat([[13,4],[52,4],[91,4]]);
}
if ("svg".match(word)) {
result = result.concat([[10,1],[128,2]]);
}
if ("syntactically".match(word)) {
result = result.concat([[67,2],[68,2]]);
}
if ("system".match(word)) {
result = result.concat([[13,4],[23,2],[46,1],[86,2],[92,8],[96,4],[110,4]]);
}
if ("systems".match(word)) {
result = result.concat([[92,4],[111,1],[122,1]]);
}
if ("t".match(word)) {
result = result.concat([[20,2],[87,2],[105,6],[106,6],[107,6],[110,44]]);
}
if ("table".match(word)) {
result = result.concat([[10,1],[13,10],[41,2],[42,2],[46,2],[50,2],[51,8],[52,10],[55,6],[103,2],[104,9],[105,11],[106,6],[107,8],[110,6],[113,1],[116,3]]);
}
if ("tables".match(word)) {
result = result.concat([[67,2],[68,2],[113,2]]);
}
if ("table’s".match(word)) {
result = result.concat([[104,1]]);
}
if ("table”".match(word)) {
result = result.concat([[107,1]]);
}
if ("tailored".match(word)) {
result = result.concat([[6,2]]);
}
if ("takes".match(word)) {
result = result.concat([[14,2],[23,2],[66,4],[67,2],[110,12]]);
}
if ("target".match(word)) {
result = result.concat([[52,2],[55,8],[56,2],[61,3]]);
}
if ("tasks".match(word)) {
result = result.concat([[11,8]]);
}
if ("technologies".match(word)) {
result = result.concat([[22,8],[23,2],[110,2]]);
}
if ("technology".match(word)) {
result = result.concat([[22,2],[23,2],[101,2],[110,2]]);
}
if ("telco".match(word)) {
result = result.concat([[13,2]]);
}
if ("telcooutsideplant".match(word)) {
result = result.concat([[13,2]]);
}
if ("tells".match(word)) {
result = result.concat([[23,2]]);
}
if ("template".match(word)) {
result = result.concat([[17,4],[20,2]]);
}
if ("temporary".match(word)) {
result = result.concat([[14,2]]);
}
if ("ten".match(word)) {
result = result.concat([[110,2]]);
}
if ("term".match(word)) {
result = result.concat([[22,2]]);
}
if ("terminates".match(word)) {
result = result.concat([[14,2]]);
}
if ("terms".match(word)) {
result = result.concat([[116,1]]);
}
if ("tessellate".match(word)) {
result = result.concat([[103,2]]);
}
if ("test".match(word)) {
result = result.concat([[23,2],[105,2],[106,2],[107,2],[110,66]]);
}
if ("text".match(word)) {
result = result.concat([[66,2],[68,7],[69,1],[70,5],[83,21],[85,1],[88,10],[100,1],[110,1]]);
}
if ("text:".match(word)) {
result = result.concat([[83,2]]);
}
if ("textual".match(word)) {
result = result.concat([[67,2],[68,2],[88,4]]);
}
if ("theessentialfdo".match(word)) {
result = result.concat([[0,1],[113,2],[116,2],[119,2],[122,2],[125,2],[128,2]]);
}
if ("themselves".match(word)) {
result = result.concat([[66,2]]);
}
if ("therefore".match(word)) {
result = result.concat([[42,2],[44,2]]);
}
if ("thing".match(word)) {
result = result.concat([[50,2]]);
}
if ("this:".match(word)) {
result = result.concat([[67,2],[108,2]]);
}
if ("threadcapability".match(word)) {
result = result.concat([[27,4]]);
}
if ("threaded".match(word)) {
result = result.concat([[120,1],[126,1]]);
}
if ("threading".match(word)) {
result = result.concat([[111,1],[114,1],[117,1],[123,1],[129,1]]);
}
if ("three".match(word)) {
result = result.concat([[13,3],[23,1],[43,2],[52,2],[55,2],[99,2],[104,2],[110,4],[122,2]]);
}
if ("throughout".match(word)) {
result = result.concat([[46,2]]);
}
if ("throw".match(word)) {
result = result.concat([[18,4],[19,2]]);
}
if ("thrown".match(word)) {
result = result.concat([[18,4],[49,2],[64,2]]);
}
if ("throws".match(word)) {
result = result.concat([[18,2]]);
}
if ("thus".match(word)) {
result = result.concat([[16,2],[23,2],[89,1]]);
}
if ("tie".match(word)) {
result = result.concat([[9,2]]);
}
if ("time".match(word)) {
result = result.concat([[13,4],[47,1],[48,2],[60,2],[73,2],[80,4],[104,1]]);
}
if ("timestamp".match(word)) {
result = result.concat([[73,2],[80,4]]);
}
if ("together".match(word)) {
result = result.concat([[55,2]]);
}
if ("token".match(word)) {
result = result.concat([[83,2]]);
}
if ("tolerance".match(word)) {
result = result.concat([[92,2]]);
}
if ("tolerances".match(word)) {
result = result.concat([[92,2]]);
}
if ("tool".match(word)) {
result = result.concat([[50,2],[110,4]]);
}
if ("tools".match(word)) {
result = result.concat([[110,4]]);
}
if ("topic".match(word)) {
result = result.concat([[88,2]]);
}
if ("topics".match(word)) {
result = result.concat([[20,4],[58,5]]);
}
if ("topics:".match(word)) {
result = result.concat([[27,2],[29,2],[31,2]]);
}
if ("topology".match(word)) {
result = result.concat([[25,1],[102,1]]);
}
if ("touches".match(word)) {
result = result.concat([[71,2],[73,2],[111,1]]);
}
if ("transaction".match(word)) {
result = result.concat([[3,2],[14,54],[62,8],[63,21],[64,59],[113,1]]);
}
if ("transactions".match(word)) {
result = result.concat([[2,1],[3,1],[14,4],[54,2],[62,2],[63,4],[64,2],[111,2],[113,1],[114,1]]);
}
if ("transaction’s".match(word)) {
result = result.concat([[14,2]]);
}
if ("translation".match(word)) {
result = result.concat([[50,2]]);
}
if ("transmit".match(word)) {
result = result.concat([[10,1],[113,1]]);
}
if ("traps".match(word)) {
result = result.concat([[18,2]]);
}
if ("treated".match(word)) {
result = result.concat([[10,1]]);
}
if ("treats".match(word)) {
result = result.concat([[14,2],[122,1]]);
}
if ("tree".match(word)) {
result = result.concat([[66,2]]);
}
if ("trip".match(word)) {
result = result.concat([[50,6]]);
}
if ("true".match(word)) {
result = result.concat([[23,3],[50,22],[51,20],[66,2],[72,2],[73,2],[93,2]]);
}
if ("try".match(word)) {
result = result.concat([[18,4],[103,2]]);
}
if ("two".match(word)) {
result = result.concat([[12,2],[13,10],[14,4],[16,2],[19,2],[20,2],[22,2],[23,4],[41,2],[43,1],[64,4],[67,2],[86,2],[89,4],[92,4],[93,2],[108,2],[110,4]]);
}
if ("type".match(word)) {
result = result.concat([[7,2],[9,1],[13,35],[14,4],[16,2],[18,4],[27,6],[29,4],[31,4],[33,2],[41,2],[42,2],[43,1],[45,2],[46,15],[50,4],[51,8],[52,10],[55,10],[56,2],[61,6],[66,2],[83,2],[85,1],[86,2],[87,24],[89,2],[90,13],[91,8],[106,1],[109,6],[110,6],[111,28],[114,36],[117,33],[120,34],[123,29],[126,17],[129,10]]);
}
if ("typed".match(word)) {
result = result.concat([[14,2],[77,2]]);
}
if ("typedef".match(word)) {
result = result.concat([[16,2]]);
}
if ("typedefs".match(word)) {
result = result.concat([[16,7]]);
}
if ("types".match(word)) {
result = result.concat([[2,1],[9,1],[10,1],[13,16],[14,4],[18,4],[31,2],[33,4],[41,4],[42,2],[43,2],[45,7],[69,1],[74,7],[84,2],[86,1],[87,11],[89,1],[90,7],[91,9],[109,4],[110,2],[111,5],[113,1],[114,3],[117,4],[120,5],[123,5],[126,5],[129,3]]);
}
if ("types:".match(word)) {
result = result.concat([[45,2],[90,2],[109,2]]);
}
if ("type    ".match(word)) {
result = result.concat([[105,2],[106,2],[107,2]]);
}
if ("type”".match(word)) {
result = result.concat([[50,4],[51,4]]);
}
if ("typical".match(word)) {
result = result.concat([[83,2],[109,2]]);
}
if ("typically".match(word)) {
result = result.concat([[13,2],[16,2],[92,2]]);
}
if ("u".match(word)) {
result = result.concat([[105,2],[106,2],[107,2],[110,68]]);
}
if ("unary".match(word)) {
result = result.concat([[78,4],[81,2]]);
}
if ("unaryexpression".match(word)) {
result = result.concat([[72,4]]);
}
if ("unarylogicaloperator".match(word)) {
result = result.concat([[71,4]]);
}
if ("unaryoperations".match(word)) {
result = result.concat([[81,5]]);
}
if ("unchanged".match(word)) {
result = result.concat([[48,4],[49,2]]);
}
if ("underlying".match(word)) {
result = result.concat([[6,2],[7,2],[9,4],[10,1],[12,2],[14,2],[22,4],[23,2],[41,8],[108,2]]);
}
if ("underneath".match(word)) {
result = result.concat([[18,2]]);
}
if ("understand".match(word)) {
result = result.concat([[50,2]]);
}
if ("understanding".match(word)) {
result = result.concat([[12,2]]);
}
if ("unige".match(word)) {
result = result.concat([[71,2]]);
}
if ("unique".match(word)) {
result = result.concat([[22,1],[44,2],[46,2],[64,4],[111,2],[114,4]]);
}
if ("uniqueid".match(word)) {
result = result.concat([[109,2]]);
}
if ("uniquely".match(word)) {
result = result.concat([[46,2],[52,2],[56,2],[104,2]]);
}
if ("unit".match(word)) {
result = result.concat([[14,8],[63,4],[92,4]]);
}
if ("units".match(word)) {
result = result.concat([[13,2],[92,6]]);
}
if ("unix".match(word)) {
result = result.concat([[116,2]]);
}
if ("unknown".match(word)) {
result = result.concat([[117,6],[120,8],[126,17],[129,7]]);
}
if ("unless".match(word)) {
result = result.concat([[23,1]]);
}
if ("unmanaged".match(word)) {
result = result.concat([[22,1]]);
}
if ("unmodified".match(word)) {
result = result.concat([[49,2]]);
}
if ("unnecessary".match(word)) {
result = result.concat([[18,2]]);
}
if ("unqualified".match(word)) {
result = result.concat([[52,2]]);
}
if ("until".match(word)) {
result = result.concat([[14,3],[23,1],[47,1],[48,2],[92,2]]);
}
if ("up".match(word)) {
result = result.concat([[14,2],[18,4],[41,4],[67,2],[128,1]]);
}
if ("update".match(word)) {
result = result.concat([[9,1],[10,2],[14,8],[56,16],[105,2],[106,2],[107,2],[111,1],[114,1],[117,1],[120,1],[123,1],[125,1]]);
}
if ("updated".match(word)) {
result = result.concat([[46,2]]);
}
if ("updates".match(word)) {
result = result.concat([[3,2],[23,4]]);
}
if ("updating".match(word)) {
result = result.concat([[7,2],[10,1],[54,1],[56,12],[57,2],[110,2]]);
}
if ("upon".match(word)) {
result = result.concat([[2,1],[68,2]]);
}
if ("upper".match(word)) {
result = result.concat([[114,1],[117,1],[120,1],[123,1]]);
}
if ("upward".match(word)) {
result = result.concat([[41,2]]);
}
if ("uri".match(word)) {
result = result.concat([[50,2],[51,3]]);
}
if ("url".match(word)) {
result = result.concat([[50,4],[51,6]]);
}
if ("usage".match(word)) {
result = result.concat([[64,2]]);
}
if ("use:".match(word)) {
result = result.concat([[20,2]]);
}
if ("used".match(word)) {
result = result.concat([[1,2],[2,1],[6,2],[9,1],[12,1],[13,12],[14,12],[17,2],[19,2],[22,7],[23,3],[25,4],[41,6],[45,8],[46,12],[50,2],[51,2],[52,8],[55,7],[56,4],[58,2],[60,6],[63,6],[66,2],[70,2],[73,2],[82,4],[86,4],[87,4],[88,2],[90,1],[92,4],[108,2],[110,7],[111,1],[114,1],[117,1],[120,1],[123,1],[126,1]]);
}
if ("used:".match(word)) {
result = result.concat([[18,2]]);
}
if ("user".match(word)) {
result = result.concat([[6,5],[8,2],[14,9],[22,1],[23,31],[64,12],[66,2],[91,2],[104,2],[105,6],[106,6],[107,6],[110,32],[111,1],[114,1],[117,1],[123,1]]);
}
if ("username".match(word)) {
result = result.concat([[23,5],[108,2],[110,2]]);
}
if ("users".match(word)) {
result = result.concat([[13,4],[14,3],[66,2],[92,2],[108,2]]);
}
if ("user’".match(word)) {
result = result.concat([[23,4]]);
}
if ("user”".match(word)) {
result = result.concat([[23,2]]);
}
if ("uses".match(word)) {
result = result.concat([[7,2],[14,8],[22,2],[23,2],[66,2],[67,2],[77,2],[86,1],[119,2],[122,2],[125,2]]);
}
if ("using".match(word)) {
result = result.concat([[0,1],[2,9],[3,1],[8,2],[11,3],[13,8],[14,4],[18,2],[19,4],[22,3],[23,1],[25,2],[41,2],[42,2],[47,2],[53,2],[55,4],[60,6],[67,2],[68,2],[71,2],[76,2],[80,2],[81,2],[83,2],[92,2],[93,5],[103,4],[110,2],[113,2],[125,2]]);
}
if ("utf".match(word)) {
result = result.concat([[52,2]]);
}
if ("utilizing".match(word)) {
result = result.concat([[113,1]]);
}
if ("v".match(word)) {
result = result.concat([[107,2]]);
}
if ("val".match(word)) {
result = result.concat([[23,2]]);
}
if ("valid".match(word)) {
result = result.concat([[10,2],[23,3],[51,4],[83,2],[119,1],[122,1]]);
}
if ("validate".match(word)) {
result = result.concat([[51,2]]);
}
if ("validation".match(word)) {
result = result.concat([[50,2]]);
}
if ("value".match(word)) {
result = result.concat([[13,4],[16,8],[19,2],[22,4],[23,11],[27,2],[45,6],[46,1],[48,2],[50,12],[51,9],[52,2],[55,82],[56,24],[61,2],[69,1],[83,5],[86,1],[88,2],[94,2],[105,2],[106,3],[110,2],[111,8],[114,16],[117,11],[120,13],[123,10]]);
}
if ("value:".match(word)) {
result = result.concat([[55,2]]);
}
if ("valuecollectionindex".match(word)) {
result = result.concat([[55,8]]);
}
if ("valueexpression".match(word)) {
result = result.concat([[71,4],[72,4]]);
}
if ("valueexpressioncollection".match(word)) {
result = result.concat([[71,6]]);
}
if ("valueexpressions".match(word)) {
result = result.concat([[82,2]]);
}
if ("values".match(word)) {
result = result.concat([[22,11],[23,20],[31,4],[49,2],[51,2],[55,27],[56,12],[57,14],[61,5],[67,2],[83,4],[89,2],[91,1],[94,7],[110,1]]);
}
if ("values:".match(word)) {
result = result.concat([[56,2]]);
}
if ("varchar".match(word)) {
result = result.concat([[51,2],[52,2]]);
}
if ("varchar2".match(word)) {
result = result.concat([[109,2]]);
}
if ("variable".match(word)) {
result = result.concat([[19,6],[27,2],[29,2],[31,2],[33,2],[35,2],[37,2],[39,2],[99,2]]);
}
if ("variables".match(word)) {
result = result.concat([[61,2]]);
}
if ("variation".match(word)) {
result = result.concat([[113,1]]);
}
if ("variety".match(word)) {
result = result.concat([[7,2]]);
}
if ("various".match(word)) {
result = result.concat([[2,1],[9,1],[10,1],[13,2],[24,2],[25,2],[84,2]]);
}
if ("vary".match(word)) {
result = result.concat([[13,4],[42,2],[50,2]]);
}
if ("vector".match(word)) {
result = result.concat([[10,2],[128,4]]);
}
if ("vendor’s".match(word)) {
result = result.concat([[50,2]]);
}
if ("version".match(word)) {
result = result.concat([[3,3],[10,1],[14,2],[22,10],[52,2],[86,2],[104,1]]);
}
if ("versioning".match(word)) {
result = result.concat([[7,2],[104,10],[107,5]]);
}
if ("versionmajor".match(word)) {
result = result.concat([[22,2]]);
}
if ("versionminor".match(word)) {
result = result.concat([[22,2]]);
}
if ("versions".match(word)) {
result = result.concat([[63,2],[64,2]]);
}
if ("vertices".match(word)) {
result = result.concat([[13,3]]);
}
if ("via".match(word)) {
result = result.concat([[10,1],[47,1],[48,1],[49,2],[89,2]]);
}
if ("view".match(word)) {
result = result.concat([[41,2]]);
}
if ("visibility            ".match(word)) {
result = result.concat([[105,2],[106,2],[107,2]]);
}
if ("visible".match(word)) {
result = result.concat([[14,1],[98,2],[105,2],[106,2],[107,2]]);
}
if ("w3".match(word)) {
result = result.concat([[50,2],[51,8],[52,2]]);
}
if ("want".match(word)) {
result = result.concat([[22,2],[51,2],[56,2],[57,2],[108,2],[110,2]]);
}
if ("wanted".match(word)) {
result = result.concat([[16,1]]);
}
if ("wants".match(word)) {
result = result.concat([[14,4],[66,4]]);
}
if ("ways".match(word)) {
result = result.concat([[20,2]]);
}
if ("ways:".match(word)) {
result = result.concat([[85,2],[86,2],[92,2]]);
}
if ("web".match(word)) {
result = result.concat([[3,1],[10,3],[125,4],[128,5]]);
}
if ("webcgm".match(word)) {
result = result.concat([[10,1],[128,2]]);
}
if ("website".match(word)) {
result = result.concat([[10,1]]);
}
if ("wfs".match(word)) {
result = result.concat([[2,1],[3,3],[10,2],[124,7],[125,18],[126,5]]);
}
if ("whatever".match(word)) {
result = result.concat([[22,2],[37,4]]);
}
if ("what’s".match(word)) {
result = result.concat([[3,5]]);
}
if ("whereas".match(word)) {
result = result.concat([[93,2]]);
}
if ("whether".match(word)) {
result = result.concat([[3,1],[6,2],[7,4],[22,1],[23,3],[76,2],[86,1],[88,2],[110,2]]);
}
if ("white".match(word)) {
result = result.concat([[75,2]]);
}
if ("whole".match(word)) {
result = result.concat([[14,2]]);
}
if ("whose".match(word)) {
result = result.concat([[13,2],[23,2],[51,9],[52,8],[55,4],[56,8],[57,2],[68,2],[91,1],[128,1]]);
}
if ("widely".match(word)) {
result = result.concat([[42,2]]);
}
if ("width".match(word)) {
result = result.concat([[129,1]]);
}
if ("wih".match(word)) {
result = result.concat([[92,2]]);
}
if ("window".match(word)) {
result = result.concat([[110,4]]);
}
if ("windows".match(word)) {
result = result.concat([[19,4],[86,1],[99,2],[108,2],[110,4],[116,2],[122,1],[128,2]]);
}
if ("within".match(word)) {
result = result.concat([[9,2],[13,6],[14,8],[22,1],[46,2],[48,3],[64,2],[66,2],[71,2],[73,2],[87,6],[111,2],[117,1],[120,1],[123,1]]);
}
if ("withindistance".match(word)) {
result = result.concat([[71,2],[73,2]]);
}
if ("without".match(word)) {
result = result.concat([[14,2],[46,7],[63,2],[89,2],[111,1]]);
}
if ("wkb".match(word)) {
result = result.concat([[86,14]]);
}
if ("wkt".match(word)) {
result = result.concat([[83,6],[88,2],[111,1]]);
}
if ("wms".match(word)) {
result = result.concat([[2,1],[3,1],[10,3],[127,7],[128,28],[129,5]]);
}
if ("word".match(word)) {
result = result.concat([[77,2],[86,1]]);
}
if ("words".match(word)) {
result = result.concat([[67,2],[70,2],[116,1]]);
}
if ("work".match(word)) {
result = result.concat([[2,2],[12,2],[40,2],[42,2],[67,2],[68,2],[85,2],[113,2]]);
}
if ("working".match(word)) {
result = result.concat([[11,1],[15,2],[43,5],[69,2]]);
}
if ("works".match(word)) {
result = result.concat([[23,1]]);
}
if ("workspaces".match(word)) {
result = result.concat([[13,2],[92,2]]);
}
if ("world".match(word)) {
result = result.concat([[13,6]]);
}
if ("wrap".match(word)) {
result = result.concat([[16,2]]);
}
if ("wrapped".match(word)) {
result = result.concat([[16,2]]);
}
if ("wrapping".match(word)) {
result = result.concat([[17,2]]);
}
if ("wraps".match(word)) {
result = result.concat([[16,1],[18,2]]);
}
if ("write".match(word)) {
result = result.concat([[10,1],[52,2],[111,2],[114,2],[117,2],[120,1],[123,2]]);
}
if ("writexml".match(word)) {
result = result.concat([[50,4],[52,4]]);
}
if ("writing".match(word)) {
result = result.concat([[50,2],[52,7]]);
}
if ("written".match(word)) {
result = result.concat([[9,1],[50,4],[51,2],[52,2]]);
}
if ("www".match(word)) {
result = result.concat([[3,1],[10,1],[86,2]]);
}
if ("x".match(word)) {
result = result.concat([[10,1],[13,2],[16,16],[61,2],[116,5]]);
}
if ("xmax".match(word)) {
result = result.concat([[110,3]]);
}
if ("xmin".match(word)) {
result = result.concat([[110,3]]);
}
if ("xml".match(word)) {
result = result.concat([[3,8],[7,2],[9,4],[10,1],[50,17],[51,8],[52,31],[116,1],[117,1],[122,1],[123,1],[125,2],[129,1]]);
}
if ("xmlfeatureflags".match(word)) {
result = result.concat([[93,2]]);
}
if ("xmlns:xs".match(word)) {
result = result.concat([[50,2],[51,8],[52,2]]);
}
if ("xp".match(word)) {
result = result.concat([[110,4]]);
}
if ("xpath".match(word)) {
result = result.concat([[50,8],[51,12],[52,4]]);
}
if ("xs:annotation".match(word)) {
result = result.concat([[50,8],[51,26],[52,8]]);
}
if ("xs:appinfo".match(word)) {
result = result.concat([[50,2],[51,6],[52,4]]);
}
if ("xs:base64binary".match(word)) {
result = result.concat([[50,2]]);
}
if ("xs:boolean".match(word)) {
result = result.concat([[50,2]]);
}
if ("xs:complexcontent".match(word)) {
result = result.concat([[50,2],[51,6],[52,2]]);
}
if ("xs:complextype".match(word)) {
result = result.concat([[50,8],[51,9],[52,2]]);
}
if ("xs:datetime".match(word)) {
result = result.concat([[50,2]]);
}
if ("xs:documentation".match(word)) {
result = result.concat([[50,10],[51,34],[52,10]]);
}
if ("xs:double".match(word)) {
result = result.concat([[50,2]]);
}
if ("xs:element".match(word)) {
result = result.concat([[50,16],[51,21],[52,6]]);
}
if ("xs:extension".match(word)) {
result = result.concat([[50,2],[51,7],[52,2]]);
}
if ("xs:field".match(word)) {
result = result.concat([[50,6],[51,6],[52,2]]);
}
if ("xs:float".match(word)) {
result = result.concat([[50,2]]);
}
if ("xs:fractiondigits".match(word)) {
result = result.concat([[50,2]]);
}
if ("xs:import".match(word)) {
result = result.concat([[50,4],[51,2]]);
}
if ("xs:key".match(word)) {
result = result.concat([[50,2],[51,8],[52,2]]);
}
if ("xs:maxlength".match(word)) {
result = result.concat([[50,2],[51,4],[52,2]]);
}
if ("xs:restriction".match(word)) {
result = result.concat([[50,4],[51,4],[52,2]]);
}
if ("xs:schema".match(word)) {
result = result.concat([[50,4],[51,16],[52,2]]);
}
if ("xs:selector".match(word)) {
result = result.concat([[50,2],[51,6],[52,2]]);
}
if ("xs:sequence".match(word)) {
result = result.concat([[50,2],[51,6],[52,2]]);
}
if ("xs:simpletype".match(word)) {
result = result.concat([[50,2],[51,4],[52,2]]);
}
if ("xs:string".match(word)) {
result = result.concat([[50,2],[52,2]]);
}
if ("xs:totaldigits".match(word)) {
result = result.concat([[50,2]]);
}
if ("xsd".match(word)) {
result = result.concat([[50,2],[51,4]]);
}
if ("xy".match(word)) {
result = result.concat([[13,6],[37,2],[55,4],[61,2],[83,11],[87,12],[88,2],[93,2],[114,2],[129,2]]);
}
if ("xym".match(word)) {
result = result.concat([[13,2],[83,8],[87,2]]);
}
if ("xytolerance".match(word)) {
result = result.concat([[92,2]]);
}
if ("xyz".match(word)) {
result = result.concat([[10,1],[13,4],[83,10],[93,4],[117,2]]);
}
if ("xyzm".match(word)) {
result = result.concat([[13,2],[83,8],[111,2],[120,2],[123,2],[126,2]]);
}
if ("y".match(word)) {
result = result.concat([[10,1],[13,2],[41,4],[61,2],[116,5]]);
}
if ("yacc".match(word)) {
result = result.concat([[72,2]]);
}
if ("ymax".match(word)) {
result = result.concat([[110,3]]);
}
if ("ymin".match(word)) {
result = result.concat([[110,3]]);
}
if ("yyyy".match(word)) {
result = result.concat([[80,4]]);
}
if ("z".match(word)) {
result = result.concat([[10,1],[13,2],[37,2],[61,6],[87,6],[116,5]]);
}
if ("zero".match(word)) {
result = result.concat([[13,4],[22,2],[55,2],[92,2]]);
}
if ("ztolerance".match(word)) {
result = result.concat([[92,2]]);
}
if (" ".match(word)) {
result = result.concat([[23,4]]);
}
if (" //".match(word)) {
result = result.concat([[23,2]]);
}
if (" defaultval".match(word)) {
result = result.concat([[23,2]]);
}
if (" displayname".match(word)) {
result = result.concat([[23,2]]);
}
if (" if".match(word)) {
result = result.concat([[23,2]]);
}
if (" internalname".match(word)) {
result = result.concat([[23,2]]);
}
if (" isdatastorename".match(word)) {
result = result.concat([[23,2]]);
}
if (" isenumerable".match(word)) {
result = result.concat([[23,2]]);
}
if (" isfilename".match(word)) {
result = result.concat([[23,2]]);
}
if (" isfilepath".match(word)) {
result = result.concat([[23,2]]);
}
if (" isprotected".match(word)) {
result = result.concat([[23,2]]);
}
if (" isrequired".match(word)) {
result = result.concat([[23,2]]);
}
if (" localname".match(word)) {
result = result.concat([[23,2]]);
}
if (" name".match(word)) {
result = result.concat([[23,2]]);
}
if (" provider".match(word)) {
result = result.concat([[23,2]]);
}
if (" switch".match(word)) {
result = result.concat([[23,2]]);
}
if (" val".match(word)) {
result = result.concat([[23,2]]);
}
if ("  ".match(word)) {
result = result.concat([[18,2],[33,2],[50,50],[51,58],[52,18],[61,2]]);
}
if ("  //".match(word)) {
result = result.concat([[18,2],[37,4],[61,4]]);
}
if ("  abstract".match(word)) {
result = result.concat([[50,4]]);
}
if ("  arguments".match(word)) {
result = result.concat([[33,2]]);
}
if ("  attributeformdefault".match(word)) {
result = result.concat([[50,2],[51,8]]);
}
if ("  catch".match(word)) {
result = result.concat([[18,4]]);
}
if ("  classdef".match(word)) {
result = result.concat([[61,2]]);
}
if ("  connection".match(word)) {
result = result.concat([[52,6],[61,2]]);
}
if ("  default".match(word)) {
result = result.concat([[50,6]]);
}
if ("  elementformdefault".match(word)) {
result = result.concat([[50,2],[51,8]]);
}
if ("  fdo:geometrictypes".match(word)) {
result = result.concat([[50,2]]);
}
if ("  fdo:geometryname".match(word)) {
result = result.concat([[50,2]]);
}
if ("  fdo:geometryreadonly".match(word)) {
result = result.concat([[50,2]]);
}
if ("  fdo:haselevation".match(word)) {
result = result.concat([[50,2]]);
}
if ("  fdo:hasmeasure".match(word)) {
result = result.concat([[50,2]]);
}
if ("  fdo:readonly".match(word)) {
result = result.concat([[50,6]]);
}
if ("  fdo:srsname".match(word)) {
result = result.concat([[50,2]]);
}
if ("  fdocomparisonoperations".match(word)) {
result = result.concat([[61,2]]);
}
if ("  fdoexception".match(word)) {
result = result.concat([[18,2]]);
}
if ("  fdoidentifier::create".match(word)) {
result = result.concat([[61,2]]);
}
if ("  fdopropertydefinition".match(word)) {
result = result.concat([[20,2]]);
}
if ("  fdoptr".match(word)) {
result = result.concat([[20,2]]);
}
if ("  for".match(word)) {
result = result.concat([[33,2],[61,2]]);
}
if ("  functiondefinition".match(word)) {
result = result.concat([[33,2]]);
}
if ("  functiondescription".match(word)) {
result = result.concat([[33,2]]);
}
if ("  functionname".match(word)) {
result = result.concat([[33,2]]);
}
if ("  geometricproperty".match(word)) {
result = result.concat([[50,2],[91,1]]);
}
if ("  getconditiontypes".match(word)) {
result = result.concat([[35,2]]);
}
if ("  getdistanceoperations".match(word)) {
result = result.concat([[35,2]]);
}
if ("  getgeometrycomponenttypes".match(word)) {
result = result.concat([[37,2]]);
}
if ("  getgeometrytypes".match(word)) {
result = result.concat([[37,2]]);
}
if ("  getspatialcontexttypes".match(word)) {
result = result.concat([[27,2]]);
}
if ("  getspatialoperations".match(word)) {
result = result.concat([[35,2]]);
}
if ("  getsupportedautogeneratedtypes".match(word)) {
result = result.concat([[29,2]]);
}
if ("  minoccurs".match(word)) {
result = result.concat([[50,6]]);
}
if ("  numargumentdefinitions".match(word)) {
result = result.concat([[33,2]]);
}
if ("  numproperties".match(word)) {
result = result.concat([[61,2]]);
}
if ("  optional".match(word)) {
result = result.concat([[50,2]]);
}
if ("  ppropdef".match(word)) {
result = result.concat([[20,2]]);
}
if ("  pprops".match(word)) {
result = result.concat([[20,2]]);
}
if ("  properties".match(word)) {
result = result.concat([[61,2]]);
}
if ("  psz".match(word)) {
result = result.concat([[20,2]]);
}
if ("  rgbrasterdatamodel".match(word)) {
result = result.concat([[39,12]]);
}
if ("  substitutiongroup".match(word)) {
result = result.concat([[50,2]]);
}
if ("  supportsassociationproperties".match(word)) {
result = result.concat([[29,2]]);
}
if ("  supportsautoidgeneration".match(word)) {
result = result.concat([[29,2]]);
}
if ("  supportsconfiguration".match(word)) {
result = result.concat([[27,2]]);
}
if ("  supportsdatamodel".match(word)) {
result = result.concat([[39,2]]);
}
if ("  supportsdatastorescopeuniqueidgeneration".match(word)) {
result = result.concat([[29,2]]);
}
if ("  supportsgeodesicdistance".match(word)) {
result = result.concat([[35,2]]);
}
if ("  supportslongtransactions".match(word)) {
result = result.concat([[27,2]]);
}
if ("  supportsmultipleschemas".match(word)) {
result = result.concat([[29,2]]);
}
if ("  supportsnonliteralgeometricoperations".match(word)) {
result = result.concat([[35,2]]);
}
if ("  supportsobjectproperties".match(word)) {
result = result.concat([[29,2]]);
}
if ("  supportsschemamodification".match(word)) {
result = result.concat([[29,2]]);
}
if ("  supportsschemaoverrides".match(word)) {
result = result.concat([[29,2]]);
}
if ("  supportsselectdistinct".match(word)) {
result = result.concat([[31,2]]);
}
if ("  supportsselectexpressions".match(word)) {
result = result.concat([[31,2]]);
}
if ("  supportsselectfunctions".match(word)) {
result = result.concat([[31,2]]);
}
if ("  supportsselectgrouping".match(word)) {
result = result.concat([[31,2]]);
}
if ("  supportsselectordering".match(word)) {
result = result.concat([[31,2]]);
}
if ("  supportsstitching".match(word)) {
result = result.concat([[39,2]]);
}
if ("  supportssubsampling".match(word)) {
result = result.concat([[39,2]]);
}
if ("  supportstransactions".match(word)) {
result = result.concat([[27,2]]);
}
if ("  targetnamespace".match(word)) {
result = result.concat([[50,2],[51,8]]);
}
if ("  the".match(word)) {
result = result.concat([[50,2]]);
}
if ("  type".match(word)) {
result = result.concat([[50,6]]);
}
if ("  while".match(word)) {
result = result.concat([[18,2]]);
}
if ("  xmlns:".match(word)) {
result = result.concat([[50,2],[51,2]]);
}
if ("  xmlns:fdo".match(word)) {
result = result.concat([[50,2],[51,8]]);
}
if ("  xmlns:gml".match(word)) {
result = result.concat([[50,2],[51,8]]);
}
if ("  xmlns:ogc980461fs".match(word)) {
result = result.concat([[51,6]]);
}
if ("   ".match(word)) {
result = result.concat([[23,2]]);
}
if ("   case".match(word)) {
result = result.concat([[23,2]]);
}
if ("   if".match(word)) {
result = result.concat([[23,2]]);
}
if ("    ".match(word)) {
result = result.concat([[18,8],[50,16],[51,76],[52,30],[56,2],[57,2],[61,2]]);
}
if ("    //".match(word)) {
result = result.concat([[61,2]]);
}
if ("    67".match(word)) {
result = result.concat([[55,2]]);
}
if ("    73".match(word)) {
result = result.concat([[55,2]]);
}
if ("    abstract".match(word)) {
result = result.concat([[51,12],[52,4]]);
}
if ("    acutprintf".match(word)) {
result = result.concat([[18,2]]);
}
if ("    argumentdefinition".match(word)) {
result = result.concat([[33,2]]);
}
if ("    argumentdescription".match(word)) {
result = result.concat([[33,2]]);
}
if ("    argumentname".match(word)) {
result = result.concat([[33,2]]);
}
if ("    argumenttype".match(word)) {
result = result.concat([[33,2]]);
}
if ("    attributeformdefault".match(word)) {
result = result.concat([[52,2]]);
}
if ("    bluelakeexteriorringordinates".match(word)) {
result = result.concat([[55,2]]);
}
if ("    connection".match(word)) {
result = result.concat([[55,2]]);
}
if ("    curre".match(word)) {
result = result.concat([[18,2]]);
}
if ("    cw2a".match(word)) {
result = result.concat([[18,2]]);
}
if ("    elementformdefault".match(word)) {
result = result.concat([[52,2]]);
}
if ("    fdo:geometrictypes".match(word)) {
result = result.concat([[51,6],[52,2]]);
}
if ("    fdo:geometryname".match(word)) {
result = result.concat([[51,6],[52,2]]);
}
if ("    fdo:geometryreadonly".match(word)) {
result = result.concat([[51,6]]);
}
if ("    fdo:haselevation".match(word)) {
result = result.concat([[51,6],[52,2]]);
}
if ("    fdo:hasmeasure".match(word)) {
result = result.concat([[51,6],[52,2]]);
}
if ("    fdo:srsname".match(word)) {
result = result.concat([[51,6],[52,2]]);
}
if ("    fdodimensionality".match(word)) {
result = result.concat([[55,4]]);
}
if ("    fdopropertyvalue::create".match(word)) {
result = result.concat([[55,6]]);
}
if ("    gooseislandlinearringordinates".match(word)) {
result = result.concat([[55,2]]);
}
if ("    interiorringsbluelake".match(word)) {
result = result.concat([[55,2]]);
}
if ("    propertydef".match(word)) {
result = result.concat([[61,2]]);
}
if ("    propertyname".match(word)) {
result = result.concat([[61,2]]);
}
if ("    propertytype".match(word)) {
result = result.concat([[61,2]]);
}
if ("    samplegeometryfactory".match(word)) {
result = result.concat([[55,4]]);
}
if ("    samplegeometryvalue".match(word)) {
result = result.concat([[55,2]]);
}
if ("    sampleidentitydatavalue".match(word)) {
result = result.concat([[55,2]]);
}
if ("    samplenamedatavalue".match(word)) {
result = result.concat([[55,2]]);
}
if ("    samplepropertyvalues".match(word)) {
result = result.concat([[55,6]]);
}
if ("    setdatamodeltype".match(word)) {
result = result.concat([[39,2]]);
}
if ("    setorganization".match(word)) {
result = result.concat([[39,2]]);
}
if ("    sql".match(word)) {
result = result.concat([[51,2]]);
}
if ("    substitutiongroup".match(word)) {
result = result.concat([[51,6]]);
}
if ("    supportsdatamodel".match(word)) {
result = result.concat([[39,2]]);
}
if ("    switch".match(word)) {
result = result.concat([[61,2]]);
}
if ("    targetnamespace".match(word)) {
result = result.concat([[52,2]]);
}
if ("    type".match(word)) {
result = result.concat([[51,6],[52,2]]);
}
if ("    xmlns:fdo".match(word)) {
result = result.concat([[52,2]]);
}
if ("    xmlns:gml".match(word)) {
result = result.concat([[52,2]]);
}
if ("    xmlns:samplefeatureschema".match(word)) {
result = result.concat([[52,2]]);
}
if ("     ".match(word)) {
result = result.concat([[23,2]]);
}
if ("     enumnames".match(word)) {
result = result.concat([[23,2]]);
}
if ("     for".match(word)) {
result = result.concat([[23,2]]);
}
if ("      ".match(word)) {
result = result.concat([[50,6],[51,66],[52,34]]);
}
if ("      //".match(word)) {
result = result.concat([[61,8]]);
}
if ("      case".match(word)) {
result = result.concat([[61,4]]);
}
if ("      default:".match(word)) {
result = result.concat([[61,2]]);
}
if ("      samplefeatureschema".match(word)) {
result = result.concat([[52,2]]);
}
if ("      sql".match(word)) {
result = result.concat([[51,4]]);
}
if ("       enumname".match(word)) {
result = result.concat([[23,2]]);
}
if ("        ".match(word)) {
result = result.concat([[50,2],[51,12],[52,4],[61,4]]);
}
if ("        //".match(word)) {
result = result.concat([[61,2]]);
}
if ("        break".match(word)) {
result = result.concat([[61,4]]);
}
if ("        building".match(word)) {
result = result.concat([[51,4]]);
}
if ("        bytearray".match(word)) {
result = result.concat([[61,2]]);
}
if ("        datapropertydef".match(word)) {
result = result.concat([[61,2]]);
}
if ("        datatype".match(word)) {
result = result.concat([[61,2]]);
}
if ("        fdofeatureclass::create".match(word)) {
result = result.concat([[51,2]]);
}
if ("        fdogeometricpropertydefinition::create".match(word)) {
result = result.concat([[51,2]]);
}
if ("        geometry".match(word)) {
result = result.concat([[61,2]]);
}
if ("        geometrytype".match(word)) {
result = result.concat([[61,2]]);
}
if ("        switch".match(word)) {
result = result.concat([[61,4]]);
}
if ("          ".match(word)) {
result = result.concat([[51,38],[52,8],[61,4]]);
}
if ("          case".match(word)) {
result = result.concat([[61,30]]);
}
if ("          default:".match(word)) {
result = result.concat([[61,4]]);
}
if ("          dynamic".match(word)) {
result = result.concat([[61,2]]);
}
if ("          samplegeometryfactory".match(word)) {
result = result.concat([[61,2]]);
}
if ("            ".match(word)) {
result = result.concat([[51,32],[52,12],[61,4]]);
}
if ("            break".match(word)) {
result = result.concat([[61,30]]);
}
if ("            default".match(word)) {
result = result.concat([[51,4],[52,4]]);
}
if ("            dimensionality".match(word)) {
result = result.concat([[61,2]]);
}
if ("            double".match(word)) {
result = result.concat([[61,2]]);
}
if ("            exteriorring".match(word)) {
result = result.concat([[61,2]]);
}
if ("            fdo:geometrictypes".match(word)) {
result = result.concat([[51,4]]);
}
if ("            fdo:geometryname".match(word)) {
result = result.concat([[51,4]]);
}
if ("            fdo:geometryreadonly".match(word)) {
result = result.concat([[51,4]]);
}
if ("            fdo:haselevation".match(word)) {
result = result.concat([[51,4]]);
}
if ("            fdo:hasmeasure".match(word)) {
result = result.concat([[51,4]]);
}
if ("            fdo:readonly".match(word)) {
result = result.concat([[51,8]]);
}
if ("            fdo:srsname".match(word)) {
result = result.concat([[51,4]]);
}
if ("            for".match(word)) {
result = result.concat([[61,4]]);
}
if ("            minoccurs".match(word)) {
result = result.concat([[51,4]]);
}
if ("            numinteriorrings".match(word)) {
result = result.concat([[61,2]]);
}
if ("            numpositions".match(word)) {
result = result.concat([[61,2]]);
}
if ("            polygon".match(word)) {
result = result.concat([[61,2]]);
}
if ("            ref".match(word)) {
result = result.concat([[51,4]]);
}
if ("            type".match(word)) {
result = result.concat([[51,4],[52,2]]);
}
if ("              ".match(word)) {
result = result.concat([[51,28],[52,12],[61,8]]);
}
if ("              //".match(word)) {
result = result.concat([[61,2]]);
}
if ("              else".match(word)) {
result = result.concat([[61,4]]);
}
if ("              if".match(word)) {
result = result.concat([[61,2]]);
}
if ("              interiorring".match(word)) {
result = result.concat([[61,2]]);
}
if ("              position".match(word)) {
result = result.concat([[61,2]]);
}
if ("                ".match(word)) {
result = result.concat([[51,6],[52,2]]);
}
if ("                dimensionality".match(word)) {
result = result.concat([[61,2]]);
}
if ("                fdodatapropertydefinition::create".match(word)) {
result = result.concat([[51,4]]);
}
if ("                fdogeometricpropertydefinition::create".match(word)) {
result = result.concat([[51,2]]);
}
if ("                m".match(word)) {
result = result.concat([[61,2]]);
}
if ("                sample".match(word)) {
result = result.concat([[52,4]]);
}
if ("                x".match(word)) {
result = result.concat([[61,6]]);
}
if ("                y".match(word)) {
result = result.concat([[61,6]]);
}
if ("                z".match(word)) {
result = result.concat([[61,4]]);
}
if ("                  ".match(word)) {
result = result.concat([[91,1]]);
}
if ("      ‘gml:abstractfeaturetype’".match(word)) {
result = result.concat([[50,2],[51,2]]);
}
if ("‘".match(word)) {
result = result.concat([[110,2]]);
}
if ("‘101’".match(word)) {
result = result.concat([[56,2]]);
}
if ("‘12’".match(word)) {
result = result.concat([[83,1]]);
}
if ("‘a’".match(word)) {
result = result.concat([[110,2]]);
}
if ("‘columns’".match(word)) {
result = result.concat([[116,1]]);
}
if ("‘fdo".match(word)) {
result = result.concat([[23,4]]);
}
if ("‘geometry’".match(word)) {
result = result.concat([[116,1]]);
}
if ("‘gml:abstractfeaturetype’".match(word)) {
result = result.concat([[51,1]]);
}
if ("‘in’".match(word)) {
result = result.concat([[60,2]]);
}
if ("‘point".match(word)) {
result = result.concat([[93,4]]);
}
if ("‘root’".match(word)) {
result = result.concat([[23,2]]);
}
if ("‘tables’".match(word)) {
result = result.concat([[116,1]]);
}
if ("‘test’".match(word)) {
result = result.concat([[23,2]]);
}
if ("’".match(word)) {
result = result.concat([[93,4],[110,2]]);
}
if ("“".match(word)) {
result = result.concat([[16,8],[50,2],[51,2],[56,2],[81,2],[110,4]]);
}
if ("“64”".match(word)) {
result = result.concat([[51,1]]);
}
if ("“a".match(word)) {
result = result.concat([[51,1]]);
}
if ("“address".match(word)) {
result = result.concat([[51,1]]);
}
if ("“address”".match(word)) {
result = result.concat([[51,2]]);
}
if ("“autodesk".match(word)) {
result = result.concat([[22,1]]);
}
if ("“buildings”".match(word)) {
result = result.concat([[51,1]]);
}
if ("“class3”".match(word)) {
result = result.concat([[48,4]]);
}
if ("“classdescription”".match(word)) {
result = result.concat([[43,2]]);
}
if ("“classname”".match(word)) {
result = result.concat([[43,2]]);
}
if ("“column".match(word)) {
result = result.concat([[56,2]]);
}
if ("“commit".match(word)) {
result = result.concat([[14,2]]);
}
if ("“coordinate".match(word)) {
result = result.concat([[13,2]]);
}
if ("“create”".match(word)) {
result = result.concat([[89,1]]);
}
if ("“default".match(word)) {
result = result.concat([[108,2]]);
}
if ("“dependent".match(word)) {
result = result.concat([[107,1]]);
}
if ("“description”".match(word)) {
result = result.concat([[43,3]]);
}
if ("“do".match(word)) {
result = result.concat([[90,1]]);
}
if ("“esri".match(word)) {
result = result.concat([[108,4]]);
}
if ("“fdo".match(word)) {
result = result.concat([[51,2]]);
}
if ("“fdo:int32”".match(word)) {
result = result.concat([[51,1]]);
}
if ("“fdoidisposable".match(word)) {
result = result.concat([[20,2]]);
}
if ("“fdoptr".match(word)) {
result = result.concat([[20,2]]);
}
if ("“feature".match(word)) {
result = result.concat([[51,1]]);
}
if ("“featureschema".match(word)) {
result = result.concat([[43,1]]);
}
if ("“fid”".match(word)) {
result = result.concat([[51,7],[52,2]]);
}
if ("“footprint”".match(word)) {
result = result.concat([[51,1]]);
}
if ("“geometry".match(word)) {
result = result.concat([[84,2]]);
}
if ("“get”".match(word)) {
result = result.concat([[89,1]]);
}
if ("“http://fdo".match(word)) {
result = result.concat([[51,1]]);
}
if ("“includes”".match(word)) {
result = result.concat([[9,2]]);
}
if ("“lakes”".match(word)) {
result = result.concat([[52,2]]);
}
if ("“lanes".match(word)) {
result = result.concat([[14,2],[67,2]]);
}
if ("“multiversion".match(word)) {
result = result.concat([[107,1]]);
}
if ("“name”".match(word)) {
result = result.concat([[43,3],[51,3],[52,2]]);
}
if ("“not".match(word)) {
result = result.concat([[107,1]]);
}
if ("“objects".match(word)) {
result = result.concat([[89,2]]);
}
if ("“ogc".match(word)) {
result = result.concat([[51,3]]);
}
if ("“ogc980461fs”".match(word)) {
result = result.concat([[51,2]]);
}
if ("“on".match(word)) {
result = result.concat([[92,2]]);
}
if ("“ordinate".match(word)) {
result = result.concat([[110,1]]);
}
if ("“osgeo".match(word)) {
result = result.concat([[22,1]]);
}
if ("“point".match(word)) {
result = result.concat([[83,1]]);
}
if ("“point”".match(word)) {
result = result.concat([[51,1]]);
}
if ("“position".match(word)) {
result = result.concat([[51,1]]);
}
if ("“position”".match(word)) {
result = result.concat([[51,3]]);
}
if ("“read/write".match(word)) {
result = result.concat([[22,1]]);
}
if ("“road”".match(word)) {
result = result.concat([[41,2]]);
}
if ("“roll".match(word)) {
result = result.concat([[49,2]]);
}
if ("“rollback".match(word)) {
result = result.concat([[14,2]]);
}
if ("“row".match(word)) {
result = result.concat([[107,1]]);
}
if ("“rows”".match(word)) {
result = result.concat([[57,2]]);
}
if ("“row”".match(word)) {
result = result.concat([[56,4]]);
}
if ("“samplefeatureclass".match(word)) {
result = result.concat([[52,2]]);
}
if ("“samplefeatureschema".match(word)) {
result = result.concat([[52,2]]);
}
if ("“samplegeometricproperty".match(word)) {
result = result.concat([[52,2]]);
}
if ("“sampleidentitydataproperty".match(word)) {
result = result.concat([[52,2],[110,2]]);
}
if ("“sampleidentitydataproperty”".match(word)) {
result = result.concat([[56,2]]);
}
if ("“samplenamedataproperty".match(word)) {
result = result.concat([[52,2]]);
}
if ("“sc".match(word)) {
result = result.concat([[51,2]]);
}
if ("“schemaname”".match(word)) {
result = result.concat([[43,1]]);
}
if ("“shore”".match(word)) {
result = result.concat([[52,2]]);
}
if ("“sidewalks”".match(word)) {
result = result.concat([[41,2]]);
}
if ("“sidewalk”".match(word)) {
result = result.concat([[41,2]]);
}
if ("“smith”".match(word)) {
result = result.concat([[68,2]]);
}
if ("“solid”".match(word)) {
result = result.concat([[13,2]]);
}
if ("“surface”".match(word)) {
result = result.concat([[51,1]]);
}
if ("“table”".match(word)) {
result = result.concat([[56,2],[57,2]]);
}
if ("“unknown”".match(word)) {
result = result.concat([[90,1]]);
}
if ("”".match(word)) {
result = result.concat([[13,2],[14,6],[22,1],[23,1],[50,124],[51,120],[52,10],[56,2],[67,2],[81,2],[83,1],[88,1],[89,2],[93,4],[110,6]]);
}
if ("”/".match(word)) {
result = result.concat([[50,20],[51,14]]);
}
if ("”64”/".match(word)) {
result = result.concat([[51,2]]);
}
if ("”address”".match(word)) {
result = result.concat([[51,2]]);
}
if ("”alse”".match(word)) {
result = result.concat([[51,2]]);
}
if ("”buildingskey”".match(word)) {
result = result.concat([[51,4]]);
}
if ("”buildingstype”".match(word)) {
result = result.concat([[51,8]]);
}
if ("”buildings”".match(word)) {
result = result.concat([[51,4]]);
}
if ("”false”".match(word)) {
result = result.concat([[51,24]]);
}
if ("”false”/".match(word)) {
result = result.concat([[51,4]]);
}
if ("”fdo:int32”".match(word)) {
result = result.concat([[51,2]]);
}
if ("”fid”".match(word)) {
result = result.concat([[51,2]]);
}
if ("”fid”/".match(word)) {
result = result.concat([[51,4]]);
}
if ("”footprint”".match(word)) {
result = result.concat([[51,4]]);
}
if ("”gml:".match(word)) {
result = result.concat([[50,2],[51,10]]);
}
if ("”gml:abstractfeaturetype“".match(word)) {
result = result.concat([[51,4]]);
}
if ("”gml:abstractgeometrytype”".match(word)) {
result = result.concat([[50,2]]);
}
if ("”http://".match(word)) {
result = result.concat([[50,4],[51,4]]);
}
if ("”http://fdo".match(word)) {
result = result.concat([[50,2],[51,24]]);
}
if ("”http://www".match(word)) {
result = result.concat([[50,4],[51,16]]);
}
if ("”left”".match(word)) {
result = result.concat([[46,2]]);
}
if ("”point”".match(word)) {
result = result.concat([[51,2]]);
}
if ("”position”".match(word)) {
result = result.concat([[51,4]]);
}
if ("”qualified”".match(word)) {
result = result.concat([[50,2],[51,8]]);
}
if ("”sc".match(word)) {
result = result.concat([[51,6]]);
}
if ("”surface”".match(word)) {
result = result.concat([[51,4]]);
}
if ("”unqualified”".match(word)) {
result = result.concat([[50,2],[51,8]]);
}
if ("”xs:decimal”".match(word)) {
result = result.concat([[50,2]]);
}
if ("”xs:string”".match(word)) {
result = result.concat([[50,2],[51,4]]);
}
return result; }