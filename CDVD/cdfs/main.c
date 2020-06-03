#include <tamtypes.h>
#include <loadcore.h>
#include <thsemap.h>
#include <intrman.h>
#include <thbase.h>
#include <loadcore.h>
#include <sifman.h>
#include <sifcmd.h>
#include <ioman.h>
#include <cdvdman.h>
#include <sysclib.h>
#include <stdio.h>
#include <sysmem.h>
#include <iox_stat.h>
#include <alloc.h>

#include "cdfs_iop.h"

// 16 sectors worth of toc entry
#define MAX_FILES_PER_FOLDER 256
#define MAX_FILES_OPENED 16
#define MAX_FOLDERS_OPENED 16
#define MAX_BITS_READ 16384

#define CDVD_FILEPROPERTY_DIR 0x02
#define UNIT_NAME "cdfs"

struct fdtable
{
    iop_file_t *fd;
    int fileSize;
    int LBA;
    int filePos;
};

struct fodtable
{
    iop_file_t *fd;
    int files;
    int filesIndex;
    struct TocEntry entries[MAX_FILES_PER_FOLDER];
};

// File Descriptors
static struct fdtable fd_table[MAX_FILES_OPENED];
static int fd_used[MAX_FILES_OPENED];

// Folder Descriptors
static struct fodtable fod_table[MAX_FOLDERS_OPENED];
static int fod_used[MAX_FOLDERS_OPENED];

// global variables
static int lastsector;
static int last_bk = 0;

/***********************************************
*                                              *
*             DRIVER FUNCTIONS                 *
*                                              *
***********************************************/

static int CDVD_init(iop_device_t *driver)
{
    printf("CDVD: CDVD Filesystem v1.2\n\n");
    printf("Re-edited by fjtrujy\n\n");
    printf("Original implementation\n\n");
    printf("by A.Lee (aka Hiryu) & Nicholas Van Veen (aka Sjeep)\n\n");
    printf("CDVD: Initializing '%s' file driver.\n\n", driver->name);

    int ret = sceCdInit(SCECdINoD);
    printf("sceCdInit %i\n\n", ret);

    return 0;
}

static int CDVD_deinit(iop_device_t *driver)
{
#if defined(DEBUG)
    printf("CDVD_deinit\n\n");
#endif
    return 0;
}

static int CDVD_format(iop_device_t *driver)
{
#if defined(DEBUG)
    printf("CDVD: dummy CDVD_format function called\n\n");
#endif
    return -5;
}

static int CDVD_open(iop_file_t *f, const char *name, int mode)
{
    int j;
    static struct TocEntry tocEntry;

#ifdef DEBUG
    printf("CDVD: CDVD_open called.\n");
    printf("      kernel_fd.. %p\n", f);
    printf("      name....... %s %x\n", name, (int)name);
    printf("      mode....... %d\n\n", mode);
#endif

    // check if the file exists
    if (!CDVD_findfile(name, &tocEntry)) {
        printf("***** FILE %s CAN NOT FOUND ******\n\n", name);
        return -1;
    }

    if (mode != O_RDONLY) {
        printf("mode is different than O_RDONLY, expected %i, received %i\n\n", O_RDONLY, mode);
        return -2;
    }   

#ifdef DEBUG
    printf("CDVD: CDVD_open TocEntry info\n");
    printf("      TocEntry....... %p\n", &tocEntry);
    printf("      fileLBA........ %i\n", tocEntry.fileLBA);
    printf("      fileSize....... %i\n", tocEntry.fileSize);
    printf("      fileProperties. %i\n", tocEntry.fileProperties);
    printf("      dateStamp...... %s\n", tocEntry.dateStamp);
    printf("      filename....... %s\n", tocEntry.filename);
#endif

    // set up a new file descriptor
    for (j = 0; j < MAX_FILES_OPENED; j++) {
        if (fd_used[j] == 0)
            break;
    }

    if (j >= MAX_FILES_OPENED) {
        printf("File descriptor overflow!!\n\n");
        return -3;
    }

    fd_used[j] = 1;

#ifdef DEBUG
    printf("CDVD: internal fd %d\n", j);
#endif

    fd_table[j].fd = f;
    fd_table[j].fileSize = tocEntry.fileSize;
    fd_table[j].LBA = tocEntry.fileLBA;
    fd_table[j].filePos = 0;

#ifdef DEBUG
    printf("tocEntry.fileSize = %d\n", tocEntry.fileSize);

    printf("Opened file: %s\n", name);
#endif
    f->privdata = (void *)j;

    return j;
}

