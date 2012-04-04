#include <ostream>

#include "elle/serialize/JSONArchive.hh"

#include "Float.hh"

namespace elle { namespace format { namespace json { namespace detail {

    template<>
    void Float::Save(elle::serialize::OutputJSONArchive& ar) const
    {
      ar << _value;
    }

}}}}

