//! [0]
class Rectangle : public QWidget
{
protected:
    void paintEvent(QPaintEvent *) override {
        QPainter pa(this);
        pa.fillRect(0, 0, 100, 100, Qt::red);
    }
};
//! [0]
//! [1]
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QWidget mainWidget;
    mainWidget.resize(200, 200);
    QHBoxLayout layout1(&mainWidget);
    QPushButton btn1("Button 1");
    layout1.addWidget(&btn1);
    QVBoxLayout layout2;
    QPushButton btn2("Button 2");
    QLineEdit lineEdit;
    layout2.addWidget(&btn2);
    layout2.addWidget(&lineEdit);
    layout1.addLayout(&layout2);
    mainWidget.show();

    return app.exec();
}
//! [1]
//! [2]
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QWidget widget;
    widget.resize(300, 300);
    QPushButton btn("Button", &widget);
    btn.move(50, 50);
    QObject::connect(&btn, &QPushButton::clicked, []() {
        qDebug() << "Button Clicked...";
    });
    widget.show();
}
//! [2]
