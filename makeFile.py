import os
name = "LobsterEngine"

os.system\
(
    f'echo "\n\n"\n'
    f'g++ src/* -I include/ -o {name}.exe `sdl2-config --cflags --libs`'
	f' -lGL -lGLU -lGLEW\n'
    f'chown layna ./{name}.exe\n'
    f'chmod +x ./{name}.exe\n'
    f'echo "COMPILATION COMPLETED!"\n'
    f'gdb ./{name}.exe\n'
)
