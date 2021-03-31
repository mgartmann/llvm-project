// RUN: %check_clang_tidy -std=c++14-or-later %s misc-std-stream-objects-outside-main %t

#include <iostream>

void problematic() {
  // CHECK-MESSAGES: :[[@LINE+1]]:8: warning: predefined standard stream objects should not be used outside the main function [misc-std-stream-objects-outside-main]
  std::cout << "This should trigger the check";

    // CHECK-MESSAGES: :[[@LINE+1]]:8: warning: predefined standard stream objects should not be used outside the main function [misc-std-stream-objects-outside-main]
  std::wcout << "This should trigger the check";

    // CHECK-MESSAGES: :[[@LINE+1]]:8: warning: predefined standard stream objects should not be used outside the main function [misc-std-stream-objects-outside-main]
  std::cerr << "This should trigger the check";

    // CHECK-MESSAGES: :[[@LINE+1]]:8: warning: predefined standard stream objects should not be used outside the main function [misc-std-stream-objects-outside-main]
  std::wcerr << "This should trigger the check";

    // CHECK-MESSAGES: :[[@LINE+1]]:8: warning: predefined standard stream objects should not be used outside the main function [misc-std-stream-objects-outside-main]
  std::clog << "This should trigger the check";

    // CHECK-MESSAGES: :[[@LINE+1]]:8: warning: predefined standard stream objects should not be used outside the main function [misc-std-stream-objects-outside-main]
  std::wclog << "This should trigger the check";

  int I{0};
  // CHECK-MESSAGES: :[[@LINE+1]]:8: warning: predefined standard stream objects should not be used outside the main function [misc-std-stream-objects-outside-main]
  std::cin >> I;

  // CHECK-MESSAGES: :[[@LINE+1]]:8: warning: predefined standard stream objects should not be used outside the main function [misc-std-stream-objects-outside-main]
  std::wcin >> I;
}

int main() {
  std::cout << "This should not trigger the check";  // OK
  std::wcout << "This should not trigger the check"; // OK
  std::cerr << "This should not trigger the check";  // OK
  std::wcerr << "This should not trigger the check"; // OK
  std::clog << "This should not trigger the check";  // OK
  std::wclog << "This should not trigger the check"; // OK

  int I{0};
  std::cin >> I;  // OK
  std::wcin >> I; // OK
}
