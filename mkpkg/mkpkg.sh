#!/bin/sh

QT5_PATH='/opt/Qt5.7.0/5.7/gcc_64/bin'
DEPLOY_MAKER='linuxdeployqt-continuous-x86_64.AppImage'
APP_PATH='usr/share/applications'
ICON_PATH='usr/share/pixmaps'
SRC_PATH='usr/share/AgendaQt'
AGENDAQT_BIN='AgendaQt'
AGENDA_BIN='Agenda'
DPKG_NAME='agendaqt_0.1.1_amd64.deb'


# check AgendaQt and Agenda bin
echo "check binary file..."
if [[ ! -f "$AGENDAQT_BIN" ]]
then
    echo "AgendaQt binary file is not found. "
    echo "Please compile AgendaQt project first, "
    echo "and copy the binary file to here. "
    exit
fi
if [[ ! -f "$AGENDA_BIN" ]]
then
    echo "Agenda binary file is not found. "
    echo "Please compile Agenda project first, "
    echo "and copy the binary file to here. "
    exit
fi
cp -rf ../src/resources .

# setup qt5 environment
echo "setup qt5 environment..."
export PATH=$QT5_PATH:$PATH

# download linuxdeployqt if needed
if [[ ! -f "$DEPLOY_MAKER" ]]
then
    echo "File '${DEPLOY_MAKER}' not found, start download now."
    wget https://github.com/probonopd/linuxdeployqt/releases/download/continuous/linuxdeployqt-continuous-x86_64.AppImage
fi
chmod 777 $DEPLOY_MAKER

# make app deploy version
echo "make deploy qt app..."
./$DEPLOY_MAKER $AGENDAQT_BIN -always-overwrite -no-strip
rm -rf AppRun

# clean and setup dir
echo "setup dir..."
rm -rf ./pkg/
rm -rf ./*.deb
mkdir -p ./pkg/$APP_PATH
mkdir -p ./pkg/$ICON_PATH
mkdir -p ./pkg/$SRC_PATH
cp -rf ./DEBIAN ./pkg/

# setup source files
echo "setup source files..."
mv $AGENDA_BIN ./pkg/$SRC_PATH
mv $AGENDAQT_BIN ./pkg/$SRC_PATH
mv lib ./pkg/$SRC_PATH
mv plugins ./pkg/$SRC_PATH
mv resources ./pkg/$SRC_PATH

# setup icon and desktop
echo "setup icon and desktop files..."
cp -f ./others/icon.png ./pkg/$ICON_PATH/AgendaQt.png
cp -f ./others/AgendaQt.desktop ./pkg/$APP_PATH/

# create debian package
echo "create debian package..."
dpkg -b ./pkg $DPKG_NAME
echo "All Done!"
