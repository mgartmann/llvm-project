// RUN: %check_clang_tidy %s misc-cin-cout %t

#include <iostream>

std::cout << "This should trigger the check";
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: cin/cout should only be used inside main function [misc-cin-cout]

void problematic() {
  std::wcout << "This should trigger the check";
}

int main() {
  std::cout << "This should not trigger the check";
}
