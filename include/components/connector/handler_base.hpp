/*
 * Copyright (c) 2020 LACIT. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *Created by illidansr on 22.08.2020.
 * @details
 * @file
 * @brief
 */
#ifndef STORAGE_KERNEL_INCLUDE_COMPONENTS_CONNECTOR_HANDLER_BASE_HPP
#define STORAGE_KERNEL_INCLUDE_COMPONENTS_CONNECTOR_HANDLER_BASE_HPP

#include <iostream>
#include <atomic>
#include <memory>
#include <type_traits>
#include <exception>
#include <coroutine>


template<typename T>
struct coreturn {
    struct promise;
    friend struct promise;
    using handle_type = std::coroutine_handle<promise>;
    coreturn(const coreturn &) = delete;
    coreturn(coreturn &&s)
            : coro(s.coro) {
        std::cout << "Coreturn wrapper moved" << std::endl;
        s.coro = nullptr;
    }
    ~coreturn() {
        std::cout << "Coreturn wrapper gone" << std::endl;
        if ( coro ) coro.destroy();
    }
    coreturn &operator = (const coreturn &) = delete;
    coreturn &operator = (coreturn &&s) {
        coro = s.coro;
        s.coro = nullptr;
        return *this;
    }
    struct promise {
        friend struct coreturn;
        promise() {
            std::cout << "Promise created" << std::endl;
        }
        ~promise() {
            std::cout << "Promise died" << std::endl;
        }
        auto return_value(T v) {
            std::cout << "Got an answer of " << v << std::endl;
            value = v;
            return std::suspend_never{};
        }
        auto final_suspend() {
            std::cout << "Finished the coro" << std::endl;
            return std::suspend_always{};
        }
        void unhandled_exception() {
            std::exit(1);
        }
    private:
        T value;
    };
    bool await_ready() {
        const auto ready = coro.done();
        std::cout << "Await " << (ready ? "is ready" : "isn't ready") << std::endl;
        return coro.done();
    }

    void await_suspend(std::coroutine_handle<> awaiting) {
        std::cout << "About to resume the lazy" << std::endl;
        coro.resume();
        std::cout << "About to resume the awaiter" << std::endl;
        awaiting.resume();
    }

    auto await_resume() {
        const auto r = coro.promise().value;
        std::cout << "Await value is returned: " << r << std::endl;
        return r;
    }
protected:
    T get() {
        std::cout << "We got asked for the return value..." << std::endl;
        if ( not coro.done() ) coro.resume();
        return coro.promise().value;
    }
    coreturn(handle_type h)
            : coro(h) {
        std::cout << "Created a coreturn wrapper object" << std::endl;
    }
    handle_type coro;
};

template<typename T>
struct sync : public coreturn<T> {
    using coreturn<T>::coreturn;
    using handle_type = typename coreturn<T>::handle_type;
    T get() {
        std::cout << "We got asked for the return value..." << std::endl;
        return coreturn<T>::get();
    }
    bool await_ready() {
        const auto ready = coro.done();
        std::cout << "Await " << (ready ? "is ready" : "isn't ready") << std::endl;
        return coro.done();
    }

    void await_suspend(std::coroutine_handle<> awaiting) {
        std::cout << "About to resume the lazy" << std::endl;
        coro.resume();
        std::cout << "About to resume the awaiter" << std::endl;
        awaiting.resume();
    }

    auto await_resume() {
        const auto r = coro.promise().value;
        std::cout << "Await value is returned: " << r << std::endl;
        return r;
    }
    handle_type coro;
    struct promise_type : public coreturn<T>::promise {
        auto get_return_object() {
            std::cout << "Send back a sync" << std::endl;
            return sync<T>{handle_type::from_promise(*this)};
        }
        auto initial_suspend() {
            std::cout << "Started the coroutine, don't stop now!" << std::endl;
            return std::suspend_never{};
        }
    };
};


template<typename T>
struct lazy : public coreturn<T> {
    using coreturn<T>::coreturn;
    using handle_type = typename coreturn<T>::handle_type;;
    T get() {
        std::cout << "We got asked for the return value..." << std::endl;
        if ( not this->coro.done() ) this->coro.resume();
        return coreturn<T>::get();
    }


    struct promise_type : public coreturn<T>::promise {
        auto get_return_object() {
            std::cout << "Send back a lazy" << std::endl;
            return lazy<T>{handle_type::from_promise(*this)};
        }
        auto initial_suspend() {
            std::cout << "Started the coroutine, put the brakes on!" << std::endl;
            return std::suspend_always{};
        }
    };
};

lazy<int> answer() {
    std::cout << "Thinking deep thoghts..." << std::endl;
    co_return 42;
}

sync<int> await_answer() {
    std::cout << "Started await_answer" << std::endl;
    auto a = answer();
    std::cout << "Got a coroutine, let's get a value" << std::endl;
    auto v = co_await a;
    std::cout << "And the coroutine value is: " << v << std::endl;
    v = co_await a;
    std::cout << "And the coroutine value is still: " << v << std::endl;
    co_return 0;
}



class BaseHandler {
public:
    BaseHandler() : instance(std::make_shared<BaseHandler>()) {

    }

    template<typename T, typename U>
    typename std::enable_if<std::is_member_function_pointer_v<U>> wait(T type) {
        auto v = co_await type;

    }

    template <typename T>
    void set_next(T&& obj) {
        if constexpr (std::is_convertible_v<T, BaseHandler>) {
            instance = std::forward<std::atomic<BaseHandler*>>(obj);
        } else {
            throw std::runtime_error("error instance");
        }

    }

    template<typename T, typename... Types>
    decltype(auto) handle(T t, Types... types) {
        instance->set_next(t);
        handle(types...);
    }

private:
    std::shared_ptr<BaseHandler> instance;


};

#endif //STORAGE_KERNEL_INCLUDE_COMPONENTS_CONNECTOR_HANDLER_BASE_HPP
