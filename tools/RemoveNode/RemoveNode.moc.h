// #ifndef HAVE_MYWIDGET_MOC_H
// #define HAVE_MYWIDGET_MOC_H
#include "DDImage/Knobs.h"
#include <QObject>
#include <QListWidget>
#include <QPushButton>
#include <QCheckBox>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout> // Include QVBoxLayout
#include <QWidget> // Include QWidget


class CustomChannelKnob;
class KeepAllKnob;
class RemoveAllKnob;
class FilterKnob;

/// QLISTWIDGET That displays the channels
class ChannelsLists : public QListWidget
{
  Q_OBJECT

  public:
    ChannelsLists(CustomChannelKnob* knob);
    ~ChannelsLists();

    void update();
    void destroy();

    static int WidgetCallback(void* closure, DD::Image::Knob::CallbackReason reason);

  public Q_SLOTS:
    void valueChanged(int value);

  private:
    CustomChannelKnob* _knob;
};



/// CHECK ALL BUTTON
class KeepAllButton : public QPushButton
{
  Q_OBJECT

  public:
    KeepAllButton(KeepAllKnob* knob);
    ~KeepAllButton();

    void update();
    void destroy();
    static int WidgetCallback(void* closure, DD::Image::Knob::CallbackReason reason);

  public Q_SLOTS:
    void valueChanged(int value);

  private:
    KeepAllKnob* _knob;
};


/// UNCHECK ALL BUTTON
class RemoveAllButton : public QPushButton
{
  Q_OBJECT

  public:
    RemoveAllButton(RemoveAllKnob* knob);
    ~RemoveAllButton();

    void update();
    void destroy();
    static int WidgetCallback(void* closure, DD::Image::Knob::CallbackReason reason);

  public Q_SLOTS:
    void valueChanged(int value);

  private:
    RemoveAllKnob* _knob;
};



/// UNCHECK ALL BUTTON
class FilterPanel : public QLineEdit
{
  Q_OBJECT

  public:
    FilterPanel(FilterKnob* knob);
    ~FilterPanel();

    void update();
    void destroy();
    static int WidgetCallback(void* closure, DD::Image::Knob::CallbackReason reason);

  public Q_SLOTS:
    void valueChanged(int value);

  private:
    FilterKnob* _knob;
};

