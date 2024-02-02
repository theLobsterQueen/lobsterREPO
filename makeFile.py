import os
debugPath = "bin/debug"
name = "LobsterEngine"
extension = "elf"

os.system\
(
    f'echo "\n\n"\n'
    f'g++ src/* -I include/ -o {debugPath}/{name}.{extension} '
	f'`sdl2-config --cflags --libs`'
	f' -lGL -lGLU -lGLEW\n'
    f'chown layna ./{debugPath}/{name}.{extension}\n'
    f'chmod +x ./{debugPath}/{name}.{extension}\n'
    f'echo "LINUX COMPILATION COMPLETED!"\n'
)

extension = "exe"

os.system\
(
	f'echo "\n\n"\n'
    f'x86_64-w64-mingw32-g++-posix src/* -I include/ -o {debugPath}/{name}.{extension} '
	f'-lmingw32 -lSDL2main -lSDL2 -lopengl32 -lglu32 -lglew32\n'
    f'chown layna ./{debugPath}/{name}.{extension}\n'
    f'chmod +x ./{debugPath}/{name}.{extension}\n'
    f'echo "WINDOWS COMPILATION COMPLETED!"\n'
)