static int CDVD_close(iop_file_t *f)
{
    int i;

#ifdef DEBUG
    printf("CDVD: CDVD_close called.\n");
    printf("      kernel fd.. %p\n\n", f);
#endif

    i = (int)f->privdata;

    if (i >= MAX_FILES_OPENED) {
#ifdef DEBUG
        printf("CDVD_close: ERROR: File does not appear to be open!\n");
#endif
        return -1;
    }

#ifdef DEBUG
    printf("CDVD: internal fd %d\n", i);
#endif

    fd_used[i] = 0;

    return 0;
}

static int CDVD_read(iop_file_t *f, void *buffer, int size)
{
    int i;

    int start_sector;
    int off_sector;
    int num_sectors;

    int read = 0;
    static char local_buffer[9 * 2048];


#ifdef DEBUG
    printf("CDVD: CDVD_read called\n\n");
    printf("      kernel_fd... %p\n", f);
    printf("      buffer...... 0x%X\n", (int)buffer);
    printf("      size........ %d\n\n", size);
#endif

    i = (int)f->privdata;

    if (i >= MAX_FILES_OPENED) {
#ifdef DEBUG
        printf("CDVD_read: ERROR: File does not appear to be open!\n");
#endif
        return -1;
    }


    // A few sanity checks
    if (fd_table[i].filePos > fd_table[i].fileSize) {
        // We cant start reading from past the beginning of the file
        return 0;  // File exists but we couldnt read anything from it
    }

    if ((fd_table[i].filePos + size) > fd_table[i].fileSize)
        size = fd_table[i].fileSize - fd_table[i].filePos;

    if (size <= 0)
        return 0;

    if (size > MAX_BITS_READ)
        size = MAX_BITS_READ;

    // Now work out where we want to start reading from
    start_sector = fd_table[i].LBA + (fd_table[i].filePos >> 11);
    off_sector = (fd_table[i].filePos & 0x7FF);

    num_sectors = (off_sector + size);
    num_sectors = (num_sectors >> 11) + ((num_sectors & 2047) != 0);

#ifdef DEBUG
    printf("CDVD_read: read sectors %d to %d\n", start_sector, start_sector + num_sectors);
#endif

    // Skip a Sector for equal (use the last sector in buffer)
    if (start_sector == lastsector) {
        read = 1;
        if (last_bk > 0)
            memcpy(local_buffer, local_buffer + 2048 * (last_bk), 2048);
        last_bk = 0;
    }

    lastsector = start_sector + num_sectors - 1;
    // Read the data (we only ever get 16KB max request at once)

    if (read == 0 || (read == 1 && num_sectors > 1)) {
        if (!CDVD_ReadSect(start_sector + read, num_sectors - read, local_buffer + ((read) << 11))) {
#ifdef DEBUG
            printf("Couldn't Read from file for some reason\n");
#endif
        }

        last_bk = num_sectors - 1;
    }

    memcpy(buffer, local_buffer + off_sector, size);

    fd_table[i].filePos += size;

    return (size);
}

static int CDVD_write(iop_file_t *f, void *buffer, int size)
{
    if (size == 0)
        return 0;
    else {
        printf("CDVD: dummy CDVD_write function called, this is not a re-writer xD");
        return -1;
    }
}

