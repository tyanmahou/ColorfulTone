#pragma once
#include <array>
#include <type_traits>

namespace ct
{
    template<class T, size_t MaxSize> requires std::is_trivial_v<T>
    class FixedArray
    {
    public:
        using value_type = T;
        using size_type = size_t;
        using difference_type = ptrdiff_t;

        using pointer = T*;
        using const_pointer = const T*;
        using reference = T&;
        using const_reference = const T&;

        using iterator = pointer;
        using const_iterator = const_pointer;

        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    public:
        constexpr FixedArray() :
            m_size(0)
        {}
        constexpr FixedArray(const std::initializer_list<T>& list):
            m_data{},
            m_size(0)
        {
            for (const T& v : list) {
                this->push_back(v);
            }
        }
        constexpr FixedArray(const FixedArray& other):
            m_data(other.m_data),
            m_size(other.m_size)
        {
        }
        void push_back(const T& value)
        {
            if (m_size < MaxSize) {
                m_data[m_size++] = value;
            } else {
                throw std::out_of_range("FixedArray is full");
            }
        }

        void pop_back()
        {
            if (m_size > 0) {
                --m_size;
            } else {
                throw std::out_of_range("FixedArray is empty");
            }
        }

        T& operator[](std::size_t index)
        {
            return m_data[index];
        }
        T& at(std::size_t index)
        {
            if (index >= m_size) {
                throw std::out_of_range("Index out of range");
            }
            return m_data[index];
        }

        constexpr const T& operator[](size_t index) const
        {
            return m_data[index];
        }
        constexpr const T& at(size_t index) const
        {
            if (index >= size) {
                throw std::out_of_range("Index out of range");
            }
            return m_data[index];
        }
        constexpr size_t size() const noexcept
        {
            return m_size;
        }

        constexpr size_t maxSize() const noexcept
        {
            return MaxSize;
        }

        iterator begin() noexcept
        {
            return m_data.data();
        }
        constexpr const_iterator begin() const noexcept
        {
            return m_data.data();
        }

        iterator end() noexcept
        {
            return m_data.data() + m_size;
        }
        constexpr const_iterator end() const  noexcept
        {
            return m_data.data() + m_size;
        }

        constexpr bool empty() const noexcept
        {
            return m_size == 0;
        }
        constexpr bool isEmpty() const noexcept
        {
            return empty();
        }
        void clear()
        {
            m_size = 0;
        }
    private:
        std::array<T, MaxSize> m_data;
        size_t m_size;
    };
}