# file-blocker
Simple console app that can be used to test use case when file access blocked by other process. 


## Build 
```
mkdir build 
cd build 
cmake -G "Visual Studio 15 2017 Win64"  ../   
cmake --build . --target ALL_BUILD --config Release -- /p:CharacterSet=Unicode
```

## Running 

This app block itself. 

When run without params it just runs and that block file from being rewriten or deleted. 

When run with first param `-l` it will try to get exclusive access to itself with `LockFileEx` so no other app can even read that file. 
