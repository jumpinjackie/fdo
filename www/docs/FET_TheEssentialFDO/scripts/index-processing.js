function SearchForCompleteMatch(indexesDocument,searchKey)
{
    links = indexesDocument.links;

	top.highlightedIndexesNum = 0;
    for(i = 0; i < links.length; i++)
    {
        if(links[i].innerHTML == searchKey)
        {
        	top.highlightedIndexes[top.highlightedIndexesNum] = links[i].id;
        	top.highlightedIndexesNum++;
            links[i].focus();
            return true;
        }
    }
    return false;
}

function SearchForPartialMatch(indexesDocument,searchKey)
{
    var links = indexesDocument.links;
    var link;
    regexpr = new RegExp("^" + searchKey, "i");

	top.highlightedIndexesNum = 0;
    for(i = 0; i < links.length; i++)
    {
        link = links[i]
        if(searchKey && link.innerHTML.match(regexpr) )
        {
        	top.highlightedIndexes[top.highlightedIndexesNum] = link.id;
        	top.highlightedIndexesNum++;
            link.className='highlighted';
        }
    }
}

function UnhighLightAll(document)
{
    for(i = 0; i < top.highlightedIndexesNum; i++)
    {
        document.getElementById(top.highlightedIndexes[i]).blur();
        document.getElementById(top.highlightedIndexes[i]).className="unhighlighted";
    }
    top.highlightedIndexesNum=0;
    top.highlightedIndex=-1;
}

function mouseEvent(event,selectedIndexId)
{
//    alert(top.highlightedIndexes);
    if (event.button !=0) {
        UnhighLightAll(document);
        document.getElementById(selectedIndexId).className="highlighted";
        top.highlightedIndexes[top.highlightedIndexesNum] = selectedIndexId;
        top.highlightedIndexesNum++;
    } else highlightCurrent(selectedIndexId);
}

function highlightCurrent(selectedIndexId)
{
        UnhighLightAll(document);
    	top.highlightedIndexesNum = 0;
        top.highlightedIndexes[top.highlightedIndexesNum] = selectedIndexId;
        top.highlightedIndexesNum++;
}

function highlightIndex(searchKey)
{
    var indexesDocument = getIndexesDocument();
    UnhighLightAll(indexesDocument);
    SearchForPartialMatch(indexesDocument,searchKey);
    showNext(indexesDocument);
}

function OpenHighlightedIndex(indexesDocument,indexNumber)
{
	if(indexNumber > -1 && indexNumber < top.highlightedIndexesNum)
	{
        window.open(indexesDocument.getElementById(top.highlightedIndexes[indexNumber]).href, "content");
	}
}

function showNext(theIndexesDocument) {
    var indexesDocument = theIndexesDocument !=null ? theIndexesDocument : getIndexesDocument();
    if (top.highlightedIndexesNum > 0) {
        if (top.highlightedIndex < top.highlightedIndexesNum-1) {
            top.highlightedIndex++;
        } else {
            top.highlightedIndex=0;
        }
        indexesDocument.getElementById(top.highlightedIndexes[top.highlightedIndex]).focus();
        OpenHighlightedIndex(indexesDocument,top.highlightedIndex);
    }
}

function getIndexesDocument() {
    if(navigator.appName == "Netscape")
    {
        return document.getElementById("indexes").contentDocument;
    }
    else if(navigator.appName == "Microsoft Internet Explorer")
    {
        return document.frames("indexes").document;
    }
    else alert("Unsupported browser !");

}