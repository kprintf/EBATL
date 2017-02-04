#ifndef EBA_STRING_HPP
#       define EBA_STRING_HPP
#       ifndef EBA_NOSTL
#               include <string>
#       endif
namespace eba {

#       ifdef EBA_NOSTL
#               error NOSTL mode is unimplemented yet
        /*TODO string realization for stl-less version*/
#       else
        typedef std::string string;
#       endif

        template<typename TString>
        char default_string_fetch(TString s, size_t i) {
                return s[i];
        }
}
#endif
// vim: et:ts=8:sw=8
