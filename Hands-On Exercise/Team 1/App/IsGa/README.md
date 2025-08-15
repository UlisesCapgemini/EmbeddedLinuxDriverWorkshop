capgeminiSound : Custom command for audio functionality
/usr/local/bin : Designated location for storing executable files


###############################################################
Step 1: Install the Command

Make the script named capgeminiSound executable (if it is not):
$ chmod +x capgeminiSound

Move it to a directory in your PATH:
$ sudo mv capgeminiSound /usr/local/bin/
###############################################################
Step 2: Install Dependencies

The script uses common Linux audio tools. Install them with:
$ sudo apt-get update
$ sudo apt-get install alsa-utils
###############################################################
Step 3: Test the Command
Get help
$ capgeminiSound --help
