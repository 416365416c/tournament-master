import QtQuick 2.1
import QtQuick.Window 2.1
import QtQuick.Controls 1.0
import "xmlload.js" as LoadLogic

Item {
    id: container
    visible: true
    width: 640
    height: 480
    property MatchEditor me: me
    property bool figuredItOut: false//True once one opens. Might be useful to pulse help or something?
    property bool isClient: false //Set true on client shell. Not actually secure
    property url source: ""
    property string tt: tournamentTitle
    onSourceChanged: LoadLogic.loadFromUrl(source, tournament);
    signal goBack
    MatchEditor{id:me; z:100}
    Connections{
        target: me
        onSentUpdate: matches.reload();//Can't remember what this does?
    }
    MyText {
        text: "Hover over a match to see more.<br />Click on a match to edit details or record outcomes."
        anchors.right: container.right
        z: 2
        opacity: helpTimer.timedOut ? 0 : 1
        Behavior on opacity{NumberAnimation{}}
    }
    Timer{//offer help for the first minute
        id: helpTimer
        running: false
        property bool timedOut: false
        interval: 10*1000
        repeat: false
        onTriggered: timedOut = true;
    }
    Rectangle{
          id: bg
          color: '#444444'
          anchors.fill: parent
    }

    Column {
        z:2
        x: 4
        y: 4
        spacing: 8
        Button{
            text: "Reload"
            onClicked: matches.reload();
        }
        Button{
            text: "Back"
            onClicked: container.goBack();
        }
    }

    Column {
        z:2
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: 4
        spacing: 8
        Repeater{
            model: tournament.players
            delegate: Button{
                text: modelData.name
                onClicked: console.log("IMPLEMENT ME")
            }
        }
    }
    Flickable {
        id: flicker
        z:1
        width: parent.width
        height: parent.height
        //contentHeight: 768 //TODO: Actually calculate something
        //contentWidth: 1024
        anchors.centerIn:parent
        property string tournamentTitle: container.tt
        Item {
            id: main
            anchors.horizontalCenter: parent.horizontalCenter;
            Repeater{
                model: tournament.matches
                delegate: DisplayDelegate{}

            }
        }
    }

}
