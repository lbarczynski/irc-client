ARTIFACT_NAME := irc
BUILD_DIR := src/build
OUTPUT_ARTIFACT_PATH = $(BUILD_DIR)/output/$(ARTIFACT_NAME)

compile:
	rm -rf $(BUILD_DIR)
	mkdir $(BUILD_DIR)
	g++ -c TcpClient.cpp -o $(BUILD_DIR)/TcpClient.o
	g++ -c IrcClient.cpp -o $(BUILD_DIR)/IrcClient.o
	g++ -c main.cpp -o $(BUILD_DIR)/main.o
	g++ $(BUILD_DIR)/*.o -o $(OUTPUT_ARTIFACT_PATH)

clean:
	rm -rf $(BUILD_DIR)
	
run:
	 ./$(OUTPUT_ARTIFACT_PATH)

full: clean compile run