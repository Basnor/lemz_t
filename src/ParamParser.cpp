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

    params.insert(KeyName::Ip, "127.0.0.1");
    params.insert(KeyName::Port, "10700");
    params.insert(KeyName::Path, "dspRec");

    for (auto i=1; i < argc; i++)
    {
        auto keyValuePair = argv[i];
        keyValuePairs.append(keyValuePair);
    }

    if (parseParams()) isParamsCorrect = checkParams();
    else isParamsCorrect = false;
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

void ParamParser::showHelp () const
{
    std::cout << QObject::tr("Used keys:").toStdString() << std::endl; // Используемые ключи
    std::cout << QObject::tr("ip=[ip-adress]").toStdString()
              << "\t\t" << QObject::tr("set ip-adress").toStdString() << std::endl;
    std::cout << QObject::tr("port=[port number]").toStdString()
              << "\t" << QObject::tr("set port number").toStdString() << std::endl;
    std::cout << QObject::tr("path=[folder name]").toStdString()
              << "\t" << QObject::tr("set folder name").toStdString() << std::endl;
}

bool ParamParser::parseParams()
{
    for (auto keyValue : keyValuePairs)
    {
        QStringList splitedKeyValue = QString(keyValue).split("=");

        //введеное значение вида ключ=значение
        if (splitedKeyValue.size() == 2)
        {
            splitedParams.insert(splitedKeyValue[0],splitedKeyValue[1]);
        }
        else if (!(splitedKeyValue[0] == "-h"))
        {
             showErrorKey();
             return false;
        }
    }
    return true;
}

bool ParamParser::findHelp() const
{
    return keyValuePairs.contains(keyStringByName[KeyName::Help]);
}

bool ParamParser::checkParams()
{      
    bool isMatch = false;

    for (auto keyValue = splitedParams.begin(); keyValue != splitedParams.end(); ++keyValue)
    {
        isMatch = false;

        for (auto it = keyStringByName.begin(); it != keyStringByName.end(); ++it)
        {
            if (keyValue.key() == it.value())
            {
                isMatch = true;

                switch (it.key())
                {
                case KeyName::Ip:
                    if (checkIp(keyValue.value())) params[it.key()] = keyValue.value();
                    else
                    {
                        showErrorIp();
                        return false;
                    }
                    break;
                case KeyName::Port:
                    if (checkPort(keyValue.value())) params[it.key()] = keyValue.value();
                    else
                    {
                        showErrorPort();
                        return false;
                    }
                    break;
                case KeyName::Path:
                    if (checkPath(keyValue.value())) params[it.key()] = keyValue.value();
                    else
                    {
                        showErrorPath();
                        return false;
                    }
                    break;
                default:
                    return false;
                    break;
                }
            }
        }
        if (!isMatch)//введен неизвестный параметр
        {
            showErrorKey();
            return false;
        }
    }

    return true;
}

void ParamParser::showErrorKey() const
{
    std::cout << QObject::tr("Non-existent key").toStdString() << std::endl;
    showHelp();
}

bool ParamParser::checkIp (const  QString &checkingIpAdress)
{
    QRegExp regExpIp("^(25[0-5]|2[0-4][0-9]|[0-1][0-9]{2}|[0-9]{2}|[0-9])"
                     "(.(25[0-5]|2[0-4][0-9]|[0-1][0-9]{2}|[0-9]{2}|[0-9])){3}$");
    return regExpIp.exactMatch(checkingIpAdress);
}

void ParamParser::showErrorIp() const
{
    std::cout << QObject::tr("Invalid ip-address").toStdString() << std::endl;
}

bool ParamParser::checkPort (const QString &checkingPort)
{
    bool ok;
    int port = checkingPort.toInt(&ok, 10);
    if ((ok == false) || (port < 0) || (port > 65535))
    {
        return false;
    }
    return true;
}

void ParamParser::showErrorPort()  const
{
    std::cout << QObject::tr("Invalid port number").toStdString() << std::endl;
}

bool ParamParser::checkPath (const QString &checkingPath)
{
    QRegExp regExpPath("^(/)?[A-Za-z0-9-_.]+((/)[A-Za-z0-9-_.]+)*$");
    return regExpPath.exactMatch(checkingPath);
}

void ParamParser::showErrorPath() const
{
    std::cout << QObject::tr("Invalid folder name").toStdString() << std::endl;
}
