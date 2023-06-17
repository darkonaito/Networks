g++ -c *.cpp buffer/buffer.cpp -Ofast -std=c++23 -static-libgcc -static-libstdc++ -static -fext-numeric-literals -lws2_32

ar rcs dnw.a *.o 

del *.o