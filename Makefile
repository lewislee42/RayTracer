SRC_DIR = src
SRC_FILES = main.cpp SDLObject.cpp SDLTextureObject.cpp Ray.cpp Object3D.cpp Object3DList.cpp Sphere.cpp Interval.cpp Camera.cpp Vec3.cpp Material.cpp
RASTER_FILES = utils.old.cpp SDLObject.cpp SDLTextureObject.cpp
HEADER_DIR = include
OBJ_DIR = obj

TARGET = sdl_test
SOURCES = $(addprefix	$(SRC_DIR)/, $(SRC_FILES))
OBJECTS = $(subst $(SRC_DIR),$(OBJ_DIR), $(SOURCES:.cpp=.o))
RASTER_SOURCES = $(addprefix	$(SRC_DIR)/, $(RASTER_FILES))
RASTER_OBJECTS = $(subst $(RASTER_FILES),$(OBJ_DIR), $(RASTER_SOURCES:.cpp=.o))
COMPILER = clang++
CXXFLAGS = -Wall -std=c++11 -I/usr/local/include -I$(HEADER_DIR) -fsanitize=address
LINKERFLAGS = -L/usr/local/lib -Wl,-rpath,/usr/local/lib -lSDL3 

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@echo "\033[96mcompiling \033[0;33m"$(TARGET)"\033[0m"
	@$(COMPILER) $(CXXFLAGS) $(LINKERFLAGS) -o $@ $^

Raster: $(RASTER_OBJECTS)
	@echo "\033[96mcompiling \033[0;33m"$(TARGET)"\033[0m"
	@$(COMPILER) $(CXXFLAGS) $(LINKERFLAGS) -o RasterTest $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@$(COMPILER) $(CXXFLAGS) -c $< -o $@

clean:
	@echo "\033[0;31mremoving files: \033[0m" $(OBJECTS)
	@rm -f $(OBJECTS)

fclean: clean
	@echo "\033[0;31mremoving file: \033[0m" $(TARGET)
	@rm -f $(TARGET)
	@rm -f RasterTest

re: fclean all

.PHONY = all clean fclean re Raster
