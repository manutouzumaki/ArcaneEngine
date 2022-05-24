@ECHO OFF

SET compilerFlags= -MD -Od -nologo -Gm- -GR- -Oi -WX -W3 -wd4530 -wd4201 -wd4100 -wd4189 -wd4505 -wd4101 -Zi
SET linkerFlags= -incremental:no User32.lib Gdi32.lib glfw3.lib Shell32.lib

SET includeGLFWPath=..\..\libs\glfw\include
SET linkerGLFWPath=..\..\libs\glfw\lib-vc2022

SET includeGLADPath=..\..\libs\glad\include
SET includeGLMPath=..\..\libs\glm

SET includeIMGUIPath=..\..\libs\imgui /I..\..\libs\imgui\backends
SET includeTINYXMLPath=..\..\libs\tinyxml

PUSHD ..\build
cl %compilerFlags% ..\components\*.cpp ..\renderer\*.cpp ..\util\*.cpp ..\src\*.cpp ..\src\*.c ..\..\libs\tinyxml\*.cpp ..\..\libs\imgui\backends\imgui_impl_glfw.cpp ..\..\libs\imgui\backends\imgui_impl_opengl3.cpp ..\..\libs\imgui\imgui*.cpp -FeArcaneEngine /I%includeTINYXMLPath% /I%includeIMGUIPath% /I%includeGLMPath% /I%includeGLFWPath% /I%includeGLADPath% /link /LIBPATH:%linkerGLFWPath% /SUBSYSTEM:CONSOLE %linkerFlags%
POPD
