#include "ParamParser.h"

#include <iostream>
#include <iomanip>
#include <ios>

#include <QRegExp>

ParamParser::ParamParser(int argc, char *argv[])
{
    keyStringByName.insert(KeyName::Help, "-h");
    keyStringByName.insert(KeyName::Ip, "ip");
    keyStringByName.insert(KeyName::Port, "port");
    keyStringByName.insert(KeyName::Path, "path");

    for (auto i=1; i < argc; i++)
    {
        auto keyValuePair = argv[i];
        keyValuePairs.append(keyValuePair);
    }
}

bool ParamParser::findHelp ()
{
        for (auto keyValuePair : keyValuePairs)
        {
            if (!QString::localeAwareCompare(keyValuePair, keyStringByName[KeyName::Help]))
            {
                std::cout << "Определено использование ключей:" << std::endl;
                std::cout << "ip=[ip-адрес]\t\tзадать ip-адрес" << std::endl;
                std::cout << "port=[номер порта]\tзадать номер порта" << std::endl;
                std::cout << "path=[имя папки]\tзадать именя папки с результатами" << std::endl;

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

            for (QMap<KeyName,QString>::iterator it = keyStringByName.begin();
                 it != keyStringByName.end(); ++it)
            {
                if (!QString::localeAwareCompare(splitedKeyValue[0], it.value()))
                {
                    bool isKeyCorrect;
                    switch (it.key())
                    {
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

                    // отслеживание разделённых введённых ключей
                    std::cout << splitedKeyValue[0].toUtf8().constData() << std::endl;
                    std::cout << splitedKeyValue[1].toUtf8().constData() << std::endl;
                }
            }
            if (!isKeyMatch)
            {
                std::cout << "Несуществующий ключ" << std::endl;
                return false;
            }
        }
        else
        {
            std::cout << "Несуществующий ключ" << std::endl;
            return false;
        }
    }

    return true;
}


bool ParamParser::checkIp (QString checkingIpAdress) const
{
    QRegExp regExpIp("^(25[0-5]|2[0-4][0-9]|[0-1][0-9]{2}|[0-9]{2}|[0-9])"
                     "(.(25[0-5]|2[0-4][0-9]|[0-1][0-9]{2}|[0-9]{2}|[0-9])){3}$");
    if (!regExpIp.exactMatch(checkingIpAdress))
    {
        std::cout << "Неверный ip-адрес" << std::endl;
        return false;
    }
    return true;
}

bool ParamParser::checkPort (QString checkingPort) const
{
    bool ok;
    checkingPort.toInt(&ok, 10);
    if ((checkingPort.toInt(&ok, 10) < 0) || (checkingPort.toInt(&ok, 10) > 65535) || (ok == false))
    {
        std::cout << "Неверный номер порта" << std::endl;
        return false;
    }
    return true;
}
