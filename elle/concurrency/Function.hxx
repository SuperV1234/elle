#ifndef ELLE_CONCURRENCY_FUNCTION_HXX
# define ELLE_CONCURRENCY_FNUCTION_HXX

# include <elle/types.hh>

# include <elle/standalone/Report.hh>
# include <elle/standalone/Log.hh>

# include <elle/io/Dumpable.hh>

namespace elle
{
  namespace concurrency
  {

//
// ---------- constructors & destructors --------------------------------------
//

    ///
    /// the default constructor.
    ///
    template <typename R,
              typename... T>
    Function< R, radix::Parameters<T...> >::Function(Handler           handler):
      handler(handler)
    {
    }

    ///
    /// the copy constructor.
    ///
    template <typename R,
              typename... T>
    Function< R, radix::Parameters<T...> >::Function(
                                       const Function<
                                         R,
                                         radix::Parameters<
                                           T...
                                           >
                                         >&                     function):
      Object(function),

      handler(function.handler)
    {
    }

//
// ---------- methods ---------------------------------------------------------
//

    ///
    /// this method calls the handler.
    ///
    template <typename R,
              typename... T>
    R
    Function< R, radix::Parameters<T...> >::Call(T...                  arguments)
    {
      return (this->handler(arguments...));
    }

    ///
    /// this method triggers the handler without checking the return value.
    ///
    template <typename R,
              typename... T>
    Void
    Function< R, radix::Parameters<T...> >::Trigger(T...               arguments)
    {
      this->handler(arguments...);
    }

//
// ---------- object ----------------------------------------------------------
//

    ///
    /// this macro-function call generates the object.
    ///
    embed(_(Function< R, radix::Parameters<T...> >),
          _(template <typename R,
                      typename... T>));

//
// ---------- dumpable --------------------------------------------------------
//

    ///
    /// this method dumps the function state.
    ///
    template <typename R,
              typename... T>
    Status
    Function< R, radix::Parameters<T...> >::Dump(const Natural32       margin) const
    {
      String            alignment(margin, ' ');

      std::cout << alignment << "[Function]" << std::endl;

      // dump the quantum.
      std::cout << alignment << io::Dumpable::Shift << "[Quantum] "
                << std::dec << sizeof... (T) << std::endl;

      // dump the handler.
      std::cout << alignment << io::Dumpable::Shift << "[Handler] "
                << std::hex << this->handler << std::endl;

      return Status::Ok;
    }

//
// ---------- static methods --------------------------------------------------
//

    ///
    /// this method returns a function of the type of the given handler.
    ///
    template <typename R,
              typename... T>
    Function< R, radix::Parameters<T...> >
    Function<>::Infer(R                                 (*handler)(T...))
    {
      return (Function< R, radix::Parameters<T...> >(handler));
    }

  }
}

#endif
