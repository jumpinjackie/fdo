declare namespace mdoc = "java:com.idiominc.ws.opentopic.utils.saxon8.MDoc";
declare namespace opentopic = "http://www.idiominc.com/opentopic";

declare function opentopic:buildPath($href, $originalHref)
{
 let $isContainsDots := contains($href, ".."),
     $isStartsFromRoot := starts-with($originalHref, "/")
 return
 if ($isStartsFromRoot) then $originalHref
 else if ($isContainsDots) then
 let $beforeDots := substring-before($href, "/.."),
      $afterDots := substring-after($href, "/.."),
      $toc := tokenize($beforeDots, "/")[last()],
      $tocLen := string-length($toc),
      $beforeDotsLen := string-length($beforeDots),
      $parentDir := substring($beforeDots, 0, $beforeDotsLen - $tocLen),
      $res := concat($parentDir, $afterDots)
 return opentopic:buildPath($res, "")
 else $href
};

declare function opentopic:resolve_conref($conref, $parentIDs as xs:string, $thisAttributes)
{
 let $elementID := substring-after(substring-after($conref, "#"), "/"),
      $parents := concat($parentIDs, ";", substring-after($conref, "#"))
 return
  if (contains($parentIDs, substring-after($conref, "#"))) then ()
  else if ($elementID) then
   let $topicID := substring-before(substring-after($conref, "#"), "/"),
       $nodes := mdoc:multiDocument("/local/users/cmsuser/tmp/build_temp//ISD/Marble-BICW/English/2006_11_10_09_43_47_840/source/")//*[@Id = $topicID]//*[@Id = $elementID]
   return
    if ($nodes) then opentopic:process_root_element($nodes, (), 0, $parents, $thisAttributes)
    else ()
  else
   let $topicID := substring-after($conref, "#"),
       $thisID := $thisAttributes[name() = 'Id'],
       $nodes := mdoc:multiDocument("/local/users/cmsuser/tmp/build_temp//ISD/Marble-BICW/English/2006_11_10_09_43_47_840/source/")//*[@Id = $topicID]

   return
    if ($nodes) then
    if (count($nodes) = 1) then opentopic:process_root_element($nodes, (), 0, $parents, $thisAttributes)
    else (trace("Conref cannot be included into map. Two or more elements with targetId was found", "stdout"),
        trace(concat(" Destination Element Id: ",$topicID),"stdout"),
        trace(concat("           Conref path : ", $conref),"stdout"),
        trace(concat("    Current element Id : ", $thisID),"stdout"),
        trace("Elements with destination Id was found in following files: ","stdout"),
        opentopic:show_wrong_entries($nodes),
        trace("Processing continued with first acceptable element: ","stdout"),
        opentopic:process_root_element($nodes[not(@*[name() = 'conref'])][1], (), 0, $parents, $thisAttributes))
    else ()
};

declare function opentopic:show_wrong_entries($nodes)
{
  for $node in $nodes return
    trace(concat(" File: ",concat(base-uri($node),'/#',$node/@Id)), "stdout")
};

