// Their usage will become more apparent futher along in the tutorial.
require.config({
  paths: {
    // Major libraries
    jquery: 'libs/jquery/jquery-min',
    underscore: 'libs/underscore/underscore-min', // https://github.com/amdjs
    lodash: 'libs/lodash/lodash', // alternative to underscore
    backbone: 'libs/backbone/backbone-min', // https://github.com/amdjs
    sinon: 'libs/sinon/sinon.js',
    brick: 'libs/brick/brick-1.0.0rc1.min.js',
    xtag: 'libs/xtag/x-tag-components.js',

    // Require.js plugins
    text: 'libs/require/text',

  }

});

// Let's kick off the application

require([
  'data-structures',
  'jquery', 
  'underscore', 
  'backbone', 
], function(App, $, _, Backbone){
	var views = new Array();
	var TournamentView = Backbone.View.extend({ //TODO: refactor into separate file
		initialize: function() {
	        },
		render: function() {
			console.log("Hola");
			var templ = document.getElementById("tournamentListCardRow");
			templ.innerHTML = this.model.get("name") + "<br/>";
			this.$el.empty();
			this.$el.append(templ.content.cloneNode(true));
			return this;
		},
	});
	App.allTournaments.on('add', function(model) {
		var newGuy = new TournamentView({model: model});
		$("#initialList").append(newGuy.el);
		views.push(newGuy);//maintain ref
		newGuy.render();
		console.log("Oh dear...");
	});
});
