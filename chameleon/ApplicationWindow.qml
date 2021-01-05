import QtQuick.Templates 2.4 as T
import com.deepin.dtk 1.0

T.ApplicationWindow {
    palette: active ? DTK.palette : DTK.inactivePalette
}
