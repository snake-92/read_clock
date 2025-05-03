#include "ClassVM.h"


ClassVM::ClassVM(wxImage& image) : m_Image(image)
{
    m_model = std::make_shared<Model>();
}

ClassVM::~ClassVM()
{

}

