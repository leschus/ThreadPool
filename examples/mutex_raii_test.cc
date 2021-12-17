#include "mutex_raii.h"
#include <iostream>
using namespace std;

static mutex mux;

void test(long long n) {
  // Locker l(&mux);
  if (n <= 0) {
    return; // 可能提前退出
  }
  try {
    long long* arr = new long long[n];  // 可能抛出异常
    arr[0] = 1;
    arr[1] = 0;
    cout << arr[0] / arr[1] << endl; // 可能抛出异常
  } catch (bad_alloc) {
    cerr << "bad_alloc" << endl;
    return;
  } catch (...) { // 此方法无法捕获到除0异常, WHY?
    cerr << "Attempt to divide 0" << endl;
  }
}

int main() {
  test(0);
  test(100000000000000LL); // bad_alloc异常发生时, 没有打印"Unlock", WHY?
  // test(10);
  cout << "End" << endl;
  return 0;
}