# SHA-1 implementation in C++

## Warning

Do not use SHA-1 unless you have to! 
[SHA-1 is practically broken](https://en.wikipedia.org/wiki/SHA-1#Birthday-Near-Collision_Attack_%E2%80%93_first_practical_chosen-prefix_attack). Use a hash function from the [SHA-2](https://en.wikipedia.org/wiki/SHA-2) or [SHA-3](https://en.wikipedia.org/wiki/SHA-3) family instead.

## How to build tests

Using cmake
```
mkdir build
cd build
cmake ..
make
```

## License

100% Public Domain

## Authors

- Steve Reid (Original C Code)
- [Bruce Guenter](http://untroubled.org/) (Small changes to fit into bglibs)
- [Volker Diels-Grabsch](https://njh.eu/) (Translation to simpler C++ Code)
- [Eugene Hopkinson](https://riot.so/) (Safety improvements)
- [Zlatko Michailov](http://zlatko.michailov.org) (Header-only library)
- [Dan Machado](dan-machado@yandex.com) (Refactoring)
