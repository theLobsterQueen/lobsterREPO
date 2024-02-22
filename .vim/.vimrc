"Sets performance options"
:set mouse=
:set smartindent
:set tabstop=4
:set shiftwidth=0
:set autowrite
:set number

"Sets auto-compile command (must have a file called "COMPILE.sh" and/or"
	" "MAKE_LIB" in working directory.)"
:command COMPILE :!sudo python3 ./makeFile.py
:command RUN	 :!sudo python3 ./runFile.py
:command DEBUG 	 :!sudo python3 ./debugFile.py
:command MAKELIB :!sudo python3 ./makeLib.py

"Sets auto-compile commands for working in rust project directories."
:command RSCOMP :!echo COMPILING && cargo run

"Sets command to tag everything recursively in the working directory"
:command TAG :!ctags -R ./*

"Sets tab-movement commands."
:command Q :tabp
:command E :tabn

"Sets Directories..."

"...for init"
:command OpenInit :find ~/.vimrc
:command OpenBash :find ~/.bashrc

"...for misc docs"
:command OpenDOCS :cd /media/layna/WorkDisk/Docs

"...for Layna's Work Suite"
:command OpenWork :cd /media/layna/WorkDisk/Work/
:command OpenLGL :cd /media/layna/WorkDisk/Work/LWSRepo/Layna's Graphical Library
:command OpenFontMaker :cd /media/layna/WorkDisk/Work/LWSRepo/Layna's Font Maker

"...for School Stuff"
:command OpenRealtime :cd /media/layna/WorkDisk/School/Third_Year/Second_Semester/Realtime
:command OpenEngine :cd /media/layna/WorkDisk/School/Third_Year/Second_Semester/Realtime/engine
:command OpenCon :cd /media/layna/WorkDisk/School/Third_Year/Second_Semester/Concurrency/

"Set colors if opening VIM with a command argument."
:color industry
:set bg=dark

"Set Colors if opening VIM without command arguments"
if argc() == 0
	:let loaded_matchparen = !has("running_gui")
:endif
