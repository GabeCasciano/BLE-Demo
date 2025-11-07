#!/bin/bash

SESSION="BLEDEMO"
CWD=$(pwd)

if ! tmux has-session -t "$SESSION" 2 > /dev/null; then 
  tmux new-session -d -s "$SESSION" -n DEMO -c "$CWD/App"
  tmux split-window -h -t "$SESSION":DEMO -c "$CWD/Gateway"
  tmux send-keys -t "$SESSION":DEMO.0 './main.py' C-m 
  tmux send-keys -t "$SESSION":DEMO.1 './main.py' C-m
fi 

tmux attach -t "$SESSION"
