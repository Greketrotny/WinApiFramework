#ifndef BITMAP_H
#define BITMAP_H

#include <cstring>
#include <algorithm>

#include "color.h"

namespace Graphics
{
	template <typename T>
	class Buffer2D
	{
	private:
		T *mp_memory = nullptr;
		size_t m_width = size_t(0u), m_height = size_t(0u);


	public:
		Buffer2D(const Buffer2D &bitmap)
			: m_width(bitmap.m_width)
			, m_height(bitmap.m_height)
		{
			mp_memory = new T[m_width * m_height];

			for (size_t x = 0u; x < m_width; x++)
			{
				for (size_t y = 0u; y < m_height; y++)
				{
					Value(x, y) = bitmap.Value(x, y);
				}
			}
		}
		Buffer2D(Buffer2D &&bitmap) noexcept
			: m_width(bitmap.m_width)
			, m_height(bitmap.m_height)
		{
			mp_memory = bitmap.mp_memory;
			bitmap.mp_memory = nullptr;
		}
		Buffer2D(
			const size_t& width = size_t(0u), const size_t& height = size_t(0u),
			const T& fill_value = T())
			: m_width(width)
			, m_height(height)
		{
			mp_memory = new T[m_width * m_height];
			for (size_t x = 0u; x < m_width; ++x)
			{
				for (size_t y = 0u; y < m_height; ++y)
				{
					Value(x, y) = fill_value;
				}
			}
		}
		~Buffer2D()
		{
			if (mp_memory) delete[] mp_memory;
			mp_memory = nullptr;
		}


		Buffer2D& operator=(const Buffer2D& bitmap)
		{
			if (this == &bitmap)
				return *this;

			if (m_width != bitmap.m_width || m_height != bitmap.m_height)
			{
				m_width = bitmap.m_width;
				m_height = bitmap.m_height;

				if (mp_memory) delete[] mp_memory;
				mp_memory = new T[m_width * m_height];
			}

			T* ptr1 = mp_memory;
			const T* end = ptr1 + m_width * m_height;
			const T* ptr2 = bitmap.GetMapAddress();
			while (ptr1 < end)
			{
				*ptr1 = *ptr2;
				++ptr1;
				++ptr2;
			}

			return *this;
		}
		Buffer2D& operator=(Buffer2D&& bitmap) noexcept
		{
			if (this == &bitmap)
				return *this;

			m_width = bitmap.m_width;
			m_height = bitmap.m_height;

			if (mp_memory) delete[] mp_memory;
			mp_memory = bitmap.mp_memory;
			bitmap.mp_memory = nullptr;

			return *this;
		}


	public:
		void Resize(const size_t& width, const size_t& height)
		{
			if (width == m_width && height == m_height)
				return;

			m_width = width;
			m_height = height;

			if (mp_memory) delete[] mp_memory;
			mp_memory = new T[m_width * m_height];
		}
		void Resize(const size_t& width, const size_t& height, const T& fill_value)
		{
			Resize(width, height);
			Clear(fill_value);
		}
		const T* GetMapAddress() const
		{
			return mp_memory;
		}
		void CopyFromMemory(
			const void* source, const size_t bytes,
			const size_t x_offset = 0u, const size_t y_offset = 0u)
		{
			std::memcpy(
				mp_memory + (y_offset * m_width + x_offset), 
				source, 
				std::min(bytes, m_width * m_height * sizeof(T)));
		}
		void Clear(const T& value = T())
		{
			T* begin = mp_memory;
			T* end = begin + m_width * m_height;
			while (begin < end)
			{
				*begin = value;
				++begin;
			}
		}


	public:
		T& Value(const size_t& x, const size_t& y)
		{
			return mp_memory[y * m_width + x];
		}
		const T& Value(const size_t& x, const size_t& y) const
		{
			return mp_memory[y * m_width + x];
		}

		size_t GetWidth() const
		{
			return m_width;
		}
		size_t GetHeight() const
		{
			return m_height;
		}
	};

	typedef Buffer2D<Color> Bitmap;
}

#endif // !BITMAP_H