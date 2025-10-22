#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "MainFrame.h"

class MyApp : public wxApp  {
public:
    virtual bool OnInit();
private:
    wxFrame*    frame;
}; 