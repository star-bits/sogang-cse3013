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
exefile2: film.o director.o main.o
    gcc -o exefile2 film.o director.o main.o

film.o: film.c
    gcc -c -o film.o film.c

director.o: director.c
    gcc -c -o director.o director.c

main.o: main.c
    gcc -c -o main.o main.c

# `make clean`
clean:
    rm *.o exefile2
```
```Makefile
# compiler to be used
CC = gcc
# name of the final executable
target = movie
# object files to build the final executable
objects = main.o film.o director.o

$(target): $(objects)
    $(CC) -o $(target) $(objects)

$(objects) : movie.h

# `clean` as a phony target
.PHONY: clean
clean:
    rm $(target) $(objects)

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
# -g
gcc sample.c -o sample -g

# gdb
gdb sample
gdb
(gdb) file sample

# display the value of a variable 
display num
print num
# inspect the type of a variable
whatis num
# information about local variables
info locals

set var i = 2

# set breakpoint
break main
break 7 # 7 is a line number
break if i==3
info break
condition 2 # 2 is a breakpoint number
disable 2
delete 2
# set watchpoint
watch i==3

# run
run
run arg1 arg2

# continue execution until the next breakpoint or watchpoint
continue
next
step
# finish the current loop iteration or function
finish

list
list main
list 7 # 7 is a line number
```
```Makefile
cc=gcc
# -W: enable all warnings
cflags = -W -g
# name of the final executable
target = main

# object files
objects = main.o string_manipulation.o output.o

# final executable depends on the object files
$(target) : $(objects)
    $(cc) $(cflags) -o $(target) $(objects)

# rule on how to convert a `.c` source file into an `.o` object file
# for any `.o` file, look for a `.c` file with the same stem (name before the extension)
# `%` is a wildcard that matches any string
%.o : %.c
    # `$@`: automatic variable that stands for the target of the rule; i.e., the `.o` file
    # `#<`: automatic variable that represents the first dependency of the rule; i.e., the correspondng `.c` file
    $(cc) $(cflags) -c -o $@ $<

# object files listed all depends on Header.h
main.o string_manipulation.o output.o : Header.h

.PHONY : clean
clean :
    rm $(target) $(objects)
```
```c
void Remove_Blanks_At_The_End(char *line) {
  int i, k, newline_flag = 0;

  // 전체 문장에 대하여 line[k] 가 줄바꿈이면 flag를 1로 set, '\0'이면 ~~
  for (k = 0;; k++) {
    if (line[k] == '\n') {
      newline_flag = 1;
      break;
    }
    if (line[k] == '\0') {
      break;
    }
  }
  // 전체 문장에 대하여 line[i]가 space가 아니라면 break,
  for (i = k - 1; i >= 0; i--) {
    if (line[i] != ' ') {
      break;
    }
  }
  i++;
  // flag가 1일때 문장의 마지막은 줄바꿈&space, flag가 1이 아니라면 '\0'
  if (newline_flag == 1) {
    line[i] = '\n';
    line[i + 1] = '\0';
  } else {
    line[i] = '\0';
  }
}
```
