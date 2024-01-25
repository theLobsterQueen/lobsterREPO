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
    f'echo "COMPILATION COMPLETED!"\n'
    f'gdb ./{debugPath}/{name}.{extension}\n'
)
