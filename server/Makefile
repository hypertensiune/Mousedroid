SRC_DIR := src
OBJ_DIR := build/obj
INCLUDE_DIR := include
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))

OBJ_FILES += resource.o

INCLUDE := -I./include \
			-I$(ASIO)/include \
			-I$(WXWIN)\include \
			-I$(WXWIN)\lib\gcc_dll\mswu

LIBS := -L$(WXWIN)\lib\gcc_dll \
		-lwxmsw32u_core \
		-lwxbase32u \
		-lws2_32 -lwsock32 
		
FLAGS := -Wl,-subsystem,windows

OUTPUT_DIR := build/bin

main: $(OBJ_FILES)
	g++ -o $(OUTPUT_DIR)/mousedroid $^ $(LIBS) $(FLAGS)
	$(MAKE) copy_dlls
	
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(INCLUDE_DIR)/%.hpp
	@echo Compiling $<
	@g++ -c -o $@ $< $(INCLUDE)

resource:
	windres resource.rc resource.o -I$(WXWIN)\include

copy_dlls:
	@copy /y $(WXWIN)\lib\gcc_dll\wxbase32u_gcc_custom.dll build\bin
	@copy /y $(WXWIN)\lib\gcc_dll\wxmsw32u_core_gcc_custom.dll build\bin
	@copy /y app.ico build\bin
	@copy /y config.ini build\bin

clean:
	del build\obj\*.o