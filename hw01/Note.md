開發紀錄
=======

Makefile
--------

好久沒寫 Makefile 了，花了點時間查資料。
從沒寫過有多個 target 的架構，開發途中一直遇到錯誤，好在最後還是寫出來了來著。
嘗試寫了自動測試的功能，希望對之後的開發流程有所幫助。

hw0101
------
不知道給定的16進制字串會有多長，思考了一陣子才寫出來，
感覺應該可以再做些優化，等全都寫完再說吧？

hw0102
------
[輸出彩色字串]還滿有意思的，以前沒做過，查了一些資料才做出來。
字詞替換還沒寫，發現 `strstr`, `strncpy` 加上 `strlen` 就可以了的樣子，
好像沒有想像中複雜，感覺輸出的部分可以再做些優化。

[輸出彩色字串]: https://stackoverflow.com/questions/3219393/stdlib-and-colored-output-in-c

hw0103
------
複習指標操作？

hw0104
------
用C來讀取[JSON]好麻煩的感覺，還在思考中，
目前應該會全部用 `cosnt char*` 來存，
但結構要怎麼寫，查找資料的方法…思考中

[JSON]: https://www.wikiwand.com/zh/JSON

hw0105
------
就…先擺著晚點再寫，目前要處理的事情太多了，有點懶懶的。

hw0106
------
上 [linux manual page] 查了些資料…

* 兩者共通處 
    - 將錯誤代碼轉換成人類易於閱讀的形式。
* `void perror(const char* s)` 
    - 產生有關程式運作期間遇到的**最後一個**標準錯誤的資訊。
    - 存在一個全局變數 `errno` 用以保存錯誤代碼。
    - 如果錯誤發生沒有呼叫 `perror`，`errno` 的值可能會被洗掉。
    - 預設輸出至 `stderr` 。
* `char* strerror(int errnum)` 
    - 返回一個指向對應錯誤代碼的字串的指標。

```c
#include <stdio.h>
#include <string.h>
#include <errno.h>

int main(int argc, char const *argv[])
{
    errno = EPERM;  /* No such file or directory */
    
    printf("%s\n", strerror(EPERM));  /* No such file or directory */

    perror("msg from perror");  /* msg from perror: Operation not permitted */
    perror(NULL);  /* No such file or directory */

    return 0;
}

```

[linux manual page]: https://linux.die.net/man/
