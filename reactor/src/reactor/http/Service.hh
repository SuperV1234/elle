#ifndef REACTOR_HTTP_SERVICE_HH
# define REACTOR_HTTP_SERVICE_HH

# include <unordered_map>

# include <boost/asio.hpp>

# include <elle/Printable.hh>

# include <reactor/http/Request.hh>

namespace reactor
{
  namespace http
  {
    class Service:
      public boost::asio::io_service::service,
      public elle::Printable
    {
    public:
      Service(boost::asio::io_service& service);
      ~Service();
      static boost::asio::io_service::id id;
      virtual
      void
      shutdown_service();
    private:
      friend class Client;
      CURLM* _curl;

    /*---------.
    | Requests |
    `---------*/
    public:
      void add(Request& request);
      void remove(Request& request);
    private:
      std::unordered_map<void*, Request*> _requests;

    /*-------.
    | Socket |
    `-------*/
    private:
      static
      int
      socket_callback(CURL *easy,
                      curl_socket_t,
                      int action,
                      void* userp,
                      void*);
      void
      handle_socket_action(CURL* handle,
                           curl_socket_t socket,
                           int action);
      void
      handle_socket_ready(Request& r,
                          curl_socket_t socket,
                          int action,
                          boost::system::error_code const& error,
                          size_t size);
      void
      register_socket_write(Request& r,
                            curl_socket_t socket);
      void
      register_socket_read(Request& r,
                           curl_socket_t socket);

    /*--------.
    | Timeout |
    `--------*/
    private:
      static
      int
      timeout_callback(CURLM* multi,
                       long ms,
                       void* userp);
      void
      set_timeout(boost::posix_time::time_duration const& timeout);
      void
      unset_timeout();
      void
      handle_timeout(const boost::system::error_code& error);
      boost::asio::deadline_timer _timer;

    /*-----.
    | Info |
    `-----*/
    public:
      void
      _pull_messages();

    /*----------.
    | Printable |
    `----------*/
    public:
      virtual
      void
      print(std::ostream& stream) const;
    };
  }
}

#endif