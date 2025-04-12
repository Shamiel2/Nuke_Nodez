#include "RemoveNode.moc.h"

#include "DDImage/NukeWrapper.h"
#include "DDImage/PixelIop.h"
#include "DDImage/Row.h"
#include "DDImage/Knobs.h"

#include <QTabWidget>
#include <QCheckBox>
#include <string>
#include <QObject>


#include <sstream>

using namespace DD::Image;
static const char* const HELP = "Adds a constant to a set of channels";

int btn_min_width = 150;
int btn_min_height = 25;




QVector<QPair<QCheckBox*, bool>> findAllCheckBoxesWithState(ChannelsLists* channelsList) {
  QVector<QPair<QCheckBox*, bool>> checkBoxesWithState;

  if (channelsList) {
      // Access the viewport widget
      QWidget* viewportWidget = channelsList->viewport();

      // Get the layout from the viewport widget
      QLayout* layout = viewportWidget->layout();

      if (layout) {
          // Iterate through layout items
          for (int i = 0; i < layout->count(); ++i) {
              QLayoutItem* item = layout->itemAt(i);
              if (item) {
                  QWidget* widget = item->widget();
                  if (widget) {
                      // Check if the widget is a QCheckBox
                      QCheckBox* checkBox = qobject_cast<QCheckBox*>(widget);
                      if (checkBox) {
                          // Store the QCheckBox pointer and its checked state
                          checkBoxesWithState.append(qMakePair(checkBox, checkBox->isChecked()));
                      }
                  }
              }
          }
      }
  }

  return checkBoxesWithState;
}




class CustomChannelKnob : public DD::Image::Knob
{
  friend class ChannelsLists;
  int _data;
  ChannelsLists* _widget; // Add a member variable to store the ChannelsLists widget

public:
  CustomChannelKnob(DD::Image::Knob_Closure* kc, int* data, const char* n) : Knob(kc, n), _widget(nullptr)
  {
    _data = 0;
    if (data)
      _data = *data;
  }

  virtual const char* Class() const { return "CustomChannelKnob"; }
  virtual bool not_default() const { return _data != 0; }

  // This adds the custom knob to the Nuke panel
  virtual WidgetPointer make_widget(const DD::Image::WidgetContext& context)
  {
    _widget = new ChannelsLists(this); // Store the created ChannelsLists instance
    return _widget;
  }

  // Getter for the ChannelsLists widget
  ChannelsLists* widget() const { return _widget; }
};


ChannelsLists::ChannelsLists(CustomChannelKnob* knob) : _knob(knob)
{

  _knob->addCallback(WidgetCallback, this);
  connect(this, SIGNAL(valueChanged(int)), this, SLOT(valueChanged(int)));

  // // Create a QVBoxLayout
  QVBoxLayout* layout = new QVBoxLayout();
  // Create a QVBoxLayout
  QCheckBox* checkbox = new QCheckBox();
  checkbox->setText("Channel Name");

  QCheckBox* checkbox2 = new QCheckBox();
  checkbox2->setText("Test Name");



  // // Create a widget that will be the viewport
  QWidget* viewportWidget = new QWidget();

  layout->addWidget(checkbox);
  layout->addWidget(checkbox2);

  // std::cout << "Testing Channellist: " << std::endl;

  // // Set the layout for the viewport
  viewportWidget->setLayout(layout);

  // // Set the viewport of the QListWidget
  setViewport(viewportWidget);
}

ChannelsLists::~ChannelsLists()
{
  if ( _knob )
    _knob->removeCallback( WidgetCallback, this );
}

void ChannelsLists::valueChanged(int value)
{
  std::cerr << "valueChanged" << std::endl;
  // _knob->setValue( value );
}




int ChannelsLists::WidgetCallback(void* closure, Knob::CallbackReason reason)
{
  // could double check if on main thread here just in case and bail out
  // if not
  ChannelsLists* widget = (ChannelsLists*)closure;
  assert(widget);
  switch (reason) {
    case Knob::kIsVisible:
      {
        // We check for visibility up to the containing tab widget.
        // This means that a widget is still considered visible when its NodePanel is hidden due to being in hidden tab in a dock.

        for (QWidget* w = widget->parentWidget(); w; w = w->parentWidget())
          if (qobject_cast<QTabWidget*>(w))
            return widget->isVisibleTo(w);
        return widget->isVisible();
      }

    std::cerr << "Channel List Validate" << std::endl;

    // case Knob::kUpdateWidgets:
    //   widget->update();
    //   return 0;

    // case Knob::kDestroying:
    //   widget->destroy();
    //   return 0;

    default:
      return 0;
  }
}








//////////////////////////////////////////////////////////////////////////////////////
//                      CHECK ALL BUTTON                                            //
//////////////////////////////////////////////////////////////////////////////////////
class KeepAllKnob : public DD::Image::Knob
{
  friend class KeepAllButton;
  int _data;
public:
  KeepAllKnob(DD::Image::Knob_Closure* kc, int* data, const char* n) : Knob(kc, n)
  {
    _data = 0;
    if ( data )
      _data = *data;
  }

