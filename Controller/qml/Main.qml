import QtQuick 2.1

Item{
    id: main
    anchors.fill: parent
    property string versionStr: "2.5"
    Rectangle{
        id: bg
        color: '#444444'
        anchors.fill: parent
    }
    Column{
        id: introArea
        width: parent.width
        spacing: 8
        MyText{ text: "Starcraft Tournament Tracker v"+versionStr }
        Button{ caption: 'Dummy League'; 
            onClicked: {main.state='show'; display.source = 'http://bq-pindar.apac.nokia.com/html/dummyTournament.xml' }
            anchors.horizontalCenter: parent.horizontalCenter;
        }
        MyText{
        id: notes
            textFormat: Text.RichText
	    text: "Starcraft 2 Tournament coming early next year. <br />"
            + '<a href="http://bq-pindar.apac.nokia.com/SC/replays/">Download Replays</a> from prior tournaments. <br />'
            wrapMode: Text.WordWrap
            height: 800
            width: parent.width
            onLinkActivated: Qt.openUrlExternally(link);
        }
    }
    TourneyDisplay{
        id: display
        anchors.fill: parent
        opacity: 0
        onGoBack: main.state = 'intro';
    }
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
