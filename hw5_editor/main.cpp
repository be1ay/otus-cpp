#include "controller.h"
#include "view.h"
#include "shapes.h"

using namespace myeditor;

int main() {
    Controller controller;
    controller.newDocument();

    controller.createShape<Circle<int>>(10, 20, 5);
    controller.createShape<Rectangle<float>>(0.f, 0.f, 100.f, 50.f);
    controller.importDocument("import_filename");
    controller.exportDocument("export_filename");
    View view;
    controller.draw(view);
    return 0;
}
