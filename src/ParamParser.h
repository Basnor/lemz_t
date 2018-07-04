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
        Path,
        Lang
    };

    ParamParser(int argc, char *argv[]);

    //вывод справки при его наличии
    bool getHelp();
    //получение ip-адреса
    QString getIp();
    //получение номера порта
    int getPort();
    //получение названия папки
    QString getPath();
    //получение языка
    QString getLang();

private:
    //разбивает введеный запрос на ключ-значение
    void parseParams();
    //проверяет разбитые значения
    void checkParams();
    //поиск ключа -h
    bool findHelp();

    //проверяет Ip-адрес на соответствие общему виду
    static bool checkIp (const QString &checkingIpAdress);
    //проверяет введенный номер порта на соответствие диапазону и символам
    static bool checkPort (const QString &checkingPort);
    //проверяет языковой ключ
    static bool checkLang (const QString &checkingLang);
    //проверяет путь
    static bool checkPath (const QString &checkingPath);

    //введенные значения ключей(не разделенные)
    QList<QString> keyValuePairs;
    //название ключей, используемые для сравнения с введенными значениями
    QMap<KeyName, QString> keyStringByName;
    //соответствия ключ-значение
    QMap<QString, QString> params;
    //введенные значения ключей(разделенные)
    QMap<QString, QString> splitedParams;
};


