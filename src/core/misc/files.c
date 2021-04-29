#define _GNU_SOURCE
#include "misc/files.h"
#include <dirent.h>
#include <string.h>
#include <stdlib.h>

char *last_file_in_folder(char folder_path[])
{
    struct dirent **files;
    int n = scandir(folder_path, &files, 0, versionsort);
    if (n==-1)
        return NULL;

    for (int i = 0; i < n - 1; i++)
    {
        free(files[i]);
    }

    char *last_filen_name = calloc(1, NAME_MAX);
    strncpy(last_filen_name, files[n - 1]->d_name, NAME_MAX);
    free(files[n - 1]);
    return last_filen_name;
}