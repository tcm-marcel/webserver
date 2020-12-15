# Simple Webserver written in C++

Simple webserver implemented as a weekend project for learning purposes. Not intended to work out of the Box for most scenarios. Extended a few times with some smaller features like HTTP authorization. 

This will probably server as a base for future projects that need some HTTP functionality. 

## Build and Run Webserver

```
# Build binary
make bin/webserver -j 4
# Run example file handler on port 8080 with root path as home, and test/test as credentials
bin/webserver 8080 ~ test test
```

## Build and Run Unit Tests

```
# Build test binary
make tests -j 4
# Run unit tests
bin/unit_tests
```
