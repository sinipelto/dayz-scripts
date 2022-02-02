# dayz-scripts
Custom scripts for DayZ Standalone Server

init.c - Server intialization script for Custom Mission in Server. Should be located under the mission folder.

start.bat - Server startup/control/restart script. Should be in the server root dir, same than the server executable.

serverDZ.cfg - Server general configuration file. Should be located in server root. Needs customization.

If you found these scripts useful, please give a STAR and even FOLLOW the repository to receive upcoming changes and updates!

# Init.c features

- Server side built-in commands using chat
- Spawning vehicles (few predefined vehicles)
- Spawning items/weapons/gear/ammo (customizable)
- Killing players, committing suicide
- Teleporting from/to coordinates
- Player listing and stats/details
- God mode / restore health
- Admin list read in startup from text file

# Getting started

Start off by installing the DayZ Standalone server (e.g. from [Steam](https://store.steampowered.com/about/))

The server installation path depends e.g. on your Steam settings

Once installed, simply copy over the files contained in this repo:
  [start.bat](start.bat), [serverDZ.cfg](serverDZ.cfg) into the server folder root 
  
  (e.g. C:\Program Files (x86)\Steam\steamapps\common\DayZServer\)
  
  [init.c](init.c) into the Mission folder 
  
  (e.g. C:\Program Files (x86)\Steam\steamapps\common\DayZServer\mpmissions\dayzOffline.chernarusplus\)
  
  You might need to override the existing init.c file (or keep it as backup and rename it as init.c.old)

Configure the server by editing the [serverDZ.cfg](serverDZ.cfg) file to your liking (e.g. server name, password player count, ..)

To access chat commands, you need to add the player's Steam64ID (in decimal format) to the admins.txt file. You can find the user's SteamID using any any Steam profile info webpage, e.g. [SteamFinder](https://www.steamidfinder.com/). E.g. steamID64 (Dec): 76561197960287930

Copy the ID(s) over into the admins.txt file. The file is located in the user profile folder (e.g. C:\Users\USERNAME\Documents\DayZ\admins.txt, aka %USERPROFILE%\Documents\DayZ\admins.txt in Windows). Create the file if it does not exist yet. Simply insert the IDs into the file, separated by newlines and save the file.

Once finished, launch off the server by running [start.bat](start.bat). A Server GUI log window opens and the server should start in a few moments. Ensure a log message says that the cloud Hive is connected, and forward required server ports to ensure your server will be listed in the public server listing.

Once the server is up and running, test your server by launching DayZ and connecting to your server using the server menu.

Once you are in the game, you can issue chat commands by sending a chat message starting with '/' (e.g. '/help').
