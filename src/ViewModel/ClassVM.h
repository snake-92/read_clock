#ifndef _CLASS_VM_H_
#define _CLASS_VM_H_

#include "wx/wx.h"
#include "../Model/Model.h"
#include <memory>

class ClassVM
{
public:
    ClassVM(wxImage& image);
    ~ClassVM();

private:
    wxImage& m_Image;
    std::shared_ptr<Model> m_model;
};

#endif