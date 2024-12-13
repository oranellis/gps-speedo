SRC_DIRS := src lib

EXTENSIONS := cpp h hpp cc cxx

default: build

build:
	@echo "Building"
	@pio run

flash:
	@echo "Building and flashing pico"
	@pio run --target upload

compiledb:
	@echo "Generating compile_commands.json"
	@pio run --target compiledb

format:
	@echo "Formatting source files in $(SRC_DIRS)"
	@$(foreach dir, $(SRC_DIRS), \
		$(foreach ext, $(EXTENSIONS), \
		find $(dir) -type f -name '*.$(ext)' -exec clang-format -i {} +;))
