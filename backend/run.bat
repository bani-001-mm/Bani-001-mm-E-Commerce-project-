@echo off
echo Starting AuraStore C++ Backend Server on port 8080...
if not exist bin\server.exe (
    echo [INFO] server.exe not found. Building first...
    call build.bat
)

bin\server.exe 8080
