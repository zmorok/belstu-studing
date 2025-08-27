using System.Collections.Generic;

namespace CommonSet
{
    internal interface ISet<T>
    {
        void Add(T item);
        void Remove(T item);
        string Contains(T item);
        int Count { get; }
        IEnumerable<T> GetAll();
    }
}
