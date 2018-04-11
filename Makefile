ARTIFACT_NAME := irc
BUILD_DIR := build

compile:
	rm -rf $(BUILD_DIR)
	mkdir $(BUILD_DIR)
	g++ -c TcpClient.cpp -o $(BUILD_DIR)/TcpClient.o
	g++ -c IrcClient.cpp -o $(BUILD_DIR)/IrcClient.o
	g++ -c main.cpp -o $(BUILD_DIR)/main.o
	g++ $(BUILD_DIR)/*.o -o $(ARTIFACT_NAME)

clean:
	rm -rf $(ARTIFACT_NAME)

run:
	 ./$(ARTIFACT_NAME)

full: clean compile run