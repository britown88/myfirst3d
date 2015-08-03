#pragma once

#include "MemoryBuffer.hpp"

namespace utl {

	class IClosureDestroy {
	public:
		~IClosureDestroy() {}
		virtual void operator()(MemoryBuffer &buffer) = 0;
	};

	class ClosureDestroyDummy : public IClosureDestroy {
	public:
		void operator()(MemoryBuffer &buffer) {}

		static ClosureDestroyDummy &Instance() {
			static ClosureDestroyDummy instance;
			return instance;
		}
	};

	template<typename T> 
	class ClosureDestroySingleton : public IClosureDestroy {
	public:
		void operator()(MemoryBuffer &buffer) {
			if (buffer) {
				((T*)buffer.data())->~T();
				buffer.setSize(0);
			}
		}

		static ClosureDestroySingleton &Instance() {
			static ClosureDestroySingleton instance;
			return instance;
		}
	};

	template<typename T> class Closure {};

	template<typename R, typename... Args>
	class Closure<R(Args...)> {
		typedef Closure<R(Args...)> Type;

		IClosureDestroy &m_destroy;

		MemoryBuffer m_buffer;
		R(*m_execute)(MemoryBuffer &, Args...);

		template<typename L>
		void _copyFromLambda(L const &lambda) {			

			m_buffer.setSize(sizeof(L));
			new(m_buffer.data()) L(lambda);

			m_execute = [](MemoryBuffer &buff, Args... args) {
				return ((L*)buff.data())->operator()(args...);
			};
		}

		//copy constructors here are prvate... no copyrino!
		Closure(Type const &other) {}
		Type &operator=(Type const &other) { return *this; }

	public:		

		//default
		Closure() : m_execute(nullptr), m_destroy(ClosureDestroyDummy::Instance()) {}

		//move
		Closure(Type && other):m_buffer(std::move(other.m_buffer)), m_execute(other.m_execute), m_destroy(other.m_destroy) {}

		Type &operator=(Type && other) {
			m_destroy(m_buffer);			
			m_buffer = std::move(other.m_buffer);
			m_destroy = other.m_destroy;
			m_execute = other.m_execute;
			return *this;
		}

		//init from lambda
		template<typename L>
		Closure(L const &lambda):m_destroy(ClosureDestroySingleton<L>::Instance()) {
			_copyFromLambda(lambda);
		}

		template<typename L>
		Type &operator=(L const &lambda) {
			m_destroy(m_buffer);
			m_destroy = ClosureDestroySingleton<L>::Instance();
			_copyFromLambda(lambda);
			return *this;
		}

		//destruct
		~Closure() {
			m_destroy(m_buffer);
		}
		
		operator bool() {
			return m_buffer;
		}

		R operator()(Args... args) {
			return m_execute(m_buffer, args...);
		}
	};
}