#pragma once

#include <iostream>
#include <source_location>
#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/chrono.h>

namespace pa {
inline void vlog(fmt::string_view level, fmt::string_view format, fmt::format_args args, const std::source_location &loc) {
    auto nanos = std::chrono::system_clock::now().time_since_epoch().count() % 1'000'000'000;
    std::clog 
        << fmt::format("[{:%F %T}.{:0>9}] [{}({}:{})] [{}] ", 
            fmt::localtime(std::time(nullptr)), nanos, loc.file_name(), loc.line(), loc.column(), level)
        << fmt::vformat(format, args) << "\n";
}


template <typename... Ts>
struct logt {
    logt(fmt::format_string<Ts...> format, Ts&&... args, const std::source_location &loc = std::source_location::current()) {
        vlog("TRACE", format, fmt::make_format_args(args...), loc);
    }
};
template <typename... Ts>
logt(fmt::format_string<Ts...> format, Ts&&...) -> logt<Ts...>;


template <typename... Ts>
struct logd {
    logd(fmt::format_string<Ts...> format, Ts&&... args, const std::source_location &loc = std::source_location::current()) {
        vlog("DEBUG", format, fmt::make_format_args(args...), loc);
    }
};
template <typename... Ts>
logd(fmt::format_string<Ts...> format, Ts&&...) -> logd<Ts...>;


template <typename... Ts>
struct logi {
    logi(fmt::format_string<Ts...> format, Ts&&... args, const std::source_location &loc = std::source_location::current()) {
        vlog("INFO ", format, fmt::make_format_args(args...), loc);
    }
};
template <typename... Ts>
logi(fmt::format_string<Ts...> format, Ts&&...) -> logi<Ts...>;


template <typename... Ts>
struct logw {
    logw(fmt::format_string<Ts...> format, Ts&&... args, const std::source_location &loc = std::source_location::current()) {
        vlog("WARN ", format, fmt::make_format_args(args...), loc);
    }
};
template <typename... Ts>
logw(fmt::format_string<Ts...> format, Ts&&...) -> logw<Ts...>;


template <typename... Ts>
struct loge {
    loge(fmt::format_string<Ts...> format, Ts&&... args, const std::source_location &loc = std::source_location::current()) {
        vlog("ERROR", format, fmt::make_format_args(args...), loc);
    }
};
template <typename... Ts>
loge(fmt::format_string<Ts...> format, Ts&&...) -> loge<Ts...>;


template <typename... Ts>
struct logc {
    logc(fmt::format_string<Ts...> format, Ts&&... args, const std::source_location &loc = std::source_location::current()) {
        vlog("CRIT ", format, fmt::make_format_args(args...), loc);
    }
};
template <typename... Ts>
logc(fmt::format_string<Ts...> format, Ts&&...) -> logc<Ts...>;
}
