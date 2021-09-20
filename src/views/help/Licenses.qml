import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

import "../components"

CardBox {
    implicitHeight: 280

    property int fontSize: 12

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: acrossConfig.itemSpacing * 2
        spacing: acrossConfig.itemSpacing

        Label {
            text: qsTr("Libraries and Licenses")
            font.pointSize: fontSize
            color: acrossConfig.textColor
        }

        ListView {
            Layout.fillHeight: true
            Layout.fillWidth: true

            spacing: acrossConfig.itemSpacing
            model: LicenseListModels {}
            delegate: Label {
                text: key + " - " + value
                color: acrossConfig.textColor
            }
        }
    }
}
