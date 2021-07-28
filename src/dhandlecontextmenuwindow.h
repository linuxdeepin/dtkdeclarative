#ifndef DHANDLECONTEXTMENUWINDOW_H
#define DHANDLECONTEXTMENUWINDOW_H

#include <QQuickWindow>

class DHandleContextMenuWindow : public QQuickWindow
{
public:
    DHandleContextMenuWindow(QWindow *parent = nullptr);

protected:
    bool event(QEvent *) override;
    void handleMouseEvent(QMouseEvent *);
};

#endif // DHANDLECONTEXTMENUWINDOW_H
