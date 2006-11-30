function GenerateRegExp(request)
{
    var regexp = /(\x20\x20)/g;
    while(request.match(regexp)) request = request.replace(regexp,"\x20");
    regexp = /(^\x20)|(\x20$)/g;
    while(request.match(regexp)) request = request.replace(regexp,"");
    searchExpr = new RegExp(request, "i");
    return searchExpr;
}

function NormalizeSpace(string)
{
    var regexp = /(\x20\x20)/g;
    while(string.match(regexp)) string = string.replace(regexp,"\x20");
    regexp = /(^\x20)|(\x20$)/g;
    while(string.match(regexp)) string = string.replace(regexp,"");
    return string;
}

function CreateSearchResultsArray()
{
    var searchResults = new Array(TopicFiles.length);
    for(i = 0; i < searchResults.length; i++) {
        searchResults[i] = new Array(2);
        searchResults[i][0] = 0;
        searchResults[i][1] = 0;
    }
    return searchResults;
}

function PrepareRequest(request)
{
    request = request.replace(/\\/g, "\\\\");
    request = request.replace(/[\[ | \^ | \$ | \. | \| | \+ | \( | \) | ` | ~ | ! | # | % | & | - | _ | = | \] | { | } | ; | ' | " | < | > | ,]/g, " ");
    request = request.replace(/\*/g, "[^\\s]*");
    request = request.replace(/\?/g, ".");
    return request;
}

function SearchString(request)
{
    document.getElementById("searchList").length = 0;
    var preRequest = PrepareRequest(request);
    var request1 = NormalizeSpace(preRequest);
    var preRequestArr = request1.split(" ");
    var requestArr = new Array();
    for (i = 0; i < preRequestArr.length;i++) {
        var isStopWord = false
        for (j=0; j< StopWords.length; j++) {
            if (preRequestArr[i] == StopWords[j]) {
                isStopWord = true;
            }
        }
        if (!isStopWord) {
            requestArr[requestArr.length] = preRequestArr[i];
        }
    }
    top.searchWordList = requestArr;
    var regexp = new Array(requestArr.length);
    var isFullTextSearch = (request.indexOf('"') == 0 && request.lastIndexOf('"') == (request.length-1));
    searchResultsTemp = CreateSearchResultsArray();

    showSearchMessage("Search in progress...");

    top.searchResults = new Array();

    for(i = 0; i < regexp.length; i++)
    {
//        var preparedRequest = PrepareRequest(requestArr[i]);
//        regexp[i]=GenerateRegExp(preparedRequest);
        regexp[i] = GenerateRegExp(requestArr[i]);
    }
    for (j = 0; j < regexp.length; j++)
    {
        for(i = 0; i < TopicsWordsList.length; i++)
        {
            if(TopicsWordsList[i])
                if(TopicsWordsList[i][0].match(regexp[j])) {
                    for(k = 0; k < TopicsWordsList[i][1].length; k++) {
                        searchResultsTemp[TopicsWordsList[i][1][k][0]][0] += TopicsWordsList[i][1][k][1];
                        searchResultsTemp[TopicsWordsList[i][1][k][0]][1] +=1;
                    }
                }
        }
    }

    for(i = 0; i < searchResultsTemp.length; i++)
    {
        if(searchResultsTemp[i][0] > 0) {
            var j = top.searchResults.length;
            switch (top.searchMethod) {
                case "or": {
                    top.searchResults[j] = new Array(4);
                    top.searchResults[j][0]=searchResultsTemp[i][0];
                    top.searchResults[j][1]=searchResultsTemp[i][1];
                    top.searchResults[j][2]=i;
                    top.searchResults[j][3]=1;
                    if (isFullTextSearch) {
                        top.searchResults[j][3]=0;
                    }
                    break;
                }
                case "and": {
                    if (searchResultsTemp[i][1] >= requestArr.length) {
                        top.searchResults[j] = new Array(4);
                        top.searchResults[j][0]=searchResultsTemp[i][0];
                        top.searchResults[j][1]=searchResultsTemp[i][1];
                        top.searchResults[j][2]=i;
                        top.searchResults[j][3]=1;
                        if (isFullTextSearch) {
                            top.searchResults[j][3]=0;
                        }
                        break;
                    }
                }
            }

        }
    }


    if (isFullTextSearch) {
        top.searchRequest = request;
        var testFrame;
        if(navigator.appName == "Netscape")
        {
            testFrame = document.getElementById("hiddenFrame");
        }
        else if(navigator.appName == "Microsoft Internet Explorer")
        {
            testFrame = self.document.frames("hiddenFrame");
        }
        else alert("Unsupported browser !");
        for (i = 0; i <top.searchResults.length; i++) {
            if (top.searchResults[i][1] >= requestArr.length) {
                top.setPageLoaded(0);
                top.currentSearchPage = i;
                if(navigator.appName == "Netscape")
                {
                    testFrame.contentDocument.location.replace(TopicFiles[top.searchResults[i][2]]);
                }
                else if(navigator.appName == "Microsoft Internet Explorer")
                {
                    testFrame.document.location.replace(TopicFiles[top.searchResults[i][2]]);
                }
                else alert("Unsupported browser !");
                syncWithBrowser();
                break;
            }
        }
    }

    if (!isFullTextSearch) {
        displaySearchResult();
    }
}

function showSearchMessage(message) {
    document.getElementById("searchList").length = 0;
    var element = document.createElement("OPTION");
    element.text = message;
    element.value = "";
    document.getElementById("searchList")[document.getElementById("searchList").length] = element;
}

function displaySearchResult() {
    var isResultsDislpayed = false;
    document.getElementById("searchList").length = 0;

    for (i = 0; i < top.searchResults.length-1; i++)
        for (j=i+1; j <top.searchResults.length; j++) {
            if (top.searchResults[i][0] < top.searchResults[j][0]) {
                var tResult = new Array(4);
                tResult = top.searchResults[i];
                top.searchResults[i] = top.searchResults[j];
                top.searchResults[j] = tResult;
            }
        }
    var showedLinks = "";
    for(i = 0; i < top.searchResults.length; i++)
    {
        if(top.searchResults[i][3] > 0 && TopicFiles[top.searchResults[i][2]] != "" &&
           showedLinks.indexOf(TopicFiles[top.searchResults[i][2]]) == -1)
        {
            isResultsDislpayed = true;
            var element = document.createElement("OPTION");
            element.text = top.searchResults[i][0]+": "+TopicTitles[top.searchResults[i][2]];
            element.value = TopicFiles[top.searchResults[i][2]];
            showedLinks += TopicFiles[top.searchResults[i][2]];
            document.getElementById("searchList")[document.getElementById("searchList").length] = element;
        }
    }

    if (!isResultsDislpayed) {
        showSearchMessage("Search gives no results");
    }
}

function syncWithBrowser() {
    if (top.isPageLoaded == 0) {
        setTimeout("syncWithBrowser()",100);
    } else {
        var testFrame;
        var text ="";
        var nextSearchPage = 0;
        var nowSearchingPage = top.currentSearchPage;
        for (i = top.currentSearchPage+1; i < top.searchResults.length; i++) {
            if (top.searchResults[i][1] >= top.searchWordList.length) {
                nextSearchPage = i;
                break;
            }
        }
        if(navigator.appName == "Netscape")
        {
            testFrame = document.getElementById("hiddenFrame");
            text = testFrame.contentDocument.body.innerHTML;
            if (nextSearchPage != 0) {
                top.setPageLoaded(0);
                top.currentSearchPage = nextSearchPage;
                testFrame.contentDocument.location.replace(TopicFiles[top.searchResults[nextSearchPage][2]]);
            }
        }
        else if(navigator.appName == "Microsoft Internet Explorer")
        {
            testFrame = self.document.frames("hiddenFrame");
            text = testFrame.document.body.innerHTML;
            if (nextSearchPage != 0) {
                top.setPageLoaded(0);
                top.currentSearchPage = nextSearchPage;
                testFrame.document.location.replace(TopicFiles[top.searchResults[nextSearchPage][2]]);
            }
        }
        else alert("Unsupported browser !");

        var regexp = /<[^>]+>/g;
        text = text.replace(regexp,"");
        if (text.indexOf(top.searchRequest.substring(1,top.searchRequest.length-1)) != -1) {
            top.searchResults[nowSearchingPage][3] = 1;
            //alert("found "+TopicTitles[nowSearchingPage])
        }

        if (nextSearchPage != 0) {
            setTimeout("syncWithBrowser()",100);
        } else displaySearchResult();
    }
}

function ViewDocument()
{
    var searchList = document.getElementById("searchList");
    viewedDocument = open(searchList.options[searchList.selectedIndex].value, "content");
}

function switchHighliting()
{
  if (top.highlightingEnabled) top.highlightingEnabled = false;
    else top.highlightingEnabled = true;
    var w = top.content;
    w.location.reload();
}

function switchSearchMethod(value)
{
    top.searchMethod = value;
}