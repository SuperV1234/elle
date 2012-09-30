#ifndef NUCLEUS_PROTON_FAMILY_HH
# define NUCLEUS_PROTON_FAMILY_HH

# include <iosfwd>

namespace nucleus
{
  namespace proton
  {

    ///
    /// this enumeration defines the block's physical family.
    ///
    enum Family
      {
        FamilyUnknown = 0, // XXX

        FamilyContentHashBlock,
        FamilyPublicKeyBlock,
        FamilyOwnerKeyBlock,
        FamilyImprintBlock,

        Families = FamilyImprintBlock + 1 // XXX
      };

    std::ostream&
    operator <<(std::ostream& stream,
                Family const family);

  }
}

#endif
