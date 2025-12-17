DED_FLAGS_WINDOWS =	-Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline \
					-Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum \
					-Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual \
					-Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2 \
					-Wignored-qualifiers -Wlogical-op -Wno-missing-field-initializers -Wnon-virtual-dtor \
					-Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing \
					-Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -Werror=vla -D_DEBUG -D_EJUDGE_CLIENT_SIDE

DED_FLAGS_LINUX = -D _DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations \
				  -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts \
				  -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal \
				  -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op \
				  -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self \
				  -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 \
				  -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override \
				  -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast \
				  -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast \
				  -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow \
				  -flto-odr-type-merging -fno-omit-frame-pointer -Wlarger-than=81920 -Wstack-usage=8192 -pie -fPIE -Werror=vla \
				  -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr

MODE ?=

UNAME_S := $(shell uname -s)

ifeq ($(OS), Windows_NT)
	FLAGS = $(DED_FLAGS_WINDOWS) -ITreeMemStruct
else
	FLAGS = $(DED_FLAGS_LINUX) -ITreeMemStruct
endif

all: math++

math++: ObjectFiles/Main.o        ObjectFiles/Tree.o            ObjectFiles/TreeDump.o \
		ObjectFiles/Instruments.o ObjectFiles/ReadingUserFile.o ObjectFiles/PrintFunctions.o \
		ObjectFiles/Derivative.o  ObjectFiles/Graph.o           ObjectFiles/SimplifyExpression.o \
		ObjectFiles/Teylor.o
	@ g++ $(FLAGS) $(MODE) ObjectFiles/Main.o        ObjectFiles/Tree.o            ObjectFiles/TreeDump.o \
						   ObjectFiles/Instruments.o ObjectFiles/ReadingUserFile.o ObjectFiles/PrintFunctions.o \
						   ObjectFiles/Derivative.o  ObjectFiles/Graph.o           ObjectFiles/SimplifyExpression.o -o math++ \
						   ObjectFiles/Teylor.o

ObjectFiles/Tree.o: TreeMemStruct/Tree.cpp TreeMemStruct/Tree.h TreeMemStruct/TreeBase.h
	@ g++ $(FLAGS) $(MODE) -c TreeMemStruct/Tree.cpp -o ObjectFiles/Tree.o

ObjectFiles/TreeDump.o: TreeMemStruct/TreeDump.cpp TreeMemStruct/TreeDump.h \
						TreeMemStruct/Tree.h TreeMemStruct/TreeBase.h
	@ g++ $(FLAGS) $(MODE) -c TreeMemStruct/TreeDump.cpp -o ObjectFiles/TreeDump.o

ObjectFiles/Main.o: Main.cpp Math_PP.h UndefDerivativeDSL.h DerivativeDSL.h
	@ g++ $(FLAGS) $(MODE) -c Main.cpp -o ObjectFiles/Main.o

ObjectFiles/Instruments.o: Instruments.cpp Instruments.h
	@ g++ $(FLAGS) $(MODE) -c Instruments.cpp -o ObjectFiles/Instruments.o

ObjectFiles/ReadingUserFile.o: ReadingUserFile.cpp ReadingUserFile.h Instruments.h
	@ g++ $(FLAGS) $(MODE) -c ReadingUserFile.cpp -o ObjectFiles/ReadingUserFile.o

ObjectFiles/PrintFunctions.o: PrintFunctions.cpp PrintFunctions.h Instruments.h
	@ g++ $(FLAGS) $(MODE) -c PrintFunctions.cpp -o ObjectFiles/PrintFunctions.o

ObjectFiles/Derivative.o: Derivative.cpp Derivative.h Instruments.h
	@ g++ $(FLAGS) $(MODE) -c Derivative.cpp -o ObjectFiles/Derivative.o

ObjectFiles/Graph.o: Graph.cpp Graph.h Instruments.h
	@ g++ $(FLAGS) $(MODE) -c Graph.cpp -o ObjectFiles/Graph.o

ObjectFiles/SimplifyExpression.o: SimplifyExpression.cpp SimplifyExpression.h Instruments.h
	@ g++ $(FLAGS) $(MODE) -c SimplifyExpression.cpp -o ObjectFiles/SimplifyExpression.o

ObjectFiles/Teylor.o: Teylor.cpp Teylor.h Instruments.h
	@ g++ $(FLAGS) $(MODE) -c Teylor.cpp -o ObjectFiles/Teylor.o

clean:
	rm ObjectFiles/*.o math++ TreeMemStruct/svg_dot/*.svg TreeMemStruct/pdf_dot/*.pdf
