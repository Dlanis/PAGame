#pragma once

#define PA_SIMPLE_GETTERSETTER(name) \
    const decltype(name) &get_##name() const { return name; } \
    void set_##name(decltype(name) n) { name = n; }