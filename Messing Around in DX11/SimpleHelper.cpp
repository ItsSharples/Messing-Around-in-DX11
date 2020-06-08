#include "pch.h"
#include "SimpleHelper.h"


#define braced_string(x) "{" + x + "}"
#define vec_val_to_string(x) std::to_string(vec.x)

std::string SimpleHelpers::to_string(const Vector4& vec)
{
    return braced_string(vec_val_to_string(x) + "," + vec_val_to_string(y) + "," +
        vec_val_to_string(z) + "," + vec_val_to_string(w));
}

std::string SimpleHelpers::to_string(const Vector3& vec)
{
    return braced_string(vec_val_to_string(x) + "," + vec_val_to_string(y) + "," +
        vec_val_to_string(z));
}

std::string SimpleHelpers::to_string(const Vector2& vec)
{
    return braced_string(vec_val_to_string(x) + "," + vec_val_to_string(y));
}


#undef braced_string
#undef vec_val_to_string


