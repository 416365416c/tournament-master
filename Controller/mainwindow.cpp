#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../Common/tournament.h"
#include "../Common/match.h"
#include "../Common/player.h"
#include <QListWidgetItem>
#include <QQmlListProperty>
#include <QQmlContext>
#include <QInputDialog>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <qqml.h>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), curTournament(0)
{
    ui->setupUi(this);
    engine = new QQmlApplicationEngine(this);
    engine->load("qml/TourneyDisplay.qml");
    engine->setIncubationController(0);//### CRASHY?
    setWindowTitle("Tournament Controller");
    qmlRegisterType<Match>("App",0,1,"Match");
    qmlRegisterType<Player>("App",0,1,"Player");
    qmlRegisterType<Tournament>("App",0,1,"Tournament");
    loadAllInDir();
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::loadFile(QString path){
    QFile in(path);
    if(in.open(QFile::ReadOnly | QIODevice::Text)){
        Tournament *newTournament = new Tournament(this);
        newTournament->loadFromXml(in.readAll());
        ui->listWidget->addItem(newTournament->name());
        ui->listWidget->item(ui->listWidget->count() - 1)->setData(Qt::UserRole, QVariant::fromValue<QObject*>(newTournament));
        return true;
    }
    return false;
}

void MainWindow::loadAllInDir()
{
    QDir curDir(".");
    QStringList filters;
    filters << "*.xml";
    foreach(const QString &fileName, curDir.entryList(filters))
        loadFile(fileName);
}

void MainWindow::reloadMatches()
{
}

bool MainWindow::curTournamentCheck()
{
    if(!curTournament)
        QMessageBox::warning(this, "Error", "Please select a tournament first.");
    return curTournament;
}

void MainWindow::on_listWidget_currentItemChanged(QListWidgetItem* current, QListWidgetItem* previous)
{
    Q_UNUSED(previous);
    curTournament = qobject_cast<Tournament*>(current->data(Qt::UserRole).value<QObject*>());
    engine->rootContext()->setContextProperty("tournament",curTournament);
}

void MainWindow::on_addButton_clicked()
{//add tournament
    QString name = QInputDialog::getText(this, "Add Tournament","Please enter the tournament's name");
    ui->listWidget->addItem(name);
    Tournament* newTournament = new Tournament(this);
    newTournament->setName(name);
    ui->listWidget->item(ui->listWidget->count() - 1)->setData(Qt::UserRole, QVariant::fromValue<QObject*>(newTournament));
}

void MainWindow::on_removeButton_clicked()
{
    QMessageBox::information(this, "TODO", "I'm not yet implemented :)");
}

void MainWindow::on_actionImport_triggered()
{
    QString name = QFileDialog::getOpenFileName(this, "Import Tournament");
    loadFile(name);
}

void MainWindow::on_actionExport_triggered()
{
    QString name = QFileDialog::getSaveFileName(this, "Export Tournament");
    QByteArray xml = curTournament->writeToXml();
    QFile out(name);
    if(out.open(QFile::WriteOnly | QIODevice::Text)){
        out.write(xml);
    }
}

void MainWindow::on_actionAdd_triggered()
{//add player
    if(!curTournamentCheck()) return;
    QString name = QInputDialog::getText(this, "Add Player","Please enter the player's name");
    curTournament->addEmptyPlayer();
    curTournament->playerAt(curTournament->playerCount() - 1)->setName(name);
}

void MainWindow::on_actionShuffle_triggered()
{
    if(!curTournamentCheck()) return;
    curTournament->shufflePlayerOrder();
}

void MainWindow::on_actionQuit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_actionAdd_2_triggered()
{
    if(!curTournamentCheck()) return;
    curTournament->addEmptyMatch();
}

void MainWindow::on_actionSingle_Elim_triggered()
{
    if(!curTournamentCheck()) return;
    curTournament->createSingleElim();
}

void MainWindow::on_actionDouble_Elim_triggered()
{
    if(!curTournamentCheck()) return;
    curTournament->createDoubleElim();
}

void MainWindow::on_actionRound_Robin_triggered()
{
    if(!curTournamentCheck()) return;
    curTournament->createRoundRobin();
}

void MainWindow::on_actionArrange_triggered()
{
    if(!curTournamentCheck()) return;
    curTournament->arrangeMatches();
}
