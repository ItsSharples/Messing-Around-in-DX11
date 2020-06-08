#pragma once

#include <SimpleMath.h>
#include <string>


namespace SimpleHelpers {
using Vector4 = DirectX::SimpleMath::Vector4;
using Vector3 = DirectX::SimpleMath::Vector3;
using Vector2 = DirectX::SimpleMath::Vector2;

#pragma region Vector to_string
std::string to_string(const Vector4& vec);
std::string to_string(const Vector3& vec);
std::string to_string(const Vector2& vec);
#pragma endregion
} // namespace SimpleHelpers