# win-templates

Contains ready-to-use Visual Studio prject templates that can build minimalistic Windows10 executables which are not dependent on VC Runtime,<br/>
but are dependent on Microsoft's ucrtbase.dll. There are three project templates: .exe, .dll and .sys. Driver building project also contains<br/>
a post-build step of signing the .sys file with self-authorized test certificate, which allows to test the driver in Windows test mode.<br/>