static int CDVD_lseek(iop_file_t *f, int offset, int whence)
{
    int i;

#ifdef DEBUG
    printf("CDVD: CDVD_lseek called.\n");
    printf("      kernel_fd... %p\n", f);
    printf("      offset...... %d\n", offset);
    printf("      whence...... %d\n\n", whence);
#endif

    i = (int) f->privdata;

    if (i >= 16) {
#ifdef DEBUG
        printf("CDVD_lseek: ERROR: File does not appear to be open!\n");
#endif

        return -1;
    }

    switch (whence) {
        case SEEK_SET:
            fd_table[i].filePos = offset;
            break;

        case SEEK_CUR:
            fd_table[i].filePos += offset;
            break;

        case SEEK_END:
            fd_table[i].filePos = fd_table[i].fileSize + offset;
            break;

        default:
            return -1;
    }

    if (fd_table[i].filePos < 0)
        fd_table[i].filePos = 0;

    if (fd_table[i].filePos > fd_table[i].fileSize)
        fd_table[i].filePos = fd_table[i].fileSize;

    return fd_table[i].filePos;
}

static int CDVD_openDir(iop_file_t *f, const char *path) {
   int j;

#ifdef DEBUG
    printf("CDVD: CDVD_openDir called.\n");
    printf("      kernel_fd.. %p\n", f);
    printf("      name....... %s\n", f->device->name);
    printf("      mode....... %d\n\n", f->mode);
    printf("      path....... %s\n\n", path);
#endif

    // set up a new file descriptor
    for (j = 0; j < MAX_FOLDERS_OPENED; j++) {
        if (fod_used[j] == 0)
            break;
    }

    if (j >= MAX_FOLDERS_OPENED)
        return -3;

    fod_table[j].files = CDVD_GetDir(path, NULL, CDVD_GET_FILES_AND_DIRS, fod_table[j].entries, MAX_FILES_PER_FOLDER);
    if (fod_table[j].files < 0) {
        printf("The path doesn't exist\n\n");
        return -2;
    }

    fod_table[j].filesIndex = 0;
    fod_table[j].fd = f;
    fod_used[j] = 1;

#ifdef DEBUG
    printf("ITEMS %i\n\n", fod_table[j].files);
    int index = 0;
    for (index=0; index < fod_table[j].files; index++) {
        struct TocEntry tocEntry = fod_table[j].entries[index];
        
        printf("CDVD: CDVD_openDir index=%d TocEntry info\n", index);
        printf("      TocEntry....... %p\n", &tocEntry);
        printf("      fileLBA........ %i\n", tocEntry.fileLBA);
        printf("      fileSize....... %i\n", tocEntry.fileSize);
        printf("      fileProperties. %i\n", tocEntry.fileProperties);
        printf("      dateStamp....... %s\n", tocEntry.dateStamp);
        printf("      filename....... %s\n", tocEntry.filename);
    }
#endif
   
    f->privdata = (void *)j;

    return j;
}

static int CDVD_closeDir(iop_file_t *fd, const char *path) 
{
    int i;

#ifdef DEBUG
    printf("CDVD: CDVD_closeDir called.\n");
    printf("      kernel_fd.. %p\n", fd);
    printf("      path....... %s\n\n", path);
#endif

    i = (int)fd->privdata;

    if (i >= MAX_FOLDERS_OPENED) {
#ifdef DEBUG
        printf("CDVD_close: ERROR: File does not appear to be open!\n");
#endif
        return -1;
    }

#ifdef DEBUG
    printf("CDVD: internal fd %d\n", i);
#endif

    fod_used[i] = 0;

    return 0;
}

