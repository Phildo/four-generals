#
# This makefile mainly forwards make commands to correct project
#
SRC_DIR=src
ANDROID_DIR=android-project
IOS_DIR=ios-project
PC_DIR=pc-project
TEST_DIR=tests

ANDROID_DEF=FG_ANDROID
IOS_DEF=FG_IOS
PC_DEF=FG_PC

#
# first to handle argumentless make
#
r: run
	

tags:
	ctags -R $(SRC_DIR)/*

#
# defaults (switch out for sugar w/r/t currently working-on platform)
#
vmake: pvmake
	

build: pbuild
	

install: pinstall
	

run: prun
	

debug: pdebug
	

kill: pkill
	


#
# android
#
adefine:
	sed -i.bak -E -e 's/define $(IOS_DEF)|define $(PC_DEF)/define $(ANDROID_DEF)/g' $(SRC_DIR)/defines.h && rm $(SRC_DIR)/defines.h.bak

avmake:
	vim $(ANDROID_DIR)/makefile

abuild: adefine
	cd $(ANDROID_DIR); amake build;

ainstall: adefine
	cd $(ANDROID_DIR); amake install;

arun: adefine
	cd $(ANDROID_DIR); amake run;

adebug: adefine
	cd $(ANDROID_DIR); amake debug;

akill:
	cd $(ANDROID_DIR); amake kill;

#
# ios
#
idefine:
	sed -i.bak -E -e 's/define $(ANDROID_DEF)|define $(PC_DEF)/define $(IOS_DEF)/g' $(SRC_DIR)/defines.h && rm $(SRC_DIR)/defines.h.bak

ivmake:
	vim $(IOS_DIR)/makefile

ibuild: idefine
	cd $(IOS_DIR); amake build;

iinstall: idefine
	cd $(IOS_DIR); amake install;

irun: idefine
	cd $(IOS_DIR); amake run;

idebug: idefine
	cd $(IOS_DIR); amake debug;

ikill:
	cd $(IOS_DIR); amake kill;


#
# pc
#
pdefine:
	sed -i.bak -E -e 's/define $(IOS_DEF)|define $(ANDROID_DEF)/define $(PC_DEF)/g' $(SRC_DIR)/defines.h && rm $(SRC_DIR)/defines.h.bak

pvmake:
	vim $(PC_DIR)/makefile

pbuild: pdefine
	cd $(PC_DIR); amake build;

pinstall: pdefine
	cd $(PC_DIR); amake install;

prun: pdefine
	cd $(PC_DIR); amake run;

prun4: pdefine
	cd $(PC_DIR); amake run4;

pdebug: pdefine
	cd $(PC_DIR); amake debug;

pdebug4: pdefine
	cd $(PC_DIR); amake debug4;

pkill:
	cd $(PC_DIR); amake kill;


#
# testing
#
tvmake:
	vim $(TEST_DIR)/makefile

tbuild: pdefine
	cd $(TEST_DIR); amake build;

tinstall: pdefine
	cd $(TEST_DIR); amake install;

trun: pdefine
	cd $(TEST_DIR); amake run;

tdebug: pdefine
	cd $(TEST_DIR); amake debug;

