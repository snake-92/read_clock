#include "MainFrameView.h"


MainFrameView::MainFrameView(const wxString &title, const wxPoint &pos, const wxSize &size, long style) 
: wxFrame(nullptr, wxID_ANY, title, pos, size, style)
{
}

MainFrameView::~MainFrameView()
{
}


BEGIN_EVENT_TABLE(MainFrameView, wxFrame)
END_EVENT_TABLE()