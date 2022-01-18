TEMPLATE = subdirs
SUBDIRS += exhibition

CONFIG(debug, debug|release) {
    SUBDIRS += debug
}
