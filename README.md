# 소프트웨어개발도구및환경실습 (컴실1)

## 1주차

```bash
ssh cse20161277@cspro.sogang.ac.kr
passwd
logout
```

```bash
touch phone
vim phone
i

#!/bin/bash

DATA_FILE="$HOME/data"

if [ "$#" -eq 0 ]; then
    echo "Usage: $0 searchfor [...searchfor]"
    echo "(You didn't tell me what you want to search for.)"
    exit 1
fi

SEARCH_PATTERN=$(echo "$@" | sed 's/ /|/g')

egrep -i "$SEARCH_PATTERN" "$DATA_FILE" | awk -f display.awk

`esc`
:wq
```
```bash
# vim 한글 깨짐 해결
:set encoding=utf-8
```
```bash
#!/bin/bash

DATA_FILE="$HOME/data"

if [ "$#" -eq 0 ]; then
# $#: the number of arguments the script was called with
# -eq 0: the number is equal to 0
    echo "Usage: $0 searchfor [...searchfor]"
    # $0: the name of the script being executed
    echo "(You didn't tell me what you want to search for.)"
    exit 1
    # non-zero exit statuses signal that an error occurred.
fi

SEARCH_PATTERN=$(echo "$@" | sed 's/ /|/g')
# $@ holds all positional parameters, starting from $1
# |: passes output from `echo "$@"` to `sed 's/ /|/g'`
# s: substitute
# s/find/replace/g
# g: global - all instances of the find pattern will be replaced

egrep -i "$SEARCH_PATTERN" "$DATA_FILE" | awk -f display.awk
# -i: case insensitive
```
```bash
# start with all lines of the file
MATCHES=$(cat "$DATA_FILE")

# loop through arguments
for arg in "$@"; do
    # filter matches for current argument
    MATCHES=$(echo "$MATCHES" | grep -i "$arg")
done

echo "$MATCHES" | awk -f display.awk
```
```bash
ls -alh
chmod +x phone
ls -alh
```

```bash
scp data cse20161277@cspro.sogang.ac.kr:/sogang/under/cse20161277/
scp cse20161277@cspro.sogang.ac.kr:/sogang/under/cse20161277/data .
```

```bash
vim mydata

홍길동|서울시 마포구 신수동 서강대학교 AS관 301호|02-705-2665
박문수|서울시 서대문구 신촌동 연세대학교 연구관 102호|02-708-4678
Andrew|경기도 의정부시 호원동 23-12번지|031-827-7842
한승현|경기도 고양시 일산동구 장항2동 호수마을 407호 1602호|031-904-5643
신봉선|경기도 고양시 일산동구 마두동 206동|031-813-3803

vim display.awk

BEGIN{FS="|"}
{print "--------------->";}
{print "name : "$1;}
{print "address : "$2;}
{print "phone : "$3;}
{print "<---------------";}

./phone Andrew

--------------->
name : Andrew
address : 경기도 의정부시 호원동 23-12번지
phone : 031-827-7842
<---------------
```

```bash
ls -alh
cp data .data
ls -alh
chmod 700 $HOME/.data
ls -alh
```

```bash
echo "Working directory: "
read dirname

if [ -z "$dirname" ]; then
# -z checks for a zero-length string
    dirname="."
fi

if [ ! -d "$dirname" ]; then
# -d checks for the existence of a directory
    echo "input dir does not exist"
    exit 1
fi

if ! cd "$dirname" ; then
    echo "cd to input dir failed"
    exit 1
fi

for oldname in *; do
    newname=$(echo "$oldname" | tr '[:upper:][:lower:]' '[:lower:][:upper:]')
    mv "$oldname" "$newname"
done
```

## 2주차
```bash
gcc hello.c
./a.out

gcc hello.c -o hi
gcc -o hi hello.c
./hi

gcc hello.c -v

gcc hello.c -c
# hello.o

gcc -o main main.c fun1.c fun2.c

gcc -c main.c
gcc -c fun1.c
gcc -c fun2.c
gcc -o main main.o fun1.o fun2.o
```
```Makefile
# `make`
main: main.o fun1.o fun2.o
    gcc -o main main.o fun1.o fun2.o 

main.o: main.c
    gcc -c -o main.o main.c

fun1.o: fun1.c
    gcc -c -o fun1.o fun1.c

fun2.o: fun2.c
    gcc -c -o fun2.o fun2.c

# `make clean`
clean:
    rm *.o main
```
```Makefile
# compiler
CC = gcc
# compiler flags: enable warnings and debug info
CFLAGS = -W -g
# final executable
TARGET = main
# object files
OBJECTS = main.o fun1.o fun2.o

# rule for building the final executable
$(TARGET): $(OBJECTS)
    # links the object files into the final executable
    $(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

# pattern rule for converting .c files into .o files
%.o : %.c
    # $@ represents the .o file
    # #< represents the correspondng .c file
    $(CC) $(CFLAGS) -c -o $@ $<
# the .c to .o conversion is carried out automatically
# this block is actually unnecessary

# object files depend on this header
$(OBJECTS): header.h

# declare 'clean' as a phony target
.PHONY: clean
# remove executable and objects
clean:
    rm $(TARGET) $(OBJECTS)

```
```c
// sample.c
#include <stdio.h>

main(void)
{
    int i;
    double num;

    for (i=0; i<5; i++){
        num = i/2 + i;
        printf("num is %f \n", num);
    }
}
```
```bash
gcc sample.c -o sample -g

# start gdb for debugging
gdb sample
# alternatively
gdb
(gdb) file sample 

# set breakpoints
break main # set breakpoint at the beginning of main function
break 7 # set a breakpoint at line number 7
break if i==3

info break # list all breakpoints
disable 2 # disable breakpoint number 2
delete 2 # remove breakpoint number 2

# set a watchpoint to monitor variable changes
watch i==3 # triggers when i changes to 3

# run the program
run # starts the program
run arg1 arg2 # starts the program with arguments arg1 and arg2

# step through the program
continue # continue execution until the next breakpoint or watchpoint
next # execute the next line
step # execute the next step
finish # finish the current function

# inspect and manipulate variables
display num # continuously display the value of num after each step
print num # print the current value of num
whatis num # display the type of variable num
info locals # list local variables in the current frame
set var i = 2 # set the variable i to 2

# list source code for context
list # list source code around the current line
list main # list the beginning of the main function
list 7 # list source code around line number 7

```

## 5주차
```bash
mkdir w5hw
cd w5hw
git init
touch random
git add .
git commit -m "initial commit"

git remote add origin https://github.com/star-bits/w5hw.git
git branch -M main
# 현재 브랜치 이름 변경 (master였다면 main으로)
# -m: 이름 변경
# -M: 이름 강제 변경
git push -u origin main
# -u: 이후 push/pull할 때 브랜치 이름 지정하지 않아도 됨 (--set-upstream)

git checkout -b feature-1
touch feature1
git add feature1
git commit -m "feature 1"
git push -u origin feature-1
# 완료 메세지: branch 'feature-1' set up to track 'origin/feature-1'.

git checkout main
git checkout -b feature-2
touch feature2
git add feature2
git commit -m "feature 2"
git push -u origin feature-2

git checkout main
git checkout -b feature-3
touch feature3
git add feature3
git commit -m "feature 3"
git push -u origin feature-3

git checkout main
git merge feature-1
git merge feature-2
git merge feature-3
git push origin main

```

## 7주차
```cpp
// water_line.h

Dot* path = NULL;
```
