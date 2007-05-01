var tocImages =  {'ctrlImageLine':'images/void.gif',
                  'ctrlImageVoid':'images/void.gif',
                  'ctrlImage0':'images/void.gif',
                  'ctrlImage1':'images/void.gif',
                  'ctrlImage2':'images/void.gif',
                  'ctrlImage8':'images/plus.gif',
                  'ctrlImage9':'images/plus.gif',
                  'ctrlImage10':'images/plus.gif',
                  'ctrlImage12':'images/minus.gif',
                  'ctrlImage13':'images/minus.gif',
                  'ctrlImage14':'images/minus.gif',
                  'book-opened':'images/book-opened.gif',
                  'book-closed':'images/book-closed.gif',
                  'page':'images/page.gif'};
var childTrees=[];

// ------------------------------------------ Functions for nodes expansion

var chgNodeResult=0;

function expandTOCNode(nodeId)
{
    if(this.tocPointer != null && this.tocPointer.curViewed != null && this.tocPointer.curViewed.nodeId == nodeId)
        return;
    chgNodeResult = -1;
    expandTOCNodeChilds(nodeId, this);
    return chgNodeResult;
}

function expandTOCNodeChilds(nodeId, parentNode)
{
    for(var i = 0; i < parentNode.childNodes.length; i++)
    {
        if(parentNode.childNodes[i].isOpened) parentNode.childNodes[i].OpenCloseNode(false);
        if(nodeId == parentNode.childNodes[i].nodeId)
        {
            ExpandNode(parentNode.childNodes[i]);
            parentNode.childNodes[i].select(false);
            chgNodeResult = nodeId;
            return;
        };
        expandTOCNodeChilds(nodeId, parentNode.childNodes[i]);
    };
}

function ExpandNode(nodePointer)
{
    if(nodePointer.nodeLevel == 0) return;
    ExpandNode(nodePointer.parent);
    nodePointer.parent.OpenCloseNode(false);
}

// ------------------------------------------ TOCNode class

function isFirstNode()
{
    if((this.nodeLevel == 0) && (this.childNumber == 0) && (!this.isLastNode())) return true;
    else return false;
}

function isLastNode()
{
    if(this.childNumber == this.parent.childNodes.length - 1) return true;
    else return false;
}

function TOCNode(parent, childNumber)
{
    this.nodeLevel = parent.nodeLevel + 1;
    if(this.nodeLevel != 0) this.items = parent.items[childNumber + 3];
    else this.items = parent.items[childNumber];
    if(!this.items) return;
    this.tocPointer = parent.tocPointer;
    this.parent = parent;
    this.childNumber = childNumber;
    this.isOpened = false;
    this.nodeId = this.tocPointer.tocNodes.length;
    this.tocPointer.tocNodes[this.nodeId] = this;
    parent.childNodes[childNumber] = this;
    this.childNodes = [];
    for(var i = 0; i < this.items.length - 2; i++) new TOCNode(this, i);
    this.NodeImage = SetNodeImage;
    this.OpenCloseNode = OpenCloseNode;
    this.select = ViewNode;
    this.init = InitializeNode;
    this.isLastNode = isLastNode;
    this.isFirstNode = isFirstNode;
}

function SetNodeImage(setCtrlImage)
{
    if(setCtrlImage)
    {
        var flags = 0;
        if(this.childNodes.length) flags |= 0x8;
        if(this.childNodes.length && this.isOpened) flags |= 0x4;
        if(this.isLastNode()) flags |= 0x2;
        if(this.isFirstNode()) flags |= 0x1;
        return tocImages['ctrlImage' + flags];
    }
    else
    {
        currentState = this.items[2];
        if(this.isOpened && currentState == 'book-closed')
            currentState = 'book-opened';
        if(!this.isOpened && currentState == 'book-opened')
            currentState = 'book-closed';
        return tocImages[currentState];
    }
}

