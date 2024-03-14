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
