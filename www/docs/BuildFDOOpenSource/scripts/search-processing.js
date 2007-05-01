var searchResultArray;
var timer;
var timer1;

var exchangeBuffer = new Array(3);

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
//    timer = new Date().getTime();
//    timer1 = new Date().getTime();
    document.getElementById("searchList").length = 0;
    var preRequest = PrepareRequest(request);
    var request1 = NormalizeSpace(preRequest);
    var preRequestArr = request1.split(" ");
    var requestArr = new Array();
    for (var i = 0; i < preRequestArr.length;i++) {
        var isStopWord = false
        var isSameWord = false
        for (var j=0; j< StopWords.length; j++) {
            if (preRequestArr[i] == StopWords[j]) {
                isStopWord = true;
                break;
            }
        }
        for (var j=0; j<i; j++) {
            if (preRequestArr[j] == preRequestArr[i]) {
                isSameWord = true;
                break;
            }
        }
        if (!isStopWord && !isSameWord) {
            requestArr[requestArr.length] = preRequestArr[i];
        }
    }
    top.searchWordList = requestArr;
    var regexp = new Array(requestArr.length);
    var isFullTextSearch = (request.indexOf('"') == 0 && request.lastIndexOf('"') == (request.length-1));

    var searchResultsTemp = new Array();
    setTimeout("showSearchMessage('Search in progress...')",10);


    searchResultArray = new Array();
    var topicNumber = 0;
    var topicRank = 0;

    for(i = 0; i < regexp.length; i++)
    {
//        var preparedRequest = PrepareRequest(requestArr[i]);
//        regexp[i]=GenerateRegExp(preparedRequest);
        regexp[i] = GenerateRegExp(requestArr[i]);
    }
    for (j = 0; j < regexp.length; j++)
    {
        occurences = getWordOccurences(regexp[j]);
        for (i = 0; i< occurences.length;i++) {
            topicNumber = occurences[i][0];
            topicRank = occurences[i][1];
            if (searchResultsTemp[topicNumber] == null) {
                searchResultsTemp[topicNumber] = new Array(2);
                searchResultsTemp[topicNumber][0] = topicRank;
                searchResultsTemp[topicNumber][1] = 1;
            } else {
                searchResultsTemp[topicNumber][0] += topicRank;
                searchResultsTemp[topicNumber][1] +=1;
            }
        }
/*
        for(i = 0; i < TopicsWordsList.length; i++)
        {
            if(TopicsWordsList[i])
                if(TopicsWordsList[i][0].match(regexp[j])) {
                    for(k = 0; k < TopicsWordsList[i][1].length; k++) {
                        topicNumber = TopicsWordsList[i][1][k][0];
                        topicRank = TopicsWordsList[i][1][k][1]
                        if (searchResultsTemp[topicNumber] == null) {
                            searchResultsTemp[topicNumber] = new Array(2);
                            searchResultsTemp[topicNumber][0] = topicRank;
                            searchResultsTemp[topicNumber][1] = 1;
                        } else {
                            searchResultsTemp[topicNumber][0] += topicRank;
                            searchResultsTemp[topicNumber][1] +=1;
                        }
                    }
                }
        }
*/
    }

//    timer1 = new Date().getTime()-timer1;
    var j = 0;
    var currentResult;
    var currentSearchMethod = top.searchMethod
    var t1,t2,t3
    for(var i in searchResultsTemp)
    {
        currentResult = searchResultsTemp[i];
        if(currentResult[0] > 0) {
            if (isFullTextSearch && currentResult[1] >= requestArr.length) {
                searchResultArray[j] = new Array(3);
                searchResultArray[j][0]=currentResult[0];
                searchResultArray[j][1]=i;
                searchResultArray[j][2]=0;
                j++;
            } else if (currentSearchMethod == "or" || (currentSearchMethod == "and" && currentResult[1] >= requestArr.length)) {
                searchResultArray[j] = new Array(3);
                searchResultArray[j][0]=currentResult[0];
                searchResultArray[j][1]=i;
                searchResultArray[j][2]=1;
                j++;
            }
/*
            switch (top.searchMethod) {
                case "or": {
                    searchResultArray[j] = new Array(4);
                    searchResultArray[j][0]=searchResultsTemp[i][0];
                    searchResultArray[j][1]=searchResultsTemp[i][1];
                    searchResultArray[j][2]=i;
                    searchResultArray[j][3]=1;
                    if (isFullTextSearch) {
                        searchResultArray[j][3]=0;
                    }
                    j++;
                    break;
                }
                case "and": {
                    if (searchResultsTemp[i][1] >= requestArr.length) {
                        searchResultArray[j] = new Array(4);
                        searchResultArray[j][0]=searchResultsTemp[i][0];
                        searchResultArray[j][1]=searchResultsTemp[i][1];
                        searchResultArray[j][2]=i;
                        searchResultArray[j][3]=1;
                        if (isFullTextSearch) {
                            searchResultArray[j][3]=0;
                        }
                        j++;
                        break;
                    }
                }
            }
*/

        }
    }

