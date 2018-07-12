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

    bool isParamsCorrect = true;

    //вывод справки при его наличии
    void showHelp() const;
    //получение ip-адреса
    QString getIp() const;
    //получение номера порта
    int getPort() const;
    //получение названия папки
    QString getPath() const;

    //поиск ключа -h
    bool findHelp() const;

private:
    //разбивает введеный запрос на ключ-значение
    bool parseParams();
    //проверяет разбитые значения
    bool checkParams();

    //проверяет Ip-адрес на соответствие общему виду
    static bool checkIp (const QString &checkingIpAdress);
    //проверяет введенный номер порта на соответствие диапазону и символам
    static bool checkPort (const QString &checkingPort);
    //проверяет путь
    static bool checkPath (const QString &checkingPath);

    //вывод сообщений об ошибках
    void showErrorIp() const; //неверный ip-адрес
    void showErrorPath() const; //неверный путь
    void showErrorPort() const; //неверный номер порта
    void showErrorKey() const; //неверное название ключа или значение ключа

    //введенные значения ключей(не разделенные)
    QStringList keyValuePairs;
    //название ключей, используемые для сравнения с введенными значениями
    QMap<KeyName, QString> keyStringByName;
    //соответствия ключ-значение
    QMap<KeyName, QString> params;
    //введенные значения ключей(разделенные)
    QMap<QString, QString> splitedParams;
};


