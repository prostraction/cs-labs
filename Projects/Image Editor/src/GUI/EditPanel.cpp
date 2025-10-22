#include "EditPanel.h"


BEGIN_EVENT_TABLE(EditPanel, wxPanel)
    EVT_COMMAND_SCROLL(10000, EditPanel::displaySliderChanged)
    EVT_SIZE(EditPanel::onSize)
    EVT_MAXIMIZE(EditPanel::maxSize)
END_EVENT_TABLE()

EditPanel::EditPanel(wxFrame* parent) : wxPanel(parent) { 
    verticalSizer = new wxBoxSizer(wxVERTICAL);

#ifdef DEBUG_EDIT
    rawDCTvalues = new int[64];
    memset(rawDCTvalues, 0, 64 * sizeof(int));
    FILE* f = fopen("DCT_DEBUG", "r");
    if (f) {
        char buffer[5];
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                fscanf(f, "%s", buffer);
                rawDCTvalues[j + i * 8] = atoi(buffer);
            }
        }
        fclose(f);
    }
    rawDCTvaluesGrid = new wxGrid(this, 9999);
    rawDCTvaluesGrid->CreateGrid(8, 8);
    rawDCTvaluesGrid->HideColLabels();
    rawDCTvaluesGrid->HideRowLabels();
    for (int i = 0; i < 8; i++) {
        rawDCTvaluesGrid->SetColSize(i, 50);
        for (int j = 0; j < 8; j++) {
            rawDCTvaluesGrid->SetCellValue(i, j, wxString::Format(wxT("%i"), rawDCTvalues[j + i * 8]));
            rawDCTvaluesGrid->SetCellAlignment(i, j, wxALIGN_CENTRE, wxALIGN_CENTRE);
        }
    }
    verticalSizer->Add(rawDCTvaluesGrid, 0, wxALL, 20);
#endif

    brightnessValue = 0;
    brightnessSizer         = new wxBoxSizer(wxHORIZONTAL);
    brightnessName          = new wxStaticText(this, wxID_ANY, "Brightness: ");
    brightnessSlider        = new wxSlider(this, 10000, 0, -100, 100, wxDefaultPosition, wxSize(290, 30));
    brightnessValueDisplay  = new wxStaticText(this, wxID_ANY, wxString::Format(wxT("%d"), brightnessValue)); 
    brightnessSizer->Add(brightnessName,            0, wxALL | wxALIGN_CENTER_VERTICAL, 10);
    brightnessSizer->Add(brightnessSlider,          0, wxALL | wxALIGN_CENTER_VERTICAL, 0);
    brightnessSizer->Add(brightnessValueDisplay,    0, wxALL | wxALIGN_CENTER_VERTICAL, 20);
    verticalSizer->Add(brightnessSizer, 0, wxEXPAND);


    this->SetSizer(verticalSizer);
}

void EditPanel::onSize(wxSizeEvent& event) {
    event.Skip();
}

void EditPanel::maxSize(wxMaximizeEvent& event) {
    event.Skip();
}

void EditPanel::displaySliderChanged(wxScrollEvent &event) {
    brightnessValue = brightnessSlider->GetValue();
    brightnessValueDisplay->SetLabel(wxString::Format(wxT("%d"), brightnessValue));
    event.Skip();
}

#ifdef DEBUG_EDIT
void EditPanel::rawDCTvaluesGridChanged() {
    FILE* f = fopen("DCT_DEBUG", "w");
    if (!f)
        f = stderr;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            rawDCTvalues[j + i * 8] = wxAtoi(rawDCTvaluesGrid->GetCellValue(i, j));
            fprintf(f, "%d ", wxAtoi(rawDCTvaluesGrid->GetCellValue(i, j)));
        }
        fprintf(f, "\n");
    }
    fclose(f);
}
#endif