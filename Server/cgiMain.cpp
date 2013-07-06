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
#include"../common/tournament.h"
#include"../common/match.h"
//cgi app to modify the XML files.

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QByteArray in;
    int tmp;
    while((tmp = getchar()) != EOF)
        in.append(QChar(tmp));

    QMap<QString, QString> input;
    printf("Content-type: text/plain\r\n\r\n");
    QSettings settings("xmlPaths.ini", QSettings::IniFormat);
    if(!in.contains('&')){//Invalid invocation
        printf("FAIL - Error Invalid Input\n");
        printf("HINT - %s", settings.allKeys().join(",").toAscii().constData());
        return 0;
    }
    foreach(const QString &str, QString::fromUtf8(in).split(QChar('&'))){
        QStringList split = str.split('=');
        input.insert(split[0], split[1].trimmed());
    }
    //DEBUGGING
    //printf("%s\n", in.constData());

    QString targetFile = settings.value("tournaments/"+input.value("tournamentTitle", "dummy"),
                                     QVariant("/var/www/html/dummyTournament.xml")).toString();
    QString lockFilePath = QDir::tempPath() + "/" + targetFile.split('/').last() + ".lock";
    Tournament *t = new Tournament;
    if(QFile::exists(lockFilePath)){
        printf("FAIL - Error Lock File");
    }else{//Should we do decent error handling also?
        bool goingOkay = true;
        QFile lockFile(lockFilePath);
        goingOkay = goingOkay && lockFile.open(QFile::WriteOnly);
        goingOkay = (0<lockFile.write("\n"));//Assumed to be the equivalent of 'touch'
        lockFile.close();

        QFile xml(targetFile);
        goingOkay = goingOkay && xml.open(QFile::ReadOnly | QIODevice::Text);
        t->loadFromXml(xml.readAll());

        Match* m = t->matchAt(input.value("matchIndex").toInt());
        goingOkay = goingOkay && m;
        goingOkay = (1==input.remove("tournamentTitle"));
        goingOkay = (1==input.remove("matchIndex"));
        //Treat winner specially, this sets up the future matches
        if(input.value("winner") != QString("")){
            int winner = 0;
            if(input.value("winner")==m->player1())
                winner = 1;
            else if(input.value("winner")==m->player2())
                winner = 2;
            if(winner>0)
                t->matchFinished(m, winner==1);
            else
                printf("I did not know that %s was playing this match.\n", input.value("winner").toAscii().data());
        }
        input.remove("winner");

        //generic approach is forwards compatible
        if(m){
            foreach(const QString &key, input.keys()){
                m->setProperty(key.toUtf8().constData(), QVariant(input.value(key)));
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
