# Модуль ядра Linux
## Описание
### Модуль
Модуль при инициализации создаёт папку /proc/os2_module и два файла внутри os2_mod_net и os2_mod_pci. Для обоих файлов определены операции чтения и записи.  

Файл os2_mod_net предназначен для получения структуры net_device. В него пишется название сетевого интерфейса, после чего читается сама структура.  

Файл os2_mod_pci предназначен для получения структуры pci_dev. В него записываются домен, шина и слот, затем читается сама структура.   
### Клиент
Программа для взаимодействия пользователя и модуля. Она принимает на вход указание, какую структуру надо получить, идентификатор структуры: имя для net_device; домен, шину и слот для pci_dev. После того, как данные получены, она выведет поля запрашиваемой структуры, если она найдена, либо сообщит об ошибке.

## Использование
### Пример поиска struct net_device
![Example_1](https://github.com/SuperJaremy/os2/blob/main/examples/example_1.png)
### Пример поиска struct pci_dev
![Example_2](https://github.com/SuperJaremy/os2/blob/main/examples/example_2.png)
