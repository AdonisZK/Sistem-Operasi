#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

void exe_path(const char *program_path)
{
    pid_t pid = fork();
    // Child process
    if (pid == 0)
    {
        execl(program_path, program_path, NULL);
        exit(0);
    }
    // Parent process
    else if (pid > 0)
    {
        waitpid(pid, NULL, 0);
    }
}

int main(int argc, char *argv[])
{
    pid_t pid, sid;
    int fd, hh, mm, ss, ret;
    time_t now;
    struct tm time_info;
    const char *program_path;

    // Mengecek jumlah argumen yang diberikan
    if (argc != 5)
    {
        fprintf(stderr, "Error: ./mainan hh mm ss program_path\n");
        exit(EXIT_FAILURE);
    }

    // Parsing * dan kesesuaian format jam, menit, detik
    if (strcmp(argv[1], "*") == 0)
    {
        hh = -1;
    }
    else
    {
        ret = sscanf(argv[1], "%d", &hh);
        if (ret != 1 || hh < 0 || hh > 23)
        {
            fprintf(stderr, "Invalid hh argument: %s\n", argv[1]);
            exit(EXIT_FAILURE);
        }
    }
    if (strcmp(argv[2], "*") == 0)
    {
        mm = -1;
    }
    else
    {
        ret = sscanf(argv[2], "%d", &mm);
        if (ret != 1 || mm < 0 || mm > 59)
        {
            fprintf(stderr, "Invalid mm argument: %s\n", argv[2]);
            exit(EXIT_FAILURE);
        }
    }
    if (strcmp(argv[3], "*") == 0)
    {
        ss = -1;
    }
    else
    {
        ret = sscanf(argv[3], "%d", &ss);
        if (ret != 1 || ss < 0 || ss > 59)
        {
            fprintf(stderr, "Invalid ss argument: %s\n", argv[3]);
            exit(EXIT_FAILURE);
        }
    }

    // Mengambil argumen program path
    program_path = argv[4];

    // Forking parent process
    pid = fork();
    if (pid < 0)
    {
        exit(EXIT_FAILURE);
    }
    if (pid > 0)
    {
        exit(EXIT_SUCCESS);
    }

    umask(0);

    // Membuat SID untuk child process
    sid = setsid();
    if (sid < 0)
    {
        exit(EXIT_FAILURE);
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    //  Catat log dalam mode append
    FILE *fp = fopen("/home/adonis/adonis/mydaemon.log", "a");
    if (fp == NULL)
    {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        // Mendapatkan jam sistem
        now = time(NULL);
        localtime_r(&now, &time_info);

        // Mengecek kesuaian jam system dan jam yang diberikan argumen
        if ((hh == -1 || hh == time_info.tm_hour) &&
            (mm == -1 || mm == time_info.tm_min) &&
            (ss == -1 || ss == time_info.tm_sec))
        {
            // Memanggil fungsi exe_path
            exe_path(program_path);
            // log jika eksekusi berhasil dan catat waktunya
            dprintf(fd, "Executed program at %s", ctime(&now));
        }

        // Sleep untuk mengecek tiap detik
        sleep(1);
    }

    // Tutup file log
    fclose(fp);

    return EXIT_SUCCESS;
}
