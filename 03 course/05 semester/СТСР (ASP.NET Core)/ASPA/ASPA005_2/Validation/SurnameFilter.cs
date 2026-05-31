using DAL004;

namespace ASPA005_2.Validation
{
    public class SurnameFilter : IEndpointFilter
    {
        public static IRepository repository = default!;

        public async ValueTask<object?> InvokeAsync(EndpointFilterInvocationContext context, EndpointFilterDelegate next)
        {
            Celebrity? celebrity = context.GetArgument<Celebrity>(0) ?? throw new NullCelebrityException("/Celebrities error, celebrity is null");

            if (celebrity.Surname is null) throw new SurnameInvalidException("Celebrity.Surname is null");
            if (celebrity.Surname.Length < 2) throw new SurnameInvalidException("Celebrity.Surname length < 2");
            if (repository.getAllCelebrities().Any(c => c.Surname.Equals(celebrity.Surname, StringComparison.OrdinalIgnoreCase)))
                throw new SurnameConflictException($"Celebrity with surname '{celebrity.Surname}' already exists", celebrity.Surname);

            return await next(context);
        }
    }
}
