import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import CustomPlot 1.0

ApplicationWindow {
    id: root
    visible: true
    width: 1600
    height: 900
    title: qsTr("Hello QCustomPlot in QML")

    ColumnLayout {
        anchors.fill: parent

        CustomPlotItem {
            id: backend
            Layout.margins: 10
            Layout.fillHeight: true
            Layout.fillWidth: true
            Component.onCompleted: initCustomPlot()
        }

        ColumnLayout {
            Layout.alignment: Qt.AlignTop
            Layout.margins: 10

            RowLayout {

                Text {
                    text: qsTr("Время отрисовки графика: ")
                }

                TextField {
                    id: leftFreq
                    readOnly: true
                    text: backend.replotTime
                }

                Text {
                    text: qsTr("Время отрисовки кадра: ")
                }

                TextField {
                    id: rightFreq
                    readOnly: true
                    text: backend.repaintTime
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
        }
    }
}
