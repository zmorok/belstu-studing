using System;

namespace Lab6Lib
{
    public class Publisher
    {
        private event Action<string> update;
        private readonly string _eventname;

        public Publisher(string eventname)
        {
            _eventname = eventname;
        }

        public void Subscribe(ISubscriber subscriber)
        {
            update += subscriber.Update;
        }

        public bool Unsubscribe(ISubscriber subscriber)
        {
            update -= subscriber.Update;
            return true;
        }

        public int Notify()
        {
            update?.Invoke(_eventname);
            return update?.GetInvocationList()?.Length ?? 0;
        }
    }
}
