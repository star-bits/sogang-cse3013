# 소프트웨어개발도구및환경실습 (컴실1)

## Week 1

```
ssh cse20161277@cspro.sogang.ac.kr
passwd
logout
```

```
touch phone

vim phone

i

#!/bin/bash

DATA_FILE="/sogang/under/cse20161277/mydata"

if [ "$#" -eq 0 ]; then
    echo "Usage: $0 searchfor [...searchfor]"
    echo "(You didn't tell me what you want to search for.)"
    exit 1
fi

SEARCH_PATTERN=$(echo "$@" | sed 's/ /|/g')

egrep -i "$SEARCH_PATTERN" "$DATA_FILE" | awk -f display.awk

Esc
:wq
```

```
ls -lh
chmod +x phone
ls -lh
```

```
scp mydata cse20161277@cspro.sogang.ac.kr:/sogang/under/cse20161277/
scp display.awk cse20161277@cspro.sogang.ac.kr:/sogang/under/cse20161277/
```

```
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
