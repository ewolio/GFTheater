import QtQuick 2.0

Item {
    id: root
    property real value: 1

    width:25; height: 25;

    property bool dragging: false;
    signal pressed(point point);
    signal released(point point);
    signal cursorUpdate(point point);

    MultiPointTouchArea{
        id: touchArea
        anchors.fill: parent;
        maximumTouchPoints: 1;
        onPressed: {root.pressed(Qt.point(touchPoints[0].x, touchPoints[0].y))}
        onReleased: {root.released(Qt.point(touchPoints[0].x, touchPoints[0].y))}
        onUpdated: { if(touchPoints.length>0) root.cursorUpdate(Qt.point(touchPoints[0].x, touchPoints[0].y)) }
    }

    MouseArea{
        id: mouseArea
        anchors.fill: parent;
        onPressed: {
            if(!(mouse.modifiers & Qt.ControlModifier)){
                root.dragging = true;
                root.pressed(Qt.point(mouse.x, mouse.y));
            }else
                root.dragging = false;
        }
        onReleased: {if(root.dragging) root.released(Qt.point(mouse.x, mouse.y));
                     root.dragging=false;}
        onClicked:  {
            if((mouse.modifiers & Qt.ControlModifier))
                return  // Assign
            else
                root.cursorUpdate(Qt.point(mouse.x, mouse.y));
        }
        onPositionChanged: {if(root.dragging) cursorUpdate(Qt.point(mouse.x, mouse.y));}

    }
}


