// Use this as a quick template for future modules 
define([ 
  'jquery', 
  'underscore', 
  'backbone', 
], function($, _, Backbone){ 
    //Server Communication functions
    var baseUrl = "http://ec2-184-169-240-202.us-west-1.compute.amazonaws.com";
    function loadFromUrl(url, tournament, tournamentTitle) {
        var postman = new XMLHttpRequest()
        postman.open("GET", url, true);
        postman.onreadystatechange = function() {
            if (postman.readyState == postman.DONE) {
                console.log("Sent data to $"+url+"$. Got this:\n" + postman.responseText);
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

    function getTournaments() {
         var url = baseUrl + "/cgi-bin/tournamentCgi";
         var postman = new XMLHttpRequest()
         var postData = "request="+ "list" +"&"
                + "password="+"AdunToridas";
        //url = url + postData;
            postman.open("POST", url, true);
            //postman.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
            postman.onreadystatechange = function() {
                if (postman.readyState == postman.DONE) {
                    var lines = postman.responseText.split('\n');
                    var ta, tt;
                    console.log("Sent data. Got this:\n" + postman.responseText);
                    for(var l in lines) {
		    	if (lines[l].indexOf('/var/www') == 0) {
			    var urlfragment = lines[l].slice(8);
			    loadFromUrl(baseUrl + urlfragment, ta, tt);
			}
                    }
                }
            }
            postman.send(postData);
    }

    function sendChanges(match, tournament) {
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

    var badDate = new Date(1998, 2, 30, 23, 59); //invalid date, chosen as just before the SC1 release
    var pModel = Backbone.Model.extend({
        defaults: {
            pId : -1,
            name : "anonymous",
            email : "noreply@pppdnbrd.nope",
            race : "R" // one of RZTP - not enum as it is also used direct user-visible (well defined meanings)
        },
        initialize: function(){
        },
    });
    var pCollection = Backbone.Collection.extend({
        model: pModel,
        initialize: function(){
        }
    });
    var mModel = Backbone.Model.extend({
        defaults: {
            title : "Just another Match", //the user visible string
            index : -1, //int
            player1 : -1, //id, (should be collection idx too)
            player2 : -1,
            winner : -1, //"usually" p1 or p2
            winnerTo : -1, //Again, id
            loserTo : -1,
            xPos : 0, //for the free-form UIs
            yPos : 0, //for the free-form UIs
            p1approves :  false,
            p2approves : false,
            confirmed : false,
            schedule : badDate,
        },
        initialize: function(){
        },
    });
    var mCollection = Backbone.Collection.extend({
        model: mModel,
        initialize: function(){
        }
    });
    var tModel = Backbone.Model.extend({
        defaults: {
            name : "Generic Tournament",
            desc : "This one is just for fun",
            time : badDate,
            subTitle : "Really, nothing here but fun", 
            type : "hidden", //ui should currently support values of "hidden", "sc2", "lol", "arcade"
            state : "current", //states are "signup", "current"(default), "done" - should this be enum?
            players : new Array,
            matches : new Array,
        },
        initialize: function(){
        },
    });
    var tCollection = Backbone.Collection.extend({
        model: tModel,
        initialize: function(){
        }
    });

    getTournaments(); //DEBUG
    return {
        playerModel: pModel,
        playerCollection: pCollection,
        matchModel: mModel,
        matchCollection: mCollection,
        tournamentModel: tModel,
        tournamentCollection: tCollection,
        invalidDate: badDate
    };
});

