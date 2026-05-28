#include <alloca.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>

#define STB_IMAGE_IMPLEMENTATION
#include "include/stb_image.h"

typedef struct {
    unsigned char* pixels;
    int width;
    int height;
    int channels;
    char filename[256];
} ImageInfo;

int isPng(const char *filename)
{
    size_t len = strlen(filename);
    if (len < 5) return 0;
    return strcmp(filename + len - 4, ".jpg") == 0;
}

int main()
{
    const char* dirPath = "../photos/jpeg-small";
    DIR *dir = opendir(dirPath);

    if (dir == NULL) {
        perror("nahuy idi");
        return 1;
    }

    int capacity = 4;
    int imageCount = 0;

    ImageInfo *images = malloc(capacity * sizeof(ImageInfo));
    if (images == NULL) {
        perror("idi nahuy [malloc]");
        closedir(dir);
        return 1;
    }

    char fullpath[512];
    struct dirent *entry;

    printf("Сканируем папку '%s'...\n", dirPath);
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG && isPng(entry->d_name)) {
            snprintf(fullpath, sizeof(fullpath), "%s/%s", dirPath, entry->d_name);
            printf("naydena PNG kartinka %s, zagrujaem...\n", fullpath);

            if (imageCount >= capacity) {
                capacity *= 2;
                ImageInfo *temp = realloc(images, capacity * sizeof(ImageInfo));
                if (temp == NULL) {
                    printf("idi nahuy [realloc]");
                    break;
                }
                images = temp;
            }
            int w, h, ch;
            unsigned char *data = stbi_load(fullpath, &w, &h, &ch, 0);
            if (data != NULL) {
                images[imageCount].pixels = data;
                images[imageCount].width = w;
                images[imageCount].height = h;
                images[imageCount].channels = ch;
                // images[imageCount].filename
                strncpy(images[imageCount].filename, entry->d_name, sizeof(images[imageCount].filename));

                imageCount++;
            } else {
                printf("ne udalos zagruzit pixeli\n");
            }
        }
    }
    closedir(dir);

    for (size_t i = 0; i < imageCount; i++) {
        printf("%ld | w: %d, h: %d, ch: %d \t filename: %s\n", i, images[i].width, images[i].height, images[i].channels, images[i].filename);
    }

    for (int i = 0; i < imageCount; i++) {
            stbi_image_free(images[i].pixels);
        }

    free(images);

    return 0;
}
