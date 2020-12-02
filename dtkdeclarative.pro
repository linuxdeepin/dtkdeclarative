TEMPLATE = subdirs
SUBDIRS += \
    chameleon \
    src \
    qmlplugin

CONFIG(debug, debug|release) {
    SUBDIRS += examples
    examples.depends += chameleon src
}

qmlplugin.depends += src
