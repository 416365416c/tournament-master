//Qt.include("jquery-2.1.0.js"); Doesn't work, missing var $ =
function getTournaments()
{
    url = "http://ec2-184-169-240-202.us-west-1.compute.amazonaws.com/cgi-bin/tournamentCgi";
     var postman = new XMLHttpRequest()
     var postData = "request="+ "list" +"&"
            + "password="+"AdunToridas";
    //url = url + postData;
        postman.open("POST", url, true);
        //postman.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
        console.log("Debug: " + postData);
        postman.onreadystatechange = function() {
            console.log("Debug3: foo " + postman.readyState);
            if (postman.readyState == postman.DONE) {
                console.log("Sent data. Got this:\n" + postman.responseText);
            }
        }
        console.log("Debug2: " + url);
        postman.send(postData);
}

function loadFromUrl(url, tournament, tournamentTitle)
{
    var postman = new XMLHttpRequest()
    postman.open("GET", url, true);
    postman.onreadystatechange = function() {
        if (postman.readyState == postman.DONE) {
            console.log("Sent data. Got this:\n" + postman.responseText);
            var xmldata = postman.responseText;
            $(xmldata).find("Tournament").each(function() {
                var t = {
                    name: $(this).attr("name"),
                    desc: $(this).attr("desc"),
                    time: $(this).attr("time"),
                    subT: $(this).attr("subT"),
                    type: $(this).attr("type"),
                    stat: $(this).attr("stat"),
                    players: new Array(),
                    matches: new Array()
                }
                $(this).find("Player").each(function() {
                    t.players.push( {
                        pId: $(this).attr("pId"),
                        name: $(this).attr("name"),
                        email: $(this).attr("email"),
                        race: $(this).attr("race")
                    });
                });
                $(this).find("Match").each(function() {
                    t.matches.push({
                        title: $(this).attr("title"),
                        index: $(this).attr("index"),
                        player1: $(this).attr("player1"),
                        player2: $(this).attr("player2"),
                        winner: $(this).attr("winner"),
                        winnerTo: $(this).attr("winnerTo"),
                        loserTo: $(this).attr("loserTo"),
                        xPos: $(this).attr("xPos"),
                        yPos: $(this).attr("yPos"),
                        p1approves: $(this).attr("p1approves"),
                        p2approves: $(this).attr("p2approves"),
                        confirmed: $(this).attr("confirmed"),
                        schedule: $(this).attr("schedule")
                    });
                });
                tournament = t;
            });
            console.log("I love my t:" + tournament);
        }
    }
    postman.send();
}

function sendChanges(match, tournament)
{
     var postman = new XMLHttpRequest()
        var postData = "tournamentTitle="+ tournament.title +"&"
            + "matchIndex="+match.index+"&"
            + "title="+match.title+"&"
            + "player1="+match.player1+"&"
            + "player2="+match.player2+"&"
            + "p1approves="+match.p1approves+"&"
            + "p2approves="+match.p2approves+"&"
            + "schedule="+match.schedule+"&"
            + "winner="+match.sendWon;
        postman.open("POST", 'http://ec2-184-169-240-202.us-west-1.compute.amazonaws.com/cgi-bin/tournamentCgi', true);
        postman.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
        //console.log("Debug: " + postData);
        postman.onreadystatechange = function() {
            if (postman.readyState == postman.DONE) {
            //console.log("Sent data. Got this:\n" + postman.responseText);
            sentUpdate();
        }
    }
    postman.send(postData);
}
