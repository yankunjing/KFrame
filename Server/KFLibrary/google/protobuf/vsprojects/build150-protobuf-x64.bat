
:  build protobuf

"%VS150COMNTOOLS%..\IDE\Devenv" protobuf.sln /build "Debug|X64"
copy /y .\x64\Debug\libprotobuf.lib ..\..\..\..\_lib\win64\3rd\libprotobufd.lib
copy /y .\x64\Debug\libprotobuf-lite.lib ..\..\..\..\_lib\win64\3rd\libprotobuf-lited.lib

"%VS150COMNTOOLS%..\IDE\Devenv" protobuf.sln /build "Release|X64"
copy /y .\x64\Release\libprotobuf.lib ..\..\..\..\_lib\win64\3rd\libprotobuf.lib
copy /y .\x64\Release\libprotobuf-lite.lib ..\..\..\..\_lib\win64\3rd\libprotobuf-lite.lib
