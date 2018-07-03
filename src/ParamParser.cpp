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

    for (auto i=1; i < argc; i++)
    {
        auto keyValuePair = argv[i];
        keyValuePairs.append(keyValuePair);
    }
}


bool ParamParser::findLang (QMap<QString, QString> &params)
{
    for (auto keyValue : keyValuePairs)
    {
        QStringList splitedKeyValue = QString(keyValue).split("=");

        if (splitedKeyValue.size() == 2)
        {
            if ((QString::localeAwareCompare(keyValue, keyStringByName[KeyName::Lang])) && (checkLang(splitedKeyValue[1])))
            {
                 params["lang"]=splitedKeyValue[1];
                 return true;
            }
        }
    }
    return false;
}


bool ParamParser::findHelp ()
{
        for (auto keyValuePair : keyValuePairs)
        {
            if (!QString::localeAwareCompare(keyValuePair, keyStringByName[KeyName::Help]))
            {
                qDebug().noquote() << QObject::tr("Used keys:"); // Используемые ключи
                qDebug().noquote() << QObject::tr("lang=[language]") << "\t" << QObject::tr("set language (eng or rus)");
                qDebug().noquote() << QObject::tr("ip=[ip-adress]") << "\t\t" << QObject::tr("set ip-adress");
                qDebug().noquote() << QObject::tr("port=[port number]") << "\t" << QObject::tr("set port number");
                qDebug().noquote() << QObject::tr("path=[folder name]") << "\t" << QObject::tr("set folder name");

                return false;
            }
        }
    return true;
}

bool ParamParser::parseParams (QMap<QString, QString> &params)
{
    for (auto keyValue : keyValuePairs)
    {
        QStringList splitedKeyValue = QString(keyValue).split("=");

        //введеное значение вида ключ=значение
        if (splitedKeyValue.size() == 2)
        {
            bool isKeyMatch = false;

            for (auto it = keyStringByName.begin(); it != keyStringByName.end(); ++it)
            {
                if (!QString::localeAwareCompare(splitedKeyValue[0], it.value()))
                {
                    bool isKeyCorrect = false;
                    switch (it.key())
                    {
                    case KeyName::Lang:
                        isKeyCorrect = true;
                        break;
                    case KeyName::Ip:
                        isKeyCorrect = checkIp(splitedKeyValue[1]);
                        break;
                    case KeyName::Port:
                        isKeyCorrect = checkPort(splitedKeyValue[1]);
                        break;
                    case KeyName::Path:
                        isKeyCorrect = true;
                        break;
                    default:
                        break;
                    }

                    if (isKeyCorrect)
                    {
                        params[it.value()]=splitedKeyValue[1];
                        //введенный ключ существует
                        isKeyMatch = true;
                    }
                    else return false;
                }
            }
            if (!isKeyMatch)
            {
                qDebug().noquote() << QObject::tr("Non-existent key");
                return false;
            }
        }
        else
        {
            qDebug().noquote() << QObject::tr("Non-existent key");
            return false;
        }
    }
    return true;
}


bool ParamParser::checkIp (const  QString &checkingIpAdress)
{
    QRegExp regExpIp("^(25[0-5]|2[0-4][0-9]|[0-1][0-9]{2}|[0-9]{2}|[0-9])"
                     "(.(25[0-5]|2[0-4][0-9]|[0-1][0-9]{2}|[0-9]{2}|[0-9])){3}$");
    if (!regExpIp.exactMatch(checkingIpAdress))
    {
        qDebug().noquote() << QObject::tr("Invalid ip-address");
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
        qDebug().noquote() << QObject::tr("Invalid port number");
        return false;
    }
    return true;
}

bool ParamParser::checkLang (const QString &checkingLang)
{
    if ((checkingLang == "rus") || (checkingLang == "eng")) return true;
    else return false;
}
