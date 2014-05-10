#!/bin/bash

echo 'Sync with GitHub'

# change to the correct directory
echo 'Change to the local syncing directory'
cd '/mnt/Six_2TB/pub/Software_GitHub_Sync/'

# add all new untracked files to the local repository
echo 'Check for any new files and add to the local repository'
git add *

# commit all changes to the local repository
echo 'Commit the local repository'
git commit -a -m 'Auto update from Six'

# push the local repository out to Git-Hub
# need to pass the user ID and password 
echo 'Push out the local repository'
git push 'https://sampsontech:lantra01@github.com/sampsontech/Teensyduino-Code.git' 
# Pull all updates from Git-Hub 
# need to pass the user ID and password 
echo 'pull in and merge the remote repository'
git pull 'https://sampsontech:lantra01@github.com/sampsontech/Teensyduino-Code.git'

#done
echo 'Done...'


 