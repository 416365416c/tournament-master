import bb.cascades 1.2

ImageView {
    property string race: "T"
    imageSource: {
        switch(race) {
            case "T": return "asset:///images/terran.png";
            case "Z": return "asset:///images/zerg.png";
            case "P": return "asset:///images/toss.png";
            default: return "asset:///images/random.png";
        } 
    }
}
