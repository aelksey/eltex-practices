#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

#define DEVICE_PATH "/dev/my_chardev"
#define MODULE_NAME "chardev_module"

int test_write_then_read() {
    int fd;
    char write_buffer[] = "Hello from userspace!";
    char read_buffer[1024] = {0};
    ssize_t bytes;
    off_t position;

    fd = open(DEVICE_PATH, O_RDWR);
    if (fd < 0) {
        perror("Ошибка открытия устройства...");
        return EXIT_FAILURE;
    }

    printf("Запись в устройство...\n");
    bytes = write(fd, write_buffer, strlen(write_buffer));
    if (bytes < 0) {
        perror("Не удалось записать в устройство");
        close(fd);
        return EXIT_FAILURE;
    }
    
    assert(bytes == strlen(write_buffer));
    printf("Записано %zd байт: %s\n", bytes, write_buffer);

    // Check current position
    position = lseek(fd, 0, SEEK_CUR);

    // Reset to beginning
    position = lseek(fd, 0, SEEK_SET);

    printf("Чтение из устройства...\n");
    bytes = read(fd, read_buffer, sizeof(read_buffer) - 1);
    if (bytes < 0) {
        perror("Не удалось прочитать из устройтсва");
        close(fd);
        return EXIT_FAILURE;
    }
    
    if (bytes == 0) {
        printf("Ошибка чтения буфера(пустой буфер)\n");
    } else {
        read_buffer[bytes] = '\0';
        assert(bytes == strlen(read_buffer));
        printf("Прочитано %zd байт: %s\n", bytes, read_buffer);
    }

    assert(strcmp(write_buffer, read_buffer) == 0);
    printf("Данные совпадают! Тест пройден.\n\n");

    close(fd);
    return EXIT_SUCCESS;
}

int main() {
    printf("Запуск тестов для модуля ядра : %s...\n",MODULE_NAME);
    if(test_write_then_read()) return EXIT_FAILURE;
    printf("Тестирование завершено.\n");
    return EXIT_SUCCESS;
}