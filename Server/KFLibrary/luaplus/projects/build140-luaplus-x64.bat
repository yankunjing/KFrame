
:  build luaplus

"%VS140COMNTOOLS%..\IDE\Devenv" LuaPlus.sln /build "Debug|X64"
copy /y .\x64\Debug\LuaPlusd.lib ..\..\..\_lib\win64\LuaPlusd.lib


"%VS140COMNTOOLS%..\IDE\Devenv" LuaPlus.sln /build "Release|X64"
copy /y .\x64\Release\LuaPlus.lib ..\..\..\_lib\win64\LuaPlus.lib
