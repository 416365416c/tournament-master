import bb.cascades 1.2

Page {
    Container {
        TextField  {
            id: tf
            hintText: "Alternate Server Address"
        }
        Button {
            text: "Change servers"
            onClicked: appData.changeServer(tf.text);
        }
        Label {
            text: "Warning: Server changes are not currently persistent!"
        }
    }
}
