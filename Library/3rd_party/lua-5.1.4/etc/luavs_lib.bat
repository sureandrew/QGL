rem script to build Lua under "Visual Studio .NET Command Prompt".
rem do not run it from this directory, run it from the toplevel: etc\luavs.bat
rem it creates lua51.dll, lua51.lib, lua.exe, and luac.exe in src.

cd src
rem generate debug lib
cl /MDd /Od /W3 /Zi /Zp4 /c /D_DEBUG /D_CRT_SECURE_NO_DEPRECATE /D_CRT_NONSTDC_NO_DEPRECATE /Fd"..\lua\Debug\lua5.1d.pdb" l*.c
del lua.obj luac.obj
lib.exe /nologo /out:..\lua\Debug\lua5.1d.lib l*.obj
del *.obj

rem generate release lib
cl /MD /O2 /W3 /c /Zp4 /D_CRT_SECURE_NO_DEPRECATE /D_CRT_NONSTDC_NO_DEPRECATE l*.c
del lua.obj luac.obj
lib.exe /nologo /out:..\lua\Release\lua5.1.lib l*.obj
del *.obj

cd ..
