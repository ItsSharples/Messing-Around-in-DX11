#pragma once
#include <pch.h>

class Debug_Graph
{
public:
    Debug_Graph() = delete;
    Debug_Graph(FLOAT _width, FLOAT _height, FLOAT _x, FLOAT _y):
        width(_width),
        height(_height),
        width_segment( width / ((FLOAT)num_points - 1.f)),
        mid_height(height / 2.F)
    {
        moveTo(_x, _y);

        for(int i = 0; i < num_points; i++)
        {
            setNextPoint(mid_height);
        }
    }

    void moveTo(FLOAT _x, FLOAT _y)
    {
        D2D1_POINT_2F last_pos = current_pos;
        current_pos = { _x, _y };

        for(int i = 0; i < num_points; i++)
        {
            test_geometry[i] = { test_geometry[i].x - last_pos.x + current_pos.x,
            test_geometry[i].y - last_pos.y + current_pos.y };
        }
    }

    void setNextPoint(FLOAT y_point)
    {
        test_geometry[current_iter] = { current_iter * width_segment + current_pos.x, y_point + current_pos.y};
        current_iter = (current_iter + 1) & (num_points - 1);
    }

    auto getPoints() -> D2D1_POINT_2F*
    {
        return test_geometry;
    }

    auto getNumPoints() -> UINT32
    {
        return num_points;
    }

protected:
private:
    FLOAT width;
    FLOAT height;
    FLOAT width_segment;
    FLOAT mid_height;

    D2D1_POINT_2F current_pos;

    static constexpr UINT32 num_points = 1 << 6; // 2^10
    D2D1_POINT_2F test_geometry[num_points];
    UINT32 current_iter = 0;


};

