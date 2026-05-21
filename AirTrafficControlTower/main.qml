import QtQuick 2.15
import QtPositioning
import QtLocation

Item {
    id: root
    objectName: "MapView"

    Plugin {
        id: mapPlugin
        name: "osm"

        PluginParameter {
            name: "osm.useragent"
            value: "AirTrafficControlTower"
        }
        PluginParameter {
            name: "osm.mapping.host"
            value: "https://cartodb-basemaps-a.global.ssl.fastly.net/"
        }
        PluginParameter {
            name: "osm.mapping.copyright"
            value: "© OpenStreetMap contributors, © CartoDB"
        }
    }

    // Properties for selected object
    property double mapLatitude: 32.4279
    property double mapLongitude: 53.6880

    Map {
        id: map
        plugin: mapPlugin
        anchors.fill: parent
        center: QtPositioning.coordinate(root.mapLatitude, root.mapLongitude)
        // activeMapType: supportedMapTypes[supportedMapTypes.length - 1]
        activeMapType: supportedMapTypes[3]

        // onSupportedMapTypesChanged: {
        //         for (var i = 0; i < supportedMapTypes.length; ++i) {
        //             if (supportedMapTypes[i].name === "Cycle Map") {
        //                 console.log("MapType:", supportedMapTypes[i].name, i)
        //                 break
        //             }
        //         }
        //     }

        MapItemView {
            model: objectListModel
            delegate: MapQuickItem {
                coordinate: QtPositioning.coordinate(latitude, longitude)
                anchorPoint.x: 16
                anchorPoint.y: 16
                sourceItem: Image {
                    source: icon
                    width: 32
                    height: 32
                    rotation: azimuth
                }
            }
        }

        WheelHandler {
            id: wheel
            acceptedDevices: Qt.platform.pluginName === "cocoa" || Qt.platform.pluginName === "wayland" ? PointerDevice.Mouse | PointerDevice.TouchPad : PointerDevice.Mouse
            rotationScale: 1 / 120
            property: "zoomLevel"
        }

        DragHandler {
            id: drag
            target: null
            onTranslationChanged: delta => map.pan(-delta.x, -delta.y)
        }

        Shortcut {
            enabled: map.zoomLevel < map.maximumZoomLevel
            sequence: StandardKey.ZoomIn
            onActivated: map.zoomLevel = Math.round(map.zoomLevel + 1)
        }
        Shortcut {
            enabled: map.zoomLevel > map.minimumZoomLevel
            sequence: StandardKey.ZoomOut
            onActivated: map.zoomLevel = Math.round(map.zoomLevel - 1)
        }
    }
}
