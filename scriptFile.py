import os
import time

compileAll = False

for file in os.listdir("./srcAPI") :
    moduleName = os.path.splitext(file)[0].lower()
    if file.endswith(".cpp") :
        toCompile = False
        try :
            if(os.path.getmtime(f"./srcAPI/{file}") > os.path.getmtime(f"./modulesAPI/_{moduleName}.so")) :
                toCompile = True
        except OSError :
            toCompile = True

        if toCompile or compileAll :
            os.system\
            (
                f'g++ ./srcAPI/{file} -I include/ -I vendor/' 
                f' -o ./modulesAPI/_{moduleName}.so'
                f' -shared -fPIC' # MAKES THE COMPILED .SO A "SHARED OBJECT" #
                f' -lpython3.10 `python3.10 -m pybind11 --includes` `python3.10-config --ldflags`'
                f' -Wno-attributes\n'
            )
        print(f"Compiled {file} into {moduleName}...")
print("Script compilation completed!")
