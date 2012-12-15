#include <cryptography/SecretKey.hh>
#include <cryptography/Digest.hh>
#include <cryptography/random.hh>
#include <cryptography/Cipher.hh>
#include <cryptography/cryptography.hh>
#include <cryptography/finally.hh>

#include <elle/log.hh>

ELLE_LOG_COMPONENT("infinit.cryptography.SecretKey");

namespace infinit
{
  namespace cryptography
  {
    /*----------.
    | Constants |
    `----------*/

    elle::Character const SecretKey::Constants::magic[] = "Salted__";

    ::EVP_CIPHER const* SecretKey::Algorithms::Cipher =
      ::EVP_aes_256_cbc();
    ::EVP_MD const* SecretKey::Algorithms::Digest =
      ::EVP_md5();

    /*---------------.
    | Static Methods |
    `---------------*/

    SecretKey
    SecretKey::generate(elle::Natural32 const length)
    {
      ELLE_TRACE_FUNCTION(length);

      // Convert the length in a byte-specific size.
      elle::Natural32 size = length / 8;

      // Generate a buffer-based password.
      elle::Buffer password{random::generate<elle::Buffer>(size)};

      // Return a new secret key.
      return (SecretKey{std::move(password)});
    }

    /*-------------.
    | Construction |
    `-------------*/

    SecretKey::SecretKey()
    {
      // Make sure the cryptographic system is set up.
      cryptography::require();
    }

    SecretKey::SecretKey(elle::String const& password):
      _buffer(reinterpret_cast<elle::Byte const*>(password.c_str()),
              password.length())
    {
      // Make sure the cryptographic system is set up.
      cryptography::require();
    }

    SecretKey::SecretKey(SecretKey const& other):
      _buffer(other._buffer.contents(), other._buffer.size())
    {
      // Make sure the cryptographic system is set up.
      cryptography::require();
    }

    SecretKey::SecretKey(elle::Buffer&& buffer):
      _buffer(std::move(buffer))
    {
      // Make sure the cryptographic system is set up.
      cryptography::require();
    }

    /*--------.
    | Methods |
    `--------*/

    Cipher
    SecretKey::encrypt(Plain const& plain) const
    {
      ELLE_TRACE_METHOD(plain);

      // Generate a salt.
      unsigned char salt[PKCS5_SALT_LEN];

      ::RAND_pseudo_bytes(salt, sizeof (salt));

      // Check that the secret key's buffer has a non-null address.
      //
      // Otherwise, EVP_BytesToKey() is non-deterministic :(
      ELLE_ASSERT(this->_buffer.contents() != nullptr);

      // Generate a key/IV tuple based on the salt.
      unsigned char key[EVP_MAX_KEY_LENGTH];
      unsigned char iv[EVP_MAX_IV_LENGTH];

      if (::EVP_BytesToKey(SecretKey::Algorithms::Cipher,
                           SecretKey::Algorithms::Digest,
                           salt,
                           this->_buffer.contents(),
                           this->_buffer.size(),
                           1,
                           key,
                           iv) > static_cast<int>(sizeof(key)))
        throw elle::Exception("the generated key size is too large");

      // Initialize the cipher context.
      ::EVP_CIPHER_CTX context;

      ::EVP_CIPHER_CTX_init(&context);

      CRYPTOGRAPHY_FINALLY_ACTION_CLEANUP_CIPHER_CONTEXT(context);

      // Initialise the ciphering process.
      if (::EVP_EncryptInit_ex(&context,
                               SecretKey::Algorithms::Cipher,
                               nullptr,
                               key,
                               iv) == 0)
        throw elle::Exception("%s",
                              ::ERR_error_string(ERR_get_error(), nullptr));

      // Retreive the cipher-specific block size.
      elle::Natural32 block_size = ::EVP_CIPHER_CTX_block_size(&context);

      // Allocate the cipher.
      Cipher cipher(sizeof (Constants::magic) -
                    1 +
                    sizeof (salt) +
                    plain.buffer().size() +
                    block_size);

      // Embed the magic directly into the cipher.
      ::memcpy(cipher.buffer().mutable_contents(),
               Constants::magic,
               sizeof (Constants::magic) - 1);

      // Copy the salt directly into the cipher.
      ::memcpy(cipher.buffer().mutable_contents() +
               sizeof (Constants::magic) - 1,
               salt,
               sizeof (salt));

      // Initialise the cipher's size.
      int size_header(sizeof (Constants::magic) - 1 + sizeof (salt));

      // Cipher the plain text.
      int size_update(0);

      if (::EVP_EncryptUpdate(&context,
                              cipher.buffer().mutable_contents() +
                              size_header,
                              &size_update,
                              plain.buffer().contents(),
                              plain.buffer().size()) == 0)
        throw elle::Exception("%s",
                              ::ERR_error_string(ERR_get_error(), nullptr));

      // Finialise the ciphering process.
      int size_finalize(0);

      if (::EVP_EncryptFinal_ex(&context,
                                cipher.buffer().mutable_contents() +
                                size_header + size_update,
                                &size_finalize) == 0)
        throw elle::Exception("%s",
                              ::ERR_error_string(ERR_get_error(), nullptr));

      // Update the cipher size with the actual size of the generated data.
      cipher.buffer().size(size_header + size_update + size_finalize);

      // Clean up the cipher context.
      ::EVP_CIPHER_CTX_cleanup(&context);

      CRYPTOGRAPHY_FINALLY_ABORT(context);

      return (cipher);
    }

