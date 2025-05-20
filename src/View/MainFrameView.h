#ifndef _MAIN_FRAME_VIEW_H_
#define _MAIN_FRAME_VIEW_H_

#include <wx/wx.h>
#include "BufferedBitmap.h"
#include "../ViewModel/ClassVM.h"

enum ID_GUI
{
    BUTTON_READ_HOUR = wxID_HIGHEST + 1,
    BUTTON_DETECT_CLOCK,
    BUTTON_NEXT_IMAGE,
    BUTTON_PREVIOUS_IMAGE
};

class MainFrameView : public wxFrame
{
public:
    MainFrameView(const wxString& title, const wxPoint& pos, const wxSize& size, long style = wxDEFAULT_FRAME_STYLE);
    ~MainFrameView();

protected:
    // Init
    void InitMenuBar();
    void InitToolsBar();
    void InitStatusBar();

    void UpdateImage(const wxImage &image);
    void EnableButtonTools(bool enable);

    // Event menu
    void OnLoadImage(wxCommandEvent& event);
    void OnQuit(wxCommandEvent& event);
    void OnHelp(wxCommandEvent& event);

    // Event button
    void OnClickReadHour(wxCommandEvent& event);
    void OnClickDetectClock(wxCommandEvent& event);
    void OnZoomIn(wxCommandEvent& event);
    void OnZoomOut(wxCommandEvent& event);
    void OnClickNextImage(wxCommandEvent& event);
    void OnClickPreviousImage(wxCommandEvent& event);
    void OnMouseMoveEvent(wxMouseEvent& event);

private:
    std::shared_ptr<ClassVM> m_ViewModel;
    std::shared_ptr<BufferedBitmap> m_bitmap;
    wxImage m_CurrentImage; 
    wxStaticText* m_staticTextTime;
    bool m_NewImage;

DECLARE_EVENT_TABLE()
};


#endif