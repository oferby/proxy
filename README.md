# proxy

simple TCP proxy

### compile
    g++ *.cpp *.h tcp/*.cpp tcp/*.h network/*.h network/*.cpp proxy/*.h proxy/*.cpp  -o bin/proxy -levent -lpthread -levent_pthreads


### NC server
    nc -k -l 8586


### run
    ./proxy localhost 8585 TCP localhost 8586 TCP
