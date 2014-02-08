#!/bin/bash
id="$@"
echo "Grabbing video from camera $id"
echo "Running mplayer tv:// -tv width=640:height=480:device=/dev/video$id"
mplayer tv:// -tv width=640:height=480:device=/dev/video$id
