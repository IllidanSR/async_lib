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
 *Created by illidansr on 23.08.2020.
 * @details
 * @file
 * @brief
 */
#ifndef STORAGE_KERNEL_INCLUDE_COMPONENTS_CONNECTOR_BASETASKQUEUE_HPP
#define STORAGE_KERNEL_INCLUDE_COMPONENTS_CONNECTOR_BASETASKQUEUE_HPP


#include <mutex>
#include <memory>
#include <coroutine>
#include <deque>
#include <condition_variable>

template <typename T>
class WorkQueue {
public:
    void push_task(T task) {
        std::unique_lock<std::mutex> lock(tasks_mutex_);
        tasks_.push_back(std::move(task));
        lock.unlock();
        tasks_cond_.notify_one();
    };
    void run() {
        stop_ = false;
        while (1) {
            T t = pop_task();
            if (!t)
                return;
            t();
        }
    };
    void shut_down() {
        stop_ = true;
        tasks_cond_.notify_all();
    };

private:
    std::mutex              tasks_mutex_;
    std::condition_variable tasks_cond_;
    std::deque<T>           tasks_;
    std::atomic<bool>       stop_ = false;

    T pop_task() {
        T ret{};
        std::unique_lock<std::mutex> lock(tasks_mutex_);
        tasks_cond_.wait(lock, [this]{ return !tasks_.empty() || stop_; });
        if (tasks_.empty()) {
            return ret;
        }
        ret = std::move(tasks_.front());
        tasks_.pop_front();
        return ret;
    };
};

#endif //STORAGE_KERNEL_INCLUDE_COMPONENTS_CONNECTOR_BASETASKQUEUE_HPP
