import os
debugPath = "bin/debug"
name = "LobsterEngine"
extension = "elf"

os.system\
(
	f'echo "\n\n"\n'
	f'g++ src/* -I include/ -o {debugPath}/{name}.{extension} '
	f'-I vendor/ -lSDL2 -lSDL2_image'
	f' -lGL -lGLU -lGLEW -Wno-narrowing -Wfatal-errors\n'
	f'chown layna ./{debugPath}/{name}.{extension}\n'
	f'chmod +x ./{debugPath}/{name}.{extension}\n'
	f'echo "LINUX COMPILATION COMPLETED!"\n'
)

extension = "exe"
os.system\
(
	f'echo "\n\n"\n'
	f'x86_64-w64-mingw32-g++-win32 src/* -I include/ '
	f'-I vendor/ -o {debugPath}/{name}.{extension} '
	f'-L lib/ -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lopengl32 -lglu32 -lglew32 '
	f'-static-libstdc++ -static-libgcc -Wno-narrowing -Wfatal-errors\n'
	f'echo "WINDOWS COMPILATION COMPLETED!"\n'
)
