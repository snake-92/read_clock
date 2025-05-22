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
#include <wx/snglinst.h>

class MyApp : public wxApp 
{
public:
    virtual bool OnInit();

private:
    wxSingleInstanceChecker* m_checker = nullptr;
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    m_checker = new wxSingleInstanceChecker("MyInstanceAppLock");

    if (m_checker->IsAnotherRunning())
    {
        wxMessageBox("The app is already running.", "Info", wxOK | wxICON_INFORMATION);
        delete m_checker;
        return false; // Quitte l'application
    }

    wxInitAllImageHandlers(); // allow all events on the image

    MainFrameView *frame = new MainFrameView("Hello World", wxDefaultPosition, wxSize(1280, 720));
    frame->Show(true);
    return true;
}