

#### `How to build on windows`
use cygwin64 normaly found in C:/cygwin64

For the server
```shell
    git clone https://github.com/anzerr/tic-tac-toe.git
    cd tic-tac-toe/sever
    make re
    ./server --port 980
```

For the client the client is the same to start use this
```shell
    ./client --port 980 --ip 127.0.0.1
```

#### `How to build on linux`
```shell
    sudo apt-get update
    sudo apt-get install -y --no-install-recommends gcc-5 build-essential
    git clone https://github.com/anzerr/tic-tac-toe.git tic
    cd tic/lib/lib_src
    make re
    cp libmy.a ../../server
    cd ../../server
    make re
    chmod server
    ./server --port 980
```

#### `How to build docker image`
Build docker image and run the server
```shell
    docker build . -t tic/server:1.0
    docker run -p 980:980 --name server tic/server:1.0
```

#### `WHY`
This is shit and has no point
