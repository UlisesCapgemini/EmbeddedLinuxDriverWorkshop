# capgeminiSound - Capgemini Sound Utility v1.1

## Description

capgeminiSound : Custom linux command for audio functionality
/usr/local/bin : Designated location for storing executable files

## Features

- Support `--record` to record audio to a specified WAV audio file. The recording duration is automatically set to 5 seconds
- Support `--play` to play the most recently recorded/played WAV audio file
- Create directories automatically if they don't exist
- Store the last WAV audio file path for convenient replay

## Installation

```bash
# Make the script named capgeminiSound executable (if it is not):
chmod +x capgeminiSound

# Move it to a directory in your PATH:
sudo mv capgeminiSound /usr/local/bin/

# Install dependencies
sudo apt-get update
sudo apt-get install alsa-utils

# Test the Command
capgeminiSound --help
