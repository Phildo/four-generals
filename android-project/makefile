build: bin/SDLActivity-debug.apk
	ndk-build ; ant -S debug

install: build
	adb uninstall com.phildogames.fourgenerals ; adb -d install bin/SDLActivity-debug.apk

run: install
	adb shell am start -n com.phildogames.fourgenerals/com.phildogames.fourgenerals.FourGeneralsActivity

debug: run
	adb logcat -c && adb logcat | sed -n /FG/p

