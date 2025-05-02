#ifndef _MAIN_FRAME_VIEW_H_
#define _MAIN_FRAME_VIEW_H_

#include <wx/wx.h>

class MainFrameView : public wxFrame
{
public:
    MainFrameView(const wxString& title, const wxPoint& pos, const wxSize& size, long style = wxDEFAULT_FRAME_STYLE);
    ~MainFrameView();

private:

DECLARE_EVENT_TABLE()
};


#endif