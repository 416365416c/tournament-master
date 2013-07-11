import bb.cascades 1.2

Page {
    id: container
    //property url xmlSrc: "notyet"
    property url imgSrc: "images/event.png"
    property string description: "Non-descript event"
    property string time: "Somewhen"
    property alias title: titleBar.title
    property string status: "done"
    titleBar: TitleBar { 
        id: titleBar
        // Localized text with the dynamic translation and locale updates support is unused
    }
    Container {
        ImageView {
            imageSource: container.imgSrc
        }
        Label {
            text: container.description
            multiline: true
        }
        Label {
            text: container.time
            textStyle.fontStyle: FontStyle.Italic
        }
        Button {
            visible: container.status == "signup"
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Bottom
            text: "Sign me up!"
            onClicked: {
                signupDialog.open();
            }
        }
        ListView {
            id: matchView
            visible: container.status != "signup"
            /*
            dataModel: XmlDataModel {
                source: container.xmlSrc
            }
            */
            dataModel: appData.matchesModel
            listItemComponents: [
                ListItemComponent {
                    type: "item"
                    StandardListItem {
                        description: ListItemData.player1 + " vs " + (ListItemData.player2 == "" ? "bye" : ListItemData.player2)
                        title: ListItemData.title
                        status: ListItemData.winner == "" ? (ListItemData.schConf ? "Scheduled" : "Unplayed") : "Played"
                    }
                }/*,
                ListItemComponent {
                    type: "header"
                    Header {
                        title: ListItemData.title
                    }
                }*/
            ]
            onTriggered: {
                var itemData = dataModel.data(indexPath);
                var pg3 = thirdPageDefinition.createObject();
                pg3.mobj = itemData;
                navigationPane.push(pg3);
            }
        }
    }  
    attachedObjects: [
        // Definition of the third Page, used to dynamically create the Page above.
        ComponentDefinition {
            id: thirdPageDefinition
            source: "MatchPage.qml"
        },
        QuotSystemQuotDialog {
            id: signupDialog
            Label {
                text: "Sign up now!"
            }
            TextField {
                id: name
                hintText: "Screen Name"
            }
            TextField {
                id: email
                hintText: "Friend code OR real id email"
                inputMode: TextFieldInputMode.EmailAddress
            }
            SegmentedControl {
                id: raceSelect
                preferredHeight: 800
                Option {
                    imageSource: "asset:///images/terranSml.png"
                    value: "T"
                    text: "T"
                    selected: true
                }
                Option {
                    value: "Z"
                    text: "Z"
                    imageSource: "asset:///images/zergSml.png"
                }
                Option {
                    value: "P"
                    text: "P"
                    imageSource: "asset:///images/tossSml.png"
                }
                Option {
                    value: "R"
                    text: "R"
                    imageSource: "asset:///images/randomSml.png"
                }
            } 
            Button {
                text: "Submit"
                onClicked: {
                    appData.registerPlayer(container.title, name.text, email.text, raceSelect.selectedValue);
                    signupDialog.close()
                }
                horizontalAlignment: HorizontalAlignment.Center
            }
            Button {
                text: "Cancel"
                onClicked: {
                    signupDialog.close()
                }
                horizontalAlignment: HorizontalAlignment.Center
            }
        }
    ]
}
