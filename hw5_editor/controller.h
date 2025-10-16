#pragma once
#include "document.h"
#include <memory>

namespace myeditor{
 class View;
    /**
 * @brief Controller
 *
 */
    class Controller {
        std::shared_ptr<Document> doc_;
    public:
        Controller();
        void newDocument();
        void importDocument(const std::string& filename);
        void exportDocument(const std::string& filename);
        template<typename ShapeType, typename... Args>
        void createShape(Args&&... args);
        void clearShapes();
        void draw(const View& view) const;
    };

    template<typename ShapeType, typename... Args>
    void Controller::createShape(Args&&... args) {
        auto shape = std::make_shared<ShapeType>(std::forward<Args>(args)...);
        doc_->addShape(shape);
    }
}//namespace myeditor