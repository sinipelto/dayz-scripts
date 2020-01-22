@echo off

::Server name
set serverName=DayZ_Server

::Server files location
set serverLocation="D:\DayZServer"

::Server Port
set serverPort=2302

::Server config
set serverConfig=serverDZ.cfg

::Logical CPU cores to use (Equal or less than available)
set serverCPU=4

::Sets title for terminal (DONT edit)
title %serverName% batch

::DayZServer location (DONT edit)
cd "%serverLocation%"

:launch

taskkill /im DayZServer_x64.exe /F

::Time in seconds to wait before starting
timeout 3

echo (%time%) %serverName% started.

::Launch parameters (edit end: -config=|-port=|-profiles=|-doLogs|-adminLog|-netLog|-freezeCheck|-filePatching|-BEpath=|-cpuCount=)
start "DayZ_Server" "DayZServer_x64.exe" -config=%serverConfig% -port=%serverPort% -cpuCount=%serverCPU% -dologs -adminlog -netlog -freezecheck

::Time in seconds before kill server process time + 40 to match server shutdown time
timeout 86440

::Go back to the top and repeat the whole cycle again
goto launch
