# win-templates

Contains ready-to-use Visual Studio prject templates that can build minimalistic Windows10 executables.  
There are three of them: .exe, .dll and .sys and thery are not dependent on VC Runtime. Conversely, they  
dependent on Microsoft's ucrtbase.dll.  
  
Driver building project also contains a post-build step of signing the .sys file with self-authorized  
test certificate, which allows to test the driver in Windows test mode.  

To disable signature verification for x64 drivers, run the command with administrative privileges:  
bcdedit /set testsigning off  
and restart Windows.  

