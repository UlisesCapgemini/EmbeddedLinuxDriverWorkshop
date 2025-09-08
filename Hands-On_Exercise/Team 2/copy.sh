# copy source code to a folder without spaces for kernel flavor
M_SRC_DIR="$PWD"
M_DEST_DIR=/home/victorb/EmbeddedLinuxDriverWorkshop/Team_2

# Copy files preserving permissions
if [ -d "$M_SRC_DIR" ]; then
    rsync -a --exclude='kernel_headers_6.8.0-65-generic' "$M_SRC_DIR"/* "$M_DEST_DIR"/
    echo " Files copied to $M_DEST_DIR"
else
    echo " Source directory $M_SRC_DIR does not exist. Make sure it's mounted."
fi
cd /home/victorb/EmbeddedLinuxDriverWorkshop/Team_2
