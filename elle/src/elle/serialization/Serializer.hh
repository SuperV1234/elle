#ifndef ELLE_SERIALIZATION_SERIALIZER_HH
# define ELLE_SERIALIZATION_SERIALIZER_HH

# include <functional>
# include <list>
# include <memory>
# include <string>
# include <unordered_map>

# include <boost/optional.hpp>

# include <elle/serialization/fwd.hh>
# include <elle/sfinae.hh>

namespace elle
{
  namespace serialization
  {
    template <typename T>
    class as;

    class Serializer
    {
    /*------.
    | Types |
    `------*/
    public:
      typedef Serializer Self;

    /*--------------.
    | Serialization |
    `--------------*/
    public:
      template <typename T>
      void
      serialize(std::string const& name, T& v);
      template <typename T>
      void
      serialize(std::string const& name, boost::optional<T>& opt);
      template <typename T>
      void
      serialize(std::string const& name, std::unique_ptr<T>& opt);
      template <typename T, typename A>
      void
      serialize(std::string const& name, T& v, as<A>);
      template <typename T>
      void
      serialize_object(std::string const& name, T& v);
      template <typename T>
      void
      serialize_pod(std::string const& name, T& v);

    protected:
      virtual
      void
      _enter(std::string const& name);
      virtual
      void
      _leave(std::string const& name);
      virtual
      void
      _serialize_array(std::string const& name,
                       std::function<void ()> const& f) = 0;
      virtual
      void
      _serialize(std::string const& name, int64_t& v) = 0;
      virtual
      void
      _serialize(std::string const& name, int32_t& v) = 0;
      virtual
      void
      _serialize(std::string const& name, double& v) = 0;
      virtual
      void
      _serialize(std::string const& name, bool& v) = 0;
      virtual
      void
      _serialize(std::string const& name, std::string& v) = 0;
      virtual
      void
      _serialize_option(std::string const& name,
                        bool present,
                        std::function<void ()> const& f) = 0;
      template <template <typename, typename> class C, typename T, typename A>
      void
      _serialize(std::string const& name, C<T, A>& collection);
      template <typename T1, typename T2>
      void
      _serialize(std::string const& name, std::pair<T1, T2>& collection);
      template <typename K, typename V, typename ... Rest>
      void
      _serialize(std::string const& name,
                std::unordered_map<K, V, Rest...>& map);
    private:
      template <typename T>
      void
      _serialize_anonymous(std::string const& name, T& v);
      bool
      _out() const;
    };

    template <typename T>
    class as
    {};
  }
}

# include <elle/serialization/Serializer.hxx>

#endif
