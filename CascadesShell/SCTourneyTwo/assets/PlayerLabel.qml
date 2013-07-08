import bb.cascades 1.2

Container {
    property alias race: rcImg.race
    property alias name: nmLbl.text
    layout: StackLayout {
        orientation: LayoutOrientation.LeftToRight
    }
    RacialImage{ id: rcImg }
    Label { id: nmLbl }
}
