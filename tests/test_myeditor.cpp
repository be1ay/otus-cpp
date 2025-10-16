#define BOOST_TEST_MODULE GraphicEditorTest
#include <boost/test/included/unit_test.hpp>
#include "controller.h"
#include "shapes.h"
#include "view.h"

using namespace myeditor;

BOOST_AUTO_TEST_CASE(add_and_draw_shapes) {
    Controller c;
    c.newDocument();
    c.createShape<Circle<int>>(10, 20, 5);
    c.createShape<Rectangle<float>>(0.f, 0.f, 100.f, 50.f);
    View view;
    BOOST_CHECK_NO_THROW(c.draw(view));
}
