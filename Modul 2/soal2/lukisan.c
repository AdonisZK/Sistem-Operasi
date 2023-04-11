#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

bool validateArgument(int argc, char* argv[]) {
    if (argc == 2) {
        char mode = argv[1][1];
        if (mode == 'a' || mode == 'b')
            return true;
        else {
            printf("Invalid argument\n");
            return false;
        }
    } else if (argc > 2) {
        printf("Too many arguments!\n");
        return false;
    } else {
        printf("One argument expected!\n");
        return false;
    }
}

long int formatName(char* time_string) {
    time_t current_time = time(NULL);
    struct tm* time_info = localtime(&current_time);
    strftime(time_string, 20, "%Y-%m-%d_%H:%M:%S", time_info);
    return current_time;
}

void makePath(char* path, char* dirname, char* filename) {
    sprintf(path, "%s/%s", dirname, filename);
}

void makeDir(char* dirname) {
    char* args[] = {"mkdir", dirname, NULL};
    execv("/bin/mkdir", args);
}

void downloadPhoto(char* dirname, char* filename, char* path) {
    long int size = (formatName(filename) % 1000) + 50;

    char url[27];
    sprintf(url, "https://picsum.photos/%ld", size);
    makePath(path, dirname, filename);
    char* args[] = {"wget", "-q", "-O", path, url, NULL};

    execv("/usr/bin/wget", args);
    perror("execv");
    exit(EXIT_FAILURE);
}

void zipFolder(char* dirname) {
    char* args[] = {"zip", "-r", dirname, dirname, NULL};
    execv("/usr/bin/zip", args);
}

void removeFolder(char* dirname) {
    char* args[] = {"rm", "-r", dirname, NULL};
    execv("/bin/rm", args);
}

int main(int argc, char* argv[]) {
    if (!validateArgument(argc, argv)) return 0;

    pid_t pid, sid;
    int status;

    pid = fork();
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    umask(0);

    sid = setsid();
    if (sid < 0) {
        exit(EXIT_FAILURE);
    }

    char mode = argv[1][1];
    printf("Mode: %c\n", mode);

    FILE* killerFile = fopen("killer.c", "w");
    char* killerCode = "#include <unistd.h>\n#include <wait.h>\n\nint main() {\n    pid_t child;\n    int status;\n\n    child = fork();\n    if (child == 0) {\n        %s\n        execv(\"%s\", args);\n    }\n    waitpid(child, &status, 0);\n\n    char* args[] = {\"rm\", \"killer\", NULL};\n    execv(\"/bin/rm\", args);\n}";
    char killerCommand[100];
    char* commandPath;

    if (mode == 'a') {
        commandPath = "/bin/pkill";
        sprintf(killerCommand, "char* args[] = {\"pkill\", \"-9\", \"-s\", \"%d\", NULL};\n", sid);
        fprintf(killerFile, killerCode, killerCommand, commandPath);
    } else if (mode == 'b') {
        commandPath = "/bin/kill";
        sprintf(killerCommand, "char* args[] = {\"kill\", \"-9\", \"%d\", NULL};\n", getpid());
        fprintf(killerFile, killerCode, killerCommand, commandPath);
    }

    fclose(killerFile);

    // Compile killer program
    pid = fork();
    if (pid == 0) {
        char* args[] = {"gcc", "-o", "killer", "killer.c", NULL};
        execv("/bin/gcc", args);
    }
    // Tunggu kompilasi kemudian hapus killer.c
    waitpid(pid, &status, 0);

    // Hapus killer.c
    pid = fork();
    if (pid == 0) {
        char* args[] = {"rm", "killer.c", NULL};
        execv("/bin/rm", args);
    }
    waitpid(pid, &status, 0);

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    while (1) {
        char dirname[20], filename[20], path[45];
        pid_t downloadProcess;
        int downloadStatus;

        // Child process untuk download batch foto, interval 30 detik
        downloadProcess = fork();
        if (downloadProcess == 0) {
            formatName(dirname);
            // Proses untuk membuat folder
            downloadProcess = fork();
            if (downloadProcess == 0) {
                makeDir(dirname);
            }
            // Tunggu sampai folder selesai dibuat
            waitpid(downloadProcess, &downloadStatus, 0);

            for (int i = 0; i < 15; i++) {
                // Buat 1 child process untuk download tiap foto, interval 5 detik
                downloadProcess = fork();
                if (downloadProcess == 0) {
                    downloadPhoto(dirname, filename, path);
                }
                sleep(5);
            }
            // Tunggu proses download batch foto selesai (15/15)
            waitpid(downloadProcess, &downloadStatus, 0);

            // Proses zip folder ketika download batch selesai
            downloadProcess = fork();
            if (downloadProcess == 0) {
                zipFolder(dirname);
            }
            // Tunggu proses zip selesai, kemudian hapus folder
            waitpid(downloadProcess, &downloadStatus, 0);
            removeFolder(dirname);
        }
        sleep(30);
    }
}