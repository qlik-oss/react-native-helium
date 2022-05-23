# @qlik/react-native-helium

<img src="logo/he2.png" height="256"/>

Skia base JSI renderer for Picasso and Nebula

## Building SKIA

1. clone skia
2. cd skia
3. python tools/git-sync-deps
4. generate the build command by choosing which platform listed below
5. ninja -C [whereever your outdir is]

### Android

```sh
bin/gn gen out/arm64 --args='target_os="android" target_cpu="arm64" skia_use_icu=false skia_use_piex=true skia_use_sfntly=false skia_use_system_expat=false skia_use_system_libjpeg_turbo=false skia_use_system_libpng=false skia_use_system_libwebp=false skia_use_system_zlib=false skia_enable_tools=false is_official_build=true skia_enable_skottie=false is_debug=false skia_enable_pdf=false skia_enable_flutter_defines=true paragraph_tests_enabled=false is_component_build=false ndk="/Users/vel/Library/Android/sdk/ndk/21.4.7075529" skia_use_system_freetype2=false skia_use_gl=true cc="clang" cxx="clang++"'
```

### iOS
#### Device
run ```xcrun --sdk iphoneos --show-sdk-path``` to get path to sdk
```sh
bin/gn gen out/ios64 --args='target_os="ios" target_cpu="arm64" skia_use_icu=false
skia_use_piex=true
skia_use_sfntly=false
skia_use_system_expat=false
skia_use_system_libjpeg_turbo=false
skia_use_system_libpng=false
skia_use_system_libwebp=false
skia_use_system_zlib=false
skia_enable_tools=false
is_official_build=true
skia_enable_skottie=false
is_debug=false
skia_enable_pdf=false
skia_enable_flutter_defines=true
paragraph_tests_enabled=false
is_component_build=false
skia_use_metal=true
skia_use_gl=true
cc="clang"
cxx="clang++"
ios_min_target="11.0"
xcode_sysroot="/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS15.4.sdk"
extra_ldflags=["--sysroot='/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS15.4.sdk'"]
extra_cflags=["-DSKIA_C_DLL", "-DHAVE_ARC4RANDOM_BUF", "-target", "arm64-apple-ios", "-fembed-bitcode"]
'
```

#### Sim
run ```xcrun --sdk iphonesimulator --show-sdk-path``` to get path to sdk
```sh
bin/gn gen out/iosSim --args='target_os="ios" target_cpu="x64" skia_use_icu=false
skia_use_piex=true
skia_use_sfntly=false
skia_use_system_expat=false
skia_use_system_libjpeg_turbo=false
skia_use_system_libpng=false
skia_use_system_libwebp=false
skia_use_system_zlib=false
skia_enable_tools=false
is_official_build=true
skia_enable_skottie=false
is_debug=false
skia_enable_pdf=false
skia_enable_flutter_defines=true
paragraph_tests_enabled=false
is_component_build=false
skia_use_metal=true
skia_use_gl=true
cc="clang"
cxx="clang++"
ios_min_target="11.0"
xcode_sysroot="/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator15.4.sdk"
extra_ldflags=["--sysroot='/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator15.4.sdk'"]
extra_cflags=["-DSKIA_C_DLL", "-DHAVE_ARC4RANDOM_BUF", "-target", "arm64-apple-ios-simulator"]
'
```

### Build framework
```sh
xcodebuild -create-xcframework -library ios64/libskia.a -headers iosheaders -library iosSim/libskia.a -headers iosheaders -output framework/skia.xcframework
```
where iosheaders has a copy of the skia/include folder
## License

MIT
