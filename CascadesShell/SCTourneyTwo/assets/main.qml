import bb.cascades 1.2

NavigationPane {
    id: navigationPane

    Page {
        titleBar: TitleBar {
            // Localized text with the dynamic translation and locale updates support
            title: qsTr("BB AHGL - Event Tracker") + Retranslate.onLocaleOrLanguageChanged
        }

        Container {
            ActivityIndicator {
                id: ai
                onCreationCompleted: {
                    ai.start();
                }
            }
            ListView {
                dataModel: XmlDataModel {
                    id: dataModel
                    source: "dummydata/dummyTournamentList.xml"
                }
                listItemComponents: [
                    ListItemComponent {
                        type: "eventGroup"
                        Header { title: ListItemData.title }
                    },
                    ListItemComponent {
                        type: "tournament"
                        StandardListItem {
                            id: slic
                            title: ListItemData.name
                            description: ListItemData.subTitle
                            status: ListItemData.time
                            imageSource: ListItemData.imgSrc
                        }
                    }
                ]
                onTriggered: {
                    var itemData = dataModel.data(indexPath);
                    var pg2 = secondPageDefinition.createObject();
                    pg2.xmlSrc = itemData.source;
                    pg2.imgSrc = itemData.imgSrc;
                    pg2.title = itemData.name;
                    pg2.time = itemData.time;
                    pg2.description = itemData.desc;
                    navigationPane.push(pg2);
                }
            }
        }

        actions:[ ActionItem {
            title: qsTr("Info Page") + Retranslate.onLocaleOrLanguageChanged
            ActionBar.placement: ActionBarPlacement.OnBar

            onTriggered: {
                // A second Page is created and pushed when this action is triggered.
                navigationPane.push(infoPageDefinition.createObject());
            }
        }
        , ActionItem {
            title: qsTr("Settings Page") + Retranslate.onLocaleOrLanguageChanged
            ActionBar.placement: ActionBarPlacement.OnBar
            
            onTriggered: {
                // A second Page is created and pushed when this action is triggered.
                navigationPane.push(settingsPageDefinition.createObject());
            }
        }
    
    ]
    }

    attachedObjects: [
        // Definition of the second Page, used to dynamically create the Page above.
        ComponentDefinition {
            id: secondPageDefinition
            source: "TournamentPage.qml"
        },
        ComponentDefinition {
            id: infoPageDefinition
            source: "InfoPage.qml"
        },
        ComponentDefinition {
            id: settingsPageDefinition
            source: "settingsPage.qml"
        }
    ]

    onPopTransitionEnded: {
        // Destroy the popped Page once the back transition has ended.
        page.destroy();
    }
}
