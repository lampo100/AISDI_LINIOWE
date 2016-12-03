#include <cstddef>
#include <cstdlib>
#include <string>
#include <type_traits>

#include "Vector.h"
#include "LinkedList.h"
#include "../CODEine-master/benchmark.h"

namespace
{


    template <typename T>
using LinearCollection = aisdi::LinkedList<T>;
/*
void benchmarking(int numberEle){
    aisdi::LinkedList<int> list;
    std::random_device rd;
    std::mt19937 eng(rd());
    std::uniform_int_distribution<int> distr(0,numberEle);

    for(int i = 0, val; i <numberEle ; i++){
        val = distr(eng);
        list.append(val);
    }

    while(numberEle){
        auto it = list.begin();
        std::advance(it,distr(eng) % numberEle);
        --numberEle;
    }
}*/





    void perfomTest()
{
  LinearCollection<std::string> collection;
  collection.append("TODO");
}

} // namespace

int main(int argc, char** argv)
{
  const std::size_t repeatCount = argc > 1 ? std::atoll(argv[1]) : 10000;
  for (std::size_t i = 0; i < repeatCount; ++i)
    perfomTest();

    aisdi::LinkedList a;
    a.append
    //Benchmark for inserting and randomly ereasing ints
/*
    bm.run("list", 10, insertingEreasingInt<aisdi::LinkedList<int>>, "number of elements",
           {10, 30, 60, 100, 200, 300, 600, 1000, 2000, 3000, 6000, 10000});
    bm.run("vector", 10, insertingEreasingInt<aisdi::Vector<int>>, "number of elements",
           {10, 30, 60, 100, 200, 300, 600, 1000, 2000, 3000, 6000, 10000});
    bm.serialize("Randomly inserting and ereasing ints", "list_vs_vector_10000elem_insert_erase.txt");
*/
    //Benchmark for randomly inserting doubles
/*
    bm.run("list", 10, insertingDouble<aisdi::LinkedList<int>>, "number of elements",
           {10, 30, 60, 100, 200, 300, 600, 1000, 2000, 3000, 6000, 10000});
    bm.run("vector", 10, insertingDouble<aisdi::Vector<int>>, "number of elements",
           {10, 30, 60, 100, 200, 300, 600, 1000, 2000, 3000, 6000, 10000});
    bm.serialize("Randomly inserting doubles", "list_vs_vector_10000elem_insert_double.txt");*/

    //Benchmark for appending random ints
/*
    bm.run("list", 10, appendingInt<aisdi::LinkedList<int>>, "number of elements",
           {10, 100, 1000, 3000, 6000, 10000, 20000, 40000, 60000, 100000, 200000, 400000, 600000, 1000000, 2000000,  10000000});
    bm.run("vector", 10, appendingInt<aisdi::Vector<int>>, "number of elements",
           {10, 100, 1000, 3000, 6000, 10000, 20000, 40000, 60000, 100000, 200000, 400000, 600000, 1000000, 2000000,  10000000});
    bm.serialize("Appending Intigers", "list_vs_vector_100kkelem_append_int.txt");*/

    //Benchmark for prepending ints
/*
    bm.run("list", 10, prependInt<aisdi::LinkedList<int>>, "number of elements",
           {10, 100, 1000, 3000, 6000, 10000, 20000, 40000, 60000, 100000});
    bm.run("vector", 10, prependInt<aisdi::Vector<int>>, "number of elements",
           {10, 100, 1000, 3000, 6000, 10000, 20000, 40000, 60000, 100000});
    bm.serialize("Prepending Intigers", "Prepending_int_100kelem.txt");
*/
  return 0;
}
