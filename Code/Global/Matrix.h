#pragma once

#include "Vector.h"

namespace ge2
{
    template<typename VALUE_TYPE, int ROWS, int COLUMNS>
    struct Matrix
    {
        VALUE_TYPE values[ROWS][COLUMNS] = { 0 };

        static constexpr Matrix Identity();

        Matrix& operator+=(Matrix const&);
        Matrix& operator+(Matrix const&) const;

        VALUE_TYPE& Get(int row, int column);
        VALUE_TYPE Get(int row, int column) const;
    };

    using Matrix4x4f = Matrix<float, 4, 4>;
    using Matrix3x3f = Matrix<float, 3, 3>;

    template<typename VALUE_TYPE, int LHS_ROWS, int RHS_COLUMNS, int LHS_COLUMNS>
    Matrix<VALUE_TYPE, LHS_ROWS, RHS_COLUMNS> operator*(Matrix<VALUE_TYPE, LHS_ROWS, LHS_COLUMNS> const& lhs, Matrix<VALUE_TYPE, LHS_COLUMNS, RHS_COLUMNS> const& rhs)
    {
        Matrix<VALUE_TYPE, LHS_ROWS, RHS_COLUMNS> result;
        for (auto row = 0; row < LHS_ROWS; ++row)
        {
            for (auto column = 0; column < RHS_COLUMNS; ++column)
            {
                auto& value = result.Get(row, column);
                value = 0;
                for (auto shared = 0; shared < LHS_COLUMNS; ++shared)
                {
                    value += lhs.Get(row, shared) * rhs.Get(shared, column);
                }
            }
        }

        return result;
    }

    template<typename VALUE_TYPE, int ROWS, int COLUMNS>
    inline constexpr Matrix<VALUE_TYPE, ROWS, COLUMNS> Matrix<VALUE_TYPE, ROWS, COLUMNS>::Identity()
    {
        Matrix result;

        static_assert(ROWS == COLUMNS);
        for (auto i = 0; i < ROWS; ++i)
        {
            result.values[i][i] = 1.f;
        }

        return result;
    }

    template<typename VALUE_TYPE, int ROWS, int COLUMNS>
    inline Matrix<VALUE_TYPE, ROWS, COLUMNS>& Matrix<VALUE_TYPE, ROWS, COLUMNS>::operator+=(Matrix const& rhs)
    {
        for (auto row = 0; row < ROWS; ++row)
        {
            for (auto column = 0; column < COLUMNS; ++column)
            {
                values[row][column] += rhs.values[row][column];
            }
        }
    }

    template<typename VALUE_TYPE, int ROWS, int COLUMNS>
    inline Matrix<VALUE_TYPE, ROWS, COLUMNS>& Matrix<VALUE_TYPE, ROWS, COLUMNS>::operator+(Matrix const& rhs) const
    {
        Matrix result;
        for (auto row = 0; row < ROWS; ++row)
        {
            for (auto column = 0; column < COLUMNS; ++column)
            {
                result[row][column] = values[row][column] + rhs.values[row][column];
            }
        }
    }

    template<typename VALUE_TYPE, int ROWS, int COLUMNS>
    inline VALUE_TYPE& Matrix<VALUE_TYPE, ROWS, COLUMNS>::Get(int row, int column)
    {
        _ASSERT(row < ROWS&& column < COLUMNS); //Requested element out of bounds
        return values[row][column];
    }

    template<typename VALUE_TYPE, int ROWS, int COLUMNS>
    inline VALUE_TYPE Matrix<VALUE_TYPE, ROWS, COLUMNS>::Get(int row, int column) const
    {
        _ASSERT(row < ROWS&& column < COLUMNS); //Requested element out of bounds
        return values[row][column];
    }

}