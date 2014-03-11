// Their usage will become more apparent futher along in the tutorial.
require.config({
  paths: {
    // Major libraries
    jquery: 'libs/jquery/jquery-min',
    underscore: 'libs/underscore/underscore-min', // https://github.com/amdjs
    lodash: 'libs/lodash/lodash', // alternative to underscore
    backbone: 'libs/backbone/backbone-min', // https://github.com/amdjs
    sinon: 'libs/sinon/sinon',
    //brick: 'libs/brick/brick-1.0.0rc1',
    xtag: 'libs/brick/x-tag-components',

    // Require.js plugins
    text: 'libs/require/text',

  }

});

// Let's kick off the application

require([
  'xtag',
  'data-structures',
  'list-element',
  'player-element',
  'jquery', 
  'underscore', 
  'backbone',
], function(X, App, ListElemRegistrationProbablyWontAccess, PERPWA, $, _, Backbone){
	var views = new Array();
    function timeStr (tModel) { //TODO: Refactor to app utility
        // Returns a string combining time and active semantics
        switch(tModel.get("state")) {
            case "signup":
                return "Tournament signup now open, starts "+tModel.get("time")+".";
            case "done":
                return "Now finished, started "+tModel.get("time")+".";
            default: //Also case "current"
                return "Tournament currently in progress!"
        }
        return "Oh s***";
    }
    function winnerStr(pIdx, wIdx, tModel) {
        var pstr = playerStr(pIdx, tModel);
        if (wIdx > 0 && pIdx == wIdx)
            return "<em>" + pstr + "</em>";
        return pstr;
    }
    function playerStr(pIdx, tModel) {
        if (pIdx == undefined || pIdx <= 0)
            return "??? (?)";
        var pModel = tModel.get("players").get(pIdx);
        return pModel.get("name") + " (" +  pModel.get("race") + ")";
    }
    function matchZoom (li, tMod, container) {
        var mMod = li.matchVar;
        $("#editorModal", container).remove(); //clean up prev
        $("#loeModal", container).remove(); //clean up prev
        function pinv(key) { return mMod.get(key) <= 0; }
        function val(key) { return mMod.get(key); }
        if (pinv("player1") || pinv("player2") || !pinv("winner")) {
            var dialogTmpl = document.getElementById("lackOfEditorTemplate");
            container.appendChild(dialogTmpl.content.cloneNode(true));
            return;
        }
        var matchTmpl = document.getElementById("editorTemplate");
        matchTmpl.content.querySelector("#p1win").setAttribute("name", tMod.get("players").get(mMod.get("player1")).get("name"));
        matchTmpl.content.querySelector("#p1win").setAttribute("race", tMod.get("players").get(mMod.get("player1")).get("race"));
        matchTmpl.content.querySelector("#p1win").setAttribute("onclick", "winMe('player1')");
        matchTmpl.content.querySelector("#p2win").setAttribute("name", tMod.get("players").get(mMod.get("player2")).get("name"));
        matchTmpl.content.querySelector("#p2win").setAttribute("race", tMod.get("players").get(mMod.get("player2")).get("race"));
        matchTmpl.content.querySelector("#p2win").setAttribute("onclick", "winMe('player2')");
        matchTmpl.content.querySelector("#editTime").setAttribute("value", mMod.get("schedule"));
        //WARNING: Accursed globals!
        winMe = function (who) {
            mMod.set("winner", mMod.get(who));
            mMod.save();
        };
        doTheConf = function (who) {
            var whom = (who == 1 ? "p1approves" : "p2approves");
            var notWhom = (whom != 1 ? "p1approves" : "p2approves");
            mMod.set(whom, true);
            mMod.set("schedule", container.querySelector("#editTime").value);
            if (mMod.get(notWhom))
                mMod.set("confirmed", true);
            mMod.save();
        };
        matchTmpl.content.querySelector("#p1Conf").setAttribute("onclick", "doTheConf(1)");
        matchTmpl.content.querySelector("#p2Conf").setAttribute("onclick", "doTheConf(2)");
        container.appendChild(matchTmpl.content.cloneNode(true));
    }
    var TournamentPageView = Backbone.View.extend({
		initialize: function() {
	        },
		render: function() {
			this.$el.empty();
            var pgTmpl = $("#pageTemplate")[0];
            this.el.appendChild(pgTmpl.content.cloneNode(true));
            $("#pageDesc", this.el).html(this.model.get("desc") + "<br />" +  timeStr(this.model));
            $("#pageImg", this.el)[0].src = "img/sc2.png";//this.model.get("type"));
            var listTarget = $("#pageListPane", this.el);
            if (this.model.get("state") == "signup") {
                listTarget.remove();
                var pg2Tmpl = $("#tournamentSignup")[0];
                var thisModel = this.model;
                //WARNING! Accursed globals!
                signHere = function () {
                    App.addPlayer(
                        thisModel,
                        document.getElementById("signName").value,
                        document.getElementById("signCode").value,
                        document.getElementById("signRace").value
                    );
                    document.getElementById('signer').hide();
                }
                pg2Tmpl.content.querySelector("#confirmSign").setAttribute("onclick", "signHere()");
                this.el.appendChild(pg2Tmpl.content.cloneNode(true));
                return this;
            }

            for (var i = 0; i<this.model.get("matches").length; i++){
                var listItem = document.createElement("a-listitem");
                var match = this.model.get("matches").get(i); // ### get over at?
                listItem.title = match.get("title");
                listItem.subTitle = winnerStr(match.get("player1"), match.get("winner"), this.model) + " vs " + winnerStr(match.get("player2"), match.get("winner"), this.model);
                listItem.time = match.get("schedule") + (match.get("confirmed") == true ? "!" : "?");
                listItem.matchVar = match;
                //listItem.image = "img/sc2.png"; //this.model.get("name"); ???
                if (this.model.state != "done") {
                    var thisModel = this.model;
                    var thisEl = this.el;
                    listItem.onclick = function() { matchZoom(this, thisModel, thisEl); };
                }
                listTarget.append(listItem);
            }
			return this;
		},
    });
    function diveInto( t ) { //launch tournament view, or bring back main view
        if (t === undefined) {
            $("#mainDeck")[0].showCard(0);
            $("#titleBar").text("Tournament List");
            $("#backButton")[0].disabled = true;
        } else {
            var newGuy = new TournamentPageView({model: t});
            newGuy.render(); //generates nodes, might be faster before it's dom inserted?;
            $("#contentPane").empty();
            $("#contentPane").append(newGuy.el);
            $("#backButton")[0].disabled = false;
            $("#mainDeck")[0].showCard(1);
            $("#titleBar").text(t.get("name"));
        }
    }
	var TournamentRowView = Backbone.View.extend({ //TODO: refactor into separate file
		initialize: function() {
	        },
		render: function() {
			this.$el.empty();
            if (this.model.type == "hidden")
                return;
            var listItem = document.createElement("a-listitem");
            listItem.title = this.model.get("name");
            listItem.subTitle = this.model.get("subTitle");
            listItem.time = this.model.get("time");
            listItem.image = "img/sc2.png"; //this.model.get("name");
            var thisModel = this.model;
            listItem.onclick = function() { diveInto(thisModel); }
			this.el.appendChild(listItem);
			return this;
		},
	});
	App.allTournaments.on('add', function(model) {
		var newGuy = new TournamentRowView({model: model});
		$("#initialList").append(newGuy.el);
		views.push(newGuy);//maintain ref
		newGuy.render();
	});
    //Workaround for https://github.com/x-tag/x-tag/issues/71 . Not as fixed as they think? 1/10 page loads, it worked for me
    xtag.innerHTML($("#fullBody")[0], '<x-appbar> <h1 id="titleBar">Tournament List</h1> <button id="backButton" disabled="true"> &lt;</button> </x-appbar> <x-modal id="modal"></x-modal> <x-deck id="mainDeck" transition-type="slide-left"> <x-card id="initialList"> </x-card> <x-card id="contentPane"></x-card> </x-deck>');
    $("#backButton")[0].onclick = function() { diveInto(); }
    $("#mainDeck")[0].showCard(0);
});
