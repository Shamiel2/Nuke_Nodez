#include "Scripter.moc.h"

#include "DDImage/NukeWrapper.h"
#include "DDImage/PixelIop.h"
#include "DDImage/Row.h"
#include "DDImage/Knobs.h"

#include <QTabWidget>
#include <QCheckBox>
#include <string>


#include <sstream>

using namespace DD::Image;
static const char* const HELP = "Adds a constant to a set of channels";



class PythonKknob : public DD::Image::Knob
{
  friend class PythonInter;
  int _data;
public:
  PythonKknob(DD::Image::Knob_Closure* kc, int* data, const char* n) : Knob(kc, n)
  {
    _data = 0;
    if ( data )
      _data = *data;
  }

  virtual const char* Class() const { return "PythonKknob"; }
  virtual bool not_default () const { return _data != 0; }

  // THis adds the custom knob to the nuke panel
  virtual WidgetPointer make_widget(const DD::Image::WidgetContext& context)
  {
    PythonInter* widget = new PythonInter( this );
    return widget;
  }
};


PythonInter::PythonInter(PythonKknob* knob) : _knob(knob)
{

  _knob->addCallback(WidgetCallback, this);
  connect(this, SIGNAL(valueChanged(int)), this, SLOT(valueChanged(int)));

}

PythonInter::~PythonInter()
{
  if ( _knob )
    _knob->removeCallback( WidgetCallback, this );
}

void PythonInter::valueChanged(int value)
{
  std::cerr << "valueChanged" << std::endl;
  // _knob->setValue( value );
}




int PythonInter::WidgetCallback(void* closure, Knob::CallbackReason reason)
{
  // could double check if on main thread here just in case and bail out
  // if not
  PythonInter* widget = (PythonInter*)closure;
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






/// @brief ///////////////////////////////////////////
/// MAIN NUKE NODE CLASS FOR EVERYTHING NUKE RELATED

class Scripter : public PixelIop
{
  int _value;

  public:
  // void in_channels(int input, ChannelSet& mask) const;
  Scripter(Node* node) : PixelIop(node)
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
};



void Scripter::_validate(bool for_real)
{

  std::cout << "SCRIPTER VALIDATE" << knob("scripter")->value() << std::endl;
  set_out_channels(input0().channels());
  copy_info();

}

void Scripter::pixel_engine(const Row& in, int y, int x, int r, ChannelMask channels, Row& out){}

void Scripter::knobs(Knob_Callback f)
{
  // a custom knob with custom data
  CustomKnob1(PythonKknob, f, &_value, "scripter"); // Store the pointer to PythonInter
}


static Iop* build(Node* node) { return new Scripter(node); }
const Iop::Description Scripter::d("Scripter", "Color/Math/Scripter", build);
