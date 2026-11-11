@echo off
setlocal

cmake -S . -B build -A x64
if errorlevel 1 goto :error

cmake --build build --config Release --target AntihumanBitcrush_VST3
if errorlevel 1 goto :error

echo.
echo Build complete. Look in:
echo build\AntihumanBitcrush_artefacts\Release\VST3\
goto :end

:error
echo.
echo Build failed. Check the CMake / Visual Studio output above.
exit /b 1

:end
endlocal
