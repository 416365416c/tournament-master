// Use this as a quick template for future modules 
define([ 
  'jquery', 
  'underscore', 
  'backbone', 
], function($, _, Backbone){ 
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
            players : new Players,
            matches : new Matches,
        },
        initialize: function(){
        },
    });
    var tCollection = Backbone.Collection.extend({
        model: tModel,
        initialize: function(){
        }
    });

    var lXML = function (xmldata) {
        var ts = new Array<Tournament>();//TODO: ME!
        $(xmldata).find("Tournament").each(function() {
            var t = new Tournament($(this).attr("name"),
                                     $(this).attr("desc"),
                                     $(this).attr("time"),
                                     $(this).attr("subT"),
                                     $(this).attr("type"),
                                     $(this).attr("stat"));
            $(this).find("Player").each(function() {
                t.players.push(new Player($(this).attr("pId"),
                                          $(this).attr("name"),
                                          $(this).attr("email"),
                                          $(this).attr("race")));

            });
            $(this).find("Match").each(function() {
                t.matches.push(new Match($(this).attr("title"),
                                          $(this).attr("index"),
                                          $(this).attr("player1"),
                                          $(this).attr("player2"),
                                          $(this).attr("winner"),
                                          $(this).attr("winnerTo"),
                                          $(this).attr("loserTo"),
                                          $(this).attr("xPos"),
                                          $(this).attr("yPos"),
                                          $(this).attr("p1approves"),
                                          $(this).attr("p2approves"),
                                          $(this).attr("confirmed"),
                                          $(this).attr("schedule")));

            });
            ts.push(t);
        });
        return new DataState(ts);
    }

    var sXML = function (state) {
        return "unimplemented"; //TODO: Me!
    }

    return dataStuff {
        playerModel: pModel,
        playerCollection: pCollection,
        matchModel: mModel,
        matchCollection: mCollection,
        tournamentModel: tModel,
        tournamentCollection: tCollection,
        loadXML: lXML,
        saveXML: sXML,
        invalidDate: badDate
    };
});

