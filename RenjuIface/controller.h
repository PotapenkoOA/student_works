#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QString>

// REMOVE test ////////////////
#include <QTimer>
#include <cstdlib>
#include <ctime>
#include <QThread>

class Controller: public QObject
{
    Q_OBJECT
public:
    Controller();
    void start();
    void move(QString m);
    void save(QString m);
    
    
signals:
    void start_();
    void move_(QString moveRecord);
    void save_(QString saveFilePath);
    
// REMOVE test //////////////
public:
    Q_INVOKABLE void startController();
private:
    QThread timerThread;
    QTimer testTimer;
    int counter{};
private slots:
    void timerTick();
signals:
    void startTimer();
};

#endif // CONTROLLER_H
