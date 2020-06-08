#pragma once

namespace RawVector 
{
template<typename Type, size_t Count>
class Vector
{
public:
  Vector() = default;
  template<typename... Args>
  Vector(Args... args); // Take any number of arguments of Type

  //void Vector(const Vector& vec) = default;
  //Vector& Vector(const Vector& vec) = default;

  Type operator[](size_t pos)
  {
    if (pos < Count) return values[pos];
    throw std::out_of_range("%s is more than %s", pos, Count);
  }

  std::string to_string()
  {
    std::string out;
    std::for_each(
      &values[0], &values[Count], [&](Type val) { out += std::to_string(val); });
    return out;
  }
protected:
private:
  // Raw representation of values
  Type values[Count];
};
template<typename Type, size_t Count>
template<typename... Args>
inline Vector<Type, Count>::Vector(Args... args)
{
  auto new_array = assign(values, args...);
  memcpy(values, new_array, sizeof(new_array));
}
} // namespace RawVector

namespace //Templated Array assigning mess
{
template<typename Type>
Type* assign(Type assign_to[])
{
  return assign_to;
}

template<typename Type, typename... Args>
Type* assign(Type assign_to[], Type val, Args... rest)
{
  const size_t Length = sizeof(assign_to) / sizeof(assign_to[0]);
  Type new_array[Length + 1];
  memcpy(new_array, assign_to, sizeof(assign_to));
  new_array[Length] = val;
  return assign(new_array, rest...);
}
}