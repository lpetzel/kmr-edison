#include "TestModbus.h"
#include "BaseStation.h"
#include "RingStation.h"
#include "CapStation.h"
#include "DeliveryStation.h"
#include "StorageStation.h"
#include <iostream>

using namespace std;

void TestModbus::run(BaseStation* station) {
  cout << "Communication with base station started!" << endl;
  int choice;
  do {
    cout << "[0] reset\n[1] get base\n[2] end" << endl;
    cin >> choice;
    if (choice == 0) {
      station->reset();
    } else if (choice == 1) {
      int choice2;
      do {
        cout << "Which colour (1-3, 4 = abort)?" << endl;
        cin >> choice2;
        if ((choice2 > 0) and (choice2 < 4)) {
          station->getCap(choice2);
        } else if (choice2 != 4) {
          cout << "Illegal choice " << choice2 << "!" << endl;
        }
      } while ( (choice2 < 1 ) || (choice2 > 4));
    } else if (choice == 2) {
      break;
    } else {
      cout << "Illegal choice " << choice << "!" << endl;
    }
  } while (choice != 2);
  cout << "Bye!" <<endl;
}


void TestModbus::run(DeliveryStation* station) {
  cout << "Communication with delivery station started!" << endl;
  int choice;
  do {
    cout << "[0] reset\n[1] deliver product\n[2] end" << endl;
    cin >> choice;
    if (choice == 0) {
      station->reset();
    } else if (choice == 1) {
      int choice2;
      do {
        cout << "Which slot (1-3, 4 = abort)?" << endl;
        cin >> choice2;
        if ((choice2 > 0) and (choice2 < 4)) {
          station->deliverProduct(choice2);
        } else if (choice2 != 4) {
          cout << "Illegal choice " << choice2 << "!" << endl;
        }
      } while ( (choice2 < 1 ) || (choice2 > 4));
    } else if (choice == 2) {
      break;
    } else {
      cout << "Illegal choice " << choice << "!" << endl;
    }
  } while (choice != 2);
  cout << "Bye!" <<endl;
}


void TestModbus::run(RingStation* station) {
  cout << "Communication with ring station started!" << endl;
  int choice;
  do {
    cout << "[0] reset\n[1] get ring\n[2] end" << endl;
    cin >> choice;
    if (choice == 0) {
      station->reset();
    } else if (choice == 1) {
      station->getRing();
    } else if (choice == 2) {
      break;
    } else {
      cout << "Illegal choice " << choice << "!" << endl;
    }
  } while (choice != 2);
  cout << "Bye!" <<endl;
}



void TestModbus::run(StorageStation* station) {
  cout << "Communication with storage station started!" << endl;
  int choice;
  do {
    cout << "[0] reset\n[1] get product\n[2] end" << endl;
    cin >> choice;
    if (choice == 0) {
      station->reset();
    } else if (choice == 1) {
      int choice2;
      do {
        cout << "Which slot (1-3, 4 = abort)?" << endl;
        cin >> choice2;
        if ((choice2 > 0) and (choice2 < 4)) {
          station->getProduct(choice2);
        } else if (choice2 != 4) {
          cout << "Illegal choice " << choice2 << "!" << endl;
        }
      } while ( (choice2 < 1 ) || (choice2 > 4));
    } else if (choice == 2) {
      break;
    } else {
      cout << "Illegal choice " << choice << "!" << endl;
    }
  } while (choice != 2);
  cout << "Bye!" <<endl;
}

