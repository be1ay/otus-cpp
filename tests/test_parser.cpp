#define BOOST_TEST_MODULE GraphicEditorTest
#include <boost/test/included/unit_test.hpp>

#include "CommandCollector.h"



struct TestObserver : public Observer { //просто сохраняет в память
    struct Bulk {
        VectorCommands commands;
        std::string timestamp;
    };
    std::vector<Bulk> received;

    void onBulk(const VectorCommands& commands, const std::string& timestamp) override {
        received.push_back({commands, timestamp});
    }
};

BOOST_AUTO_TEST_SUITE(BulkCollectorTests)

BOOST_AUTO_TEST_CASE(SimpleBulkTest) {
    BulkNotifier notifier;
    auto obs = std::make_shared<TestObserver>();
    notifier.addObserver(obs);

    CommandCollector collector(3, notifier);
    collector.addCommand("cmd1");
    collector.addCommand("cmd2");
    collector.addCommand("cmd3"); // должно сработать flush
    collector.finish();

    BOOST_CHECK_EQUAL(obs->received.size(), 1);
    BOOST_CHECK((obs->received[0].commands == VectorCommands{"cmd1","cmd2","cmd3"}));
}

BOOST_AUTO_TEST_CASE(DynamicBlockTest) {
    BulkNotifier notifier;
    auto obs = std::make_shared<TestObserver>();
    notifier.addObserver(obs);

    CommandCollector collector(2, notifier);
    collector.addCommand("cmd1");
    collector.addCommand("cmd2"); 
    collector.openBrace(); // обычный flush
    collector.addCommand("cmd3");
    collector.addCommand("cmd4");
    collector.closeBrace(); // flush динамического блока
    collector.finish();

    BOOST_CHECK_EQUAL(obs->received.size(), 2);
    BOOST_CHECK((obs->received[0].commands == VectorCommands{"cmd1","cmd2"}));
    BOOST_CHECK((obs->received[1].commands == VectorCommands{"cmd3","cmd4"}));
}

BOOST_AUTO_TEST_CASE(NestedBracesTest) {
    BulkNotifier notifier;
    auto obs = std::make_shared<TestObserver>();
    notifier.addObserver(obs);

    CommandCollector collector(3, notifier);
    collector.addCommand("cmd1");
    collector.openBrace();
    collector.addCommand("cmd2");
    collector.openBrace(); // вложенный блок — игнорируем вложенность
    collector.addCommand("cmd3");
    collector.closeBrace();
    collector.addCommand("cmd4");
    collector.closeBrace(); // flush после закрытия внешнего блока
    collector.finish();

    BOOST_CHECK_EQUAL(obs->received.size(), 2);
    BOOST_CHECK(obs->received[0].commands == VectorCommands{"cmd1"});
    BOOST_CHECK((obs->received[1].commands == VectorCommands{"cmd2","cmd3","cmd4"}));
}

BOOST_AUTO_TEST_CASE(EOFInsideDynamicBlockTest) {
    BulkNotifier notifier;
    auto obs = std::make_shared<TestObserver>();
    notifier.addObserver(obs);

    CommandCollector collector(2, notifier);
    collector.addCommand("cmd1");
    collector.openBrace();
    collector.addCommand("cmd2");
    collector.addCommand("cmd3");
    collector.finish(); // EOF внутри динамического блока — блок игнорируется

    BOOST_CHECK_EQUAL(obs->received.size(), 1); // только cmd1 сработал
    BOOST_CHECK(obs->received[0].commands == VectorCommands{"cmd1"});
}

BOOST_AUTO_TEST_CASE(ConsecutiveBracesTest) {
    BulkNotifier notifier;
    auto obs = std::make_shared<TestObserver>();
    notifier.addObserver(obs);

    CommandCollector collector(2, notifier);
    collector.openBrace(); // пустой блок, ничего не срабатывает
    collector.closeBrace();
    collector.openBrace();
    collector.closeBrace();
    collector.addCommand("cmd1");
    collector.addCommand("cmd2");
    collector.finish();

    BOOST_CHECK_EQUAL(obs->received.size(), 1);
    BOOST_CHECK((obs->received[0].commands == VectorCommands{"cmd1","cmd2"}));
}

BOOST_AUTO_TEST_CASE(EndBlockTest) {
    BulkNotifier notifier;
    auto obs = std::make_shared<TestObserver>();
    notifier.addObserver(obs);

    CommandCollector collector(3, notifier);
    collector.addCommand("cmd1");
    collector.addCommand("cmd2");
    collector.finish();

    BOOST_CHECK_EQUAL(obs->received.size(), 1);
    BOOST_CHECK((obs->received[0].commands == VectorCommands{"cmd1","cmd2"}));
}
BOOST_AUTO_TEST_SUITE_END()