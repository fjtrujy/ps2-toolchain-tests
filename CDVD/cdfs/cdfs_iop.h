#ifndef _CDVD_IOP_H
#define _CDVD_IOP_H

enum CDVD_getMode {
    CDVD_GET_FILES_ONLY = 1,
    CDVD_GET_DIRS_ONLY = 2,
    CDVD_GET_FILES_AND_DIRS = 3
};

struct TocEntry {
    u32 fileLBA;
    u32 fileSize;
    u8 fileProperties;
    unsigned char dateStamp[8];
    char filename[128 + 1];
} __attribute__((packed));

int CDVD_prepare(void);
int CDVD_start(void);
int CDVD_findfile(const char *fname, struct TocEntry *tocEntry);
int CDVD_ReadSect(u32 lsn, u32 sectors, void *buf);
int CDVD_GetDir(const char *pathname, const char *extensions, enum CDVD_getMode getMode, struct TocEntry tocEntry[], unsigned int req_entries);

#endif  // _CDVD_H
