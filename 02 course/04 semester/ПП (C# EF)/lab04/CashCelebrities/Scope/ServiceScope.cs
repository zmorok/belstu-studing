using Microsoft.Extensions.DependencyInjection;

namespace CashCelebrities.Scope
{
    public class ServiceScope : IServiceScope
    {
        private readonly Dictionary<Type, Func<TimeSpan, object>> services;
        private readonly TimeSpan cacheDuration;
        private readonly Dictionary<Type, object> scopeservices = new Dictionary<Type, object>();

        public IServiceProvider ServiceProvider { get; }

        public ServiceScope(Dictionary<Type, Func<TimeSpan, object>> services, TimeSpan cacheDuration)
        {
            this.services = services;
            this.cacheDuration = cacheDuration;
            ServiceProvider = new ServiceProvider(services, scopeservices, cacheDuration);
        }

        public void Dispose() { scopeservices.Clear(); }
    }
}
