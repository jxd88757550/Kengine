#!/bin/bash
ACCNAME="AbhiGod123"

read -r -p 'Project name: ' PROJNAME

git clone "https://github.com/$ACCNAME/$PROJNAME"

git config --global user.name "Abhinav Neelam"
git init
git config --global user.email "abhinavneelam949@gmail.com"

git remote add origin "https://github.com/$ACCNAME/$PROJNAME" + ".git" 

git remote -v
git status

Portable/premake5 vs2017