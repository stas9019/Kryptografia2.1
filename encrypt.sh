#! /bin/bash


#check args

if [ $# -eq 0 ];then	
	echo "Usage:" "[keystorePath]" "[keyID]" "[filePath]" "[action]" "[mode]"
	echo "Example"
	echo "~/ClionProjects/Keystore Encrypted ~/ClionProjects/Kryptografia2.1/Encrypted decrypt CBC"
	exit
fi

if [ $# -lt 5 ];then	
	echo "Not enough arguments"
	exit
fi


keystorePath=$1
keyID=$2
filePath=$3
action=$4
mode=$5




#compile krypto2.1
g++ main.cpp -lcrypto -lssl -o exe

#compile keystore
g++ /home/stas/ClionProjects/Keystore/main.cpp -lcrypto -lssl -o /home/stas/ClionProjects/Keystore/exe

#run keystore app with key ID
"$keystorePath/exe" $keyID


#take key from temp file
key=`cat "$keystorePath/temp"`
rm "$keystorePath/temp"

#echo $key

#run krypto app with filePath, action, mode and key
./exe $filePath $action $mode $key




