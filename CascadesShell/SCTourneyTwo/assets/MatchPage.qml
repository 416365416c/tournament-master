import bb.cascades 1.2

Page {
    id: container
    property variant mobj: {
        "player1" : "Monkey",
        "player2" : "Orange",
        "title" : "Dummy Data",
        "p1race" : "Z",
        "p2race" : "Z",
        "schedule" : "",
        "winner" : "",
        "active" : true
    }
    onMobjChanged: {
        titleBar.title = mobj.title;
    }
    titleBar: TitleBar {
        id: titleBar
        // Localized text with the dynamic translation and locale updates support is unused
    }
    Container {
        PlayerLabel {
            name: mobj.player1
            race: mobj.p1race
        }
        Label { text: "VERSUS" }
        PlayerLabel {
            name: mobj.player2
            race: mobj.p2race
        }
        Label {
            text: "Schedule: " + (mobj.schedule = "" ? "Unscheduled" : mobj.schedule)
        }
        Label {
            text: "Result: " + (mobj.winner == "" ? "No result" : mobj.winner + "won!")
        }
        Button {
            enabled: mobj.active
            text: "Suggest Schedule"
            onClicked: {
                scheduleDialog.open();
            }
        }
        Button {
            enabled: mobj.active
            text: "Confirm Schedule"
            onClicked: {
                confirmDialog.open();
            }
        }
        Button {
            enabled: mobj.active
            text: "Record Result"
            onClicked: {
                resultDialog.open();
            }
        }
    }  
    attachedObjects: [
        QuotSystemQuotDialog {
            id: scheduleDialog
            DateTimePicker {
                id: dtp
                title: "Suggest Schedule:"
                value: mobj.schedule == "" ? "2013-07-07T10:00:00" : mobj.schedule
            }
            Button {
                text: "Submit"
                onClicked: { mobj.schedule = dtp.value; scheduleDialog.close(); appData.setSchedule(mobj.mId, mobj.schedule); }
            }
        },
        QuotSystemQuotDialog {
            id: confirmDialog
            Button {
                text: "Confirm Schedule"
                onClicked: { appData.confirmSchedule(mobj.mId, true); confirmDialog.close(); }
            }
            Button {
                text: "Cancel Confirmation"
                onClicked: { appData.confirmSchedule(mobj.mId, false); confirmDialog.close(); }
            }
        },
        QuotSystemQuotDialog {
            id: resultDialog
            Label {
                text: "The Winner was..."
            }
            PlayerLabel {
                name: mobj.player1
                race: mobj.p1race
                gestureHandlers: TapHandler {
                    onTapped: {
                        appData.setWinner(mobj.mId, 1)
                        resultDialog.close()
                    }
                }
            }
            PlayerLabel {
                name: mobj.player2
                race: mobj.p2race
                gestureHandlers: TapHandler {
                    onTapped: {
                        appData.setWinner(mobj.mId, 2)
                        resultDialog.close()
                    }
                }
            }
            Button {
                text: "Cancel"
                onClicked: {
                    resultDialog.close()
                }
            }
        }
    ]
}