//    alert((new Date().getTime()-timer) + "; "+ timer1+": "+searchResultsTemp.length+": ");

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
        for (i = 0; i <searchResultArray.length; i++) {
            if (searchResultArray[i][1] >= requestArr.length) {
                top.setPageLoaded(0);
                top.currentSearchPage = i;
                if(navigator.appName == "Netscape")
                {
                    testFrame.contentDocument.location.replace(TopicFiles[searchResultArray[i][2]]);
                }
                else if(navigator.appName == "Microsoft Internet Explorer")
                {
                    testFrame.document.location.replace(TopicFiles[searchResultArray[i][2]]);
                }
                else alert("Unsupported browser !");
                syncWithBrowser();
                break;
            }
        }
    }

    if (!isFullTextSearch) {
        setTimeout("displaySearchResult()",10);
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

    quicksort(0,searchResultArray.length,true);

    var searchList = document.getElementById("searchList");

    searchList.length = 0;
    for(i = 0; i < searchResultArray.length; i++)
    {
        if(searchResultArray[i][2] > 0 && TopicFiles[searchResultArray[i][1]] != "")
        {
            isResultsDislpayed = true;
            var element = document.createElement("OPTION");
            element.text = searchResultArray[i][0]+": "+TopicTitles[searchResultArray[i][1]];
            element.value = TopicFiles[searchResultArray[i][1]];
            try
            {
                searchList.add(element, null);
                // standards compliant
            }
            catch(ex)
            {
                searchList.add(element);
                // IE only
            }
        }
    }

    if (searchList.length == 0) {
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
        for (i = top.currentSearchPage+1; i < searchResultArray.length; i++) {
            if (searchResultArray[i][1] >= top.searchWordList.length) {
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
                testFrame.contentDocument.location.replace(TopicFiles[searchResultArray[nextSearchPage][2]]);
            }
        }
        else if(navigator.appName == "Microsoft Internet Explorer")
        {
            testFrame = self.document.frames("hiddenFrame");
            text = testFrame.document.body.innerHTML;
            if (nextSearchPage != 0) {
                top.setPageLoaded(0);
                top.currentSearchPage = nextSearchPage;
                testFrame.document.location.replace(TopicFiles[searchResultArray[nextSearchPage][2]]);
            }
        }
        else alert("Unsupported browser !");

        var regexp = /<[^>]+>/g;
        text = text.replace(regexp,"");
        if (text.indexOf(top.searchRequest.substring(1,top.searchRequest.length-1)) != -1) {
            searchResultArray[nowSearchingPage][3] = 1;
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
    if (searchList.selectedIndex > -1 ) {
        viewedDocument = open(searchList.options[searchList.selectedIndex].value, "content");
    }
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

function quicksort(m, n, desc) {
    if (n <= m + 1) return;
    if ((n - m) == 2) {
        if (compare(get(n - 1), get(m), desc)) exchange(n - 1, m);
        return;
    }
    i = m + 1;
    j = n - 1;
    if (compare(get(m), get(i), desc)) exchange(i, m);
    if (compare(get(j), get(m), desc)) exchange(m, j);
    if (compare(get(m), get(i), desc)) exchange(i, m);
    pivot = get(m);
    while (true) {
        j--;
        while (compare(pivot, get(j), desc)) j--;
        i++;
        while (compare(get(i), pivot, desc)) i++;
        if (j <= i) break;
        exchange(i, j);
    }
    exchange(m, j);
    if ((j - m) < (n - j)) {
        quicksort(m, j, desc);
        quicksort(j + 1, n, desc);
    } else {
        quicksort(j + 1, n, desc);
        quicksort(m, j, desc);
    }
}

function get(i) {
    return searchResultArray[i][0];
}

function compare(val1, val2, desc) {
    return (desc) ? val1 > val2 : val1 < val2;
}

function exchange(i, j) {
    // exchange adjacent
    // var tResult = new Array(4);
    exchangeBuffer = searchResultArray[i];
    searchResultArray[i] = searchResultArray[j];
    searchResultArray[j] = exchangeBuffer;
}