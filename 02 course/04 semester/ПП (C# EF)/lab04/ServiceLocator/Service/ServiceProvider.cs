namespace ServiceLocator.Service
{
    internal class ServiceProvider : IServiceProvider
    {
        private readonly Dictionary<Type, Func<object>> services;
        private readonly Dictionary<Type, object> scopeservices;
        public ServiceProvider(Dictionary<Type, Func<object>> services, Dictionary<Type, object> scopeservices) // второй параметер scopeservices не нужен
        {
            this.services = services;
            this.scopeservices = new Dictionary<Type, object>();
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
                obj = this.scopeservices[serviceType] = services[serviceType]();
            }
            return obj;
        }
    }
}
