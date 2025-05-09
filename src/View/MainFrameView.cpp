#include "MainFrameView.h"


MainFrameView::MainFrameView(const wxString &title, const wxPoint &pos, const wxSize &size, long style) 
: wxFrame(nullptr, wxID_ANY, title, pos, size, style)
{
    
    // init
    InitMenuBar();
    InitStatusBar();

    // panels
    auto sizer = new wxBoxSizer(wxHORIZONTAL);

    // panel boutons
    auto sizerButton = new wxBoxSizer(wxVERTICAL);
    auto buttonReadHour = new wxButton(this, ID_GUI::BUTTON_READ_HOUR, "Read hour");
    auto buttonDetectClock = new wxButton(this, ID_GUI::BUTTON_DETECT_CLOCK, "Detect clock");
    sizerButton->Add(buttonReadHour, 0, wxEXPAND | wxALL, FromDIP(5));
    sizerButton->Add(buttonDetectClock, 0, wxEXPAND | wxALL, FromDIP(5));
    sizer->Add(sizerButton, 0, wxEXPAND | wxALL, FromDIP(5));

    // panel viewer image
    auto sizerImage = new wxBoxSizer(wxHORIZONTAL);
    m_bitmap = std::make_shared<BufferedBitmap>(this, wxID_ANY, wxBitmap(wxSize(1, 1)), wxDefaultPosition, FromDIP(wxSize(700, 500)));
    sizerImage->Add(m_bitmap.get(), 1, wxEXPAND | wxALL, FromDIP(5));

    sizer->Add(sizerButton, 0, wxEXPAND | wxALL, FromDIP(5));
    sizer->AddSpacer(FromDIP(5));
    sizer->Add(sizerImage, 1, wxEXPAND | wxALL, FromDIP(5));

    this->SetSizerAndFit(sizer);

    // init view model
    m_ViewModel = std::make_shared<ClassVM>(m_CurrentImage);    
}

MainFrameView::~MainFrameView()
{
}


BEGIN_EVENT_TABLE(MainFrameView, wxFrame)
    EVT_MENU(wxID_EXIT,  MainFrameView::OnQuit)
    EVT_MENU(wxID_HELP, MainFrameView::OnHelp)
    EVT_MENU(wxID_FILE, MainFrameView::OnLoadImage)

    EVT_BUTTON(ID_GUI::BUTTON_READ_HOUR, MainFrameView::OnClickReadHour)
    EVT_BUTTON(ID_GUI::BUTTON_DETECT_CLOCK, MainFrameView::OnClickDetectClock)
    
END_EVENT_TABLE()


void MainFrameView::InitMenuBar()
{
    // File menu
    wxMenu* menuFile = new wxMenu();
    wxMenuItem* fileItem = new wxMenuItem(menuFile, wxID_FILE, "open image");
    //fileItem->SetBitmap(wxBitmap(loadImage));
    menuFile->Append(fileItem);
    menuFile->AppendSeparator();
    wxMenuItem* quitItem = new wxMenuItem(menuFile, wxID_EXIT, "Exit");
    //quitItem->SetBitmap(wxBitmap(closeIcon));
    menuFile->Append(quitItem);

    // Help menu
    wxMenu* menuHelp = new wxMenu();
    wxMenuItem* helpItem = new wxMenuItem(menuHelp, wxID_HELP, "wiki");
    //helpItem->SetBitmap(wxBitmap(helpIcon));
    menuHelp->Append(helpItem);

    // add different menu in menu bar 
    wxMenuBar* menuBarre = new wxMenuBar();
    menuBarre->Append(menuFile,("File"));
    menuBarre->Append(menuHelp,("Help"));

    SetMenuBar(menuBarre);
}


void MainFrameView::InitStatusBar()
{
    CreateStatusBar(2); // barre de status avec deux emplacements
    SetStatusText(_("Ready !"),0); // texte sur emplacement 0
    SetStatusText(_(" mouse ..."),1);  // texte sur emplacement 1

    //PushStatusText(_("ton texte"),0);  // ecrire un texte sur l'emplecement 0
    //PopStatusText(0);   // revenir sur le texte de base de l'emplecement 0
}


/// @brief mise à jour de l'image dans le viewer
/// @param image 
void MainFrameView::UpdateImage(const wxImage &image)
{
    m_bitmap->SetBitmap(wxBitmap(image));
    this->Layout();
}

/// @brief Event loading an image
/// @param event 
void MainFrameView::OnLoadImage(wxCommandEvent& event)
{
    PushStatusText(_("Load an image ..."),0); 
    
    wxFileDialog openFileDialog(this, _("Load an image"), "", "",
                       "images (*.bmp;*.gif;*.tif;*.png;*.jpg;*.jpeg)|*.bmp;*.gif;*.tif;*.png;*.jpg;*.jpeg", wxFD_OPEN|wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;

    if (!m_CurrentImage.LoadFile(openFileDialog.GetPath()))
    {
        wxMessageBox("Error during opening the file", "Error", wxOK | wxICON_ERROR);
        return;
    }

    UpdateImage(m_CurrentImage);

    PopStatusText(0);
}


/// @brief Event to close the application
/// @param WXUNUSED 
void MainFrameView::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    int answer = wxMessageBox(_("Do you whant to quit ?"), _("Close"), wxYES_NO);
    if(answer == wxYES)
    {
        Close(true);
    }
}


void MainFrameView::OnClickReadHour(wxCommandEvent& event)
{
    if(!m_CurrentImage.IsOk())
    {
        return;
    }

    try
    {
        m_ViewModel->ReadHour();
        UpdateImage(m_CurrentImage);
    }
    catch(const std::exception& e)
    {
        wxMessageBox(_(e.what()), _("Error"));
    }
}


void MainFrameView::OnClickDetectClock(wxCommandEvent& event)
{
    if(!m_CurrentImage.IsOk())
    {
        return;
    }
    
    try
    {
        m_ViewModel->DetectClock();
        UpdateImage(m_CurrentImage);
    }
    catch(const std::exception& e)
    {
        wxMessageBox(_(e.what()), _("Error"));
    }   
}


/// @brief Event to display documentation
/// @param event 
void MainFrameView::OnHelp(wxCommandEvent& event)
{

}