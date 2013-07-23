import bb.cascades 1.2

Container {
    property alias race: rcImg.race
    property alias name: nmLbl.text
    background: Color.create(0.8,0.8,0.8,0.8)
    layout: StackLayout {
        orientation: LayoutOrientation.LeftToRight
    }
    RacialImage{ id: rcImg }
    QuotLabelQuot { id: nmLbl }
}
