

file(GLOB PUBLIC_HEADERS "./*.h")
file(GLOB_RECURSE HEADERS "./*.h")
file(GLOB_RECURSE SRCS   "./*.cpp")
file(GLOB D_HEADERS   "./D*")
set(ASSETS_RCS ${PROJECT_SOURCE_DIR}/src/dtkdeclarative_assets.qrc)
file(GLOB TS_FILES "translations/*.ts")

set(DTK_SOURCES
    ${PROJECT_SOURCE_DIR}/src/private/dblitframebuffernode.cpp    ${PROJECT_SOURCE_DIR}/src/private/dblitframebuffernode_p.h
    ${PROJECT_SOURCE_DIR}/src/private/dblurimagenode.cpp    ${PROJECT_SOURCE_DIR}/src/private/dblurimagenode_p.h
    ${PROJECT_SOURCE_DIR}/src/private/dcoloroverlaynode.cpp    ${PROJECT_SOURCE_DIR}/src/private/dcoloroverlaynode.h
    ${PROJECT_SOURCE_DIR}/src/private/dconfigwrapper_p.h ${PROJECT_SOURCE_DIR}/src/private/dconfigwrapper.cpp
    ${PROJECT_SOURCE_DIR}/src/private/dhandlecontextmenuwindow_p.h    ${PROJECT_SOURCE_DIR}/src/private/dhandlecontextmenuwindow.cpp
    ${PROJECT_SOURCE_DIR}/src/private/dmaskeffectnode_p.h    ${PROJECT_SOURCE_DIR}/src/private/dmaskeffectnode.cpp
    ${PROJECT_SOURCE_DIR}/src/private/dmessagemanager_p.h    ${PROJECT_SOURCE_DIR}/src/private/dmessagemanager.cpp
    ${PROJECT_SOURCE_DIR}/src/private/dobjectmodelproxy_p.h    ${PROJECT_SOURCE_DIR}/src/private/dobjectmodelproxy.cpp
    ${PROJECT_SOURCE_DIR}/src/private/dopacitymasknode.cpp    ${PROJECT_SOURCE_DIR}/src/private/dopacitymasknode.h
    ${PROJECT_SOURCE_DIR}/src/private/dpopupwindowhandle_p.h    ${PROJECT_SOURCE_DIR}/src/private/dpopupwindowhandle.cpp
    ${PROJECT_SOURCE_DIR}/src/private/dqmlglobalobject_p_p.h    ${PROJECT_SOURCE_DIR}/src/private/dqmlglobalobject_p.h    ${PROJECT_SOURCE_DIR}/src/private/dqmlglobalobject.cpp
    ${PROJECT_SOURCE_DIR}/src/private/dquickapploaderitem_p.h    ${PROJECT_SOURCE_DIR}/src/private/dquickapploaderitem.cpp
    ${PROJECT_SOURCE_DIR}/src/private/dquickarrowboxpath_p.h    ${PROJECT_SOURCE_DIR}/src/private/dquickarrowboxpath.cpp
    ${PROJECT_SOURCE_DIR}/src/private/dquickbehindwindowblur_p_p.h    ${PROJECT_SOURCE_DIR}/src/private/dquickbehindwindowblur_p.h
    ${PROJECT_SOURCE_DIR}/src/private/dquickbehindwindowblur.cpp    ${PROJECT_SOURCE_DIR}/src/private/dquickbusyindicator_p.h    ${PROJECT_SOURCE_DIR}/src/private/dquickbusyindicator.cpp
    ${PROJECT_SOURCE_DIR}/src/private/dquickcoloroverlay_p.h    ${PROJECT_SOURCE_DIR}/src/private/dquickcoloroverlay.cpp
    ${PROJECT_SOURCE_DIR}/src/private/dquickcontrolpalette_p.h    ${PROJECT_SOURCE_DIR}/src/private/dquickcontrolpalette.cpp
    ${PROJECT_SOURCE_DIR}/src/private/dquickdciicon_p.h    ${PROJECT_SOURCE_DIR}/src/private/dquickdciicon.cpp
    ${PROJECT_SOURCE_DIR}/src/private/dquickdciiconimage_p_p.h    ${PROJECT_SOURCE_DIR}/src/private/dquickdciiconimage_p.h    ${PROJECT_SOURCE_DIR}/src/private/dquickdciiconimage.cpp
    ${PROJECT_SOURCE_DIR}/src/private/dquickglow_p_p.h    ${PROJECT_SOURCE_DIR}/src/private/dquickglow_p.h    ${PROJECT_SOURCE_DIR}/src/private/dquickglow.cpp
    ${PROJECT_SOURCE_DIR}/src/private/dquickiconimage_p_p.h    ${PROJECT_SOURCE_DIR}/src/private/dquickiconimage_p.h    ${PROJECT_SOURCE_DIR}/src/private/dquickiconimage.cpp
    ${PROJECT_SOURCE_DIR}/src/private/dquickiconlabel_p_p.h    ${PROJECT_SOURCE_DIR}/src/private/dquickiconlabel_p.h    ${PROJECT_SOURCE_DIR}/src/private/dquickiconlabel.cpp
    ${PROJECT_SOURCE_DIR}/src/private/dquickimageprovider_p.h    ${PROJECT_SOURCE_DIR}/src/private/dquickimageprovider.cpp
    ${PROJECT_SOURCE_DIR}/src/private/dquickinwindowblur_p.h    ${PROJECT_SOURCE_DIR}/src/private/dquickinwindowblur.cpp
    ${PROJECT_SOURCE_DIR}/src/private/dquickitemviewport_p.h    ${PROJECT_SOURCE_DIR}/src/private/dquickkeylistener_p.h    ${PROJECT_SOURCE_DIR}/src/private/dquickkeylistener.cpp
    ${PROJECT_SOURCE_DIR}/src/private/dquickopacitymask_p_p.h    ${PROJECT_SOURCE_DIR}/src/private/dquickopacitymask_p.h    ${PROJECT_SOURCE_DIR}/src/private/dquickopacitymask.cpp
    ${PROJECT_SOURCE_DIR}/src/private/dquickrectangle_p_p.h    ${PROJECT_SOURCE_DIR}/src/private/dquickrectangle_p.h    ${PROJECT_SOURCE_DIR}/src/private/dquickrectangle.cpp
    ${PROJECT_SOURCE_DIR}/src/private/dquickwaterprogressattribute_p.h    ${PROJECT_SOURCE_DIR}/src/private/dquickwaterprogressattribute.cpp
    ${PROJECT_SOURCE_DIR}/src/private/drectanglenode_p.h    ${PROJECT_SOURCE_DIR}/src/private/drectanglenode.cpp
    ${PROJECT_SOURCE_DIR}/src/private/dshadownode_p.cpp    ${PROJECT_SOURCE_DIR}/src/private/dshadownode_p.h
    ${PROJECT_SOURCE_DIR}/src/private/dsoftwareeffectrendernode.cpp    ${PROJECT_SOURCE_DIR}/src/private/dsoftwareeffectrendernode.h
    ${PROJECT_SOURCE_DIR}/src/private/dsoftwareroundedimagenode_p.h    ${PROJECT_SOURCE_DIR}/src/private/dsoftwareroundedimagenode.cpp
    ${PROJECT_SOURCE_DIR}/src/private/dapploader_p.h    ${PROJECT_SOURCE_DIR}/src/dapploader.cpp    ${PROJECT_SOURCE_DIR}/src/dapploader.h
    ${PROJECT_SOURCE_DIR}/src/private/dplatformthemeproxy_p.h    ${PROJECT_SOURCE_DIR}/src/dplatformthemeproxy.cpp    ${PROJECT_SOURCE_DIR}/src/dplatformthemeproxy.h
    ${PROJECT_SOURCE_DIR}/src/dqmlappmainwindowinterface.cpp    ${PROJECT_SOURCE_DIR}/src/dqmlappmainwindowinterface.h
    ${PROJECT_SOURCE_DIR}/src/dqmlapppreloadinterface.cpp    ${PROJECT_SOURCE_DIR}/src/dqmlapppreloadinterface.h
    ${PROJECT_SOURCE_DIR}/src/dquickblitframebuffer.cpp    ${PROJECT_SOURCE_DIR}/src/dquickblitframebuffer.h
    ${PROJECT_SOURCE_DIR}/src/dquickitemviewport.cpp    ${PROJECT_SOURCE_DIR}/src/dquickitemviewport.h
    ${PROJECT_SOURCE_DIR}/src/private/dquicksystempalette_p.h    ${PROJECT_SOURCE_DIR}/src/dquicksystempalette.cpp    ${PROJECT_SOURCE_DIR}/src/dquicksystempalette.h
    ${PROJECT_SOURCE_DIR}/src/private/dquickwindow_p.h    ${PROJECT_SOURCE_DIR}/src/dquickwindow.cpp    ${PROJECT_SOURCE_DIR}/src/dquickwindow.h
    ${PROJECT_SOURCE_DIR}/src/private/dquickglobal_p.h
    ${PROJECT_SOURCE_DIR}/src/dtkdeclarative_config.h
    ${PROJECT_SOURCE_DIR}/src/dtkdeclarative_global.h
)

