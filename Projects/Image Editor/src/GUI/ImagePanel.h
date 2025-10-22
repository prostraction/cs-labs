#include <wx/wx.h>
#include <wx/sizer.h>

#include "../Core/Image.h"

class ImagePanel : public wxPanel {  
public:
    ImagePanel(wxFrame* parent);
    ImagePanel(wxFrame* parent, 
                    wxString file, 
                    wxBitmapType format);

    bool loadImage(wxString file, 
                     wxBitmapType format);
    
    void paintEvent(wxPaintEvent & evt);
    void paintNow();   
    void render(wxDC& dc);

    void setBrightness(const int& value);
    void setDCT(const int* DCT_Matrix);

    void onSize(wxSizeEvent& event);
    void maxSize(wxMaximizeEvent& event);
    
    // some useful events
    /*
     void mouseMoved(wxMouseEvent& event);
     void mouseDown(wxMouseEvent& event);
     void mouseWheelMoved(wxMouseEvent& event);
     void mouseReleased(wxMouseEvent& event);
     void rightClick(wxMouseEvent& event);
     void mouseLeftWindow(wxMouseEvent& event);
     void keyPressed(wxKeyEvent& event);
     void keyReleased(wxKeyEvent& event);
     */
    
    DECLARE_EVENT_TABLE()
private:
    Image* img;

    wxImage     image;
    wxBitmap    resized;
    int         w, h;
    bool        pictureLoaded;
    float       ratio;
    float       zoom;
};