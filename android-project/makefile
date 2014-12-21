DEVICE=-d #-e = emulator, -d = device (usb)

build:
	ndk-build ; ant -S debug

install: build
	adb $(DEVICE) install -r bin/SDLActivity-debug.apk

run: install
	adb shell am start -n com.phildogames.fourgenerals/com.phildogames.fourgenerals.FourGeneralsActivity

debug: run
	adb logcat -c && adb logcat | sed -n /FG/p

debug-unfiltered: run
	adb logcat -c && adb logcat

vactivity:
	vim src/com/phildogames/fourgenerals/FourGeneralsActivity.java

vsrc:
	vim jni/src/Android.mk

