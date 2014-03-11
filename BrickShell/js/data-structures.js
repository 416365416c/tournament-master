// Abstracts all client-server communications
define([ 
  'jquery', 
  'underscore', 
  'backbone', 
], function($, _, Backbone){ 
    //Server Communication functions
    //TODO: If caching is enabled (by default, obviously) the app will never update because the browser feeds it a cached version. Need to work around this...
    //NOTE: index/pId changed to uniform "id" attr for theorized backbone compatibility
    var baseUrl = "http://ec2-184-169-240-202.us-west-1.compute.amazonaws.com";
    function loadTFromUrl(urlFragment, tournamentCollection) {
        url = baseUrl + urlFragment;
        var postman = new XMLHttpRequest()
        postman.open("GET", url, true);
        postman.onreadystatechange = function() {
            if (postman.readyState == postman.DONE) {
                var xmldata = $.parseXML(postman.responseText);
                $(xmldata).find("Tournament").each(function() {
                    var t = {
                        tournamentFile: urlFragment.split("/").pop().split(".").shift(),
                        name: $(this).attr("name"),
                        desc: $(this).attr("desc"),
                        time: $(this).attr("time"),
                        subTitle: $(this).attr("subT"),
                        type: $(this).attr("type"),
                        state: $(this).attr("stat"),
                        players: new pCollection(),
                        matches: new mCollection()
                    }
                    $(this).find("Player").each(function() {
                        t.players.push( {
                            id: $(this).attr("pId"),
                            name: $(this).attr("name"),
                            email: $(this).attr("email"),
                            race: $(this).attr("race")
                        });
                    });
                    $(this).find("Match").each(function() {
                        t.matches.push({
                            tournamentFile: urlFragment.split("/").pop().split(".").shift(),
                            tLink: t,
                            title: $(this).attr("title"),
                            id: $(this).attr("index"),
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
                    tournamentCollection.add(t);
                });
            }
        }
        postman.send();
    }

    function makeMatchDataStr(match) {
        var postData = "tournamentTitle="+ match.get("tournamentFile") +"&"
            + "matchIndex="+match.get("id")+"&"
            + "title="+match.get("title")+"&"
            + "player1="+match.get("player1")+"&"
            + "player2="+match.get("player2")+"&"
            + "p1approves="+match.get("p1approves")+"&"
            + "p2approves="+match.get("p2approves")+"&"
            + "schedule="+match.get("schedule");
        if (match.get("winner") > -1)
            postData = postData +"&winner="+match.get("tLink")["players"].get(match.get("winner")).get("name");
        return postData;
    }

    function addPlz(t, name, code, race) {
        var url = baseUrl + "/cgi-bin/tournamentCgi";
        var postman = new XMLHttpRequest()
        var postData = "tournamentTitle="+t.get("tournamentFile")+"&playerIndex=-1&name="+name+"&email="+code+"&race="+race
            + "&request=edit&password="+"AdunToridas";//So much for the shared "secret"!
        postman.open("POST", url, true);
        postman.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
        postman.onreadystatechange = function() {
            if (postman.readyState == postman.DONE) {
                console.log("Sent datum. Got this:\n" + postman.responseText);
            }
        }
        postman.send(postData);
    }
    var badDate = new Date(1998, 2, 30, 23, 59); //invalid date, chosen as just before the SC1 release
    var pModel = Backbone.Model.extend({
        defaults: {
            id : -1,
            name : "anonymous",
            email : "noreply@pppdnbrd.nope",
            race : "R" // one of RZTP - not enum as it is also used direct user-visible (well defined domain meanings)
        },
        initialize: function(){
        },
	sync: function() { alert("Player model sync not supported."); },
    });
    var pCollection = Backbone.Collection.extend({
        model: pModel,
        initialize: function(){
        },
	sync: function() { alert("Player collection sync not supported."); },
    });
    var mModel = Backbone.Model.extend({
        defaults: {
            tournamentTitle : "invalidTournament", //Back reference needed to make model separate :( (exists ONLY for BackBone client)
            title : "Just another Match", //the user visible string
            id : -1, //int
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
        sync: function(method, model, options) { 
            options || (options = {})
            if (method != "update") {
                alert("Match list doesn't fully sync. Ever.");
                return;
            }
            var url = baseUrl + "/cgi-bin/tournamentCgi";
            var postman = new XMLHttpRequest()
            var postData = makeMatchDataStr(model)
                + "&request=edit&password="+"AdunToridas";//So much for the shared "secret"!
            postman.open("POST", url, true);
            postman.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
            postman.onreadystatechange = function() {
                if (postman.readyState == postman.DONE) {
                    console.log("Sent data. Got this:\n" + postman.responseText);
                }
            }
            postman.send(postData);
        }
    });
    var mCollection = Backbone.Collection.extend({
        model: mModel,
        initialize: function(){
        },
	sync: function() { alert("Match collection sync not supported."); },
    });
    var tModel = Backbone.Model.extend({
        defaults: {
            name : "Generic Tournament",
            desc : "This one is just for fun",
            time : badDate,
            subTitle : "Really, nothing here but fun", 
            type : "hidden", //ui should currently support values of "hidden", "sc2", "lol", "arcade"
            state : "current", //states are "signup", "current"(default), "done" - should this be enum?
            players : new pCollection,
            matches : new mCollection,
        },
        initialize: function(){
        },
    });
    var tCollection = Backbone.Collection.extend({
        model: tModel,
	sync: function(method, collection, options) {
	    options || (options = {})
	    if (method != "read") {
		    alert("Tournaments list doesn't fully sync. Ever.");
		    return;
	    }
            var url = baseUrl + "/cgi-bin/tournamentCgi";
            var postman = new XMLHttpRequest()
            var postData = "request="+ "list" +"&"
                + "password="+"AdunToridas";//So much for the shared "secret"!
            postman.open("POST", url, true);
            postman.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
            postman.onreadystatechange = function() {
                if (postman.readyState == postman.DONE) {
                    var lines = postman.responseText.split('\n');
                    var ta, tt;
                    for(var l in lines) {
		    	if (lines[l].indexOf('/var/www') == 0) {
			    var urlfragment = lines[l].slice(8);
			    loadTFromUrl(urlfragment, collection);
			}
                    }
                }
            }
            postman.send(postData);
	},
        initialize: function(){
		this.sync("read",this);
        }
    });

    var tcAlpha = new tCollection;
    //tcAlpha.on("add", function() { console.log(JSON.stringify(this.toJSON())); }, tcAlpha);
    return {
        playerModel: pModel,
        playerCollection: pCollection,
        matchModel: mModel,
        matchCollection: mCollection,
        tournamentModel: tModel,
        tournamentCollection: tCollection,
        allTournaments: tcAlpha,
        invalidDate: badDate,
        addPlayer: addPlz
    };
});

