#ifndef _MAIN_FRAME_VIEW_H_
#define _MAIN_FRAME_VIEW_H_

#include <wx/wx.h>
#include "BufferedBitmap.h"
#include "../ViewModel/ClassVM.h"

class MainFrameView : public wxFrame
{
public:
    MainFrameView(const wxString& title, const wxPoint& pos, const wxSize& size, long style = wxDEFAULT_FRAME_STYLE);
    ~MainFrameView();

protected:
    // Init
    void InitMenuBar();
    void InitStatusBar();

    void UpdateImage(const wxImage &image);

    // Event menu
    void OnLoadImage(wxCommandEvent& event);
    void OnQuit(wxCommandEvent& event);
    void OnHelp(wxCommandEvent& event);

private:
    std::shared_ptr<ClassVM> m_ViewModel;
    std::shared_ptr<BufferedBitmap> m_bitmap;
    wxImage m_CurrentImage; 

DECLARE_EVENT_TABLE()
};


#endif