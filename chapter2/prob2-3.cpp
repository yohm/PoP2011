#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <boost/timer.hpp>
#include <boost/random.hpp>

double MeasureSortTime( int nArraySize);
void PrintVector( std::vector<int> vec);
void ShuffleVector( std::vector<int> &vec);
void MySort( std::vector<int> & vec);
void Swap( std::vector<int> & vec, long i, long j);
void PrintVector( std::vector<int> vec);

// ---------------------------------------
int main() {

  std::vector<int> vecSize;
  vecSize.push_back( 1 << 8);
  vecSize.push_back( 1 << 10);
  vecSize.push_back( 1 << 12);
  vecSize.push_back( 1 << 14);
  vecSize.push_back( 1 << 16);
  vecSize.push_back( 1 << 18);
  vecSize.push_back( 1 << 20);
  vecSize.push_back( 1 << 22);
  vecSize.push_back( 1 << 24);


  for( int i=0; i<vecSize.size(); i++) {
    std::cout << vecSize[i] << ' ' << MeasureSortTime( vecSize[i]) * 1000 << std::endl;
  }

  return 0;
}

// ---------------------------------------
double MeasureSortTime( int nArraySize) {
  
  std::vector<int> vecInt;
  for( long i=0; i<nArraySize; i++) { vecInt.push_back( i); }
  std::vector<int> vecSorted = vecInt;

  const int nIteration = 128;
  boost::timer zTm;
  double dTotalElapsed = 0.0;
  for( int i=0; i<nIteration; i++) {
    ShuffleVector( vecInt);
    zTm.restart();
    MySort( vecInt);
    dTotalElapsed += zTm.elapsed();
    assert( vecSorted == vecInt);
  }
  return dTotalElapsed / nIteration;
}

// ---------------------------------------
void PrintVector( std::vector<int> vec) {
  for( std::vector<int>::iterator it = vec.begin(); it != vec.end(); it++) {
    std::cout << *it << std::endl;
  }
}


// ---------------------------------------
void ShuffleVector( std::vector<int> &vec) {
  boost::mt19937 zGen;  // functor which generates a random number distributing [0, 2^32-1].
  for( long i=0; i<vec.size(); i++) { Swap( vec, i, zGen() % vec.size() ); }
}

// ---------------------------------------
void Swap( std::vector<int> & vec, long i, long j) {
  int temp = vec[i];
  vec[i] = vec[j];
  vec[j] = temp;
}

// ---------------------------------------
void MySort( std::vector<int> & vec) {
  std::sort( vec.begin(), vec.end() );  
}

