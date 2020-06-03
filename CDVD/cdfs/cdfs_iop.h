#ifndef _CDVD_IOP_H
#define _CDVD_IOP_H

#include "cdfs.h"

int CDVD_prepare();
int CDVD_findfile(const char *fname, struct TocEntry *tocEntry);
int CDVD_ReadSect(u32 lsn, u32 sectors, void *buf);
int CDVD_GetDir(const char *pathname, const char *extensions, enum CDVD_getMode getMode, struct TocEntry tocEntry[], unsigned int req_entries);

#endif  // _CDVD_H
