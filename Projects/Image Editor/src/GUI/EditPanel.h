#include <wx/wx.h>
#include <wx/grid.h>
#include <wx/sizer.h>

#define DEBUG_EDIT

class EditPanel : public wxPanel {
public: 
    EditPanel(wxFrame* parent);
    int getBrightness() {return brightnessValue;}

#ifdef DEBUG_EDIT
    int* getDCTValues() {return rawDCTvalues;}
    void rawDCTvaluesGridChanged();
#endif 

private:
    wxBoxSizer*     verticalSizer;

#ifdef DEBUG_EDIT
    wxCheckBox*     rawDCTenabled;
    int*            rawDCTvalues;
    wxGrid*         rawDCTvaluesGrid;
#endif

    wxStaticText*   brightnessName;
    wxSlider*       brightnessSlider;
    wxStaticText*   brightnessValueDisplay;
    wxBoxSizer*     brightnessSizer;
    int             brightnessValue;


    void onSize(wxSizeEvent& event);
    void maxSize(wxMaximizeEvent& event);
    void displaySliderChanged(wxScrollEvent &event);

    DECLARE_EVENT_TABLE()
};