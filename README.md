# codelive

# Installation (dev)
Install the dependencies of `drogon`. You most likely have all of them already, minus `jsoncpp`.
Install `mariadb` with dependencies, including `mariadb-libs`.

Clone the `drogon` repository.
```sh
git clone https://github.com/an-tao/drogon
cd drogon
git submodule update --init
mkdir build
cd build
cmake ..
make && sudo make install
```

Clone this repository
```sh
git clone https://github.com/atemmel/codelive
cd codelive
mkdir build
cd build
cmake ..
make && sudo ./clive
```

You can now access the site at `localhost` or `127.0.0.1`.

# Deployment
Install the dependencies of `drogon`. You most likely have all of them already, minus `jsoncpp`.
Install `mariadb` with dependencies, including `mariadb-libs`.

Clone the `drogon` repository.
```sh
git clone https://github.com/an-tao/drogon
cd drogon
git submodule update --init
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make && sudo make install
```

Clone this repository
```sh
git clone https://github.com/atemmel/codelive
cd codelive
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make && sudo ./clive &
```
