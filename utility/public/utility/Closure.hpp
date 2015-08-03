#pragma once

#include "MemoryBuffer.hpp"

namespace utl {

	template<typename T> class _ClosureInner {};

	//the inner storage for our closure
	template<typename R, typename... Args>
	class _ClosureInner<R(Args...)> {
		MemoryBuffer *m_buffer;
		R(*_execute)(MemoryBuffer &, Args...);
	public:
		//the actual execution
		R operator()(Args... args) {
			return _execute(*m_buffer, args...);
		}

	protected:
		_ClosureInner(MemoryBuffer *buffer) :m_buffer(buffer) {}
		~_ClosureInner() {}

		template<typename T>
		void generate(T const &c) {
			_execute = [](MemoryBuffer &buff, Args... args) {
				return ((T*)buff.data())->operator()(args...);
			};
		}

		void reset(_ClosureInner<R(Args...)> const &other) {
			_execute = other._execute;
		}

		void setBuffer(MemoryBuffer *buffer) {
			m_buffer = buffer;
		}
	};

	template<typename F> class Closure {};

	template<typename R, typename... Args>
	class Closure<R(Args...)> : public _ClosureInner<R(Args...)> {
		MemoryBuffer m_buffer;
		void(*_delete)(MemoryBuffer &);
		MemoryBuffer(*_copy)(MemoryBuffer &);

		template<typename T>
		void _copyFromLambda(T const &lambda){
			//we need to store our copy and destroy functions for this lambda
			//as lambdas so we can copy them elsehwere
			if (m_buffer) {
				_delete(m_buffer);
			}

			m_buffer.setSize(sizeof(T));
			new(m_buffer.data()) T(lambda);

			generate(lambda);

			_delete = [](MemoryBuffer &buffer) {
				if (buffer) {
					buffer.setSize(0);
				}
				
			};

			_copy = [](MemoryBuffer &buffer) {
				MemoryBuffer out;
				if (buffer) {
					out.setSize(buffer.getSize());
					new(out.data()) T(*(T*)buffer.data());
				}
				return std::move(out);
			};
		}
	public:
		Closure() : _ClosureInner<R(Args...)>(m_buffer), _copy(nullptr), _delete(nullptr) {}

		//copy
		Closure(Closure<R(Args...)> const &other) : _ClosureInner<R(Args...)>(&m_buffer),
			_copy(other._copy), _delete(other._delete) {

			if (_copy) {
				//copy it over
				m_buffer = std::move(_copy(other.m_buffer));
			}

			setBuffer(&m_buffer);
			reset(other);
		}

		//init from a lambda
		template<typename T>
		Closure(T const &lambda) : _ClosureInner<R(Args...)>(&m_buffer) {
			_copyFromLambda(lambda);
		}

		//move
		Closure(Closure<R(Args...)> && other): m_buffer(std::move(other.m_buffer)),
			_ClosureInner<R(Args...)>(nullptr), _copy(other._copy), _delete(other._delete) {
			setBuffer(&m_buffer);
			reset(other);
		}

		~Closure() {
			if (_delete) {
				_delete(m_buffer);
			}
		}

		//copy
		Closure<R(Args...)> &operator=(Closure<R(Args...)> const &other) {
			if (other._copy) {
				//copy it over
				m_buffer = std::move(other._copy(other.m_buffer));
			}
			else {
				//assigning to a nulled out closure...ok
				m_buffer.setSize(0);
			}
			reset(other);
			_delete = other._delete;
			_copy = other._copy;
			return *this;
		}

		//move 
		Closure<R(Args...)> &operator=(Closure<R(Args...)> && other) {
			if (other._copy) {
				//copy it over
				m_buffer = std::move(other._copy(other.m_buffer));
			}
			else {
				//assigning to a nulled out closure...ok
				m_buffer.setSize(0);
			}
			setBuffer(&m_buffer);
			reset(other);
			_delete = other._delete;
			_copy = other._copy;

			return *this;
		}

		//assign from lambda
		template<typename T>
		Closure<R(Args...)> &operator=(T const &lambda) {
			_copyFromLambda(lambda);
			return *this;
		}

		operator bool() {
			return m_buffer;
		}
	};




}