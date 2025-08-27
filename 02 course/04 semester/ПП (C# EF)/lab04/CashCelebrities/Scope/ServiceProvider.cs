namespace CashCelebrities.Scope
{
    public class ServiceProvider : IServiceProvider
    {
        private readonly Dictionary<Type, Func<TimeSpan, object>> services;
        private readonly Dictionary<Type, object> scopeservices;
        private readonly TimeSpan cacheDuration;

        public ServiceProvider(Dictionary<Type, Func<TimeSpan, object>> services, Dictionary<Type, object> scopeservices, TimeSpan cacheDuration)
        {
            this.services = services;
            this.scopeservices = scopeservices;
            this.cacheDuration = cacheDuration;
        }

        public object? GetService<T>() where T : class
        {
            object? obj = GetService(typeof(T));
            return obj == null ? null : (T)obj;
        }

        public object? GetService(Type serviceType)
        {
            object? obj = scopeservices.GetValueOrDefault(serviceType);
            if (obj is null)
            {
                obj = scopeservices[serviceType] = services[serviceType](cacheDuration);
            }
            return obj;
        }
    }
}
