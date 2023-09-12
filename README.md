# proxy

simple TCP proxy

### install libevent
sudo apt install libevent-dev


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


# Testing

## Server Proxy
    ./proxy 192.168.70.6 5000 ROCE hrn0_1 localhost 8080 TCP

## Client Proxy
    ./proxy localhost 8081 TCP 192.168.70.6 5000 ROCE hrn0_0


## Server
    sudo  docker run -p 8080:8080 -p 8079:8079 fortio/fortio server

## Client
    docker run fortio/fortio load -t 20s -qps 1000 http://192.168.70.4:8081
