#include "controller.h"

// REMOVE test
#include <QDebug>
#include <iostream>

Controller::Controller()
{
    // REMOVE test /////////////////
    testTimer.setInterval(100);
    connect(&testTimer, SIGNAL(timeout()), this, SLOT(timerTick()));
    connect(this, SIGNAL(startTimer()), &testTimer, SLOT(start()));
}

void Controller::start()
{
    emit start_();
    // REMOVE test /////////////////
    counter = 0;
}

void Controller::move(QString m)
{
    emit move_(m);
    // REMOVE test /////////////////
    counter++;
    if (counter == 20) {
        save("test_save.png");
        testTimer.stop();
    }
}

void Controller::save(QString p)
{
    emit save_(p);
}

void Controller::startController()
{
    start();
    
    // REMOVE test /////////////////
    using uint = unsigned int;
    std::srand(uint(std::time({})));
    testTimer.moveToThread(&timerThread);
    timerThread.start();
    emit startTimer();
}

// REMOVE test /////////////////
void Controller::timerTick()
{
    auto x = std::rand()%31;
    auto y = std::rand()%31;
    auto color = std::rand()%2;
    QString cmd = "{ \"point\": { \"x\": " + QString::number(x) + 
                                 ", \"y\": " + QString::number(y) + "}, \"color\": " + QString::number(color) + " }";
    move(cmd);
}
