#include "ParamParser.h"

#include <iostream>
#include <iomanip>
#include <ios>

#include <QRegExp>
#include <QDebug>

ParamParser::ParamParser(int argc, char *argv[])
{
    keyStringByName.insert(KeyName::Help, "-h");
    keyStringByName.insert(KeyName::Ip, "ip");
    keyStringByName.insert(KeyName::Port, "port");
    keyStringByName.insert(KeyName::Path, "path");
    keyStringByName.insert(KeyName::Lang, "lang");

    params.insert("ip", "127.0.0.1");
    params.insert("port", "10600");
    params.insert("path", "dspRec");
    params.insert("lang", "eng");

    for (auto i=1; i < argc; i++)
    {
        auto keyValuePair = argv[i];
        keyValuePairs.append(keyValuePair);
    }

    parseParams();
    checkParams();
}

QString ParamParser::getLang()
{
    return params["lang"];
}

QString ParamParser::getIp()
{
    return params["ip"];
}

int ParamParser::getPort()
{
    return params["port"].toInt();
}

QString ParamParser::getPath()
{
    return params["path"];
}

bool ParamParser::getHelp ()
{
    if (findHelp())
    {
        std::cout << QObject::tr("Used keys:").toUtf8().constData() << std::endl; // Используемые ключи
        std::cout << QObject::tr("lang=[language]").toUtf8().constData()
                  << "\t" << QObject::tr("set language (eng or rus)").toUtf8().constData() << std::endl;
        std::cout << QObject::tr("ip=[ip-adress]").toUtf8().constData()
                  << "\t\t" << QObject::tr("set ip-adress").toUtf8().constData() << std::endl;
        std::cout << QObject::tr("port=[port number]").toUtf8().constData()
                  << "\t" << QObject::tr("set port number").toUtf8().constData() << std::endl;
        std::cout << QObject::tr("path=[folder name]").toUtf8().constData()
                  << "\t" << QObject::tr("set folder name").toUtf8().constData() << std::endl;
        return true;
    }
    return false;
}

void ParamParser::parseParams()
{
    for (auto keyValue : keyValuePairs)
    {
        QStringList splitedKeyValue = QString(keyValue).split("=");

        //введеное значение вида ключ=значение
        if (splitedKeyValue.size() == 2)
        {
            splitedParams.insert(splitedKeyValue[0],splitedKeyValue[1]);
        }
        else if  (QString::localeAwareCompare(splitedKeyValue[0], "-h"))
        {
            qDebug().noquote() << QObject::tr("Non-existent key");
        }
    }
}

bool ParamParser::findHelp ()
{
    for (auto keyValuePair : keyValuePairs)
    {
        if (!QString::localeAwareCompare(keyValuePair, keyStringByName[KeyName::Help]))
        {
            return true;
        }
    }
    return false;
}

void ParamParser::checkParams ()
{      
    for (auto keyValue = splitedParams.begin(); keyValue != splitedParams.end(); ++keyValue)
        for (auto it = keyStringByName.begin(); it != keyStringByName.end(); ++it)
        {
            if (!QString::localeAwareCompare(keyValue.key(), it.value()))
            {
                switch (it.key())
                {
                case KeyName::Lang:
                    if (checkLang(keyValue.value())) params["lang"] = keyValue.value();
                    break;
                case KeyName::Ip:
                    if (checkIp(keyValue.value())) params["ip"] = keyValue.value();
                    break;
                case KeyName::Port:
                    if (checkPort(keyValue.value())) params["port"] = keyValue.value();
                    break;
                case KeyName::Path:
                    if (checkPath(keyValue.value())) params["path"] = keyValue.value();
                    break;
                default:
                    //введен неизвестный параметр
                    std::cout << QObject::tr("Non-existent key").toUtf8().constData() << std::endl;
                    break;
             }
         }
    }
}


bool ParamParser::checkIp (const  QString &checkingIpAdress)
{
    QRegExp regExpIp("^(25[0-5]|2[0-4][0-9]|[0-1][0-9]{2}|[0-9]{2}|[0-9])"
                     "(.(25[0-5]|2[0-4][0-9]|[0-1][0-9]{2}|[0-9]{2}|[0-9])){3}$");
    if (!regExpIp.exactMatch(checkingIpAdress))
    {
        std::cout << QObject::tr("Invalid ip-address").toUtf8().constData() << std::endl;
        return false;
    }
    return true;
}

bool ParamParser::checkPort (const QString &checkingPort)
{
    bool ok;
    int port = checkingPort.toInt(&ok, 10);
    if ((ok == false) || (port < 0) || (port > 65535))
    {
        std::cout << QObject::tr("Invalid port number").toUtf8().constData() << std::endl;
        return false;
    }
    return true;
}

bool ParamParser::checkLang (const QString &checkingLang)
{
    return ((checkingLang == "rus") || (checkingLang == "eng"));
}

bool ParamParser::checkPath (const QString &checkingPath)
{

    QRegExp regExpPath("^(/)?[A-Za-z0-9-_][A-Za-z0-9-_.]*((/)[A-Za-z0-9-_][A-Za-z0-9-_.]*)*$");
    if (!regExpPath.exactMatch(checkingPath))
    {
        std::cout << QObject::tr("Invalid folder name").toUtf8().constData() << std::endl;
        return false;
    }
    return true;
}
