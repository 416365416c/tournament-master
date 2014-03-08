define([ 
  'jquery', 
  'underscore', 
  'xtag', 
], function($, _, xTag){ 
    function doMe(ele) {
        //build the element from its custom properties
        ele.innerHTML = "<p>" + ele.title + " : " + ele.subTitle + " ; " + ele.time + "</p>"
            + ( ele.image ? "<img src=\"" + ele.image + "\" />" : "");
    }
    xTag.register('a-listItem', {
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
            subTitle: {
                attribute : {},
            },
            title: {
                attribute : {},
            },
            time: {
                attribute : {},
            },
            image: {
                attribute : {},
            },
        }
    });
});
