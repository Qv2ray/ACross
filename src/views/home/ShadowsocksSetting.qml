import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import ACross

Item {
    implicitWidth: 680
    implicitHeight: 160

    property int fontSize: 14

    GridLayout {
        anchors.fill: parent

        columns: 4
        rowSpacing: acrossConfig.itemSpacing
        columnSpacing: acrossConfig.itemSpacing

        Label {
            Layout.fillWidth: true
            Layout.columnSpan: 4

            text: qsTr("Shadowsocks Setting")
            font.pointSize: fontSize
            color: acrossConfig.textColor
        }

        Label {
            text: qsTr("IVCheck")
            color: acrossConfig.textColor
        }

        Item {
            Layout.fillWidth: true
            Layout.columnSpan: 2
        }

        SwitchBox {
            id: ivCheckSelect
        }

        Label {
            text: qsTr("Security")
            color: acrossConfig.textColor
        }

        DropDownBox {
            id: securitySelect
            Layout.fillWidth: true
            Layout.columnSpan: 3

            model: ["aes-256-gcm", "aes-128-gcm", "chacha20-ietf-poly1305", "chacha20-poly1305", "plain", "none"]
        }

        Item {
            Layout.fillHeight: true
        }
    }

    Connections {
        target: protocolSettingsLoader

        function onAcceptAll() {
            shadowsocksSetting = {
                "ivCheck": ivCheckSelect.checked,
                "security": securitySelect.currentText
            }
        }
    }
}
