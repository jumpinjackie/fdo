function SearchForCompleteMatchMosilla(searchKey)
{
    links = document.getElementById("indexes").contentDocument.links;

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

function SearchForCompleteMatchIE(searchKey)
{
    links = document.frames("indexes").document.links;

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

function SearchForPartialMatchMosilla(searchKey)
{
    links = document.getElementById("indexes").contentDocument.links;
    regexpr = new RegExp("^" + searchKey, "i");
    var str;

	top.highlightedIndexesNum = 0;
    for(i = 0; i < links.length; i++)
    {
        if(links[i].innerHTML.match(regexpr) && searchKey)
        {
        	top.highlightedIndexes[top.highlightedIndexesNum] = links[i].id;
        	top.highlightedIndexesNum++;
            links[i].focus();
            break;
        }
    }
}

function SearchForPartialMatchIE(searchKey)
{
    links = document.frames("indexes").document.links;
    regexpr = new RegExp("^" + searchKey, "i");
    var str;
	top.highlightedIndexesNum = 0;

    for(i = 0; i < links.length; i++)
    {
        if(links[i].innerHTML.match(regexpr) && searchKey)
        {
        	top.highlightedIndexes[top.highlightedIndexesNum] = links[i].id;
        	top.highlightedIndexesNum++;
            links[i].focus();
            break;
        }
    }
}

function UnhighLightAllMosilla(document)
{
    for(i = 0; i < top.highlightedIndexesNum; i++)
    {
        document.getElementById(top.highlightedIndexes[i]).blur();
        document.getElementById(top.highlightedIndexes[i]).className="unhighlighted";
    }
}

function UnhighLightAllIE(document)
{
    for(i = 0; i < top.highlightedIndexesNum; i++)
    {
        document.getElementById(top.highlightedIndexes[i]).blur();
        document.getElementById(top.highlightedIndexes[i]).className="unhighlighted";
    }
}

function mouseEvent(event,selectedIndexId)
{
//    alert(top.highlightedIndexes);
    if (event.button !=0) {
        UnhighLightAllMosilla(document);
        document.getElementById(selectedIndexId).className="highlighted";
        top.highlightedIndexes[top.highlightedIndexesNum] = selectedIndexId;
        top.highlightedIndexesNum++;
    } else highlightCurrent(selectedIndexId);
}

function highlightCurrent(selectedIndexId)
{
    if(navigator.appName == "Netscape")
    {
        UnhighLightAllMosilla(document);
    	top.highlightedIndexesNum = 0;
        top.highlightedIndexes[top.highlightedIndexesNum] = selectedIndexId;
        top.highlightedIndexesNum++;
    }
    else if(navigator.appName == "Microsoft Internet Explorer")
    {
        UnhighLightAllIE(document);
    	top.highlightedIndexesNum = 0;
        top.highlightedIndexes[top.highlightedIndexesNum] = selectedIndexId;
        top.highlightedIndexesNum++;
    }
    else alert("Unsupported browser !");
}

function highlightIndex(searchKey)
{
    if(navigator.appName == "Netscape")
    {
        UnhighLightAllMosilla(document.getElementById("indexes").contentDocument);
        if(!SearchForCompleteMatchMosilla(searchKey))
            SearchForPartialMatchMosilla(searchKey);
    }
    else if(navigator.appName == "Microsoft Internet Explorer")
    {
        UnhighLightAllIE(document.frames("indexes").document);
        if(!SearchForCompleteMatchIE(searchKey))
            SearchForPartialMatchIE(searchKey);
    }
    else alert("Unsupported browser !");
}

function OpenHighlightedIndex()
{
	if(top.highlightedIndexesNum > 0)
	{
	    if(navigator.appName == "Netscape")
    	{
			window.open(document.getElementById("indexes").contentDocument.links[top.highlightedIndexes[0]].href, "content");
		}
	    else if(navigator.appName == "Microsoft Internet Explorer")
		{
        	window.open(document.frames("indexes").document.getElementById(top.highlightedIndexes[0]).href, "content");
		}
	    else alert("Unsupported browser !");
	}
}