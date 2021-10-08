#include <ostream>
#include <dongmensql/literal.h>

std::ostream &operator<<(std::ostream &os, Literal *val) {
    if (val == nullptr) {
        os << "<nullptr>";
        return os;
    }

    os << val->original_value;
    return os;
}