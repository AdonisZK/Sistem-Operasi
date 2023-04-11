#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

int main()
{
    pid_t child;
    int status;
    char *url = "https://drive.google.com/uc?export=download&id=1oDgj5kSiDO0tlyS7-20uz7t20X3atwrq";
    char *downloadName = "binatang.zip";

    printf("\n\nSOAL 1A\n");

    // Mendownload binatang.zip
    child = fork();
    if (child == 0) 
    {
        char *args[] = {"wget", "-q", "-O", downloadName, url, NULL};
        execv("/bin/wget", args);
    }
    waitpid(child, &status, 0);

    // Melakukan unzip File
    child = fork();
    if (child == 0) 
    {
        char *args[] = {"unzip", downloadName, NULL};
        execv("/bin/unzip", args);
    }
    waitpid(child, &status, 0);

    printf("\n\nSOAL 1B\n");

    // Memilih gambar secara acak
    DIR *dir;
    struct dirent *ent;
    int count = 0;
    char *dir_name = ".";
    char **files = NULL;

    // Membuka direktori
    dir = opendir(dir_name);

    // Menghitung banyaknya gambar format jpg
    while ((ent = readdir(dir)) != NULL) 
    {
        char *extension = strrchr(ent->d_name, '.');
        if (extension != NULL && (strcmp(extension, ".jpg") == 0)) 
        {
            count++;
        }
    }

    files = malloc(sizeof(char *) * count);
    rewinddir(dir);

    int i = 0;
    while ((ent = readdir(dir)) != NULL) 
    {
        char *extension = strrchr(ent->d_name, '.');
        if (extension != NULL && (strcmp(extension, ".jpg") == 0)) 
	{
            files[i] = strdup(ent->d_name);
            i++;
        }
    }

    closedir(dir);

    // Memilih gambar secara random
    if (count > 0) 
    {
        srand(time(NULL));
        int index = rand() % count;
        printf("Melakukan Penjagaan Hewan: %s\n", files[index]);
    }

    // free array of filenames
    for (int i = 0; i < count; i++) {
        free(files[i]);
    }

    free(files);

    printf("\nSOAL 1C\n");

    // Digunakan untuk membuat folder
    char *dirname1 = "HewanDarat";
    char *dirname2 = "HewanAmphibi";
    char *dirname3 = "HewanAir";
    if (mkdir(dirname1, 0777) == -1 || mkdir(dirname2, 0777) == -1 || mkdir(dirname3, 0777) == -1) 
    {
        fprintf(stderr, "Error: %s\n", strerror(errno));
    } 
    else 
    {
        printf("3 Direktori berhasil dibuat\n");
    }

    // Digunakan untuk memindahkan file ke subdirektori
    struct stat st;
    char old_path[300];
    char new_path[300];
    char unique_name[300];
    char target_dir[300];

    // Membuka direktori
    dir = opendir(".");

    // Membaca setiap file yang ada pada direktori
    while ((ent = readdir(dir)) != NULL) 
    {
        sprintf(old_path, "./%s", ent->d_name);

        // Skip file dan direktori tersembunyi
        if (ent->d_type == DT_DIR || ent->d_name[0] == '.') 
	{
            continue;
        }

        // Untuk setiap file yang bernama "_air"
        if (strstr(ent->d_name, "_air") != NULL) 
	{
            sprintf(new_path, "./HewanAir/%s", ent->d_name);

            // Pindahkan ke path baru
            rename(old_path, new_path);
        }

        // Untuk setiap file yang bernama "_darat"
        else if (strstr(ent->d_name, "_darat") != NULL) 
	{
            sprintf(new_path, "./HewanDarat/%s", ent->d_name);

            // Pindahkan ke path baru
            rename(old_path, new_path);
        }

        // Untuk setiap file yang bernama "_amphibi"
        else if (strstr(ent->d_name, "_amphibi") != NULL) 
	{
            sprintf(new_path, "./HewanAmphibi/%s", ent->d_name);

            // Pindahkan ke oath baru
            rename(old_path, new_path);
        }

        else 
	{
            continue;
        }
    }

    printf("\nSOAL 1D\n");

    // Melakukan Zip 3 Direktori
    child = fork();
    if (child == 0) 
    {
        char *args[] = {"zip", "-r", "HewanDarat.zip", "HewanDarat/", NULL};
        execv("/bin/zip", args);
    }
    waitpid(child, &status, 0);

    child = fork();
    if (child == 0) 
    {
        char *args[] = {"zip", "-r", "HewanAir.zip", "HewanAir/", NULL};
        execv("/bin/zip", args);
    }
    waitpid(child, &status, 0);

    child = fork();
    if (child == 0) 
    {
        char *args[] = {"zip", "-r", "HewanAmphibi.zip", "HewanAmphibi/", NULL};
        execv("/bin/zip", args);
    }
    waitpid(child, &status, 0);

    // Menghapus direktori yang tidak ter-zip
    child = fork();
    if (child == 0) 
    {
        char *args[] = {"rm", "-r", "HewanDarat/", NULL};
        execv("/bin/rm", args);
    }
    waitpid(child, &status, 0);

    child = fork();
    if (child == 0) 
    {
        char *args[] = {"rm", "-r", "HewanAir/", NULL};
        execv("/bin/rm", args);
    }
    waitpid(child, &status, 0);

    child = fork();
    if (child == 0) 
    {
        char *args[] = {"rm", "-r", "HewanAmphibi/", NULL};
        execv("/bin/rm", args);
    }
    waitpid(child, &status, 0);

    closedir(dir);
    return 0;
}
