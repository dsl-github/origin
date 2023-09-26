#ifndef __PVT_INT_H__
#define __PVT_INT_H__

typedef signed char     s8;     // 8 bits signed integer
typedef unsigned char   u8;     // 8 bits unsigned integer
typedef signed int      s16;    // 16 bits signed integer
typedef unsigned int    u16;    // 16 bits unsigned integer
typedef signed long     s32;    // 32 bits signed integer
typedef unsigned long   u32;    // 32 bits unsigned integer
typedef float           f32;

/* ��/��ȷ */
#ifndef TRUE
#define TRUE (1)
#endif

/* ��/���� */
#ifndef FALSE
#define FALSE (0)
#endif

#ifndef NULL
#ifdef __cplusplus
#define NULL (0)
#else
#define NULL ((void *)0)
#endif
#endif

#endif /* __PVT_INT_H__ */
