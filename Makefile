COMPILING_TEXT 	   := @echo -e "\033[0m \033[0;33m Compiling:\033[0m"
COMPILING_TEXT_OK  := @echo -e "\033[0m \033[0;32m [All compiled succesfully]\033[0m"
LINKING_TEXT       := @echo -e "\033[0m \033[0;33m Linking...\033[0m"
LINKING_TEXT_OK    := @echo -e "\033[0m \033[0;32m [Linked succesfully]\033[0m"
ALL_CLEANED_TEXT   := @echo -e "\033[0m \033[0;32m [Cleaned succesfully]\033[0m"
#EXECUTING_TEXT     := @echo -e "\033[0m \033[5;32m Executing...\033[0m"
JUMP_LINE		   := @echo


ifdef DEBUG
	CXXFLAGS += -g
else
	CXXFLAGS += -O3
endif


SOURCES  	:= $(wildcard *.cpp)
OBJ_PATH    := obj
SRC_PATH	:= src
LIBS 	    := -L./lib/irrlicht -lIrrlicht -L./lib/fmod -lfmod -lfmodstudio -L./lib/bullet -lBulletSoftBody -lBulletDynamics -lBulletCollision -lLinearMath
INCLUDE     := -I. 
INCLUDE_IRR := -I /lib/irrlicht/irrlicht.h
INCLUDE_FMOD := -I ./include/fmod/core -I ./include/fmod/studio
INCLUDE_BULLET := -I./include/bullet -I./include
CC			:= g++
NAME_EXE	:= Beast_Brawl
CXXFLAGS 	+= -Wall -Wno-unknown-pragmas -std=c++17 # el no-unknown-pragmas es para que no salga el warning de los pragma region

ALLCPPS		:= $(shell find src/ -type f -iname *.cpp)
ALLCPPSOBJ	:= $(patsubst $(SRC_PATH)/%.cpp,$(OBJ_PATH)/%.o,$(ALLCPPS))
SUBDIRS		:= $(shell find src/ -type d)
OBJSUBDIRS  := $(patsubst $(SRC_PATH)%,$(OBJ_PATH)%,$(SUBDIRS))

CREATE_SYMLINKS := bash symlinks.sh


#Esto crea el ejecutable
$(NAME_EXE): $(OBJSUBDIRS) $(ALLCPPSOBJ)
	$(CREATE_SYMLINKS)
	$(COMPILING_TEXT_OK)
	$(JUMP_LINE)
	$(LINKING_TEXT)
	$(CC) -o $(NAME_EXE) $(patsubst $(SRC_PATH)%,$(OBJ_PATH)%,$(ALLCPPSOBJ)) $(INCLUDE) $(LIBS) 
	$(LINKING_TEXT_OK)
	$(JUMP_LINE)


#Esto compila y crea los .o (aqui aun no entra en juego el linker)
$(OBJ_PATH)/%.o: $(SRC_PATH)/%.cpp
	$(PRUEBA_TEXT)
	$(COMPILING_TEXT) $<
	@$(CC) $(CXXFLAGS) -o $@ -c $^ $(INCLUDE) $(INCLUDE_IRR) $(INCLUDE_FMOD) $(INCLUDE_BULLET)
	

$(OBJSUBDIRS):
	@mkdir -p $(OBJSUBDIRS)


info:
	$(info $(SUBDIRS))
	$(info $(ALLCPPS))
	$(info $(ALLCPPSOBJ))

.PHONY: exe
exe:
	$(CREATE_SYMLINKS)
	@sh ./$(NAME_EXE).sh

.PHONY: clean
clean:
	@rm -Rf $(OBJ_PATH)/ && rm -f $(NAME_EXE)
	$(ALL_CLEANED_TEXT)

.PHONY: all
all:
	@make clean ; make ; make exe
