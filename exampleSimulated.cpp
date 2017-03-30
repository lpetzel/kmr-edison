
#include "SimulatedMachine.h"
#include <thread>
#include <iostream>

using namespace std;

int main() {
  SimulatedMachine<Machine> machine;
  //machine.initializeTTY();
  cout << "Lets start thread to control machine..." << endl;
  //thread t(&SimulatedMachine<Machine>::run, &machine);
  machine.run();
  // cout << "joining..." << endl;
  // t.join();
  return 0;
}
