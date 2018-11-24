#ifndef	ERRNO_H
#define	ERRNO_H

#define	EPERM	1	/* Permission denied */
#define	ENOENT	2	/* No such file or directory */
#define	ENOMEM	12	/* Not enough space */
#define	EBUSY	16	/* Device or resource busy */
#define	ENOTDIR	20	/* Not a directory */
#define	EISDIR	21	/* Is a directory */
#define	EINVAL	22	/* Invalid argument */
#define	ENOSYS	88	/* Invalid system call number */

extern int errno;

#endif
