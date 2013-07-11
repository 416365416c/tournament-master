import bb.cascades 1.2

Page {
    Container {
        Label {
            content.flags: TextContentFlag.ActiveText
            text: "More at http://go/ahgl"
            attachedObjects: [
                ContentSecurity  {
                    id: contentsecurity
                    defaultPerimeter: TextAreaScrollMode.Elastic + 0 //SecurityPerimeter.Enterprise //try to open in work browser
                }
            ]
        }
    }
}
