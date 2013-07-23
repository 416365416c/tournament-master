import bb.cascades 1.2
import bb.system 1.2 //has SecurityPerimeter?

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
                preferredWidth: 720 //virtually full screen? Or try to center?
                onCreationCompleted: {
                    ai.start();
                    appData.loadedOneChanged.connect(ai.stop());
                }
            }
            Label {
                content.flags: TextContentFlag.ActiveText
                text: "More at http://go/ahgl"
                /*attachedObjects: [ //Can't get it to open it in work browser?
                    ContentSecurity  {
                        id: contentsecurity
                        defaultPerimeter: SecurityPerimeter.Enterprise //try to open in work browser
                    }
                ]*/
               /*activeTextHandler: ActiveTextHandler {
                   onTriggered: {
                       invoke.trigger("bb.action.open");
                       event.abort(); //managed myself
                   }
               }
                attachedObjects: [
                                   Invocation {
                                       id: invoke
                                       query {
                                           //invokeTargetId: "sys.browser"
                                           uri: "http://go/ahgl"
                                           perimeter: SecurityPerimeter.Enterprise 
                                       }
                                   }
                               ]*/
            }
            ListView {
                /*
                dataModel: XmlDataModel {
                    id: dataModel
                    source: "dummydata/dummyTournamentList.xml"
                }
                */
                dataModel: appData.tournamentsModel
                listItemComponents: [
                    /*ListItemComponent {
                        type: "header"
                        Header { title: ListItemData.name}
                    },*/
                    ListItemComponent {
                        type: "item"
                        StandardListItem {
                            id: slic
                            title: ListItemData.name == "" ? "Unnamed" : ListItemData.name
                            description: ListItemData.subTitle
                            status: ListItemData.time
                            imageSource: "images/" + ListItemData.eventType + ".png"
                        }
                    }
                ]
                onTriggered: {
                    var itemData = dataModel.data(indexPath);
                    appData.loadEvent(itemData.name);
                    var pg2 = secondPageDefinition.createObject();
                    //pg2.xmlSrc = itemData.source;
                    pg2.imgSrc =  "images/" + itemData.eventType + ".png";
                    pg2.title = itemData.name;
                    pg2.status = itemData.status;
                    pg2.time = itemData.time;
                    pg2.description = itemData.description;
                    navigationPane.push(pg2);
                }
            }
        }

        actions:[ 
        ActionItem {
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
            source: "SettingsPage.qml"
        }
    ]

    onPopTransitionEnded: {
        // Destroy the popped Page once the back transition has ended.
        page.destroy();
    }
}