    Clear
    SecretKey::decrypt(Cipher const& cipher) const
    {
      ELLE_TRACE_METHOD(cipher);

      // Check whether the cipher was produced with a salt.
      if (::memcmp(Constants::magic,
                   cipher.buffer().contents(),
                   sizeof (Constants::magic) - 1) != 0)
        throw elle::Exception("the cipher was produced without any salt");

      // Copy the salt for the sack of clarity.
      unsigned char salt[PKCS5_SALT_LEN];

      ::memcpy(salt,
               cipher.buffer().contents() + sizeof (Constants::magic) - 1,
               sizeof (salt));

      // Check that the secret key's buffer has a non-null address.
      //
      // Otherwise, EVP_BytesToKey() is non-deterministic :(
      ELLE_ASSERT(this->_buffer.contents() != nullptr);

      // Generate the key/IV tuple based on the salt.
      unsigned char key[EVP_MAX_KEY_LENGTH];
      unsigned char iv[EVP_MAX_IV_LENGTH];

      if (::EVP_BytesToKey(SecretKey::Algorithms::Cipher,
                           SecretKey::Algorithms::Digest,
                           salt,
                           this->_buffer.contents(),
                           this->_buffer.size(),
                           1,
                           key,
                           iv) > static_cast<int>(sizeof(key)))
        throw elle::Exception("the generated key size is too large");

      // Initialize the cipher context.
      ::EVP_CIPHER_CTX context;

      ::EVP_CIPHER_CTX_init(&context);

      CRYPTOGRAPHY_FINALLY_ACTION_CLEANUP_CIPHER_CONTEXT(context);

      // Initialise the ciphering process.
      if (::EVP_DecryptInit_ex(&context,
                               SecretKey::Algorithms::Cipher,
                               nullptr,
                               key,
                               iv) == 0)
        throw elle::Exception("%s",
                              ::ERR_error_string(ERR_get_error(), nullptr));

      // Retreive the cipher-specific block size.
      elle::Natural32 block_size = ::EVP_CIPHER_CTX_block_size(&context);

      // Allocate the clear;
      Clear clear(cipher.buffer().size() -
                  (sizeof (Constants::magic) - 1 + sizeof (salt)) +
                  block_size);

      // Decipher the cipher text.
      int size_update(0);

      if (::EVP_DecryptUpdate(&context,
                              clear.buffer().mutable_contents(),
                              &size_update,
                              cipher.buffer().contents() +
                              sizeof (Constants::magic) - 1 +
                              sizeof (salt),
                              cipher.buffer().size() -
                              (sizeof (Constants::magic) - 1 +
                               sizeof (salt))) == 0)
        throw elle::Exception("%s",
                              ::ERR_error_string(ERR_get_error(), nullptr));

      // Finalise the ciphering process.
      int size_final(0);

      if (::EVP_DecryptFinal_ex(&context,
                                clear.buffer().mutable_contents() + size_update,
                                &size_final) == 0)
        throw elle::Exception("%s",
                              ::ERR_error_string(ERR_get_error(), nullptr));

      // Update the clear size with the actual size of the data decrypted.
      clear.buffer().size(size_update + size_final);

      // Clean up the cipher context.
      ::EVP_CIPHER_CTX_cleanup(&context);

      CRYPTOGRAPHY_FINALLY_ABORT(context);

      return (clear);
    }

    /*----------.
    | Operators |
    `----------*/

    elle::Boolean
    SecretKey::operator ==(SecretKey const& other) const
    {
      if (this == &other)
        return (true);

      // Compare the internal buffers.
      return (this->_buffer == other._buffer);
    }

    /*----------.
    | Printable |
    `----------*/

    void
    SecretKey::print(std::ostream& stream) const
    {
      stream << this->_buffer;
    }
  }
}
