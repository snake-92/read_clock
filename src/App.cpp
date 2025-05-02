/*
    Name: Ghislain SIDJE NZALI
    Date: 27/10/2022
    Version: 1.0
    Description: Application for read image and detect clock in the image or read hour if the image is a clock
    Version C++: 17
    Version wxwidgets: 3.2.5
    Version OpenCV: 4.9.0
    Compiler: TDM MinGW 64 bits
*/

#include <iostream>
#include "View/MainFrameView.h"

class MyApp : public wxApp 
{
public:
    virtual bool OnInit();
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    wxInitAllImageHandlers(); // allow all events on the image

    MainFrameView *frame = new MainFrameView("Hello World", wxDefaultPosition, wxSize(1280, 720));
    frame->Show(true);
    return true;
}