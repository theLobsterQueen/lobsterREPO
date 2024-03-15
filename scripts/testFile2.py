# ADDS API MODULE SCRIPT DIRECTORY #
    # IN FUTURE THIS WOULD BE EXECUTED AT THE BEGINNING OF THE PROGRAM #
import sys
import os
sys.path.append("./modulesAPI/")

# IMPORTS AND INITIALIZATIONS #
import coremodule

# FUNCTION DEFINITIONS #
def start_function() :
    print("BINGUS")

# INITIALIZES THE FILE AS A LOBSTER API SCRIPT #
this = "testFile2"
coremodule.initialize(this)
coremodule.set_start(this, start_function)
coremodule.set_update(this, start_function)
