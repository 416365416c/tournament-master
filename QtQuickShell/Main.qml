import QtQuick 2.1
import QtQuick.Window 2.1
import QtQuick.Controls 1.0 as Controls
import "../Common/qml" //Most logic is here, shared with Controller

Window{
    id: main
    width: 640
    height: 480
    visible: true
    property string versionStr: "3.1"
    property var tournament: new Object
    Item {
        id: stateGuy
        states:[State{ name: 'intro';
                PropertyChanges{target: introArea; opacity: 1}
                PropertyChanges{target: display; opacity: 0}
            },State{ name: 'show';
                PropertyChanges{target: introArea; opacity: 0}
                PropertyChanges{target: display; opacity: 1}
            }]
        transitions:Transition{
            PropertyAnimation{property:"opacity"; duration:200}
        }
    }
    Rectangle{
        id: bg
        color: '#444444'
        anchors.fill: parent
    }
    TourneyDisplay{
        id: display
        anchors.fill: parent
        visible: opacity == 0
        opacity: 0
        onGoBack: stateGuy.state = 'intro';
        isClient: true
    }
    Column{
        id: introArea
        width: parent.width
        spacing: 8
        MyText{ text: "Starcraft Tournament Tracker v"+versionStr }
        Controls.Button{
            text: 'Dummy League'; 
            onClicked: {
                stateGuy.state='show';
                display.source = 'http://ec2-184-169-240-202.us-west-1.compute.amazonaws.com/xml/testTournament.xml';
                display.tt="dummy";
            }
            anchors.horizontalCenter: parent.horizontalCenter;
        }
        MyText{
        id: notes
            textFormat: Text.RichText
	    text: "Starcraft 2 Tournament!<br />"
            + 'This is just a test shell for now. Other shells use the NEW! functionality<br />'
            wrapMode: Text.WordWrap
            height: 800
            width: parent.width
           // onLinkActivated: Qt.openUrlExternally(link);
        }
    }
}
