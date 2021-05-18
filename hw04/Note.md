開發紀錄
=======

hw0402
------
```c
int a = ' ';   // 32
//  33 ~  47: !"#$%&'()*+,-./
//  61      : =
//  91 ~  94: [\]^
// 123 ~ 125: {|}

int n = !"";   // 0

int d[] = {!"", !!"", ' '^'#'};
```
> Well... dark magic OwO

- read file to buffer, for each character...
- maintain ...
    + status { macro, comment, multi-line-comment, normal, string }
    + multi-line { true, false }
    + ASCII-escape { true, false }
- status == macro
    + until newline
        * check multi-line flag
    + status = normal
- status == comment
    + until newline
    + status = normal
- status == multi-line-comment
    + until newline
    + status = normal
- status == string
    + check ASCII-escape flag
    + status = normal
- status == normal
    + check string begin
    + check macro begin
    + check comment begin
    + check multi-line-comment begin
    + check multi-line flag
