#include "MainFrame.h"

MyFrame::MyFrame(wxWindow *parent, 
                    wxWindowID id, 
                    const wxString& title, 
                    const wxPoint& pos, 
                    const wxSize& size, 
                    long style) : wxFrame(parent, id, title, pos, size, style) {
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(1, "&Hello...\tCtrl-H", "Help string shown in status bar for this menu item");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);

    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");
    SetMenuBar( menuBar );

    CreateStatusBar();

    SetStatusText("Welcome to wxWidgets!");

    sizer = new wxBoxSizer(wxHORIZONTAL);

    imagePanel  = new ImagePanel(this);// this, wxT("image.jpg"), wxBITMAP_TYPE_JPEG);
    editPanel   = new EditPanel(this);
    editPanel->Bind(wxEVT_SLIDER, &MyFrame::invokeBrightnessChange, this, 10000);
#ifdef DEBUG_EDIT
    editPanel->Bind(wxEVT_GRID_CELL_CHANGED, &MyFrame::invokeDCTChange, this, 9999);
#endif

    sizer->Add(imagePanel, 1, wxEXPAND);

    sizer->Add(
        editPanel,
        0,              // make horizontally unstretchable
        wxEXPAND,       // make border all around (implicit top alignment)
        10);            // set border width to 10
    
    
    this->SetSizer(sizer);

    Bind(wxEVT_MENU, &MyFrame::OnHello, this, 1);
    Bind(wxEVT_MENU, &MyFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MyFrame::OnExit, this, wxID_EXIT);
}

void MyFrame::OnExit(wxCommandEvent& event) {
    Close(true);
}

void MyFrame::OnAbout(wxCommandEvent& event) {
    wxMessageBox("This is a wxWidgets Hello World example", "About Hello World", wxOK | wxICON_INFORMATION);
}

void MyFrame::OnHello(wxCommandEvent& event) {
    wxFileDialog openFileDialog(this,
                                _("Open image file"),
                                "",
                                "",
                                "Image files (*.jpeg;*.jpg)|*.jpeg;*.jpg",
                                wxFD_OPEN|wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;

    if (!imagePanel->loadImage(openFileDialog.GetPath(), wxBITMAP_TYPE_JPEG)) {
        wxString error = "Failed to load image:\t" + openFileDialog.GetPath();
        SetStatusText(_(error));
    }
    else
        SetStatusText("");    
}


/* Image changes */
void MyFrame::invokeBrightnessChange(wxCommandEvent &event) {
    imagePanel->setBrightness(editPanel->getBrightness());
}

#ifdef DEBUG_EDIT
void MyFrame::invokeDCTChange(wxGridEvent& event) {
    editPanel->rawDCTvaluesGridChanged();
    imagePanel->setDCT(editPanel->getDCTValues());
    imagePanel->setBrightness(editPanel->getBrightness());
}
#endif