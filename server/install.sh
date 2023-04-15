echo "COPYING FILES IN /usr/share/mousedroid"

sudo mkdir /usr/share/mousedroid

sudo cp icon.png /usr/share/mousedroid
sudo cp cmake/bin/Mousedroid /usr/share/mousedroid

(
    echo MINIMIZE_TASKBAR=0
    echo MOVE_SENSITIVITY=10
    echo RUN_STARTUP=0
    echo SCROLL_SENSITIVITY=3 
)>>/usr/share/mousedroid


echo "SETTINGS PERMISSIONS"

sudo chmod +r /usr/share/mousedroid/icon.png /usr/share/mousedroid/config.ini
sudo chmod +r /usr/share/mousedroid/Mousedroid

(
    sudo echo [Desktop Entry]
    sudo echo Name=Mousedroid
    sudo echo Exec=/usr/share/mousedroid/Mousedroid
    sudo echo Icon=/usr/share/mousedroid/icon.png
    sudo echo Type=Application
)>>/usr/share/applications/mousedroid.desktop

sudo chmod +r /usr/share/applications/mousedroid.desktop

sudo echo KERNEL==\"uinput\", TAG+=\"uaccess\">>/etc/udev/rules.d/50-uinput.rules 

echo "DONE"
