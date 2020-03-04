import QtQuick 2.0
import QtGraphicalEffects 1.12
import "utils.js" as Util

BaseControllerWidget {
    id: slider

    width: 50
    height: 400

    property int handleHPadding: 5
    property int handleVPadding: 2
    property int rightPadding: 10
    property int yMax: height - handle.height - (2*handleVPadding)

    onValueChanged: {handle.y = (1-value) * slider.yMax + handleVPadding;}
    onCursorUpdate: {value = 1-Util.clip(
                    (point.y-handleVPadding-handle.height/2)/slider.yMax,
                    0, 1);}

    Item {
        id: visualBackground
        anchors.fill: parent
        property int visualPathWidth: 6
        property int visualPathX: (width- slider.rightPadding-visualPathWidth)/2
        property int visualPathY: (height-slider.yMax)/2
        property int longTickWidth: (handle.width-visualBackground.visualPathWidth)/2-3
        property int smallTickWidth: longTickWidth/2

        Image{
            id: visualSliderPathTop
            width: visualBackground.visualPathWidth
            height: visualBackground.visualPathWidth/2
            x: visualBackground.visualPathX
            y: visualBackground.visualPathY - height
            source: "faderPathTop.svg"
        }
        Image{
            id: visualSliderPathCentral
            width: visualBackground.visualPathWidth
            height: slider.yMax
            x: visualBackground.visualPathX
            y: visualBackground.visualPathY
            source: "faderPath.svg"
        }
        Image{
            id: visualSliderPathBottom
            width: visualBackground.visualPathWidth
            height: visualBackground.visualPathWidth/2
            x: visualBackground.visualPathX
            y: parent.height - visualBackground.visualPathY
            source: "faderPathBottom.svg"
        }

        Repeater{
            anchors.fill: parent
            model: [.05, .15, .25, .35, .45, .55, .65, .75, .85, .95,
                    .025, .125, .225, .325, .425, .525, .625, .725, .825, .925,
                    .075, .175, .275, .375, .475, .575, .675, .775, .875, .975]
            Item{
                height: 1
                width:slider.width-slider.rightPadding
                y: visualBackground.visualPathY + (1-modelData)*slider.yMax
                Rectangle{
                    height: 1
                    width: visualBackground.smallTickWidth
                    x: 1; y: 0
                    color:  slider.value>=modelData ? "#ccc" : "#999"
                }
                Rectangle{
                    height: 1
                    color:  slider.value>=modelData ? "#ccc" : "#999"
                    width: visualBackground.smallTickWidth
                    x: parent.width-width-1; y: 0
                }
            }
        }

        Repeater{
            anchors.fill: parent
            model: [0, .1, .2, .3, .4, .5, .6, .7, .8, .9, 1]
            Item{
                height: 1
                width:slider.width
                y: visualBackground.visualPathY + (1-modelData)*slider.yMax
                Rectangle{
                    height: 1
                    width: visualBackground.longTickWidth
                    x: 1; y: 0
                    color: slider.value>=modelData ? "#ccc" : "#999"
                }
                Rectangle{
                    height: 1
                    width: visualBackground.longTickWidth
                    x: parent.width-width-1-slider.rightPadding; y: 0
                    color: slider.value>=modelData ? "#ccc" : "#999"
                }
                Text{
                    text: (modelData*100).toString()
                    font.pixelSize: 8
                    x: parent.width-slider.rightPadding+1; y: -height/2
                    color: slider.value>=modelData ? "#ccc" : "#999"
                }
            }
        }

    }

    //  ---  Handle  ---
    Image {
        id: handle; smooth: true
        source: "fader.svg"
        x: handleVPadding
        height: width*2.2
        width: slider.width-2*handleHPadding-rightPadding
    }

    DropShadow {
            anchors.fill: handle
            horizontalOffset: 0
            verticalOffset: 0
            radius: dragging ? 12.0 : 6.0;
            samples: 12
            color: "#80000000"
            source: handle
        }
}
