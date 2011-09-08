//
// ---------- header ----------------------------------------------------------
//
// project       nucleus
//
// license       infinit
//
// file          /home/mycure/infinit/nucleus/proton/Network.hh
//
// created       julien quintard   [sun may  8 01:21:07 2011]
// updated       julien quintard   [wed sep  7 09:05:56 2011]
//

#ifndef NUCLEUS_PROTON_NETWORK_HH
#define NUCLEUS_PROTON_NETWORK_HH

//
// ---------- includes --------------------------------------------------------
//

#include <elle/Elle.hh>

namespace nucleus
{
  namespace proton
  {

    ///
    /// this class identifies a network through a unique name.
    ///
    class Network:
      public elle::Object
    {
    public:
      //
      // constants
      //
      static const Network	Null;

      //
      // constructors & destructors
      //
      Network();

      //
      // methods
      //
      elle::Status	Create(const elle::String&);

      //
      // attributes
      //
      elle::String	name;

      //
      // interfaces
      //

      // object
      declare(Network);
      elle::Boolean	operator==(const Network&) const;
      elle::Boolean	operator<(const Network&) const;

      // dumpable
      elle::Status	Dump(const elle::Natural32 = 0) const;

      // archivable
      elle::Status	Serialize(elle::Archive&) const;
      elle::Status	Extract(elle::Archive&);
    };

  }
}

#endif
