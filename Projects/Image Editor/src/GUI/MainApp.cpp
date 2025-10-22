#include "MainApp.h"

bool MyApp::OnInit() {
    wxInitAllImageHandlers();
    frame = new MyFrame(NULL, wxID_ANY, wxT("Hello wxDC"), wxPoint(50,50), wxSize(1280,720), wxDEFAULT_FRAME_STYLE);  
    frame->Show();
    return true;
}