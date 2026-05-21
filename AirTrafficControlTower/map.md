# Qt Location Open Street Map Plugin — Reviewer Notes

## Key Concepts

- **Qt Location OSM Plugin**: Enables Qt applications to use OpenStreetMap (OSM) services for mapping, geocoding, routing, and places.
- **Plugin Key**: `"osm"` is used to load the OSM plugin.
- **Data Providers**: Map data and imagery are sourced from OSM, ThunderForest, and contributors under the Open Database License.
- **Map Types**: Standard map types may change or be removed without notice, depending on provider availability.
- **Parameters**: Customization is done via plugin parameters, all prefixed with `osm.` since Qt 5.5.

## Important Tips

- **Provider Selection**: Always verify the terms of service for any map/data provider used in production.
- **HTTPS Support**: Some providers serve tiles over HTTPS, which may require SSL support in Qt (especially on Android).
- **Custom Tile Servers**: Use `osm.mapping.custom.host` for custom map tiles; set `Map::activeMapType` to `MapType.CustomMap` to use them.
- **Caching**: Tile caching is configurable for disk, memory, and texture. Default locations and sizes are provided, but can be overridden.
- **User Agent**: Set `osm.useragent` to uniquely identify your application; some providers may block requests without it.

## Examples

**Basic Plugin Setup in QML**

````qml
Plugin {
    name: "osm"
    PluginParameter { name: "osm.useragent"; value: "MyQtOSMApp" }
    PluginParameter { name: "osm.mapping.host"; value: "https://tile.openstreetmap.org/" }
    PluginParameter { name: "osm.mapping.copyright"; value: "© OpenStreetMap contributors" }
    PluginParameter { name: "osm.routing.host"; value: "https://router.project-osrm.org/" }
    PluginParameter { name: "osm.geocoding.host"; value: "https://nominatim.openstreetmap.org/" }
}
````

**Custom Tile Server Example**

````qml
PluginParameter {
    name: "osm.mapping.custom.host"
    value: "https://my.custom.tileserver.com/"
}
````

**Cache Configuration Example**

````qml
PluginParameter {
    name: "osm.mapping.cache.directory"
    value: "/path/to/cache"
}
PluginParameter {
    name: "osm.mapping.cache.disk.size"
    value: "100000000" // bytes
}
````

## Warnings

- **Map Types May Change**: Standard map types and providers can change or be removed without notice.
- **SSL/HTTPS Issues**: On platforms without SSL support (e.g., Android), HTTPS tile servers may not work.
- **Cache Invalidation**: Changing the custom tile server (`osm.mapping.custom.host`) invalidates the previous cache for custom maps.
- **Parameter Prefix**: All parameters must be prefixed with `osm.` in Qt 5.5 and later.
- **Provider Attribution**: Always display proper copyright information as required.

## References

- Source file: main.qml
- Qt Location Plugin Documentation: <https://doc.qt.io/qt-6/location-plugin-parameters.html>
- OpenStreetMap Wiki (Providers): <https://wiki.openstreetmap.org/wiki/Standard_tile_layer>

## Common Pitfalls

- **Incorrect Parameter Prefix**: Omitting `osm.` prefix will result in parameters being ignored.
- **Not Setting User Agent**: Some providers may block requests without a custom user agent.
- **Cache Directory Issues**: Ensure the cache directory is writable and properly set for persistent caching.
- **Custom Map Activation**: Setting a custom host does not automatically switch to custom map type; must set `Map::activeMapType`.

## Reviewer Checklist

- Are all plugin parameters correctly prefixed?
- Is the user agent set for network requests?
- Is the correct tile server URL used for the desired map style?
- Are cache settings appropriate for the application's needs?
- Is provider attribution displayed as required?
- Are terms of service for data providers reviewed and followed?
