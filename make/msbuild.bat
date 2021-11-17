call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x86_amd64


set WDKPATH=C:\WinDDK\7600.16385.0
set MASM32PATH=C:\masm32

rem set latin encoding
chcp 850

if "%1"=="" (
    msbuild.exe win-templates.proj
) else (
    msbuild.exe "%1"
)

pause