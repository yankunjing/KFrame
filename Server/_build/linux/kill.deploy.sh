ps -ef|grep "deployserver" | grep -v grep | cut -c 9-15 | xargs kill -9
