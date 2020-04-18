import QtQuick 2.0

Rectangle {

    color: "black"

    GridView {
        id: gridView
        anchors.fill: parent
        anchors.topMargin: 100
        anchors.leftMargin: 50
        anchors.rightMargin: 50

        cellWidth: 150
        cellHeight: 200

        model: ListModel {
            ListElement {
                name: "2019/7/5"
                portrait: "images/img_diary1.jpg"
            }
            ListElement {
                name: "2019/7/6"
                portrait: "images/img_diary2.jpg"
            }
            ListElement {
                name: "2019/7/7"
                portrait: "images/img_diary3.jpg"
            }
            ListElement {
                name: "2019/7/8"
                portrait: "images/img_diary4.jpg"
            }
            ListElement {
                name: "2019/7/9"
                portrait: "images/img_diary5.jpg"
            }
            ListElement {
                name: "2019/7/10"
                portrait: "images/img_diary6.jpg"
            }
            ListElement {
                name: "2019/7/11"
                portrait: "images/img_diary7.jpg"
            }
        }
        delegate: Rectangle{
            color: touchArea.pressed ? Qt.rgba(100/255, 100/255, 100/255, 100/255) : "transparent"
            width: 150
            height: 200
            radius: 5

            MouseArea{
                id: touchArea
                anchors.fill: parent
            }

            Column {
                id: itemCol
                spacing: 10

                anchors.centerIn: parent

                Image {
                    source: portrait
                    height: 150
                    fillMode: Image.PreserveAspectFit
                    anchors.horizontalCenter: parent.horizontalCenter
                }
                Text {
                    text: name;
                    color: "white"
                    anchors.horizontalCenter: parent.horizontalCenter
                }
            }
        }

    }

}
