import QtQuick 2.1
import QtQuick.Controls 1.0
import "xmlload.js" as LoadLogic

Rectangle {
    id: container
    signal sentUpdate
    width: parent.width
    height: parent.height
    color: "#33000000"//gray out all behind
    opacity: 0
    visible: opacity > 0
    property QtObject match
    property string sendWon: ''
    function getName(m,p) {
        if (m == null)
            return ""
        if (m[p] == undefined || m[p] == null)
            return ""
        return m[p].name
    }

    function load(m){
        titleTxt.text = m.title;
        p1Txt.text = getName(m,"player1");
        p2Txt.text = getName(m,"player2");
        schedTxt.text = m.schedule;
        wTxt.text = m.winnerTo;
        lTxt.text = m.loserTo;
        match = m;
    }
    
    function saveChanges(){//Save data
        match.title = titleTxt.text;
        match.player1.name = p1Txt.text;
        match.player2.name = p2Txt.text;
        match.schedule = schedTxt.text;
        match.winnerTo = Number(wTxt.text);
        match.loserTo = Number(lTxt.text);
        if(sendWon == match.player1.name)
            tournament.matchFinished(match, true);
        else if(sendWon == match.player2.name)
            tournament.matchFinished(match, false);
        if(parent.isClient)//directly under TourneyDisplay
            LoadLogic.sendChanges(match, parent.source);
    }

    NumberAnimation { id: fadeIn; target: container; property: "opacity"; to: 1; duration: 400 }
    NumberAnimation { id: fadeOut; target: container; property: "opacity"; to: 0; duration: 400 }

    function show(m){ load(m); fadeIn.start(); }
    MouseArea{ anchors.fill: parent; onClicked: fadeOut.start(); enabled: parent.visible}
    Rectangle{id: bg
        anchors.fill: col
        anchors.margins: -8
        color: '#555555'
        border.color: '#222222'
        radius: 8
    }
    Column{
        id: col
        width: container.width/2
        anchors.centerIn: parent
        Row{ height: titleTxt.height
            MyText{
                width: container.width/4;
                text: 'Title:'
            }
            TextField { id: titleTxt
                width: container.width/4;
              //  text: title
            }
        }
        Row{ height: p1Txt.height
            MyText{
                width: container.width/4;
                text: 'Player 1:'
            }
            TextField { id: p1Txt
                width: container.width/4;
               // text: player1
            }
        }
        MyText { id: vs
            width: container.width;
            text: ' vs '
        }
        Row{ height: p2Txt.height
            MyText{
                width: container.width/4;
                text: 'Player 2:'
            }
            TextField { id: p2Txt
                width: container.width/4;
             //   text: player2
            }
        }

        Row{ height: schedTxt.height
            MyText{
                width: container.width/4;
                text: 'Scheduled Time:'
            }
                width: container.width/4;
                TextField { id: schedTxt
             //   text: schedule
                onTextChanged: {p1Y.ticked = false; p2Y.ticked = false;}
            }
        }
        Row{ height: wTxt.height
            MyText{
                width: container.width/4;
                text: 'Winner To:'
            }
            TextField { id: wTxt
                width: container.width/4;
             //   text: winnerTo
            }
        }

        Row{ height: lTxt.height
            MyText{
                width: container.width/4;
                text: 'Loser To:'
            }
            TextField { id: lTxt
                width: container.width/4;
              //  text: loserTo
            }
        }
        Button{
            id: syncButton
            width: parent.width
            text: "Save Changes"
            onClicked: saveChanges();
        }
        Item{id:spacer
            height: 40
        }
        Row{
            Button{
                text: getName(match, "player1") + ' won!';
                width: container.width/4
                onClicked: {sendWon = match.player1.name; saveChanges();}
            }
            Button{
                text: getName(match, "player2") + ' won!';
                width: container.width/4
                onClicked: {sendWon = match.player2.name; saveChanges();}
            }
        }
    }
}
