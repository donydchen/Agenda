#!/bin/sh

APP_PATH='usr/share/applications'
ICON_PATH='usr/share/pixmaps'
SRC_PATH='/usr/share/AgendaQt'


rm -rf ./pkg/
rm -rf ./*.deb
mkdir -p ./pkg/$APP_PATH
mkdir -p ./pkg/$ICON_PATH
mkdir -p ./pkg/$SRC_PATH
cp -rf ./DEBIAN ./pkg/

cp -f ./bin/Agenda* ./pkg/$SRC_PATH

cp -f ./others/icon.png ./pkg/$ICON_PATH/AgendaQt.png
cp -f ./others/AgendaQt.desktop ./pkg/$APP_PATH/

dpkg -b ./pkg agendaqt_0.1.1_amd64.deb
