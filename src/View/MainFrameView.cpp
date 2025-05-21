#include "MainFrameView.h"
#include "../../resources/icons.h"


MainFrameView::MainFrameView(const wxString &title, const wxPoint &pos, const wxSize &size, long style) 
: wxFrame(nullptr, wxID_ANY, title, pos, size, style)
{
    
    // init
    InitMenuBar();
    InitToolsBar();
    InitStatusBar();

    // all widgets in main window
    // buttons
    auto buttonReadHour = new wxButton(this, ID_GUI::BUTTON_READ_HOUR, "Read hour");
    buttonReadHour->SetBitmap(wxBitmap(readTimeIcon), wxRIGHT);
    buttonReadHour->SetBitmapMargins(4,0);
    buttonReadHour->Fit();
    buttonReadHour->Bind(wxEVT_MOTION, &MainFrameView::OnMouseMoveEvent, this);
    auto buttonDetectClock = new wxButton(this, ID_GUI::BUTTON_DETECT_CLOCK, "Detect clock");
    buttonDetectClock->SetBitmap(wxBitmap(searchClockIcon), wxRIGHT);
    buttonDetectClock->SetBitmapMargins(4,0);
    buttonDetectClock->Fit();
    buttonDetectClock->Enable(false);
    buttonDetectClock->Bind(wxEVT_MOTION, &MainFrameView::OnMouseMoveEvent, this);

    // viewer
    m_bitmap = std::make_shared<BufferedBitmap>(this, wxID_ANY, wxBitmap(wxSize(1, 1)), wxDefaultPosition, FromDIP(wxSize(700, 500)));
    m_bitmap->Bind(wxEVT_MOTION, &MainFrameView::OnMouseMoveEvent, this);

    // result of the detection
    m_staticTextTime = new wxStaticText(this, wxID_ANY, "00:00");
    wxFont font = m_staticTextTime->GetFont();
    font.SetPointSize(16);   // increase size (by default : 10 or 11)
    font.MakeBold();         // bold texte
    m_staticTextTime->SetFont(font);

    // all sizers
    // sizer button
    auto sizerLeft = new wxBoxSizer(wxVERTICAL);
    sizerLeft->Add(buttonReadHour, 1, wxEXPAND | wxALL, FromDIP(5));
    sizerLeft->AddSpacer(FromDIP(5));
    sizerLeft->Add(buttonDetectClock, 1, wxEXPAND | wxALL, FromDIP(5));

    // sizer image and result text
    auto sizerRight = new wxBoxSizer(wxVERTICAL);
    sizerRight->Add(m_bitmap.get(), 1, wxEXPAND | wxALL, FromDIP(5));
    sizerRight->Add(m_staticTextTime, 0, wxALIGN_CENTER | wxALL, FromDIP(5)); 

    auto sizer = new wxBoxSizer(wxHORIZONTAL);
    sizer->Add(sizerLeft, 0, wxEXPAND | wxALL, FromDIP(5));
    sizer->AddSpacer(FromDIP(5));
    sizer->Add(sizerRight, 1, wxEXPAND | wxALL, FromDIP(5));
    this->SetSizerAndFit(sizer);

    // init view model
    m_ViewModel = std::make_shared<ClassVM>(m_CurrentImage);
    m_NewImage = false;    
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
    
    EVT_TOOL(wxID_ZOOM_IN, MainFrameView::OnZoomIn)
    EVT_TOOL(wxID_ZOOM_OUT, MainFrameView::OnZoomOut)
    EVT_TOOL(wxID_UNDO, MainFrameView::OnClickNextImage)
    EVT_TOOL(wxID_REDO, MainFrameView::OnClickPreviousImage)
END_EVENT_TABLE()


void MainFrameView::InitMenuBar()
{
    // File menu
    wxMenu* menuFile = new wxMenu();
    wxMenuItem* fileItem = new wxMenuItem(menuFile, wxID_FILE, "open image");
    fileItem->SetBitmap(wxBitmap(loadIcon));
    menuFile->Append(fileItem);
    menuFile->AppendSeparator();
    wxMenuItem* quitItem = new wxMenuItem(menuFile, wxID_EXIT, "Exit");
    quitItem->SetBitmap(wxBitmap(quitIcon));
    menuFile->Append(quitItem);

    // Help menu
    wxMenu* menuHelp = new wxMenu();
    wxMenuItem* helpItem = new wxMenuItem(menuHelp, wxID_HELP, "wiki");
    helpItem->SetBitmap(wxBitmap(helpIcon));
    menuHelp->Append(helpItem);

    // add different menu in menu bar 
    wxMenuBar* menuBarre = new wxMenuBar();
    menuBarre->Append(menuFile,("File"));
    menuBarre->Append(menuHelp,("Help"));

    SetMenuBar(menuBarre);
}


void MainFrameView::EnableButtonTools(bool enable)
{
    GetToolBar()->EnableTool(wxID_REDO,   enable);
    GetToolBar()->EnableTool(wxID_UNDO,  enable);
}

void MainFrameView::InitToolsBar()
{
    wxToolBar* toolbar = CreateToolBar(wxTB_FLAT | wxTB_HORIZONTAL | wxNO_BORDER);
    toolbar->AddSeparator();

    // zoom et dezoom
    toolbar->AddTool(wxID_ZOOM_IN, _("zoom In"), wxBitmap(zoomInIcon), _("zoom In"));
    toolbar->AddTool(wxID_ZOOM_OUT, _("zoom Out"), wxBitmap(zoomOutIcon), _("zoom Out"));
    toolbar->AddSeparator();

    // undo et redo
    toolbar->AddTool(wxID_REDO, _("redo"), wxBitmap(previousIcon), _("next image"));
    toolbar->AddTool(wxID_UNDO, _("undo"), wxBitmap(nextIcon), _("previous image"));
    toolbar->AddSeparator();

    EnableButtonTools(false);

    toolbar->Realize();
}


void MainFrameView::InitStatusBar()
{
    auto statusBar = CreateStatusBar(2); // barre de status avec deux emplacements
    statusBar->SetDoubleBuffered(true);  // help to stop flickering
    SetStatusText(_("Ready !"),0); // texte sur emplacement 0
    SetStatusText(_(" mouse ..."),1);  // texte sur emplacement 1

    //PushStatusText(_("ton texte"),0);  // ecrire un texte sur l'emplecement 0
    //PopStatusText(0);   // revenir sur le texte de base de l'emplecement 0
}

void MainFrameView::OnMouseMoveEvent(wxMouseEvent& event)
{
    ID_GUI id = (ID_GUI)event.GetId();

    SetStatusText(_("Ready !"),0);

    if(id == ID_GUI::BUTTON_READ_HOUR)
    {
        SetStatusText(_("Read hour on the image"),0);
    }
    else if(id == ID_GUI::BUTTON_DETECT_CLOCK)
    {
        SetStatusText(_("Display all brand detection"),0);
    } 
    else
    {
        wxPoint pt = event.GetPosition();
        SetStatusText(wxString::Format("mouse position: (%d, %d)", pt.x, pt.y),1);
    } 
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
    EnableButtonTools(false);
    wxButton* btn = wxDynamicCast(FindWindow(ID_GUI::BUTTON_DETECT_CLOCK), wxButton);
    if(btn) btn->Enable(false);
    m_NewImage = true;
    m_staticTextTime->SetLabel("00:00");

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

    if(!m_NewImage)
    {
        return;
    }

    try
    {
        m_staticTextTime->SetLabel(m_ViewModel->ReadHour());
        UpdateImage(m_CurrentImage);
        EnableButtonTools(true);        
        wxButton* btn = wxDynamicCast(FindWindow(ID_GUI::BUTTON_DETECT_CLOCK), wxButton);
        if (btn) btn->Enable(true);  
        m_NewImage = false;     
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


void MainFrameView::OnClickNextImage(wxCommandEvent& event)
{
    if(!m_CurrentImage.IsOk())
    {
        return;
    }

    try
    {
        m_ViewModel->NextImage();
        UpdateImage(m_CurrentImage);
    }
    catch(const std::exception& e)
    {
        wxMessageBox(_(e.what()), _("Error"));
    }
}

void MainFrameView::OnClickPreviousImage(wxCommandEvent& event)
{
    if(!m_CurrentImage.IsOk())
    {
        return;
    }

    try
    {
        m_ViewModel->PreviousImage();
        UpdateImage(m_CurrentImage);
    }
    catch(const std::exception& e)
    {
        wxMessageBox(_(e.what()), _("Error"));
    }
}


void MainFrameView::OnZoomIn(wxCommandEvent& event)
{
    m_bitmap->ZoomIn();
}


void MainFrameView::OnZoomOut(wxCommandEvent& event)
{
    m_bitmap->ZoomOut();
}


/// @brief Event to display documentation
/// @param event 
void MainFrameView::OnHelp(wxCommandEvent& event)
{

}