  virtual const char* Class() const { return "KeepAllKnob"; }
  virtual bool not_default () const { return _data != 0; }


  // THis adds the custom knob to the nuke panel
  virtual WidgetPointer make_widget(const DD::Image::WidgetContext& context)
  {
    KeepAllButton* widget = new KeepAllButton( this );
    return widget;
  }
};


KeepAllButton::KeepAllButton(KeepAllKnob* knob) : _knob(knob)
{
    _knob->addCallback(WidgetCallback, this);
    setText("Keep All");
    setToolTip("Enables all the channels in the channels list");
    setMinimumSize(btn_min_width, btn_min_height);
    connect(this, &QPushButton::clicked, [this]() {
        std::cerr << "KeepAllButton pressed!" << std::endl;

        // Find the NodePanel widget
        QWidget* nodePanel = nullptr;
        for (QWidget* w = parentWidget(); w; w = w->parentWidget()) {
            if (w->objectName() == "NodePanel_RemoveNode1") {
                nodePanel = w;
                break;
            }
        }

        if (nodePanel) {
            // Find the ChannelsLists widget within the NodePanel
            ChannelsLists* channelsList = nodePanel->findChild<ChannelsLists*>();

            if (channelsList) {
                // Find all QCheckBoxes with their state
                QVector<QPair<QCheckBox*, bool>> checkBoxesWithState = findAllCheckBoxesWithState(channelsList);

                // Output the number of QCheckBoxes found
                std::cout << "Found " << checkBoxesWithState.size() << " QCheckBoxes." << std::endl;

                checkBoxesWithState[0].first->setChecked(true);

                // ... (rest of your code) ...
            } else {
                std::cout << "ChannelsLists instance not found within NodePanel!" << std::endl;
            }
        } else {
            std::cout << "NodePanel not found!" << std::endl;
        }
    });
}



KeepAllButton::~KeepAllButton()
{
  if ( _knob )
    _knob->removeCallback( WidgetCallback, this );
}

void KeepAllButton::valueChanged(int value)
{
  std::cerr << "valueChanged" << std::endl;
  // _knob->setValue( value );
}


int KeepAllButton::WidgetCallback(void* closure, Knob::CallbackReason reason)
{
  // could double check if on main thread here just in case and bail out
  // if not
  KeepAllButton* widget = (KeepAllButton*)closure;
  assert(widget);
  switch (reason) {
    case Knob::kIsVisible:
      {
        // We check for visibility up to the containing tab widget.
        // This means that a widget is still considered visible when its NodePanel is hidden due to being in hidden tab in a dock.

        for (QWidget* w = widget->parentWidget(); w; w = w->parentWidget())
          if (qobject_cast<QTabWidget*>(w))
            return widget->isVisibleTo(w);
        return widget->isVisible();
      }

    // case Knob::kUpdateWidgets:
    //   widget->update();
    //   return 0;

    // case Knob::kDestroying:
    //   widget->destroy();
    //   return 0;

    default:
      return 0;
  }
}











//////////////////////////////////////////////////////////////////////////////////////
//                      UNCHECK ALL BUTTON                                          //
//////////////////////////////////////////////////////////////////////////////////////
class RemoveAllKnob : public DD::Image::Knob
{
  friend class RemoveAllButton;
  int _data;
public:
  RemoveAllKnob(DD::Image::Knob_Closure* kc, int* data, const char* n) : Knob(kc, n)
  {
    _data = 0;
    if ( data )
      _data = *data;
  }

  virtual const char* Class() const { return "RemoveAllKnob"; }
  virtual bool not_default () const { return _data != 0; }

  // THis adds the custom knob to the nuke panel
  virtual WidgetPointer make_widget(const DD::Image::WidgetContext& context)
  {
    RemoveAllButton* widget = new RemoveAllButton( this );
    return widget;
  }
};


RemoveAllButton::RemoveAllButton(RemoveAllKnob* knob) : _knob(knob)
{
  // Sets all the Knob Properties and or Flags
  _knob->addCallback(WidgetCallback, this);
  setText("Remove All");
  setToolTip("Unchecks all the channels in the channels list");
  setMinimumSize(btn_min_width, btn_min_height);
  // Connect the clicked signal to a lambda function
  connect(this, &QPushButton::clicked, [this]() {
    std::cerr << "RemoveAllButton pressed!" << std::endl;

    // Find the NodePanel widget
    QWidget* nodePanel = nullptr;
    for (QWidget* w = parentWidget(); w; w = w->parentWidget()) {
        if (w->objectName() == "NodePanel_RemoveNode1") {
            nodePanel = w;
            break;
        }
    }

    if (nodePanel) {
        // Find the ChannelsLists widget within the NodePanel
        ChannelsLists* channelsList = nodePanel->findChild<ChannelsLists*>();

        if (channelsList) {
            // Find all QCheckBoxes with their state
            QVector<QPair<QCheckBox*, bool>> checkBoxesWithState = findAllCheckBoxesWithState(channelsList);

            // Output the number of QCheckBoxes found
            std::cout << "Found " << checkBoxesWithState.size() << " QCheckBoxes." << std::endl;

            checkBoxesWithState[0].first->setChecked(false);

            // ... (rest of your code) ...
        } else {
            std::cout << "ChannelsLists instance not found within NodePanel!" << std::endl;
        }
    } else {
        std::cout << "NodePanel not found!" << std::endl;
    }
});
}

