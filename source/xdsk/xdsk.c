#include <stdio.h>
#include "heap.h"
#include <string.h>
#include "msxdisk.h"



uint8_t *FAT, *buffer;



static uint16_t next_cluster(uint16_t cluster)
{
    uint16_t tmp;

    tmp = cluster / 2;
    tmp = cluster + tmp;

    if (cluster % 2 == 0)
    {
        return ((uint16_t)(FAT[tmp + 1] & 0x0F) << 8) | FAT[tmp];
    }
    else
    {
        return ((uint16_t)FAT[tmp + 1] << 4) | ((FAT[tmp] & 0xF0) >> 4);
    }
}


static void print(char *string)
{
    while (*string)
    {
        putchar(*string);
        string++;
    }
}


static void dir_print_name(dir_t *dir)
{
    uint8_t c;

    for (c = 0; c < 8; c++) putchar(dir->name[c]);
    putchar(' ');
    for ( ; c < 11; c++) putchar(dir->name[c]);
}


int main(char **argv, int argc)
{
    fcb_t fcb1, fcb2;
    bsi_t bsi;
    dir_t dir;
    uint16_t i, j;
    uint16_t FATsize, clusterSize, rest;
    uint32_t dirPos, clustersPos;
    uint16_t numClusters, cluster;

    if (argc != 1)
    {
        print("Use: xdsk <imagen>\r\n");
        return 1;
    }

    memset(&fcb1, 0x00, sizeof(fcb1));
    fcb_set_name(&fcb1, argv[0]);

    if (open(&fcb1) != 0)
    {
        print("Error al abrir el fichero\r\n");
        return 1;
    }

    seek(&fcb1, 0xB);
    read(&fcb1, 1, sizeof(bsi), (char *)&bsi);

    FATsize = bsi.sizeFAT * bsi.sectorSize;
    clusterSize = bsi.clusterSize * bsi.sectorSize;
    dirPos = 0x200 + FATsize * bsi.numFATs;
    clustersPos = dirPos + bsi.numDirs * sizeof(dir_t);

    FAT = (uint8_t *)malloc(FATsize);
    buffer = (uint8_t *)malloc(clusterSize);
    
    seek(&fcb1, 0x200);
    read(&fcb1, 1, FATsize, FAT);

    for (i = 0; i < bsi.numDirs; i++)
    {
        seek(&fcb1, dirPos);
        read(&fcb1, 1, sizeof(dir), (char *)&dir);

        /* FAT12 Entry Values:
           0x000 (Free Cluster)
           0x001 (Reserved Cluster)
           0x002 - 0xFEF (Used cluster; value points to next cluster)
           0xFF0 - 0xFF6 (Reserved values)
           0xFF7 (Bad cluster)
           0xFF8 - 0xFFF (Last cluster in file) */

        /* attribute:
           Bit 0: read only
           Bit 1: hidden
           Bit 2: system file
           Bit 3: volume label
           Bit 4: subdirectory
           Bit 5: archive
           Bit 6: unused
           Bit 7: unused */

        if (dir.topCluster != 0 && // no está libre
            (uint8_t)dir.name[0] != 0xE5 && // no fue borrado
            (dir.attribute & 0b11011000) == 0) // no es etiqueta de volumen,
                                               // ni subdirectorio,
                                               // ni otra cosa
        {
            memset(&fcb2, 0x00, sizeof(fcb2));
            fcb_set_name(&fcb2, dir.name);

            dir_print_name(&dir);
            if (open(&fcb2) == 0)
            {
                char c;

                close(&fcb2);
                print(" - Sobreescribir? (s/n)");
                c = getchar();
                putchar(c);
                if (c != 's' && c != 'S')
                {
                    print("\r\n");
                    dirPos += sizeof(dir);
                    continue;
                }
            }

            if (create(&fcb2) != 0 || open(&fcb2) != 0)
            {
                print("\tError al escribir\r\n");
                return 1;
            }

            numClusters = dir.size / clusterSize;
            cluster = dir.topCluster;
            rest = dir.size % clusterSize;
            for (j = 0; j < numClusters; j++)
            {
                seek(&fcb1, clustersPos + (cluster - 2) * (uint32_t)clusterSize);
                read(&fcb1, 1, clusterSize, buffer);
                write(&fcb2, 1, clusterSize, buffer);
                cluster = next_cluster(cluster);
            }
            seek(&fcb1, clustersPos + (cluster - 2) * (uint32_t)clusterSize);
            read(&fcb1, 1, rest, buffer);
            write(&fcb2, 1, rest, buffer);

            close(&fcb2);

            print("\tOk\r\n");
        }
        dirPos += sizeof(dir);
    }

    close(&fcb1);

    return 0;
}


