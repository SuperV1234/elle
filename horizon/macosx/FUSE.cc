//
// ---------- header ----------------------------------------------------------
//
// project       facade
//
// license       infinit
//
// author        julien quintard   [fri jul 31 22:10:21 2009]
//

//
// ---------- includes --------------------------------------------------------
//

#include <facade/macosx/FUSE.hh>
#include <facade/macosx/Crux.hh>
#include <facade/macosx/InterlacedFUker.hh>
#include <facade/macosx/ParallelFUker.hh>
#include <facade/macosx/SequentialFUker.hh>

#include <Infinit.hh>

namespace facade
{
  namespace macosx
  {

//
// ---------- definitions -----------------------------------------------------
//

    ///
    /// the FUSE mountpoint.
    ///
    elle::String                        FUSE::Mountpoint;

    ///
    /// the FUSE-Broker i.e FUker.
    ///
    FUker*                              FUSE::Fuker = NULL;

    ///
    /// this structure contains the function pointers to the FUSE
    /// implementation routines located in the crux.
    ///
    struct ::fuse_operations            FUSE::Operations;

//
// ---------- methods ---------------------------------------------------------
//

    ///
    /// this method initializes the FUSE component.
    ///
    elle::Status        FUSE::Initialize(
                          const struct ::fuse_operations&       operations)
    {
      enter();

      // set the fuse operations.
      FUSE::Operations = operations;

      //
      // allocate the FUker according to the configuration
      //
      {
        // according to the configuration...
        switch (Infinit::Configuration.facade.fuker)
          {
          case FUker::TypeSequential:
            {
              // allocate the sequential FUker.
              FUSE::Fuker = new SequentialFUker;

              break;
            }
          case FUker::TypeInterlaced:
            {
              // allocate the interlaced FUker.
              FUSE::Fuker = new InterlacedFUker;

              break;
            }
          case FUker::TypeParallel:
            {
              // allocate the parallel FUker.
              FUSE::Fuker = new ParallelFUker;

              break;
            }
          default:
            {
              escape("unknown fuker '%u'",
                     Infinit::Configuration.facade.fuker);
            }
          }
      }

      leave();
    }

    ///
    /// this methods sets up the FUSE module.
    ///
    elle::Status        FUSE::Setup(const elle::String&         mountpoint)
    {
      enter();

      // set the mountpoint.
      FUSE::Mountpoint = mountpoint;

      // set up the FUker.
      if (FUSE::Fuker->Setup() == elle::StatusError)
        escape("unable to set up the FUker");

      leave();
    }

    ///
    /// this method cleans the system module.
    ///
    elle::Status        FUSE::Clean()
    {
      enter();

      // delete the FUker.
      delete FUSE::Fuker;

      leave();
    }

  }
}
