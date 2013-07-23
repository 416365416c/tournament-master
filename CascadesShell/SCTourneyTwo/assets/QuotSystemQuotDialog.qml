import bb.cascades 1.2
import bb.system 1.2
/* While it exists, it does not provide the versatility I need
SystemDialog { //Huh...  I guess it does exist.
    
}*/
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
            //apparently no way to create custom colors in QML? There is, I just didn't see it
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