NASM = ../z_tools/nasm.exe
OBJS_PACK = ./out/entry.obj ./out/syscall.obj ./out/rand.obj ./out/arg.obj \
			./out/mouse.obj ./out/math.obj ./out/libc.obj ./out/net_syscall.obj
OBJS_GUI = ./out/WinEntry.obj ./out/syscall.obj ./out/rand.obj ./out/arg.obj \
			./out/math.obj ./out/gui_syscall.obj ./out/libc.obj ./out/net_syscall.obj
OBJS_PACK_CPP = ./out/CppEntry.obj ./out/syscall.obj ./out/rand.obj ./out/arg.obj \
			./out/mouse.obj ./out/math.obj ./out/libc.obj ./out/net_syscall.obj
build : $(OBJS_PACK) $(OBJS_GUI) $(OBJS_PACK_CPP) game.bin gobang.bin memz.bin Makefile
	build bainian $(OBJS_PACK)
	build test $(OBJS_GUI)
	build uigobang $(OBJS_GUI)
	build bitz $(OBJS_PACK)
	build calc $(OBJS_PACK)
	build AIGOBANG $(OBJS_PACK)
	build RandNum $(OBJS_PACK)
	build cale $(OBJS_PACK)
	build pfn $(OBJS_PACK)
	build oi $(OBJS_PACK)
	build sort $(OBJS_PACK)
	build copy $(OBJS_PACK)
	build ps2test $(OBJS_PACK)
	build pwsh $(OBJS_PACK)
	build snake $(OBJS_PACK)
	build cgobang $(OBJS_PACK)
	build CREUS $(OBJS_PACK)
	build bf $(OBJS_PACK)
	build edit $(OBJS_PACK)
	build cc $(OBJS_PACK)
	build uicalc $(OBJS_GUI)
	build invader $(OBJS_GUI)
	build winver $(OBJS_GUI)
	build login $(OBJS_GUI)
	build student $(OBJS_GUI)
	build tetris $(OBJS_GUI)
	build basic $(OBJS_PACK)
	build setup1 $(OBJS_PACK)
	build Maze $(OBJS_PACK)
	build clock $(OBJS_PACK)
	build nettest $(OBJS_PACK)
	buildpp editor $(OBJS_PACK_CPP)
	build lua $(OBJS_PACK)
	buildpp SPGAME $(OBJS_PACK_CPP)
	buildpp CodeEdit $(OBJS_PACK_CPP)
clean :
	del out /s/q

./out/%.obj : ./src/%.c Makefile
	gcc -m32 -I./include -nostdinc -nostdlib -fno-builtin -ffreestanding -fno-stack-protector -Qn -fno-pic -fno-pie -fno-asynchronous-unwind-tables -mpreferred-stack-boundary=2 -fomit-frame-pointer -mno-mmx -mno-sse -mfpmath=387 -Os -w -c ./src/$*.c -o ./out/$*.obj
./out/%.obj : ./src/%.cpp Makefile
	gcc -m32 -I./include -nostdinc -nostdlib -fno-builtin -ffreestanding -fno-stack-protector -Qn -fno-pic -fno-pie -fno-asynchronous-unwind-tables -mpreferred-stack-boundary=2 -fomit-frame-pointer -mno-mmx -mno-sse -mfpmath=387 -Os -w -c ./src/$*.cpp -o ./out/$*.obj
%.bin : ./src/%.asm Makefile
	$(NASM) ./src/$*.asm -o ./out/$*.bin

./out/%.obj : src/%.asm Makefile
	"../z_tools/nasm.exe" --prefix _ -f win32 src/$*.asm -o ./out/$*.obj