function OpenCloseNode(currentState)
{
    var htmlContent = [];
    var childDiv = document.getElementById('divtree' + this.nodeId);
    if(!childDiv) return;
    if(!childDiv.innerHTML)
    {
        for(var i = 0; i < this.childNodes.length; i++)
            htmlContent[i] = this.childNodes[i].init();
        childDiv.innerHTML = htmlContent.join('');
    }
    if(currentState) childDiv.style.display = 'none';
    else childDiv.style.display = 'block';
    this.isOpened = !currentState;
    var controlImage = document.getElementById('ctrlImage' + this.nodeId),
        bookImage = document.getElementById('bookImage' + this.nodeId);
    if(controlImage) controlImage.src = this.NodeImage(true);
    if(bookImage) bookImage.src = this.NodeImage(false);
}

function ViewNode(onoffPrevious)
{
    if(!onoffPrevious)
    {
        var viewedNode = this.tocPointer.curViewed;
        this.tocPointer.curViewed = this;
        if(viewedNode) viewedNode.select(true);
        top.changeCurrPageNumber(this.nodeId);
    }
    var bookImage = document.getElementById('bookImage' + this.nodeId);
    if(bookImage) bookImage.src = this.NodeImage();
    document.getElementById('nodeAhchor' + this.nodeId).className = onoffPrevious ? 'toc_normal' : 'toc_selected';
    return Boolean(this.items[1]);
}

function InitializeNode()
{
    var parentNodesHTML=[], parentNode=this.parent;
    var navImage;
    var resNodeHTML;
    for(var i = this.nodeLevel; i > 0; i--)
    {
        if(parentNode.isLastNode()) navImage = 'ctrlImageVoid';
        else navImage = 'ctrlImageLine';
        parentNodesHTML[i] = '<img src="' + tocImages[navImage] + '" border="0" align="absbottom">';
        parentNode=parentNode.parent;
    }
    resNodeHTML = '<a name="#nodeAhchor' + this.nodeId + '"></a><table cellpadding="0" cellspacing="0" border="0"><tr><td nowrap>';
    resNodeHTML += parentNodesHTML.join('');
    if(this.childNodes.length)
    {
        resNodeHTML += '<a href="javascript:childTrees[' + this.tocPointer.nodeId + '].toggle(' + this.nodeId + ')">';
        resNodeHTML += '<img src="' + this.NodeImage(true) + '" border="0" align="absbottom" id="ctrlImage' + this.nodeId+'">';
        resNodeHTML += '</a>';
    }
    else
    {
        resNodeHTML += '<img src="' + this.NodeImage(true) + '" border="0" align="absbottom">';
    }
    resNodeHTML += '<a href="' + this.items[1] + '" target="content" onclick="top.addTOCHistoryItem(' + this.nodeId + ');return childTrees[' + this.tocPointer.nodeId + '].select(' + this.nodeId + ');" ondblclick="childTrees[' + this.tocPointer.nodeId + '].toggle(' + this.nodeId + ')" class="treeitem" id="nodeAhchor' + this.nodeId + '">';
    resNodeHTML += '<img src="' + this.NodeImage() + '" border="0" align="absbottom" id="bookImage' + this.nodeId + '" class="treeitem">';
    resNodeHTML += this.items[0] + '</td></tr></table>';
    if(this.childNodes.length)
        resNodeHTML += '<div id="divtree' + this.nodeId + '" style="display:none"></div>';
    return resNodeHTML;
}

// --------------------------------------------------------


// ------------------------------------------ TOC class
function TOCSelect(nodeId)
{
    return this.tocNodes[nodeId].select();
}

function TOCToggle(nodeId)
{
    var node = this.tocNodes[nodeId];
    node.OpenCloseNode(node.isOpened);
}

function TOC(tocItems)
{
    this.items = tocItems;
    this.tocPointer = this;
    this.tocNodes = [];
    this.curViewed = null;
    this.nodeLevel = -1;

    this.select = TOCSelect;
    this.toggle = TOCToggle;

    this.childNodes = [];
    for(var i = 0; i < tocItems.length; i++) new TOCNode(this, i);

    this.nodeId = childTrees.length;
    childTrees[this.nodeId] = this;
    for(var i = 0; i < this.childNodes.length; i++)
        document.write(this.childNodes[i].init());
    this.expandTOCNode = expandTOCNode;
}

// ----------------------------------------------------
