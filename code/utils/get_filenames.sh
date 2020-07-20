#!/bin/bash

if [[ $# -ne 2 ]]; then
    echo "Usage: ./get_filenames.sh /path/to/PU/files /path/to/noPU/files"
    exit 1
fi

PU=$1
noPU=$2

echo "PU_files:" > files.yml

for i in $(ls $PU); do 
  echo "  - \"/myhome/adlintul/PU/$i\"" >> files.yml 
done

echo "noPU_files:" >> files.yml

for i in $(ls $noPU); do 
  echo "  - \"/myhome/adlintul/noPU/$i\"" >> files.yml 
done
