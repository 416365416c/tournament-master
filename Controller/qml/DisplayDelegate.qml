import QtQuick 2.1

Rectangle{
    id: rect
    property int ySpacing: 4 //constant

    //NOTE: It sort of happens that match.index is ignored - so the list MUST be in sorted order
    //NOTE: It sort of happens also that index of -1 leads to crashes in declarative!
    property bool done: winner != null
    property bool confirmed: (p1approves=="true" && p2approves=="true") || schConf == "true"
    property bool ready: tournament != undefined && tournament.matches[index].player1 != null && tournament.matches[index].player2 != null
    property QtObject wTo: winnerTo >= 0 ? tournament.matches[winnerTo] : fakeTo
    property QtObject lTo: loserTo >= 0 ? tournament.matches[loserTo] : fakeTo
    property string scheduleStr : schedule == "" ? "unscheduled"
                : (schedule + (confirmed ? '' : '?'))

    QtObject { id: fakeTo
        property real xPos: -1
        property real yPos: -1
    }

    Behavior on x{NumberAnimation{}}
    Behavior on y{NumberAnimation{}}
    x: xPos - 47
    y: yPos + 40//(90 + ySpacing) * row  - 30//reevaluated with setPos called, note that 90 is initial height
    Binding{
        property: "xPos"
        target: tournament.matches[index]
        when: dragger.drag.active;
        value: rect.x
    }
    Binding{
        property: "yPos"
        target: tournament.matches[index]
        when: dragger.drag.active;
        value: rect.y - 80
    }

    z: index
    width: 220
    height: 90
    border.color: 'black'
    border.width: 1
    radius: 2
    smooth:true
    color: if(done){'#6699cc';}else if(confirmed){'#cc9911';}else if(ready){'#66cc11';}else{'#669911';}
    Rectangle {//Shadow
        x:4
        y:4
        width: parent.width
        height: parent.height
        color: '#333333'
        radius: 2
        z:-1
    }

    MyText {
        id: name
        anchors.fill:parent
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        text: title + "("+index+")"+ '<br />' + (player1 == null ? "???" : player1.name) + '<br />vs<br />' + (player2 == null ? "???" : player2.name) + '<br />'
              + ( winner != null ? "Winner: " + winner.name : scheduleStr )
        z: 1
    }

    Line{//winner
        id: line
        x1: rect.width/2 + 2
        y1: 0
        //x2: (main.width * (Math.floor(col/2)+1) / ((myRowSize/2) + 1))-rect.x//it's relative
        x2: wTo.xPos - xPos + rect.width/2;
        y2: wTo.yPos - yPos + 70;
        z: -1
        visible: wTo != fakeTo
        color: "green"
    }
    Line{//loser
        id: line2
        x1: rect.width/2 - 2
        y1: 0
        //x2: (main.width * (Math.floor(col/2)+1) / ((myRowSize/2) + 1))-rect.x//it's relative
        x2: lTo.xPos - xPos + rect.width/2;
        y2: lTo.yPos - yPos + 70;
        z: -1
        visible: lTo != fakeTo
        color: "red"
    }


    MouseArea{
        id: ma
        anchors.fill: parent
        hoverEnabled: true
        onClicked: container.me.show(tournament.matches[index]);
    }

    MouseArea{
        id: dragger
        hoverEnabled: true
        anchors.left: parent.left
        anchors.top:  parent.top
        anchors.margins: -4
        width: 20
        height: 20
        Rectangle{
            anchors.fill: parent
            color: "lightsteelblue"
            visible: parent.enabled
        }
        enabled: false
        drag.target: parent
        drag.axis: Drag.XandYAxis
        z: 1
    }


    onStateChanged: if(state == '') container.figuredItOut = true;
    states: State{name: "shrunk"; when: !ma.containsMouse
        PropertyChanges{
            target: name
            text: title
        }
        PropertyChanges{
            target: line
            y2: wTo.yPos - yPos + 30;
        }PropertyChanges{
            target: line2
            y2: lTo.yPos - yPos + 30;
        }
        PropertyChanges{
            target: rect
            width: 128
            height: 30
            z: 0 - index
            x: xPos
            y: yPos + 80
        }
        PropertyChanges{
            target: dragger
            enabled: true
        }
    }
    transitions: Transition{ to: "shrunk"; from: ""; reversible: true
        SequentialAnimation{
            PropertyAction{ property: "text" }
            NumberAnimation{ properties: "x,y,width,height,y2" }
            PropertyAction{ property: "z" }

        }
    }

}
