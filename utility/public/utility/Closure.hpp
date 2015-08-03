#pragma once

#include "MemoryBuffer.hpp"

namespace utl {

	template<typename T> class Closure {};

	template<typename R, typename... Args>
	class Closure<R(Args...)> {
		MemoryBuffer m_buffer;
		R(*m_execute)(MemoryBuffer &, Args...);
		void(*m_delete)(MemoryBuffer &);
		MemoryBuffer(*m_copy)(MemoryBuffer &);

		template<typename L>
		void _generateExecutor(L const &) {
			m_execute = [](MemoryBuffer &buff, Args... args) {
				return ((L*)buff.data())->operator()(args...);
			};
		}

		template<typename L>
		void _copyFromLambda(L const &lambda) {
			if (m_delete && m_buffer) {
				m_delete(m_buffer);
			}

			m_buffer.setSize(sizeof(L));
			new(m_buffer.data()) L(lambda);

			_generateExecutor(lambda);

			m_delete = [](MemoryBuffer &buffer) {
				if (buffer) {
					((L*)buffer.data())->~L();
					buffer.setSize(0);
				}

			};

			m_copy = [](MemoryBuffer &buffer) {
				MemoryBuffer out;
				if (buffer) {
					out.setSize(buffer.getSize());
					new(out.data()) L(*(L*)buffer.data());
				}
				return std::move(out);
			};
		}


	public:
		typedef Closure<R(Args...)> Type;

		//default
		Closure() : m_execute(nullptr), m_copy(nullptr), m_delete(nullptr) {}

		//copy
		Closure(Type const &other) : m_execute(other.m_execute), m_copy(other.m_copy), m_delete(other.m_delete) {
			if (_copy) {
				m_buffer = std::move(m_copy(other.m_buffer));
			}
		}

		Type &operator=(Type const &other) {
			if (m_delete && m_buffer) {
				m_delete(m_buffer);
			}

			if (other.m_copy) {
				m_buffer = std::move(other.m_copy(other.m_buffer));
			}

			m_execute = other.m_execute;
			m_delete = other.m_delete;
			m_copy = other.m_copy;
			return *this;
		}

		//move
		Closure(Type && other):m_buffer(std::move(other.m_buffer)), m_execute(other.m_execute), m_copy(other.m_copy), m_delete(other.m_delete) {}

		Type &operator=(Type && other) {
			if (m_delete && m_buffer) {
				m_delete(m_buffer);
			}

			m_buffer = std::move(other.m_buffer);			

			m_execute = other.m_execute;
			m_delete = other.m_delete;
			m_copy = other.m_copy;
			return *this;
		}

		//init from lambda
		template<typename L>
		Closure(L const &lambda) {
			_copyFromLambda(lambda);
		}

		template<typename L>
		Type &operator=(L const &lambda) {
			_copyFromLambda(lambda);
			return *this;
		}

		//destruct
		~Closure() {
			if (m_delete && m_buffer) {
				m_delete(m_buffer);
			}
		}
		
		operator bool() {
			return m_buffer;
		}

		R operator()(Args... args) {
			return m_execute(m_buffer, args...);
		}
	};
}