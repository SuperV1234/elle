#ifndef ELLE_CONCURRENCY_FUNCTION_HH
# define ELLE_CONCURRENCY_FUNCTION_HH

# include <elle/types.hh>

# include <elle/radix/Object.hh>
# include <elle/radix/Parameters.hh>

# include <elle/idiom/Open.hh>

namespace elle
{
  namespace concurrency
  {

    ///
    /// Function generic class.
    ///
    template <typename... T>
    class Function;

    ///
    /// this class represents a function.
    ///
    template <typename R,
              typename... T>
    class Function< R,
                    radix::Parameters<T...> >:
      public radix::Object
    {
    public:
      //
      // types
      //
      typedef radix::Parameters<T...>          P;

      //
      // types
      //
      typedef R                         (*Handler)(T...);

      //
      // constructors & destructors
      //
      Function(Handler);
      Function(const Function<R, P>&);

      //
      // methods
      //
      R                 Call(T...);
      Void              Trigger(T...);

      //
      // interfaces
      //

      // object
      declare(_(Function<R, P>));

      // dumpable
      Status            Dump(const Natural32 = 0) const;

      //
      // attributes
      //
      Handler           handler;
    };

    ///
    /// a specific class for function inference.
    ///
    template <>
    class Function<>
    {
    public:
      //
      // static methods
      //
      template <typename R,
                typename... T>
      static Function< R, radix::Parameters<T...> >    Infer(R (*)(T...));
    };

  }
}

#include <elle/concurrency/Function.hxx>

#endif
