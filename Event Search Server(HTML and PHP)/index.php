<?php
include 'geoHash.php';
$keyword = $category = $distance = $location = $segmentId = $eventId = $json_file = $venueName = "";
$json_file = "\" \"";
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    if (isset($_POST["eventId"])) {
        $eventId = $_POST["eventId"];
        $url = "https://app.ticketmaster.com/discovery/v2/events/" . $eventId . ".json?apikey=rB4AA7GAeP49iRe8B6qMXl0";
        $json_file = file_get_contents($url);
        echo $json_file;
        die;
    }
    if (isset($_POST["venueName"])) {
        $venueName = $_POST["venueName"];
        $url = "https://app.ticketmaster.com/discovery/v2/venues?apikey=rB4AA7GAeP49ivDzGWryXl0&keyword=" . $venueName;
        $json_file = file_get_contents($url);
        echo $json_file;
        die;
    }
    $keywordOri = $_POST["keyword"];
    $keyword = str_replace(" ", "+", test_input($keywordOri));
    $category = $_POST["category"];
    if ($category == "music") {
        $segmentId = "KZFzniwnSyZfZ7v7nJ";
    } elseif ($category == "sports") {
        $segmentId = "KZFzniwnSyZfZ7v7nE";
    } elseif ($category == "at") {
        $segmentId = "KZFzniwnSyZfZ7v7na";
    } elseif ($category == "film") {
        $segmentId = "KZFzniwnSyZfZ7v7nn";
    } elseif ($category == "miscellaneous") {
        $segmentId = "KZFzniwnSyZfZ7v7n1";
    }
    if (empty($_POST["distance"])) {
        $distance = 10;
    } else {
        $distance = $_POST["distance"];
    }
    if ($_POST["location"] == "textbox") {
        // call google API get geocode
        $location = $_POST["location_text"];
        $locationSend = str_replace(" ", "+", test_input($location));
        $googleUrl = "https://maps.googleapis.com/maps/api/geocode/json?address=".$locationSend."&key=AIzaSyAHzLmHsyVpDJAdB9Itzz78M";
        $google_json = file_get_contents($googleUrl);
        $geo_json = json_decode($google_json);
        if (property_exists($geo_json, 'results')){
            $lat = $geo_json->{'results'}[0]->{'geometry'}->{'location'}->{'lat'};
            $lng = $geo_json->{'results'}[0]->{'geometry'}->{'location'}->{'lng'};
            $location = array($lat,$lng);
            if (is_numeric($distance)){
                $geohash = encode($location[0], $location[1]);
                $url = "https://app.ticketmaster.com/discovery/v2/events.json?apikey=&keyword=" . $keyword . "&segmentId=" . $segmentId . "&radius=" . $distance . "&unit=miles&geoPoint=" . $geohash;
                $json_file = file_get_contents($url);
            } elseif (!empty($keyword)){
                $json_file = "\"none\"";
            }
        }
        else {
            $json_file = "\"none\"";
        }
    }
    else {
        $location = explode(",", $_POST["location"]);
        if (is_numeric($distance)){
            $geohash = encode($location[0], $location[1]);
            $url = "https://app.ticketmaster.com/discovery/v2/events.json?apikey=rB4AA7GAeP4l0&keyword=" . $keyword . "&segmentId=" . $segmentId . "&radius=" . $distance . "&unit=miles&geoPoint=" . $geohash;
            $json_file = file_get_contents($url);
        } elseif (!empty($keyword)){
            $json_file = "\"none\"";
        }
    }
}
function test_input($data)
{
    $data = trim($data);
    $data = stripslashes($data);
    $data = htmlspecialchars($data);
    return $data;
}
?>

<!DOCTYPE html>
<html>
<head>
    <title>Homework 6 - Events Search</title>
    <style type="text/css">
        table {
            margin: 0px auto;
            border-collapse: collapse;
            font-family: "Times New Roman";
            font-size: 16px;
        }
        table, td, tr {
            border: 2px solid #dddddd;
        }
        a {
            color: black;
            text-decoration: none;
        }
        a:hover {
            color: gray;
        }
        #detail h3 {
            margin: 10px auto;
        }
        #detail p {
            margin: 0;
            font-size: 14px;
        }
        .mapBound {
            width: 350px;
            height: 250px;
            background-color: cornsilk;
            display: inline-block;
            position: absolute;
        }
        .direction {
            width:80px;
            text-align: center;
            padding: 10px 5px;
            background-color: #f0f0f0;
        }
        .direction:hover {
            background-color: #dcdcdc;
            color: gray;
        }
        .detailDirection {
            width:80px;
            text-align: center;
            background-color: #f0f0f0;
        }
        .click {
            font-size: 12px;
            color: gray;
            margin: 5px;
        }
        .venueTitle {
            width: 200px;
            font-weight: bold;
            text-align: right;
        }
        .venueContent {
            text-align: center;
            width: 600px;
        }
        .noFound {
            font-size: 13px;
            font-weight: bold;
            text-align: center;
            width: 800px;
        }
        .venueButton {
            border-color: white;
            background-color: white;
            outline: none;
        }
    </style>
