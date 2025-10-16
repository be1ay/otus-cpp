#include "document.h"
#include <iostream>
namespace myeditor{
    void Document::addShape(const IShapeBasePtr& shape) {
        shapes_.push_back(shape);
    }

    void Document::removeAll() {
        shapes_.clear();
    }
}//namespace myeditor