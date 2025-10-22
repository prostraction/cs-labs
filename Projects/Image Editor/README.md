# ImageEditor

Fedora lib's path differs from default wx's:

```
export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
```

Compilation:

```
g++ -Wall -g `find . -name "*.cpp"` `wx-config --cxxflags --libs` -o bin/main && ./bin/main
```
