// RUN: %check_clang_tidy %s misc-avoid-std-io-outside-main %t

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

int printf(const char *Format, ...);
int vprintf(const char *const, ...);
int puts(const char *Str);
int putchar(int Character);
int scanf(const char *Format, ...);
int getchar(void);
char *gets(char *Str);
} // namespace std

int printf(const char *Format, ...);
int vprintf(const char *const, ...);
int puts(const char *Str);
int putchar(int Character);
int scanf(const char *Format, ...);
int getchar(void);
char *gets(char *Str);

namespace arbitrary_namespace {
std::Ostream cout{};
std::Istream cin{};
} // namespace arbitrary_namespace

void anyNonMainFunction() {
  // CHECK-MESSAGES: :[[@LINE+1]]:8: warning: predefined standard stream objects should not be used outside the main function [misc-avoid-std-io-outside-main]
  std::cout << "This should trigger the check";

  // CHECK-MESSAGES: :[[@LINE+1]]:8: warning: predefined standard stream objects should not be used outside the main function [misc-avoid-std-io-outside-main]
  std::wcout << "This should trigger the check";

  // CHECK-MESSAGES: :[[@LINE+1]]:8: warning: predefined standard stream objects should not be used outside the main function [misc-avoid-std-io-outside-main]
  std::cerr << "This should trigger the check";

  // CHECK-MESSAGES: :[[@LINE+1]]:8: warning: predefined standard stream objects should not be used outside the main function [misc-avoid-std-io-outside-main]
  std::wcerr << "This should trigger the check";

  arbitrary_namespace::cout << "This should not trigger the check"; // OK

  std::string Foo{"bar"};

  // CHECK-MESSAGES: :[[@LINE+1]]:8: warning: predefined standard stream objects should not be used outside the main function [misc-avoid-std-io-outside-main]
  std::cin >> Foo;

  // CHECK-MESSAGES: :[[@LINE+1]]:8: warning: predefined standard stream objects should not be used outside the main function [misc-avoid-std-io-outside-main]
  std::wcin >> Foo;

  arbitrary_namespace::cin >> Foo; // OK

  // CHECK-MESSAGES: :[[@LINE+1]]:8: warning: cstdio functions should not be used outside the main function [misc-avoid-std-io-outside-main]
  std::printf("This should trigger the check");
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: cstdio functions should not be used outside the main function [misc-avoid-std-io-outside-main]
  printf("This should trigger the check");

  // CHECK-MESSAGES: :[[@LINE+1]]:8: warning: cstdio functions should not be used outside the main function [misc-avoid-std-io-outside-main]
  std::puts("This should trigger the check");
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: cstdio functions should not be used outside the main function [misc-avoid-std-io-outside-main]
  puts("This should trigger the check");

  // CHECK-MESSAGES: :[[@LINE+1]]:8: warning: cstdio functions should not be used outside the main function [misc-avoid-std-io-outside-main]
  std::putchar('m');
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: cstdio functions should not be used outside the main function [misc-avoid-std-io-outside-main]
  putchar('m');

  char Input[5];
  // CHECK-MESSAGES: :[[@LINE+1]]:8: warning: cstdio functions should not be used outside the main function [misc-avoid-std-io-outside-main]
  std::scanf("%s", Input);
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: cstdio functions should not be used outside the main function [misc-avoid-std-io-outside-main]
  scanf("%s", Input);

  // CHECK-MESSAGES: :[[@LINE+1]]:8: warning: cstdio functions should not be used outside the main function [misc-avoid-std-io-outside-main]
  std::getchar();
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: cstdio functions should not be used outside the main function [misc-avoid-std-io-outside-main]
  getchar();

  // CHECK-MESSAGES: :[[@LINE+1]]:8: warning: cstdio functions should not be used outside the main function [misc-avoid-std-io-outside-main]
  std::gets(Input);
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: cstdio functions should not be used outside the main function [misc-avoid-std-io-outside-main]
  gets(Input);

  // CHECK-MESSAGES: :[[@LINE+1]]:8: warning: cstdio functions should not be used outside the main function [misc-avoid-std-io-outside-main]
  std::vprintf("This should trigger the check %d", 1);
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: cstdio functions should not be used outside the main function [misc-avoid-std-io-outside-main]
  vprintf("This should trigger the check %d", 1);

  // CHECK-MESSAGES: :[[@LINE+1]]:17: warning: cstdio functions should not be used outside the main function [misc-avoid-std-io-outside-main]
  auto Print = &puts;
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: cstdio functions should not be used outside the main function [misc-avoid-std-io-outside-main]
  Print("This should trigger the check");
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

  char Input[5];
  std::printf("This should not trigger the check");        // OK
  std::puts("This should not trigger the check");          // OK
  std::putchar('m');                                       // OK
  std::scanf("%s", Input);                                 // OK
  std::getchar();                                          // OK
  std::gets(Input);                                        // OK
  std::vprintf("This should not trigger the check %d", 1); // OK
  printf("This should not trigger the check");             // OK
  puts("This should not trigger the check");               // OK
  putchar('m');                                            // OK
  scanf("%s", Input);                                      // OK
  getchar();                                               // OK
  gets(Input);                                             // OK
  vprintf("This should not trigger the check %d", 1);      // OK
  auto Print = &puts;                                      // OK
  Print("This should not trigger the check");              // OK
}
