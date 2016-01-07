// copies head and link text from below the tabs into the banner
function setBannerText() {

    // set heading text
    var head_text_span = document.getElementById("head_text");
    var heading_ones = document.getElementsByTagName("h1");

    if (heading_ones[0])
    {

        var heading_text = heading_ones[0].firstChild.data;
        head_text_span.firstChild.data = heading_text;

    } else {

        head_text_span.firstChild.data = "nothing";

    }

    // set link text
    


}