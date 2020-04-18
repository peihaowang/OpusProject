import QtQuick 2.0
import QtQuick.Controls 2.2

Rectangle{
    color: "transparent"
    Row {
        anchors.centerIn: parent
        spacing: 10

        RoundButton {
            icon.source: "images/img_airdrop.png"
            icon.width: 100
            icon.height: 100
            width: 100
            height: 100
            anchors.verticalCenter: parent.verticalCenter
        }

        RoundButton {
            icon.source: "images/img_facebook.png"
            icon.width: 80
            icon.height: 80
            width: 100
            height: 100
            anchors.verticalCenter: parent.verticalCenter
        }

        RoundButton {
            icon.source: "images/img_twitter.png"
            icon.width: 80
            icon.height: 80
            width: 100
            height: 100
            anchors.verticalCenter: parent.verticalCenter
        }

        RoundButton {
            icon.source: "images/img_instagram.png"
            icon.width: 80
            icon.height: 80
            width: 100
            height: 100
            anchors.verticalCenter: parent.verticalCenter
        }

        RoundButton {
            icon.source: "images/img_link.png"
            icon.width: 80
            icon.height: 80
            width: 100
            height: 100
            anchors.verticalCenter: parent.verticalCenter
        }
    }
}
