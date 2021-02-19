TEMPLATE = subdirs
SUBDIRS += \
    chameleon \
    src \
    qmlplugin \
    tests

CONFIG(debug, debug|release) {
    SUBDIRS += examples
    examples.depends += chameleon src qmlplugin
}

qmlplugin.depends += src
chameleon.depends += src