</head>
<body onload="checkip()">
<script type="text/javascript">
    function location_write() {
        document.getElementById("location_input").value = "textbox";
        document.getElementById("location_here").value = "";
        document.getElementById("location_text").readOnly = "";
        document.getElementById("location_text").style = "color: black";
    }
    function location_read() {
        document.getElementById("location_here").value = getGeo();
        document.getElementById("location_input").value = "";
        document.getElementById("location_text").readOnly = "true";
        document.getElementById("location_text").style = "color: gray";
    }
    function checkip() {
        var x = "http://ip-api.com/json";
        if (loadJSON(x) == null) {
            document.getElementById("search").disabled = true;
        }
        else {
            document.getElementById("search").disabled = false;
            document.getElementById("location_here").value = getGeo();
        }
    }
    function loadJSON(url) {
        var xmlhttp = new XMLHttpRequest();
        try {
            xmlhttp.open("GET", url, false);
            xmlhttp.send();
        }
        catch (error) {
            return null;
        }
        if (xmlhttp.status == 404) {
            return null;
        }
        else {
            var jsonDoc = xmlhttp.responseText;
            return jsonDoc;
        }
    }
    function getGeo() {
        var x = "http://ip-api.com/json";
        var jsonDoc = JSON.parse(loadJSON(x));
        var lat = jsonDoc["lat"];
        var lon = jsonDoc["lon"];
        var Geo = new Array(lat, lon)
        return Geo
    }
    function reset_form() {
        deleteDetail();
        document.getElementsByTagName("input")[0].removeAttribute("value");
        document.getElementsByTagName("input")[1].removeAttribute("value");
        for (var i = 1; i <= 5; i++) {
            if (document.getElementsByTagName("option")[i].hasAttribute("selected")) {
                document.getElementsByTagName("option")[i].removeAttribute("selected");
            }
        }
        document.getElementsByTagName("input")[3].removeAttribute("checked");
        document.getElementsByTagName("input")[4].removeAttribute("value");
        document.getElementById("location_text").readOnly = true;
        document.getElementById("event").reset();
    }
</script>

<div style="font-family: 'Times New Roman'; height: 240px; width: 650px; background-color:rgb(250,250,250); border:#CBCBCB solid medium; margin: 0px auto;">
    <div style="text-align: center; width: 98%; margin: 0px auto; border-bottom: #CBCBCB solid thin;">
        <h1 style="font-style: italic; font-weight:100; margin: 3px 0px 15px 0px;">Events Search</h1>
    </div>

    <form method="post" id="event" name="event" action="<?php echo htmlspecialchars($_SERVER["PHP_SELF"]); ?>"
          style="font-weight: bold; font-size: 18px; margin: 10px 10px; line-height: 30px;">
        <span>Keyword</span> <input type="text" name="keyword" style="width: 150px;" value="<?php echo $keywordOri; ?>"
                                    required> <br/>
        Category
        <select name="category">
            <option value="default">Default</option>
            <option value="music" <?php if ($category == "music") echo "selected" ?>>Music</option>
            <option value="sports" <?php if ($category == "sports") echo "selected" ?>>Sports</option>
            <option value="at" <?php if ($category == "at") echo "selected" ?>>Arts & Theatre</option>
            <option value="film" <?php if ($category == "film") echo "selected" ?>>Film</option>
            <option value="miscellaneous" <?php if ($category == "miscellaneous") echo "selected" ?>>Miscellaneous
            </option>
        </select> <br/>
        Distance (miles) <input type="text" id="distance" name="distance" style="width: 150px;"
                                value="<?php echo $_POST["distance"]; ?>" placeholder="10">
        from <input type="radio" name="location" id="location_here" checked=true onclick="location_read()"> <span
            style="font-weight: normal">Here</span> <br/>
        <div style="margin-left: 330px;">
            <input type="radio" name="location" id="location_input" onclick="location_write()">
            <input type="text" name="location_text" id="location_text" placeholder="location" readonly="true"
                   value="<?php echo $_POST["location_text"]; ?>" required><span style="display: none;" id="textLat"><?php echo $location[0]?></span><span style="display: none;" id="textLng"><?php echo $location[1]?></span>
        </div>
        <div style="margin-left: 120px;">
            <input type="submit" id="search" value="Search" style="font-size: 20px; border-radius: 0px">
            <input type="button" <?php $keyword = $category = $distance = $location = ""; ?> onclick="reset_form()"
                   value="clear" style="font-size: 20px; border-radius: 0px"/>
        </div>
    </form>
