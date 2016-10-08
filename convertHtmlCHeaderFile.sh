#!/bin/sh

cp index.html index_html 

perl -p -i -e 's/[\n|\r|\t]//g;' index_html

echo "#ifndef INDEX_HTML" > indexHtml.h
echo "#define INDEX_HTML" >> indexHtml.h



xxd -i --include index_html >> indexHtml.h
sed -i 's/unsigned char/const char/g' indexHtml.h
sed -i 's/unsigned int/const unsigned int/g' indexHtml.h
sed -i 's/= {/PROGMEM = {/g' indexHtml.h

echo "#endif" >> indexHtml.h


