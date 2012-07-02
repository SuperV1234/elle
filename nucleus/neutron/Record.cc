
#include <nucleus/neutron/Record.hh>
#include <elle/idiom/Open.hh>

namespace nucleus
{
  namespace neutron
  {

//
// ---------- definitions -----------------------------------------------------
//

    ///
    /// this defines an unexisting record.
    ///
    const Record                Record::Null;

//
// ---------- constructors & destructors --------------------------------------
//

    ///
    /// empty constructor.
    ///
    Record::Record()
    {
    }

    ///
    /// default constructor.
    ///
    Record::Record(const Subject&                               subject,
                   const Permissions&                           permissions,
                   const Token&                                 token):
      subject(subject),
      permissions(permissions),
      token(token)
    {
    }

//
// ---------- methods ---------------------------------------------------------
//

    ///
    /// this method creates/updates a record.
    ///
    elle::Status        Record::Update(const Subject&           subject,
                                       const Permissions&       permissions,
                                       elle::cryptography::SecretKey const&   key)
    {
      // set the subject.
      this->subject = subject;

      // set the permissions.
      this->permissions = permissions;

      // then, if the subject has the read permission, create a token.
      if ((this->permissions & PermissionRead) == PermissionRead)
        {
          // create the token with the public key of the user or the
          // group owner, depending on the subject.
          switch (subject.type)
            {
            case Subject::TypeUser:
              {
                // create the token with the user's public key.
                if (this->token.Update(key,
                                       *subject.user) == elle::Status::Error)
                  escape("unable to create the token");

                break;
              }
            case Subject::TypeGroup:
              {
                // XXX to implement.

                break;
              }
            default:
              {
                escape("unknown subject type");
              }
            }
        }
      else
        {
          // reinitialize the token.
          this->token = Token::Null;
        }

      return elle::Status::Ok;
    }

    ///
    /// this method creates/updates a record with a ready-to-be-used token.
    ///
    elle::Status        Record::Update(const Subject&           subject,
                                       const Permissions&       permissions,
                                       const Token&             token)
    {
      // set the subject.
      this->subject = subject;

      // set the permissions.
      this->permissions = permissions;

      // set the token.
      this->token = token;

      return elle::Status::Ok;
    }

//
// ---------- object ----------------------------------------------------------
//

    ///
    /// this operator compares two objects.
    ///
    elle::Boolean       Record::operator==(const Record&        element) const
    {
      // check the address as this may actually be the same object.
      if (this == &element)
        return true;

      // compare the attributes.
      if ((this->subject != element.subject) ||
          (this->permissions != element.permissions) ||
          (this->token != element.token))
        return false;

      return true;
    }

    ///
    /// this macro-function call generates the object.
    ///
    embed(Record, _());

//
// ---------- dumpable --------------------------------------------------------
//

    ///
    /// this function dumps a record.
    ///
    elle::Status        Record::Dump(elle::Natural32            margin) const
    {
      elle::String      alignment(margin, ' ');

      std::cout << alignment << "[Record]" << std::endl;

      // dump the subject.
      if (this->subject.Dump(margin + 2) == elle::Status::Error)
        escape("unable to dump the subject");

      // dump the permissions.
      std::cout << alignment << elle::io::Dumpable::Shift
                << "[Permissions] " << std::dec
                << (int)this->permissions << std::endl;

      // dump the token.
      if (this->token.Dump(margin + 2) == elle::Status::Error)
        escape("unable to dump the token");

      return elle::Status::Ok;
    }

    ///
    /// this method returns the symbol of a record i.e the subject.
    ///
    Subject&            Record::Symbol()
    {
      return (this->subject);
    }

  }
}
