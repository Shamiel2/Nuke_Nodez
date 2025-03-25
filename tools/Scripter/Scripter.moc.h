// #ifndef HAVE_MYWIDGET_MOC_H
// #define HAVE_MYWIDGET_MOC_H
#include "DDImage/Knobs.h"
#include <QObject>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout> // Include QVBoxLayout
#include <QWidget> // Include QWidget


class PythonKknob;
class ReloadKnob;

/// QLISTWIDGET That displays the channels
class PythonInter : public QPlainTextEdit
{
  Q_OBJECT

  public:
    PythonInter(PythonKknob* knob);
    ~PythonInter();

    void update();
    void destroy();

    static int WidgetCallback(void* closure, DD::Image::Knob::CallbackReason reason);

  public Q_SLOTS:
    void valueChanged(int value);

  private:
    PythonKknob* _knob;
};

/// QLISTWIDGET That displays the channels
class ForceReload : public QPushButton
{
  Q_OBJECT

  public:
    ForceReload(ReloadKnob* knob);
    ~ForceReload();

    void update();
    void destroy();

    static int WidgetCallback(void* closure, DD::Image::Knob::CallbackReason reason);

  public Q_SLOTS:
    void valueChanged(int value);

  private:
    ReloadKnob* _knob;
};


