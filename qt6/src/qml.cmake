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
    "qml/ButtonIndicator.qml"
    "qml/EmbeddedProgressBar.qml"
    "qml/WaterProgressBar.qml"
    "qml/PlaceholderText.qml"
    "qml/ControlGroup.qml"
    "qml/ControlGroupItem.qml"
)

foreach(QML_FILE ${QML_DTK_CONTROLS})
    get_filename_component(file_name ${QML_FILE} NAME)
    set_source_files_properties(${QML_FILE}
        PROPERTIES QT_RESOURCE_ALIAS ${file_name}
    )
endforeach()
