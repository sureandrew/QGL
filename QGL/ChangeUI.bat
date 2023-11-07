@set PATH=..\Library\swigwin-1.3.31;%PATH%

swig -c++ -lua UI.i
swig -c++ -lua CommonI.i
swig -c++ -lua LuaI.i
pause