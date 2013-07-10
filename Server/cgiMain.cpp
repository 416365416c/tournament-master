#include<QCoreApplication>
#include<QByteArray>
#include<QFile>
#include<QDir>
#include<QMap>
#include<QUrl>
#include<QStringList>
#include<QSettings>
#include<QDebug>
#include<cstdio>
#include"../Common/tournament.h"
#include"../Common/match.h"
#include"../Common/player.h"
#include"../Common/password.h"
//cgi app to modify the XML files.

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    printf("Content-type: text/plain\r\n\r\n");

    QByteArray in;
    int tmp;
    while((tmp = getchar()) != EOF)
        in.append(QChar(tmp));
    if(!in.contains(QByteArray("password=")+QByteArray(SHARED_PASSWORD))){//Invalid invocation
        printf("FAIL - Authentication Error\n");
        printf("DEBUG -" + in + "\n");
        printf("DEBUG -" + QByteArray("password=")+QByteArray(SHARED_PASSWORD) +"\n");
        return 0;
    }

    QMap<QString, QString> input;
    QSettings settings("xmlPaths.ini", QSettings::IniFormat);
    if(!in.contains('&')){//Invalid invocation
        printf("FAIL - Error Invalid Input\n");
        printf("HINT - %s", settings.allKeys().join(",").toLatin1().constData());
        return 0;
    }
    foreach(const QString &str, QString::fromUtf8(in).split(QChar('&'))){
        QStringList split = str.split('=');
        input.insert(split[0], split[1].trimmed());
    }
    //DEBUGGING
    printf("%s\n", in.constData());
    QString request = input.value("request", "list");
    if (request == "list") {
        settings.beginGroup("tournaments");
        foreach (const QString &s, settings.allKeys())
            printf("%s\n",settings.value(s).toString().toLatin1().constData());

        printf("\n");
        return 0;
    }

    QString targetFile = settings.value("tournaments/"+input.value("tournamentTitle", "dummy"),
                                     QVariant("/var/www/xml/dummyTournament.xml")).toString();
    QString lockFilePath = QDir::tempPath() + "/" + targetFile.split('/').last() + ".lock";
    if(QFile::exists(lockFilePath)){
        printf("FAIL - Error Lock File");
    } else if (request != "edit") {
        printf("FAIL - Unknown request");
    } else {//Should we do decent error handling also?
        bool goingOkay = true;
        QFile lockFile(lockFilePath);
        goingOkay = goingOkay && lockFile.open(QFile::WriteOnly);
        goingOkay = (0<lockFile.write("\n"));//Assumed to be the equivalent of 'touch'
        lockFile.close();

        goingOkay = (1==input.remove("tournamentTitle"));

        QFile xml(targetFile);
        goingOkay = goingOkay && xml.open(QFile::ReadOnly | QIODevice::Text);
        Tournament *t = new Tournament;
        t->loadFromXml(xml.readAll());

        QObject* o = 0;
        if (input.contains("matchIndex")) {
            Match* m = t->matchAt(input.value("matchIndex").toInt());
            goingOkay = goingOkay && m;
            goingOkay = (1==input.remove("matchIndex"));
            //Treat winner specially, this sets up the future matches
            if(input.value("winner") != QString("")){
                int winner = 0;
                if(input.value("winner")== m->player1()->getName())
                    winner = 1;
                else if(input.value("winner")==m->player2()->getName())
                    winner = 2;
                if(winner>0)
                    t->matchFinished(m, winner==1);
                else
                    printf("I did not know that %s was playing this match.\n", input.value("winner").toLatin1().data());
            }
            input.remove("winner");
            o = m;
        } else if (input.contains("playerIndex")) {
            int idx = input.value("playerIndex").toInt();
            if (idx == -1) {
                idx = t->playerCount();
                t->addEmptyPlayer();
            }
            Player *p = t->playerAt(idx);
            goingOkay = goingOkay && p;
            goingOkay = (1==input.remove("playerIndex"));
            o = p;
        } else {
            o = t;
        }
        goingOkay = goingOkay && o;


        //generic approach is forwards compatible
        if(o){
            foreach(const QString &key, input.keys()){
                o->setProperty(key.toUtf8().constData(), QVariant(input.value(key)));
            }
        }
        if(goingOkay){//Should have at least some error checking before we delete a file
            xml.remove();//also closes
            xml.open(QFile::WriteOnly | QFile::Text);
            xml.write(t->writeToXml().constData());
            printf("DONE");
            lockFile.remove();
        }else{
            printf("FAIL - Error WTF");
        }
    }
    printf("\n");
    return 0;
};
