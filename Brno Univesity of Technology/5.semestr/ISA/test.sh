#!/usr/bin/env bash

t=3
counter=0
p=0
c=10
n=0
a=""
m=""
l=""
r=""
napoveda="Napoveda: Parametry s argumenty zadavejte stejne jako pri spusteni serveru dle pravidel daneho modu + pripojte parametr \"-n\" s ciselnou hodnotou vyjadrujici pocet pripojujicich se klientu."

if [ $# = 0 ]; then
  echo "Nebyly zadany argumenty!" >&2
  exit 1
fi

while getopts ':c:n:a:t:p:m:r:l::' opt; do
  case "$opt" in
    t)
	  t=$OPTARG ;;
    a)
	  a=$OPTARG ;;
    p)
	  p=$OPTARG ;;
	m)
	  m=$OPTARG ;;
	c)
	  c=$OPTARG ;;
	l)
	  l=$OPTARG ;;
	r)
	  r=$OPTARG ;;
	n)
	  n=$OPTARG ;;
	\?)
	  echo "$napoveda"
      echo "Neznamy parametr: -$OPTARG" >&2
      exit 1
      ;;
    :)
	  echo "$napoveda"
      echo "Parametr -$OPTARG potrebuje argument" >&2
      exit 1
      ;;
  esac
done

echo "-m $m"
echo "-t $t"
echo "-a $a"
echo "-p $p"
echo "-c $c"
echo "-l $l"
echo "-n $n"

case "$m" in
  ftp )
	echo "Vitej na ftp"
	gnome-terminal -x sh -c "sudo ./fakesrv -m $m -p $p -a $a -l $l -c $c -t $t; bash"
    while [ "$counter" -ne "$n" ]; do
	  ((counter++))
	  gnome-terminal -x sh -c "ftp $a $p; bash"
	done
    ;;
  ssh )
	echo "Vitej na ssh"
	gnome-terminal -x sh -c "sudo ./fakesrv -m $m -p $p -a $a -l $l -r $r -c $c -t $t; bash"
    while [ "$counter" -ne "$n" ]; do
	  ((counter++))
	  gnome-terminal -x sh -c "ssh -l client$counter $a -p $p; bash"
	done
    ;;
  * )
    echo "Nebyl zadan spravny mod"
    exit 1
    ;;
esac