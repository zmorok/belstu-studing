using Microsoft.Extensions.DependencyInjection;

namespace CashCelebrities.Scope
{
    public class ServiceScopeFactory : IServiceScopeFactory
    {
        private readonly Dictionary<Type, Func<TimeSpan, object>> services;
        private readonly TimeSpan cacheDuration;

        public ServiceScopeFactory(Dictionary<Type, Func<TimeSpan, object>> services, TimeSpan cacheDuration)
        {
            this.services = services;
            this.cacheDuration = cacheDuration;
        }

        public IServiceScope CreateScope()
        {
            return new ServiceScope(services, cacheDuration);
        }
    }
}
