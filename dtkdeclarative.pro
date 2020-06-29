TEMPLATE = subdirs
SUBDIRS += \
    chameleon

CONFIG(debug, debug|release) {
    SUBDIRS += examples
    examples.depends += chameleon
}
