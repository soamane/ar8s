# ar8s - Удобный инструмент для сервис-бомбинга
**ar8s** — это инструмент, позволяющий выполнять HTTP(-s) запросы в форматах POST и GET к различным интернет-сервисам, что позволяет заспамить конечную цель (номер телефона).  
**ar8s** разрабатывался с целью предоставить пользователям максимально возможные настройки конфигурации. Почти каждый новый инструмент, добавленный в исходный код, может быть настроен пользователем под его потребности.

# Предисловие к документации
Продукт включает в себя два *конфигурационных файла*: первый — ``services.json``, содержащий информацию о ваших сервисах. Во втором конфигурационном файле ``settings.json`` хранятся основные настройки и параметры приложения. Все изменения вносятся исключительно через эти файлы. Программа не поддерживает изменение или ввод данных через свой интерфейс (консоль). В случае неверного формата или опечаток в документах программа сообщит об этом, указав название документа с ошибкой и предоставив информацию о ней.

# Подробнее о ``services.json``
Конфигурационный файл ``services.json`` представляет собой документ формата JSON, основой которого является массив ``services``, содержащий отдельные сервисы для выполнения запросов. Каждый объект имеет одинаковый набор полей, рассмотрим их подробнее.
### Важно: ``services.json`` поддерживает систему плейсхолдеров, о которой рассказывается в следующем пункте.
| Название поля     | Описание                                                                 |
|-------------------|--------------------------------------------------------------------------|
| name              | Имя предоставленного сервиса (используется для отображения в логах)      |
| url               | Основной URL-адрес запроса для сервиса                                   |
| payload           | Передаваемые данные (параметры) при POST запросе **(оставляйте пустым при использовании GET формата)** |
| headers           | Список заголовков запроса                                               |
| request-type      | Тип исполняемого запроса (используйте ``0/1`` для ``GET/POST`` соответственно) |
| protocol-type     | Тип протокола исполняемого запроса (используйте ``0/1`` для ``HTTP/HTTPS`` соответственно) |

# Подробнее о ``settings.json``
Конфигурационный файл ``settings.json`` представляет собой документ формата JSON. В данном документе хранятся основные настройки программы.
| Название поля       | Описание                                                               |
|---------------------|------------------------------------------------------------------------|
| attacks-count       | Количество итераций выполнения                                         |
| loop-timeout        | Задержка после *каждого прохода всех сервисов* для выполнения следующей итерации |
| use-proxy           | Включение/отключение поддержки использования прокси-серверов          |
| proxies             | Массив, включающий в себя отдельные прокси-сервера. В поле ``address`` используйте формат ``address:port``. Оставьте пустыми поля ``username`` и ``password`` если используете публичный прокси |
| use-placeholders    | Включение/отключение поддержки использования плейсхолдеров для сервисов из ``services.json`` |
| placeholders        | Массив, включающий в себя отдельные объекты плейсхолдеров. Поле ``key`` используется для именования вашего плейсхолдера, являясь его ключом. Поле ``value`` хранит данные, на которые будет заменен текущий плейсхолдер |

## Система замены собственных переменных (плейсхолдеры)
### Важно: поддержка плейсхолдеров осуществляется только в конфигурационном файле ``services.json`` в полях ``url`` и ``payload``.
Конфигурационный файл ``services.json`` поддерживает использование пользовательских данных (плейсхолдеров). Этот функционал был разработан для замены стандартного подхода, когда введенный номер телефона заменялся в одном месте, указанном разработчиком.

С этой возможностью вы можете вставлять свои уникальные данные в любое место полей: ``url`` и ``payload``, что предоставляет вам гибкость в создании запросов. Использование плейсхолдеров не ограничено. Вся входная строка данных полей проверяется на наличие указанного плейсхолдера.

Но это не всё! Вы также можете задавать уникальные ключи для плейсхолдеров, вместо использования стандартизированной системы, предложенной разработчиком.

Пример использования плейсхолдера с ключом ``${phone}`` и значением ``value: 88005553535`` в произвольном адресе: ``https://google.com/registr?phone=${phone}``  
Выходные данные после замены: ``https://google.com/registr?phone=88005553535``

# Сборка проекта
## Клонируйте репозиторий в удобное для вас место

```
git clone https://github.com/soamane/ar8s.git
```

## Сборка решения с помощью CMake
### Необходимые библиотеки для компиляции: ``CURL``, ``NLOHMANN-JSON``, а так же ``MyCURL``, обёртка библиотеки CURL написанная мною: https://github.com/soamane/MyCurl
```
> mkdir build
> cd build
> cmake ..
> cmake --build . --config=Release
```



