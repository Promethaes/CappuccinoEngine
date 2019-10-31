@echo off
setlocal
:: This code is almost entirely not mine, it is heavily inspired by INSTALL.cmd from https://github.com/EmilianC/Jewel3D - please check out Jewel3D!
:: Check for permissions.
 
net session >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo CappuccinoEngineSetup.bat must be run as administrator.
	echo.
    pause
    exit
)

echo Setting "CappuccinoPath" environment variable...
echo.

for %%i in ("%~dp0..") do set "folder=%%~fi"
setx CappuccinoPath "%folder%" /m > nul
echo CappuccinoPath set to "%folder%"

echo.
echo Setting up Cappuccino Engine...
echo.
call %folder%\Premake\premake5.exe --file="%folder%\premake5.lua" vs2019

echo.
echo Setup complete.
echo IMPORTANT: You may need to restart your PC in order for the environment variable to be set properly!
echo.

pause