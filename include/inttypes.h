#ifndef	INTTYPES_H
#define	INTTYPES_H
typedef	char	int8_t;
typedef	short	int16_t;
typedef	int	int32_t;
typedef	long long	int64_t;

typedef	unsigned char	uint8_t;
typedef	unsigned short	uint16_t;
typedef	unsigned int	uint32_t;
typedef	unsigned long long	uint64_t;

#ifndef	_HAVE_SIZE_T
#define	_HAVE_SIZE_T
typedef	unsigned long	size_t;
#endif

/* Boolean type */
#define	true	1
#define	false	0
typedef	uint8_t	bool;
#endif
