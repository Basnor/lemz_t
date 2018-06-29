#pragma once

#include <QObject>
#include <QMap>
#include <QList>


/* ParamPaeser class
 * Предназначен для определения вызова при вводе ключа -h и параметров: id, port и path.
 * Класс осуществляет проверку вводимых значений.
 */

class ParamParser
{
public:
    //имеющиеся ключи
    enum class KeyName
    {
        Help,
        Ip,
        Port,
        Path
    };

    ParamParser(int argc, char *argv[]);

    //поиск ключа -h и вывод справки при его наличии
    bool findHelp();
    //разбивает введеный запрос на ключ-значение и обновляет значения исходных параметров
    bool parseParams(QMap<QString, QString> &params);

private:
    //проверяет Ip-адрес на соответствие общему виду
    bool checkIp (QString checkingIpAdress) const;
    //проверяет введенный номер порта на соответствие диапазону и символам
    bool checkPort (QString checkingPort) const;

    //введенные значения ключей
    QList<QString> keyValuePairs;
    QMap<KeyName, QString> keyStringByName;
};


