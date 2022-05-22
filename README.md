# proxy

simple TCP proxy

### compile
    g++ *.cpp *.h tcp/*.cpp tcp/*.h network/*.h network/*.cpp proxy/*.h proxy/*.cpp  -o bin/proxy -levent -lpthread -levent_pthreads


### NC server
    nc -k -l 8586


### run
    ./proxy localhost 8585 TCP localhost 8586 TCP



### utilities

    ibdev2netdev
    ibv_devices
    sudo /usr/local/bin/hiroce gids


## memlock
    check that we have enough memory using: ulimit -l
    if needed, edit /etc/security/limits.conf
