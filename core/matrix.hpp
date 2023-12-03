#include <vector>
#include <assert.h>
#include <span>

template <typename T>
class matrix
{
private:
    size_t m_rows, m_columns;
    std::vector<T> m_buffer;

    size_t index(size_t m, size_t n) { return m_columns * m + n; }

public:
    matrix(const size_t m, const size_t n) : m_rows(m), m_columns(n), m_buffer(m * n) {}
    matrix(const size_t m, const size_t n, std::span<T> data) : m_rows(m), m_columns(n), m_buffer(data.begin(), data.end()) {}
    matrix(const matrix &other) : m_rows(other.m_rows), m_columns(other.m_columns), m_buffer(other.m_buffer) {}

    T &get(size_t m, size_t n) const { return m_buffer[index(m, n)]; }
    void set(size_t m, size_t n, T value) { m_buffer[index(m, n)] = value; }

    size_t rows() { return m_rows; }
    size_t columns() { return m_columns; }

    std::vector<T>::iterator begin() { return m_buffer.begin(); }
    std::vector<T>::iterator end() { return m_buffer.end(); }
    matrix transpose() const
    {
        matrix mat(m_columns, m_rows);
        for (size_t i = 0; i < m_rows; ++i)
        {
            for (size_t j = 0; j < m_columns; j++)
            {
                mat.set(j, i, get(i, j));
            }
        }
        return mat;
    }

    /// @brief matrix multiplication
    /// @brief todo
    // matrix matmul(const matrix &other)
    // {
    //     assert(m_columns == other.m_rows);

    //     matrix mat(m_rows, other.m_columns);
    //     for (size_t i = 0; i < m_rows; ++i)
    //     {
    //         for (size_t j = 0; j < other.m_columns; ++j)
    //         {
    //             T sum = 0;
    //             sum += get(i, k) * other.get(k, j);
    //         }
    //         mat.set(i, j, sum);
    //     }
    //     return mat;
    // }
};