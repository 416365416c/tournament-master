#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

//Done here to prevent the cgiStub needing qml
#include <qqml.h>
#include <QQmlApplicationEngine>
#include "../Common/tournament.h"
#include "../Common/match.h"
#include "../Common/player.h"
//QML_DECLARE_TYPE(Tournament);
//QML_DECLARE_TYPE(Match);
//QML_DECLARE_TYPE(Player);

class QListWidgetItem;
namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QQmlApplicationEngine *engine;
    Tournament* curTournament;

private slots:
    void on_actionQuit_triggered();
    bool loadFile(QString);
    void loadAllInDir();
    void reloadMatches();
    bool curTournamentCheck();

    void on_actionShuffle_triggered();
    void on_actionAdd_triggered();
    void on_actionImport_triggered();
    void on_actionExport_triggered();
    void on_addButton_clicked();
    void on_removeButton_clicked();
    void on_listWidget_currentItemChanged(QListWidgetItem* current, QListWidgetItem* previous);
    void on_actionAdd_2_triggered();
    void on_actionSingle_Elim_triggered();
    void on_actionDouble_Elim_triggered();
    void on_actionRound_Robin_triggered();
    void on_actionArrange_triggered();
};

#endif // MAINWINDOW_H
