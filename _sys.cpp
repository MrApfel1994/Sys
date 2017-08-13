
#pragma warning(disable : 4996)

#include "AssetFile.cpp"
#include "AssetFileIO.cpp"
#include "Json.cpp"
#include "Pack.cpp"
#include "Time.cpp"

#if defined(WIN32) || defined(__linux__) || defined(__EMSCRIPTEN__)
    #include "PlatformSDL.cpp"
#endif
