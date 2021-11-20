@echo off
rem +--------------------------------------------------------------------------------------------------------------------+
rem | This script creates a test certificate, adds it to the local store and signs the given driver with it.             |
rem | Note, that MakeCert.exe and SignTool.exe utilities are taken from Microsoft Windows SDK.                           |
rem +--------------------------------------------------------------------------------------------------------------------+

if "%1" == "" (
  echo Usage: %0 file_to_sign
  goto :eof
) else (
  goto StartSigning
)

:StartSigning
set SIGNTOOLS=.
set FILEPATH=%~f1
set CERTPATH=TestCert.cer
set CERTNAME="Driver-Template"
set STORENAME="Driver-Template Test Store"

if not exist %FILEPATH% (
  echo File %1 does not exist. Exiting.
  goto :eof
)

goto SignTool

:MakeCert
echo Generating test certificate file...
"%SIGNTOOLS%\MakeCert.exe" -r -pe -ss %STORENAME% -n "CN=%CERTNAME%" "%CERTPATH%"

:SignTool
echo Signing the file with generated certificate...
"%SIGNTOOLS%\SignTool.exe" sign /v /s %STORENAME% /n "%CERTNAME%" "%FILEPATH%" 2> nul

if errorlevel == 1 goto MakeCert




