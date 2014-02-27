import QtQuick 2.1
import "LineControl.js" as Control

Rectangle {
    id:r
    property real x1 : 0
    property real y1 : 0
    property real x2 : 0
    property real y2 : 0
    x: x1
    y: y1
    transformOrigin: Item.TopLeft
    smooth: true
    width: Control.mag(x1-x2, y1-y2);
    rotation: Control.angle(x1-x2, y1-y2) + 180;

    //Default look
    height: 1
    color: 'white'
    z:-1
}
