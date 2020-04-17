* SHA-1 implementation in C++

** Warning

Do not use SHA-1 unless you have to! [[https://en.wikipedia.org/wiki/SHA-1#Birthday-Near-Collision_Attack_%E2%80%93_first_practical_chosen-prefix_attack][SHA-1 is practically broken]]. Use a hash function from the [[https://en.wikipedia.org/wiki/SHA-2][SHA-2]] or [[https://en.wikipedia.org/wiki/SHA-3][SHA-3]] family instead.

** License

100% Public Domain

** Authors

- Steve Reid (Original C Code)
- [[http://untroubled.org/][Bruce Guenter]] (Small changes to fit into bglibs)
- [[https://njh.eu/][Volker Diels-Grabsch]] (Translation to simpler C++ Code)
- [[https://riot.so/][Eugene Hopkinson]] (Safety improvements)
- [[http://zlatko.michailov.org][Zlatko Michailov]] (Header-only library)
