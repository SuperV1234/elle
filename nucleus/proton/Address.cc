#include <nucleus/proton/Address.hh>
#include <nucleus/proton/Network.hh>
#include <nucleus/Exception.hh>

#include <elle/format/hexadecimal.hh>
#include <elle/log.hh>

namespace nucleus
{
  namespace proton
  {
//
// ---------- Address ---------------------------------------------------------
//

    /*----------.
    | Constants |
    `----------*/

    cryptography::oneway::Algorithm const Address::Constants::oneway_algorithm(
      cryptography::oneway::Algorithm::sha256);

    /*---------------.
    | Static Methods |
    `---------------*/

    Address const&
    Address::null()
    {
      static Address address(Address::Type::null);

      return (address);
    }

    /*-------------.
    | Construction |
    `-------------*/

    Address::Address():
      _valid(nullptr)
    {
    }

    Address::Address(Type const type):
      _type(type),
      _valid(nullptr)
    {
      switch (this->_type)
        {
        case Type::null:
          {
            // Nothing to do; this is the right way to construct such special
            // addresses.

            break;
          }
        case Type::valid:
          {
            throw Exception("valid addresses cannot be built through this "
                            "constructor");
          }
        default:
          throw Exception("unknown address type '%s'", this->_type);
        }
    }

    Address::Address(Address const& other):
      _type(other._type),
      _valid(nullptr)
    {
      if (other._valid != nullptr)
        {
          this->_valid =
            new Valid(
              other._valid->network(),
              other._valid->family(),
              other._valid->component(),
              other._valid->digest());
        }
    }

    Address::~Address()
    {
      delete this->_valid;
    }

    /*--------.
    | Methods |
    `--------*/

    elle::String const
    Address::unique() const
    {
      ELLE_ASSERT(this->_type == Type::valid);
      ELLE_ASSERT(this->_valid != nullptr);

      // note that a unique element i.e the digest has already been computed
      // when the address was created.
      //
      // therefore, this method simply returns a string representation of
      // the digest.
      return (elle::format::hexadecimal::encode(
        this->_valid->digest().buffer()));
    }

    Network const&
    Address::network() const
    {
      ELLE_ASSERT(this->_type == Type::valid);
      ELLE_ASSERT(this->_valid != nullptr);

      return (this->_valid->network());
    }

    Family const&
    Address::family() const
    {
      ELLE_ASSERT(this->_type == Type::valid);
      ELLE_ASSERT(this->_valid != nullptr);

      return (this->_valid->family());
    }

    neutron::Component const&
    Address::component() const
    {
      ELLE_ASSERT(this->_type == Type::valid);
      ELLE_ASSERT(this->_valid != nullptr);

      return (this->_valid->component());
    }

    /*----------.
    | Operators |
    `----------*/

    elle::Boolean
    Address::operator ==(Address const& other) const
    {
      if (this == &other)
        return (true);

      if (this->_type != other._type)
        return (false);

      if (this->_type == Type::valid)
        {
          ELLE_ASSERT(this->_valid != nullptr);
          ELLE_ASSERT(other._valid != nullptr);

          return (this->_valid->digest() == other._valid->digest());
        }

      return (true);
    }

    elle::Boolean
    Address::operator <(Address const& other) const
    {
      if (this == &other)
        return (false);

      if (this->_type != other._type)
        return (this->_type < other._type);

      if (this->_type == Type::valid)
        {
          ELLE_ASSERT(this->_valid != nullptr);
          ELLE_ASSERT(other._valid != nullptr);

          return (this->_valid->digest() < other._valid->digest());
        }

      return (false);
    }

    elle::Boolean
    Address::operator <=(Address const& other) const
    {
      if (this == &other)
        return (true);

      if (this->_type != other._type)
        return (this->_type <= other._type);

      if (this->_type == Type::valid)
        {
          ELLE_ASSERT(this->_valid != nullptr);
          ELLE_ASSERT(other._valid != nullptr);

          return (this->_valid->digest() <= other._valid->digest());
        }

      return (false);
    }

    /*---------.
    | Dumpable |
    `---------*/

    elle::Status
    Address::Dump(elle::Natural32           margin) const
    {
      elle::String      alignment(margin, ' ');

      // check the value.
      switch (this->_type)
        {
        case Address::Type::null:
          {
            std::cout << alignment << "[Address] " << elle::none << std::endl;

            break;
          }
        case Address::Type::valid:
          {
            ELLE_ASSERT(this->_valid != nullptr);

            // display the name.
            std::cout << alignment << "[Address] " << this << std::endl;

            if (this->_valid->network().Dump(margin + 2) == elle::Status::Error)
              escape("XXX");

            // display the family.
            std::cout << alignment << elle::io::Dumpable::Shift << "[Family] "
                      << this->_valid->family() << std::endl;

            // display the component.
            std::cout << alignment << elle::io::Dumpable::Shift
                      << "[Component] "
                      << this->_valid->component() << std::endl;

            // dump the digest.
            if (this->_valid->digest().Dump(margin + 2) == elle::Status::Error)
              escape("unable to dump the digest");

            break;
          }
        default:
          throw Exception("unknown address type '%s'", this->_type);
        }

      return elle::Status::Ok;
    }

    /*----------.
    | Printable |
    `----------*/

    void
    Address::print(std::ostream& stream) const
    {
      switch (this->_type)
        {
        case Type::null:
          {
            stream << "null";
            break;
          }
        case Type::valid:
          {
            ELLE_ASSERT(this->_valid != nullptr);

            stream << "{"
                   << this->_valid->network()
                   << ", "
                   << this->_valid->family()
                   << ", "
                   << this->_valid->component()
                   << ", "
                   << this->unique()
                   << "}";
            break;
          }
        default:
          throw Exception("unknown address type '%s'", this->_type);
        }
    }

//
// ---------- Valid -----------------------------------------------------------
//

    /*-------------.
    | Construction |
    `-------------*/

    Address::Valid::Valid()
    {
    }

    Address::Valid::Valid(Network const& network,
                          Family const& family,
                          neutron::Component const& component,
                          cryptography::Digest const& digest):
      _network(network),
      _family(family),
      _component(component),
      _digest(digest)
    {
    }

//
// ---------- Operators -------------------------------------------------------
//

    std::ostream&
    operator <<(std::ostream& stream,
                Address::Type const type)
    {
      switch (type)
        {
        case Address::Type::null:
          {
            stream << "null";
            break;
          }
        case Address::Type::valid:
          {
            stream << "valid";
            break;
          }
        default:
          {
            throw Exception("unknown address type: '%s'",
                            static_cast<int>(type));
          }
        }

      return (stream);
    }
  }
}
