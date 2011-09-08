//
// ---------- header ----------------------------------------------------------
//
// project       nucleus
//
// license       infinit
//
// file          /home/mycure/infinit/nucleus/proton/MutableBlock.hh
//
// created       julien quintard   [sat may 21 12:27:09 2011]
// updated       julien quintard   [wed sep  7 09:04:16 2011]
//

#ifndef NUCLEUS_PROTON_MUTABLEBLOCK_HH
#define NUCLEUS_PROTON_MUTABLEBLOCK_HH

//
// ---------- includes --------------------------------------------------------
//

#include <elle/Elle.hh>

#include <nucleus/proton/Block.hh>
#include <nucleus/proton/Address.hh>
#include <nucleus/proton/Network.hh>
#include <nucleus/proton/Family.hh>
#include <nucleus/proton/Version.hh>
#include <nucleus/proton/Base.hh>

#include <nucleus/neutron/Component.hh>

namespace nucleus
{
  namespace proton
  {

//
// ---------- classes ---------------------------------------------------------
//

    ///
    /// this class abstracts the notion of mutable block, this difference
    /// with immutable blocks lying in the fact that the block's address
    /// is computed in a way that it does not change over time no matter
    /// how the content does.
    ///
    /// note that a version number is included in order to distinguish the
    /// different version of a block's history.
    ///
    /// the _base attribute is used internally to keep a view of the
    /// block's original state i.e before being modified.
    ///
    class MutableBlock:
      public Block
    {
    public:
      //
      // constructors & destructors
      //
      MutableBlock();
      MutableBlock(const Family,
		   const neutron::Component);

      //
      // interfaces
      //

      // object
      declare(MutableBlock);

      // dumpable
      elle::Status	Dump(const elle::Natural32 = 0) const;

      // archivable
      elle::Status	Serialize(elle::Archive&) const;
      elle::Status	Extract(elle::Archive&);

      // fileable
      elle::Status	Load(const Network&,
			     const Address&,
			     const Version&);
      elle::Status	Store(const Network&,
			      const Address&) const;
      elle::Status	Erase(const Network&,
			      const Address&) const;
      elle::Status	Exist(const Network&,
			      const Address&,
			      const Version&) const;

      //
      // attributes
      //
      Version		version;

      Base		_base;
    };

  }
}

#endif
