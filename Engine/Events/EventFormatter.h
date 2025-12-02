#pragma once

#include "Events.h"
#include "spdlog/fmt/ostr.h"

namespace fmt {

    template <typename T, typename Char>
    struct formatter<
        T,
        Char,
        std::enable_if_t<std::is_base_of<Monsi::Event, T>::value>
    > : ostream_formatter {
    };

}