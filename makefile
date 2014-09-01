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
# defaults (switch out for sugar w/r/t currently working-on platform)
#
build: abuild
	

install: ainstall
	

run: arun
	

debug: adebug
	


#
# android
#
adefine:
	sed -i.bak -E -e 's/$(IOS_DEF)|$(PC_DEF)/$(ANDROID_DEF)/g' $(SRC_DIR)/defines.h && rm $(SRC_DIR)/defines.h.bak

abuild: adefine
	cd $(ANDROID_DIR); amake build;

ainstall: adefine
	cd $(ANDROID_DIR); amake install;

arun: adefine
	cd $(ANDROID_DIR); amake run;

adebug: adefine
	cd $(ANDROID_DIR); amake debug;

#
# ios
#
idefine:
	sed -i.bak -E -e 's/$(ANDROID_DEF)|$(PC_DEF)/$(IOS_DEF)/g' $(SRC_DIR)/defines.h && rm $(SRC_DIR)/defines.h.bak

ibuild: idefine
	cd $(IOS_DIR); amake build;

iinstall: idefine
	cd $(IOS_DIR); amake install;

irun: idefine
	cd $(IOS_DIR); amake run;

idebug: idefine
	cd $(IOS_DIR); amake debug;

#
# pc
#
pdefine:
	sed -i.bak -E -e 's/$(IOS_DEF)|$(ANDROID_DEF)/$(PC_DEF)/g' $(SRC_DIR)/defines.h && rm $(SRC_DIR)/defines.h.bak

pbuild: pdefine
	cd $(PC_DIR); amake build;

pinstall: pdefine
	cd $(PC_DIR); amake install;

prun: pdefine
	cd $(PC_DIR); amake run;

pdebug: pdefine
	cd $(PC_DIR); amake debug;

#
# testing
#
tbuild: pdefine
	cd $(TEST_DIR); amake build;

tinstall: pdefine
	cd $(TEST_DIR); amake install;

trun: pdefine
	cd $(TEST_DIR); amake run;

tdebug: pdefine
	cd $(TEST_DIR); amake debug;

