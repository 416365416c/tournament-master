import QtQuick 2.1
import QtQuick.Window 2.1
import "../../Common/qml"

Window {
    width: td.width
    height: td.height
    visible: true
    property TourneyDisplay td: theTD
    TourneyDisplay{id: theTD}
}
