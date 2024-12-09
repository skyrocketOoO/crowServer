#include <string>
#include <variant>

namespace Validator{

enum class ErrorType {
  // common
  In,
  Eq,
  Ne,
  NotWritable,

  // string
  MaxLen,

  // number
  Min,
  Max,
  Gt,
  Lt,

};

template <typename T>
using CompareVal = std::variant<std::vector<T>, T>;

template <typename T>
struct Error {
  ErrorType type;               
  std::string fieldName;         
  T value;                       
  CompareVal<T> compareVal;
};

}