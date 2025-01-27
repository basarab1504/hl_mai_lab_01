# Лабораторная работа №1 #

Выполнил: Петрушин Влад, М8О-103М-20

## Сборка проекта ##

```bash
cmake configure .
cmake .
cmake --build ./
```

## Настройка базы данных ##

1. create_db.sql
2. gen_data.sql

Выполнение скрипта из файла:

```bash
source <имя файла>.sql;
```

## Запуск сервера ##

Для запуска сервера следует выполнить команду:

```bash
sudo sh ./start.sh
```

Сервер работает на порту 8080.

## Тестирование с помощью gtest ##

Запуск модульных тестов:

```bash
./gtests
```

## Тестирование с помощью wrk ##

Нагрузочное тестирование производилось для 1, 2, 6 и 10 потоков при 50 подключениях в течение 30 секунд. Полученные данные (Requests/sec - количество запросов в секунду, Latency(ms) - задержка в миллисекундах):

Threads | Requests/sec | Latency(ms)
---     | ---          | ---
1       | 1.00k        | 22.70
2       | 638.59       | 15.09
6       | 218.50       | 12.01
10      | 132.12       | 12.67
