#include "ImagePanel.h"


BEGIN_EVENT_TABLE(ImagePanel, wxPanel)
    // some useful events
    /*
    EVT_MOTION(ImagePanel::mouseMoved)
    EVT_LEFT_DOWN(ImagePanel::mouseDown)
    EVT_LEFT_UP(ImagePanel::mouseReleased)
    EVT_RIGHT_DOWN(ImagePanel::rightClick)
    EVT_LEAVE_WINDOW(ImagePanel::mouseLeftWindow)
    EVT_KEY_DOWN(ImagePanel::keyPressed)
    EVT_KEY_UP(ImagePanel::keyReleased)
    EVT_MOUSEWHEEL(ImagePanel::mouseWheelMoved)
    */ 
    // catch paint events
    EVT_PAINT(ImagePanel::paintEvent)
    //Size event
    EVT_SIZE(ImagePanel::onSize)
    EVT_MAXIMIZE(ImagePanel::maxSize)
END_EVENT_TABLE()


// some useful events
/*
 void ImagePanel::mouseMoved(wxMouseEvent& event) {}
 void ImagePanel::mouseDown(wxMouseEvent& event) {}
 void ImagePanel::mouseWheelMoved(wxMouseEvent& event) {}
 void ImagePanel::mouseReleased(wxMouseEvent& event) {}
 void ImagePanel::rightClick(wxMouseEvent& event) {}
 void ImagePanel::mouseLeftWindow(wxMouseEvent& event) {}
 void ImagePanel::keyPressed(wxKeyEvent& event) {}
 void ImagePanel::keyReleased(wxKeyEvent& event) {}
 */

ImagePanel::ImagePanel(wxFrame* parent) : wxPanel(parent) {
    w = 0;
    h = 0;
    pictureLoaded = false;
    img = nullptr;
}

ImagePanel::ImagePanel(wxFrame* parent, 
                            wxString file, 
                            wxBitmapType format) :  wxPanel(parent) {
    img = nullptr;
    w = 0;
    h = 0;
    pictureLoaded = false;
    if (image.LoadFile(file, format)) {
        pictureLoaded = true;
        wxSize size = image.GetSize();
        w = size.GetWidth();
        h = size.GetHeight();
        zoom = 1.0f;
        ratio = float(w) / float(h);
    }
}

bool ImagePanel::loadImage(wxString file, wxBitmapType format) {
    if (image.LoadFile(file, format)) {
        pictureLoaded = true;
        wxSize size = image.GetSize();
        w = size.GetWidth();
        h = size.GetHeight();
        zoom = 1.0f;
        ratio = float(w) / float(h);

        img = new Image();
        img->fromBits(image.GetData(), image.GetWidth(), image.GetHeight());
        img->doDCT();
        // temp
        img->doDWT();
        image.SetData((unsigned char *) img->getBitsModified(), img->getImageX(), img->getImageY(), true);
        Refresh();


        return true;
    }
    return false;
}

void ImagePanel::paintEvent(wxPaintEvent & evt) {
    // depending on your system you may need to look at double-buffered dcs
    wxPaintDC dc(this);
    render(dc);
}

void ImagePanel::paintNow() {
    // depending on your system you may need to look at double-buffered dcs
    wxClientDC dc(this);
    render(dc);
}

void ImagePanel::render(wxDC&  dc) {
    if (!pictureLoaded)
        return;

    int neww, newh;
    GetClientSize( &neww, &newh );

    if (neww > (float)newh * ratio)
        neww = (float)newh * ratio;
    else
        newh = (float)neww / ratio;


    if (neww <= 0 || newh <= 0)
        return;
       
    if( ( neww != w || newh != h ) && ( neww < (w*zoom) && newh < (h*zoom) ) )  {
        resized = wxBitmap( image.Scale( neww*zoom, newh*zoom, wxIMAGE_QUALITY_HIGH ) );
        dc.DrawBitmap( resized, 0, 0, false );
    } else {
        resized = wxBitmap( image.Scale( w*zoom, h*zoom, wxIMAGE_QUALITY_HIGH ) );
        dc.DrawBitmap( resized, 0, 0, false );
    }
}

void ImagePanel::onSize(wxSizeEvent& event) {
    Refresh();
    event.Skip();
}

void ImagePanel::maxSize(wxMaximizeEvent& event) {
    Refresh();
    event.Skip();
}

void ImagePanel::setBrightness(const int& value) {
    if (img != nullptr) {
        img->changeDCT(value);  
        image.SetData((unsigned char *) img->getBitsModified(), img->getImageX(), img->getImageY(), true);
        Refresh();
        fprintf(stderr, "%d\n", value);
    }
}

void ImagePanel::setDCT(const int* DCT_Matrix) {
    if (img != nullptr) {
        img->setCustomDCTMatrix(DCT_Matrix);
    }
}