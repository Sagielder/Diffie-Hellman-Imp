# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -std=c++17
LDFLAGS = -lws2_32

# Directories
NETWORK_DIR = Network
CLIENT_DIR = Client

# Source files
NETWORK_SOURCES = $(NETWORK_DIR)/main.cpp $(NETWORK_DIR)/network.cpp
CLIENT_SOURCES = $(CLIENT_DIR)/main.cpp $(CLIENT_DIR)/Client.cpp

# Output executables
NETWORK_EXE = network.exe
CLIENT_EXE = client.exe

# Default target - build everything
all: $(NETWORK_EXE) $(CLIENT_EXE)

# Build network process
$(NETWORK_EXE): $(NETWORK_SOURCES)
	$(CXX) $(CXXFLAGS) $(NETWORK_SOURCES) -o $(NETWORK_EXE) $(LDFLAGS)

# Build client
$(CLIENT_EXE): $(CLIENT_SOURCES)
	$(CXX) $(CXXFLAGS) $(CLIENT_SOURCES) -o $(CLIENT_EXE) $(LDFLAGS)

# Build only network
network: $(NETWORK_EXE)

# Build only client
client: $(CLIENT_EXE)

# Clean up executables
clean:
	del /Q $(NETWORK_EXE) $(CLIENT_EXE) 2>nul || echo "Clean complete"

# Run network process
run-network: $(NETWORK_EXE)
	./$(NETWORK_EXE)

# Run client
run-client: $(CLIENT_EXE)
	./$(CLIENT_EXE)

# Help target
help:
	@echo Available targets:
	@echo   all          - Build both network and client (default)
	@echo   network      - Build only network process
	@echo   client       - Build only client
	@echo   clean        - Remove executables
	@echo   run-network  - Build and run network process
	@echo   run-client   - Build and run client
	@echo   help         - Show this help message

.PHONY: all network client clean run-network run-client help