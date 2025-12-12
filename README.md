# SHA-1 implementation in C++

## Warning

Do not use SHA-1 unless you have to. [SHA-1 is practically broken](https://en.wikipedia.org/wiki/SHA-1#Birthday-Near-Collision_Attack_%E2%80%93_first_practical_chosen-prefix_attack). 
Use a hash function from the [SHA-2](https://en.wikipedia.org/wiki/SHA-2) or [SHA-3](https://en.wikipedia.org/wiki/SHA-3) family instead.

- Despite been proven that SHA-1 has collision attacks, SHA-1 is still extensively used, for example in
websocket protocol.

- For security related purposes one should use a combination of at least two hashes. For example in pseudocode:
   - sha2(sha1(data)) not very good
   - sha1(data)+sha2(data) better
   - sha1(data.part1)+sha2(data.part2)+sha3(data) strong (where data=data.part1+data.part2)


## To build the tests
```
cd tests
mkdir build
cd build
cmake .. 
```

## License

100% Public Domain

## Authors

- Steve Reid (Original C Code)
- [Bruce Guenter](http://untroubled.org/) (Small changes to fit into bglibs)
- [Volker Diels-Grabsch](https://njh.eu/) (Translation to simpler C++ Code)
- [Eugene Hopkinson](https://riot.so/) (Safety improvements)
- [Zlatko Michailov](http://zlatko.michailov.org) (Header-only library)
- [Dan Machado](dan-machado@yandex.com) (Optimization)
