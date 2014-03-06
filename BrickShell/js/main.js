// Their usage will become more apparent futher along in the tutorial.
require.config({
  paths: {
    // Major libraries
    jquery: 'libs/jquery/jquery-min',
    underscore: 'libs/underscore/underscore-min', // https://github.com/amdjs
    lodash: 'libs/lodash/lodash', // alternative to underscore
    backbone: 'libs/backbone/backbone-min', // https://github.com/amdjs
    sinon: 'libs/sinon/sinon',
    brick: 'libs/brick/brick-1.0.0rc1.min',
    xtag: 'libs/brick/x-tag-components',

    // Require.js plugins
    text: 'libs/require/text',

  }

});

// Let's kick off the application

require([
  'data-structures',
  'list-element',
  'jquery', 
  'underscore', 
  'backbone',
  'xtag',
], function(App, ListElemRegistrationProbablyWontAccess, $, _, Backbone,X){
	var views = new Array();
	var TournamentView = Backbone.View.extend({ //TODO: refactor into separate file
		initialize: function() {
	        },
		render: function() {
		//	var templ = document.getElementById("tournamentListCardRow");
		//	templ.innerHTML = "<a-listitem title=\""this.model.get("name")"\" + "<br/>";
			this.$el.empty();
            var listItem = document.createElement("a-listitem");
            listItem.title = this.model.get("name");
            listItem.subTitle = this.model.get("subT");
            listItem.time = this.model.get("time");
            listItem.image = "img/sc2.png"; //this.model.get("name");
            listItem.onclick = function() { alert("It's ALLIIIIIIIIIIIVE! Starting from " + listItem.time); }
			this.el.appendChild(listItem);
			return this;
		},
	});
	App.allTournaments.on('add', function(model) {
		var newGuy = new TournamentView({model: model});
		$("#initialList").append(newGuy.el);
		views.push(newGuy);//maintain ref
		newGuy.render();
	});
});
