#pragma once
#include <memory>
#include <vector>
#include <iostream>
#include "ishape.h"
namespace myeditor{
 /**
 * @brief View
 *
 */
    class View {
    public:
        void render(const std::vector<IShapeBasePtr>& shapes) const {
            std::cout << "Drawing document:"<<std::endl;
            for (const auto& s : shapes)
                s->draw();
        }
    };
}//namespace myeditor
