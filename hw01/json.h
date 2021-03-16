#ifndef __JSON__H__
#define __JSON__H__

typedef enum _eJSON
{
    String, // key = string
    Array,  // value = item, *next = *next
    Object  // key = key, vlaue = value, *next = *next
} eJSON;

typedef struct _sJSON
{
    eJSON type;
    char *key;
    struct _sJSON *value, *next;
} sJSON;

extern sJSON* StringToJSON(char *str);
extern sJSON* JSONGetValue(const sJSON *source, const char *key);
extern sJSON* JSONGetItem(const sJSON *source, int index);
extern void PrintJSON(const sJSON *source);

#endif
