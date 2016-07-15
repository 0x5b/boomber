#!/bin/bash

changed="$(git ls-files -m)"
path=""
n=0

for i in ${changed}; do
	scp ~/develop/c2/$i ec2-user@185.12.28.21:c2/c2/$i
	dir=$(echo $i | tr "/" "\n")
	for j in ${dir}; do
		if [ $n -eq 0 ]; then
			((n++))
			continue
		fi
		path+=$j
		path+='/'
	done
	path=$(echo -n $path | head -c -1)
	echo changed:  $i
	pdsh -w root@dev.avazh.devel.croc.ru cp /home/ec2-user/c2/c2/$i /usr/lib64/python2.6/site-packages/$path
	path=""
	n=0
done
pdsh -w root@dev.avazh.devel.croc.ru "yes | c2-manage --restart-services all" 


