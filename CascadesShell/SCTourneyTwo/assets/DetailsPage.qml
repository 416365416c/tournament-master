import bb.cascades 1.2

Page {
    titleBar: TitleBar {
        // Localized text with the dynamic translation and locale updates support
        title: qsTr("BB AHGL - Foo Event") + Retranslate.onLocaleOrLanguageChanged
    }
    Container {
        Label {
            text: appData.secondPageText
        }
    }
}
