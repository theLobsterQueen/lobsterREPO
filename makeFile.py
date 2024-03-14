import os
debugPath = "bin/debug"
name = "LobsterEngine"
extension = "elf"

os.system\
(
    f'echo "\n\n"\n'
    f'g++ ./src/* ./vendor/* -I ./include/ -o {debugPath}/{name}.{extension}'
    f' -I ./vendor/ -I /usr/include/python3.10 -lSDL2'
    f' -lGL -lGLU -lGLEW -lpython3.10 -Wno-narrowing'
    f' `python3.10 -m pybind11 --includes` `python3.10-config --ldflags`\n'
    f'chown layna ./{debugPath}/{name}.{extension}\n'
    f'chmod +x ./{debugPath}/{name}.{extension}\n'
    f'echo "LINUX COMPILATION COMPLETED!"\n'
)
