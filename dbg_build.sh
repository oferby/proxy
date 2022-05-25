set -x

g++ -g *.cpp *.h network/tcp/*.cpp network/tcp/*.h network/*.h network/*.cpp network/roce/*.cpp network/roce/*.h proxy/*.h proxy/*.cpp  -o bin/proxy -levent -lpthread -levent_pthreads -libverbs
