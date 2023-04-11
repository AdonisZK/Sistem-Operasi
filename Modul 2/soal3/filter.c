#include <dirent.h>
#include <limits.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void buatTim(int n_bek, int n_gelandang, int n_penyerang);

typedef struct {
    char name[50];
    char team[20];
    char position[10];
    int rating;
} Player;

int comparePlayers(const void* P1, const void* P2) {
    const Player* player1 = (const Player*)P1;
    const Player* player2 = (const Player*)P2;

    if (player1->rating < player2->rating) {
        return 1;
    } else if (player1->rating > player2->rating) {
        return -1;
    } else {
        return 0;
    }
}

int main() {
    pid_t child;
    int status, gc, ggc;
    char* url = "https://drive.google.com/uc?id=1zEAneJ1-0sOgt13R1gL4i1ONWfKAtwBF&export=download";
    char* downloadName = "players.zip";
    const int n_position = 4;
    char posisi[4][10] = {"Kiper", "Bek", "Gelandang", "Penyerang"};
    int count_players[4] = {0};

    child = fork();
    if (child == 0) {
        char* args[] = {"wget", "-O", downloadName, url, NULL};
        execv("/bin/wget", args);
    }
    waitpid(child, &status, 0);

    child = fork();
    if (child == 0) {
        char* args[] = {"unzip", downloadName, NULL};
        execv("/bin/unzip", args);
    }
    waitpid(child, &status, 0);

    child = fork();
    if (child == 0) {
        char* args[] = {"rm", downloadName, NULL};
        execv("/bin/rm", args);
    }
    waitpid(child, &status, 0);

    char path[PATH_MAX];
    getcwd(path, sizeof(path));

    // printf("%s\n", path);
    strcat(path, "/players");
    // printf("%s\n", path);

    DIR* dir;
    struct dirent* entry;
    dir = opendir(path);
    if (dir != NULL) {
        // Hapus pemain bukan MU
        while ((entry = readdir(dir)) != NULL) {
            // Skip file pemain MU
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 || strstr(entry->d_name, "ManUtd")) {
                continue;
            }

            char deletePath[5000];
            snprintf(deletePath, sizeof(deletePath), "%s/%s", path, entry->d_name);

            child = fork();
            if (child == 0) {
                char* args[] = {"rm", deletePath, NULL};
                execv("/bin/rm", args);
            }
            // waitpid(child, &status, 0);
        }
        closedir(dir);
    }

    for (int i = 0; i < 4; i++) {
        char dirname[5000];
        snprintf(dirname, sizeof(dirname), "%s/%s", path, posisi[i]);

        // Proses filter pemain tiap posisi
        if ((child = fork()) == 0) {
            // Proses untuk membuat folder kategori pemain
            pid_t grandchild_pid = fork();
            if (grandchild_pid == 0) {
                char* args[] = {"mkdir", dirname, NULL};
                execv("/bin/mkdir", args);
            }
            waitpid(grandchild_pid, &gc, 0);

            dir = opendir(path);
            if (dir != NULL) {
                while ((entry = readdir(dir)) != NULL) {
                    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                        continue;
                    }

                    if (strstr(entry->d_name, posisi[i]) != NULL) {
                        // count_players[i]++;
                        // printf("%s : %d\n", posisi[i], count_players[i]);
                        char playerPath[5000];
                        snprintf(playerPath, sizeof(playerPath), "%s/%s", path, entry->d_name);

                        pid_t great_grandchild_pid = fork();
                        if (great_grandchild_pid == 0) {
                            char* args[] = {"mv", playerPath, dirname, NULL};
                            execv("/bin/mv", args);
                        }
                        waitpid(great_grandchild_pid, &ggc, 0);
                    }
                }
                closedir(dir);
            }
            exit(0);
        }
    }

    while (wait(NULL) > 0)
        ;
    printf("Berhasil dikategorikan\n");

    // Hitung jumlah pemain tiap posisi
    dir = opendir(path);
    if (dir != NULL) {
        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                for (int i = 0; i < n_position; i++) {
                    char subdir_path[5000];
                    if (strcmp(entry->d_name, posisi[i]) == 0) {
                        // construct the full path to the subdirectory
                        snprintf(subdir_path, sizeof(subdir_path), "%s/%s", path, entry->d_name);

                        // open the subdirectory and count the number of files inside
                        DIR* subdir = opendir(subdir_path);
                        if (subdir == NULL) {
                            perror("opendir failed");
                            return 1;
                        }

                        struct dirent* subentry;
                        while ((subentry = readdir(subdir)) != NULL) {
                            if (subentry->d_type == DT_REG) {  // count regular files
                                // printf("%s", subentry->d_name);
                                count_players[i]++;
                            }
                        }
                        closedir(subdir);  // close the subdirectory
                    }
                }
            }
        }
        closedir(dir);
    }

    const int kp = 1;
    for (int bek = 1; bek < count_players[1]; bek++) {
        for (int gel = 1; gel <= count_players[2]; gel++) {
            int pny = 11 - kp - bek - gel;
            if (pny > 0 && pny <= count_players[3]) {
                buatTim(bek, gel, pny);
            }
        }
    }
}

