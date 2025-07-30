| Сигнал    | Ожидание | Результат |
| --------- | -------- | --------- |
| SIGINT    | Вывод обработки в файле(после 3-его завершение программы) | Успех |
| SIGQUIT   | Вывод обработки в файле| Успех |
| SIGABRT   | - | Успех( signal() настроен на игнорирование сигнала)    |
| SIGKILL   | Завершение программы | Успех|
| SIGTSTP   | - | Успех( signal() настроен на игнорирование сигнала)    |
| SIGSTOP   | - | Успех( signal() настроен на игнорирование сигнала)    |
| SIGCONT   | - | Успех( signal() настроен на игнорирование сигнала)    |

### SIGINT:

Первичная компиляция, запуск в фоне:

![alt text](imgs/image.png)

Обработка сигнала SIGINT:

![alt text](imgs/image-1.png)

Выход из программы:

![alt text](imgs/image-2.png)

![alt text](imgs/image-3.png)

### SIGQUIT:

Обработка сигнала SIGQUIT:

![alt text](imgs/image-4.png)

![alt text](imgs/image-5.png)

### SIGKILL:

Обработка сигнала SIGKILL:

![alt text](imgs/image-6.png)

![alt text](imgs/image-7.png)