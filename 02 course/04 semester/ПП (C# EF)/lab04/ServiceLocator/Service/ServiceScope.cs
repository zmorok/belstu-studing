using Microsoft.Extensions.DependencyInjection;

namespace ServiceLocator.Service
{
    public class ServiceScope : IServiceScope
    {
        private Dictionary<Type, Func<object>> services;
        private Dictionary<Type, object> scopeservices;
        public IServiceProvider ServiceProvider { get; private set; }
        public ServiceScope(Dictionary<Type, Func<object>> services)
        {
            this.services = services;
            this.scopeservices = new Dictionary<Type, object>();
            this.ServiceProvider = new ServiceProvider(services, scopeservices);
        }
        public void Dispose() { scopeservices.Clear(); }
    }
}
