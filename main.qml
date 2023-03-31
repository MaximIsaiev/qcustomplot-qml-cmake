import QtQuick 2.7
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import CustomPlot 1.0

ApplicationWindow {
    id: root
    visible: true
    width: 1280
    height: 720
    title: qsTr("Hello QCustomPlot in QML")

    RowLayout {
        anchors.fill: parent

        CustomPlotItem {
            id: backend
            Layout.margins: 10
            Layout.fillHeight: true
            Layout.preferredWidth: root.width * 2 / 3
            Component.onCompleted: initCustomPlot()
        }

        ColumnLayout {
            Layout.alignment: Qt.AlignTop

            RowLayout {

                Text {
                    text: qsTr("От: ")
                }

                TextField {
                    id: leftFreq
                    validator: IntValidator {bottom: 0; top: 500}
                }

                Text {
                    text: qsTr("До: ")
                }

                TextField {
                    id: rightFreq
                    validator: IntValidator {bottom: 0; top: 500}
                }
            }
            RowLayout {
                Button {
                    Layout.alignment: Qt.AlignTop
                    Layout.rightMargin: 10
                    Layout.fillWidth: true
                    height: 30
                    text: qsTr("Add range")

                    onClicked: backend.addRange(Number(leftFreq.text), Number(rightFreq.text), 0)
                }

                Button {
                    Layout.alignment: Qt.AlignTop
                    Layout.rightMargin: 10
                    Layout.fillWidth: true
                    height: 30
                    text: qsTr("Add post")

                    onClicked: backend.addPost()
                }

                Button {
                    Layout.alignment: Qt.AlignTop
                    Layout.rightMargin: 10
                    Layout.fillWidth: true
                    height: 30
                    text: qsTr("Paint")

                    onClicked: backend.paint()
                }
            }
            Button {
                Layout.alignment: Qt.AlignTop
                Layout.rightMargin: 10
                Layout.fillWidth: true
                height: 30
                text: qsTr("Reset Zoom")

                onClicked: backend.reset()
            }
        }
    }
}
