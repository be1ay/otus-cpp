#pragma once
#include "ishape.h"
#include <vector>
#include <memory>
namespace myeditor{
 /**
 * @brief Model
 *
 */
    class Document {
        std::vector<IShapeBasePtr> shapes_;
    public:
        void addShape(const IShapeBasePtr& shape);
        void removeAll();
        const std::vector<IShapeBasePtr>& shapes() const {
        return shapes_;
        }
    };
}//namespace myeditor