set(QML_DTK_CONTROLS
    "qml/SearchEdit.qml"
    "qml/LineEdit.qml"
    "qml/ControlBackground.qml"
    "qml/PasswordEdit.qml"
    "qml/ButtonBox.qml"
    "qml/FloatingButton.qml"
    "qml/IconButton.qml"
    "qml/RecommandButton.qml"
    "qml/ToolButton.qml"
    "qml/WarningButton.qml"
    "qml/FocusBoxBorder.qml"
    "qml/CicleSpreadAnimation.qml"
    "qml/TitleBar.qml"
    "qml/DialogTitleBar.qml"
    "qml/WindowButton.qml"
    "qml/WindowButtonGroup.qml"
    "qml/DialogWindow.qml"
    "qml/AboutDialog.qml"
    "qml/HelpAction.qml"
    "qml/AboutAction.qml"
    "qml/QuitAction.qml"
    "qml/ThemeMenu.qml"
    "qml/RectangularShadow.qml"
    "qml/AlertToolTip.qml"
    "qml/FlowStyle.qml"
    "qml/Button.qml"
    "qml/style/Style.qml"
    "qml/Control.qml"
    "qml/PlusMinusSpinBox.qml"
    "qml/EditPanel.qml"
    "qml/TextArea.qml"
    "qml/TextField.qml"
    "qml/SpinBox.qml"
    "qml/SpinBoxIndicator.qml"
    "qml/ApplicationWindow.qml"
    "qml/BoxShadow.qml"
    "qml/FloatingPanel.qml"
    "qml/FloatingMessage.qml"
    "qml/Menu.qml"
    "qml/MenuItem.qml"
    "qml/MenuSeparator.qml"
    "qml/HighlightPanel.qml"
    "qml/ArrowListView.qml"
    "qml/ScrollIndicator.qml"
    "qml/StyledBehindWindowBlur.qml"
    "qml/Popup.qml"
    "qml/PopupWindow.qml"
    "qml/Switch.qml"
    "qml/Slider.qml"
    "qml/SliderHandle.qml"
    "qml/ScrollBar.qml"
    "qml/ComboBox.qml"
    "qml/ActionButton.qml"
    "qml/ItemDelegate.qml"
    "qml/CheckDelegate.qml"
    "qml/TipsSlider.qml"
    "qml/SliderTipItem.qml"
    "qml/BoxInsetShadow.qml"
    "qml/CheckBox.qml"
    "qml/IpV4LineEdit.qml"
    "qml/OutsideBoxBorder.qml"
    "qml/InsideBoxBorder.qml"
    "qml/SortFilterModel.qml"
    "qml/ArrowShapePopupWindow.qml"
    "qml/StyledArrowShapeWindow.qml"
    "qml/ArrowShapePopup.qml"
    "qml/Action.qml"
    "qml/ActionGroup.qml"
    "qml/Label.qml"
    "qml/Container.qml"
    "qml/ButtonGroup.qml"
    "qml/ScrollView.qml"
    "qml/MenuBar.qml"
    "qml/AbstractButton.qml"
    "qml/TabBar.qml"
    "qml/SwipeDelegate.qml"
    "qml/SwipeView.qml"
    "qml/Drawer.qml"
    "qml/Dial.qml"
    "qml/Dialog.qml"
    "qml/DelayButton.qml"
    "qml/RoundButton.qml"
    "qml/RadioButton.qml"
    "qml/ToolTip.qml"
    "qml/DialogButtonBox.qml"
    "qml/Frame.qml"
    "qml/GroupBox.qml"
    "qml/PageIndicator.qml"
    "qml/StackView.qml"
    "qml/BusyIndicator.qml"
    "qml/ProgressBar.qml"
    "qml/BoxPanel.qml"
    "qml/Pane.qml"
    "qml/KeySequenceEdit.qml"
    "qml/private/KeySequenceLabel.qml"
    "qml/ButtonIndicator.qml"
    "qml/EmbeddedProgressBar.qml"
    "qml/WaterProgressBar.qml"
    "qml/PlaceholderText.qml"
)

set_source_files_properties(qml/style/Style.qml
    PROPERTIES
        QT_QML_SINGLETON_TYPE true
)

set(DTK_SHARERS_NG
    "shaders_ng/quickitemviewport.vert"
    "shaders_ng/quickitemviewport.frag"
    "shaders_ng/quickitemviewport-opaque.vert"
    "shaders_ng/quickitemviewport-opaque.frag"
    "shaders_ng/cornerscolorshader.vert"
    "shaders_ng/cornerscolorshader.frag"
    "shaders_ng/shadowmaterial.vert"
    "shaders_ng/shadowmaterial.frag"
)
