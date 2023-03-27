import QtQuick 2.0
import CustomPlot 1.0
import QtQuick.Layouts 1.0

Item {
    id: plotForm

    ColumnLayout {
        anchors.fill: parent

        CustomPlotItem {
            id: customPlot
            Layout.fillWidth: true
            Layout.fillHeight: true

            Component.onCompleted: initCustomPlot()
        }
    }
}
