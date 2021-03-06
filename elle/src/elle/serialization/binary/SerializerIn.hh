#ifndef ELLE_SERIALIZATION_BINARY_SERIALIZERIN_HH
# define ELLE_SERIALIZATION_BINARY_SERIALIZERIN_HH

# include <vector>

# include <elle/attribute.hh>
# include <elle/serialization/SerializerIn.hh>

namespace elle
{
  namespace serialization
  {
    namespace binary
    {
       class ELLE_API SerializerIn
         : public serialization::SerializerIn
      {
      /*------.
      | Types |
      `------*/
      public:
        typedef SerializerIn Self;
        typedef serialization::SerializerIn Super;

      /*-------------.
      | Construction |
      `-------------*/
      public:
        SerializerIn(std::istream& input, bool versioned = true);
        SerializerIn(std::istream& input,
                     Versions versions, bool versioned = true);
      private:
        void
        _check_magic(std::istream& input);

      /*--------------.
      | Serialization |
      `--------------*/
      protected:
        virtual
        bool
        _text() const override;
        virtual
        void
        _serialize(int64_t& v) override;
        virtual
        void
        _serialize(uint64_t& v) override;
        virtual
        void
        _serialize(int32_t& v) override;
        virtual
        void
        _serialize(uint32_t& v) override;
        virtual
        void
        _serialize(int16_t& v) override;
        virtual
        void
        _serialize(uint16_t& v) override;
        virtual
        void
        _serialize(int8_t& v) override;
        virtual
        void
        _serialize(uint8_t& v) override;
        virtual
        void
        _serialize(double& v) override;
        virtual
        void
        _serialize(bool& v) override;
        virtual
        void
        _serialize(std::string& v) override;
        virtual
        void
        _serialize(elle::Buffer& v) override;
        virtual
        void
        _serialize(boost::posix_time::ptime& v) override;
        virtual
        void
        _serialize_time_duration(std::int64_t& ticks,
                                 std::int64_t& num,
                                 std::int64_t& denom) override;
        virtual
        void
        _serialize_named_option(std::string const& name,
                                bool,
                                std::function<void ()> const& f) override;
        virtual
        void
        _serialize_option(bool,
                          std::function<void ()> const& f) override;
        virtual
        void
        _serialize_array(int size,
                         std::function<void ()> const& f) override;
        virtual
        void
        _deserialize_dict_key(
          std::function<void (std::string const&)> const& f) override;

        virtual
        bool
        _enter(std::string const& name) override;
        virtual
        void
        _leave(std::string const& name) override;
      public:
        static
        size_t
        serialize_number(std::istream& output,
                         int64_t& value);
      private:
        int64_t _serialize_number();
        template <typename T>
        void
        _serialize_int(T& v);
    };
  }
}

}

#endif
