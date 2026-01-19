#include <iostream>

#include "sha1.hpp"

#include "time_profiler/time_profiler.h"

using Profiler=tprofiler::TimeProfiler<std::chrono::seconds>;

//====================================================================

int main()
{
	const char* testFile="";
	#ifndef TEST_FILE
		std::cout<<TEST_FILE<<" no found.\n";
		return 0;
	#else
		testFile= TEST_FILE;
	#endif

	Profiler watch("sha1_new_1file", "#5400AA");

	SHA1 sha1;

	std::string hash;

	for(int i=0; i<10; i++){
		std::cout<<"sha1sum command: ";

		watch.start();
			system("sha1sum  /tmp/1G_sha1_test_file.txt");
		watch.takeSample(true);

		std::cout<<"\nSHA1 class: ";
		watch.start();
			if(sha1(testFile, hash)){
				std::cout<<hash<<"\n";
			}
			else{
				std::cout<<"Error: "<<sha1.getError()<<"\n";
			}
		watch.takeSample(true);
		std::cout<<"\n";
	}

	return 0;
}
