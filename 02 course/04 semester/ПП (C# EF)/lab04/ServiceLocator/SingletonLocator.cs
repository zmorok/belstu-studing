namespace ServiceLocator
{
    public static class SingletonLocator
    {
        private static readonly Dictionary<Type, object> services = new Dictionary<Type, object>();

        public static void Register<T>(T instance) where T : class
        {
            services[typeof(T)] = instance;
        }

        public static T Resolve<T>()
        {
            return (T)services[typeof(T)];
        }
        
        public static void Reset()
        {
            services.Clear();
        }
    }
}
