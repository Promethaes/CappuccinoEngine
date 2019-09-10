@echo off
:: This code is almost entirely not mine, it is heavily inspired by UNINSTALL.cmd from https://github.com/EmilianC/Jewel3D - please check out Jewel3D!
:: Check for permissions.
net session >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    ECHO Cappuccino Uninstall.cmd must be run as admin.
    Pause
    Exit
)

ECHO Removing Environment Variable...
set CappuccinoPath=
ECHO Done!
ECHO.

ECHO IMPORTANT: You may need to restart your PC in order for the environment variable to be set properly!
Pause