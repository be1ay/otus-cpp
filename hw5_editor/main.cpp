#include "controller.h"

using namespace myeditor;

 /**
 * @brief View
 *
 */
int main() {
    Controller c;
    c.newDocument();
    c.createShape<Circle<int>>(10, 20, 5);
    c.createShape<Rectangle<float>>(0.f, 0.f, 100.f, 50.f);
    c.exportDocument("export_file.txt");
    c.importDocument("import_file.txt");
    c.draw();
    c.clearShapes();

    c.createShape<Circle<double>>(5.0, 10.0, 20.0);
    c.createShape<Rectangle<int>>(10, 10, 200, 150);

    c.draw();
}
