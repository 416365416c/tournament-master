import bb.cascades 1.2

Dialog {
    id: scheduleDialog//NOTE: System dialogs apparently have no QML API? Lame...
    default property alias contentzor: mainContainer.controls
    Container {
        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Fill
        preferredHeight: 1280
        preferredWidth: 1000
        layout: DockLayout {
            //???
        }
        ImageView {
            //apparently no way to create custom colors in QML?
            imageSource: "asset:///images/slightlytransparentwhite...youheardme,thisneedsanimage.png"
            scalingMethod: ScalingMethod.Fill
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
            preferredHeight: 1280
            preferredWidth: 1000
        }
        Container {
            id: mainContainer
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
            layout: StackLayout {
            
            }
        }
    }
}