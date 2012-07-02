#ifndef ELLE_CRYPTOGRAPHY_KEYPAIR_HH
# define ELLE_CRYPTOGRAPHY_KEYPAIR_HH

# include <elle/types.hh>

# include <elle/radix/Object.hh>

# include <elle/concept/Fileable.hh>

# include <elle/cryptography/fwd.hh>
# include <elle/cryptography/PublicKey.hh>
# include <elle/cryptography/PrivateKey.hh>

# include <elle/idiom/Open.hh>

namespace elle
{
  namespace cryptography
  {

    ///
    /// this class represents a cryptographic key pair _i.e_ a pair of public
    /// and private keys.
    ///
    /// a public key is noted with a capital 'K' while a private key is
    /// noted with a lower-case 'k'.
    ///
    class KeyPair
      : public radix::Object
      , public elle::concept::Serializable<KeyPair>
      , public elle::concept::Fileable<>
    {
    public:
      //
      // static methods
      //
      static Status     Initialize();
      static Status     Clean();

      //
      // constants
      //
      struct                            Default
      {
        static const Natural32          Length;
      };

      static const KeyPair              Null;

      static const String               Extension;

      //
      // constructors & destructors
      //
      KeyPair();

      //
      // methods
      //
      Status            Generate();
      Status            Generate(const Natural32);

      Status            Create(const PublicKey&,
                               const PrivateKey&);

      Status            Rotate(const Seed&,
                               KeyPair&) const;

      //
      // interfaces
      //

      // object
      declare(KeyPair);
      Boolean           operator ==(const KeyPair&) const;

      // dumpable
      Status            Dump(const Natural32 = 0) const;

      // fileable
      Status            Load(const io::Path&,
                             const String&);
      Status            Store(const io::Path&,
                              const String&) const;
      Status            Erase(const io::Path&) const;
      Status            Exist(const io::Path&) const;

      //
      // attributes
      //
      PublicKey         K;
      PrivateKey        k;

      struct                    Contexts
      {
        static ::EVP_PKEY_CTX*  Generate;
      };
    };

  }
}

#include <elle/cryptography/KeyPair.hxx>

#endif
