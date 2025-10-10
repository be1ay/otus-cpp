#include "controller.h"
#include <iostream>
#include "shape.h"
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

    void Controller::draw() {
        doc_->drawAll();
    }
}//namespace myeditor