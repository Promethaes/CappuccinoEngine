@echo off
:: This code is almost entirely not mine, it is heavily inspired by UNinSTALL.cmd from https://github.com/EmilianC/Jewel3D - please check out Jewel3D!
:: Check for permissions.

net session >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo CappuccinoEngineUninstall.bat must be run as administrator.
	echo.
    pause
    exit
)

echo Deleting files...
cd %CappuccinoPath%

for /d /r . %%d in (.vs) do @if exist "%%d" rd /s /q "%%d"
for /d /r . %%d in (Build) do @if exist "%%d" rd /s /q "%%d"
for /d /r . %%d in (Externals\Build) do @if exist "%%d" rd /s /q "%%d"

del /s /q %CappuccinoPath%\*.vcxproj
del /s /q %CappuccinoPath%\*.vcxproj.user
del /s /q %CappuccinoPath%\*.vcxproj.filters
del /s /q %CappuccinoPath%\*.sln

echo Removing environment variable...
reg delete "HKLM\SYSTEM\CurrentControlSet\Control\Session Manager\Environment" /v CappuccinoPath /f

echo.
echo Cleanup complete.
echo IMPORTANT: You may need to restart your PC in order for the environment variable to be set properly!
echo.

pause