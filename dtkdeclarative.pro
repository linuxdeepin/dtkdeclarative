TEMPLATE = subdirs
SUBDIRS += \
    chameleon \
    src \
    qmlplugin \
    tests

SUBDIRS += examples
examples.depends += chameleon src qmlplugin

qmlplugin.depends += src
chameleon.depends += src
