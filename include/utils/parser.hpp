#include "rfl/json.hpp"
#include "rfl.hpp"

template <typename T>
T parseJsonToStruct(std::string jsonStr){
  return rfl::json::read<T>(jsonStr).value();
}

// typedef struct yyjson_val yyjson_val;
// struct YYJSONInputVar {
//   YYJSONInputVar() : val_(nullptr) {}
//   YYJSONInputVar(yyjson_val* _val) : val_(_val) {}
//   yyjson_val* val_;
// };
// struct Reader {
//   struct YYJSONInputVar {
//     YYJSONInputVar() : val_(nullptr) {}
//     YYJSONInputVar(yyjson_val* _val) : val_(_val) {}
//     yyjson_val* val_;
//   };
//   using InputVarType = YYJSONInputVar;
// };

// using InputVarType = typename Reader::InputVarType;

// template <class T, class... Ps>
// auto read(const InputVarType& _obj) {
//   const auto r = Reader();
//   return Parser<T, Processors<Ps...>>::read(r, _obj);
// }