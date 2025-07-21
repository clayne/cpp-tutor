//========================================================================
// FILE:
//    smart_pointers_main.cpp
//
// AUTHOR:
//    banach-space@github
//
// DESCRIPTION:
//    Examples of how to use std::unique_ptr, std::shared_ptr and
//    std::weak_ptr. Build and run this example and analyse the output. Make
//    sure that you understand where it comes from.
//
//    The version guarded with:
//      * MEMORY_LEAK exhibits memory leaks
//      * COMPILER_ERROR triggers a compiler error
//    Make sure you that understand why!
//
// License: MIT
//========================================================================
#include <iostream>
#include <memory>
#include <string>

#include "cppt_ag.hpp"

//========================================================================
// Data structures
//========================================================================
class A {
 public:
  explicit A(const std::string &name) : m_name(name) {
    m_data = new int[10];
    std::cout << "Constructor for " << m_name << std::endl;
  }
  ~A() {
    std::cout << "Destructor for " << m_name << std::endl;
    delete[] m_data;
  }

 private:
  std::string m_name;
  int *m_data;
};

struct Player {
#ifdef MEMORY_LEAK
  std::shared_ptr<Player> partner;
#else
  std::weak_ptr<Player> partner;
#endif

  ~Player() { std::cout << "~Player\n"; }
};

//========================================================================
// main
//========================================================================
int main() {
  size_t sec_num = {1};

  // 1. RAW vs SMART POINTER
  std::cout << sec_num++ << ". RAW vs SMART POINTER:" << std::endl;
  A *obj_a = new A("Andrzej");
  std::unique_ptr<A> obj_b(new A("Jack"));

  A *obj_a_copy = obj_a;
#ifdef COMPILATION_ERROR
  std::unique_ptr<A> obj_b_copy = obj_b;
#endif

  // These two should be identical.
  std::cout << "\n(should be identical)\n";
  std::cout << "Size of raw pointer: " << sizeof obj_a << std::endl;
  std::cout << "Size of unique_ptr: " << sizeof obj_b << std::endl;

  delete obj_a;

  // 2. SHARED_PTR vs WEAK_PTR
  std::cout << std::endl << sec_num++ << ". SHARED_PTR vs WEAK_PTR:" << std::endl;
  std::shared_ptr<A> obj_c(new A("David"));
  std::shared_ptr<A> obj_c_copy(obj_c);

  std::cout << "Use count for David: " << obj_c.use_count() << std::endl;

  std::weak_ptr<A> obj_c_weak_ptr(obj_c);
  std::cout << "Use count for David: " << obj_c.use_count() << std::endl;

  // These two should be identical.
  std::cout << "\n(should be smaller than unique and raw pointers)\n";
  std::cout << "Size of shared pointer: " << sizeof obj_c << std::endl;
  std::cout << "Size of weak pointer: " << sizeof obj_c_weak_ptr << std::endl;

  // 3. CIRCULAR DEPENDENCY
  std::cout << std::endl << sec_num++ << ". CIRCULAR DEPENDENCY" << std::endl;
  std::shared_ptr<Player> andrzej(new Player());
  std::shared_ptr<Player> jack(new Player());

  jack->partner = andrzej;
  andrzej->partner = jack;

  // 4. MAKE_UNIQUE AND MAKE_SHARED
  std::cout << std::endl << sec_num++ << ". STD::MAKE_UNIQUE" << std::endl;
#if __cplusplus >= 201403L
  std::unique_ptr<Player> warzynski = std::make_unique<Player>();
#endif
  std::shared_ptr<Player> moulson = std::make_shared<Player>();
}
