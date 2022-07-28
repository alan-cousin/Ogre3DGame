@echo off
set /p nombre="Crea una nueva clase: "
set "tab=	"
if exist %nombre%.h (
echo "Ya existe el archivo"
) else (

echo #pragma once > %nombre%.h
echo class %nombre% { >> %nombre%.h
echo public: >> %nombre%.h
echo %tab%%nombre%^(^); >> %nombre%.h
echo %tab%~%nombre%^(^); >> %nombre%.h
echo }; >> %nombre%.h



)

if exist %nombre%.cpp (
echo "Ya existe el archivo"
) else (
echo #include "%nombre%.h"  > %nombre%.cpp
echo. >> %nombre%.cpp
echo %nombre%::%nombre%^(^) { >>%nombre%.cpp
echo } >> %nombre%.cpp
echo %nombre%::~%nombre%^(^) { >>%nombre%.cpp
echo } >> %nombre%.cpp
 
)

if exist ..\..\Projects\Application\Application.vcxproj (
cscript CreateClass.vbs
)



pause