static int CDVD_dread(iop_file_t *fd, io_dirent_t *dirent)
{
    int i;
    int filesIndex;
#ifdef DEBUG
    printf("CDVD: CDVD_dread called.\n");
    printf("      kernel_fd.. %p\n", fd);
    printf("      mode....... %p\n\n", dirent);
#endif
    i = (int)fd->privdata;

    if (i >= MAX_FOLDERS_OPENED) {
#ifdef DEBUG
        printf("CDVD_dread: ERROR: Folder does not appear to be open!\n\n");
#endif
        return -1;
    }

    filesIndex = fod_table[i].filesIndex;
    if (filesIndex >= fod_table[i].files) {
#ifdef DEBUG
        printf("CDVD_dread: No more items pending to read!\n\n");
#endif
        return -1;
    }
    struct TocEntry entry = fod_table[i].entries[filesIndex];
#ifdef DEBUG
    printf("CDVD_dread: fod_table index=%i, fileIndex=%i\n\n", i, filesIndex);
    printf("CDVD_dread: entries=%i\n\n", fod_table[i].files);
    printf("CDVD_dread: reading entry\n\n");
    printf("      entry.. %p\n", entry);
    printf("      filesize....... %i\n\n", entry.fileSize);
    printf("      filename....... %s\n\n", entry.filename);
    printf("      fileproperties.. %i\n\n", entry.fileProperties);
#endif
    dirent->stat.mode = (entry.fileProperties == CDVD_FILEPROPERTY_DIR) ? FIO_SO_IFDIR : FIO_SO_IFREG;
    dirent->stat.attr = entry.fileProperties;
    dirent->stat.size = entry.fileSize;
    memcpy(dirent->stat.ctime, entry.dateStamp, 8);
    memcpy(dirent->stat.atime, entry.dateStamp, 8);
    memcpy(dirent->stat.mtime, entry.dateStamp, 8);
    strncpy(dirent->name, entry.filename, 128);
    dirent->unknown = 0;
    
    fod_table[i].filesIndex++;

    return fod_table[i].filesIndex;
}

static int CDVD_getstat(iop_file_t *fd, const char *name, iox_stat_t *stat) 
{
    struct TocEntry entry;
    int ret = -1;
#ifdef DEBUG
    printf("CDVD: CDVD_getstat called.\n");
    printf("      kernel_fd.. %p\n", fd);
    printf("      name....... %s\n\n", name);
#endif
    ret = CDVD_findfile(name, &entry);
#ifdef DEBUG
    printf("      entry.. %p\n", entry);
    printf("      filesize....... %i\n\n", entry.fileSize);
    printf("      filename....... %s\n\n", entry.filename);
    printf("      fileproperties.. %i\n\n", entry.fileProperties);
#endif
    stat->mode = (entry.fileProperties == CDVD_FILEPROPERTY_DIR) ? FIO_SO_IFDIR : FIO_SO_IFREG;
    stat->attr = entry.fileProperties;
    stat->size = entry.fileSize;
    memcpy(stat->ctime, entry.dateStamp, 8);
    memcpy(stat->atime, entry.dateStamp, 8);
    memcpy(stat->mtime, entry.dateStamp, 8);

    return ret;
}

static int dummy() {
    printf("CDVD: dummy function called\n\n");
    return -5;
}

static iop_device_ops_t filedriver_ops = {
    &CDVD_init,
    &CDVD_deinit,
    &CDVD_format,
    &CDVD_open,
    &CDVD_close,
    &CDVD_read,
    &CDVD_write,
    &CDVD_lseek,
    (void *)&dummy,
    (void *)&dummy,
    (void *)&dummy,
    (void *)&dummy,
    &CDVD_openDir,
    &CDVD_closeDir,
    &CDVD_dread,
    &CDVD_getstat,
    (void *)&dummy
};

int _start(int argc, char **argv)
{
    static iop_device_t file_driver;

    // Prepare cache and read mode
    CDVD_prepare();

    // setup the file_driver structure
    file_driver.name = UNIT_NAME;
    file_driver.type = IOP_DT_FS;
    file_driver.version = 1;
    file_driver.desc = "CDVD Filedriver";
    file_driver.ops = &filedriver_ops;

    DelDrv(UNIT_NAME);
    AddDrv(&file_driver);
}
