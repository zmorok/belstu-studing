using Microsoft.Extensions.DependencyInjection;

namespace ServiceLocator.Service
{
    public class ServiceScopeFactory : IServiceScopeFactory
    {
        private readonly Dictionary<Type, Func<object>> services;
        public ServiceScopeFactory(Dictionary<Type, Func<object>> services)
        {
            this.services = services;
        }

        public IServiceScope CreateScope()
        {
            return new ServiceScope(this.services);
        }
    } 
}
