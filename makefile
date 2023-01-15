assembler: src/assembler.cpp inc/assembler.h inc/types.h inc/regexes.h src/mainAs.cpp
	g++ -o assembler src/assembler.cpp src/mainAs.cpp -Iinc -w
linker: src/linker.cpp inc/linker.h inc/types.h src/mainL.cpp
	g++ -o linker src/linker.cpp src/mainL.cpp -Iinc -w
emulator: src/emulator.cpp inc/emulator.h src/mainE.cpp
	g++ -o emulator src/emulator.cpp src/mainE.cpp -Iinc -w