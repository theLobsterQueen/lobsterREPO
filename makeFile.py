import os
import datetime

debugPath = "bin/debug"
name = "LobsterEngine"
extension = "elf"
compileAll = False
build = False

def compileDir(dirPath) :
    toCompile = []

    # FINDS ALL FILES IN THE SOURCE DIRECTORY, DETERMINES WHETHER OR NOT #
        # THEY HAVE BEEN MODIFIED SINCE LAST COMPILATION. #
        # ADDS FILES TO COMPILE ARRAY IF VALID #
    for file in os.listdir(dirPath) :
        if file.endswith(".cpp") :
            if compileAll :
                toCompile.append(file)
                continue

            fileName = os.path.splitext(file)[0] + ".o"
            try :
                if(os.path.getmtime(dirPath + file) >= os.path.getmtime(f"./bin/prec/{fileName}")) :
                    toCompile.append(file)

            except OSError :
                toCompile.append(file)

    # COMPILES SRC FILES INTO BINARY FILES #
    for srcFile in toCompile :
        fileName = os.path.splitext(srcFile)[0] + ".o"
        os.system\
        (
            f'g++ -c {dirPath + srcFile} -I ./include/ -I ./vendor/ -I /usr/include/python3.10 -lSDL2'
            f' -lGL -lGLU -lGLEW -lpython3.10 -Wno-narrowing -Wno-attributes -Wno-format-security'
            f' `python3.10 -m pybind11 --includes` `python3.10-config --ldflags` -o ./bin/prec/{fileName}'
        )
        print(f"Compiled {srcFile}...")

compileDir("./src/")
compileDir("./vendor/")
print("Compilation done!")

# LINKS ALL OBJECTS INTO A SINGLE EXECUTABLE #
os.system\
(
    f'g++ ./bin/prec/*.o -lSDL2 -lGL -lGLU -lGLEW -lpython3.10'
    f'  -Wno-narrowing -Wno-attributes `python3.10 -m pybind11 --includes`'
    f'  `python3.10-config --ldflags` -o bin/debug/LobsterEngine.{extension}'
)
print("Linking done!")

# IF BUILD IS ACTIVE, ALSO RUNS THE SCRIPT COMPILATION FILE AND THEN LINKS #
    # ALL COMPILED CODE INTO A SINGLE ARCHIVE #
if build :
    os.system("python3 ./scriptFile.py")
    print("Building compiled script archive!")
    os.system("ar rs ./bin/libLobsterEngine.a ./bin/prec/*.o")
    print("Building done!")
