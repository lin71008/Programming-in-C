#ifndef __STATUS_H__
#define __STATUS_H__

typedef enum _eStatus
{
    Success=0,  // success (do something).
    Continue,  // success (do something), continue (do something).
    InvalidNull,  // given argument can't be NULL.
    InvalidValue,  // given argument is an invalid value. (ex: must greater than zero)
    ErrorFormat_0,  // given (string type) argument have syntax error. (ex: must begin with '$')
    ErrorFormat_1,  // same as ErrorFormat_0
    ErrorFormat_2,  // same as ErrorFormat_0
    InvalidCharacter_0,  // given (string type) argument have invalid character. (ex: can't use '$' character)
    InvalidCharacter_1,  // same as InvalidValue_0
    InvalidCharacter_2,  // same as InvalidValue_0
    InvalidPatten_0,  // given (string type) argument have invalid patten. (ex: can't use '>' character after '-' character)
    RunOutMemory, // run out of memory.
    Unknown  // error by unknown reason.
} eStatus;

extern const char* eStatusStr(const eStatus e);

#endif