RemoveAllButton::~RemoveAllButton()
{
  if ( _knob )
    _knob->removeCallback( WidgetCallback, this );
}

void RemoveAllButton::valueChanged(int value)
{
  std::cout << "valueChanged" << std::endl;
  // _knob->setValue( value );
}



int RemoveAllButton::WidgetCallback(void* closure, Knob::CallbackReason reason)
{
  // could double check if on main thread here just in case and bail out
  // if not
  RemoveAllButton* widget = (RemoveAllButton*)closure;
  assert(widget);
  switch (reason) {
    case Knob::kIsVisible:
      {
        // We check for visibility up to the containing tab widget.
        // This means that a widget is still considered visible when its NodePanel is hidden due to being in hidden tab in a dock.

        for (QWidget* w = widget->parentWidget(); w; w = w->parentWidget())
          if (qobject_cast<QTabWidget*>(w))
            return widget->isVisibleTo(w);
        return widget->isVisible();
      }

    // case Knob::kUpdateWidgets:
    //   widget->update();
    //   return 0;

    // case Knob::kDestroying:
    //   widget->destroy();
    //   return 0;

    default:
      return 0;
  }
}






/// @brief ///////////////////////////////////////////
/// MAIN NUKE NODE CLASS FOR EVERYTHING NUKE RELATED

class RemoveNode : public PixelIop
{
  int _value;

  public:
  // void in_channels(int input, ChannelSet& mask) const;
  RemoveNode(Node* node) : PixelIop(node)
  {
    _value = 0;
    // _channelsList = nullptr;

  }

  // bool pass_transform() const { return true; }
  void pixel_engine(const Row &in, int y, int x, int r, ChannelMask, Row & out);
  virtual void knobs(Knob_Callback);
  static const Iop::Description d;
  const char* Class() const { return d.name; }
  const char* node_help() const { return HELP; }
  void _validate(bool);
  void printUncheckedCheckboxes(QLayout* layout);
};

void RemoveNode::knobs(Knob_Callback f)
{
  // a custom knob with custom data
  CustomKnob1(KeepAllKnob, f, &_value, "check_all_button");
  CustomKnob1(RemoveAllKnob, f, &_value, "uncheck_all_button");
  ClearFlags(f, Knob::STARTLINE);
  CustomKnob1(CustomChannelKnob, f, &_value, "channels_list"); // Store the pointer to ChannelsLists
}


void RemoveNode::_validate(bool for_real)
{

  // Retrieve the CustomChannelKnob instance
  CustomChannelKnob* customKnob = dynamic_cast<CustomChannelKnob*>(knob("channels_list"));
  if (!customKnob) {
    std::cerr << "CustomChannelKnob not found!" << std::endl;
    return;
  }

  // Access the ChannelsLists widget associated with the CustomChannelKnob
  ChannelsLists* channelsList = customKnob->widget(); // Use the new widget() method
  if (!channelsList) {
    std::cerr << "ChannelsLists widget not found!" << std::endl;
    return;
  }

  // Access the layout of the ChannelsLists widget
  QWidget* viewportWidget = channelsList->viewport();
  QLayout* layout = viewportWidget->layout();
  if (!layout) {
    std::cerr << "Layout not found in ChannelsLists viewport!" << std::endl;
    return;
  }

  // Clear the layout by removing all widgets
  while (QLayoutItem* item = layout->takeAt(0)) {
    if (QWidget* widget = item->widget()) {
        widget->deleteLater(); // Schedule the widget for deletion
    }
    delete item; // Delete the layout item
  }

  // Iterate through the input channels and add a QCheckBox for each channel
  ChannelSet inputChannels = input0().channels();
  for (Channel channel : inputChannels) {
    QCheckBox* newCheckBox = new QCheckBox("Test Name");
    layout->addWidget(newCheckBox);
  }

  std::cout << "Added QCheckBox widgets for all input channels." << std::endl;

  // Perform other validation tasks
  set_out_channels(input0().channels());
  copy_info();
}


void RemoveNode::pixel_engine(const Row& in, int y, int x, int r, ChannelMask channels, Row& out){
}




static Iop* build(Node* node) { return new RemoveNode(node); }
const Iop::Description RemoveNode::d("RemoveNode", "Color/Math/RemoveNode", build);
