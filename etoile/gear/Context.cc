//
// ---------- header ----------------------------------------------------------
//
// project       etoile
//
// license       infinit
//
// author        julien quintard   [thu jun 16 10:37:02 2011]
//

//
// ---------- includes --------------------------------------------------------
//

#include <etoile/gear/Context.hh>

namespace etoile
{
  namespace gear
  {

//
// ---------- constructors & destructors --------------------------------------
//

    ///
    /// default constructor.
    ///
    Context::Context(const Nature                               nature):
      nature(nature),
      state(Context::StateUnknown),
      operation(OperationUnknown)
    {
    }

//
// ---------- dumpable --------------------------------------------------------
//

    ///
    /// this function dumps an context object.
    ///
    elle::Status        Context::Dump(elle::Natural32           margin) const
    {
      elle::String      alignment(margin, ' ');

      // display the name.
      std::cout << alignment << "[Context] " << std::endl;

      // display the nature.
      std::cout << alignment << elle::io::Dumpable::Shift << "[Nature] "
                << this->nature << std::endl;

      // display the state.
      std::cout << alignment << elle::io::Dumpable::Shift << "[State] "
                << this->state << std::endl;

      // display the operation.
      std::cout << alignment << elle::io::Dumpable::Shift << "[Operation] "
                << this->operation << std::endl;

      // dump the transcript.
      if (this->transcript.Dump(margin + 2) == elle::Status::Error)
        escape("unable to dump the transcript");

      return elle::Status::Ok;
    }

//
// ---------- archivable ------------------------------------------------------
//

    ///
    /// this method serializes the context object.
    ///
    //elle::Status        Context::Serialize(elle::Archive&       archive) const
    //{
    //  // serialize the attributes.
    //  if (archive.Serialize(
    //        static_cast<elle::Natural8>(this->nature),
    //        static_cast<elle::Natural8>(this->state)) == elle::Status::Error)
    //    escape("unable to serialize the attributes");

    //  return elle::Status::Ok;
    //}

    /////
    ///// this method extracts the context object.
    /////
    //elle::Status        Context::Extract(elle::Archive&         archive)
    //{
    //  // extract the attributes.
    //  if (archive.Extract(
    //        reinterpret_cast<elle::Natural8&>(this->nature),
    //        reinterpret_cast<elle::Natural8&>(this->state)) ==
    //      elle::Status::Error)
    //    escape("unable to extract the attributes");

    //  return elle::Status::Ok;
    //}

  }
}
