using System;
using System.Linq;

namespace CommonSet
{
    public partial class CommonSet<T> : ISet<T> where T : IComparable<T>
    {
        #region overriding operations

        public static CommonSet<T> operator <<(CommonSet<T> set, T item)
        {
            set.Add(item);
            return set;
        }

        public static CommonSet<T> operator >>(CommonSet<T> set, T item)
        {
            set.Remove(item);
            return set;
        }

        public static bool operator >(CommonSet<T> set1, CommonSet<T> set2) =>
            set2.Elements.IsSubsetOf(set1.Elements);

        public static bool operator <(CommonSet<T> set1, CommonSet<T> set2) =>
            !set2.Elements.IsSubsetOf(set1.Elements);

        public static bool operator ==(CommonSet<T> set1, CommonSet<T> set2) =>
            set1.Elements.SetEquals(set2.Elements);

        public static bool operator !=(CommonSet<T> set1, CommonSet<T> set2) =>
            !set1.Elements.SetEquals(set2.Elements);

        public static CommonSet<T> operator %(CommonSet<T> set1, CommonSet<T> set2) =>
            new CommonSet<T>(set1.Elements.Intersect(set2.Elements));

        public static class StatisticOperation
        {
            public static int Count(CommonSet<T> set) => set.Count;
        }

        #endregion
    }
}