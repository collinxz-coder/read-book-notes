/* mman.h,v 1.1 2001/06/01 20:56:47 joeh Exp

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/vxworks/mman.h
 *
 * = AUTHOR
 *    Joe Hoffert
 *
 * ============================================================================ */

#ifndef PACE_SYS_MMAN_H_VXWORKS
#define PACE_SYS_MMAN_H_VXWORKS

#include <sys/mman.h>

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

#define PACE_MAP_FAILED MAP_FAILED
#define PACE_MAP_FIXED MAP_FIXED
#define PACE_MAP_PRIVATE MAP_PRIVATE
#define PACE_MAP_SHARED MAP_SHARED
#define PACE_MCL_CURRENT MCL_CURRENT
#define PACE_MS_ASYNC MS_ASYNC
#define PACE_MS_INVALIDATE
#define PACE_MS_SYNC MS_SYNC
#define PACE_PROT_EXEC PROT_EXEC
#define PACE_PROT_NONE PROT_NONE
#define PACE_PROT_READ PROT_READ
#define PACE_PROT_WRITE PROT_WRITE

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_SYS_MMAN_H_VXWORKS */
