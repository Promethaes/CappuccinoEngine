@echo off
:: This code is almost entirely not mine, it is heavily inspired by INSTALL.cmd from https://github.com/EmilianC/Jewel3D - please check out Jewel3D!
:: Check for permissions. 
net session >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    ECHO Cappuccino Setup.cmd must be run as admin.
    Pause
    goto :eof
)

ECHO.
ECHO Installing Cappuccino...
ECHO.
ECHO Setting Environment Variable...
SETX CappuccinoPath "%~dp0\" -m > nul
ECHO Done!
ECHO.
ECHO Starting Premake...
START %CappuccinoPath%\..\"External Dependencies"\Premake\premake5.exe --file="%~dp0\premake5.lua" vs2019


ECHO Done!
ECHO.

ECHO IMPORTANT: You may need to restart your PC in order for the environment variable to be set properly!
Pause