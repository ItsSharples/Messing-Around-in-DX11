#pragma once
using Flag = bool;
inline void flip(Flag& flag) { flag ^= true; }
inline std::string FlagToString(const Flag& val) { return val ? "true" : "false"; }