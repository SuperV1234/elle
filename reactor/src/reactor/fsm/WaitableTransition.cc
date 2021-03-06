#include <elle/log.hh>
#include <elle/memory.hh>

#include <reactor/fsm/State.hh>
#include <reactor/fsm/WaitableTransition.hh>
#include <reactor/scheduler.hh>
#include <reactor/signal.hh>

ELLE_LOG_COMPONENT("reactor.fsm.Transition")

namespace reactor
{
  namespace fsm
  {
    boost::optional<std::function<void()>>
    WaitableTransition::run(reactor::Signal& triggered,
                            Transition*& trigger,
                            Thread& action_thread)
    {
      ELLE_ASSERT(Scheduler::scheduler());
      auto& sched = *Scheduler::scheduler();

      if (this->_pre_trigger && this->_pre_trigger())
      {
        ELLE_DEBUG("%s: pre-triggered", *this);
        if (!trigger)
        {
          ELLE_TRACE("%s: trigger", *this);
          trigger = this;
          // FIXME: thread isn't started yet.
          if (this->preemptive())
            action_thread.terminate();
          triggered.signal();
        }
        return boost::none;
      }

      ELLE_DEBUG("Initializing %s", *this);
      return std::function<void ()>(
        [this,
         &sched,
         &trigger,
         &action_thread,
         &triggered] ()
        {
          sched.current()->wait(this->trigger());
          if (!trigger)
          {
            ELLE_TRACE("%s: trigger", *this);
            trigger = this;
            if (this->preemptive())
              action_thread.terminate();
            triggered.signal();
          }
        });
    };

    WaitableTransition::WaitableTransition(State& start,
                                           State& end,
                                           Waitables const& trigger,
                                           bool preemptive,
                                           PreTrigger const& pre_trigger):
      Transition(start, end),
      _trigger(trigger),
      _preemptive(preemptive),
      _pre_trigger(pre_trigger)
    {}

    /*----------.
    | Printable |
    `----------*/

    void
    WaitableTransition::print(std::ostream& stream) const
    {
      stream << "Transition(" << this->start().name() << " , "
             << this->end().name() << " , " << this->trigger() << ')';
    }
  }
}
