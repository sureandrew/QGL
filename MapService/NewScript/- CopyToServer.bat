for %%i in (*.lua) do luac -p %%i
xcopy /Y *.lua ..\..\..\..\Bin\Server\Data\NewScript\
pause
