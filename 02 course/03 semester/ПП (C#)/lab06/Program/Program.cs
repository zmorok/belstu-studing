using System;
using Lab6Lib;

namespace lab06
{
    internal class Program
    {
        static void Main(string[] args)
        {
            Publisher event1 = new Publisher("EVENT-1");
            Publisher event2 = new Publisher("EVENT-2");
            Publisher event3 = new Publisher("EVENT-3");

            SubscriberA subscriberA = new SubscriberA();
            SubscriberB subscriberB = new SubscriberB();
            SubscriberC subscriberC = new SubscriberC();

            Console.WriteLine("--------------------Тест 1 ------------------");
            event1.Subscribe(subscriberA);
            event2.Subscribe(subscriberA);
            event3.Subscribe(subscriberA);
            event1.Notify(); event2.Notify(); event3.Notify();

            Console.WriteLine("--------------------Тест 2 ------------------");
            event1.Subscribe(subscriberB);
            event2.Unsubscribe(subscriberA);
            event1.Notify(); event2.Notify(); event3.Notify();

            Console.WriteLine("--------------------Тест 3 ------------------");
            event2.Subscribe(subscriberB);
            event3.Subscribe(subscriberC);
            event1.Notify(); event2.Notify(); event3.Notify();

            Console.WriteLine("--------------------Тест 4 ------------------");
            event2.Subscribe(subscriberB);
            event3.Subscribe(subscriberC);
            event1.Notify(); event2.Notify(); event3.Notify();

            Console.WriteLine("--------------------Тест 5 ------------------");
            event1.Unsubscribe(subscriberA);
            event1.Unsubscribe(subscriberB);
            event2.Unsubscribe(subscriberB);
            event3.Unsubscribe(subscriberA);
            event3.Unsubscribe(subscriberC);
            event1.Notify(); event2.Notify(); event3.Notify();
        }


        public class SubscriberA : ISubscriber
        {
            public void Update(string eventname)
            {
                Console.WriteLine(string.Format("Subscriber:{0}, Event:{1}", "A", eventname));
            }
        }


        public class SubscriberB : ISubscriber
        {
            public void Update(string eventname)
            {
                Console.WriteLine(string.Format("Subscriber:{0}, Event:{1}", "B", eventname));
            }
        }


        public class SubscriberC : ISubscriber
        {
            public void Update(string eventname)
            {
                Console.WriteLine(string.Format("Subscriber:{0}, Event:{1}", "C", eventname));
            }
        }
    }
}