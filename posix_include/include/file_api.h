#ifndef __FILE_API_H__
#define __FILE_API_H__

#include <stdint.h>
#include <time.h>

#define FILE_PATH_MAX_LEN (64)

#if __cplusplus
extern "C"{
#endif

typedef void* DIR_HANDLE;

enum file_whence_e
{
    FILE_SEEK_SET,
    FILE_SEEK_CUR,
    FILE_SEEK_END,
};

enum file_mode_e
{
    FILE_MODE_NONE = 0,

    FILE_MODE_LINK = 0x1,
    FILE_MODE_REG = 0x2,
    FILE_MODE_DIR = 0x4,
    FILE_MODE_CHR = 0x8,
    FILE_MODE_BLK = 0x10,
    FILE_MODE_SOCK = 0x20,
};

enum dir_access_mode_e
{
    DIR_MODE_R = 0x1,
    DIR_MODE_W = 0x2,
    DIR_MODE_X = 0x4,
};

#ifndef F_OK
# define F_OK 0
# define X_OK 1
# define W_OK 2
# define R_OK 4
#endif

struct file_stat {
    enum file_mode_e st_mode;       //file mode - link/file/dir/char dev/blk dev/socket
    uint32_t    st_uid;        //user id
    uint32_t    st_gid;        //group id
    uint32_t    st_size;       //file size
    time_t      st_atime;      //file access time
    time_t      st_mtime;      //file modify time
    time_t      st_ctime;      //file create time
    char        st_name[FILE_PATH_MAX_LEN];     //file name
};

struct directory_entry
{
    long d_off;			        /* offset to this dirent */
    char d_name[FILE_PATH_MAX_LEN + 1];	/* file name (null-terminated) */
    uint32_t d_type;			/* type of this record */
    void *reserved;
};

#ifndef FILE_ISLNK
    #define FILE_ISLNK(st_mode) ((st_mode) == FILE_MODE_LINK)
#endif

#ifndef FILE_ISREG
    #define FILE_ISREG(st_mode) ((st_mode) == FILE_MODE_REG)
#endif

#ifndef FILE_ISDIR
    #define FILE_ISDIR(st_mode) ((st_mode) == FILE_MODE_DIR)
#endif

#ifndef FILE_ISCHR
    #define FILE_ISCHR(st_mode) ((st_mode) == FILE_MODE_CHR)  
#endif

#ifndef FILE_ISBLK
    #define FILE_ISBLK(st_mode) ((st_mode) == FILE_MODE_BLK)
#endif

#ifndef FILE_ISSOCK
    #define FILE_ISSOCK(st_mode) ((st_mode) == FILE_MODE_SOCK)
#endif

/*
 * filesystem format
 *
 * return - 0 : success
 *          -1: failure
 */
int8_t filesystem_format(char *path,int32_t format);

/*
 * filesystem mount
 *
 * return - 0 : success
 *          -1: failure
 */
int8_t filesystem_mount(char *mount_path);

/*
 * filesystem unmount
 *
 * return - 0 : success
 *          -1: failure
 */
int8_t filesystem_unmount(char *mount_path);

/*
 * filesystem sync
 *
 * return - 0 : success
 *          -1: failure
 */
int8_t filesystem_sync(char *path);

/*
 * file open
 *
 * path - file path
 * mode - "r"/"rb"   : read only,file must exist
 *        "r+"/"rb+" : r/w,file must exist
 *        "rw"       : r/w
 *        "w"/"wb"   : write only,truncate to zero
 *        "w+"/"wb+" : r/w,truncate to zero
 *        "a"/"ab"   : write only,append write
 *        "a+"/"ab+" : r/w,append write
 *
 * return - fd : -1    (failure)
 *          fd : other (success)
 *
 */
int32_t file_open(char *path,char *mode);

#define FILE_WRITE_PERM (0)

/*
 * file_fcntl
 *
 * fd        : fd
 * perm_type : permission type (FILE_WRITE_PERM)
 * enable    : 1 - enable
 *             0 - disable
 *
 * return    : 0 - ok
 *            -1 - fail
 *
 */
int8_t file_fcntl(int fd, int perm_type, int8_t enalbe);

/*
 * file write
 *
 * return - actual write len
 */
uint32_t file_write(int32_t fd,uint8_t *buf,uint32_t len);

/*
 * file read
 * 
 * return - actual read len
 */
uint32_t file_read(int32_t fd,uint8_t *buf,uint32_t len);

/*
 * file gets
 *
 */
char* file_gets(int32_t fd,uint8_t *buf,uint32_t buf_size);


int32_t creat_file_fastseek_map(int32_t fd, uint32_t * buf, uint32_t size);
/*
 * file seek
 *
 * return - offset from file header
 */
uint32_t file_seek(int32_t fd,int32_t offset, enum file_whence_e whence);

/*
 * file stat
 * 
 * return - 0 : success
 *          -1: failure
 */
int8_t file_stat(int32_t fd,struct file_stat *stat);

/*
 * file stat
 * 
 * return - 0 : success
 *          -1: failure
 */
int8_t file_truncate(int32_t fd,uint32_t new_size);

/*
 * file flush
 *
 * return - 0 : sucess
 *          -1: failure
 */
int8_t file_flush(int fd);

/*
 * file close
 *
 * return - 0 : success
 *          -1: failure
 */
int8_t file_close(int32_t fd);

/*
 * file lstat
 *
 * return - 0 : success
 *          -1: failure
 */
int8_t file_lstat(char *path,struct file_stat *stat);

/*
 * file del
 *
 * return - 0  : success
 *          -1 : failure
 */
int8_t file_del(char *path);

/*
 * file rename
 * 
 */
int8_t file_rename(char *old_path,char *new_path);

/*
 * directory mk
 *
 * return - 0 : success
 *          -1: failure
 */
int8_t directory_mk(char *path,enum dir_access_mode_e mode);

/*
 * directory open
 *
 */
DIR_HANDLE directory_open(char *path);

/*
 * directory read
 *
 * return - 0 : failure
 *          1 : success
 *
 */
int8_t directory_read(DIR_HANDLE dir,struct directory_entry *direntry);

/*
 * directory close
 *
 */
int8_t directory_close(DIR_HANDLE dir);

/*
 * directory remove
 */
int8_t directory_remove(char *path);

/*
 * file access
 */
int8_t file_access(char *path, int amode);

int file_get_error(void);

unsigned int file_get_free_space(void);

int file_get_free_fds(void);

void dump_open_files_info(void);

int file_expand(int32_t fd, uint32_t size, uint8_t opt);

#if __cplusplus
}
#endif

#endif
