#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#define PROC_FILE "/proc/proc_module"
#define MODULE_NAME "proc_module"


void test_write_then_read() {
    char *test_data = "Hello from userspace!";
    char read_buffer[256] = {0};
    int fd;
    ssize_t bytes;

    printf("Тест : Запись -> чтение...\n");

    // Открываем proc файл
    fd = open(PROC_FILE, O_RDWR);
    if (fd < 0) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Запись данных
    bytes = write(fd, test_data, strlen(test_data));
    assert(bytes == strlen(test_data));
    printf("  Wrote %zd bytes: '%s'\n", bytes, test_data);

    // Смещаем файловый указатель для чтения
    lseek(fd, 0, SEEK_SET);

    // Читаем данные обратно
    bytes = read(fd, read_buffer, sizeof(read_buffer) - 1);
    assert(bytes == strlen(test_data)); // Проверка совпадения размерности
    printf("  Read %zd bytes: '%s'\n", bytes, read_buffer);

    // Проверка на тождественность данных
    assert(strcmp(test_data, read_buffer) == 0);
    printf("  Данные совпадают! Тест пройден.\n\n");

    close(fd);
}

int main() {
    printf("Запуск тестов для модуля ядра : %s...\n",MODULE_NAME);
    test_write_then_read();
    printf("Тестирование завершено.\n");
    return 0;
}