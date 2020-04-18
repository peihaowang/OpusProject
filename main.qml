import QtQuick 2.9
import QtQuick.Controls 2.2

import com.opus.components 1.0

ApplicationWindow {
    id: window
    visible: true
    width: 640
    height: 480
    title: "Opus"

    Drawer {
        id: drawer
        width: window.width * 0.3
        height: window.height

        Column {
            anchors.fill: parent

            Item {
                width: 1 // dummy value != 0
                height: 50
            }

            RoundButton {
                icon.source: "images/img_user.png"
                icon.width: 80
                icon.height: 80
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Text{
                text: "Mr. Opus"
                font.pixelSize: Qt.application.font.pixelSize * 1.3
                font.bold: true
                anchors.horizontalCenter: parent.horizontalCenter
                color: "white"
            }

            Text{
                text: "20, Male, Shanghai"
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.topMargin: 30
                color: "white"
            }

            Item {
                width: 1 // dummy value != 0
                height: 20
            }

            ItemDelegate {
                text: "Resume"

                icon.source: "images/img_resume.png"
                icon.width: 16
                icon.height: 16

                width: parent.width
                onClicked: {
                    drawer.close()
                }
            }

            ItemDelegate {
                text: "Restart"

                icon.source: "images/img_restart.png"
                icon.width: 16
                icon.height: 16

                width: parent.width
                onClicked: {
                    scribbleArea.restart();
                    drawer.close()
                }
            }

            ItemDelegate {
                text: "Stop"

                icon.source: "images/img_stop.png"
                icon.width: 16
                icon.height: 16

                width: parent.width
                onClicked: {
                    scribbleArea.stop();
                    goButton.visible = true;
                    drawer.close()
                }
            }

            ItemDelegate {
                text: "Diary"

                icon.source: "images/img_diary.png"
                icon.width: 16
                icon.height: 16

                width: parent.width
                onClicked: {
                    stackView.push("diary.qml")
                    drawer.close()
                }
            }

            ItemDelegate {
                text: "Share"

                icon.source: "images/img_share.png"
                icon.width: 16
                icon.height: 16

                width: parent.width
                onClicked: {
                    stackView.push("share.qml")
                    drawer.close()
                }
            }

            ItemDelegate {
                text: "Settings"

                icon.source: "images/img_settings.png"
                icon.width: 16
                icon.height: 16

                width: parent.width
                onClicked: {
//                    stackView.push("Page2Form.ui.qml")
                    drawer.close()
                }
            }
        }
    }

    StackView {
        id: stackView
        initialItem: Rectangle{
            ScribbleArea{
                id: scribbleArea
                anchors.fill: parent
                paused: drawer.opened || goButton.visible || stackView.depth > 1
            }

            RoundButton {
                id: goButton
                anchors.centerIn: parent
                width: 200
                height: 200

                text: "\u25B6"
                font.pixelSize: Qt.application.font.pixelSize * 5

                onClicked: {
                    goButton.visible = false;
                }
            }

        }
        anchors.fill: parent
    }

    ToolButton {
        id: toolButton
        anchors.left: parent.left
        anchors.top: parent.top
        width: 75
        height: 75

        text: stackView.depth > 1 ? "\u25C0" : "\u2630"
        font.pixelSize: Qt.application.font.pixelSize * 1.6
        onClicked: {
            if(stackView.depth == 1){
                drawer.open()
            }else{
                stackView.pop();
            }
        }
    }

}
