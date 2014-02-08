#!/bin/bash
# Backup files on an external drive
sudo rsync -avz --delete /home/caiosba/Mestrado /media/Caio/
sudo rsync -avz --delete /usr /media/Caio/
sudo rsync -avz --delete /etc /media/Caio/
