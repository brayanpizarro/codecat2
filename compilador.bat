@echo off
echo Iniciando proceso de compilacion...

echo.
echo Generando parser.tab.c y parser.tab.h con Bison...
bison -d parser.y
if %errorlevel% neq 0 (
    echo Error al generar archivos con Bison.
    goto error
)

echo.
echo Generando lex.yy.c con Flex...
flex scanner.l
if %errorlevel% neq 0 (
    echo Error al generar archivo con Flex.
    goto error
)

echo.
echo Compilando ast.c...
gcc -D_POSIX_C_SOURCE=200809L -c ast.c
if %errorlevel% neq 0 (
    echo Error al compilar ast.c.
    goto error
)

echo.
echo Compilando parser.tab.c...
gcc -D_POSIX_C_SOURCE=200809L -c parser.tab.c
if %errorlevel% neq 0 (
    echo Error al compilar parser.tab.c.
    goto error
)

echo.
echo Compilando lex.yy.c...
gcc -D_POSIX_C_SOURCE=200809L -c lex.yy.c
if %errorlevel% neq 0 (
    echo Error al compilar lex.yy.c.
    goto error
)

echo.
echo Enlazando objetos...
gcc -o analizador ast.o parser.tab.o lex.yy.o -lm
if %errorlevel% neq 0 (
    echo Error al enlazar objetos.
    goto error
)

echo.
echo Compilacion completada con exito. El ejecutable 'analizador' ha sido creado.
goto end

:error
echo.
echo Ha ocurrido un error durante la compilacion.

:end
pause