compile:
	g++ -g ./src/main.cpp -o PostMartian `pkg-config gtkmm-3.0 --cflags --libs` -Wno-deprecated-declarations -I./include -std=c++17 -lPocoFoundation -lPocoNet  -lPocoNetSSL -lPocoUtil -lPocoXML -lPocoJSON
