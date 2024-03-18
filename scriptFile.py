import os

for file in os.listdir("./srcAPI") :
    fileName = os.path.splitext(file)[0].lower()
    if file.endswith(".swp") :
        continue

    os.system\
    (
        f'g++ ./srcAPI/{file} ./src/globals.cpp -I include/ -I vendor/' 
        f' -o ./modulesAPI/_{fileName}.so'
        f' -shared -fPIC' # MAKES THE COMPILED .SO A "SHARED OBJECT," SO IT CAN BE USED BY PYTHON
        f' -lpython3.10 `python3.10 -m pybind11 --includes` `python3.10-config --ldflags`'
        f' -Wno-attributes\n'
        f'echo "Compiled {file}!"'
    )
