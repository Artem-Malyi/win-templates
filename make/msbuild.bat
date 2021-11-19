call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x86_amd64

rem Important: if the path contains spaces, escape them with ^. Do not quote the path!

set WDKINCPATH=C:\Program^ Files^ (x86)\Windows^ Kits\10\Include\10.0.19041.0
set WDKLIBPATH=C:\Program^ Files^ (x86)\Windows^ Kits\10\Lib\10.0.19041.0
set MASM32PATH=C:\masm32

rem set latin encoding
chcp 850

if "%1"=="" (
    msbuild.exe win-templates.proj
) else (
    msbuild.exe "%1"
)

pause