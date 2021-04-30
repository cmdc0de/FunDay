#!/bin/bash
ESPCONTAINERVERSION=`cat espcontainerver`
PROJECTNAME=`cat projname`

echo $ESPCONTAINERVERSION

docker build -t $PROJECTNAME:$ESPCONTAINERVERSION .
