CXXFLAGS := -O2 -std=c++17 -Wall -Wextra

smd-sum: smd-sum.cc rom-t.cc cstdio-opt.cc

clean:
	rm -f smd-sum
