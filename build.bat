@echo off
setlocal enabledelayedexpansion

REM コンパイラとフラグの設定
set CC=gcc
set CFLAGS=-Wall -Wextra -O2
set LDFLAGS=

REM ディレクトリ設定
set SRCDIR=src
set BUILDDIR=build
set BINDIR=bin
set TARGET=tetris

REM 必要なディレクトリの作成
if not exist %BUILDDIR% mkdir %BUILDDIR%
if not exist %BINDIR% mkdir %BINDIR%

REM ソースファイルのリスト作成
set OBJECTS=
for %%f in (%SRCDIR%\*.c) do (
    set "OBJECTS=!OBJECTS! %BUILDDIR%\%%~nf.o"
)

REM ソースファイルのコンパイル
for %%f in (%SRCDIR%\*.c) do (
    echo Compiling %%f...
    %CC% %CFLAGS% -c %%f -o %BUILDDIR%\%%~nf.o
    if errorlevel 1 exit /b 1
)

REM オブジェクトファイルをリンクして実行ファイルを生成
echo Linking %TARGET%.exe...
%CC% %LDFLAGS% -o %BINDIR%\%TARGET%.exe !OBJECTS!
if errorlevel 1 exit /b 1

echo Build completed successfully.
exit /b 0
