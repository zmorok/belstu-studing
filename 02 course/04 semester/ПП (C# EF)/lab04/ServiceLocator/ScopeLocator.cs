using Microsoft.Extensions.DependencyInjection;
using ServiceLocator.Service;

namespace ServiceLocator
{
    public static class ScopeLocator
    {
        public static IServiceScopeFactory CreateServiceScopeFactory()
        {
            return new ServiceScopeFactory(services);
        }

        private static readonly Dictionary<Type, Func<object>> services = new Dictionary<Type, Func<object>>();
        
        public static void Register<T>(Func<T> resolver) where T : class
        {
            services[typeof(T)] = () => resolver();
        }
        
        public static T Resolve<T>()
        {
            return (T)services[typeof(T)]();
        }
        
        public static void Reset()
        {
            services.Clear();
        }
    }
}
