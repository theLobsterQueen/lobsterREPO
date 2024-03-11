import os
debugPath = "bin/debug"
name = "LobsterEngine"
extension = "elf"
compileLinux = False

if compileLinux :
    os.system\
    (
        f'echo "\n\n"\n'
        f'g++ src/* vendor/* -I include/ -o {debugPath}/{name}.{extension} '
        f'-I vendor/ -lSDL2'
        f' -lGL -lGLU -lGLEW -Wno-narrowing -Wfatal-errors\n'
        f'chown layna ./{debugPath}/{name}.{extension}\n'
        f'chmod +x ./{debugPath}/{name}.{extension}\n'
        f'echo "LINUX COMPILATION COMPLETED!"\n'
    )

else :
    extension = "exe"
    os.system\
    (
        f'echo "\n\n"\n'
        f'x86_64-w64-mingw32-g++-win32 src/* vendor/* -I include/ '
        f'-I vendor/ -o {debugPath}/{name}.{extension} '
        f'-L lib/ -lmingw32 -lSDL2main -lSDL2 -lopengl32 -lglu32 -lglew32 '
        f'-static-libstdc++ -static-libgcc -Wno-narrowing -Wfatal-errors\n'
        f'echo "WINDOWS COMPILATION COMPLETED!"\n'
    )
