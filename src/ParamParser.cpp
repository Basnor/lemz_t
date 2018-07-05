#include "ParamParser.h"

#include <iostream>
#include <iomanip>
#include <ios>

#include <QRegExp>
#include <QDebug>
#include <QDir>

ParamParser::ParamParser(int argc, char *argv[])
{
    keyStringByName.insert(KeyName::Help, "-h");
    keyStringByName.insert(KeyName::Ip, "ip");
    keyStringByName.insert(KeyName::Port, "port");
    keyStringByName.insert(KeyName::Path, "path");
    keyStringByName.insert(KeyName::Lang, "lang");

    params.insert(KeyName::Ip, "127.0.0.1");
    params.insert(KeyName::Port, "10600");
    params.insert(KeyName::Path, "dspRec");
    params.insert(KeyName::Lang, "eng");

    for (auto i=1; i < argc; i++)
    {
        auto keyValuePair = argv[i];
        keyValuePairs.append(keyValuePair);
    }

    parseParams();
    checkParams();
}

QString ParamParser::getLang() const
{
    return params[KeyName::Lang];
}

QString ParamParser::getIp() const
{
    return params[KeyName::Ip];
}

int ParamParser::getPort() const
{
    return params[KeyName::Port].toInt();
}

QString ParamParser::getPath() const
{
    return params[KeyName::Path];
}

bool ParamParser::getHelp () const
{
    if (findHelp())
    {
        std::cout << QObject::tr("Used keys:").toStdString() << std::endl; // Используемые ключи
        std::cout << QObject::tr("lang=[language]").toStdString()
                  << "\t" << QObject::tr("set language (eng or rus)").toStdString() << std::endl;
        std::cout << QObject::tr("ip=[ip-adress]").toStdString()
                  << "\t\t" << QObject::tr("set ip-adress").toStdString() << std::endl;
        std::cout << QObject::tr("port=[port number]").toStdString()
                  << "\t" << QObject::tr("set port number").toStdString() << std::endl;
        std::cout << QObject::tr("path=[folder name]").toStdString()
                  << "\t" << QObject::tr("set folder name").toStdString() << std::endl;
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
        else if  (splitedKeyValue[0] == "-h")
        {
             std::cout <<QObject::tr("Non-existent key").toStdString() << std::endl;
        }
    }
}

bool ParamParser::findHelp() const
{
    return keyValuePairs.contains(keyStringByName[KeyName::Help]);
}

void ParamParser::checkParams()
{      
    for (auto keyValue = splitedParams.begin(); keyValue != splitedParams.end(); ++keyValue)
        for (auto it = keyStringByName.begin(); it != keyStringByName.end(); ++it)
        {
            if (keyValue.key() == it.value())
            {
                switch (it.key())
                {
                case KeyName::Lang:
                    if (checkLang(keyValue.value())) params[it.key()] = keyValue.value();
                    break;
                case KeyName::Ip:
                    if (checkIp(keyValue.value())) params[it.key()] = keyValue.value();
                    break;
                case KeyName::Port:
                    if (checkPort(keyValue.value())) params[it.key()] = keyValue.value();
                    break;
                case KeyName::Path:
                    if (checkPath(keyValue.value())) params[it.key()] = keyValue.value();
                    break;
                default:
                    //введен неизвестный параметр
                    std::cout << QObject::tr("Non-existent key").toStdString() << std::endl;
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
        std::cout << QObject::tr("Invalid ip-address").toStdString() << std::endl;
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
        std::cout << QObject::tr("Invalid port number").toStdString() << std::endl;
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

    /*QRegExp regExpPath("^(/)?[A-Za-z0-9-_][A-Za-z0-9-_.]*((/)[A-Za-z0-9-_][A-Za-z0-9-_.]*)*$");
    if (!regExpPath.exactMatch(checkingPath))
    {
        std::cout << QObject::tr("Invalid folder name").toStdString() << std::endl;
        return false;
    } */

    QDir Path;
    if(Path.mkpath(checkingPath))
    {
        return true;
    }
    else {
        std::cout << QObject::tr("Invalid folder name").toStdString() << std::endl;
        return false;
    }
}
