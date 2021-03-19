#ifndef _UTILS_H_
#define _UTILS_H_

#ifndef _LINUX_
#define INT64 __int64
#else
#define INT64 int64_t
#endif

#define ARRAY_SIZE(a)       (sizeof(a) / sizeof(a[0]))
#define PI 3.14159265

extern int sign(int x);



#ifdef _LINUX_
#define fopen_s(pFile,filename,mode) ((*(pFile))=fopen((filename),(mode)))==NULL
//#define sprintf_s(str, strsiz, format, ...) sprintf(str, format, __VA_ARGS__)
#endif

#endif //_UTILS_H_


