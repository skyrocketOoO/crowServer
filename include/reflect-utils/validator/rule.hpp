#include <string>
#include <functional>

namespace Rule {
  namespace Common {
    template <typename T>
    std::function<std::string(std::string, T)> In(std::vector<T> values);

    template <typename T>
    std::function<std::string(std::string, T)> And(std::vector<std::function<std::string(std::string, T)>> validateFuncs);

    template <typename T>
    std::function<std::string(std::string, T)> Or(std::vector<std::function<std::string(std::string, T)>> validateFuncs);
  
    template <typename T>
    std::function<std::string(std::string, T)> Eq(T value);
  
    template <typename T>
    std::function<std::string(std::string, T)> Ne(T value);

    template <typename T>
    std::function<std::string(std::string, T)> NotWritable();
  }
  namespace String{
    using ret = std::function<std::string(std::string, std::string)>;
    ret MaxLen(int threshold);
    ret RegExp(std::string pattern);
  }

  namespace Number{
    using ret = std::function<std::string(std::string, int)>;
    ret Min(int threshold);
    ret Max(int threshold);
  }
}