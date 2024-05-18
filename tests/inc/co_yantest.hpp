#pragma once
#include <array>
#include <iostream>
#include <string>
#include <sstream>
#include <functional>
#include <format>
#include <chrono>
#include <future>
#include <algorithm>
#include <optional>
#include <coroutine>
#include <cassert>


namespace my
{
    namespace test // my::test
    {
class case_t
{
public:
    struct promise_type;
    using handle_type = std::coroutine_handle<promise_type>;

    enum class state
    {
        INITIALIZED, FAILED, PASSED, TIMEOUT
    };

    struct promise_type
    {
        struct check_awaiter
        {
            bool result;
            inline check_awaiter(bool res) : result(res) {}
            inline bool await_ready() noexcept { return result; }
            inline void await_suspend(std::coroutine_handle<promise_type>) noexcept {}
            inline bool await_resume() noexcept { return result; }
        };


        std::string actions;
        std::string error_message;
        std::stringstream cout;

        case_t::state state;

        inline auto get_return_object() { return case_t(handle_type::from_promise(*this)); }
        inline auto yield_value(std::nullptr_t)
        {
            actions.clear();
            error_message.clear();
            cout.clear();
            return std::suspend_never{};
        }
        inline auto yield_value(const std::string& action)
        {
            this->actions += action + "\n";
            return std::suspend_never{};
        }
        inline auto yield_value(const std::pair<bool, std::string>& check_result)
        {
            this->error_message = check_result.second;
            this->state = state::FAILED; // not suspend if check succeed
            return check_awaiter{ check_result.first };
        }
        inline auto initial_suspend() noexcept
        {
            std::cout.rdbuf(this->cout.rdbuf());
            this->state = state::INITIALIZED;
            return std::suspend_always{};
        }
        inline auto final_suspend() noexcept
        {
            this->state = state::PASSED;
            return std::suspend_always{};
        }
        inline void return_void() noexcept {}
        inline void unhandled_exception()
        {
            this->state = state::FAILED;
            try { throw; }
            catch (const std::exception& e)
            {
                this->error_message = std::string("Exception: ") + e.what();
                throw;
            }
            catch (...)
            {
                this->error_message = "Unknown exception!";
                throw;
            }
        }
    };
private:
    handle_type coro;

public:
    inline case_t(const handle_type& h) : coro(h) {}
    inline case_t(const case_t&) = delete;
    inline case_t(case_t&& other) noexcept: coro(std::exchange(other.coro, nullptr)) {}
    inline case_t& operator=(const case_t&) = delete;
    inline case_t& operator=(case_t&& other) noexcept
    {
        coro = std::exchange(other.coro, nullptr);
        return *this;
    }

    inline auto resume() const
    {
        if (coro && !coro.done()) {
            coro.resume();
        }
        return coro.promise().state;
    }

    inline std::string actions() const { return coro.promise().actions; }
    inline std::string error_message() const { return coro.promise().error_message; }
    inline std::stringstream& cout() const { return coro.promise().cout; }
};

using testing_duration = long long;

class test
{
private:
    size_t _total_cases;
    size_t _passed_cases;
    testing_duration _duration;

public:
    inline test() : _total_cases(0), _passed_cases(0), _duration(0) {}
    inline ~test()
    {
        std::cerr
            << std::format("{}/{} cases passed in {:.1f} seconds.",
                _passed_cases, _total_cases, _duration * 1. / 1000)
            << std::endl;
    }
    inline auto&& new_case(case_t&& t, std::string name, testing_duration time_limit = 3000)
    {
        ++_total_cases;
        std::cerr << std::format("Test {}: {}", _total_cases, name);
        std::chrono::system_clock clock;
        auto start = clock.now();

        auto f = std::future(std::async(std::launch::async, [&t, &clock, &start]() {
            auto state = t.resume();
            while (true)
            {
                switch (state)
                {
                case my::test::case_t::state::FAILED:
                    goto out;
                    break;
                case my::test::case_t::state::PASSED:
                    goto out;
                    break;
                default:
                    break;
                }
                auto state = t.resume();
            }
        out:
            return state;
            }));
        
        auto r = f.wait_for(std::chrono::milliseconds(time_limit));
        case_t::state state = case_t::state::FAILED;
        if (r == std::future_status::ready)
        {
            try
            {
                state = f.get();
            } catch (...) {}
        }
        else
        {
            state = case_t::state::TIMEOUT;
        }


        std::cerr << (state == case_t::state::PASSED ? " -- passed " : " -- failed ")
            << std::format("({}ms)", std::chrono::duration_cast<std::chrono::milliseconds>(clock.now() - start).count())
            << "\n";

        if (state == case_t::state::FAILED)
        {
            std::cerr << "\n** Failed **\n" << t.error_message() << "\n";
        }
        else if (state == case_t::state::TIMEOUT)
        {
            std::cerr << "\n** Timeout **\n";
        }

        if (state != case_t::state::PASSED)
        {
            if (!t.actions().empty())
            {
                std::cerr << "\n[Actions]\n"
                    << t.actions();
            }
            if (auto cout_str = t.cout().str(); !cout_str.empty())
            {
                std::cerr << "\n[Output]\n"
                    << cout_str
                    << "\n";
            }
        }
        else
        {
            _passed_cases++;
        }
        std::cerr << std::endl;
        if (state == case_t::state::TIMEOUT)
        {
            exit(1);
        }

        return *this;
    }
};
    } // namespace my::test
} // namespace my