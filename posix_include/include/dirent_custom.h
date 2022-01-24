#ifndef __DIRENT_CUSTOM_H__
#define __DIRENT_CUSTOM_H__

#include "file_api.h"

#if __cplusplus
extern "C"{
#endif

#define PATH_MAX (256)

typedef void DIR;

struct dirent {
    /* Always zero */
    long d_ino;

    /* File position within stream */
    long d_off;

    /* Structure size */
    unsigned short d_reclen;

    /* Length of name without \0 */
    size_t d_namlen;

    /* File type */
    int d_type;

    /* File name */
    char d_name[PATH_MAX+1];
};
typedef struct dirent dirent;

#if __cplusplus
}
#endif

#endif
