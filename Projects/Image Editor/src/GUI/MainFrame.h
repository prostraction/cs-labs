#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "ImagePanel.h"
#include "EditPanel.h"

class MyFrame : public wxFrame {
public:
    MyFrame(wxWindow *parent, 
            wxWindowID id, 
            const wxString& title, 
            const wxPoint& pos, 
            const wxSize& size, 
            long style);
private:
    void OnHello(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

    void invokeBrightnessChange(wxCommandEvent &event);

#ifdef DEBUG_EDIT
    void invokeDCTChange(wxGridEvent& event);
#endif


    enum {
        ID_Hello = 1
    };

    ImagePanel* imagePanel;
    EditPanel*  editPanel;

    wxBoxSizer* sizer;
};