</div>

<div id="content" style="margin: 20px;">
    <script type="text/javascript">
        if ("<?php echo $_POST["location"];?>" == "textbox"){
            document.getElementById("location_here").checked = false;
            document.getElementById("location_input").checked = true;
            document.getElementById("location_input").value = "textbox";
            document.getElementById("location_text").readOnly = "";
        }
        var jsonTicket = "";
        var flag = 0;
        jsonTicket = <?php echo $json_file; ?>;
        try {
            if (jsonTicket["page"]["totalElements"] != 0) {
                var num = jsonTicket["page"]["totalElements"];
                flag = 1;
            }
            else {
                flag = 0;
                document.write("<div id=\"showEvents\" style=\"text-align: center; margin: 0px auto; width: 800px; border:#c8c8c8 solid medium; background-color: #f0f0f0;\"><p style=\"margin:0px;\">No records have been found</p></div>");
            }
        }
        catch (error) {
            if (jsonTicket != " "){
                document.write("<div id=\"showEvents\" style=\"text-align: center; margin: 0px auto; width: 800px; border:#c8c8c8 solid medium; background-color: #f0f0f0;\"><p style=\"margin:0px;\">No records have been found</p></div>");
            }
        }
        // --------------
        // show List Page
        // --------------
        if (flag == 1) {
            var eventTable = "<div id=\"showEvents\"><table ><tr style=\"font-weight: bold; text-align: center; \"><td style='min-width: 80px;'>Date</td><td>Icon</td><td style='max-width: 550px;'>Event</td><td>Genre</td><td>Venue</td>";
            num = jsonTicket["_embedded"]["events"].length;
            for (var i = 0; i < num; i++) {
                var event = jsonTicket["_embedded"]["events"][i];
                try {
                    var localDate = event["dates"]["start"]["localDate"];
                }
                catch (e) {
                    var localDate = "N / A"
                }
                try {
                    var localTime = event["dates"]["start"]["localTime"];
                    if (localTime == undefined) {
                        localTime = "";
                    }
                }
                catch (e) {
                    var localTime = "";
                }
                try {
                    var iconUrl = event["images"][0]["url"];
                }
                catch (e) {
                    var iconUrl = "";
                }
                try {
                    var eventName = event["name"];
                }
                catch (e) {
                    var eventName = "N/A";
                }
                try {
                    var eventId = event["id"];
                }
                catch (e) {
                    var eventId = "N/A";
                }
                try {
                    var genre = event["classifications"][0]["segment"]["name"];
                }
                catch (error) {
                    var genre = "N/A";
                }
                try {
                    var venue = event["_embedded"]["venues"][0]["name"];
                    var listLat = event["_embedded"]["venues"][0]["location"]["latitude"];
                    var listLng = event["_embedded"]["venues"][0]["location"]["longitude"];
                    console.log(listLat, listLng);
                }
                catch (e) {
                    var venue = "N/A";
                    var listLat = "";
                    var listLng = "";
                }
                var newEvent = "<tr> <td style=\"text-align: center;\">" +
                    localDate + "<br/>" + localTime + "</td><td style=\"text-align: center;\"><img src=\'" +
                    iconUrl + "\' style=\"height: 40px; width: auto;\"/></td><td style=\"padding: 10px; min-width: 450px; max-width: 550px;\"><a href=\"javascript:void(0);\" onclick=\"javascript:showDetail(" + i + ");\">" +
                    eventName + "</a></td><td style=\"padding: 5px; min-width: 50px;\">" +
                    genre + "</td><td style=\"padding: 10px; min-width: 300px;\"><a href=\"javascript:void(0);\" onclick=\"javascript:showMap(" + i + ");\">"+
                    venue + "</a><span id='listLat' style='display: none;'>"+listLat+"</span><span id='listLng' style='display: none;'>"+listLng+"</span><br/><span class='mapBound' id=\"mapBound"+i+"\" style='visibility: hidden;'>"+
                    "<div style='position:absolute; z-index: 5;'><div class='direction'><a href='javascript:void(0)' onclick='walkThere(\"map"+i+"\")'>Walk there</a></div><div class='direction'><a href='javascript:void(0)' onclick='bikeThere(\"map"+i+"\")'>Bike there</a></div><div class='direction'><a href='javascript:void(0)' onclick='driveThere(\"map"+i+"\")'>Drive there</a></div></div>"+
                    "<div id='map"+i+"' style='width: 350px; height: 250px;'></div></span></td></tr>";
                eventTable += newEvent;
            }
            eventTable += "</td></table></div>";
            document.getElementById("content").innerHTML = eventTable;
        }
        function deleteDetail() {
            var parent = document.getElementById("content");
            var child = document.getElementById("showEvents");
            parent.removeChild(child);
        }

        function showDetail(index) {
            deleteDetail();
            <?php echo $location;?>
            var eventId = jsonTicket["_embedded"]["events"][index]["id"];
            var formData = new FormData();
            formData.append('eventId', eventId);
            var xmlhttp = new XMLHttpRequest();
            xmlhttp.open("POST", document.URL);
            xmlhttp.send(formData);
            // ------------------------------
            //Load event detail
            // ------------------------------
            xmlhttp.onload = function () {
                if (xmlhttp.readyState == xmlhttp.DONE) {
                    if (xmlhttp.status == 200) {
                        var jsonDoc = xmlhttp.responseText;
                        eventDetail = JSON.parse(jsonDoc);
                        var eventName = eventDetail["name"];
                        var eventTable = "<div id=\"showEvents\" ><h3 style='text-align: center;'>" + eventName + "</h3><div id='detail' style=\"margin: 10px auto 30px auto; display: table;\"><div id='info' style='float:left ;font-family: \"Times New Roman\";'>";
                        // get event Date
                        try {
                            var eventDate = eventDetail["dates"]["start"]["localDate"];
                            var eventTime = eventDetail["dates"]["start"]["localTime"];
                            eventTable += "<h3>Date</h3><p>" + eventDate + " " + eventTime + "</p>";
                        }
                        catch (e) {
                            console.log("no date");
                        }
                        // get event Artist / Team
                        try {
                            var num = eventDetail["_embedded"]["attractions"].length;
                            eventTable += "<h3>Artist / Team</h3><p>";
                            for (var i = 0; i < num; i++) {
                                var attra = eventDetail["_embedded"]["attractions"][i]["name"];
                                var attraUrl = eventDetail["_embedded"]["attractions"][i]["url"];
                                eventTable += "<a target='blank' href=\"" + attraUrl + "\">" + attra + "</a>" + " | ";
                            }
                            eventTable = eventTable.slice(0, -3);
                            eventTable += "</p>";
                        }
                        catch (e) {
                            console.log("no at");
                        }
                        // get event venue
                        try {
                            var eventVenue = "";
                            eventVenue = eventDetail["_embedded"]["venues"][0]["name"];
                            eventTable += "<h3>Venues</h3><p>" + eventVenue + "</p>";
                        }
                        catch (e) {
                            console.log("no venue");
                            var eventVenue = "";
                        }
                        if (eventVenue.length != 0) {
                            var eventVenueSend = eventVenue.replace(/ /g, "%20");
                        } else {
                            var eventVenueSend = "";
                        }
                        // get event genre
                        try {
                            var eventGenre = eventDetail["classifications"][0];
                            var subGenre = eventGenre["subGenre"]["name"];
                            var genre = " | " + eventGenre["genre"]["name"];
                            var segment = " | " + eventGenre["segment"]["name"];
                            if (eventGenre["subType"]["name"] != "Undefined") {
                                var subType = " | " + eventGenre["subType"]["name"];
                            }
                            else {
                                var subType = "";
                            }
                            if (eventGenre["type"]["name"] != "Undefined") {
                                var type = " | " + eventGenre["type"]["name"];
                            }
                            else {
                                var type = "";
                            }
                            eventTable += "<h3>Genre</h3><p>" + subGenre + genre + segment + subType + type + "</p>";
                        }
                        catch (e) {
                            console.log("no genre");
                        }
                        // get price range
                        try {
                            var priceRange = eventDetail["priceRanges"][0];
                            var min = priceRange["min"];
                            if (min == undefined) {
                                min = "";
                            }
                            var max = priceRange["max"];
                            if (max == undefined) {
                                max = "";
                            }
                            var currency = priceRange["currency"];
                            eventTable += "<h3>Price Ranges</h3><p>" + min + " - " + max + " " + currency + "</p>";
                        }
                        catch (e) {
                            console.log("no range");
                        }
                        // get ticket status
                        try {
                            var status = eventDetail["dates"]["status"]["code"];
                            eventTable += "<h3>Ticket Status</h3><p>" + status + "</p>";
                        }
                        catch (e) {
                            console.log("no status");
                        }
                        // get buy ticket at
                        try {
                            var ticketUrl = eventDetail["url"];
                            eventTable += "<h3>Buy Ticket At:</h3><p><a href=\"" + ticketUrl + "\" target=\"_blank\">Ticketmaster</a></p>";
                        }
                        catch (e) {
                            console.log("no url");
                        }
                        eventTable += "</div>";
                        // get seat map
                        try {
                            var seatUrl = eventDetail["seatmap"]["staticUrl"];
                            eventTable += "<img style='float:left;' src=\"" + seatUrl + "\">";
                        }
                        catch (e) {
                            console.log("no seat map");
                        }
                        eventTable += "</div><div id='venues' style='margin: 0px auto; display: table;'></div></div>";
                        // ------------------------------
                        //Show venue detail
                        // ------------------------------
                        if (eventVenueSend == "") {
                            var doc = "<div id='venueInfo' style='text-align: center; margin-bottom: 20px;'><button onclick='showVenueInfo()' class='venueButton'><p class='click' id='infoP'>click to show venue info</p><img id='infoImg' src='http://csci571.com/hw/hw6/images/arrow_down.png' style='height: 20px; width:auto;margin-bottom: 10px;'/></button><br /><div id='venueInfoContent' style='display: none;'><table id='venueInfoTable'><tr><td class='noFound'>No Venue Info Found</td></tr>";
                            doc += "</table></div></div><div id='venuePhoto' style='text-align: center;'><button onclick='showVenuePhotos()' class='venueButton'><p class='click' id='photoP'>click to show venue photos</p><img id='photoImg' src='http://csci571.com/hw/hw6/images/arrow_down.png' style='height: 20px; width:auto;margin-bottom: 10px;'/></button><br/><div id='venuePhotoContent' style='display: none;'><table><tr><td class='noFound'>No Venue Photos Found</td></tr></table><div></div>"
                            document.getElementById("venues").innerHTML = doc;
                        }
                        else {
                            var formData = new FormData();
                            formData.append('venueName', eventVenueSend);
                            var xmlhttpr = new XMLHttpRequest();
                            xmlhttpr.open("POST", document.URL);
                            xmlhttpr.send(formData);
                            // ------------------------------
                            //Show valid venue detail
                            // ------------------------------
                            xmlhttpr.onload = function () {
                                if (xmlhttpr.readyState == xmlhttp.DONE) {
                                    if (xmlhttpr.status == 200) {
                                        var jsonDoc = xmlhttpr.responseText;
                                        var venueDoc = JSON.parse(jsonDoc);
                                        if (venueDoc["page"]["totalElements"] == 0) {
                                            var doc = "<div id='venueInfo' style='text-align: center; margin-bottom: 20px;'><button onclick='showVenueInfo()' class='venueButton'><p class='click' id='infoP'>click to show venue info</p><img id='infoImg' src='http://csci571.com/hw/hw6/images/arrow_down.png' style='height: 20px; width:auto;margin-bottom: 10px;'/></button><br /><div id='venueInfoContent' style='display: none;'><table id='venueInfoTable'><tr><td class='noFound'>No Venue Info Found</td></tr>";
                                            doc += "</table></div></div><div id='venuePhoto' style='text-align: center;'><button onclick='showVenuePhotos()' class='venueButton'><p class='click' id='photoP'>click to show venue photos</p><img id='photoImg' src='http://csci571.com/hw/hw6/images/arrow_down.png' style='height: 20px; width:auto;margin-bottom: 10px;'/></button><br/><div id='venuePhotoContent' style='display: none;'><table><tr><td class='noFound'>No Venue Photos Found</td></tr></table><div></div>"
                                            document.getElementById("venues").innerHTML = doc;
                                        }
                                        else {
                                            var venueDetail = venueDoc["_embedded"]["venues"][0];
                                            var doc = "<div id='venueInfo' style='text-align: center; margin-bottom: 20px;'><button onclick='showVenueInfo()' class='venueButton'><p class='click' id='infoP'>click to show venue info</p><img id='infoImg' src='http://csci571.com/hw/hw6/images/arrow_down.png' style='height: 20px; width:auto;margin-bottom: 10px;'/></button><br /><div id='venueInfoContent' style='display: none;'><table id='venueInfoTable'>";
                                            var venueName = venueDetail["name"];
                                            doc += "<tr><td class=\"venueTitle\">Name</td><td class=\"venueContent\">" + venueName + "</td></tr>";
                                            //get venue map
                                            try {
                                                var venueLat = venueDetail["location"]["latitude"];
                                                var venueLon = venueDetail["location"]["longitude"];
                                                doc += "<tr><td class=\"venueTitle\">Map<p style='display: none;' id='venueLat' >"+venueLat+"</p><p style='display: none' id='venueLon' >"+venueLon+"</p></td><td><div class=\"venueContent\">"+
                                                    "<div style='position:absolute; padding: 40px 15px;'><div class='detailDirection'><a href='javascript:void(0)' onclick='walkThere(\"venueMap\")'>Walk there</a></div><div class='detailDirection'><a href='javascript:void(0)' onclick='bikeThere(\"venueMap\")'>Bike there</a></div><div class='detailDirection'><a href='javascript:void(0)' onclick='driveThere(\"venueMap\")'>Drive there</a></div></div>"+
                                                    "<div id=\"venueMap\" style='width: 380px; height: 300px; margin: 0px auto;'></div></div></td></tr>";
                                            }
                                            catch (e) {
                                                doc += "<tr><td class=\"venueTitle\">Map</td><td class=\"venueContent\">N/A</td></tr>";
                                                console.log("no location");
                                            }
                                            try {
                                                var venueAdd = venueDetail["address"]["line1"];
                                                doc += "<tr><td class=\"venueTitle\">Address</td><td class=\"venueContent\">" + venueAdd + "</td></tr>";
                                            }
                                            catch (e) {
                                                doc += "<tr><td class=\"venueTitle\">Address</td><td class=\"venueContent\">N/A</td></tr>";
                                                console.log("no address");
                                            }
                                            try {
                                                var venueCity = venueDetail["city"]["name"];
                                                doc += "<tr><td class=\"venueTitle\">city</td><td class=\"venueContent\">" + venueCity + "</td></tr>";
                                            }
                                            catch (e) {
                                                doc += "<tr><td class=\"venueTitle\">city</td><td class=\"venueContent\">N/A</td></tr>";
                                                console.log("no city");
                                            }
                                            try {
                                                var venuePost = venueDetail["postalCode"];
                                                doc += "<tr><td class=\"venueTitle\">Postal Code</td><td class=\"venueContent\">" + venuePost + "</td></tr>";
                                            }
                                            catch (e) {
                                                doc += "<tr><td class=\"venueTitle\">Postal Code</td><td class=\"venueContent\">N/A</td></tr>";
                                                console.log("no postal code");
                                            }
                                            try {
                                                var venueUrl = venueDetail["url"];
                                                doc += "<tr><td class=\"venueTitle\">Upcoming Event</td><td class=\"venueContent\"><a href='" + venueUrl + "' target=\"_blank\">" + venueName + " Tickets</a></td></tr>";
                                            }
                                            catch (e) {
                                                doc += "<tr><td class=\"venueTitle\">Upcoming Event</td><td class=\"venueContent\">N/A</td></tr>";
                                                console.log("no city");
                                            }
                                            doc += "</table></div></div><div id='venuePhoto' style='text-align: center;'><button onclick='showVenuePhotos()' class='venueButton'><p class='click' id='photoP'>click to show venue photos</p><img id='photoImg' src='http://csci571.com/hw/hw6/images/arrow_down.png' style='height: 20px; width:auto;margin-bottom: 10px;'/></button><br/><div id='venuePhotoContent' style='display: none;'><table>"
                                            try {
                                                var imgNum = venueDetail["images"].length;
                                                if (imgNum == 0) {
                                                    doc += "<tr><td class='noFound'>No Venue Photos Found</td></tr>";
                                                } else {
                                                    for (var j = 0; j < imgNum; j++) {
                                                        var venueImgUrl = venueDetail["images"][j]["url"];
                                                        doc += "<tr><td style='width: 1000px'><img src='" + venueImgUrl + "' style='max-width: 850px'></td></tr>";
                                                    }
                                                }
                                            }
                                            catch (e) {
                                                doc += "<tr><td class='noFound'>No Venue Photos Found</td></tr>";
                                            }
                                            doc += "</table><div></div>";
                                            document.getElementById("venues").innerHTML = doc;
                                        }
                                    }
                                }
                            }
                        }
                        document.getElementById("content").innerHTML = eventTable;
                    }
                }
            }
        }

        function walkThere(id) {
            if (document.getElementById("location_input").checked == true){
                var oriLat = parseFloat(document.getElementById("textLat").innerText);
                var oriLng = parseFloat(document.getElementById("textLng").innerText);
                var origin = {lat: oriLat, lng: oriLng};
            }
            else{
                var location = document.getElementById("location_here").value.split(",");
                var origin = {lat: parseFloat(location[0]) , lng: parseFloat(location[1])};
            }
            if (id == "venueMap"){
                var lat = parseFloat(document.getElementById("venueLat").innerText);
                var lng = parseFloat(document.getElementById("venueLon").innerText);
                var destination = {lat: lat , lng: lng};
            }
            else{
                var lat = parseFloat(document.getElementById("listLat").innerText);
                var lng = parseFloat(document.getElementById("listLng").innerText);
                var destination = {lat: lat , lng: lng};
            }
            var map = new google.maps.Map(document.getElementById(id));
            var directionsDisplay = new google.maps.DirectionsRenderer;
            directionsDisplay.setMap(map);
            var directionsService = new google.maps.DirectionsService;
            directionsService.route({
                origin: origin,
                destination: destination,
                travelMode: 'WALKING'
            },function(response, status) {
                if (status === 'OK') {
                    directionsDisplay.setDirections(response);
                }
                else{
                    window.alert('Directions request failed due to ' + status);
                }
            });
        }
        function bikeThere(id) {
            if (document.getElementById("location_input").checked == true){
                var oriLat = parseFloat(document.getElementById("textLat").innerText);
                var oriLng = parseFloat(document.getElementById("textLng").innerText);
                var origin = {lat: oriLat, lng: oriLng};
            }
            else{
                var location = document.getElementById("location_here").value.split(",");
                var origin = {lat: parseFloat(location[0]) , lng: parseFloat(location[1])};
            }
            if (id == "venueMap"){
                var lat = parseFloat(document.getElementById("venueLat").innerText);
                var lng = parseFloat(document.getElementById("venueLon").innerText);
                var destination = {lat: lat , lng: lng};
            }
            else{
                var lat = parseFloat(document.getElementById("listLat").innerText);
                var lng = parseFloat(document.getElementById("listLng").innerText);
                var destination = {lat: lat , lng: lng};
            }
            var map = new google.maps.Map(document.getElementById(id));
            var directionsDisplay = new google.maps.DirectionsRenderer;
            directionsDisplay.setMap(map);
            var directionsService = new google.maps.DirectionsService;
            directionsService.route({
                origin: origin,
                destination: destination,
                travelMode: 'BICYCLING'
            },function(response, status) {
                if (status === 'OK') {
                    directionsDisplay.setDirections(response);
                }
                else{
                    window.alert('Directions request failed due to ' + status);
                }
            });
        }
        function driveThere(id) {
            if (document.getElementById("location_input").checked == true){
                var oriLat = parseFloat(document.getElementById("textLat").innerText);
                var oriLng = parseFloat(document.getElementById("textLng").innerText);
                var origin = {lat: oriLat, lng: oriLng};
            }
            else{
                var location = document.getElementById("location_here").value.split(",");
                var origin = {lat: parseFloat(location[0]) , lng: parseFloat(location[1])};
            }
            if (id == "venueMap"){
                var lat = parseFloat(document.getElementById("venueLat").innerText);
                var lng = parseFloat(document.getElementById("venueLon").innerText);
                var destination = {lat: lat , lng: lng};
            }
            else{
                var lat = parseFloat(document.getElementById("listLat").innerText);
                var lng = parseFloat(document.getElementById("listLng").innerText);
                var destination = {lat: lat , lng: lng};
            }
            var map = new google.maps.Map(document.getElementById(id));
            var directionsDisplay = new google.maps.DirectionsRenderer;
            directionsDisplay.setMap(map);
            var directionsService = new google.maps.DirectionsService;
            directionsService.route({
                origin: origin,
                destination: destination,
                travelMode: 'DRIVING'
            },function(response, status) {
                if (status === 'OK') {
                    directionsDisplay.setDirections(response);
                }
                else{
                    window.alert('Directions request failed due to ' + status);
                }
            });
        }
        function showVenueInfo() {
            var info = document.getElementById("venueInfoContent");
            var photo = document.getElementById("venuePhotoContent");
            var infoP = document.getElementById("infoP");
            var infoImg = document.getElementById("infoImg");
            var photoP = document.getElementById("photoP");
            var photoImg = document.getElementById("photoImg");
            var flag = 0;
            if (document.getElementById("venueInfoTable").getElementsByTagName("tr").length > 1){
                flag = 1;
                var lat = parseFloat(document.getElementById("venueLat").innerText);
                var lng = parseFloat(document.getElementById("venueLon").innerText);
                var venueCenter = {lat: lat , lng: lng};
            }
            if (info.style.display === "none" && photo.style.display === "none") {
                info.style.display = "block";
                infoP.innerHTML = "click to hide venue info";
                infoImg.src = "http://csci571.com/hw/hw6/images/arrow_up.png";
                if (flag == 1){
                    var map = new google.maps.Map(
                        document.getElementById("venueMap"), {zoom: 14, center: venueCenter});
                    var marker = new google.maps.Marker({position: venueCenter, map: map});
                }
            } else if (info.style.display === "block" && photo.style.display === "none") {
                info.style.display = "none";
                infoP.innerHTML = "click to show venue info";
                infoImg.src = "http://csci571.com/hw/hw6/images/arrow_down.png";
            } else if (info.style.display === "none" && photo.style.display === "block") {
                photo.style.display = "none";
                photoP.innerHTML = "click to show venue photo";
                photoImg.src = "http://csci571.com/hw/hw6/images/arrow_down.png";
                info.style.display = "block";
                infoP.innerHTML = "click to hide venue info";
                infoImg.src = "http://csci571.com/hw/hw6/images/arrow_up.png";
                if (flag == 1){
                    var map = new google.maps.Map(
                        document.getElementById("venueMap"), {zoom: 14, center: venueCenter});
                    var marker = new google.maps.Marker({position: venueCenter, map: map});
                }
            }
        }
        function showVenuePhotos() {
            var info = document.getElementById("venueInfoContent");
            var photo = document.getElementById("venuePhotoContent");
            var infoP = document.getElementById("infoP");
            var infoImg = document.getElementById("infoImg");
            var photoP = document.getElementById("photoP");
            var photoImg = document.getElementById("photoImg");
            if (info.style.display === "none" && photo.style.display === "none") {
                photo.style.display = "block";
                photoP.innerHTML = "click to hide venue photo";
                photoImg.src = "http://csci571.com/hw/hw6/images/arrow_up.png";
            } else if (info.style.display === "block" && photo.style.display === "none") {
                info.style.display = "none";
                infoP.innerHTML = "click to show venue info";
                infoImg.src = "http://csci571.com/hw/hw6/images/arrow_down.png";
                photo.style.display = "block";
                photoP.innerHTML = "click to hide venue photo";
                photoImg.src = "http://csci571.com/hw/hw6/images/arrow_up.png";
            } else if (info.style.display === "none" && photo.style.display === "block") {
                photo.style.display = "none";
                photoP.innerHTML = "click to show venue photo";
                photoImg.src = "http://csci571.com/hw/hw6/images/arrow_down.png";
            }
        }

        function showMap(index) {
            var bound = "mapBound"+index;
            var x = document.getElementById(bound);
            if (x.style.visibility === "hidden") {
                var num = document.getElementsByClassName("mapBound").length;
                for (var i = 0; i<num; i++) {
                    document.getElementsByClassName("mapBound")[i].style.visibility = "hidden";
                }
                // console.log(num);
                x.style.visibility = "visible";
                var event = jsonTicket["_embedded"]["events"][index];
                try {
                    var vLat = event["_embedded"]["venues"][0]["location"]["latitude"];
                    var vLng = event["_embedded"]["venues"][0]["location"]["longitude"];
                    // initMap(vLat,vLng, index);
                    var venueCenter = {lat: parseFloat(vLat) , lng: parseFloat(vLng)};
                    var id = "map"+index;
                    var map = new google.maps.Map(
                        document.getElementById(id), {zoom: 13, center: venueCenter});
                    var marker = new google.maps.Marker({position: venueCenter, map: map});
                }
                catch (e) {
                    x.style.visibility = "hidden";
                }
            } else {
                x.style.visibility = "hidden";
            }
        }
    </script>

    <script async defer
            src="https://maps.googleapis.com/maps/api/js?key=AIzaSxsrlaLpGzeI">
    </script>
</div>

</body>
</html>