declare function opentopic:process_root_element($rootelement, $topicref, $inserttopicref as xs:integer, $parentIDs as xs:string, $conrefElementAttributes)
{
 let $conref := $rootelement/@conref,
     $thisAttributes := $rootelement/@*[not(name()='conref')],
     $isLinkTypeElement := contains($rootelement/@class, " topic/link ") or contains($rootelement/@class, " topic/xref ")
 return
 if ($conref) then opentopic:resolve_conref($conref, $parentIDs, $thisAttributes)
 else
  let $elementName := name($rootelement)
  return
  if ($elementName = "#text") then $rootelement
  else if ($elementName = "#cdata-section") then $rootelement
  else if (contains($elementName, "#")) then element {"auxiliary_element"} {}
  else if ($elementName = "") then $rootelement
  else element {name($rootelement)}
  {
   if ($isLinkTypeElement) then
    for $attr in $rootelement/@*[not(name() = 'href')] return $attr,
    for $href in $rootelement/@href return attribute href
     {
      if (contains($href, "#") or ($rootelement/@scope = 'external')) then $href
      else
       let $linkFilePath := base-uri($rootelement),
           $linkFileName := tokenize($linkFilePath, "/")[last()],
           $linkFilePathLen := string-length($linkFilePath),
           $linkFileNameLen := string-length($linkFileName),
           $preFilePath := concat(substring($linkFilePath, 0, $linkFilePathLen - $linkFileNameLen), "/", $href),
           $linkedFilePath := opentopic:buildPath($preFilePath, $href),
           $linkedTopicId := mdoc:multiDocument($linkedFilePath)/*[contains(@class, " topic/topic ")]/@Id
       return concat($href, "/#", $linkedTopicId)
     }
   else if ($conrefElementAttributes) then
    opentopic:process_conref_attributes($rootelement/@*, $conrefElementAttributes)
   else for $attr in $rootelement/@* return $attr,
   for $childs in ($rootelement/* | $rootelement/text()) return opentopic:process_root_element($childs, $topicref, 0, $parentIDs,()),
   for $childtopicref in $topicref/*[contains(@class, ' map/topicref ')]
   return
   if ($inserttopicref = 0) then ()
   else if (contains($childtopicref/@class, ' mapgroup/topichead ')) then opentopic:process_topichead($childtopicref)
   else opentopic:process_topicref($childtopicref)
  }
};

declare function opentopic:process_topicref($topicref)
{
 let $href := $topicref/@href,
     $topicID := substring-after($href, "#"),
     $print := $topicref/@print
 return
  if ('chm' = 'pdf' and $print = 'no') then ()
  else if (not($topicref/@href)) then
   for $childtopicref in $topicref/*[contains(@class, ' map/topicref ')]
    return
   if (contains($childtopicref/@class, ' mapgroup/topichead ')) then opentopic:process_topichead($childtopicref)
   else opentopic:process_topicref($childtopicref)
  else if ($topicID = "") then
   let $path := opentopic:buildPath(concat("/local/users/cmsuser/tmp/build_temp//ISD/Marble-BICW/English/2006_11_10_09_43_47_840/source/", $href), $href)
   for $node in mdoc:multiDocument($path)/*
    return opentopic:process_root_element($node, $topicref, 1, "", ())
  else
   for $node in mdoc:multiDocument("/local/users/cmsuser/tmp/build_temp//ISD/Marble-BICW/English/2006_11_10_09_43_47_840/source/")//*[@Id = $topicID]
    return opentopic:process_root_element($node, $topicref, 1, "", ())
};

declare function opentopic:generate_topichead_id($sequence, $result)
{
 if (empty($sequence)) then $result
 else opentopic:generate_topichead_id(subsequence($sequence, 2), concat($result, string(subsequence($sequence, 1, 1)), '-'))
};

declare function opentopic:process_topichead($topicref)
{
 let $print := $topicref/@print,
     $title := $topicref/@navtitle,
     $id := string-to-codepoints(string($title))
 return
  if ('chm' = 'pdf' and $print = 'no') then ()
  else
   <topic class="- topic/topic " id="{opentopic:generate_topichead_id($id, '')}">
   {
    <title class="- topic/title ">{string($title)}</title>,
    <body class="- topic/body "/>,
    for $childtopicref in $topicref/*[contains(@class, ' map/topicref ')]
     return
      if (contains($childtopicref/@class, ' mapgroup/topichead ')) then opentopic:process_topichead($childtopicref)
      else opentopic:process_topicref($childtopicref)
   }
   </topic>
};

declare function opentopic:process_map_info($mapelement)
{
  let $print := $mapelement/@print
  return
   if ('chm' = 'pdf' and $print = 'no') then ()
   else
    element {name($mapelement)}
    {
     for $attr in $mapelement/@* return $attr,
     if (contains($mapelement/@class, " map/topicref ")) then
      let $href := $mapelement/@href,
           $topicID := substring-after($href, "#"),
           $path := opentopic:buildPath(concat("/local/users/cmsuser/tmp/build_temp//ISD/Marble-BICW/English/2006_11_10_09_43_47_840/source/", $href), $href)
      return
       if (not($href) or $href = "") then ()
       else attribute id
       {
        if ($topicID = "") then mdoc:multiDocument($path)/*[contains(@class, " topic/topic ") and not(contains(@class, " bkinfo/bkinfo "))][1]/@Id
        else $topicID
       }
     else (),
     for $child in $mapelement/* return opentopic:process_map_info($child)
    }
};

declare function opentopic:process_conref_attributes($elementAttrs, $conrefAttrs)
{
    for $attrs1 in $elementAttrs
        let $attName := name($attrs1)
        return
            if (not($conrefAttrs[name() = $attName])) then $attrs1
            else (),
    for $attrs2 in $conrefAttrs return $attrs2
};

for $map in mdoc:multiDocument("/local/users/cmsuser/tmp/build_temp//ISD/Marble-BICW/English/2006_11_10_09_43_47_840/source/")/*[contains(@class, " map/map ")][not(contains(substring-after(base-uri(.), "/local/users/cmsuser/tmp/build_temp//ISD/Marble-BICW/English/2006_11_10_09_43_47_840/source/"), '/'))]
return
element {name($map)}
{
 for $attr in $map/@* return $attr,
 <opentopic:map xmlns:opentopic = "http://www.idiominc.com/opentopic">
 {
  for $mapelem in $map/* return opentopic:process_map_info($mapelem)
 }
 </opentopic:map>,
 for $topicref in $map/*[contains(@class, ' map/topicref ') and not(contains(@class, ' bookmap/front-matter '))]
  return
   if (contains($topicref/@class, ' mapgroup/topichead ')) then opentopic:process_topichead($topicref)
   else opentopic:process_topicref($topicref)
}
