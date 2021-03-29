// RUN: %check_clang_tidy -std=c++14-or-later %s misc-cin-cout %t

#include <iostream>

void problematic() {
  // CHECK-MESSAGES: :[[@LINE+1]]:8: warning: predefined standard stream objects should not be used outside the main function [misc-cin-cout]
  std::cout << "This should trigger the check";

    // CHECK-MESSAGES: :[[@LINE+1]]:8: warning: predefined standard stream objects should not be used outside the main function [misc-cin-cout]
  std::wcout << "This should trigger the check";

    // CHECK-MESSAGES: :[[@LINE+1]]:8: warning: predefined standard stream objects should not be used outside the main function [misc-cin-cout]
  std::cerr << "This should trigger the check";

    // CHECK-MESSAGES: :[[@LINE+1]]:8: warning: predefined standard stream objects should not be used outside the main function [misc-cin-cout]
  std::wcerr << "This should trigger the check";

    // CHECK-MESSAGES: :[[@LINE+1]]:8: warning: predefined standard stream objects should not be used outside the main function [misc-cin-cout]
  std::clog << "This should trigger the check";

    // CHECK-MESSAGES: :[[@LINE+1]]:8: warning: predefined standard stream objects should not be used outside the main function [misc-cin-cout]
  std::wclog << "This should trigger the check";

  int i;
  // CHECK-MESSAGES: :[[@LINE+1]]:8: warning: predefined standard stream objects should not be used outside the main function [misc-cin-cout]
  std::cin >> i;

  // CHECK-MESSAGES: :[[@LINE+1]]:8: warning: predefined standard stream objects should not be used outside the main function [misc-cin-cout]
  std::wcin >> i;
}

int main() {
  std::cout << "This should not trigger the check";  // OK
  std::wcout << "This should not trigger the check"; // OK
  std::cerr << "This should not trigger the check";  // OK
  std::wcerr << "This should not trigger the check"; // OK
  std::clog << "This should not trigger the check";  // OK
  std::wclog << "This should not trigger the check"; // OK

  int i;
  std::cin >> i;  // OK
  std::wcin >> i; // OK
}