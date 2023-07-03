#####################################################################
## dtkdeclarativeprivatesplugin Plugin:
#####################################################################

qt_add_qml_module(dtkdeclarativeprivatesplugin
    PLUGIN_TARGET dtkdeclarativeprivatesplugin
    URI "org.deepin.dtk.private"
    VERSION "1.0"
    QML_FILES
        "ProgressBarImpl.qml"
        "ProgressBarPanel.qml"
        "ColorOverlay.qml"
        "SoftwareColorOverlay.qml"
        "ArrowListViewButton.qml"
        "ButtonPanel.qml"
        "KeySequenceLabel.qml"
    OUTPUT_DIRECTORY
        "${PROJECT_BINARY_DIR}/plugins/org/deepin/dtk/private"
)

target_link_libraries(dtkdeclarativeprivatesplugin
PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Quick
)

