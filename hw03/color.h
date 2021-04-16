#ifndef __COLOR_H__
#define __COLOR_H__

#define ESC "\033"

#define CLR_BLA ESC"[30m"
#define CLR_RED ESC"[31m"
#define CLR_GRE ESC"[32m"
#define CLR_YEL ESC"[33m"
#define CLR_BLU ESC"[34m"
#define CLR_MAG ESC"[35m"
#define CLR_CYA ESC"[36m"
#define CLR_WHI ESC"[37m"

#define CLR_24_FG(R,G,B) ESC"[38;2;"#R ";"#G ";"#B "m"
#define CLR_24_BG(R,G,B) ESC"[48;2;"#R ";"#G ";"#B "m"

#endif
