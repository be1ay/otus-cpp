#pragma once
#include "shape.h"
#include <vector>
#include <memory>
namespace myeditor{
    using IShapeBasePtr = std::shared_ptr<IShapeBase>;
 /**
 * @brief Model
 *
 */
    class Document {
        std::vector<IShapeBasePtr> shapes_;
    public:
        void addShape(const IShapeBasePtr& shape);
        void removeAll();
        void drawAll() const;
    };
}//namespace myeditor