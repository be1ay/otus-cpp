#include "controller.h"
#include <iostream>
#include "view.h"

namespace myeditor{
    Controller::Controller() : doc_(std::make_shared<Document>()) {}

    void Controller::newDocument() {
        doc_ = std::make_shared<Document>();
        std::cout << "New document created."<<std::endl;
    }

    void Controller::importDocument(const std::string& filename) {
        std::cout << "Import from " << filename << std::endl;
    }

    void Controller::exportDocument(const std::string& filename) {
        std::cout << "Export to " << filename << std::endl;
    }

    void Controller::clearShapes() {
        doc_->removeAll();
        std::cout << "removeAll shapes" << std::endl;
    }

    void Controller::draw(const View& view) const {
        view.render(doc_->shapes());
    }
}//namespace myeditor