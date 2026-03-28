CXX      = g++
STRIP    = strip
OUT_NAME = samp_discord.asi
SRC_DIR  = src

# --- Compiler Flags ---
# -m32: create a 32-bit binary for compatibility with SA-MP, which is a 32-bit application.
# -Os: optimize for small file size
# -Wall: show all warnings
# -static-libgcc -static-libstdc++: resolves all dependencies into the final .asi, ensuring no external DLLs are needed.
# -I$(SRC_DIR): Tells the compiler to look in 'src/' for includes so "discord-rpc/discord_rpc.h" resolves correctly.
CXXFLAGS = -m32 -Os -Wall -static-libgcc -static-libstdc++ -I$(SRC_DIR)

# --- Linker Flags ---
# -shared: Create a DLL/ASI (dynamic library)
# -Wl,--subsystem,windows:5.1: to support older Windows versions (XP and later) since SA-MP is compatible with those.
# -L$(SRC_DIR)/discord-rpc: Tells the linker where to find discord-rpc.lib
# -ldiscord-rpc: link the discord rpc library remember to use the library that is supported or compiled by your compiler
# -lws2_32: socket library for communicating with samp server (query.h)
# -lwininet: http request library for making http requests (http.h)
LDFLAGS  = -shared -Wl,--subsystem,windows:5.1 -L$(SRC_DIR)/discord-rpc -ldiscord-rpc -lws2_32 -lwininet

# --- Source Files ---
# Automatically grabs all .cpp files directly inside the src/ folder.
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)

all: $(OUT_NAME)

$(OUT_NAME): $(SOURCES)
	@echo [BUILDING] Generating $(OUT_NAME)...
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)
	@echo [STRIPPING] Removing debug symbols...
	$(STRIP) $@
	@echo [SUCCESS] Build Complete!

clean:
	@if exist $(OUT_NAME) del $(OUT_NAME)
	@echo [CLEAN] Removed $(OUT_NAME)