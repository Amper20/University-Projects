CFLAGS = /MD /nologo /W4 /EHsc /Za /D_CRT_SECURE_NO_WARNINGS  
build : so-cpp.exe

so-cpp.exe : so-cpp.obj
	cl $(CFLAGS) so-cpp.obj

so-cpp.obj: so-cpp.c
	cl $(CFLAGS) /c so-cpp.c