import bb.cascades 1.2

NavigationPane {
    id: navigationPane

    Page {
        titleBar: TitleBar {
            // Localized text with the dynamic translation and locale updates support
            title: qsTr("BB AHGL - Event Tracker") + Retranslate.onLocaleOrLanguageChanged
        }

        Container {
            layout: DockLayout {}
            ActivityIndicator {
                id: loadThing
                onCreationCompleted: {
                    loadThing.start();
                    appData.loadedOneChanged.connect(loadThing.stop());
                }
                preferredWidth: 600
            }
            ListView {
                visible: appData.loadedOne
                dataModel: appData.modelOne == null ? null : appData.modelOne
                layout: GridListLayout {
                    columnCount: 3
                    headerMode: ListHeaderMode.Standard
                }
                listItemComponents: [
                    ListItemComponent {
                        type: "header" //This has to be the worst way to do section headers!
                        Header { title: ListItemData.title }
                    },
                    ListItemComponent {
                        type: "item"
                        StandardListItem {
                            title: ListItemData.title
                            imageSource: "asset:///images/event.png"
                        }
                    }
                ]
            }
        }

        actions: ActionItem {
            title: qsTr("Lanch Pg 2") + Retranslate.onLocaleOrLanguageChanged
            ActionBar.placement: ActionBarPlacement.OnBar

            onTriggered: {
                // A second Page is created and pushed when this action is triggered.
                appData.loadEvent("Alpha Test");
                navigationPane.push(secondPageDefinition.createObject());
            }
        }
    }

    attachedObjects: [
        // Definition of the second Page, used to dynamically create the Page above.
        ComponentDefinition {
            id: secondPageDefinition
            source: "DetailsPage.qml"
        }
    ]

    onPopTransitionEnded: {
        // Destroy the popped Page once the back transition has ended.
        page.destroy();
    }
}
