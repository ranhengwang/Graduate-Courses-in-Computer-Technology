#include "bitmap.h"
#include "../include/debug/macros.h"
#include "../fs/super.h"
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>

int init_bitmap()
{
    int fd = open_virt_file(O_RDWR);
    if (fd < 0) return 1;

    struct SuperBlock sb;
    read_super_block(&sb);
    pthread_mutex_lock(&bitmap_mutex);
    // 位图: 64 位
    uint8_t *bitmap_array = new uint8_t[sb.bitmap_size];
    memset(bitmap_array, 0, sizeof(*bitmap_array) * sb.bitmap_size);
    bitmap_array[BITMAP_SIZE - 1] = 1;
    write_bitmap(&sb, bitmap_array, sb.bitmap_size);
    pthread_mutex_unlock(&bitmap_mutex);
    delete[] bitmap_array;
    bitmap_array = NULL;
    close(fd);

    return 0;
}

int read_bitmap(const struct SuperBlock *sb_ptr, uint8_t *bitmap_ptr)
{
    int fd = open_virt_file(O_RDONLY);
    if (fd < 0)
    {
        return 1;
    }

    if (bitmap_ptr == NULL)
    {
        ERROR("%s", "bitmap_ptr is NULL");
        return 2;
    }
    
    int bitmap_size = sb_ptr->bitmap_size;

    size_t offset = sb_ptr->super_block_size + sb_ptr->inode_size * sb_ptr->inode_count;
    lseek(fd, offset, SEEK_SET);
    read(fd, bitmap_ptr, bitmap_size);
    close(fd);

    return 0;
}

int write_bitmap(const struct SuperBlock *sb_ptr, void *bitmap_ptr, int size)
{
    int fd = open_virt_file(O_RDWR);
    if (fd < 0)
    {
        ERROR("%s", "fd < 0");
        return 1;
    }

    if (bitmap_ptr == NULL)
    {
        ERROR("%s", "bitmap_ptr == NULL");
        return 2;
    }

    size_t offset = sb_ptr->super_block_size
        + sb_ptr->inode_size * sb_ptr->inode_count;
    lseek(fd, offset, SEEK_SET);
    write(fd, bitmap_ptr, size);
    close(fd);

    return 0;
}

int is_valid_inode(const struct SuperBlock *sb_ptr, uint8_t *bitmap_ptr, int num)
{
    if (bitmap_ptr == NULL)
    {
        ERROR("%s", "bitmap_ptr is NULL");
        return -1;
    }

    if (num <= 0 || num >= sb_ptr->inode_count)
    {
        ERROR("%s", "num is unavailable");
        return -2;
    }

    int bitmap_size = sb_ptr->bitmap_size;
    int column_size = 8;

    int second_offset = num / column_size;
    int first_offset = num % column_size;

    if ((bitmap_ptr[bitmap_size - second_offset - 1] & (1 << first_offset)) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int delete_bitmap(const struct SuperBlock *sb_ptr, uint8_t *bitmap_ptr, int num)
{
    int fd = open_virt_file(O_RDWR);
    if (fd < 0) return 1;
    if (sb_ptr == NULL) return 2;
    if (bitmap_ptr == NULL) return 3;
    if (num <= 0 || num >= sb_ptr->bitmap_bit_count) return 0;

    int first_offset = num % 8;
    int second_offset = num / 8;

    uint8_t flag = (1 << first_offset);
    flag = ~flag;
    bitmap_ptr[sb_ptr->bitmap_size - second_offset - 1] &= flag;
    ERROR("bitmap_ptr[%d]: %d", second_offset, bitmap_ptr[sb_ptr->bitmap_size - second_offset - 1]);
    write_bitmap(sb_ptr, (void *)bitmap_ptr, sb_ptr->bitmap_size);
    close(fd);

    return 0;
}

int print_bitmap_array(const uint8_t *bitmap_array, uint size)
{
    if (bitmap_array == NULL)
    {
        ERROR("%s", "bitmap_array is NULL");
        return 1;
    }
    
    printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> bitmap >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
    for (int i = size - 1; i >= 0; --i)
    {
        printf("%-8d", bitmap_array[i]);
    }
    printf("\n");
    printf("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< bitmap <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
    
    return 0;
}


