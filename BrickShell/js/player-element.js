define([ 
  'jquery', 
  'underscore', 
  'xtag', 
], function($, _, xTag){ 
    function getImage(r) {
        switch (r) {
            case 'T': return "img/terran.png";
            case 'Z': return "img/zerg.png";
            case 'P': return "img/toss.png";
            default : return "img/random.png";
        }
    }
    function doMe(ele) {
        //build the element from its custom properties
        ele.innerHTML = "<p>"+ele.name+"</p><img src=\""+getImage(ele.race)+"\" />";
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
