#ifndef _MAIN_FRAME_VIEW_H_
#define _MAIN_FRAME_VIEW_H_

#include <wx/wx.h>
#include "../ViewModel/ClassVM.h"

class MainFrameView : public wxFrame
{
public:
    MainFrameView(const wxString& title, const wxPoint& pos, const wxSize& size, long style = wxDEFAULT_FRAME_STYLE);
    ~MainFrameView();

private:
    std::shared_ptr<ClassVM> m_ViewModel; 

DECLARE_EVENT_TABLE()
};


#endif