#!/bin/bash
echo "" > ./repl/tmp.pn;

echo "Pinecone Programming language"
echo "Type .exit or Ctrl+C to Exit"
while [ true ]
do
  read -p ">> " cmd;
  if [ "$cmd" == ".clear" ]; then
    echo "" > ./repl/tmp.pn;
    clear;
    echo "[cleared]";

  elif [ "$cmd" == ".exit" ]; then
    exit 0;
  elif [ "$cmd" == ".read" ]; then
    cat ./repl/tmp.pn;
  else
    echo $cmd >> ./repl/tmp.pn;
    echo " -- ";
    ./pinecone ./repl/tmp.pn;
  fi
done
