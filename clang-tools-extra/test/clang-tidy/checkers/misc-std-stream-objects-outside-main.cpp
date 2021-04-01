// RUN: %check_clang_tidy %s misc-std-stream-objects-outside-main %t

namespace std {
struct string {
  string(const char *);
  ~string();
};

struct Ostream {
  Ostream &operator<<(string Message);
};

struct Istream {
  Istream &operator>>(string Message);
};

Ostream cout{}, wcout{}, cerr{}, wcerr{};
Istream cin{}, wcin{};

} // namespace std

namespace arbitrary_namespace {
std::Ostream cout{};
std::Istream cin{};
} // namespace arbitrary_namespace

void problematic() {
  // CHECK-MESSAGES: :[[@LINE+1]]:8: warning: predefined standard stream objects should not be used outside the main function [misc-std-stream-objects-outside-main]
  std::cout << "This should trigger the check";

  // CHECK-MESSAGES: :[[@LINE+1]]:8: warning: predefined standard stream objects should not be used outside the main function [misc-std-stream-objects-outside-main]
  std::wcout << "This should trigger the check";

  // CHECK-MESSAGES: :[[@LINE+1]]:8: warning: predefined standard stream objects should not be used outside the main function [misc-std-stream-objects-outside-main]
  std::cerr << "This should trigger the check";

  // CHECK-MESSAGES: :[[@LINE+1]]:8: warning: predefined standard stream objects should not be used outside the main function [misc-std-stream-objects-outside-main]
  std::wcerr << "This should trigger the check";

  arbitrary_namespace::cout << "This should not trigger the check"; // OK

  std::string Foo{"bar"};

  // CHECK-MESSAGES: :[[@LINE+1]]:8: warning: predefined standard stream objects should not be used outside the main function [misc-std-stream-objects-outside-main]
  std::cin >> Foo;

  // CHECK-MESSAGES: :[[@LINE+1]]:8: warning: predefined standard stream objects should not be used outside the main function [misc-std-stream-objects-outside-main]
  std::wcin >> Foo;

  arbitrary_namespace::cin >> Foo; // OK
}

int main() {
  std::cout << "This should not trigger the check";                 // OK
  std::wcout << "This should not trigger the check";                // OK
  std::cerr << "This should not trigger the check";                 // OK
  std::wcerr << "This should not trigger the check";                // OK
  arbitrary_namespace::cout << "This should not trigger the check"; // OK

  std::string Foo{"bar"};
  std::cin >> Foo;                 // OK
  std::wcin >> Foo;                // OK
  arbitrary_namespace::cin >> Foo; // OK
}
