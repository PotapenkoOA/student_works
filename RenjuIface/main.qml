import QtQuick 2.12
import QtQuick.Window 2.12

Window {
    visible: true
    title: qsTr("Hello World")
    Component.onCompleted: {
        showMaximized();
        Controller.startController();
    }
    color: "#444"
    
    Item {
        id: board
        width: back.width + log.width + log.anchors.leftMargin
        height: back.height
        property int fieldSideLen: 32
        property int fieldSpacing: 2
        property int borderWidth: 2
        property alias backColor: back.color
        property ListModel logModel: ListModel {
            ListElement {
                pc: 0
                px: 1
                py: 2
            }
            ListElement {
                pc: 1
                px: 15
                py: 27
            }
        }
        Rectangle {
            x: 10
            y: 10
            id: back
            width: board.fieldSideLen * 31 + board.fieldSpacing * 30 + board.borderWidth * 2
            height: width
            border.width: board.borderWidth
            color: "#ccc"
            border.color: "#ccc"
            Grid {
                id: fields
                x: board.borderWidth
                y: x
                columns: 31
                spacing: board.fieldSpacing
                Repeater {
                    model: 31*31
                    Rectangle {
                        id: field
                        property int owner: -1
                        width: board.fieldSideLen
                        height: width
                        color: "#666"
                        Rectangle {
                            width: parent.width-4
                            height: width
                            anchors.centerIn: parent
                            radius: width*0.5
                            color: field.owner == 0 ? "#edd" :
                                                      field.owner == 1 ? "#121" :
                                                                         field.color
                        }
                    }
                }
            }
        }
        Rectangle {
            id: log
            anchors.top: back.top
            anchors.left: back.right
            anchors.leftMargin: 10
            height: back.height
            width: 200
            color: "#666"
            clip: true
            ListView {
                id: logScroll
                anchors.fill: parent
                anchors.margins: 6
                model: board.logModel
                delegate: 
                    Text {
                    width: parent.width
                    height: 26
                    color: pc === 0 ? "#edd" : "#121"
                    text: px.toFixed(0) + ", " + py.toFixed(0)
                    font.pixelSize: height-4
                }
            }
        }
        
        function clearFields(val) {
            var ff = fields.children;
            for (var i = 0; i < ff.length; i++) {
                if (ff[i].owner)
                    ff[i].owner = val;
            }
        }
        function clearLog() {
            board.logModel.clear();
        }
        
        function set(x, y, color) {
            if (x < 0 || x > 30) return;
            if (y < 0 || y > 30) return;
            if (!(color === 0 || color === 1)) return;
            var idx = x + (30 - y) * 31;
            if (fields.children[idx].owner !== -1) return;
            fields.children[idx].owner = color;
        }
        
        Component {
            id: logRecord
            ListElement {
                
            }
        }
        Connections {
            target: Controller
            onStart_: {
                board.clearFields(-1);
                board.clearLog();
            }
            onMove_: function(cmd) {
                var o = JSON.parse(cmd);
                var x = o.point.x;
                var y = o.point.y;
                var color = o.color;
                board.set(x, y, color);
                
                board.logModel.append({"px": x, "py": y, "pc": color });
                logScroll.positionViewAtEnd();
            }
            onSave_: function (path) {
                board.grabToImage(function(result) {
                    result.saveToFile(path);
                });
            }
        }
    }
}
