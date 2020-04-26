    g++ src/*.cpp src/*/*.cpp -pthread -std=c++17 `pkg-config --cflags --libs glib-2.0 gio-2.0`
