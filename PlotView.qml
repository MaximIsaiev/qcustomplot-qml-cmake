import QtQuick 2.0
import CustomPlot 1.0
import QtQuick.Layouts 1.0

Item {
    id: plotForm

    CustomPlotItem {
        id: customPlot
        anchors.fill: plotForm
        Component.onCompleted: initCustomPlot()
    }
}
