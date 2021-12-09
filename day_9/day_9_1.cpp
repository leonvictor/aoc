#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <assert.h>

class HeightMapBufferReader
{
private:
    std::fstream *m_pStream;

    std::array<std::vector<unsigned int>, 3> m_buffers;

    size_t current_index = 0;

    int m_buffer_size = -1;
    int m_line_count = 0;

public:
    HeightMapBufferReader(std::fstream *pStream) : m_pStream(pStream) {}

    bool read_line()
    {
        m_line_count++;
        std::cout << "reading line : << " << m_line_count << std::endl;
        std::cout << "populating buffer : << " << current_index << std::endl;

        std::string line_string;
        if (!std::getline(*m_pStream, line_string))
        {
            // we reached the end of the file
            m_buffers[current_index] = std::vector<unsigned int>(m_buffer_size, 10);
            current_index = (current_index + 1) % 3;
            return false;
        }

        if (m_buffer_size == -1)
        {
            // first reading, initialize buffers with 10s
            m_buffer_size = line_string.size() + 2;
            for (auto &buffer : m_buffers)
            {
                buffer = std::vector<unsigned int>(m_buffer_size, 10);
            }
        }

        m_buffers[current_index].clear();
        m_buffers[current_index].push_back(10);
        for (auto &point : line_string)
        {
            m_buffers[current_index].push_back(point - '0');
        }
        m_buffers[current_index].push_back(10);

        current_index = (current_index + 1) % 3;
        return true;
    }

    /// @brief compute the total risk on the current line
    int detect_low_points()
    {

        size_t curr = (current_index + 1) % 3;
        size_t top = (current_index + 2) % 3;
        size_t bottom = (current_index + 0) % 3;

        std::cout << "detecting on line: " << m_line_count << std::endl;
        std::cout << "using middle buffer idx: << " << curr << std::endl;
        int risk = 0;
        for (int i = 1; i < m_buffers[curr].size() - 1; i++)
        {
            auto &point = m_buffers[curr][i];
            if (point < m_buffers[top][i] &&
                point < m_buffers[bottom][i] &&
                point < m_buffers[curr][i - 1] &&
                point < m_buffers[curr][i + 1])
            {
                std::cout << "low point found: " << point << " (risk " << point + 1 << ") at " << m_line_count - 1 << ", " << i - 1 << std::endl;
                std::cout << "surrounded by: " << m_buffers[top][i] << ", " << m_buffers[bottom][i] << ", " << m_buffers[curr][i - 1] << ", " << m_buffers[curr][i + 1] << std::endl;
                risk += (point + 1);
            }
        }
        return risk;
    }
};

int main()
{
    std::fstream fs("input", std::fstream::in);
    std::string line;

    HeightMapBufferReader reader(&fs);
    int total_risk = 0;
    // read the first line
    reader.read_line();
    while (reader.read_line())
    {
        total_risk += reader.detect_low_points();
    }
    total_risk += reader.detect_low_points();
    std::cout << "total risk: " << total_risk << std::endl;
}
