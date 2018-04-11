ARTIFACT_NAME := irc
SRC_DIR := src
BUILD_DIR := $(SRC_DIR)/build
OUTPUT_DIR := $(BUILD_DIR)/output
OUTPUT_ARTIFACT_PATH = $(OUTPUT_DIR)/$(ARTIFACT_NAME)

compile:
	rm -rf $(BUILD_DIR)
	mkdir $(BUILD_DIR)
	mkdir $(OUTPUT_DIR)
	g++ -c $(SRC_DIR)/TcpClient.cpp -o $(BUILD_DIR)/TcpClient.o
	g++ -c $(SRC_DIR)/IrcClient.cpp -o $(BUILD_DIR)/IrcClient.o
	g++ -c $(SRC_DIR)/main.cpp -o $(BUILD_DIR)/main.o
	g++ $(BUILD_DIR)/*.o -o $(OUTPUT_ARTIFACT_PATH)

clean:
	rm -rf $(BUILD_DIR)

run:
	 ./$(OUTPUT_ARTIFACT_PATH)

full: clean compile run