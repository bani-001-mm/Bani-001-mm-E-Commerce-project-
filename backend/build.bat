@echo off
echo ========================================================
echo  Compiling AuraStore C++ Backend Server
echo ========================================================

if not exist bin mkdir bin
if not exist data mkdir data

g++ -std=c++14 -O2 main.cpp database.cpp http_server.cpp -lws2_32 -o bin\server.exe

if %ERRORLEVEL% EQU 0 (
    echo [SUCCESS] Compilation complete: bin\server.exe
) else (
    echo [ERROR] Compilation failed with error code %ERRORLEVEL%
)
