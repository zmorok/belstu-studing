using StudentLib;
using System;
using System.Collections.Specialized;

namespace ObservableCollection
{
    internal partial class Program
    {
        public static void OnCollectionChanged(object sender, NotifyCollectionChangedEventArgs e)
        {
            switch (e.Action)
            {
                case NotifyCollectionChangedAction.Add:
                    {
                        Console.WriteLine("добавлен студент:");
                        foreach (Student newItem in e.NewItems) Console.WriteLine("\t" + newItem);
                        break;
                    }

                case NotifyCollectionChangedAction.Remove:
                    {
                        Console.WriteLine("удалён элемент:");
                        foreach (Student oldItem in e.OldItems) Console.WriteLine("\t" + oldItem);
                        break;
                    }

                case NotifyCollectionChangedAction.Replace:
                    {
                        Console.WriteLine("студент был заменён:");
                        foreach (Student oldItem in e.OldItems) Console.WriteLine($"\tстарый: {oldItem}");
                        foreach (Student newItem in e.NewItems) Console.WriteLine($"\tновый: {newItem}");
                        break;
                    }

                case NotifyCollectionChangedAction.Reset:
                    {
                        Console.WriteLine("студенты очищены.");
                        break;
                    }
            }
        }
    }
}
