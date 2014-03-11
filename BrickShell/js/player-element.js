define([ 
  'jquery', 
  'underscore', 
  'xtag', 
], function($, _, xTag){ 
    function doMe(ele) {
        //build the element from its custom properties
        ele.innerHTML = "<p>" + ele.name + " (" + ele.race + ")</p>";
    }
    xTag.register('a-player', {
        extends: 'div',
        lifecycle: { 
            created:  function () {
                this.made = false;
            },
            inserted: function () {
                doMe(this);
                this.made = true;
            },

            attributeChanged: function() {
                if (this.made == true)
                    doMe(this);
            },
        },
        accessors: {
            name: {
                attribute : {},
            },
            race: {
                attribute : {},
            },
        }
    });
});
