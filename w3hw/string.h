class Str {
 private:
  char *str;  // pointer to the char array. stores the string content.
  int len;    // integer. stores the length of the string.

 public:
  // constructor
  Str(int leng);      // constructs a Str object with length leng.
  Str(char *neyong);  // constructs a Str object with content in neyong.
  // destructor
  ~Str();
  int length(void);      // function returns the length of the string.
  char *contents(void);  // function returns a pointer to the string content.
  int compare(class Str &a);  // function compares the string content with
                              // another Str object.
  int compare(char *a);  // function compares the string with another string.
  // overload
  void operator=(char *a);  // function assigns a string to this Str object.
  void operator=(
      class Str &a);  // function assigns a Str object to this Str object.
};