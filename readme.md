# PParser - парсер корпоративных проектов

Сервис, позволяющий спарсить проекты в БД, на основании директорий и файлов маркеров.
Работает из под UNIX

### Возможности:
- Поиск по каталогам маркеров;
- Сохранение базовой информации в БД;
- Администрирование, поиск и отображение в веб оболочке информации о проекте;
- Резервирование БД за счет сохранения в .json файлы информации о проекте;
- Восстановление пути в случае переименования\переноса

#### Режим отладки
- Запросы не уходят в БД, вместо этого они отображаются в логах
- Расширенная информация о работе сервиса

### TODO:
- Синхронизация изменения данных в файлах и БД по принципу кто последний тот и прав
- Переделать веб часть с vanilla JS(JQuery)/PHP на ReactJS/Laravel
- Автоматическое создание таблиц в БД

### Конфигурация:
| Параметр       | Тип             | По умолчание                                                                                                                       | Описание                                                    |
|----------------|-----------------|------------------------------------------------------------------------------------------------------------------------------------|-------------------------------------------------------------|
| dbName         | string          | db                                                                                                                                 | Название БД                                                 |
| dbUser         | string          | root                                                                                                                               | Пользователь СУБД                                           |
| dbPass         | string          | root                                                                                                                               | Пароль пользователя СУБД                                    |
| dbUrl          | string          | localhost                                                                                                                          | Адрес БД                                                    |
| debug          | bool            | ```false```                                                                                                                        | Режим отладки                                               |
| delay          | int             | 60                                                                                                                                 | Время задержки до следующей итерации                        |
| markerDirs     | array of string | "Документ",<br> "Фото",<br> "Чертежи",<br> "Программ",<br> "Doc",<br> "Схемы",<br> "Draw",<br> "Prog",<br> "SOFT",<br> "Исходники" | Маркеры директорий                                          |
| markerFiles    | array of string | "ttt.txt",<br> "project.txt",<br> "info_project.txt",<br>"project.json"                                                            | Маркеры файлов                                              |
| maxSearchDepth | int             | 6                                                                                                                                  | Максимальная глубина вложения поиска, чем больше тем дольше |
| realRoot       | string          | //srv-adnt/projects$                                                                                                               | Настоящий сетевой путь                                      |
|  smbRoot       | string          | /media/srv-adnt                                                                                                                    | Путь смонтированный в UNIX систему                          |