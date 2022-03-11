TEMPLATE = subdirs
SUBDIRS += \
    chameleon \
    src \
    qmlplugin \
    tests \
    doc \

SUBDIRS += examples
examples.depends += chameleon src qmlplugin

qmlplugin.depends += src
chameleon.depends += src
