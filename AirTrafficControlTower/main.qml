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
        // Use the official OSM tile server for general use
        PluginParameter {
            name: "osm.mapping.host"
            value: "https://tile.openstreetmap.org/"
        }
        // Copyright notice for OSM
        PluginParameter {
            name: "osm.mapping.copyright"
            value: "© OpenStreetMap contributors"
        }
        // Optional: Use public OSRM routing server (or leave default)
        PluginParameter {
            name: "osm.routing.host"
            value: "https://router.project-osrm.org/route/v1"
        }
        // Optional: Use Nominatim for geocoding (or leave default)
        PluginParameter {
            name: "osm.geocoding.host"
            value: "https://nominatim.openstreetmap.org/"
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

        // onSupportedMapTypesChanged: {
        //         for (var i = 0; i < supportedMapTypes.length; ++i) {
        //             if (supportedMapTypes[i].name === "Cycle Map") {
        //                 console.log("MapType:", supportedMapTypes[i].name, i)
        //                 break
        //             }
        //         }
        //     }
        activeMapType: supportedMapTypes[3]

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