void buatTim(int n_bek, int n_gelandang, int n_penyerang) {
    char path[PATH_MAX];
    getcwd(path, sizeof(path));
    strcat(path, "/players");

    // Ganti username
    char* username = "aurelioklv";
    struct passwd* pw;
    char destpath[256];
    char filename[PATH_MAX];

    // extern char posisi[4][10];
    char posisi[4][10] = {"Kiper", "Bek", "Gelandang", "Penyerang"};
    const int n_kiper = 1;
    int n_selected[4] = {n_kiper, n_bek, n_gelandang, n_penyerang};
    int players_count[4] = {0};
    Player players_bek[50], players_gelandang[50], players_penyerang[50], players_kiper[50];
    Player* players_position[4] = {players_kiper, players_bek, players_gelandang, players_penyerang};
    Player players_selected[11];

    pid_t child;
    int status;

    pw = getpwnam(username);
    if (pw == NULL) {
        fprintf(stderr, "Error: could not find user %s\n", username);
    }

    // Buat tiap file pemain menjadi objek pemain lalu sortir berdasarkan rating
    int selected = 0;
    for (int i = 0; i < 4; i++) {
        char position_path[5000];
        sprintf(position_path, "%s/%s", path, posisi[i]);

        DIR* dir;
        struct dirent* entry;
        dir = opendir(position_path);
        if (dir != NULL) {
            int player_index = 0;
            while ((entry = readdir(dir)) != NULL) {
                if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                    continue;
                }
                char p_name[50], p_team[20], p_position[10];
                int p_rating;

                sscanf(entry->d_name, "%[^_]_%[^_]_%[^_]_%d.png", p_name, p_team, p_position, &p_rating);

                Player p;
                strcpy(p.name, p_name);
                strcpy(p.team, p_team);
                strcpy(p.position, p_position);
                p.rating = p_rating;

                players_position[i][player_index] = p;
                player_index++;
            }
            players_count[i] = player_index;
        } else {
            printf("Dir null\n");
        }
        closedir(dir);
        // Sortir pemain tiap posisi berdasarkan rating
        qsort(players_position[i], players_count[i], sizeof(Player), comparePlayers);

        // Pilih player terbaik dengan jumlah yang telah ditentunkan (n_bek dkk)
        for (int j = 0; j < n_selected[i]; j++) {
            players_selected[selected] = players_position[i][j];
            selected++;
        }
    }

    sprintf(filename, "Formasi_%d-%d-%d.txt", n_bek, n_gelandang, n_penyerang);
    FILE* formasi = fopen(filename, "w");

    // Tulis informasi formasi pada file
    for (int i = 0; i < 11; i++) {
        fprintf(formasi, "%s_%s_%s_%d.png\n", players_selected[i].name, players_selected[i].team, players_selected[i].position, players_selected[i].rating);
    }
    fclose(formasi);

    // Pindah file ke /home/[users]/
    child = fork();
    if (child == 0) {
        char* args[] = {"mv", filename, pw->pw_dir, NULL};
        execv("/bin/mv", args);
    }
    waitpid(child, NULL, 0);
}