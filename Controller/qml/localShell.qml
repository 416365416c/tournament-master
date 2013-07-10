import QtQuick 2.1

Rectangle{
    width: 840//Or fullscreen?
    height: 480
    Loader{
        id: loader
        focus: true
        anchors.fill: parent
        source: "Main.qml"
    }
    Text{
        text: qsTr("Sorry, but this app requires a\nconnection to the nokia internal network.")
        anchors.centerIn: parent
        visible: loader.status == Loader.Error
    }
    Text{
        text: qsTr("Loading. Please Wait.")
        anchors.centerIn: parent
        visible: loader.status == Loader.Loading
    }
}
